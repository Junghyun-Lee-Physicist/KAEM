//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// $Id$
// 
/// \file PrimaryGeneratorAction.cc
/// \brief Implementation of the PrimaryGeneratorAction class

#include "PrimaryGeneratorAction.hh"

#include "G4RunManager.hh"
#include "G4IonTable.hh"
#include "G4Geantino.hh"


#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

enum GenMode{
    e4PiPositron,
    eNa22Decay,
    eFixedPositron
};

PrimaryGeneratorAction::PrimaryGeneratorAction()
 : G4VUserPrimaryGeneratorAction(),
   fParticleGun(0)
{
  G4int nofParticles = 1;
  fParticleGun = new G4ParticleGun(nofParticles);

  // default particle kinematic
  //fParticleGun->SetParticleEnergy(100*keV);
  fParticleGun->SetParticlePosition(G4ThreeVector(0.,0.,0.));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete fParticleGun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
    GenMode mode = eNa22Decay;
    switch(mode){
        case e4PiPositron:
            Generate4PiPositron(anEvent);
            break;
        case eNa22Decay:
            GenerateNa22Decay(anEvent);
            break;
        case eFixedPositron:
            GenerateFixedPositron(anEvent);
    }

}//End of [GeneratePrimaries] function

void PrimaryGeneratorAction::Generate4PiPositron(G4Event* anEvent)
{
    G4double theta = 2.0 * M_PI * G4UniformRand();
    G4double phi = acos(1.0 - 2.0 * G4UniformRand());

    G4double x = sin(phi) * cos(theta);
    G4double y = sin(phi) * sin(theta);
    G4double z = cos(phi);
 
    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(x,y,z));
 
    G4ParticleDefinition* particleDefinition = G4ParticleTable::GetParticleTable()->FindParticle("e+");
    fParticleGun->SetParticleDefinition(particleDefinition);
 
    G4double pE = G4UniformRand()*216.5*keV;
    fParticleGun->SetParticleEnergy(pE);
    fParticleGun->GeneratePrimaryVertex(anEvent);
}//End of the [Generate4PiPositron] function

void PrimaryGeneratorAction::GenerateNa22Decay(G4Event* anEvent)
{
    // Set the initial position of the Na22
    G4ThreeVector Na22_Position(0., 0., 0.);
    fParticleGun->SetParticlePosition(Na22_Position);

 
    // Set the initial direction of the decay to a random direction within a sphere
    G4double costheta = 2.0*G4UniformRand() - 1.0;
    G4double sintheta = std::sqrt(1.0 - costheta*costheta);
    G4double phi = 2.0*M_PI*G4UniformRand();
    G4double sinphi = std::sin(phi);
    G4double cosphi = std::cos(phi);
    G4ThreeVector decay_direction(sintheta*cosphi,sintheta*sinphi,costheta);
    //fParticleGun->SetParticleMomentumDirection(decay_directiona);
    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0., 0., 0.));

    // Set the initial energy of the decay to the decay energy of Na-22 (3.47 MeV)
    G4double decay_energy = 3.47*MeV;
    //fParticleGun->SetParticleEnergy(decay_energy);
    fParticleGun->SetParticleEnergy(0.);

    // Set the ion charge to +1
    G4double ion_charge = 1.0*eplus;
    fParticleGun->SetParticleCharge(ion_charge);

    // Set the excitation energy to 0
    G4double excitation_energy = 0*keV;

    // Set the particle definition as Na-22
    G4int Z = 11, A = 22;
    G4ParticleDefinition* Na22 = G4IonTable::GetIonTable()->GetIon(Z,A,excitation_energy);
    fParticleGun->SetParticleDefinition(Na22);

    // Set the lifetime of the Na-22 ion (2.6 years)
    const G4double year = 365.24*24.0*60.0*60.0*second; //number of seconds per year
    G4double lifetime = 2.6*year;
    Na22->SetPDGLifeTime(lifetime);

    // Generate the primary vertex for the event
    fParticleGun->GeneratePrimaryVertex(anEvent);
}//End of the [GenerateNa22Decay] function

void PrimaryGeneratorAction::GenerateFixedPositron(G4Event* anEvent)
{
    // Set the initial position of the positron
    G4ThreeVector eplusPosition(0., 0., 0.);
    fParticleGun->SetParticlePosition(eplusPosition);
 
    // Set the initial direction of the positron
    G4double phi = 2.0*M_PI*G4UniformRand();
    G4double theta_degreeUnit = 90.0;
    G4double theta = theta_degreeUnit*(M_PI/180.0);
    
    G4double x = sin(theta)*cos(phi);
    G4double y = sin(theta)*sin(phi);
    G4double z = cos(phi);
    G4ThreeVector eplusDirection(x, y, z);
    //G4ThreeVector eplusDirection(-92.5512, 177.864, 219.549);
    fParticleGun->SetParticleMomentumDirection(eplusDirection);

    // Set the initial energy of the positron
    //G4double eplusEnergy = 500.0*keV;
    G4double eplusEnergy = G4UniformRand()*100.00*keV;
    fParticleGun->SetParticleEnergy(eplusEnergy);

    // Set the initial particle type as positron
    G4ParticleDefinition* particleDefinition 
      = G4ParticleTable::GetParticleTable()->FindParticle("e+");
    fParticleGun->SetParticleDefinition(particleDefinition);

    // Generate the primary vertex for the event
    fParticleGun->GeneratePrimaryVertex(anEvent);

}//End of the [GenerateFixedPositron] function


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

