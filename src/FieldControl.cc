
//==================================================================
//Created:   19 August 2020, Jh. Lee
//==================================================================


#include "FieldControl.hh"
//#include "Field1cmAnd1mmSetup.hh"
#include "Field1cmSetup.hh"

#include "G4UniformMagField.hh"
#include "G4FieldManager.hh"
#include "G4Mag_UsualEqRhs.hh"
#include "G4TransportationManager.hh"
#include "G4ClassicalRK4.hh"
#include "G4PropagatorInField.hh"
#include "G4UserLimits.hh"
#include "G4ChordFinder.hh"
#include "G4MagIntegratorStepper.hh"
#include "G4UniformMagField.hh"

#include "G4DormandPrince745.hh"

//#include "G4MonopoleEquation.hh"

#include "G4SystemOfUnits.hh"

FieldControl::FieldControl()
 : fFieldManager(0),
   fChordFinder(0),
   fUsualChordFinder(0),
   //fMonopoleChordFinder(0),
   fEquation(0),
   //fMonopoleEquation(0),
   fMagneticField(0),
   fStepper(0),
   //fMonopoleStepper(0),
   fMinStep(0.0)
{
  fFieldManager = GetGlobalFieldManager();
}

FieldControl::~FieldControl()
{
  if(fMagneticField) delete fMagneticField;
  if(fUsualChordFinder)   delete fUsualChordFinder;
  //if(fMonopoleChordFinder)   delete fMonopoleChordFinder;
  if(fStepper)       delete fStepper;
  //if(fMonopoleStepper)  delete fMonopoleStepper;
}

void FieldControl::ConstructMagField()
{

  if (fMagneticField) { delete fMagneticField;}

   fMagneticField = new Field1cmSetup();
   //set user defined field value
   //fMagneticField = new G4UniformMagField(G4ThreeVector(0.0, 0.0, 0.0));
   
   InitialiseAll();
   //field manager setting(chord finder, minimum step length etc)

   fFieldManager->SetDetectorField(fMagneticField);
}

void FieldControl::InitialiseAll()
{
  //G4double Mul = 1.0;
  //G4double Mul = 1000.0;

  fEquation = new G4Mag_UsualEqRhs(fMagneticField);
  //fMonopoleEquation = new G4MonopoleEquation(fMagneticField);

  //fMinStep     = 0.01*mm ; // minimal step of 1 mm is default
  //fMinStep = 1e-9*m;
  //fMinStep      =  1e-5*m;
  fMinStep = 1e-9*mm;
  //fMinStep = Mul*0.001*mm; //FieldSetup value ref to /u/user/jhlee/backup_home/examples/extended/field/field05/

  //fMonopoleStepper = new G4ClassicalRK4( fMonopoleEquation );
  
  //fStepper = new G4ClassicalRK4( fEquation, 8);
  fStepper = new G4DormandPrince745(fEquation, 8);

  G4int driverId = 3;
  fUsualChordFinder = new G4ChordFinder( fMagneticField, fMinStep, fStepper, driverId);
  //fUsualChordFinder = new G4ChordFinder( fMagneticField, fMinStep, fStepper);
  //fMonopoleChordFinder = new G4ChordFinder( fMagneticField, fMinStep,
  //                                          fMonopoleStepper);

  SetStepperAndChordFinder(0);
}

void FieldControl::SetStepperAndChordFinder(G4int val)
{
  G4double Mul = 1.0;
  //G4double Mul = 10.0;

  if (fMagneticField)
  {
    fFieldManager->SetDetectorField(fMagneticField );

    switch (val)
    {
      case 0:
        fChordFinder = fUsualChordFinder;
        break;
   //   case 1:
   //     fChordFinder = fMonopoleChordFinder;
   //     break;
    }

    fFieldManager->SetChordFinder( fChordFinder );
    
    
    fFieldManager->GetChordFinder()->SetDeltaChord(1e-4*mm);
    //fFieldManager->GetChordFinder()->SetDeltaChord(Mul*0.3*mm);

	fFieldManager->SetDeltaIntersection(1e-3*mm);
    //fFieldManager->SetDeltaIntersection(Mul*0.01*mm);

	fFieldManager->SetDeltaOneStep(1e-3*mm);
   	//fFieldManager->SetDeltaOneStep(Mul*0.001*mm);

    
    G4PropagatorInField* fPropInField =
    G4TransportationManager::GetTransportationManager()->GetPropagatorInField();
    fPropInField->SetMinimumEpsilonStep(1e-8);
    fPropInField->SetMaximumEpsilonStep(1e-7);
    //fPropInField->SetMinimumEpsilonStep(Mul*0.25e-7*mm);
    //fPropInField->SetMaximumEpsilonStep(Mul*0.005*mm);
   
    
    /*    
    fFieldManager->GetChordFinder()->SetDeltaChord(1e-3*m);
    fFieldManager->SetDeltaIntersection(1e-2*m);
    fFieldManager->SetDeltaOneStep(1e-2*m);
    
    
    G4PropagatorInField* fPropInField =
    G4TransportationManager::GetTransportationManager()->GetPropagatorInField();
    fPropInField->SetMinimumEpsilonStep(1e-8);
    fPropInField->SetMaximumEpsilonStep(1e-7);
    */

    }

}

G4FieldManager*  FieldControl::GetGlobalFieldManager()
{
  return  G4TransportationManager::GetTransportationManager()
                                  ->GetFieldManager();
}

