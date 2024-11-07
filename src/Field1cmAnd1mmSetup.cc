
#include "Field1cmAnd1mmSetup.hh"
#include "G4SystemOfUnits.hh"
#include "G4Exp.hh"

#include "G4AutoLock.hh"

namespace
{
  G4Mutex myField3DLock = G4MUTEX_INITIALIZER;
}

Field1cmAnd1mmSetup::Field1cmAnd1mmSetup()
: G4MagneticField()
{
ifstream file;
fNx = 21;
fNy = 21;
fNz = 255;
lenUnit = cm;
double positionNum;

  G4AutoLock lock(&myField3DLock);

  //file.open("SolenoidField_GridMap_1cm.map");
  file.open("SolenoidField_GridMap.map");

  fXField.resize( fNx );
  fYField.resize( fNx );
  fZField.resize( fNx );
  int ix, iy, iz;
  for (ix=0; ix<fNx; ix++)
  {
    fXField[ix].resize(fNy);
    fYField[ix].resize(fNy);
    fZField[ix].resize(fNy);
    for (iy=0; iy<fNy; iy++)
    {
      fXField[ix][iy].resize(fNz);
      fYField[ix][iy].resize(fNz);
      fZField[ix][iy].resize(fNz);
    }
  }

  double bx,by,bz;
  
   for (ix=0; ix<fNx; ix++)
   {
    for (iy=0; iy<fNy; iy++)
    {
      for (iz=0; iz<fNz; iz++)
      {
        fXField[ix][iy][iz] = 0 ;
        fYField[ix][iy][iz] = 0 ;
        fZField[ix][iy][iz] = 0 ;
      }
    }
   }
  
  fMaxix = 10. *lenUnit;
  fMaxiy = 10. *lenUnit;
  fMaxiz = 127. *lenUnit;
  
  fMinix = -10. *lenUnit;
  fMiniy = -10. *lenUnit;
  fMiniz = -127. *lenUnit;

  fDx = 20  *lenUnit;
  fDy = 20  *lenUnit;
  fDz = 254 *lenUnit;

  positionNum = 0;
/*
  while(positionNum<=38703)
  //while(1)
  {
       file >> positionNum >> px >> py >> pz >> bx >> by >> bz;
       fXField[px][py][pz] = bx*tesla ;
       fYField[px][py][pz] = by*tesla ;
       fZField[px][py][pz] = bz*tesla ;
    if(positionNum==38703) break;
    //{G4cout<< "input complete"<<endl;}
  }
*/

  int pxmv,pymv,pzmv;
  int OnecmGridStart = 29792;
  int OnecmGridEnd = 118786;
  int OnecmLineNumber = (OnecmGridEnd - OnecmGridStart) + 1; //=88995
 // cout<<"now start file input..."<<endl;
  for(int i=0; i<OnecmLineNumber;i++){
       file >> positionNum >> px >> py >> pz >> bx >> by >> bz;
       pxmv = px +10;
       pymv = py +10;
       pzmv = pz +127;
  //G4cout<<positionNum<<"st file inf"<<G4endl;
  //cout<<"xyz mv position = "<<pxmv<<", "<<pymv<<", "<<pzmv<<endl;
       fXField[pxmv][pymv][pzmv] = bx*tesla ;
       fYField[pxmv][pymv][pzmv] = by*tesla ;
       fZField[pxmv][pymv][pzmv] = bz*tesla ;
       //임의의 값을 지안트에 넣어줄 때는 지안트에게 단위를 알려줘야함  
    }
  //file.close();


//////////////////////////////////////////////////////////////////////////
//Center 1mm 단위 field 시작

fNx2 = 31;
fNy2 = 31;
fNz2 = 31;
lenUnit2 = mm;
//double positionNum; 위에서 선언됨

  //G4AutoLock lock(&myField3DLock);

  fXField2.resize( fNx2 );
  fYField2.resize( fNx2 );
  fZField2.resize( fNx2 );
  //int ix, iy, iz; 위에서 정의됨
  for (ix=0; ix<fNx2; ix++)
  {
    fXField2[ix].resize(fNy2);
    fYField2[ix].resize(fNy2);
    fZField2[ix].resize(fNy2);
    for (iy=0; iy<fNy2; iy++)
    {
      fXField2[ix][iy].resize(fNz2);
      fYField2[ix][iy].resize(fNz2);
      fZField2[ix][iy].resize(fNz2);
    }
  }

  //double bx,by,bz; 위에서 정의됨
  
   for (ix=0; ix<fNx2; ix++)
   {
    for (iy=0; iy<fNy2; iy++)
    {
      for (iz=0; iz<fNz2; iz++)
      {
        fXField2[ix][iy][iz] = 0 ;
        fYField2[ix][iy][iz] = 0 ;
        fZField2[ix][iy][iz] = 0 ;
      }
    }
   }
  
  fMaxix2 = 15. *lenUnit2;
  fMaxiy2 = 15. *lenUnit2;
  fMaxiz2 = 15. *lenUnit2;
  
  fMinix2 = -15. *lenUnit2;
  fMiniy2 = -15. *lenUnit2;
  fMiniz2 = -15. *lenUnit2;

  fDx2 = 30  *lenUnit2;
  fDy2 = 30  *lenUnit2;
  fDz2 = 30  *lenUnit2;

  positionNum = 0;

  //int pxmv,pymv,pzmv;
  int OnemmLineNumber = 29791;

 // cout<<"now start file input..."<<endl;
  for(int i=0; i<OnemmLineNumber;i++){
       file >> positionNum >> px2 >> py2 >> pz2 >> bx >> by >> bz;
       pxmv = px2*10 +15;
       pymv = py2*10 +15;
       pzmv = pz2*10 +15;
  //G4cout<<positionNum<<"st file inf"<<G4endl;
  //cout<<"xyz mv position = "<<pxmv<<", "<<pymv<<", "<<pzmv<<endl;
       fXField2[pxmv][pymv][pzmv] = bx*tesla ;
       fYField2[pxmv][pymv][pzmv] = by*tesla ;
       fZField2[pxmv][pymv][pzmv] = bz*tesla ;
       //임의의 값을 지안트에 넣어줄 때는 지안트에게 단위를 알려줘야함  
    }
  file.close();

}

