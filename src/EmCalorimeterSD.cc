#include "EmCalorimeterSD.hh"
#include "EmCalorimeterHit.hh"

#include "G4HCofThisEvent.hh"
#include "G4SDManager.hh"
#include "G4TouchableHistory.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4ios.hh"

#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"

EmCalorimeterSD::EmCalorimeterSD(G4String name)
: G4VSensitiveDetector(name),
  //fHitsCollection(nullptr), fHCID(-1)
  fHitsCollection(0), fHCID(-1)
{
  //collectionName.insert("EmcalorimeterColl");
  collectionName.insert("EMcalorimeterColl"); //뒤의 Coll은 Collection인듯
  //dual readout은 위의 hit collection name을 인자로 받는다.
}

EmCalorimeterSD::~EmCalorimeterSD()
{}

void EmCalorimeterSD::Initialize(G4HCofThisEvent* hce)
{
  fHitsCollection = new EmCalorimeterHitsCollection(SensitiveDetectorName, collectionName[0]);
  //Hit class에 using operator로 정의되어 있는 듯 하다. Collection을 사용하기 위한 이름을 정해둔듯
  if (fHCID<0) {
    fHCID = G4SDManager::GetSDMpointer()->GetCollectionID(fHitsCollection);
  }
  hce->AddHitsCollection(fHCID, fHitsCollection);

}


G4bool EmCalorimeterSD::ProcessHits(G4Step* step, G4TouchableHistory* )
{
  
  if(step->GetTrack()->GetDefinition() !=G4OpticalPhoton::OpticalPhotonDefinition()) 
  {
  //std::cout<<"ProcessHits are NOT PASS!!!!!!!!!!!"<<std::endl;
  return false;
  }
  //특정 G4Step 인스턴스를 인자로 받으므로 이 안의 정보를 이용 가능할 것이다.
  //근데 gamma는 어떡할지 모르겠다.  
  
  //G4cout<<"Now ProcessHits are pass if condition (optical photon)"<<G4endl;

  //G4int copyNo = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetCopyNo();
  //Touchable 의미, 잘은 모르겠다. 
  //https://geant4-userdoc.web.cern.ch/UsersGuides/ForApplicationDeveloper/html/Detector/Geometry/geomTouch.html
  
  G4int copyNo = step->GetPostStepPoint()->GetTouchable()->GetVolume()->GetCopyNo();
  G4int nofHits = fHitsCollection->entries();
  G4int TrackID = step->GetTrack()->GetTrackID();  
  G4int CurrentStepNum = step->GetTrack()->GetCurrentStepNumber();

//G4String VolumeName = step->GetPreStepPoint()->GetPhysicalVolume()->GetName();
//G4cout<<"Test PreStep Volume = "<<VolumeName<<G4endl;

  EmCalorimeterHit* hit = NULL;

  for(G4int i = 0; i < nofHits; i++) {
    if( (*fHitsCollection)[i]->GetIsFront()== 1 && (*fHitsCollection)[i]->GetTrackID()==TrackID)
    //Front 값이 1이라는 뜻은 맨 처음 step을 지났다는 의미다. hit 선언 시 Front값이 1이면 맨 앞 hit이다.
    {
      hit = (*fHitsCollection)[i];
      hit -> SetNotFront(); //어찌됐건 여기 조건문 안의 hit은 첫 번째 step이 아니므로 0(Not Front)를 해준다.
      hit -> photonNotCount();
      break;
      //이 구문은 이때까지 만들어진 hit을 collection에서 뒤져 맨 앞의 hit 정보를 불러온 후
      //뒤의 hit들에는 이 앞의 정보를 모두 덧씌우게 만드는 구문이다.
    }
  }

  if (hit==NULL) {
  hit = new EmCalorimeterHit();
  //G4int SiPMnum = step->GetPostStepPoint()->GetTouchable()->GetVolume()->GetCopyNo();
  //std::cout<< "This is SiPMnum = " << SiPMnum << std::endl;

  //if ( (hit->GetIsFront()) == 0) 
  //{
  hit->SetFront();
  //}
  //첫 선언 시 hit은 당연히 0이다.

  //G4int TrackID = step->GetTrack()->GetTrackID();
  //hit -> SetTrackID(TrackID);
  //SD에서 step정보를 뽑고 그것을 Hit의 접근자로 정보를 전달
  //이러한 정보는 processhit멤버 함수를 통해 step마다 기록된다.
  //hit의 cc 코드의 생성자는 SD에서 매번 step마다 시행될 것이며 이때 정보를 hitcollection으로 넘길 것이다.
  //Eventaction에서 이러한 hitcollection을 이용 정보를 다시 끄집어 내어 사용한다.

  //G4String VolumeName = step->GetPreStepPoint()->GetPhysicalVolume()->GetName();
  G4String VolumeName = step->GetPostStepPoint()->GetTouchable()->GetVolume()->GetName();
  hit -> SetVolumeName(VolumeName);  
  hit -> SetTrackID(TrackID);
  hit -> photonCount();
  hit -> SetLYSO_number(copyNo);
  fHitsCollection->insert(hit);
  }
  //위의 for문에서 맨 앞의 hit이 존재하지 않을 경우에만 위의 if문이 작동한다.
  //이 때 photoncount가 밖에 있는 이유는 매번 new를 통해 hit instance가 생성된다면
  //이 count 또한 초기화가 될 것이기 때문이다.
  //따라서 count 횟수는 영향을 받지 않게 하기 위해 if문 밖에 존재하는 것이다.
  //또한 count는 비록 new를 통해 초기화 되지 않지만 if 위의 for문에서 이전 hit 정보를 받고 그것이  사실 상 hit collection에 들어가게 된다. (문제는 어디서 insert가 되는지인데 이걸 좀 더 생각해 볼 필요가 있는 듯 하다.

  //hit -> photonCount();
  //dual readout에서 왜 여기서 photonCount를 넣었는지 몰라도 이럴 시 step마다 count가 올라간다.

  return true;
}

