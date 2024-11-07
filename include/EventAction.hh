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
/// \file EventAction.hh
/// \brief Definition of the EventAction class

#ifndef EventAction_h
#define EventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"

#include <vector>
using namespace std;

class EventAction : public G4UserEventAction
{
  public:
    EventAction();
    virtual ~EventAction();

    virtual void  BeginOfEventAction(const G4Event* event);
    virtual void    EndOfEventAction(const G4Event* event);

	vector<G4int>&		GetTrackID()	    {return VTrackID;}   
	vector<G4int>&		GetStepNumber()     {return VStepNumber;}
	vector<G4int>&		GetParentID()       {return VParentID;}
	vector<G4int>&		GetParticlePDG()    {return VParticlePDG;}
	vector<G4double>&	GetPosition_x()     {return VPosition_x;}
	vector<G4double>&	GetPosition_y()     {return VPosition_y;}
	vector<G4double>&	GetPosition_z()     {return VPosition_z;}
	vector<G4double>&	GetMomentum_x()		{return VMomentum_x;}
	vector<G4double>&	GetMomentum_y()		{return VMomentum_y;}
	vector<G4double>&	GetMomentum_z()		{return VMomentum_z;}
	vector<G4double>&	GetKineticEnergy()	{return VKineticEnergy;}
	vector<G4double>&	GetLocalTime()		{return VLocalTime;}
	vector<G4double>&	GetGlobalTime()		{return VGlobalTime;}
	vector<G4int>&		GetVolumeNumber()	{return VVolumeNumber;}
	vector<G4int>&		GetCreatorNumber()	{return VCreatorNumber;}
	vector<G4int>&		GetProcessNumber()	{return VProcessNumber;}
	vector<G4int>&		GetBoundaryNumber()	{return VBoundaryNumber;}

	void StepInfo( 	G4int i1, G4int i2, G4int i3, G4int i4,
					G4double d1, G4double d2, G4double d3,
					G4double d4, G4double d5, G4double d6, G4double d7,
					G4double d8, G4double d9,
					G4int i5, G4int i6, G4int i7,
					G4int i8
					);


  private:
    G4String  fDecayChain;
    G4int	  EventID;

	vector<G4int>		VTrackID;
	vector<G4int>		VStepNumber;
	vector<G4int>		VParentID;
	vector<G4int>		VParticlePDG;
	vector<G4double>	VPosition_x;
	vector<G4double>	VPosition_y;
	vector<G4double>	VPosition_z;
	vector<G4double>	VMomentum_x;
	vector<G4double>	VMomentum_y;
	vector<G4double>	VMomentum_z;
        vector<G4double>        VKineticEnergy;
	vector<G4double>	VLocalTime;
	vector<G4double>	VGlobalTime;
	vector<G4int>		VVolumeNumber;
	vector<G4int>		VCreatorNumber;
	vector<G4int>		VProcessNumber;
	vector<G4int>		VBoundaryNumber;

};

inline void EventAction::StepInfo( 	G4int i1, G4int i2, G4int i3, G4int i4,
									G4double d1, G4double d2, G4double d3,
									G4double d4, G4double d5, G4double d6, G4double d7,
									G4double d8, G4double d9, 
									G4int i5, G4int i6, G4int i7,
									G4int i8
									)
{
	VTrackID.push_back(i1);
	VStepNumber.push_back(i2);
	VParentID.push_back(i3);
	VParticlePDG.push_back(i4);
	VPosition_x.push_back(d1);
	VPosition_y.push_back(d2);
	VPosition_z.push_back(d3);
	VMomentum_x.push_back(d4);
	VMomentum_y.push_back(d5);
	VMomentum_z.push_back(d6);
        VKineticEnergy.push_back(d7);
	VLocalTime.push_back(d8);
	VGlobalTime.push_back(d9);
	VVolumeNumber.push_back(i5);
	VCreatorNumber.push_back(i6);
	VProcessNumber.push_back(i7);
	VBoundaryNumber.push_back(i8);
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif


