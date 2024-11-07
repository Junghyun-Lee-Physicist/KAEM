
#ifndef Field1cmAnd1mmSetup_H
#define Field1cmAnd1mmSetup_H 1

#include "globals.hh"
#include "G4MagneticField.hh"
#include "G4SystemOfUnits.hh"
#include "G4UImessenger.hh"

#include <fstream>
#include <vector>

using namespace std;

class Field1cmAnd1mmSetup 
#ifndef STANDALONE
: public G4MagneticField
#endif

{
public:
  Field1cmAnd1mmSetup();
  virtual ~Field1cmAnd1mmSetup();

  void GetFieldValue( const double Point[4], double *Bfield) const;

private:
  
  vector< vector< vector< double > > > fXField;

  vector< vector< vector< double > > > fYField;

  vector< vector< vector< double > > > fZField;
 
  G4double px, py, pz;

  G4int fNx,fNy,fNz;

  G4double fMinix, fMaxix, fMiniy, fMaxiy, fMiniz, fMaxiz;

  G4double fDx, fDy, fDz;

  G4double lenUnit;  

///////////////////////////////////////////////////////
//for center field (grid 1mm)
  vector< vector< vector< double > > > fXField2;

  vector< vector< vector< double > > > fYField2;

  vector< vector< vector< double > > > fZField2;

  G4double px2, py2, pz2;

  G4int fNx2,fNy2,fNz2;

  G4double fMinix2, fMaxix2, fMiniy2, fMaxiy2, fMiniz2, fMaxiz2;

  G4double fDx2, fDy2, fDz2;

  G4double lenUnit2;  


};

#endif