Field1cmAnd1mmSetup::~Field1cmAnd1mmSetup()
{}


void Field1cmAnd1mmSetup::GetFieldValue(const double point[4], double *Bfield) const
{
  Bfield[0] = 0.0;
  Bfield[1] = 0.0;
  Bfield[2] = 0.0;
  Bfield[3] = 0.0;
  Bfield[4] = 0.0;
  Bfield[5] = 0.0;

  /*
  double x = point[0]*cm; 
//지안트에서 값을 빼올때는 단위를 알려줄 필요가 없고 내가 지안트 단위를 알아서 계산해줘야함
//지안트의 기본 길이 단위는 mm다.
//애초에 지안트에 값일 대입해줄 때 단위를 알려준다는 것은 사실 지안트 단위로 바꾸는 것이다.
//즉 아래의 fMinix는 이미 mm단위다. 
  double y = point[1]*cm;
  double z = point[2]*cm; 
  */

  double x = point[0];
  double y = point[1];
  double z = point[2]; 

 if
 ( (fabs(x)<=fMaxix) && (fabs(y)<=fMaxiy) && (fabs(z)<=fMaxiz) )
 {
 
	if(fabs(x)<=fMaxix2 && fabs(y)<=fMaxix2 && fabs(z)<=fMaxiz2){
    Bfield[0] = 0.0;
    Bfield[1] = 0.0;
    Bfield[2] = 0.0;
    Bfield[3] = 0.0;
    Bfield[4] = 0.0;
    Bfield[5] = 0.0;

    double xfraction = (x - fMinix2) / fDx2;
    double yfraction = (y - fMiniy2) / fDy2;
    double zfraction = (z - fMiniz2) / fDz2;

    double xdindex, ydindex, zdindex;

    double xlocal = ( std::modf(xfraction*(fNx2-1), &xdindex));
    double ylocal = ( std::modf(yfraction*(fNy2-1), &ydindex));
    double zlocal = ( std::modf(zfraction*(fNz2-1), &zdindex));

    int xindex = static_cast<int>(std::floor(xdindex));
    int yindex = static_cast<int>(std::floor(ydindex));
    int zindex = static_cast<int>(std::floor(zdindex));
    
////////////////////////////////////////////////////////////
//여기서 각 기본 grid 정사각형의 꼭지점 자기장 값

    double ABx = fXField2[xindex][yindex][zindex];
    double ABy = fYField2[xindex][yindex][zindex];
    double ABz = fZField2[xindex][yindex][zindex];

    double BBx = fXField2[xindex+1][yindex][zindex];
    double BBy = fYField2[xindex+1][yindex][zindex];
    double BBz = fZField2[xindex+1][yindex][zindex];

    double CBx = fXField2[xindex+1][yindex][zindex+1];
    double CBy = fYField2[xindex+1][yindex][zindex+1];
    double CBz = fZField2[xindex+1][yindex][zindex+1];

    double DBx = fXField2[xindex][yindex][zindex+1];
    double DBy = fYField2[xindex][yindex][zindex+1];
    double DBz = fZField2[xindex][yindex][zindex+1];

    double EBx = fXField2[xindex][yindex+1][zindex];
    double EBy = fYField2[xindex][yindex+1][zindex];
    double EBz = fZField2[xindex][yindex+1][zindex];

    double FBx = fXField2[xindex+1][yindex+1][zindex];
    double FBy = fYField2[xindex+1][yindex+1][zindex];
    double FBz = fZField2[xindex+1][yindex+1][zindex];

    double GBx = fXField2[xindex+1][yindex+1][zindex+1];
    double GBy = fYField2[xindex+1][yindex+1][zindex+1];
    double GBz = fZField2[xindex+1][yindex+1][zindex+1];
    
    double HBx = fXField2[xindex][yindex+1][zindex+1];
    double HBy = fYField2[xindex][yindex+1][zindex+1];
    double HBz = fZField2[xindex][yindex+1][zindex+1];

////////////////////////////////////////////////////////////////
//각 축의 포인트를 기반으로 보간을 시작한다.

    double NBx = ABx * (1.-zlocal) + DBx * (zlocal);
    double NBy = ABy * (1.-zlocal) + DBy * (zlocal);
    double NBz = ABz * (1.-zlocal) + DBz * (zlocal);

    double MBx = BBx * (1.-zlocal) + CBx * (zlocal);
    double MBy = BBy * (1.-zlocal) + CBy * (zlocal);
    double MBz = BBz * (1.-zlocal) + CBz * (zlocal);

    double VBx = EBx * (1.-zlocal) + HBx * (zlocal);
    double VBy = EBy * (1.-zlocal) + HBy * (zlocal);
    double VBz = EBz * (1.-zlocal) + HBz * (zlocal);
  
    double UBx = FBx * (1.-zlocal) + GBx * (zlocal);
    double UBy = FBy * (1.-zlocal) + GBy * (zlocal);
    double UBz = FBz * (1.-zlocal) + GBz * (zlocal);

    double RBx = NBx * (1.-xlocal) + MBx * (xlocal);
    double RBy = NBy * (1.-xlocal) + MBy * (xlocal);
    double RBz = NBz * (1.-xlocal) + MBz * (xlocal);

    double SBx = VBx * (1.-xlocal) + UBx * (xlocal);
    double SBy = VBy * (1.-xlocal) + UBy * (xlocal);
    double SBz = VBz * (1.-xlocal) + UBz * (xlocal);

    Bfield[0] = RBx * (1.-ylocal) + SBx * (ylocal);
    Bfield[1] = RBy * (1.-ylocal) + SBy * (ylocal);
    Bfield[2] = RBz * (1.-ylocal) + SBz * (ylocal);

	}

	else
	{
    double xfraction = (x - fMinix) / fDx;
    double yfraction = (y - fMiniy) / fDy;
    double zfraction = (z - fMiniz) / fDz;

    double xdindex, ydindex, zdindex;

    double xlocal = ( std::modf(xfraction*(fNx-1), &xdindex));
    double ylocal = ( std::modf(yfraction*(fNy-1), &ydindex));
    double zlocal = ( std::modf(zfraction*(fNz-1), &zdindex));

    int xindex = static_cast<int>(std::floor(xdindex));
    int yindex = static_cast<int>(std::floor(ydindex));
    int zindex = static_cast<int>(std::floor(zdindex));
    
////////////////////////////////////////////////////////////
//여기서 각 기본 grid 정사각형의 꼭지점 자기장 값

    double ABx = fXField[xindex][yindex][zindex];
    double ABy = fYField[xindex][yindex][zindex];
    double ABz = fZField[xindex][yindex][zindex];

    double BBx = fXField[xindex+1][yindex][zindex];
    double BBy = fYField[xindex+1][yindex][zindex];
    double BBz = fZField[xindex+1][yindex][zindex];

    double CBx = fXField[xindex+1][yindex][zindex+1];
    double CBy = fYField[xindex+1][yindex][zindex+1];
    double CBz = fZField[xindex+1][yindex][zindex+1];

    double DBx = fXField[xindex][yindex][zindex+1];
    double DBy = fYField[xindex][yindex][zindex+1];
    double DBz = fZField[xindex][yindex][zindex+1];

    double EBx = fXField[xindex][yindex+1][zindex];
    double EBy = fYField[xindex][yindex+1][zindex];
    double EBz = fZField[xindex][yindex+1][zindex];

    double FBx = fXField[xindex+1][yindex+1][zindex];
    double FBy = fYField[xindex+1][yindex+1][zindex];
    double FBz = fZField[xindex+1][yindex+1][zindex];

    double GBx = fXField[xindex+1][yindex+1][zindex+1];
    double GBy = fYField[xindex+1][yindex+1][zindex+1];
    double GBz = fZField[xindex+1][yindex+1][zindex+1];
    
    double HBx = fXField[xindex][yindex+1][zindex+1];
    double HBy = fYField[xindex][yindex+1][zindex+1];
    double HBz = fZField[xindex][yindex+1][zindex+1];

////////////////////////////////////////////////////////////////
//각 축의 포인트를 기반으로 보간을 시작한다.

    double NBx = ABx * (1.-zlocal) + DBx * (zlocal);
    double NBy = ABy * (1.-zlocal) + DBy * (zlocal);
    double NBz = ABz * (1.-zlocal) + DBz * (zlocal);

    double MBx = BBx * (1.-zlocal) + CBx * (zlocal);
    double MBy = BBy * (1.-zlocal) + CBy * (zlocal);
    double MBz = BBz * (1.-zlocal) + CBz * (zlocal);

    double VBx = EBx * (1.-zlocal) + HBx * (zlocal);
    double VBy = EBy * (1.-zlocal) + HBy * (zlocal);
    double VBz = EBz * (1.-zlocal) + HBz * (zlocal);
  
    double UBx = FBx * (1.-zlocal) + GBx * (zlocal);
    double UBy = FBy * (1.-zlocal) + GBy * (zlocal);
    double UBz = FBz * (1.-zlocal) + GBz * (zlocal);

    double RBx = NBx * (1.-xlocal) + MBx * (xlocal);
    double RBy = NBy * (1.-xlocal) + MBy * (xlocal);
    double RBz = NBz * (1.-xlocal) + MBz * (xlocal);

    double SBx = VBx * (1.-xlocal) + UBx * (xlocal);
    double SBy = VBy * (1.-xlocal) + UBy * (xlocal);
    double SBz = VBz * (1.-xlocal) + UBz * (xlocal);

    Bfield[0] = RBx * (1.-ylocal) + SBx * (ylocal);
    Bfield[1] = RBy * (1.-ylocal) + SBy * (ylocal);
    Bfield[2] = RBz * (1.-ylocal) + SBz * (ylocal);
	}
    //사실상 위의 식이 P점이 된다.
    //https://darkpgmr.tistory.com/117 (관련 그래프 모습, 원노트에도 있음)


  } //필드 자기장 영역 if문
/////////////////////////////////////////////////////////////////////////////////
}//GetFieldValue 종료
