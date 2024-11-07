#ifndef EmCalorimeterSD_h
#define EmCalorimeterSD_h 1

#include "G4VSensitiveDetector.hh"
#include "EmCalorimeterHit.hh"

//class G4HCofThisEvent;

class EmCalorimeterSD : public G4VSensitiveDetector
{
public:
  //Constructor 생성자
  EmCalorimeterSD(G4String SDname);
  
  //Destructor
  virtual ~EmCalorimeterSD();
  //가상함수 설명 https://genesis8.tistory.com/105
  
  //////////////////////////////////////////////////////
  //중심 기능
  virtual void Initialize(G4HCofThisEvent* HCE); //시작 시 불러지는 함수
    //아마도 가상 함수의 경우 부모 클래스의 기능을 받아올 시
  //오버라이딩, 즉 다형적인 기능 실행을 위한 용도로 보인다.
  //예를 들어 RTS 100개의 캐릭터가 있다고 생각하면
  //공격 명령 시 100번의 부모 클래스 호출을 하기보단 한번에 받는게 나은 것인듯 하다.
  
  //나머지는 부가 함수지만 ProcessHits은 필수다.
  //Sensitive detector 내의 모든 step에서 call된다.
  virtual G4bool ProcessHits(G4Step* aStep, G4TouchableHistory* );
  
  //virtual void EndOfEvent(G4HCofThisEvent* HCE); //이벤트 끝날 때 불러지는 함수

private:
  EmCalorimeterHitsCollection* fHitsCollection;
  G4int fHCID;  
};
  
#endif
