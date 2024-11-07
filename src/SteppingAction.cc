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
/// \file SteppingAction.cc
/// \brief Implementation of the SteppingAction class

#include "SteppingAction.hh"
#include "EventAction.hh"
#include "DetectorConstruction.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"
#include "G4Event.hh"
#include "Analysis.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::SteppingAction(
    const DetectorConstruction* detectorConstruction,
    EventAction* eventAction)
  : G4UserSteppingAction(),
  fDetConstruction(detectorConstruction),
  fEventAction(eventAction)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::~SteppingAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SteppingAction::UserSteppingAction(const G4Step* step)
{
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

 //Step information
  //G4int EventID  = G4RunManager::GetRunManager()->GetCurrentEvent() -> GetEventID();
  G4int TrackID  = step->GetTrack()->GetTrackID();
  G4int ParentID = step->GetTrack()->GetParentID();
  G4int ParticlePDG = step->GetTrack()->GetDefinition()-> GetPDGEncoding();
  G4int StepNumber  = step->GetTrack()->GetCurrentStepNumber();

  G4String ParticleName = step->GetTrack()->GetParticleDefinition()->GetParticleName();
  //G4cout<<"EventID = "<<EventID<<", TrackID = "<<TrackID<<", ParentID = "<<ParentID<<", Name = "<<ParticleName<<", ParticlePDG = "<<ParticlePDG<<G4endl;


  G4bool IsEnv = (step->GetPreStepPoint()->GetPhysicalVolume()->GetName()) == ("WorldEnv");
  if(IsEnv) step->GetTrack()->SetTrackStatus(fStopAndKill);
  //if(VolumeName == "FieldEnv") step->GetTrack()->SetTrackStatus(fStopAndKill);
  //if((ParticleName != "e+") && (StepNumber>3)) step->GetTrack()->SetTrackStatus(fStopAndKill);

////  if(StepNumber >= 10000) step->GetTrack()->SetTrackStatus(fStopAndKill);

  //PDG info
  //Na22 = 1000110220
  //e+ = -11
  //nu_e = 12
  //Ne22[1274.577] = 1000100221
  //Ne22 = 1000100220
  //gamma = 22

  //Physical quantity initialize
  G4double Position_x	    = -50000000.000;
  G4double Position_y		= -50000000.000;
  G4double Position_z		= -50000000.000;
  G4double Momentum_x		= -50000000.000;
  G4double Momentum_y		= -50000000.000;
  G4double Momentum_z		= -50000000.000;
  G4double KineticEnergy        = -60000000.000;
  G4double LocalTime		= -50000000.000;
  G4double GlobalTime		= -50000000.000;
  
  G4String VolumeName		= "Initialized"; 
  G4int	   VolumeNumber		= -15;
  
  G4String CreatorProcess	= "Initialized";
  G4int	   CreatorNumber	= -15;

  G4String Process			= "Initialized";
  G4int	   ProcessNumber	= -15;

  G4bool   IsBoundary		= false;
  G4int	   BoundaryNumber	= -6;

  if(TrackID == 1) CreatorProcess = "Primary"; //Primary Particle
  else CreatorProcess = step->GetTrack()->GetCreatorProcess()->GetProcessName();
  CreatorNumber = ProcessDistinguisher(CreatorProcess);

  if(StepNumber == 1){
	 Position_x	    = step->GetPreStepPoint()->GetPosition().x();
	 Position_y		= step->GetPreStepPoint()->GetPosition().y();
	 Position_z		= step->GetPreStepPoint()->GetPosition().z();
	 Momentum_x		= step->GetPreStepPoint()->GetMomentum().x();
	 Momentum_y		= step->GetPreStepPoint()->GetMomentum().y();
	 Momentum_z		= step->GetPreStepPoint()->GetMomentum().z();
         KineticEnergy          = step->GetPreStepPoint()->GetKineticEnergy();
	 LocalTime		= step->GetPreStepPoint()->GetLocalTime();
	 GlobalTime		= step->GetPreStepPoint()->GetGlobalTime();
	 VolumeName		= step->GetPreStepPoint()->GetPhysicalVolume()->GetName();
	 VolumeNumber	= VolumeDistinguisher(VolumeName);
	 Process		= "Primary";
	 ProcessNumber  = ProcessDistinguisher(Process);
	 IsBoundary		= ( ( step->GetPreStepPoint()->GetStepStatus() ) == (fGeomBoundary) );
	 if(IsBoundary)		 BoundaryNumber = 1;
	 else				 BoundaryNumber = 0;

	 fEventAction -> StepInfo( 
						TrackID, 0, ParentID, ParticlePDG,
						Position_x/mm, Position_y/mm, Position_z/mm,
						Momentum_x/keV, Momentum_y/keV, Momentum_z/keV, KineticEnergy/keV,
						LocalTime/ns, (GlobalTime/second)/(365.*24.*60.*60.),
						VolumeNumber, CreatorNumber, ProcessNumber,
						BoundaryNumber
						);
  }

  Position_x	    = step->GetPostStepPoint()->GetPosition().x();
  Position_y		= step->GetPostStepPoint()->GetPosition().y();
  Position_z		= step->GetPostStepPoint()->GetPosition().z();
  Momentum_x		= step->GetPostStepPoint()->GetMomentum().x();
  Momentum_y		= step->GetPostStepPoint()->GetMomentum().y();
  Momentum_z		= step->GetPostStepPoint()->GetMomentum().z();
  KineticEnergy         = step->GetPostStepPoint()->GetKineticEnergy();
  LocalTime		= step->GetPostStepPoint()->GetLocalTime();
  GlobalTime		= step->GetPostStepPoint()->GetGlobalTime();
  VolumeName		= step->GetPostStepPoint()->GetPhysicalVolume()->GetName();
  VolumeNumber		= VolumeDistinguisher(VolumeName);
  Process			= step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();
  ProcessNumber = ProcessDistinguisher(Process);
  IsBoundary		= ( ( step->GetPostStepPoint()->GetStepStatus() ) == (fGeomBoundary) );	
  if(IsBoundary)	 BoundaryNumber = 1;
  else				 BoundaryNumber = 0;

  G4cout<<"Global time: "<<(GlobalTime/second)/(365.*24.*60.*60.)<<G4endl;

  fEventAction -> StepInfo( 
                             TrackID, StepNumber, ParentID, ParticlePDG,
			     Position_x/mm, Position_y/mm, Position_z/mm,
			     Momentum_x/keV, Momentum_y/keV, Momentum_z/keV, KineticEnergy/keV,
			     LocalTime/ns, (GlobalTime/second)/(365.*24.*60.*60.),
			     VolumeNumber, CreatorNumber, ProcessNumber,
			     BoundaryNumber
							);

/*
	  analysisManager->FillNtupleIColumn(0,EventID);
	  analysisManager->FillNtupleIColumn(1,TrackID);
	  analysisManager->FillNtupleIColumn(2,StepNumber);
	  analysisManager->FillNtupleIColumn(3,ParentID);             
	  analysisManager->FillNtupleIColumn(4,ParticlePDG);

	  analysisManager->FillNtupleDColumn(5,  Position_x/mm);
	  analysisManager->FillNtupleDColumn(6,  Position_y/mm);
	  analysisManager->FillNtupleDColumn(7,  Position_z/mm);
	  analysisManager->FillNtupleDColumn(8,  Momentum_x/keV);             
	  analysisManager->FillNtupleDColumn(9,  Momentum_y/keV);
	  analysisManager->FillNtupleDColumn(10, Momentum_z/keV);
	  analysisManager->FillNtupleDColumn(11,  VertexM_x/keV);             
	  analysisManager->FillNtupleDColumn(12,  VertexM_y/keV);             
	  analysisManager->FillNtupleDColumn(13,  VertexM_z/keV);             
	  analysisManager->FillNtupleDColumn(14,  LocalTime/ns);           
	  analysisManager->FillNtupleDColumn(15,  GlobalTime/ns);

	  analysisManager->FillNtupleSColumn(16,  PostVolumeName);

	  analysisManager->AddNtupleRow();
*/

  /*
  //For cehcking validation
  G4String VolumeName = "DefinedName";
  //if(StepNumber == 0) VolumeName = "FirstStep";
  //else VolumeName = step->GetPreStepPoint()->GetPhysicalVolume()->GetName();
  
  VolumeName = step->GetPreStepPoint()->GetPhysicalVolume()->GetName();
  //if( (VolumeName == "WorldEnv") || (VolumeName == "World") ) 
  if(VolumeName == "WorldEnv") step->GetTrack()->SetTrackStatus(fStopAndKill);
  
  //G4int VolumeCopyNo = 0;
  G4int VolumeCopyNo = step->GetPreStepPoint()->GetPhysicalVolume()->GetCopyNo();
  G4String ProcessName = "IniStep";
  if(StepNumber > 1) ProcessName = step->GetPreStepPoint()->GetProcessDefinedStep()->GetProcessName();
  //G4cout<<"ProcessName = "<<ProcessName<<G4endl;

  G4String CreatorProcessName = "NoCreator";
  if(ParticleName != "e+") 
	  CreatorProcessName = step->GetTrack()->GetCreatorProcess()->GetProcessName();

  G4double VertexPosition_x = -1000000.0;
  G4double VertexPosition_y = -1000000.0;
  G4double VertexPosition_z = -1000000.0;

  if(ParticleName != "e+"){
	  VertexPosition_x = step->GetTrack()->GetVertexPosition().x();
	  VertexPosition_y = step->GetTrack()->GetVertexPosition().y();
	  VertexPosition_z = step->GetTrack()->GetVertexPosition().z();
  }

  
  analysisManager->FillH3(0, VertexPosition_x, VertexPosition_y, VertexPosition_z);
  analysisManager->FillH2(1, VertexPosition_x, VertexPosition_y);
  analysisManager->FillH2(2, VertexPosition_x, VertexPosition_z);
  analysisManager->FillH2(3, VertexPosition_y, VertexPosition_z);


  analysisManager->FillNtupleIColumn(0,EventID);
  analysisManager->FillNtupleIColumn(1,TrackID);
  analysisManager->FillNtupleIColumn(2,StepNumber);
  analysisManager->FillNtupleSColumn(3,ParticleName);             

  analysisManager->FillNtupleDColumn(4,Position_x/mm);
  analysisManager->FillNtupleDColumn(5,Position_y/mm);
  analysisManager->FillNtupleDColumn(6,Position_z/mm);
  analysisManager->FillNtupleDColumn(7,Momentum_x/keV);             
  analysisManager->FillNtupleDColumn(8,Momentum_y/keV);
  analysisManager->FillNtupleDColumn(9,Momentum_z/keV);
  analysisManager->FillNtupleDColumn(10,LocalTime/ns);           
  analysisManager->FillNtupleDColumn(11,GlobalTime/ns);

  analysisManager->FillNtupleSColumn(12,VolumeName);             
  analysisManager->FillNtupleIColumn(13,VolumeCopyNo);             
  analysisManager->FillNtupleSColumn(14,ProcessName);             
  analysisManager->FillNtupleSColumn(15,CreatorProcessName);             

  analysisManager->FillNtupleDColumn(16,VertexPosition_x/mm);
  analysisManager->FillNtupleDColumn(17,VertexPosition_y/mm);
  analysisManager->FillNtupleDColumn(18,VertexPosition_z/mm);
  
  analysisManager->AddNtupleRow();
*/
}

