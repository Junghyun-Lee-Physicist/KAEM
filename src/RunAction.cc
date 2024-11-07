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
/// \file RunAction.cc
/// \brief Implementation of the RunAction class

#include "RunAction.hh"
#include "Analysis.hh"
#include "EventAction.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::RunAction(EventAction* eventAction)
	: G4UserRunAction(),
	fEventAction(eventAction)
{ 
  G4RunManager::GetRunManager()->SetPrintProgress(10000);   
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  G4cout << "Using " << analysisManager->GetType() << G4endl;

  analysisManager->SetVerboseLevel(1);
  analysisManager->SetNtupleMerging(true);

  /*
  analysisManager->CreateH1("histo","", 3, 0, 4);
  
  analysisManager->CreateH3("XYZGamma","#gamma XYZposition", 100, -1.*cm, 1.*cm, 100, -1.*cm, 1.*cm, 100, -1.*cm, 1.*cm);
  analysisManager->CreateH2("XYGamma","#gamma XYposition", 100, -10.*cm, 10.*cm, 100, -10.*cm, 10.*cm);
  analysisManager->CreateH2("XZGamma","#gamma XZposition", 100, -1.*cm, 1.*cm, 100, -1.*cm, 1.*cm);
  analysisManager->CreateH2("YZGamma","#gamma YZposition", 100, -1.*cm, 1.*cm, 100, -1.*cm, 1.*cm);
  */
/*
  analysisManager->CreateNtuple("Ntuple", "Simulation Info");
  analysisManager->CreateNtupleIColumn("EventID");
  analysisManager->CreateNtupleIColumn("TrackID");
  analysisManager->CreateNtupleIColumn("StepNumber");
  analysisManager->CreateNtupleSColumn("ParticleName");

  analysisManager->CreateNtupleDColumn("Position_x");
  analysisManager->CreateNtupleDColumn("Position_y");
  analysisManager->CreateNtupleDColumn("Position_z");
  analysisManager->CreateNtupleDColumn("Momentum_x");
  analysisManager->CreateNtupleDColumn("Momentum_y");
  analysisManager->CreateNtupleDColumn("Momentum_z");
  analysisManager->CreateNtupleDColumn("LocalTime");        
  analysisManager->CreateNtupleDColumn("GlobalTime");        

  analysisManager->CreateNtupleSColumn("VolumeName");
  analysisManager->CreateNtupleIColumn("VolumeCopyNo");
  analysisManager->CreateNtupleSColumn("ProcessName");
  analysisManager->CreateNtupleSColumn("CreatorProcessName");

  analysisManager->CreateNtupleDColumn("VertexPosition_x");
  analysisManager->CreateNtupleDColumn("VertexPosition_y");
  analysisManager->CreateNtupleDColumn("VertexPosition_z");

  analysisManager->FinishNtuple();
  */

  analysisManager->CreateNtuple("Ntuple", "Simulation Info");
  analysisManager->CreateNtupleIColumn("TrackID", fEventAction->GetTrackID());
  analysisManager->CreateNtupleIColumn("StepNumber", fEventAction->GetStepNumber());
  analysisManager->CreateNtupleIColumn("ParentID", fEventAction->GetParentID());
  analysisManager->CreateNtupleIColumn("ParticlePDG", fEventAction->GetParticlePDG());
  analysisManager->CreateNtupleDColumn("Position_x", fEventAction->GetPosition_x());
  analysisManager->CreateNtupleDColumn("Position_y", fEventAction->GetPosition_y());
  analysisManager->CreateNtupleDColumn("Position_z", fEventAction->GetPosition_z());
  analysisManager->CreateNtupleDColumn("Momentum_x", fEventAction->GetMomentum_x());
  analysisManager->CreateNtupleDColumn("Momentum_y", fEventAction->GetMomentum_y());
  analysisManager->CreateNtupleDColumn("Momentum_z", fEventAction->GetMomentum_z());
  analysisManager->CreateNtupleDColumn("KineticEnergy", fEventAction->GetKineticEnergy());
  analysisManager->CreateNtupleDColumn("LocalTime", fEventAction->GetLocalTime());
  analysisManager->CreateNtupleDColumn("GlobalTime", fEventAction->GetGlobalTime());
  analysisManager->CreateNtupleIColumn("VolumeNumber", fEventAction->GetVolumeNumber());
  analysisManager->CreateNtupleIColumn("CreatorNumber", fEventAction->GetCreatorNumber());
  analysisManager->CreateNtupleIColumn("ProcessNumber", fEventAction->GetProcessNumber());
  analysisManager->CreateNtupleIColumn("BoundaryNumber", fEventAction->GetBoundaryNumber());

  analysisManager->FinishNtuple();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::~RunAction()
{
  delete G4AnalysisManager::Instance();  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::BeginOfRunAction(const G4Run* /*run*/)
{ 
  // Get analysis manager
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

  G4String fileName = "Mirror";
  analysisManager->OpenFile(fileName);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::EndOfRunAction(const G4Run* /*run*/)
{
  // print histogram statistics
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  //analysisManager->SetNtupleMerging(true);
  //Above merging code is for multi thread work

  analysisManager->Write();
  analysisManager->CloseFile();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
