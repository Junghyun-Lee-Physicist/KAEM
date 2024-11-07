//==================================================================
//Created:   19 August 2020, Jh. Lee
//==================================================================

#ifndef FieldControl_H
#define FieldControl_H

#include "G4MagneticField.hh"

class FieldSetup;

class G4FieldManager;
class G4ChordFinder;
class G4Mag_UsualEqRhs;
class G4MagIntegratorStepper;
//class G4MonopoleEquation;


class FieldControl
{
public:
 
  FieldControl();
  ~FieldControl();

  void InitialiseAll();
  void SetStepperAndChordFinder(G4int val);
  void ConstructMagField ();

private:

  G4FieldManager*         GetGlobalFieldManager() ;

  G4FieldManager*         fFieldManager ;
  G4ChordFinder*          fChordFinder ;
  G4ChordFinder*          fUsualChordFinder ;
  //G4ChordFinder*          fMonopoleChordFinder ;
  G4Mag_UsualEqRhs*       fEquation ;
  //G4MonopoleEquation*     fMonopoleEquation ;

  G4MagneticField*        fMagneticField ;

  G4MagIntegratorStepper* fStepper ;
  //G4MagIntegratorStepper* fMonopoleStepper ;

  G4double                fMinStep ;

};

#endif