G4int SteppingAction::VolumeDistinguisher(G4String NameOfVolume_){

    static map<G4String, G4int> volMap = {
        {"World", 0},
        {"WorldEnv", 1},
        
        {"Trigger_Box_Shell", 2},
        {"Trigger_Box", 3},
        {"CylinderShell1_InBoxShell", 4},
        {"Cylinder1_InBoxShell", 5},
        {"Cylinder1_Shell", 6},
        {"Cylinder1", 7},
        {"CylinderShell2_InBoxShell", 8},
        {"Cylinder2_InBoxShell", 9},
        {"Cylinder2_Shell", 10},
        {"Cylinder2", 11},
        
        {"ECAL1_Shell", 12},
        {"ECAL1", 13},
        {"ECAL2_Shell", 14},
        {"ECAL2", 15},

        {"Shielding_Mirror", 16},
        {"Shielding1", 17},
        {"Shielding2", 18},
        {"Solenoid1", 19},
        {"Solenoid2", 20},
        {"ShieldingECAL1", 21},
        {"ShieldingECAL2", 22},

        {"sq1", 23},
        {"AirCap1", 24},
        {"Insq1", 25},
        {"sq2", 26},
        {"AirCap2", 27},
        {"Insq2", 28},
        {"SiPM1", 29},
        {"SiPM2", 30},
        {"SiPMFront1", 31},
        {"SiPMFront2", 32},
        
        {"Trigger_Veto_Box", 33},
        {"VetoAirCap", 34},
        {"InTrigger_Veto_Box", 35},
        {"Trigger_Veto_SiPMFront1", 36},
        {"Trigger_Veto_SiPMFront2", 37},
        {"Trigger_Veto_SiPM1", 38},
        {"Trigger_Veto_SiPM2", 39},

        {"TargetEnv3", 40},
        {"TargetEnv2", 41},
        {"TargetEnv1", 42},
        {"TargetLeft", 43},
        {"TargetRight", 44},

        {"Tracker_Left", 45},
        {"Tracker_Right", 46}
    };
 
    map<G4String, G4int>::const_iterator iter = volMap.find(NameOfVolume_);
    if(iter != volMap.end())
        return iter->second;
    else
        return -100;

}       

G4int SteppingAction::ProcessDistinguisher(G4String NameOfProcess_){
	G4String NoP = NameOfProcess_;
	//G4cout<<"Process Name = "<<NoP<<G4endl;
	G4int Number = -35;
	
	if(NoP == "Primary") Number = 0;
	if(NoP == "Transportation") Number = 1;
	if(NoP == "msc") Number = 2;
	if(NoP == "eIoni") Number = 3;
	if(NoP == "annihil") Number = 4;
	if(NoP == "eBrem") Number = 5;
	if(NoP == "RadioactiveDecayBase") Number = 6;
	if(NoP == "ionIoni") Number = 7;
	if(NoP == "phot") Number = 8;
	if(NoP == "compt") Number = 9;
	if(NoP == "StepLimiter") Number = 10;

	return Number;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.......cker_Left Transportation
      //.oooOO0OOooo......
