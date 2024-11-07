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
//
/// \file DetectorConstruction.hh
/// \brief Definition of the DetectorConstruction class

#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4TransportationManager.hh"
#include "G4UserLimits.hh"

#include "G4Cache.hh"

//Elements
#include "G4Element.hh"

//Color visual
#include "G4VisAttributes.hh"

//For Optical surface
#include "G4OpticalSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4LogicalBorderSurface.hh"


class FieldControl;
class G4Box;
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4LogicalVolume;
/// Detector construction class to define materials and geometry.

class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    DetectorConstruction();
    virtual ~DetectorConstruction() override;

    virtual  G4VPhysicalVolume* Construct() override;
    
    G4LogicalVolume* GetScoringVolume() const { return fScoringVolume; }

    virtual void ConstructSDandField() override;

    FieldControl* GetFieldControl() const { return fFieldControl.Get(); }

    const G4VPhysicalVolume* GetTracker_L() const;
    const G4VPhysicalVolume* GetTracker_R() const;
   
  private:

    G4Cache<FieldControl*> fFieldControl;
/////////////////////////////////////////////////////////
//트리거 박스 쉘과 트리거 박스
    G4Box*		STrigger_Box_Shell;
    G4LogicalVolume* 	LTrigger_Box_Shell;
    G4VPhysicalVolume* 	PTrigger_Box_Shell;   

    G4Box*		STrigger_Box;
    G4LogicalVolume* 	LTrigger_Box;
    G4VPhysicalVolume* 	PTrigger_Box;   

//트리거 내부 실린더와 그 쉘
    G4Tubs*		STrigger_Cylinder_Shell;
    G4LogicalVolume*	LTrigger_Cylinder_Shell;
    G4VPhysicalVolume*	PTrigger_Cylinder_Shell;

    G4Tubs*		STrigger_Cylinder;
    G4LogicalVolume*	LTrigger_Cylinder;
    G4VPhysicalVolume*	PTrigger_Cylinder;

//좌우의 박스 쉘 안에 존재하는 얇은 쉘과 실린더
    G4Tubs*		SCylinderShell1_InBoxShell;
    G4LogicalVolume*	LCylinderShell1_InBoxShell;
    G4VPhysicalVolume*	PCylinderShell1_InBoxShell;

    G4Tubs*		SCylinder1_InBoxShell;
    G4LogicalVolume*	LCylinder1_InBoxShell;
    G4VPhysicalVolume*	PCylinder1_InBoxShell;

    G4Tubs*		SCylinderShell2_InBoxShell;
    G4LogicalVolume*	LCylinderShell2_InBoxShell;
    G4VPhysicalVolume*	PCylinderShell2_InBoxShell;

    G4Tubs*		SCylinder2_InBoxShell;
    G4LogicalVolume*	LCylinder2_InBoxShell;
    G4VPhysicalVolume*	PCylinder2_InBoxShell;

////////////////////////////////////////////////////////
//양쪽 실린더
    G4Tubs*		SCylinder1_Shell;
    G4LogicalVolume*	LCylinder1_Shell;
    G4VPhysicalVolume*	PCylinder1_Shell;

    G4Tubs*		SCylinder1;
    G4LogicalVolume*	LCylinder1;
    G4VPhysicalVolume*	PCylinder1;

    G4Tubs*		SCylinder2_Shell;
    G4LogicalVolume*	LCylinder2_Shell;
    G4VPhysicalVolume*	PCylinder2_Shell;

    G4Tubs*		SCylinder2;
    G4LogicalVolume*	LCylinder2;
    G4VPhysicalVolume*	PCylinder2;

//ECAL
    G4Tubs*		SECAL1_Shell;
    G4LogicalVolume*	LECAL1_Shell;
    G4VPhysicalVolume*	PECAL1_Shell;

    G4Tubs*		SECAL1;
    G4LogicalVolume*	LECAL1;
    G4VPhysicalVolume*	PECAL1;

    G4Tubs*		SECAL2_Shell;
    G4LogicalVolume*	LECAL2_Shell;
    G4VPhysicalVolume*	PECAL2_Shell;

    G4Tubs*		SECAL2;
    G4LogicalVolume*	LECAL2;
    G4VPhysicalVolume*	PECAL2;

