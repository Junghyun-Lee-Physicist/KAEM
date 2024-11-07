
#include "EmCalorimeterHit.hh"

G4ThreadLocal G4Allocator<EmCalorimeterHit>* EmCalorimeterHitAllocator = 0;

EmCalorimeterHit::EmCalorimeterHit()
//: G4VHit(), fEdep(0.), fTrackID(0.), fVolumeName(0.)
: G4VHit(), fVolumeName(0.), fPhotons(0), fFront(0), fTrackID(0.), fCopynum(0.)
{}

EmCalorimeterHit::EmCalorimeterHit(G4int cellID)
//: G4VHit(), fEdep(0.), fTrackID(0.), fVolumeName(0.)
: G4VHit(), fVolumeName(0.), fPhotons(0), fFront(0), fTrackID(0.), fCopynum(0.)
{}

EmCalorimeterHit::~EmCalorimeterHit()
{}

EmCalorimeterHit::EmCalorimeterHit(const EmCalorimeterHit &right)
: G4VHit(), fVolumeName(right.fVolumeName), fPhotons(right. fPhotons), fFront(right. fFront), fTrackID(right. fTrackID), fCopynum(right. fCopynum)
{}

const EmCalorimeterHit& EmCalorimeterHit::operator=(const EmCalorimeterHit &right)
{
  fVolumeName = right.fVolumeName;
  fPhotons = right.fPhotons;
  fFront = right.fFront;  
  fTrackID = right.fTrackID;
  fCopynum = right.fCopynum;

  return *this;
}
//이것이 대입 연산자긴 해도 pushback(G4에선 insert)로 HitsCollection에
//저장되며 이 때 insert인자로 hit형태가 들어간다.
//이 말인즉슨 아마도 위의 operator는 어떤 변수가 저장될지를 정한다고도 생각 할 수 있을 것이라 사료된다. (나의 추측)


G4bool EmCalorimeterHit::operator==(const EmCalorimeterHit &right) const
{
  //return (fCellID==right.fCellID);
  return (fFront==right.fFront && fTrackID==right.fTrackID);
}

//operator를 통해 == 비교 연산자와 = 대입 연산자를 정의한다.
//'같다'의 비교 정의를 위해 논리 구문을 만들어 bool로 리턴하는 것이다.
  
