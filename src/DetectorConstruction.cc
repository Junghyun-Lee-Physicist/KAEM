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
/// \file DetectorConstruction.cc
/// \brief Implementation of the DetectorConstruction class

#include "DetectorConstruction.hh"
#include "FieldControl.hh"

#include "G4NistManager.hh"
#include "G4Material.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh" //For volume copy

#include "G4SDManager.hh"
#include "G4VSensitiveDetector.hh" //For sensitive and magnetic field 

#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"

#include "G4UserLimits.hh"
#include "G4AutoDelete.hh"

#include "G4VisAttributes.hh" //For Color Visual

//For sensitive detector
#include "EmCalorimeterSD.hh"
#include "G4VSensitiveDetector.hh"

//For optical physics
#include "G4OpticalSurface.hh"

#include <cmath>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  DetectorConstruction::DetectorConstruction()
: G4VUserDetectorConstruction(),
  fScoringVolume(0)
{
  ConstructMaterials();
  DefineMaterials();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction()
{
  delete m_ElLu;
  delete m_ElY;
  delete m_ElSi;
  delete m_ElO;
  delete m_ElCe;
  delete Si;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::ConstructMaterials()
{
  //Elements for DefineMaterials()
  m_ElLu = new G4Element( "Lutetium", "Lu", 71., 174.9668  *g/mole);
  m_ElY  = new G4Element( "Yttrium" , "Y" , 39., 88.90585 *g/mole);
  m_ElSi = new G4Element( "Silicon" , "Si", 14., 28.0855  *g/mole);
  m_ElO  = new G4Element( "Oxygen"  ,  "O",  8., 15.9994  *g/mole);
  m_ElCe = new G4Element( "Cerium"  , "Ce", 58., 140.116  *g/mole);

  //Elements for Silicon(SiPM Front material)
  Si = new G4Element("Silicon" , "Si", 14., 28.085  *g/mole);
  H = new G4Element("Hydrogen","H" , 1., 1.01*g/mole);
  C = new G4Element("Carbon"  ,"C" , 6., 12.01*g/mole);

}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::DefineMaterials()
{
  G4NistManager* nist = G4NistManager::Instance();

  //fTarget_material = nist->FindOrBuildMaterial("G4_Galactic");
  //fTracker_material = nist->FindOrBuildMaterial("G4_Galactic");
  //fWorld_material = nist->FindOrBuildMaterial("G4_Galactic");

  fTarget_material = nist->FindOrBuildMaterial("G4_Al");
  //fTarget_material = nist->FindOrBuildMaterial("G4_Galactic");

  fTracker_material = nist->FindOrBuildMaterial("G4_Galactic");
  //fTracker_material = nist->FindOrBuildMaterial("G4_Al");
  fWorld_material = nist->FindOrBuildMaterial("G4_Galactic");


  fVacuum = nist->FindOrBuildMaterial("G4_Galactic");
  //nist->FindOrBuildMaterial("G4_Galactic");
  //fVacuum = G4Material::GetMaterial("G4_Galactic");

  fAir = nist->FindOrBuildMaterial("G4_AIR");
  //fAir = nist->FindOrBuildMaterial("G4_Galactic");
  //nist->FindOrBuildMaterial("G4_AIR");
  //fAir = G4Material::GetMaterial("G4_Air");
  //fAir = G4Material::GetMaterial("G4_Galactic");

  fAl = nist->FindOrBuildMaterial("G4_Al");
  //fAl = nist->FindOrBuildMaterial("G4_Galactic");
  //nist->FindOrBuildMaterial("G4_Al");
  //fAl = G4Material::GetMaterial("G4_Al");
  //fAl = G4Material::GetMaterial("G4_Galactic");

  //////////////////////////////////////////////////////////////////////////////
  //optical properties에서 배열만 따로 먼저 선언한다.
  //Material Properties for Optical process 
  G4double photonEnergy[] = { // from 900nm to 300nm with 25nm step
    1.37760*eV, 1.41696*eV, 1.45864*eV, 1.50284*eV, 1.54980*eV, 1.59980*eV, 1.65312*eV, 1.71013*eV,
    1.77120*eV, 1.83680*eV, 1.90745*eV, 1.98375*eV, 2.06640*eV, 2.15625*eV, 2.25426*eV, 2.36160*eV,
    2.47968*eV, 2.61019*eV, 2.75520*eV, 2.91728*eV, 3.09960*eV, 3.30625*eV, 3.54241*eV, 3.81490*eV, 4.13281*eV
  };
  const G4int nEntries = sizeof(photonEnergy)/sizeof(G4double);

  G4double refractiveIndex1[nEntries];
  std::fill_n(refractiveIndex1, nEntries, 1.82);  
  G4double absorption[nEntries];
  std::fill_n(absorption, nEntries, 42*cm);

  G4double scintil[] = 
  {   0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00,
    0.00, 0.00, 0.00, 0.03, 0.07, 0.12, 0.16, 0.32,
    0.50, 0.68, 0.92, 0.97, 0.50, 0.07, 0.00, 0.00, 0.00
  };
  assert(sizeof(scintil) == sizeof(photonEnergy));  

  //////////////////////////////////////////////////////////////////////////////
  //Vacuum optical properties
  
  G4double RI_Vacuum[nEntries]; std::fill_n(RI_Vacuum,nEntries,1.0);
  G4MaterialPropertiesTable* mpVacuum = new G4MaterialPropertiesTable();
  mpVacuum->AddProperty("RINDEX",photonEnergy,RI_Vacuum,nEntries);
  fVacuum->SetMaterialPropertiesTable(mpVacuum);

  //Air optical properies
  G4double RI_Air[nEntries]; std::fill_n(RI_Air,nEntries,1.0);
  G4MaterialPropertiesTable* mpAir = new G4MaterialPropertiesTable();
  mpAir->AddProperty("RINDEX",photonEnergy,RI_Air,nEntries);
  fAir->SetMaterialPropertiesTable(mpAir);

  //Al optical properties
  G4double RI_Al[nEntries]; std::fill_n(RI_Vacuum,nEntries,0.99);
  G4MaterialPropertiesTable* mpAl = new G4MaterialPropertiesTable();
  mpAl->AddProperty("RINDEX",photonEnergy,RI_Al,nEntries);
  fAl->SetMaterialPropertiesTable(mpAl);
  
  //////////////////////////////////////////////////////////////////////////////
  //Shell material
  fCylinder_Shell_material = nist->FindOrBuildMaterial("G4_STAINLESS-STEEL");
  //fCylinder_Shell_material = nist->FindOrBuildMaterial("G4_Galactic");
  //cylinder material
  fCylinder_material = nist->FindOrBuildMaterial("G4_Galactic");

  //이것은 모든 실린더와 쉘 메터리얼을 정의한다. 즉 트리거에 있건 좌우에 있건 아니면 Cubic이건 말이다.

  fECAL_Shell_material = nist->FindOrBuildMaterial("G4_STAINLESS-STEEL");
  //fECAL_Shell_material = fVacuum;
  fECAL_material = nist->FindOrBuildMaterial("G4_Galactic");

  fTrigger_Box_Shell_material = nist->FindOrBuildMaterial("G4_STAINLESS-STEEL");
  //fTrigger_Box_Shell_material = fVacuum;
  fTrigger_Box_material = nist->FindOrBuildMaterial("G4_Galactic");

  fShielding_Mirror_material = nist->FindOrBuildMaterial("G4_STAINLESS-STEEL");
  fShielding1_material = nist->FindOrBuildMaterial("G4_STAINLESS-STEEL");
  fShielding2_material = nist->FindOrBuildMaterial("G4_STAINLESS-STEEL");
  fSolenoid1_material = nist->FindOrBuildMaterial("G4_Cu");
  fSolenoid2_material = nist->FindOrBuildMaterial("G4_Cu");

  //fShielding_Mirror_material = nist->FindOrBuildMaterial("G4_Galactic");;
  //fShielding1_material = nist->FindOrBuildMaterial("G4_Galactic");;
  //fShielding2_material = nist->FindOrBuildMaterial("G4_Galactic");;
  //fSolenoid1_material = nist->FindOrBuildMaterial("G4_Galactic");;
  //fSolenoid2_material = nist->FindOrBuildMaterial("G4_Galactic");;

  fSiPMFront_material = new G4Material("Silicon", 14., 28.09*g/mole, 2.33*g/cm3);
  //fSiPMFront_material = nist->FindOrBuildMaterial("G4_Galactic");

  fSiPM_material = new G4Material("Glass", 1.032*g/cm3, 2);
  fSiPM_material->AddElement(C, 91.533*perCent);
  fSiPM_material->AddElement(H, 8.467*perCent);

  G4double RI_Glass[nEntries]; std::fill_n(RI_Glass, nEntries, 1.52);
  G4double Abslength_Glass[nEntries]; std::fill_n(Abslength_Glass, nEntries, 420.*cm);
  G4MaterialPropertiesTable* mpGlass = new G4MaterialPropertiesTable();
  mpGlass->AddProperty("RINDEX",photonEnergy,RI_Glass,nEntries);
  mpGlass->AddProperty("ABSLENGTH",photonEnergy,Abslength_Glass,nEntries);
  fSiPM_material->SetMaterialPropertiesTable(mpGlass);
  //fSiPM_material =  nist->FindOrBuildMaterial("G4_Galactic");

  ////////////////////////////////////////////////////////////////////////////////

  //For LYSO material
  fSq_material 
    = new G4Material("LYSO", 7.25*g/cm3, 5);
  fSq_material -> AddElement(m_ElLu,  73.856/100.0);  
  fSq_material -> AddElement(m_ElY,   1.975/100.0);
  fSq_material -> AddElement(m_ElSi,  6.240/100.0);
  fSq_material -> AddElement(m_ElO,   17.773/100.0);
  fSq_material -> AddElement(m_ElCe,  0.156/100.0);

  G4MaterialPropertiesTable* myMPT1 = new G4MaterialPropertiesTable();

  myMPT1->AddProperty("RINDEX",       photonEnergy, refractiveIndex1,nEntries)
    ->SetSpline(true);
  myMPT1->AddProperty("ABSLENGTH",    photonEnergy, absorption,     nEntries)
    ->SetSpline(true);
  myMPT1->AddProperty("FASTCOMPONENT",photonEnergy, scintil,  nEntries)
    ->SetSpline(true);
  myMPT1->AddConstProperty("SCINTILLATIONYIELD",33200./MeV);
  //myMPT1->AddConstProperty("SCINTILLATIONYIELD",500./MeV);
  myMPT1->AddConstProperty("RESOLUTIONSCALE",2.0);
  myMPT1->AddConstProperty("FASTTIMECONSTANT", 42.*ns);
  myMPT1->AddConstProperty("YIELDRATIO", 1.0);
  fSq_material -> SetMaterialPropertiesTable(myMPT1);
  //fSq_material = nist->FindOrBuildMaterial("G4_Galactic");

  //fTrigger_Veto_Box_material -> SetMaterialPropertiesTable(myMPT1);
  //두 개의Material을 같은 Properties를 인자로 넣으니 오류 발생

  //////////////////////////////////////////////////////////////
  //reflect 반사 (dual에서 SiPM 껍데기의 반사율 0으로 설정)
  //사실 이건 SiPM의 (quantum) efficiency를 위한 것으로 보임

  G4MaterialPropertiesTable* SiPM_Front;
  SiPM_Front = new G4MaterialPropertiesTable();
  G4double refl_SiPM[nEntries]; std::fill_n(refl_SiPM, nEntries, 0.);

     G4double eff_SiPM[nEntries] = {
     0.03, 0.04, 0.05, 0.06, 0.07, 0.08, 0.09, 0.10,
     0.11, 0.13, 0.15, 0.17, 0.19, 0.20, 0.22, 0.23,
     0.24, 0.25, 0.24, 0.23, 0.21, 0.20, 0.17, 0.14, 0.10
     };
     
  //G4double eff_SiPM[nEntries]; std::fill_n(eff_SiPM, nEntries, 1.0);
  SiPM_Front->AddProperty("REFLECTIVITY", photonEnergy, refl_SiPM, nEntries);
  SiPM_Front->AddProperty("EFFICIENCY", photonEnergy, eff_SiPM, nEntries);

  fSiPMFront_Surf = new G4OpticalSurface("SiPMFront",glisur,polished,dielectric_metal);
  fSiPMFront_Surf->SetMaterialPropertiesTable(SiPM_Front);

  //For Al optical Properties
  G4MaterialPropertiesTable* Al_Surf;
  Al_Surf = new G4MaterialPropertiesTable();
  G4double refl_Test[nEntries]; std::fill_n(refl_Test, nEntries, 1.0);
  G4double eff_Test[nEntries]; std::fill_n(eff_Test, nEntries, 1.0);
  Al_Surf->AddProperty("REFLECTIVITY", photonEnergy, refl_Test, nEntries);
  Al_Surf->AddProperty("EFFICIENCY", photonEnergy, eff_Test, nEntries);

  fAl_Surf = new G4OpticalSurface("Al surf",glisur,polished,dielectric_metal);
  fAl_Surf->SetMaterialPropertiesTable(Al_Surf);

  //////////////////////////////////////////////////////////////  
  G4MaterialPropertiesTable* SiPM = new G4MaterialPropertiesTable();
  G4double refractiveIndexSiPM[nEntries];
  //std::fill_n(refractiveIndexSiPM, nEntries, 1);  
  //우선 SiPM의 반사가 어느정도로 될지는 모르겠다. 그냥 통과하게 만들려면 굴절률이 같아야 한다.
  //따라서 우선 LYSO의 굴절률을 대입시켜 준다.
  //SiPM->AddProperty("RINDEX", photonEnergy, refractiveIndexSiPM,nEntries)
  SiPM->AddProperty("RINDEX", photonEnergy, refractiveIndex1,nEntries)
    ->SetSpline(true);
  fSiPMFront_material -> SetMaterialPropertiesTable(SiPM);

}

G4VPhysicalVolume* DetectorConstruction::Construct()
{  

  // Option to switch on/off checking of volumes overlaps
  G4bool checkOverlaps = true;
  if(checkOverlaps == false)
  {
    for(int warning = 0; warning<5; warning++){
      G4cout<<"===================================WARNING==================================="<<G4endl;
      G4cout<<"                   WE ARE NOT CHECKING OVERLAPS OF VOLUMES                   "<<G4endl; 
	  G4cout<<"===================================WARNING==================================="<<G4endl;
    }
  }

  //Trigger Box shell 즉 박스 껍데기
  G4double Trigger_Box_ShellX = 340*mm;
  G4double Trigger_Box_ShellY = 139.8*2.*mm; //이거 값 정확하게 해야함
  G4double Trigger_Box_ShellZ = 300*mm;
  G4double Trigger_Box_PosiX = 0.;
  G4double Trigger_Box_PosiY = 0.;
  G4double Trigger_Box_PosiZ = 0.;
  G4ThreeVector Trigger_Box_Posi = G4ThreeVector(Trigger_Box_PosiX,
      Trigger_Box_PosiY,
      Trigger_Box_PosiZ);
  G4double Trigger_Box_Shell_Thickness = 5*mm; //이것은 내부 박스 길이를 정의하기 위함이다. 정작 쉘에는 딱히 사용하는 값은 아님

  //Trigger Box 껍데기 내부 박스
  //포지션은 쉘이랑 같으니까 박스 크기만 새로 정의하면 됨
  G4double Trigger_BoxX = Trigger_Box_ShellX - 2.*Trigger_Box_Shell_Thickness;
  G4double Trigger_BoxY = Trigger_Box_ShellY - 2.*Trigger_Box_Shell_Thickness;
  G4double Trigger_BoxZ = Trigger_Box_ShellZ - 2.*Trigger_Box_Shell_Thickness;

  //트리거 실린더 쉘은 트리거 박스 내의 실린더 껍질이다.
  G4double Cylinder_Shell_Thickness = 5.*mm; //두께는 모든 실린더가 같은 값을 가진다.
  G4double Trigger_Cylinder_Shell_OuterR = (139.8/2.)*mm;
  G4double Trigger_Cylinder_Shell_InnerR = 
    Trigger_Cylinder_Shell_OuterR - Cylinder_Shell_Thickness;
  G4double Trigger_Cylinder_ShellZ = Trigger_BoxZ; //(= 300*mm-Trigger_Box_Shell_Thickness = 290mm)
  G4ThreeVector Trigger_Cylinder_Posi = Trigger_Box_Posi;
  //실린더 포지션은 박스 포지션과 같다.
  //사실 이게 마더인 박스의 상대 좌표계로
  //적용되는지는 잘 모르겠다.
  //허나 중요한건 어차피 이건 그 중간에 있어야 한다.
  //따라서 Trigger_Box 포지션과 동등하게 대입한다.
  //쉘은 실린더 포지션과 동일해야 한다.

  G4double Trigger_Cylinder_OuterR = Trigger_Cylinder_Shell_InnerR;
  //항상 실린더는 실린더 쉘의 안쪽 반경을 바깥쪽 반경으로 가져야 딱 쉘에 둘러쌓인다.
  G4double Trigger_Cylinder_InnerR = 0.;
  G4double Trigger_CylinderZ = Trigger_Cylinder_ShellZ; //(= 300*mm, 또한 쉘의 z길이와도 같음)
  //사실 이 포지션은 실린더 쉘에서 먼저 정의되었다.
  //쉘이 먼저 정의되야 실린더의 outerR을 정의 가능하므로 어쩔수 없이 Trigger_Cylinder_Posi는 쉘부분에서 정의된다.

  //박스 쉘 부분의 빈 공간을 채우는 얇은 실린더,
  //우선 Z+ 즉 왼쪽 실린더와 그 쉘부터 정한다
  //먼저 쉘 파라미터부터 정한다
  G4double CylinderShell1_InBoxShell_OuterR = (139.8/2.)*mm;
  G4double CylinderShell1_InBoxShell_InnerR = 
    Trigger_Cylinder_Shell_OuterR - Cylinder_Shell_Thickness;
  G4double CylinderShell1_InBoxShellZ = Trigger_Box_Shell_Thickness; 

  G4double CylinderShell1_InBoxShell_PosiX = Trigger_Box_PosiX; //(=0.)
  G4double CylinderShell1_InBoxShell_PosiY = Trigger_Box_PosiY; //(=0.)
  G4double CylinderShell1_InBoxShell_PosiZ = 0.5*Trigger_BoxZ + 0.5*Trigger_Box_Shell_Thickness;
  G4ThreeVector CylinderShell1_InBoxShell_Posi
    = G4ThreeVector(CylinderShell1_InBoxShell_PosiX,
	 CylinderShell1_InBoxShell_PosiY,
	 CylinderShell1_InBoxShell_PosiZ);


  G4double Cylinder1_InBoxShell_OuterR = CylinderShell1_InBoxShell_InnerR;
  G4double Cylinder1_InBoxShell_InnerR = 0.;
  G4double Cylinder1_InBoxShellZ = CylinderShell1_InBoxShellZ;
  //(쉘의 z길이와 같아야 한다)
  G4ThreeVector Cylinder1_InBoxShell_Posi = CylinderShell1_InBoxShell_Posi;


  //오른쪽 얇은 쉘 및 실린더 정리
  G4double CylinderShell2_InBoxShell_OuterR = (139.8/2.)*mm;
  G4double CylinderShell2_InBoxShell_InnerR = 
    Trigger_Cylinder_Shell_OuterR - Cylinder_Shell_Thickness;
  G4double CylinderShell2_InBoxShellZ = Trigger_Box_Shell_Thickness; 

  G4double CylinderShell2_InBoxShell_PosiX = Trigger_Box_PosiX; //(=0.)
  G4double CylinderShell2_InBoxShell_PosiY = Trigger_Box_PosiY; //(=0.)
  G4double CylinderShell2_InBoxShell_PosiZ = -1.*CylinderShell1_InBoxShell_PosiZ;
  //CylinderShell1과 다른 점은 단 하나 바로 Z포지션이다

  G4ThreeVector CylinderShell2_InBoxShell_Posi
    = G4ThreeVector(CylinderShell2_InBoxShell_PosiX,
	 CylinderShell2_InBoxShell_PosiY,
	 CylinderShell2_InBoxShell_PosiZ);


  G4double Cylinder2_InBoxShell_OuterR = CylinderShell2_InBoxShell_InnerR;
  G4double Cylinder2_InBoxShell_InnerR = 0.;
  G4double Cylinder2_InBoxShellZ = CylinderShell2_InBoxShellZ;
  //(쉘의 z길이와 같아야 한다)
  G4ThreeVector Cylinder2_InBoxShell_Posi = CylinderShell2_InBoxShell_Posi;

  //////////////////////////////////////////////////////////////////////////
  //양쪽 실린더 정의 (1이 z의 양의 방향, vis화면에서 왼쪽이다.
  G4double Cylinder1_Shell_OuterR = Trigger_Cylinder_Shell_OuterR;
  // 실린더 반경은 다 같음(139.8/2.)*mm;
  G4double Cylinder1_Shell_InnerR = Trigger_Cylinder_Shell_InnerR;
  G4double Cylinder1_ShellZ = 1060.*mm+60.*mm; //=1120mm

  G4double Cylinder1_PosiX = 0.;
  G4double Cylinder1_PosiY = 0.;
  G4double Cylinder1_PosiZ = (0.5*Trigger_Box_ShellZ)
    + (0.5*Cylinder1_ShellZ);

  G4ThreeVector Cylinder1_Posi = G4ThreeVector(Cylinder1_PosiX,
      Cylinder1_PosiY,
      Cylinder1_PosiZ);
  //포지션은 쉘 없이 그냥 실린더 포지(Posi)로 정의하자

  G4double Cylinder1_OuterR = Cylinder1_Shell_InnerR ;
  // 실린더 반경은 다 같음(139.8/2.)*mm;
  G4double Cylinder1_InnerR = 0.; //내부 반경은 0이다.
  G4double Cylinder1Z = Cylinder1_ShellZ; //=1120mm //쉘과 실린더는 Z길이가 같다.

  //실린더 2, 모든 스펙이 같고 단지 포지션만 Z가 부호가 다를 뿐이다.
  G4double Cylinder2_Shell_OuterR = Trigger_Cylinder_Shell_OuterR;
  G4double Cylinder2_Shell_InnerR = Trigger_Cylinder_Shell_InnerR;
  G4double Cylinder2_ShellZ = 1060.*mm+60.*mm; //=1120mm

  G4double Cylinder2_PosiX = 0.;
  G4double Cylinder2_PosiY = 0.;
  G4double Cylinder2_PosiZ = -1.*Cylinder1_PosiZ; //얘만 부호 반대로 해준다

  G4ThreeVector Cylinder2_Posi = G4ThreeVector(Cylinder2_PosiX,
      Cylinder2_PosiY,
      Cylinder2_PosiZ);


  G4double Cylinder2_OuterR = Cylinder2_Shell_InnerR ;
  G4double Cylinder2_InnerR = 0.; 
  G4double Cylinder2Z = Cylinder2_ShellZ;

  //양쪽 ECAL cylinder 및 Crystal 배치
  //ECAL = Electromagnetic Calorimeter
  G4double ECAL1_Shell_OuterR = Trigger_Cylinder_Shell_OuterR;
  G4double ECAL1_Shell_InnerR = Trigger_Cylinder_Shell_InnerR;
  G4double ECAL1_Shell_Z = 200*mm;

  G4double ECAL1_PosiX = 0.;
  G4double ECAL1_PosiY = 0.;
  G4double ECAL1_PosiZ = (0.5*Trigger_Box_ShellZ)
    + (Cylinder1_ShellZ)
    + (0.5*ECAL1_Shell_Z);

  G4ThreeVector ECAL1_Posi = G4ThreeVector(ECAL1_PosiX,
      ECAL1_PosiY,
      ECAL1_PosiZ);

  G4double ECAL1_OuterR = ECAL1_Shell_InnerR ;
  G4double ECAL1_InnerR = 0.; 
  G4double ECAL1_Z = ECAL1_Shell_Z; //=200mm 

  G4double ECAL2_Shell_OuterR = Trigger_Cylinder_Shell_OuterR;
  G4double ECAL2_Shell_InnerR = Trigger_Cylinder_Shell_InnerR;
  G4double ECAL2_Shell_Z = 200*mm;

  G4double ECAL2_PosiX = 0.;
  G4double ECAL2_PosiY = 0.;
  G4double ECAL2_PosiZ = -1.*ECAL1_PosiZ;
  G4ThreeVector ECAL2_Posi = G4ThreeVector(ECAL2_PosiX,
      ECAL2_PosiY,
      ECAL2_PosiZ);

  G4double ECAL2_OuterR = ECAL2_Shell_InnerR ;
  G4double ECAL2_InnerR = 0.; 
  G4double ECAL2_Z = ECAL2_Shell_Z; //=200mm 


  /////////////////////////////////////////////////////////////////////////////
  //Shielding 들, Shielding1은 왼쪽 2는 오른쪽 Sheiding_Mirror는 중간이다.

  G4double Shielding_Mirror_OuterR = 830./2.*mm;
  G4double Shielding_Mirror_InnerR = 690./2. *mm; //쉴딩 두께는 7cm 따라서 직경은 140mm가 줄어든다.
  G4double Shielding_Mirror_Z = Trigger_Box_ShellZ + 60.*4.*mm; // = 300+60*4

  G4double Shielding_Mirror_PosiX = 0.;
  G4double Shielding_Mirror_PosiY = 0.;
  G4double Shielding_Mirror_PosiZ = 0.;

  G4ThreeVector Shielding_Mirror_Posi = G4ThreeVector(Shielding_Mirror_PosiX,
      Shielding_Mirror_PosiY,
      Shielding_Mirror_PosiZ);


  G4double Shielding1_OuterR = Shielding_Mirror_OuterR;
  G4double Shielding1_InnerR = Shielding_Mirror_InnerR; 
  G4double Shielding1_Z = 1000.*mm;

  G4double Shielding1_PosiX = 0.;
  G4double Shielding1_PosiY = 0.;
  G4double Shielding1_PosiZ = 0.5*Shielding1_Z + 0.5*Shielding_Mirror_Z;

  G4ThreeVector Shielding1_Posi = G4ThreeVector(Shielding1_PosiX,
      Shielding1_PosiY,
      Shielding1_PosiZ);


  G4double Shielding2_OuterR = Shielding_Mirror_OuterR;
  G4double Shielding2_InnerR = Shielding_Mirror_InnerR; 
  G4double Shielding2_Z = 1000.*mm;

  G4double Shielding2_PosiX = 0.;
  G4double Shielding2_PosiY = 0.;
  G4double Shielding2_PosiZ = -1.*Shielding1_PosiZ ;

  G4ThreeVector Shielding2_Posi = G4ThreeVector(Shielding2_PosiX,
      Shielding2_PosiY,
      Shielding2_PosiZ);

  //ECAL 부분에 있는 shielding들이다. 가장 바깥쪽에 있는 쉴딩이라고 생각하면 된다.
  G4double ShieldingECAL1_OuterR = Shielding_Mirror_OuterR; //ECAL 쉴딩도 중간 쉴딩이랑 다를것은 없다.
  G4double ShieldingECAL1_InnerR = Trigger_Cylinder_Shell_OuterR; //그러나 실린더 부분 쉴딩과는 달리 솔레노이드가 없어 실린더까지 감싼다.
  G4double ShieldingECAL1_Z = 70.*mm;

  G4double ShieldingECAL1_PosiX = 0.;
  G4double ShieldingECAL1_PosiY = 0.;
  G4double ShieldingECAL1_PosiZ = Shielding1_Z + 0.5*Shielding_Mirror_Z + 0.5*ShieldingECAL1_Z;

  G4ThreeVector ShieldingECAL1_Posi = G4ThreeVector(ShieldingECAL1_PosiX,
      ShieldingECAL1_PosiY,
      ShieldingECAL1_PosiZ);


  G4double ShieldingECAL2_OuterR = Shielding_Mirror_OuterR;
  G4double ShieldingECAL2_InnerR = Trigger_Cylinder_Shell_OuterR; 
  G4double ShieldingECAL2_Z = 70.*mm;

  G4double ShieldingECAL2_PosiX = 0.;
  G4double ShieldingECAL2_PosiY = 0.;
  G4double ShieldingECAL2_PosiZ = -1.*ShieldingECAL1_PosiZ ;

  G4ThreeVector ShieldingECAL2_Posi = G4ThreeVector(ShieldingECAL2_PosiX,
      ShieldingECAL2_PosiY,
      ShieldingECAL2_PosiZ);


  //솔레노이드들이다. 철로된 쉴딩에 감싸져 있으며 쉴딩 직경은 450~590이다.
  G4double Solenoid1_OuterR = Shielding1_InnerR; //=690mm (직경, 반지름 아님)
  G4double Solenoid1_InnerR = 200./2.*mm; //=200mm (직경, 마찬가지로 반지름 아님)
  G4double Solenoid1_Z = 1000.*mm;

  G4double Solenoid1_PosiX = 0.;
  G4double Solenoid1_PosiY = 0.;
  G4double Solenoid1_PosiZ = 0.5*Solenoid1_Z + 0.5*Shielding_Mirror_Z;

  G4ThreeVector Solenoid1_Posi = G4ThreeVector(Solenoid1_PosiX,
      Solenoid1_PosiY,
      Solenoid1_PosiZ);

  G4double Solenoid2_OuterR = Shielding2_InnerR;
  G4double Solenoid2_InnerR = 200./2.*mm; 
  G4double Solenoid2_Z = 1000.*mm;
  G4double Solenoid2_PosiX = 0.;
  G4double Solenoid2_PosiY = 0.;
  G4double Solenoid2_PosiZ = -1.*Solenoid1_PosiZ;

  G4ThreeVector Solenoid2_Posi = G4ThreeVector(Solenoid2_PosiX,
      Solenoid2_PosiY,
      Solenoid2_PosiZ);

  ///////////////////////////////////////////////////////////////////////
  // World
  //G4double world_sizeXY = 1.2*2.*EnvOuterR;
  //G4double world_sizeZ  = 1.2*EnvZ;
  //G4double world_sizeXY = 2.*Shielding_Mirror_OuterR*1.2; // 830(직경)/2 * 2 * 1.2 = 996*mm
  G4double world_sizeXY = 1200*mm;							  
  G4double world_sizeZ  = 3500*mm;


  fSolidWorld =    
    new G4Box("World",                       //its name
	 0.5*world_sizeXY , 0.5*world_sizeXY , 0.5*world_sizeZ );     //its size

  fLogicWorld =                         
    new G4LogicalVolume(fSolidWorld,          //its solid
	 fWorld_material,           //its material
	 "World");            //its name
  /*
  //color vis
  fVisAttrBlue = new G4VisAttributes(G4Colour(0., 0., 1.0, 1.0));
  fVisAttrBlue -> SetVisibility(true);
  fLogicWorld ->SetVisAttributes(fVisAttrBlue); 
  */

  fVisAttrTransparent = new G4VisAttributes();
  fVisAttrTransparent -> SetVisibility(false);
  fLogicWorld ->SetVisAttributes(fVisAttrTransparent); 


  fPhysWorld = 
    new G4PVPlacement(0,                     //no rotation
	 G4ThreeVector(),       //at (0,0,0)
	 fLogicWorld,            //its logical volume
	 "World",               //its name
	 0,                     //its mother  volume
	 false,                 //no boolean operation
	 0,                     //copy number
	 checkOverlaps);        //overlaps checking

  //World Env 월드 내부의 볼륨이다.
  fSolidWorldEnv =    
    new G4Box("WorldEnv",                       //its name
	 0.5*world_sizeXY - 10, 0.5*world_sizeXY - 10, 0.5*world_sizeZ - 10);     //its size

  fLogicWorldEnv =                         
    new G4LogicalVolume(fSolidWorldEnv,          //its solid
	 fWorld_material,           //its material
	 "WorldEnv");            //its name

  /*
     fVisAttrTransparent = new G4VisAttributes();
     fVisAttrTransparent -> SetVisibility(false);
     fLogicWorldEnv ->SetVisAttributes(fVisAttrTransparent); 
     */

  fPhysWorldEnv = 
    new G4PVPlacement(0,                     //no rotation
	 G4ThreeVector(),       //at (0,0,0)
	 fLogicWorldEnv,            //its logical volume
	 "WorldEnv",               //its name
	 fLogicWorld,                     //its mother  volume
	 false,                 //no boolean operation
	 0,                     //copy number
	 checkOverlaps);        //overlaps checking


  //여기까지 월드 및 트리거 부분 변수 선언 완료
  /////////////////////////////////////////////////////////////////
  //트리거 박스 쉘과 트리거 박스

  STrigger_Box_Shell = new G4Box("Trigger_Box_Shell", 0.5*Trigger_Box_ShellX,
      0.5*Trigger_Box_ShellY, 0.5*Trigger_Box_ShellZ);

  LTrigger_Box_Shell = new G4LogicalVolume(STrigger_Box_Shell, fTrigger_Box_Shell_material,"Trigger_Box_Shell");

  /*
     fVisAttrGreen = new G4VisAttributes(G4Colour(0.3, 0.7, 0.3));
     fVisAttrGreen -> SetVisibility(true);
     LTrigger_Box ->SetVisAttributes(fVisAttrGreen); 
     */

  PTrigger_Box_Shell = new G4PVPlacement(0, Trigger_Box_Posi, LTrigger_Box_Shell, "Trigger_Box_Shell",fLogicWorldEnv, false, 0, checkOverlaps); 

  //트리거 박스 쉘 내부 박스
  STrigger_Box = new G4Box("Trigger_Box", 0.5*Trigger_BoxX,
      0.5*Trigger_BoxY, 0.5*Trigger_BoxZ);

  LTrigger_Box = new G4LogicalVolume(STrigger_Box, fTrigger_Box_material,"Trigger_Box");

  PTrigger_Box = new G4PVPlacement(0, Trigger_Box_Posi, LTrigger_Box, "Trigger_Box"
      ,LTrigger_Box_Shell, false, 0, checkOverlaps); 
  //반드시 Trigger Box는 mother가 Shell이어야 한다.
  /////////////////////////////////////////////////////////////////////////////

  //트리거 실린더 시작
  /*
     STrigger_Cylinder_Shell = new G4Tubs("Trigger_Cylinder_Shell",
     Trigger_Cylinder_Shell_InnerR,
     Trigger_Cylinder_Shell_OuterR,
     Trigger_Cylinder_ShellZ/2.,
     0., 
     CLHEP::twopi);

     LTrigger_Cylinder_Shell = new G4LogicalVolume(STrigger_Cylinder_Shell, fCylinder_Shell_material,"Trigger_Cylinder_Shell");

     PTrigger_Cylinder_Shell = new G4PVPlacement(0, Trigger_Cylinder_Posi, LTrigger_Cylinder_Shell, "Trigger_Cylinder_Shell", LTrigger_Box, false, 0, checkOverlaps);


     STrigger_Cylinder = new G4Tubs("Trigger_Cylinder",
     Trigger_Cylinder_InnerR,
     Trigger_Cylinder_OuterR,
     Trigger_CylinderZ/2.,
     0., 
     CLHEP::twopi);

     LTrigger_Cylinder = new G4LogicalVolume(STrigger_Cylinder, fCylinder_material,"Trigger_Cylinder");

     PTrigger_Cylinder = new G4PVPlacement(0, Trigger_Cylinder_Posi, LTrigger_Cylinder, "Trigger_Cylinder", LTrigger_Box, false, 0, checkOverlaps);
  //마더는 Trigger_Box의 로지컬 볼륨이다.
  */

  //////////////////////////////////////////////////////////////////////

  //////////////////////////////////////////////////////////////////////////
  //BoxShell 을 채우는 얇은 실린더형 쉘과 실린더
  //우선 왼쪽 실린더와 쉘부터 정의

  SCylinderShell1_InBoxShell = new G4Tubs("CylinderShell1_InBoxShell",
      CylinderShell1_InBoxShell_InnerR,
      CylinderShell1_InBoxShell_OuterR,
      CylinderShell1_InBoxShellZ/2.,
      0., 
      CLHEP::twopi);

  LCylinderShell1_InBoxShell = new G4LogicalVolume(SCylinderShell1_InBoxShell , fCylinder_Shell_material,"CylinderShell1_InBoxShell");
  //실린더 쉘 메터리얼은 모든 실린더 공통이다

  PCylinderShell1_InBoxShell  = new G4PVPlacement(0, CylinderShell1_InBoxShell_Posi, LCylinderShell1_InBoxShell, "CylinderShell1_InBoxShell", LTrigger_Box_Shell, false, 0, checkOverlaps);
  //이 녀석은 Trigger_Box_Shell의 dauther인게 중요하다

  SCylinder1_InBoxShell = new G4Tubs("Cylinder1_InBoxShell",
      Cylinder1_InBoxShell_InnerR,
      Cylinder1_InBoxShell_OuterR,
      Cylinder1_InBoxShellZ/2.,
      0., 
      CLHEP::twopi);

  LCylinder1_InBoxShell = new G4LogicalVolume(SCylinder1_InBoxShell, fCylinder_material,"Cylinder1_InBoxShell" );

  PCylinder1_InBoxShell  = new G4PVPlacement(0, Cylinder1_InBoxShell_Posi, LCylinder1_InBoxShell , "Cylinder1_InBoxShell", LTrigger_Box_Shell, false, 0, checkOverlaps);


  //오른쪽 실린더 및 쉘 정리
  SCylinderShell2_InBoxShell = new G4Tubs("CylinderShell2_InBoxShell",
      CylinderShell2_InBoxShell_InnerR,
      CylinderShell2_InBoxShell_OuterR,
      CylinderShell2_InBoxShellZ/2.,
      0., 
      CLHEP::twopi);

  LCylinderShell2_InBoxShell = new G4LogicalVolume(SCylinderShell2_InBoxShell , fCylinder_Shell_material,"CylinderShell2_InBoxShell");
  //실린더 쉘 메터리얼은 모든 실린더 공통이다

  PCylinderShell2_InBoxShell  = new G4PVPlacement(0, CylinderShell2_InBoxShell_Posi, LCylinderShell2_InBoxShell, "CylinderShell2_InBoxShell", LTrigger_Box_Shell, false, 0, checkOverlaps);
  //이 녀석은 Trigger_Box_Shell의 dauther인게 중요하다

  SCylinder2_InBoxShell = new G4Tubs("Cylinder2_InBoxShell",
      Cylinder2_InBoxShell_InnerR,
      Cylinder2_InBoxShell_OuterR,
      Cylinder2_InBoxShellZ/2.,
      0., 
      CLHEP::twopi);

  LCylinder2_InBoxShell = new G4LogicalVolume(SCylinder2_InBoxShell, fCylinder_material,"Cylinder2_InBoxShell" );

  PCylinder2_InBoxShell  = new G4PVPlacement(0, Cylinder2_InBoxShell_Posi, LCylinder2_InBoxShell , "Cylinder2_InBoxShell ", LTrigger_Box_Shell, false, 0, checkOverlaps);



  /////////////////////////////////////////////////////////////////
  //이제부터 양쪽 실린더 정의
  SCylinder1_Shell = new G4Tubs("SCylinder1_Shell",
      Cylinder1_Shell_InnerR,
      Cylinder1_Shell_OuterR,
      Cylinder1_ShellZ/2.,
      0., 
      CLHEP::twopi);

  LCylinder1_Shell = new G4LogicalVolume(SCylinder1_Shell, fCylinder_Shell_material,"Cylinder1_Shell");

  PCylinder1_Shell = new G4PVPlacement(0, Cylinder1_Posi, LCylinder1_Shell, "Cylinder1_Shell", fLogicWorldEnv, false, 0, checkOverlaps);

  SCylinder1 = new G4Tubs("SCylinder1",
      Cylinder1_InnerR,
      Cylinder1_OuterR,
      Cylinder1Z/2.,
      0., 
      CLHEP::twopi);

  LCylinder1 = new G4LogicalVolume(SCylinder1, fCylinder_material,"Cylinder1");
  PCylinder1 = new G4PVPlacement(0, Cylinder1_Posi, LCylinder1, "Cylinder1", fLogicWorldEnv, false, 0, checkOverlaps);
  //포지션은 다시 말하지만 쉘이건 그냥 실린더건 같은 실린더 포지다.

  SCylinder2_Shell = new G4Tubs("SCylinder2_Shell",
      Cylinder2_Shell_InnerR,
      Cylinder2_Shell_OuterR,
      Cylinder2_ShellZ/2.,
      0., 
      CLHEP::twopi);

  LCylinder2_Shell = new G4LogicalVolume(SCylinder2_Shell, fCylinder_Shell_material,"Cylinder2_Shell");

  PCylinder2_Shell = new G4PVPlacement(0, Cylinder2_Posi, LCylinder2_Shell, "Cylinder2_Shell", fLogicWorldEnv, false, 0, checkOverlaps);

  SCylinder2 = new G4Tubs("SCylinder2",
      Cylinder2_InnerR,
      Cylinder2_OuterR,
      Cylinder2Z/2.,
      0., 
      CLHEP::twopi);

  LCylinder2 = new G4LogicalVolume(SCylinder2, fCylinder_material,"Cylinder2");
  PCylinder2 = new G4PVPlacement(0, Cylinder2_Posi, LCylinder2, "Cylinder2", fLogicWorldEnv, false, 0, checkOverlaps);


  //ECAL
  SECAL1_Shell = new G4Tubs("ECAL1_Shell",
      ECAL1_Shell_InnerR,
      ECAL1_Shell_OuterR,
      ECAL1_Shell_Z/2.,
      0., 
      CLHEP::twopi);

  LECAL1_Shell = new G4LogicalVolume(SECAL1_Shell, fECAL_Shell_material,"ECAL1_Shell");

  PECAL1_Shell = new G4PVPlacement(0, ECAL1_Posi, LECAL1_Shell, "ECAL1_Shell", fLogicWorldEnv, false, 0, checkOverlaps);

  SECAL1 = new G4Tubs("ECAL1",
      ECAL1_InnerR,
      ECAL1_OuterR,
      ECAL1_Z/2.,
      0., 
      CLHEP::twopi);

  LECAL1 = new G4LogicalVolume(SECAL1, fECAL_material,"ECAL1");

  PECAL1 = new G4PVPlacement(0, ECAL1_Posi, LECAL1, "ECAL1", fLogicWorldEnv, false, 0, checkOverlaps);



  SECAL2_Shell = new G4Tubs("ECAL2_Shell",
      ECAL2_Shell_InnerR,
      ECAL2_Shell_OuterR,
      ECAL2_Shell_Z/2.,
      0., 
      CLHEP::twopi);

  LECAL2_Shell = new G4LogicalVolume(SECAL2_Shell, fECAL_Shell_material,"ECAL2_Shell");

  PECAL2_Shell = new G4PVPlacement(0, ECAL2_Posi, LECAL2_Shell, "ECAL2_Shell", fLogicWorldEnv, false, 0, checkOverlaps);

  SECAL2 = new G4Tubs("ECAL2",
      ECAL2_InnerR,
      ECAL2_OuterR,
      ECAL2_Z/2.,
      0., 
      CLHEP::twopi);

  LECAL2 = new G4LogicalVolume(SECAL2, fECAL_material,"ECAL2");

  PECAL2 = new G4PVPlacement(0, ECAL2_Posi, LECAL2, "ECAL2", fLogicWorldEnv, false, 0, checkOverlaps);

  //////////////////////////////////////////////////////////////////////
  //Shielding and Solenoid 배치

  //Mirror부분은 Solenoid가 없고 Shielding만 존재한다.
  //Shielding은 철로 되어 있으며 직경59cm~82cm의 실린더다.
  SShielding_Mirror = new G4Tubs("Shielding_Mirror",
      Shielding_Mirror_InnerR,
      Shielding_Mirror_OuterR,
      Shielding_Mirror_Z/2.,
      0., 
      CLHEP::twopi);

  LShielding_Mirror = new G4LogicalVolume(SShielding_Mirror, fShielding_Mirror_material,"Shielding_Mirror");


  PShielding_Mirror = new G4PVPlacement(0, Shielding_Mirror_Posi, LShielding_Mirror, "Shielding_Mirror", fLogicWorldEnv, false, 0, checkOverlaps);

  //왼쪽 솔레노이드 쉴딩
  SShielding1 = new G4Tubs("Shielding1",
      Shielding1_InnerR,
      Shielding1_OuterR,
      Shielding1_Z/2.,
      0., 
      CLHEP::twopi);

  LShielding1 = new G4LogicalVolume(SShielding1, fShielding1_material,"Shielding1");


  PShielding1 = new G4PVPlacement(0, Shielding1_Posi, LShielding1, "Shielding1", fLogicWorldEnv, false, 0, checkOverlaps);


  //오른쪽 솔레노이드 쉴딩
  SShielding2 = new G4Tubs("Shielding2",
      Shielding2_InnerR,
      Shielding2_OuterR,
      Shielding2_Z/2.,
      0., 
      CLHEP::twopi);

  LShielding2 = new G4LogicalVolume(SShielding2, fShielding2_material,"Shielding2");


  PShielding2 = new G4PVPlacement(0, Shielding2_Posi, LShielding2, "Shielding2", fLogicWorldEnv, false, 0, checkOverlaps);



  //왼쪽 솔레노이드
  SSolenoid1 = new G4Tubs("Solenoid1",
      Solenoid1_InnerR,
      Solenoid1_OuterR,
      Solenoid1_Z/2.,
      0., 
      CLHEP::twopi);

  LSolenoid1 = new G4LogicalVolume(SSolenoid1, fSolenoid1_material,"Solenoid1");

  PSolenoid1 = new G4PVPlacement(0, Solenoid1_Posi, LSolenoid1, "Solenoid1", fLogicWorldEnv, false, 0, checkOverlaps);


  SSolenoid2 = new G4Tubs("Solenoid2",
      Solenoid2_InnerR,
      Solenoid2_OuterR,
      Solenoid2_Z/2.,
      0., 
      CLHEP::twopi);

  LSolenoid2 = new G4LogicalVolume(SSolenoid2, fSolenoid2_material,"Solenoid2");

  PSolenoid2 = new G4PVPlacement(0, Solenoid2_Posi, LSolenoid2, "Solenoid2", fLogicWorldEnv, false, 0, checkOverlaps);
  //////////////////////////////////////////////////////////////////////////
  //ECAL 왼쪽 쉴딩
  SShieldingECAL1 = new G4Tubs("ShieldingECAL1",
      ShieldingECAL1_InnerR,
      ShieldingECAL1_OuterR,
      ShieldingECAL1_Z/2.,
      0., 
      CLHEP::twopi);

  LShieldingECAL1 = new G4LogicalVolume(SShieldingECAL1, fShielding1_material,"ShieldingECAL1");


  PShieldingECAL1 = new G4PVPlacement(0, ShieldingECAL1_Posi, LShieldingECAL1, "ShieldingECAL1", fLogicWorldEnv, false, 0, checkOverlaps);


  //ECAL 오른쪽 쉴딩
  SShieldingECAL2 = new G4Tubs("ShieldingECAL2",
      ShieldingECAL2_InnerR,
      ShieldingECAL2_OuterR,
      ShieldingECAL2_Z/2.,
      0., 
      CLHEP::twopi);

  LShieldingECAL2 = new G4LogicalVolume(SShieldingECAL2, fShielding2_material,"ShieldingECAL2");


  PShieldingECAL2 = new G4PVPlacement(0, ShieldingECAL2_Posi, LShieldingECAL2, "ShieldingECAL2", fLogicWorldEnv, false, 0, checkOverlaps);



  ////////////////////////////////////////////////////////////////////////
  //Shielding and Solenoid vis
  fVisAttrShielding_Mirror = new G4VisAttributes(G4Colour(1.0, 1.0, 0.0, 0.6));
  fVisAttrShielding_Mirror -> SetVisibility(true);
  LShielding_Mirror ->SetVisAttributes(fVisAttrShielding_Mirror); 
  //LShielding_Mirror ->SetVisAttributes(fVisAttrTransparent); 

  fVisAttrShielding1 = new G4VisAttributes(G4Colour(1.0, 1.0, 0.0, 0.6));
  fVisAttrShielding1 -> SetVisibility(true);
  LShielding1 ->SetVisAttributes(fVisAttrShielding1); 
  //LShielding1 ->SetVisAttributes(fVisAttrTransparent); 

  fVisAttrShielding2 = new G4VisAttributes(G4Colour(1.0, 1.0, 0.0, 0.6));
  fVisAttrShielding2 -> SetVisibility(true);
  LShielding2 ->SetVisAttributes(fVisAttrShielding2); 
  //LShielding2 ->SetVisAttributes(fVisAttrTransparent); 

  fVisAttrSolenoid1 = new G4VisAttributes(G4Colour(1.0, 0.0, 1.0, 0.3));
  fVisAttrSolenoid1 -> SetVisibility(true);
  LSolenoid1 ->SetVisAttributes(fVisAttrSolenoid1); 
  //LSolenoid1 ->SetVisAttributes(fVisAttrTransparent); 

  fVisAttrSolenoid2 = new G4VisAttributes(G4Colour(1.0, 0.0, 1.0, 0.3));
  fVisAttrSolenoid2 -> SetVisibility(true);
  LSolenoid2 ->SetVisAttributes(fVisAttrSolenoid2); 
  //LSolenoid2 ->SetVisAttributes(fVisAttrTransparent); 

  fVisAttrShieldingECAL1 = new G4VisAttributes(G4Colour(1.0, 1.0, 0.0, 0.6));
  fVisAttrShieldingECAL1 -> SetVisibility(true);
  LShieldingECAL1 ->SetVisAttributes(fVisAttrShieldingECAL1); 
  //LShieldingECAL1 ->SetVisAttributes(fVisAttrTransparent); 

  fVisAttrShieldingECAL2 = new G4VisAttributes(G4Colour(1.0, 1.0, 0.0, 0.6));
  fVisAttrShieldingECAL2 -> SetVisibility(true);
  LShieldingECAL2 ->SetVisAttributes(fVisAttrShieldingECAL2); 
  //LShieldingECAL2 ->SetVisAttributes(fVisAttrTransparent); 


  //////////////////////////////////////////////////////////////////////////
  //LYSO 배치 우선 상대좌표로 생각하고 배치하자
  G4double r =  ECAL1_OuterR; //Cylinder의 내부 R( = 139.8에서 Thick를 빼고 /2)
  G4double AlThick = 0.01*mm;
  G4double SqL = (10 + 0.02)*mm + 2*AlThick; // SqL(사실 사각형 길이라는 뜻
  // = Crystal length (x, y축 방향 길이)
  // 이것이 알루미늄 호일의 두께다

  G4double SqL_Size = 10 *mm; 

  G4double AirThick = 0.01*mm;
  G4double AirCapL = (SqL_Size + 2*AirThick); 
  //얘는 XY포지션과 길이에 사용됨, z길이는 따로 함

  G4double CenterCirX = ECAL1_PosiX; 
  G4double CenterCirY = ECAL1_PosiY;
  //ECAL의 중심 X좌표가 라이소 xy평면 원의 중심. 값은 0이다.
  //사실상 Circle의 중심을 기반으로 Crystal의 x, y 포지션을 잡는 것이다.

  G4double Crystals_Z = 60*mm;
  G4double AirThick_Z = AirThick; //만약 z쪽으로 감싼다면
  //SiPM 반대쪽 방향만 감싸지게 되어야 한다.(어차피 다른 쪽은 SiPM기능상 감싸면 안됨)
  //따라서 Thick*1만큼만 z축 길이가 됨, 만약 안감싸면 0으로 하면 끝
  G4double AirCap_Z = Crystals_Z + AirThick_Z;

  G4double AlThick_Z = AlThick;
  G4double Al_Z = AirCap_Z + AlThick;
  //G4double Crystals1_PosiZ = (0.5*Crystal_Z + Cylinder1Z + 0.5*Trigger_BoxZ);
  //지금 위에 저걸로 하면 생성 안됨, 마더가 ECAL이라 이것의 상대 좌표계로 해야하나봄
  G4double Crystals1_PosiZ = -(0.5*ECAL1_Z - 0.5*Al_Z);
  G4double Crystals2_PosiZ = -1.*Crystals1_PosiZ;

  //Crystal은ECAL을 마더 볼륨으로 할 것이다.  
  //x, y포지션은 계산을 통해 구할 것이므로 Z position만 주면 된다.
  //이거 나중에 상대 좌표계로 시도해 보자. mother를 ECAL로 두고
  //현재는 상대 좌표계로 되어 있는 셈이다.

  G4double Nof_halfColumn = floor( (r-SqL/2.) / SqL ) + 1; 
  //열의 개수(위 조건의 경우 20)
  //G4double Nof_AllColumn = 2*Nof_halfColumn-1; 
  //중간 배치에 따라 바로 위 두줄이 바뀌는 셈이다.

  G4double x, y = 0 *m;
  G4int count_Sq = 1;
  //G4int Allcount = 0;

  G4Box* SSq = new G4Box("SSq", 0.5*SqL, 0.5*SqL, 0.5*Al_Z);

  G4LogicalVolume* LSq = new G4LogicalVolume(SSq, fAl, "LYSOsquare");

  //LYSO Color
  //fVisAttrSq = new G4VisAttributes(G4Colour(0., 1.0, 1.0));
  fVisAttrSq = new G4VisAttributes(G4Colour(0., 0., 1.0));
  //원래 LYSO는 trigger꺼랑 전부 청록색인데 트리거 부분 자기장 비교 위해 잠시 다른색 변경
  //청록색은 오직 트리거 라이소 뿐이다.
  fVisAttrSq -> SetVisibility(true);
  LSq ->SetVisAttributes(fVisAttrSq); 


  //-19행~-1행, 중간에 0행, 원점에 원을 배치할 경우 양수쪽 1~19행
  for(G4double i = -Nof_halfColumn+1 ; i < Nof_halfColumn; i++ )
  {
    if (i<0)
    {y = -SqL/2. + i*SqL;}
    else
    {y = SqL/2. + i*SqL;}
    x = -sqrt(r*r - y*y); 

    SpaceOfCircle = SpaceFinder(x, y, SqL, r);

    while(1){
      CenterFinder(x+(SpaceOfCircle/2.), y, SqL, i); 
      //원 쌓기에는 i가 안쓰이지만
      //나중에 cout으로 버그 찾을 때를 위해 넣어둠
      new G4PVPlacement(0,
	   G4ThreeVector(SqCenter_x + CenterCirX,
	     SqCenter_y + CenterCirY,
	     Crystals1_PosiZ),
	   LSq,
	   "sq1",
	   LECAL1,
	   false,
	   count_Sq,  //반복문을 보면 맨 아래쪽의 왼쪽부터 채워나가며 그것부터 1번으로 한다.
	   checkOverlaps); 

      x = x + SqL;
      count_Sq++;

      if( sqrt(r*r - y*y) < x + SqL) //반복문 끊기
      {
	 //G4cout<<"Number of sq in this line = "<<count_in<<G4endl;
	 break;
      }

    }//LYSO while문 끝
  }//LYSO for문 끝
  G4LogicalSkinSurface*SurfaceAl1 = new G4LogicalSkinSurface("SurfaceAl1", LSq, fAl_Surf);

  ///////////////////////////////////////////////////////////////////////////////
  //Air cap
  //Inner LYSO(진짜 라이소)를 둘러싸는 알루미늄 포일 사이의 공기층

  G4Box* SAirCap1 = new G4Box("SAirCap1", 0.5*AirCapL, 0.5*AirCapL, 0.5*AirCap_Z);

  G4LogicalVolume* LAirCap1 = new G4LogicalVolume(SAirCap1, fAir, "AirCap1");

  new G4PVPlacement(0,
      G4ThreeVector(0,
	 0,
	 AlThick/2.), //이거 mother인 Al포일(Sq)의 중심과 차이를 만드는 것 자체가
      //AirCap이 아닌 Al의 douther와의 길이 차이가 만드는 것임
      //그래서 AlThick_Z/2 를 더해주는 것이다. 
      LAirCap1,
      "AirCap1",
      LSq,
      false,
      0,  
      checkOverlaps); 


  //Inner LYSO1
  //이러한 안쪽 라이소는 진공인 겉면 라이소가 빈 공간 없이 볼륨을 구성하는데 반해
  //0.001mm만큼 작은 실제 라이소 물질이 채워진 볼륨이다.
  //이것을 통해 내부 반사시에도 바깥 라이소를 넘지 않을 것이며 이를 이용, 라이소에 진입하는 파티클을
  //볼륨 네임을 통해 구분할 수 있게 된다.
  G4Box* SInSq = new G4Box("SInSq", 0.5*SqL_Size, 0.5*SqL_Size, 0.5*(Crystals_Z));

  G4LogicalVolume* LInSq = new G4LogicalVolume(SInSq, fSq_material, "InnerLYSOsquare");

  new G4PVPlacement(0,
      G4ThreeVector(0,
	 0,
	 0),
      LInSq,
      "Insq1",
      LAirCap1,
      false,
      0,  
      checkOverlaps); 


  /////////////////////////////////////////////////////////////////////////////////////////
  //LYSO 오른쪽
  G4Box* SSq2 = new G4Box("SSq2", 0.5*SqL, 0.5*SqL, 0.5*Al_Z);
  //이것은 사실상 Al포일 볼륨이다.  

  G4LogicalVolume* LSq2 = new G4LogicalVolume(SSq2, fAl, "LYSOsquare2");

  LSq2 ->SetVisAttributes(fVisAttrSq); 
  //음의 z좌표축에 있는 즉 ECAL2의 Crystal도 색깔을 바꿔준다.

  count_Sq = 1;
  //-19행~-1행, 중간에 0행, 원점에 원을 배치할 경우 양수쪽 1~19행
  for(G4double i = -Nof_halfColumn+1 ; i < Nof_halfColumn; i++ )
  {
    if (i<0)
    {y = -SqL/2. + i*SqL;}
    else
    {y = SqL/2. + i*SqL;}
    x = -sqrt(r*r - y*y); 

    SpaceOfCircle = SpaceFinder(x, y, SqL, r);

    while(1){
      CenterFinder(x+(SpaceOfCircle/2.), y, SqL, i);  
      //원 쌓기에는 i가 안쓰이지만
      //나중에 cout으로 버그 찾을 때를 위해 넣어둠
      new G4PVPlacement(0,
	   G4ThreeVector(SqCenter_x + CenterCirX,
	     SqCenter_y + CenterCirY,
	     Crystals2_PosiZ),
	   LSq2,
	   "sq2",
	   LECAL2,
	   false,
	   count_Sq, 
	   checkOverlaps); 

      x = x + SqL;
      count_Sq++; 

      if( sqrt(r*r - y*y) < x + SqL) //반복문 끊기
      {
	 //G4cout<<"Number of sq in this line = "<<count_Sq_in<<G4endl;
	 break;
      }

      //else
      //{count_Sq++;} //반복 끊기의 조건이 되지 않으면 count_Sq를 더함
    }//LYSO while문 끝
  }//LYSO for문 끝
  G4LogicalSkinSurface*SurfaceAl2 = new G4LogicalSkinSurface("SurfaceAl2", LSq2, fAl_Surf);

  ///////////////////////////////////////////////////////////////////////////////
  //Air cap2
  //Inner LYSO(진짜 라이소)를 둘러싸는 알루미늄 포일 사이의 공기층

  G4Box* SAirCap2 = new G4Box("SAirCap2", 0.5*AirCapL, 0.5*AirCapL, 0.5*AirCap_Z);

  G4LogicalVolume* LAirCap2 = new G4LogicalVolume(SAirCap2, fAir, "AirCap2");

  new G4PVPlacement(0,
      G4ThreeVector(0,
	 0,
	 -AlThick/2.), //이거 mother인 Al포일(Sq)의 중심과 차이를 만드는 것 자체가
      //AirCap이 아닌 Al의 douther와의 길이 차이가 만드는 것임
      //그래서 AlThick_Z/2 를 더해주는 것이다. 
      LAirCap2,
      "AirCap2",
      LSq2,
      false,
      0,  
      checkOverlaps); 



  //Inner LYSO2
  G4Box* SInSq2 = new G4Box("SInSq2", 0.5*SqL_Size, 0.5*SqL_Size, 0.5*(Crystals_Z));

  G4LogicalVolume* LInSq2 = new G4LogicalVolume(SInSq2, fSq_material, "InnerLYSOsquare2");

  new G4PVPlacement(0,
      G4ThreeVector(0,
	 0,
	 0),
      LInSq2,
      "Insq2",
      LAirCap2,
      false,
      0,  
      checkOverlaps); 



  //////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////
  //LYSO 옆 SiPM 역할을 할 Sensitive detector volume들 선언
  //어차피 LYSO와 xy포지션은 동일하며 z축과 z길이만 바꾸면 되므로 내용은 거의 동일하다.
  //우선 SiPM만의 파라미터를 정하자
  G4double SiPM_Z = 0.3 *mm; //사이즈는 dual-readout에서 가져옴
  G4double SiPMFront_Z = 0.1 *mm; //사이즈는 dual-readout에서 가져옴
  //Front size가 SiPM의 포지션에도 영향을 주므로 이 크기를 여기다 먼저 정의해 준다.

  //G4double SiPM_Z = 100 *mm;
  //G4double Crystals1_PosiZ = (0.5*Crystal_Z + Cylinder1Z + 0.5*Trigger_BoxZ);
  //지금 위에 저걸로 하면 생성 안됨, 마더가 ECAL이라 이것의 상대 좌표계로 해야하나봄
  G4double SiPM1_PosiZ = -(0.5*ECAL1_Z - Al_Z - 0.5*SiPM_Z - SiPMFront_Z);
  //왼쪽 기준 ECAL의 중심이 0이면 오른쪽은 +임. 즉 오른쪽으로 0.5*ECAL1_Z만큼 가면
  //왼쪽 ECAL의 오른쪽 끝이 되며 -Crystals_Z를 하면 다시 왼쪽으로 가는 것이며 그 결과는 LYSO의 왼쪽 끝 좌표다
  //여기서 0.5*SiPM_Z만큼 반틈 더 가야 SiPM의 Z포지션이 될 것이다.

  G4double SiPM2_PosiZ = -1.*SiPM1_PosiZ;

  //////////////////////////////////////////////////////////////////////////

  SSiPM1 = new G4Box("SSiPM1", 0.5*SqL, 0.5*SqL, 0.5*SiPM_Z);
  LSiPM1 = new G4LogicalVolume(SSiPM1, fSiPM_material, "LSiPM1");

  fVisAttrSiPM1 = new G4VisAttributes(G4Colour(1.0, 0.0, 0.0));
  fVisAttrSiPM1 -> SetVisibility(true);
  LSiPM1 ->SetVisAttributes(fVisAttrSiPM1); 

  count_Sq = 1;
  //-19행~-1행, 중간에 0행, 원점에 원을 배치할 경우 양수쪽 1~19행
  for(G4double i = -Nof_halfColumn+1 ; i < Nof_halfColumn; i++ )
  {
    if (i<0)
    {y = -SqL/2. + i*SqL;}
    else
    {y = SqL/2. + i*SqL;}
    x = -sqrt(r*r - y*y); 

    SpaceOfCircle = SpaceFinder(x, y, SqL, r);

    while(1){
      CenterFinder(x+(SpaceOfCircle/2.), y, SqL, i); 
      //원 쌓기에는 i가 안쓰이지만
      //나중에 cout으로 버그 찾을 때를 위해 넣어둠
      new G4PVPlacement(0,
	   G4ThreeVector(SqCenter_x + CenterCirX,
	     SqCenter_y + CenterCirY,
	     SiPM1_PosiZ),
	   LSiPM1,
	   "SiPM1",
	   LECAL1,
	   false,
	   count_Sq,  //반복문을 보면 맨 아래쪽의 왼쪽부터 채워나가며 그것부터 1번으로 한다.
	   checkOverlaps); 

      x = x + SqL;
      count_Sq++; //반복 끊기의 조건이 되지 않으면 count_Sq를 더함

      if( sqrt(r*r - y*y) < x + SqL) //반복문 끊기
      {
	 //G4cout<<"Number of sq in this line = "<<count_in<<G4endl;
	 break;
      }

    }//SiPM while문 끝
  }//SiPM for문 끝


  //오른쪽 sipm 선언
  SSiPM2 = new G4Box("SSiPM2", 0.5*SqL, 0.5*SqL, 0.5*SiPM_Z);
  LSiPM2 = new G4LogicalVolume(SSiPM2, fSiPM_material, "LSiPM2");

  fVisAttrSiPM2 = new G4VisAttributes(G4Colour(1.0, 0.0, 0.0));
  fVisAttrSiPM2 -> SetVisibility(true);
  LSiPM2 ->SetVisAttributes(fVisAttrSiPM2); 

  count_Sq = 1;
  //-19행~-1행, 중간에 0행, 원점에 원을 배치할 경우 양수쪽 1~19행
  for(G4double i = -Nof_halfColumn+1 ; i < Nof_halfColumn; i++ )
  {
    if (i<0)
    {y = -SqL/2. + i*SqL;}
    else
    {y = SqL/2. + i*SqL;}
    x = -sqrt(r*r - y*y); 

    SpaceOfCircle = SpaceFinder(x, y, SqL, r);

    while(1){
      CenterFinder(x+(SpaceOfCircle/2.), y, SqL, i); 
      //원 쌓기에는 i가 안쓰이지만
      //나중에 cout으로 버그 찾을 때를 위해 넣어둠
      new G4PVPlacement(0,
	   G4ThreeVector(SqCenter_x + CenterCirX,
	     SqCenter_y + CenterCirY,
	     SiPM2_PosiZ),
	   LSiPM2,
	   "SiPM2",
	   LECAL2,
	   false,
	   count_Sq,  //반복문을 보면 맨 아래쪽의 왼쪽부터 채워나가며 그것부터 1번으로 한다.
	   checkOverlaps); 

      x = x + SqL;
      count_Sq++; //반복 끊기의 조건이 되지 않으면 count_Sq를 더함


      if( sqrt(r*r - y*y) < x + SqL) //반복문 끊기
      {
	 //G4cout<<"Number of sq in this line = "<<count_in<<G4endl;
	 break;
      }

    }//SiPM while문 끝
  }//SiPM for문 끝


  //////////////////////////////////////////////////////////////////////////
  //SiPM 앞의 Front SiPM을 만든다. 이것은 실제 SiPM이며 사실 그냥 SiPM은 이것애 Volume을 만들어
  //Step을 생성하기 위한 Galactic한 물질의 수단에 불과하다
  //파라미터를 정하자. SiPM이 0.3mm이므로 이것의 size는 실제 SiPM인 0.1mm로 설정한다.
  //G4double SiPMFront_Z = 0.1 *mm; //사이즈는 dual-readout에서 가져옴
  //사실 SiPMFront_Z 사이즈가 SiPM의포지션에도 영향을 주므로 SiPM쪽에 위 변수를 정의해 주었다.

  G4double SiPMFront1_PosiZ = -(0.5*ECAL1_Z - Al_Z - 0.5*SiPMFront_Z);
  //왼쪽 기준 ECAL의 중심이 0이면 오른쪽은 +임. 즉 오른쪽으로 0.5*ECAL1_Z만큼 가면
  //왼쪽 ECAL의 오른쪽 끝이 되며 -Crystals_Z를 하면 다시 왼쪽으로 가는 것이며 그 결과는 LYSO의 왼쪽 끝 좌표다
  //여기서 0.5*SiPMFront_Z만큼 반틈 더 가야 SiPMFront의 Z포지션이 될 것이다.

  G4double SiPMFront2_PosiZ = -1.*SiPMFront1_PosiZ;

  //////////////////////////////////////////////////////////////////////////

  SSiPMFront1 = new G4Box("SSiPMFront1", 0.5*SqL, 0.5*SqL, 0.5*SiPMFront_Z);
  LSiPMFront1 = new G4LogicalVolume(SSiPMFront1, fSiPMFront_material, "LSiPMFront1");

  fVisAttrSiPMFront1 = new G4VisAttributes(G4Colour(1.0, 0.0, 0.0));
  fVisAttrSiPMFront1 -> SetVisibility(true);
  LSiPMFront1 ->SetVisAttributes(fVisAttrSiPMFront1); 

  count_Sq = 1;
  //-19행~-1행, 중간에 0행, 원점에 원을 배치할 경우 양수쪽 1~19행
  for(G4double i = -Nof_halfColumn+1 ; i < Nof_halfColumn; i++ )
  {
    if (i<0)
    {y = -SqL/2. + i*SqL;}
    else
    {y = SqL/2. + i*SqL;}
    x = -sqrt(r*r - y*y); 

    SpaceOfCircle = SpaceFinder(x, y, SqL, r);

    while(1){
      CenterFinder(x+(SpaceOfCircle/2.), y, SqL, i); 
      //원 쌓기에는 i가 안쓰이지만
      //나중에 cout으로 버그 찾을 때를 위해 넣어둠
      new G4PVPlacement(0,
	   G4ThreeVector(SqCenter_x + CenterCirX,
	     SqCenter_y + CenterCirY,
	     SiPMFront1_PosiZ),
	   LSiPMFront1,
	   "SiPMFront1",
	   LECAL1,
	   false,
	   count_Sq,  //반복문을 보면 맨 아래쪽의 왼쪽부터 채워나가며 그것부터 1번으로 한다.
	   checkOverlaps); 

      x = x + SqL;
      count_Sq++; //반복 끊기의 조건이 되지 않으면 count_Sq를 더함

      if( sqrt(r*r - y*y) < x + SqL) //반복문 끊기
      {
	 //G4cout<<"Number of sq in this line = "<<count_in<<G4endl;
	 break;
      }

    }//SiPMFront while문 끝
  }//SiPMFront for문 끝

  //SiPMFront1의 Surface를 설정해 준다.
  SurfaceSiPMFront1 = new G4LogicalSkinSurface("SiPMFront1Surf", LSiPMFront1, fSiPMFront_Surf);


  //오른쪽 SiPMFront 선언
  SSiPMFront2 = new G4Box("SSiPMFront2", 0.5*SqL, 0.5*SqL, 0.5*SiPMFront_Z);
  LSiPMFront2 = new G4LogicalVolume(SSiPMFront2, fSiPMFront_material, "LSiPMFront2");

  fVisAttrSiPMFront2 = new G4VisAttributes(G4Colour(1.0, 0.0, 0.0));
  fVisAttrSiPMFront2 -> SetVisibility(true);
  LSiPMFront2 ->SetVisAttributes(fVisAttrSiPMFront2); 

  count_Sq = 1;
  //-19행~-1행, 중간에 0행, 원점에 원을 배치할 경우 양수쪽 1~19행
  for(G4double i = -Nof_halfColumn+1 ; i < Nof_halfColumn; i++ )
  {
    if (i<0)
    {y = -SqL/2. + i*SqL;}
    else
    {y = SqL/2. + i*SqL;}
    x = -sqrt(r*r - y*y); 

    SpaceOfCircle = SpaceFinder(x, y, SqL, r);

    while(1){
      CenterFinder(x+(SpaceOfCircle/2.), y, SqL, i); 
      //원 쌓기에는 i가 안쓰이지만
      //나중에 cout으로 버그 찾을 때를 위해 넣어둠
      new G4PVPlacement(0,
	   G4ThreeVector(SqCenter_x + CenterCirX,
	     SqCenter_y + CenterCirY,
	     SiPMFront2_PosiZ),
	   LSiPMFront2,
	   "SiPMFront2",
	   LECAL2,
	   false,
	   count_Sq,  //반복문을 보면 맨 아래쪽의 왼쪽부터 채워나가며 그것부터 1번으로 한다.
	   checkOverlaps); 

      x = x + SqL;
      count_Sq++; //반복 끊기의 조건이 되지 않으면 count_Sq를 더함

      if( sqrt(r*r - y*y) < x + SqL) //반복문 끊기
      {
	 //G4cout<<"Number of sq in this line = "<<count_in<<G4endl;
	 break;
      }

    }//SiPMFront while문 끝
  }//SiPMFront for문 끝

  //SiPMFront2의 Surface를 설정해 준다.
  SurfaceSiPMFront2 = new G4LogicalSkinSurface("SiPMFront2Surf", LSiPMFront2, fSiPMFront_Surf);


  /////////////////////////////////////////////////////////////////////////
  //Trigger_Veto 설치
  G4double VetoAirCap_Thick = 0.01*mm;
  G4double Al_Thick = 0.01*mm;
  //Al 두께가 0.01

  G4double VetoXY_Size = 20.*mm; //0.1 마이크론만큼 VetoXYL과 차이가 이유는 LYSO 사이의 간격을 만들기 위해서다. 실제로 간격이 있을 것이며 굴절을 안에서 일으키기 위해서기도 하다
  G4double VetoAirCap_XYL = (VetoXY_Size + 2*VetoAirCap_Thick)*mm;
  G4double VetoXYL = (VetoAirCap_XYL + 2*Al_Thick)*mm; //Trigger_veto의 사이즈는 2cm*2cm*12cm다. 사실상 이건 Al의 size임
  G4double VetoZL = 120.*mm;
  G4double VetoAl_Z = 120.*mm;//이 방향으론 안 감쌀 것임, 뭣보다 얘는 여기 z축방향으론 sipm달려있음
  G4double VetoAirCap_Z = 120.*mm;
  G4double Trigger_Veto_Box_PosiZ = 0.;
  G4int Count_Veto = 0;
  G4int Veto_number = 0; //위의 Count_Veto는 11*11의 박스 어레이에서 빈 부분을 제거하기 위한 number를 부여하는 변수라면 이 Veto_number는 각 LYSO에 넘버를 순서대로 부여하기 위한 변수다.



  G4Box* STrigger_Veto_Box = 
    new G4Box("STrigger_Veto_Box", 0.5*VetoXYL, 0.5*VetoXYL, 0.5*VetoAl_Z);
  G4LogicalVolume* LTrigger_Veto_Box = 
    //new G4LogicalVolume(STrigger_Veto_Box, fTrigger_Veto_Box_material, "LTrigger_Veto_Box");
    new G4LogicalVolume(STrigger_Veto_Box, fAl, "LTrigger_Veto_Box");
  //new G4LogicalVolume(STrigger_Veto_Box,fSiPM_material , "LTrigger_Veto_Box");
  //양 끝단 sensitive detector를 위해 우선 이렇게 물질을 진공(fSiPM_material)로 한다.
  //Veto box를 감싸는 알루미늄 호일로 일단 두었다.

  fVisAttrTrigger_Veto_Box = new G4VisAttributes(G4Colour(0., 1.0, 1.0));
  fVisAttrTrigger_Veto_Box -> SetVisibility(true);
  LTrigger_Veto_Box ->SetVisAttributes(fVisAttrTrigger_Veto_Box); 

  Count_Veto = 0;
  Veto_number = 0;
  for(G4double j=0.; j<11; j++){
    for(G4double i=0.; i<11; i++){

      Count_Veto++;

      if(Checking_Empty_Space(Count_Veto) == false){
	 Veto_number++;
	 new G4PVPlacement(0,
	     G4ThreeVector( (-0.5*(11.*VetoXYL)*mm+0.5*VetoXYL) + VetoXYL*i ,
		(-0.5*(11.*VetoXYL)*mm+0.5*VetoXYL) + VetoXYL*j ,
		Trigger_Veto_Box_PosiZ),
	     //왼쪽 아래부터 시작해서 쌓아나간다.
	     LTrigger_Veto_Box,
	     "Trigger_Veto_Box",
	     LTrigger_Box,
	     false,
	     Veto_number, 
	     checkOverlaps); 
      }

    }
  }

  G4LogicalSkinSurface*SurfaceAl3 = new G4LogicalSkinSurface("SurfaceAl3", LTrigger_Veto_Box, fAl_Surf);


  ///////////////////////////////////////////////////////////////////////////////
  //Veto Air cap (Air cap 3)
  //Inner Trigger Veto LYSO(진짜 라이소)를 둘러싸는 알루미늄 포일 사이의 공기층

  G4Box* SVetoAirCap = new G4Box("SVetoAirCap", 0.5*VetoAirCap_XYL, 0.5*VetoAirCap_XYL, 0.5*VetoAirCap_Z);

  G4LogicalVolume* LVetoAirCap = new G4LogicalVolume(SVetoAirCap, fAir, "VetoAirCap");

  new G4PVPlacement(0,
      G4ThreeVector(0,
	 0,
	 0), //이거 mother인 Al포일(Sq)의 중심과 차이를 만드는 것 자체가
      //AirCap이 아닌 Al의 douther와의 길이 차이가 만드는 것임
      //그래서 AlThick_Z/2 를 더해주는 것이다. 
      LVetoAirCap,
      "VetoAirCap",
      LTrigger_Veto_Box,
      false,
      0,  
      checkOverlaps); 


  //Veto LYSO
  G4Box* SInTrigger_Veto_Box = 
    new G4Box("SInTrigger_Veto_Box", 0.5*VetoXY_Size, 0.5*VetoXY_Size, 0.5*(VetoZL));
  G4LogicalVolume* LInTrigger_Veto_Box = 
    new G4LogicalVolume(SInTrigger_Veto_Box, fSq_material, "LInTrigger_Veto_Box");

  new G4PVPlacement(0,
      G4ThreeVector( 0,
	 0,
	 0),
      LInTrigger_Veto_Box,
      "InTrigger_Veto_Box",
      LVetoAirCap ,
      false,
      0, 
      checkOverlaps); 


  ////////////////////////////////////////////////////////////////////////////////////
  //Trigger_Veto SiPM1 Front 설치(왼쪽 SiPM Front)

  G4double Veto_SiPMFront1_XYL = VetoXYL; //Trigger_veto의 사이즈는 2cm*2cm*12cm다.
  G4double Veto_SiPMFront1_XY_Size = VetoXY_Size;
  G4double Veto_SiPMFront1_ZL = 0.1*mm;
  G4double Trigger_Veto_SiPMFront1_PosiZ = 0.5*VetoAl_Z + 0.5*Veto_SiPMFront1_ZL;

  STrigger_Veto_SiPMFront1 = 
    new G4Box("STrigger_Veto_SiPMFront1", 0.5*Veto_SiPMFront1_XY_Size, 
	 0.5*Veto_SiPMFront1_XY_Size, 0.5*Veto_SiPMFront1_ZL);
  LTrigger_Veto_SiPMFront1 = 
    new G4LogicalVolume(STrigger_Veto_SiPMFront1, fSiPMFront_material, "LTrigger_Veto_SiPMFront1");

  fVisAttrTrigger_Veto_SiPMFront1 = new G4VisAttributes(G4Colour(1.0, 0.0, 0.0));
  fVisAttrTrigger_Veto_SiPMFront1 -> SetVisibility(true);
  LTrigger_Veto_SiPMFront1 ->SetVisAttributes(fVisAttrTrigger_Veto_SiPMFront1); 

  Count_Veto = 0;
  Veto_number = 0; //위의 Count_Veto는 11*11의 박스 어레이에서 빈 부분을 제거하기 위한 number를 부여하는 변수라면 이 Veto_number는 각 LYSO에 넘버를 순서대로 부여하기 위한 변수다.
  for(G4double j=0.; j<11; j++){
    for(G4double i=0.; i<11; i++){

      Count_Veto++;

      if(Checking_Empty_Space(Count_Veto) == false){
	 Veto_number++;
	 new G4PVPlacement(0,
	     G4ThreeVector( (-0.5*(11.*Veto_SiPMFront1_XYL)*mm+0.5*Veto_SiPMFront1_XYL) + Veto_SiPMFront1_XYL*i ,
		(-0.5*(11.*Veto_SiPMFront1_XYL)*mm+0.5*Veto_SiPMFront1_XYL) + Veto_SiPMFront1_XYL*j ,
		Trigger_Veto_SiPMFront1_PosiZ),
	     //왼쪽 아래부터 시작해서 쌓아나간다.
	     LTrigger_Veto_SiPMFront1,
	     "Trigger_Veto_SiPMFront1",
	     LTrigger_Box,
	     false,
	     Veto_number, 
	     checkOverlaps); 
      }

    }
  }

  //Trigger SiPM Front1의 Surface를 설정해 준다.
  Surface_TriggerSiPMFront1 = new G4LogicalSkinSurface("TriggerSiPMFront1", LTrigger_Veto_SiPMFront1, fSiPMFront_Surf);


  //////////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////
  //Trigger_Veto SiPM2 Front 설치 (오른쪽 SiPM Front)

  G4double Veto_SiPMFront2_XYL = VetoXYL; //Trigger_veto의 사이즈는 2cm*2cm*12cm다.
  G4double Veto_SiPMFront2_XY_Size = VetoXY_Size;
  G4double Veto_SiPMFront2_ZL = 0.1*mm;
  G4double Trigger_Veto_SiPMFront2_PosiZ = -1.*Trigger_Veto_SiPMFront1_PosiZ;

  STrigger_Veto_SiPMFront2 = 
    new G4Box("STrigger_Veto_SiPMFront2", 0.5*Veto_SiPMFront2_XY_Size, 
	 0.5*Veto_SiPMFront2_XY_Size, 0.5*Veto_SiPMFront2_ZL);
  LTrigger_Veto_SiPMFront2 = 
    new G4LogicalVolume(STrigger_Veto_SiPMFront2, fSiPMFront_material, "LTrigger_Veto_SiPMFront2");

  fVisAttrTrigger_Veto_SiPMFront2 = new G4VisAttributes(G4Colour(1.0, 0.0, 0.0));
  fVisAttrTrigger_Veto_SiPMFront2 -> SetVisibility(true);
  LTrigger_Veto_SiPMFront2 ->SetVisAttributes(fVisAttrTrigger_Veto_SiPMFront2); 

  Count_Veto = 0;
  Veto_number = 0; //위의 Count_Veto는 11*11의 박스 어레이에서 빈 부분을 제거하기 위한 number를 부여하는 변수라면 이 Veto_number는 각 LYSO에 넘버를 순서대로 부여하기 위한 변수다.
  for(G4double j=0.; j<11; j++){
    for(G4double i=0.; i<11; i++){

      Count_Veto++;

      if(Checking_Empty_Space(Count_Veto) == false){
	 Veto_number++;
	 new G4PVPlacement(0,
	     G4ThreeVector( (-0.5*(11.*Veto_SiPMFront2_XYL)*mm+0.5*Veto_SiPMFront1_XYL) + Veto_SiPMFront1_XYL*i ,
		(-0.5*(11.*Veto_SiPMFront2_XYL)*mm+0.5*Veto_SiPMFront1_XYL) + Veto_SiPMFront1_XYL*j ,
		Trigger_Veto_SiPMFront2_PosiZ),
	     //왼쪽 아래부터 시작해서 쌓아나간다.
	     LTrigger_Veto_SiPMFront2,
	     "Trigger_Veto_SiPMFront2",
	     LTrigger_Box,
	     false,
	     Veto_number, 
	     checkOverlaps); 
      }

    }
  }

  //Trigger SiPM Front2의 Surface를 설정해 준다.
  Surface_TriggerSiPMFront2 = new G4LogicalSkinSurface("TriggerSiPMFront2", LTrigger_Veto_SiPMFront2, fSiPMFront_Surf);


  ////////////////////////////////////////////////////////////////////////////////////
  //Trigger_Veto SiPM1 설치(왼쪽 SiPM  뒷부분)

  G4double Veto_SiPM1_XYL = VetoXYL; //Trigger_veto의 사이즈는 2cm*2cm*12cm다.
  G4double Veto_SiPM1_XY_Size = VetoXY_Size;
  G4double Veto_SiPM1_ZL = 0.3*mm;
  G4double Trigger_Veto_SiPM1_PosiZ = 0.5*VetoAl_Z + Veto_SiPMFront1_ZL + 0.5*Veto_SiPM1_ZL;

  STrigger_Veto_SiPM1 = 
    new G4Box("STrigger_Veto_SiPM1", 0.5*Veto_SiPM1_XY_Size, 
	 0.5*Veto_SiPM1_XY_Size, 0.5*Veto_SiPM1_ZL);
  LTrigger_Veto_SiPM1 = 
    new G4LogicalVolume(STrigger_Veto_SiPM1, fSiPMFront_material, "LTrigger_Veto_SiPM1");

  fVisAttrTrigger_Veto_SiPM1 = new G4VisAttributes(G4Colour(1.0, 0.0, 0.0));
  fVisAttrTrigger_Veto_SiPM1 -> SetVisibility(true);
  LTrigger_Veto_SiPM1 ->SetVisAttributes(fVisAttrTrigger_Veto_SiPM1); 

  Count_Veto = 0;
  Veto_number = 0; //위의 Count_Veto는 11*11의 박스 어레이에서 빈 부분을 제거하기 위한 number를 부여하는 변수라면 이 Veto_number는 각 LYSO에 넘버를 순서대로 부여하기 위한 변수다.
  for(G4double j=0.; j<11; j++){
    for(G4double i=0.; i<11; i++){

      Count_Veto++;

      if(Checking_Empty_Space(Count_Veto) == false){
	 Veto_number++;
	 new G4PVPlacement(0,
	     G4ThreeVector( (-0.5*(11.*Veto_SiPM1_XYL)*mm+0.5*Veto_SiPM1_XYL) + Veto_SiPM1_XYL*i ,
		(-0.5*(11.*Veto_SiPM1_XYL)*mm+0.5*Veto_SiPM1_XYL) + Veto_SiPM1_XYL*j ,
		Trigger_Veto_SiPM1_PosiZ),
	     //왼쪽 아래부터 시작해서 쌓아나간다.
	     LTrigger_Veto_SiPM1,
	     "Trigger_Veto_SiPM1",
	     LTrigger_Box,
	     false,
	     Veto_number, 
	     checkOverlaps); 
      }

    }
  }

  //////////////////////////////////////////////////////////////////////////////////////////
  //Trigger_Veto SiPM2 설치(오른쪽 SiPM  뒷부분)

  G4double Veto_SiPM2_XYL = VetoXYL; //Trigger_veto의 사이즈는 2cm*2cm*12cm다.
  G4double Veto_SiPM2_XY_Size = VetoXY_Size;
  G4double Veto_SiPM2_ZL = 0.3*mm;
  G4double Trigger_Veto_SiPM2_PosiZ = -1.*Trigger_Veto_SiPM1_PosiZ;

  STrigger_Veto_SiPM2 = 
    new G4Box("STrigger_Veto_SiPM2", 0.5*Veto_SiPM2_XY_Size, 
	 0.5*Veto_SiPM2_XY_Size, 0.5*Veto_SiPM2_ZL);
  LTrigger_Veto_SiPM2 = 
    new G4LogicalVolume(STrigger_Veto_SiPM2, fSiPMFront_material, "LTrigger_Veto_SiPM2");

  fVisAttrTrigger_Veto_SiPM2 = new G4VisAttributes(G4Colour(1.0, 0.0, 0.0));
  fVisAttrTrigger_Veto_SiPM2 -> SetVisibility(true);
  LTrigger_Veto_SiPM2 ->SetVisAttributes(fVisAttrTrigger_Veto_SiPM2); 

  Count_Veto = 0;
  Veto_number = 0; //위의 Count_Veto는 11*11의 박스 어레이에서 빈 부분을 제거하기 위한 number를 부여하는 변수라면 이 Veto_number는 각 LYSO에 넘버를 순서대로 부여하기 위한 변수다.
  for(G4double j=0.; j<11; j++){
    for(G4double i=0.; i<11; i++){

      Count_Veto++;

      if(Checking_Empty_Space(Count_Veto) == false){
	 Veto_number++;
	 new G4PVPlacement(0,
	     G4ThreeVector( (-0.5*(11.*Veto_SiPM2_XYL)*mm+0.5*Veto_SiPM2_XYL) + Veto_SiPM2_XYL*i ,
		(-0.5*(11.*Veto_SiPM2_XYL)*mm+0.5*Veto_SiPM2_XYL) + Veto_SiPM2_XYL*j ,
		Trigger_Veto_SiPM2_PosiZ),
	     //왼쪽 아래부터 시작해서 쌓아나간다.
	     LTrigger_Veto_SiPM2,
	     "Trigger_Veto_SiPM2",
	     LTrigger_Box,
	     false,
	     Veto_number, 
	     checkOverlaps); 
      }

    }
  }
///////////////////////////////////////////////////////////////////////////////
/*
  G4double FieldSize = 4.0*cm;

  fSFieldLimit = new G4Box("FieldLimit", 0.5*(FieldSize+0.02), 0.5*(FieldSize+0.02), 0.5*(FieldSize+0.02));
  fLFieldLimit = new G4LogicalVolume(fSFieldLimit, fVacuum, "FieldLimit");
  fPFieldLimit = new G4PVPlacement(0,
 			 G4ThreeVector(0., 0., 0.),
			 fLFieldLimit,
			 "FieldLimit",
	         LTrigger_Box,
			 false,
			 0,
             checkOverlaps); 
  
  fSFieldEnv = new G4Box("FieldEnv", 0.5*(FieldSize+0.01), 0.5*(FieldSize+0.01), 0.5*(FieldSize+0.01));
  fLFieldEnv = new G4LogicalVolume(fSFieldEnv, fVacuum, "FieldEnv");
  fPFieldEnv = new G4PVPlacement(0,
 			 G4ThreeVector(0., 0., 0.),
			 fLFieldEnv,
			 "FieldEnv",
	         fLFieldLimit,
			 false,
			 0,
             checkOverlaps); 

  fSFieldLimit_In = new G4Box("FieldLimit_In", 0.5*FieldSize, 0.5*FieldSize, 0.5*FieldSize);
  fLFieldLimit_In = new G4LogicalVolume(fSFieldLimit_In, fVacuum, "FieldLimit_In");
  fPFieldLimit_In = new G4PVPlacement(0,
 			 G4ThreeVector(0., 0., 0.),
			 fLFieldLimit_In,
			 "FieldLimit_In",
	         fLFieldEnv,
			 false,
			 0,
             checkOverlaps); 
*/
/////////////////////////////////////////////////////////////////////////////////////////////////
//Target and Target Envs

  G4double Target_InnerR = 0.; G4double Target_OuterR = 2.0*cm;
  G4double Target_Thickness = 20.0*micrometer;
  //타겟은 10마이크로미터짜리가 두 개가 있으므로 20 마이크로미터가 된다.
  G4double Target_Env_OuterR_Thinkness = 0.1*micrometer;
  ////G4double Target_Env_Z_Thinkness = 0.2*micrometer; //이것은 전체 원통의 두께가 0.02만큼 더 늘어나는 것이므로 한 쪽 축 방면으론 0.01만큼의 두께가 늘어날 것이다.
  G4double Target_Env_Z_Thinkness = 0.02*micrometer; //이것은 전체 원통의 두께가 0.002만큼 더 늘어나는 것이므로 한 쪽 축 방면으론 0.001만큼의 두께가 늘어날 것이다.


////  fSTargetEnv3 = new G4Tubs("TargetEnv3", Target_InnerR, (Target_OuterR + (3.0*Target_Env_OuterR_Thinkness)), (Target_Thickness + 3.0*Target_Env_Z_Thinkness)/2.0,
  fSTargetEnv3 = new G4Tubs("TargetEnv3", Target_InnerR, 5.0*cm, (Target_Thickness + 3.0*Target_Env_Z_Thinkness)/2.0,
		  0., CLHEP::twopi); 
  fLTargetEnv3 = new G4LogicalVolume(fSTargetEnv3, fVacuum, "TargetEnv3"); 
  fPTargetEnv3 = new G4PVPlacement(0,      
 			 G4ThreeVector(0., 0., 0.),
			 fLTargetEnv3,
			 "TargetEnv3",
	         //fLFieldLimit_In,
			 LTrigger_Box,
			 false,
			 0,
             checkOverlaps); 
 

  fSTargetEnv2 = new G4Tubs("TargetEnv2", Target_InnerR, (Target_OuterR + (2.0*Target_Env_OuterR_Thinkness)), (Target_Thickness + 2.0*Target_Env_Z_Thinkness)/2.0,
		  0., CLHEP::twopi); 
  fLTargetEnv2 = new G4LogicalVolume(fSTargetEnv2, fVacuum, "TargetEnv2"); 
  fPTargetEnv2 = new G4PVPlacement(0,      
 			 G4ThreeVector(0., 0., 0.),
			 fLTargetEnv2,
			 "TargetEnv2",
	         fLTargetEnv3,
			 false,
			 0,
             checkOverlaps); 


  fSTargetEnv1 = new G4Tubs("TargetEnv1", Target_InnerR, Target_OuterR, Target_Thickness/2.0,
		  0., CLHEP::twopi); 
  fLTargetEnv1 = new G4LogicalVolume(fSTargetEnv1, fTarget_material, "TargetEnv1"); 
  fPTargetEnv1 = new G4PVPlacement(0,      
 			 G4ThreeVector(0., 0., 0.),
			 fLTargetEnv1,
			 "TargetEnv1",
	         fLTargetEnv2,
			 false,
			 0,
             checkOverlaps); 

  fVisAttrTargetEnv = new G4VisAttributes();
  fVisAttrTargetEnv -> SetVisibility(false);
  ////fLTargetEnv3 -> SetVisAttributes(fVisAttrTargetEnv); 
  fLTargetEnv2 -> SetVisAttributes(fVisAttrTargetEnv); 
  fLTargetEnv1 -> SetVisAttributes(fVisAttrTargetEnv); 


  fSTargetLeft = new G4Tubs("TargetLeft", Target_InnerR, Target_OuterR, Target_Thickness/4.0,
		  0., CLHEP::twopi); 
  fLTargetLeft = new G4LogicalVolume(fSTargetLeft, fTarget_material, "TargetLeft"); 
  fPTargetLeft = new G4PVPlacement(0,      
 			 G4ThreeVector(0., 0., ((Target_Thickness/2.0)/2.0)),
			 fLTargetLeft,
			 "TargetLeft",
			 fLTargetEnv1,
			 false,
			 0,
             checkOverlaps); 

  fVisAttrTargetLeft = new G4VisAttributes(G4Colour(1.0, 0.0, 1.0));
  fVisAttrTargetLeft -> SetVisibility(true);
  fLTargetLeft ->SetVisAttributes(fVisAttrTargetLeft); 


  fSTargetRight = new G4Tubs("TargetRight", Target_InnerR, Target_OuterR, Target_Thickness/4.0,
		  0., CLHEP::twopi); 
  fLTargetRight = new G4LogicalVolume(fSTargetRight, fTarget_material, "TargetRight"); 
  fPTargetRight = new G4PVPlacement(0,      
 			 G4ThreeVector(0., 0., -((Target_Thickness/2.0)/2.0)),
			 fLTargetRight,
			 "TargetRight",
			 fLTargetEnv1,
			 false,
			 0,
             checkOverlaps); 

  fVisAttrTargetRight = new G4VisAttributes(G4Colour(1.0, 0.0, 1.0));
  fVisAttrTargetRight -> SetVisibility(true);
  fLTargetRight ->SetVisAttributes(fVisAttrTargetRight); 

/////////////////////////////////////////////////////////////////////////////////////////////////

  G4bool Tracker_On = true;
  
  if(Tracker_On == true){
  //Tracker for hits
  G4double Tracker_X_thick = 50.0*mm; G4double Tracker_Y_thick = 50.0*mm;
  G4double Tracker_Z_thick = 0.5*cm;

  fSTracker_Left = new G4Box("Tracker_Left", 0.5*Tracker_X_thick, 0.5*Tracker_Y_thick, 0.5*Tracker_Z_thick); 
  fLTracker_Left = new G4LogicalVolume(fSTracker_Left, fTracker_material,"Tracker_Left"); 

  for(G4double track_L = 10.; track_L <= 1000.; track_L= track_L + 10){
    if(track_L < 150.0){
    new G4PVPlacement(0,
			 G4ThreeVector(0., 0., ( (Tracker_Z_thick*0.5) + track_L) ),
			 fLTracker_Left,
			 "Tracker_Left",
	               LTrigger_Box,
			 false,
			 static_cast<int>( (track_L / 10.0) ),
                      checkOverlaps); 
     } //End of if
    
    else if(track_L >= 150){
    new G4PVPlacement(0,
			 //G4ThreeVector(0., 0., ( (Tracker_Z_thick*0.5) + track_L) - CylinderShell1_InBoxShell_PosiZ ),
			 G4ThreeVector(0., 0., 
(Tracker_Z_thick*0.5) + track_L - ( Cylinder1_ShellZ*0.5 + Trigger_Box_ShellZ*0.5 ) ),
			 //상대 좌표계이므로 Cylinder1의 길이*0.5를 빼준다.
			 fLTracker_Left,
			 "Tracker_Left",
	               LCylinder1,
			 false,
			 static_cast<int>( (track_L / 10.0) ),
                      checkOverlaps); 
     } //End of else  
   } //End of track_L loof

  fVisAttrPurple = new G4VisAttributes(G4Colour(1.0, 0.0, 1.0));
  fVisAttrPurple -> SetVisibility(true);
  //fLTracker_Left ->SetVisAttributes(fVisAttrPurple); 
  fLTracker_Left ->SetVisAttributes(fVisAttrTransparent);

  fSTracker_Right = new G4Box("Tracker_Right", 0.5*Tracker_X_thick, 0.5*Tracker_Y_thick, 0.5*Tracker_Z_thick); 
  fLTracker_Right = new G4LogicalVolume(fSTracker_Right, fTracker_material,"Tracker_Right"); 

  for(G4double track_R = 10.; track_R <= 1000.; track_R= track_R + 10){
    if(track_R < 150.0){
    fPTracker_Right = new G4PVPlacement(0,
			 G4ThreeVector(0., 0., -( (Tracker_Z_thick*0.5) + track_R) ),
			 fLTracker_Right,
			 "Tracker_Right",
	               LTrigger_Box,
			 false,
			 static_cast<int>( (track_R / 10.0) ),
                      checkOverlaps); 
     } //End of if
    
    else if(track_R >= 150){
    fPTracker_Right = new G4PVPlacement(0,
			 //G4ThreeVector(0., 0., ( (Tracker_Z_thick*0.5) + track_R) - CylinderShell1_InBoxShell_PosiZ ),
			 G4ThreeVector(0., 0., 
-((Tracker_Z_thick*0.5) + track_R) + ( Cylinder2_ShellZ*0.5 + Trigger_Box_ShellZ*0.5 ) ),
			 //상대 좌표계이므로 Cylinder1의 길이*0.5를 빼준다.
			 fLTracker_Right,
			 "Tracker_Right",
	               LCylinder2,
			 false,
			 static_cast<int>( (track_R / 10.0) ),
                      checkOverlaps); 
     } //End of else  
   } //End of track_R loof
  

  fVisAttrYellow = new G4VisAttributes(G4Colour(1.0, 1.0, 0.0));
  fVisAttrYellow -> SetVisibility(true);
  //fLTracker_Right ->SetVisAttributes(fVisAttrYellow);
  fLTracker_Right ->SetVisAttributes(fVisAttrTransparent);
  
  }//Tracker making bool roof end

/*
  fSTracker_Right = new G4Box("Tracker_Left", Tracker_X, Tracker_Y, Tracker_Z); 
  fLTracker_Right = new G4LogicalVolume(fSTracker_Right, fTracker_material,"Tracker_Left"); 
*/

///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
  //아래의 코드는 주변 실린더와 솔레노이드를 안보이게 하고
  //트리거 박스만 잘 보게 하기 위한 투명화 작업이다.

  fVisAttr_half_Transparent = new G4VisAttributes(G4Colour(1.0, 1.0, 1.0, 0.5));


  G4VisAttributes* red = new G4VisAttributes(G4Colour(0.7, 0.0, 0.0, 0.4));
  //red->SetForceWireframe(false);
  //red->SetForceSolid(true);
  G4VisAttributes* skyBlue = new G4VisAttributes(G4Colour(0.0, 0.75, 1.0, 0.1));
  //skyBlue->SetForceWireframe(false);
  //skyBlue->SetForceSolid(true);
  G4VisAttributes* black = new G4VisAttributes(G4Colour(0.0, 0.0, 0.0, 0.4));
  G4VisAttributes* gray = new G4VisAttributes(G4Colour(0.7, 0.7, 0.7, 0.4));
  //black->SetForceWireframe(false);
  //black->SetForceSolid(true);
  G4VisAttributes* pink = new G4VisAttributes(G4Colour(1.0, 0.75, 0.8, 0.4));
  G4VisAttributes* purple = new G4VisAttributes(G4Colour(1.0, 0.0, 1.0, 0.4));
  G4VisAttributes* green = new G4VisAttributes(G4Colour(0.0, 0.8, 0.0, 0.4));
  G4VisAttributes* yellow = new G4VisAttributes(G4Colour(0.91, 0.93, 0.41, 0.1));

  // Apply the colors to the components
  G4bool Transparency = 1;
  if(Transparency){
    //fVisAttrTransparent = new G4VisAttributes();
    //fVisAttrTransparent -> SetVisibility(false);
    //사실 위 코드 두 줄은 LogicWorld를 투명화할때 이미 정의되어 있음
    //그냥 기억나라고 다시 적어서 주석처리한 것 

    fLogicWorld ->SetVisAttributes(fVisAttrTransparent); 
    fLogicWorldEnv ->SetVisAttributes(fVisAttrTransparent); 
    //LCylinderShell1_InBoxShell->SetVisAttributes(fVisAttrTransparent);
    //LCylinder1_InBoxShell->SetVisAttributes(fVisAttrTransparent);
    //LCylinderShell2_InBoxShell->SetVisAttributes(fVisAttrTransparent);
    //LCylinder2_InBoxShell->SetVisAttributes(fVisAttrTransparent);
    //LCylinder1_Shell->SetVisAttributes(fVisAttrTransparent);
    //LCylinder1->SetVisAttributes(fVisAttrTransparent);
    //LCylinder2_Shell->SetVisAttributes(fVisAttrTransparent);
    //LCylinder2->SetVisAttributes(fVisAttrTransparent);

    LECAL1_Shell->SetVisAttributes(fVisAttrTransparent);
    LECAL1->SetVisAttributes(fVisAttrTransparent);
    LECAL2_Shell->SetVisAttributes(fVisAttrTransparent);
    LECAL2 ->SetVisAttributes(fVisAttrTransparent);
    LShielding_Mirror->SetVisAttributes(fVisAttrTransparent);
    LShielding1->SetVisAttributes(fVisAttrTransparent);
    LShielding2->SetVisAttributes(fVisAttrTransparent);
    LShieldingECAL1->SetVisAttributes(fVisAttrTransparent);
    LShieldingECAL2->SetVisAttributes(fVisAttrTransparent);
    LSolenoid1->SetVisAttributes(fVisAttrTransparent);
    LSolenoid2->SetVisAttributes(fVisAttrTransparent);
    LSq->SetVisAttributes(fVisAttrTransparent);
    LSq2->SetVisAttributes(fVisAttrTransparent);
    
  
    LSiPM1->SetVisAttributes(fVisAttrTransparent);
    LSiPMFront1->SetVisAttributes(fVisAttrTransparent);
    LSiPM2->SetVisAttributes(fVisAttrTransparent);
    LSiPMFront2->SetVisAttributes(fVisAttrTransparent);
    LInSq->SetVisAttributes(fVisAttrTransparent);
    LInSq2->SetVisAttributes(fVisAttrTransparent);
    LAirCap1->SetVisAttributes(fVisAttrTransparent);
    LAirCap2->SetVisAttributes(fVisAttrTransparent);
	
    //LTrigger_Box_Shell->SetVisAttributes(fVisAttrTransparent); 
    //LTrigger_Box->SetVisAttributes(fVisAttrTransparent); 
    //LTrigger_Veto_Box->SetVisAttributes(fVisAttrTransparent);
    //LInTrigger_Veto_Box->SetVisAttributes(fVisAttrTransparent);
    //LTrigger_Veto_SiPM1->SetVisAttributes(fVisAttrTransparent);
    //LTrigger_Veto_SiPM2->SetVisAttributes(fVisAttrTransparent);
    //LTrigger_Veto_SiPMFront1->SetVisAttributes(fVisAttrTransparent);
    LTrigger_Veto_SiPMFront2->SetVisAttributes(fVisAttrTransparent);
    LVetoAirCap->SetVisAttributes(fVisAttrTransparent);
    
    fLTargetLeft->SetVisAttributes(fVisAttrTransparent);
    fLTargetRight->SetVisAttributes(fVisAttrTransparent);

  }

    LTrigger_Box_Shell->SetVisAttributes(gray);
    LTrigger_Box->SetVisAttributes(gray);
    LTrigger_Veto_Box->SetVisAttributes(gray);
    LInTrigger_Veto_Box->SetVisAttributes(green);
    LTrigger_Veto_SiPM1->SetVisAttributes(red);
    LTrigger_Veto_SiPM2->SetVisAttributes(red);
    LTrigger_Veto_SiPMFront1->SetVisAttributes(red);
    LTrigger_Veto_SiPMFront2->SetVisAttributes(red);
    LVetoAirCap->SetVisAttributes(green);

    fLTargetEnv3->SetVisAttributes(red);
    fLTargetLeft->SetVisAttributes(purple);
    fLTargetRight->SetVisAttributes(purple);
 
    LCylinderShell1_InBoxShell->SetVisAttributes(gray);
    LCylinder1_InBoxShell->SetVisAttributes(gray);
    LCylinderShell2_InBoxShell->SetVisAttributes(gray);
    LCylinder2_InBoxShell->SetVisAttributes(gray);
    LCylinder1_Shell->SetVisAttributes(gray);
    LCylinder1->SetVisAttributes(gray);
    LCylinder2_Shell->SetVisAttributes(gray);
    LCylinder2->SetVisAttributes(gray);

    LECAL1_Shell->SetVisAttributes(gray);
    LECAL1->SetVisAttributes(gray);
    LECAL2_Shell->SetVisAttributes(gray);
    LECAL2 ->SetVisAttributes(gray);
    LSq->SetVisAttributes(green);
    LSq2->SetVisAttributes(green);
    LInSq->SetVisAttributes(green);
    LInSq2->SetVisAttributes(green);

    LAirCap1->SetVisAttributes(green);
    LAirCap2->SetVisAttributes(green);
   
    LSiPM1->SetVisAttributes(red);
    LSiPMFront1->SetVisAttributes(red);
    LSiPM2->SetVisAttributes(red);
    LSiPMFront2->SetVisAttributes(red);
  
    //LShielding_Mirror->SetVisAttributes(yellow);
    //LShielding1->SetVisAttributes(yellow);
    //LShielding2->SetVisAttributes(yellow);
    //LShieldingECAL1->SetVisAttributes(yellow);
    //LShieldingECAL2->SetVisAttributes(yellow);
    
    //LSolenoid1->SetVisAttributes(skyBlue);
    //LSolenoid2->SetVisAttributes(skyBlue);


  //투명화 작업 끝
  ////////////////////////////////////////////////////////////////////////////////

  //fScoringVolume = fLogicWorld; 
   
G4bool Limit = true;
G4UserLimits* limits = new G4UserLimits(2.0*cm);

if(Limit == true){
     LTrigger_Box->SetUserLimits(limits);
     LCylinder1_InBoxShell->SetUserLimits(limits);
     LCylinder2_InBoxShell->SetUserLimits(limits);
     LCylinder1->SetUserLimits(limits);
     LCylinder2->SetUserLimits(limits);
     fLTargetEnv3->SetUserLimits(limits);
     fLTargetEnv2->SetUserLimits(limits);
     fLTargetEnv1->SetUserLimits(limits);
     fLTargetLeft->SetUserLimits(limits);
     fLTargetRight->SetUserLimits(limits);
     fLTracker_Left->SetUserLimits(limits);
     fLTracker_Right->SetUserLimits(limits);

}     

  return fPhysWorld;
}


void DetectorConstruction::ConstructSDandField()
{ 

  if ( ! fFieldControl.Get() ) {
    FieldControl* fieldcontrol
      = new FieldControl();
    G4AutoDelete::Register(fieldcontrol);
    fFieldControl.Put(fieldcontrol);
  }
  fFieldControl.Get()->ConstructMagField(); // add field value 


  //For sensitive detector
  G4SDManager *sdManager = G4SDManager::GetSDMpointer();
  G4String SDname;
  EmCalorimeterSD* emCalorimeter = new EmCalorimeterSD(SDname="/EMcalorimeter");
  sdManager->AddNewDetector(emCalorimeter);
  //LSiPM1->SetSensitiveDetector(emCalorimeter);
  //LSiPM2->SetSensitiveDetector(emCalorimeter);
  LSiPMFront1->SetSensitiveDetector(emCalorimeter);
  LSiPMFront2->SetSensitiveDetector(emCalorimeter);

  //Sensitive Detector For Trigger Veto
  //나중에 이거 emcvalorimeter말고 다른 sensitive detector class 만드는게 어떨까 생각함
  LTrigger_Veto_SiPMFront1->SetSensitiveDetector(emCalorimeter);
  LTrigger_Veto_SiPMFront2->SetSensitiveDetector(emCalorimeter);

}

//////////////////////////////////////////////////////////////////////////////////////////////
//LYSO를 위한 멤버 함수
void DetectorConstruction::CenterFinder(G4double x_, G4double y_, G4double a_, G4double i_)
{
  if(y_>=0){
    SqCenter_x = x_ + a_/2;
    SqCenter_y = y_ - a_/2;
  }

  else{
    SqCenter_x = x_ + a_/2;
    SqCenter_y = y_ + a_/2;
  }
}

G4double DetectorConstruction::SpaceFinder(G4double x_, G4double y_, G4double SqL_, G4double r_)
{
  G4double SpaceOfCircle_ = 0;
  while(1){
    //입력받는 x_위치는 사각형의 왼쪽 부분이다.
    x_ = x_+SqL_; //반복문의 첫 번째에서
    //사각형의 오른쪽 부분이다.

    if( sqrt(r_*r_ - y_*y_) < x_ + SqL_)
      //만약 원 밖 직전의 사각형이라면
      //SqL이 더해진 시점에서 원을 넘어선다.
      //이 때 원에서 x좌표를 빼주면 남는 공간이다.
    { SpaceOfCircle_ = sqrt(r_*r_ - y_*y_) - x_;
      break; }
  }

  return SpaceOfCircle_;
}
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
//Trigger veto를 위한 멤버 함수
G4bool DetectorConstruction::Checking_Empty_Space(G4int Count_Veto_){
  //사각형 공간을 꽉 채우는 반복문에서 빈 공간이 되야 하는 Veto를 찾기 위한 멤버 함수
  //만약 빈 공간이 되어야 하면 true을 반환하겠다.
  //빈 공간이 아닐 경우 false을 반환한다.

  G4int empty[20] = {1,2,3,9,10,11,12,22,23,33,89,99,100,110,111,112,113,119,120,121};

  for(int i=0; i<20; i++){
    if(Count_Veto_ == empty[i]) {return true; break;}
  }

  if( (Count_Veto_>=37 && Count_Veto_<=41) || (Count_Veto_>=48 && Count_Veto_<=52) || 
      (Count_Veto_>=59 && Count_Veto_<=63) || (Count_Veto_>=70 && Count_Veto_<=74) 
      || (Count_Veto_>=81 && Count_Veto_<=85) )
  { return true; }

  return false;
}