/////////////////////////////////////////////////////////
//Shielding and Solenoid
    G4Tubs*		SShielding_Mirror;
    G4LogicalVolume*	LShielding_Mirror;
    G4VPhysicalVolume*	PShielding_Mirror;

    G4Tubs*		SShielding1;
    G4LogicalVolume*	LShielding1;
    G4VPhysicalVolume*	PShielding1;

    G4Tubs*		SShielding2;
    G4LogicalVolume*	LShielding2;
    G4VPhysicalVolume*	PShielding2;

    G4Tubs*		SShieldingECAL1;
    G4LogicalVolume*	LShieldingECAL1;
    G4VPhysicalVolume*	PShieldingECAL1;

    G4Tubs*		SShieldingECAL2;
    G4LogicalVolume*	LShieldingECAL2;
    G4VPhysicalVolume*	PShieldingECAL2;

    G4Tubs*		SSolenoid1;
    G4LogicalVolume*	LSolenoid1;
    G4VPhysicalVolume*	PSolenoid1;

    G4Tubs*		SSolenoid2;
    G4LogicalVolume*	LSolenoid2;
    G4VPhysicalVolume*	PSolenoid2;
/////////////////////////////////////////////////////////
//SiPM
    G4Box*		SSiPM1;
    G4LogicalVolume*	LSiPM1;

    G4Box*		SSiPM2;
    G4LogicalVolume*	LSiPM2;
   
    G4Box*		SSiPMFront1;
    G4LogicalVolume*	LSiPMFront1;
    G4LogicalSkinSurface* SurfaceSiPMFront1;
    G4LogicalSkinSurface* SurfaceSiPMFront2;

    G4Box*		SSiPMFront2;
    G4LogicalVolume*	LSiPMFront2;

/////////////////////////////////////////////////////////
//Trigger_Veto SiPM
    G4Box*		STrigger_Veto_SiPMFront1;
    G4LogicalVolume*    LTrigger_Veto_SiPMFront1;
    G4LogicalSkinSurface* Surface_TriggerSiPMFront1;

    G4Box*		STrigger_Veto_SiPMFront2;
    G4LogicalVolume*    LTrigger_Veto_SiPMFront2;
    G4LogicalSkinSurface* Surface_TriggerSiPMFront2;

    G4Box*		STrigger_Veto_SiPM1;
    G4LogicalVolume*    LTrigger_Veto_SiPM1;

    G4Box*		STrigger_Veto_SiPM2;
    G4LogicalVolume*    LTrigger_Veto_SiPM2;

/////////////////////////////////////////////////////////
    G4VisAttributes* fVisAttrTransparent; //For World 투명
    G4VisAttributes* fVisAttr_half_Transparent; //For World 투명
    //G4VisAttributes* fVisAttrGreen; //For Trigger_Box
    G4VisAttributes* fVisAttrSq; //For LYSO 청록
    G4VisAttributes* fVisAttrTargetEnv; //For TargetEnv
	G4VisAttributes* fVisAttrTargetLeft; //For TargetLeft
  	G4VisAttributes* fVisAttrTargetRight; //For TargetRight
	G4VisAttributes* fVisAttrShielding_Mirror; //For solenoid_Mirror 노랑
    G4VisAttributes* fVisAttrShielding1;
    G4VisAttributes* fVisAttrShielding2;
    G4VisAttributes* fVisAttrSolenoid1; //For Solenoid1 자홍
    G4VisAttributes* fVisAttrSolenoid2; //For Solenoid2 자홍
    G4VisAttributes* fVisAttrTrigger_Veto_Box;
    G4VisAttributes* fVisAttrShieldingECAL1;
    G4VisAttributes* fVisAttrShieldingECAL2;
    G4VisAttributes* fVisAttrSiPM1;
    G4VisAttributes* fVisAttrSiPM2;
    G4VisAttributes* fVisAttrSiPMFront1;
    G4VisAttributes* fVisAttrSiPMFront2;
    G4VisAttributes* fVisAttrTrigger_Veto_SiPMFront1;
    G4VisAttributes* fVisAttrTrigger_Veto_SiPMFront2;
    G4VisAttributes* fVisAttrTrigger_Veto_SiPM1;
    G4VisAttributes* fVisAttrTrigger_Veto_SiPM2;

    G4VisAttributes* fVisAttrPurple; //For Left Tracker
    G4VisAttributes* fVisAttrYellow; //For Right Tracker

///////////////////////////////////////////////////////////
    G4Box*                 fSolidWorldEnv;
    G4LogicalVolume*       fLogicWorldEnv;
    G4VPhysicalVolume*     fPhysWorldEnv;

    G4Box*                 fSolidWorld;
    G4LogicalVolume*       fLogicWorld;
    G4VPhysicalVolume*     fPhysWorld;

