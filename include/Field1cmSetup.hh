
#ifndef Field1cmSetup_H
#define Field1cmSetup_H 1

#include "globals.hh"
#include "G4MagneticField.hh"
#include "G4SystemOfUnits.hh"
#include "G4UImessenger.hh"

#include <fstream>
#include <vector>

using namespace std;

class Field1cmSetup 
#ifndef STANDALONE
: public G4MagneticField
#endif

{
public:
  Field1cmSetup();
  virtual ~Field1cmSetup();

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

};

#endif
