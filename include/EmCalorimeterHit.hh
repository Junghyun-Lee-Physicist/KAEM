#ifndef EmCalorimeterHit_h
#define EmCalorimeterHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"

class EmCalorimeterHit : public G4VHit
{
public:
  //hit은 반드시 ID가 주어져야 한다
  EmCalorimeterHit();
  EmCalorimeterHit(G4int cellId);
  EmCalorimeterHit(const EmCalorimeterHit &right);
  virtual ~EmCalorimeterHit();

  const EmCalorimeterHit& operator=(const EmCalorimeterHit &right);
  G4bool operator==(const EmCalorimeterHit &right) const;
 
  inline void *operator new(size_t);
  inline void operator delete(void *aHit);

  /* 
  void SetCellID(G4int z) { fCellID = z; }
  G4int GetCellID() const { return fCellID; }

  void SetEdep(G4double de) { fEdep = de; }
  void AddEdep(G4double de) { fEdep += de; }
  G4double GetEdep() const { return fEdep; }

  void SetTrackID(G4int ID) { fTrackID = ID; }
  G4double GetTrackID() const { return fTrackID; }
  */
  void SetVolumeName(G4String Name) 
  { 
  if (Name == "SiPMFront1") fVolumeName = 1; 
  else if (Name == "SiPMFront2") fVolumeName = 2;
  else if (Name == "Trigger_Veto_SiPMFront1") fVolumeName = 3;
  else if (Name == "Trigger_Veto_SiPMFront2") fVolumeName = 4;
  }
  G4int GetVolumeName() const { return fVolumeName; }
  
  void photonCount() { fPhotons = 1; }
  G4int GetPhotonCount() const { return fPhotons; }
  void photonNotCount() { fPhotons = 0; }

  void SetFront() { fFront=1; }
  G4int GetIsFront() {return fFront;}
  void SetNotFront() { fFront = 0; }

  void SetTrackID(G4int ID) { fTrackID = ID; }
  G4double GetTrackID() const { return fTrackID; }
 
  void SetLYSO_number(G4int CopyNumber) { fCopynum = CopyNumber; }
  G4int GetLYSO_number() const { return fCopynum; }

private:
  /*
  G4int fCellID;
  G4double fEdep;
  G4int fTrackID;
  */
  G4int fVolumeName;
  G4int fPhotons;
  G4int fFront;
  G4int fTrackID;
  G4int fCopynum;

};

using EmCalorimeterHitsCollection = G4THitsCollection<EmCalorimeterHit>;
//user define된 hit collection인듯 하다.
//아마도 기본형이 G4THitsCollection인 벡터타입이며 <...>의 데이터 타입을 모으는 벡터다.

extern G4ThreadLocal G4Allocator<EmCalorimeterHit>* EmCalorimeterHitAllocator;

inline void* EmCalorimeterHit::operator new(size_t) {
  if (!EmCalorimeterHitAllocator) EmCalorimeterHitAllocator = new G4Allocator<EmCalorimeterHit>;
  return (void*)EmCalorimeterHitAllocator->MallocSingle();
}

inline void EmCalorimeterHit::operator delete(void*aHit) {
  EmCalorimeterHitAllocator->FreeSingle((EmCalorimeterHit*) aHit);
}

#endif