/*
    G4Tubs*                 fSolidEnv;
    G4LogicalVolume*       fLogicEnv;
    G4VPhysicalVolume*     fPhysiEnv;
*/
////////////////////////////////////////////////////////////
//For Target
	G4Tubs*                fSTargetEnv3;
    G4LogicalVolume*       fLTargetEnv3;
    G4VPhysicalVolume*     fPTargetEnv3;

	G4Tubs*                fSTargetEnv2;
    G4LogicalVolume*       fLTargetEnv2;
    G4VPhysicalVolume*     fPTargetEnv2;

	G4Tubs*                fSTargetEnv1;
    G4LogicalVolume*       fLTargetEnv1;
    G4VPhysicalVolume*     fPTargetEnv1;

	G4Tubs*                fSTargetLeft;
    G4LogicalVolume*       fLTargetLeft;
    G4VPhysicalVolume*     fPTargetLeft;

	G4Tubs*                fSTargetRight;
    G4LogicalVolume*       fLTargetRight;
    G4VPhysicalVolume*     fPTargetRight;

////////////////////////////////////////////////////////////

	G4Box*				   fSFieldLimit;
    G4LogicalVolume*       fLFieldLimit;
    G4VPhysicalVolume*     fPFieldLimit;

	G4Box*				   fSFieldEnv;
    G4LogicalVolume*       fLFieldEnv;
    G4VPhysicalVolume*     fPFieldEnv;

	G4Box*				   fSFieldLimit_In;
    G4LogicalVolume*       fLFieldLimit_In;
    G4VPhysicalVolume*     fPFieldLimit_In;

	G4Box*                 fSTracker_Left;
    G4LogicalVolume*       fLTracker_Left;
    G4VPhysicalVolume*     fPTracker_Left;

    G4Box*                 fSTracker_Right;
    G4LogicalVolume*       fLTracker_Right;
    G4VPhysicalVolume*     fPTracker_Right;

/*
    G4Tubs*                fSolidCubic1;
    G4LogicalVolume*       fLogicCubic1;
    G4VPhysicalVolume*     fPhysiCubic1;

    G4Tubs*                fSolidCubic2;
    G4LogicalVolume*       fLogicCubic2;
    G4VPhysicalVolume*     fPhysiCubic2; 
*/
    //Elements
    G4Element* m_ElLu;
    G4Element* m_ElY;
    G4Element* m_ElSi;
    G4Element* m_ElO;
    G4Element* m_ElCe;
    G4Element* Si; //위에거는 LYSO, 이거는 SiPM silicon을 위한 material
	G4Element* H;
	G4Element* C;

    G4Material*            fWorld_material;
    //G4Material*            fEnv_material;
    G4Material*            fTarget_material;
	G4Material*            fTracker_material;
    //G4Material*            fCubic_material;
    G4Material*            fSq_material;
    G4Material*		   fSiPM_material;
    //SiPM Front optical surface
    G4OpticalSurface* fSiPMFront_Surf;
    G4OpticalSurface* fAl_Surf; 

    G4Material*		   fTrigger_Box_Shell_material;
    G4Material*		   fTrigger_Box_material;
    //G4Material*	   fTrigger_Veto_Box_material;
    G4Material*		   fVacuum;
    G4Material*			 fAir;
    G4Material*			 fAl;
//////////////////////////////////////////////////////////////////////

    //Cylinder material
    G4Material*		fCylinder_Shell_material;
    G4Material*		fCylinder_material;

    //ECAL material
    G4Material*		fECAL_material;
    G4Material*		fECAL_Shell_material;

    //Shielding and Solenoid material
    G4Material*		fShielding_Mirror_material;
    G4Material*		fShielding1_material;
    G4Material*		fShielding2_material;
    G4Material*		fSolenoid1_material;
    G4Material*		fSolenoid2_material;

/////////////////////////////////////////////////////////////////////
//Silicon material(SiPM front material)
    G4Material*		fSiPMFront_material;

/////////////////////////////////////////////////////////////////////
    G4double SqCenter_x;
    G4double SqCenter_y;
    G4double SqCenter_z;
    
    G4double SpaceOfCircle;

    //static G4ThreadLocal FieldSetup* fField;
    void ConstructMaterials();
    void DefineMaterials();
    void CenterFinder(G4double x_, G4double y_, G4double a_, G4double i_);
    G4double SpaceFinder(G4double x_, G4double y_, G4double SqL_, G4double r_);   
    G4bool Checking_Empty_Space(G4int Count_Veto_); 

  protected:
    G4LogicalVolume*  fScoringVolume;

};

inline const G4VPhysicalVolume* DetectorConstruction::GetTracker_L() const{
return fPTracker_Left;
}

inline const G4VPhysicalVolume* DetectorConstruction::GetTracker_R() const{
return fPTracker_Right;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
