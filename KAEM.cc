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
/// \file example.cc
/// \brief Main program of the  example
#include "G4Types.hh"
#include "DetectorConstruction.hh"
#include "ActionInitialization.hh"

//#ifdef G4MULTITHREADED
//#include "G4MTRunManager.hh"
//#else
#include "G4RunManager.hh"
//#endif

#include "G4PhysListRegistry.hh"
#include "G4PhysListFactoryAlt.hh"
#include "G4PhysListStamper.hh" 

#include "G4UImanager.hh"
#include "G4UIcommand.hh"
#include "FTFP_BERT.hh"
#include "PhysicsList.hh"
#include "Randomize.hh"
#include <time.h>

#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include "G4StepLimiterPhysics.hh"

//#include "G4OpticalPhysics.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

namespace {
  void PrintUsage() {
    G4cerr << " Usage: " << G4endl;
    G4cerr << " example [-m macro ] [-u UIsession] [-t nThreads] [-s seed]" << G4endl;
    G4cerr << "   note: -t option is available only for multi-threaded mode."
           << G4endl;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc,char** argv)
{
  // Evaluate arguments
  //
  if ( argc > 9 ) {
    PrintUsage();
    return 1;
  }
  
  G4String macro;
  G4String session;
  G4int seed = 0;
#ifdef G4MULTITHREADED
  G4int nThreads = 1;
#endif
  for ( G4int i=1; i<argc; i=i+2 ) {
    if      ( G4String(argv[i]) == "-m" ) macro = argv[i+1];
    else if ( G4String(argv[i]) == "-u" ) session = argv[i+1];
    else if ( G4String(argv[i]) == "-s" ) seed = std::atoi(argv[i+1]);
#ifdef G4MULTITHREADED
    else if ( G4String(argv[i]) == "-t" ) {
      nThreads = G4UIcommand::ConvertToInt(argv[i+1]);
    }
#endif
    else {
      PrintUsage();
      return 1;
    }
  }  
  
  // Detect interactive mode (if no macro provided) and define UI session
  //
  G4UIExecutive* ui = nullptr;
  if ( ! macro.size() ) {
    ui = new G4UIExecutive(argc, argv, session);
  }

  // Choose the Random engine
  //

  G4Random::setTheEngine(new CLHEP::RanecuEngine);
  G4Random::setTheSeed(seed);
  // Construct the default run manager
 
  G4cout << "Set seed value "<< seed <<G4endl;
  G4cout << "Seed: " << G4Random::getTheSeed() << G4endl;

  /*
  long seeds[2];
  time_t systime = time(NULL);
  seeds[0] = seed;
  seeds[1] = (long) (systime*G4UniformRand());
  G4Random::setTheSeeds(seeds);
  G4cout << "Seed: " << G4Random::getTheSeed() << G4endl;
  G4cout << "seeds = "<< seeds[0] <<", "<<seeds[1]<<G4endl;
  */


//#ifdef G4MULTITHREADED
//  auto runManager = new G4MTRunManager;
//  if ( nThreads > 0 ) { 
//    runManager->SetNumberOfThreads(nThreads);
//  }  
//#else
  auto runManager = new G4RunManager;
//#endif

  // Set mandatory initialization classes
  //
  auto detConstruction = new DetectorConstruction();
  runManager->SetUserInitialization(detConstruction);

  //auto physicsList = new FTFP_BERT;
  //G4VModularPhysicsList* physicsList = new FTFP_BERT;
  //physicsList->RegisterPhysics(new G4StepLimiterPhysics());
  //physicsList->SetVerboseLevel(1);
  
  runManager->SetUserInitialization(new PhysicsList);
    
  auto actionInitialization = new ActionInitialization(detConstruction);
  runManager->SetUserInitialization(actionInitialization);
  
  // Initialize visualization
  //
  auto visManager = new G4VisExecutive;
  // G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
  // G4VisManager* visManager = new G4VisExecutive("Quiet");
  visManager->Initialize();

  // Get the pointer to the User Interface manager
  auto UImanager = G4UImanager::GetUIpointer();

  // Process macro or start UI session
  //
  if ( macro.size() ) {
    // batch mode
    G4String command = "/control/execute ";
    UImanager->ApplyCommand(command+macro);
  }
  else  {  
    // interactive mode : define UI session
    //UImanager->ApplyCommand("/control/execute init_vis.mac");
    UImanager->ApplyCommand("/control/execute vis.mac");
    if (ui->IsGUI()) {
      UImanager->ApplyCommand("/control/execute gui.mac");
    }
    ui->SessionStart();
    delete ui;
  }

  // Job termination
  // Free the store: user actions, physics_list and detector_description are
  // owned and deleted by the run manager, so they should not be deleted 
  // in the main() program !

  delete visManager;
  delete runManager;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....
