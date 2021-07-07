#ifndef __READFLUKA_H__
#define __READFLUKA_H__

namespace READFLUKA
{
   Int_t           RunNum;
   Int_t           EveNum;
   Int_t           NIncHits;
   Int_t           IdInc[1];   //[NIncHits]
   Int_t           IdParInc[1];   //[NIncHits]
   Int_t           TrInc[1];   //[NIncHits]
   Int_t           ParTrInc[1];   //[NIncHits]
   Int_t           IntParInc[1];   //[NIncHits]
   Int_t           RegInc[1];   //[NIncHits]
   Float_t         PosInc[1][3];   //[NIncHits]
   Float_t         PInc[1][5];   //[NIncHits]
   Float_t         TimeInc[1];   //[NIncHits]
   Int_t           NCalHits;
   Int_t           WhCal[1];   //[NCalHits]
   Float_t         PosCal[1][3];   //[NCalHits]
   Float_t         EneCal[1];   //[NCalHits]
   Float_t         EqCal[1];   //[NCalHits]
   Int_t           NCHitPiz;
   Int_t           WhCPiz[1];   //[NCHitPiz]
   Float_t         PosCPiz[1][3];   //[NCHitPiz]
   Float_t         EneCPiz[1];   //[NCHitPiz]
   Float_t         EqCPiz[1];   //[NCHitPiz]
   Int_t           NIneHits;
   Int_t           TypeIne[1];   //[NIneHits]
   Int_t           IdIne[1];   //[NIneHits]
   Int_t           TrIne[1];   //[NIneHits]
   Int_t           IdParIne[1];   //[NIneHits]
   Int_t           IntParIne[1];   //[NIncHits]
   Float_t         PIne[1][5];   //[NIneHits]
   Float_t         PosIne[1][3];   //[NIneHits]
   Float_t         TimeIne[1];   //[NIneHits]
   Int_t           NSecIne[1];   //[NIneHits]
   Int_t           FirstSec[1];   //[NIneHits]
   Int_t           NTIneSec;
   Int_t           HitSecIne[26];   //[NTIneSec]
   Int_t           IdSecIne[26];   //[NTIneSec]
   Int_t           TrSecIne[26];   //[NTIneSec]
   Float_t         PSec[26][5];   //[NTIneSec]

   // List of branches
   TBranch        *b_RunNum;   //!
   TBranch        *b_EveNum;   //!
   TBranch        *b_NIncHits;   //!
   TBranch        *b_IdInc;   //!
   TBranch        *b_IdParInc;   //!
   TBranch        *b_TrInc;   //!
   TBranch        *b_ParTrInc;   //!
   TBranch        *b_IntParInc;   //!
   TBranch        *b_RegInc;   //!
   TBranch        *b_PosInc;   //!
   TBranch        *b_PInc;   //!
   TBranch        *b_TimeInc;   //!
   TBranch        *b_NCalHits;   //!
   TBranch        *b_WhCal;   //!
   TBranch        *b_PosCal;   //!
   TBranch        *b_EneCal;   //!
   TBranch        *b_EqCal;   //!
   TBranch        *b_NCHitPiz;   //!
   TBranch        *b_WhCPiz;   //!
   TBranch        *b_PosCPiz;   //!
   TBranch        *b_EneCPiz;   //!
   TBranch        *b_EqCPiz;   //!
   TBranch        *b_NIneHits;   //!
   TBranch        *b_TypeIne;   //!
   TBranch        *b_IdIne;   //!
   TBranch        *b_TrIne;   //!
   TBranch        *b_IdParIne;   //!
   TBranch        *b_IntParIne;   //!
   TBranch        *b_PIne;   //!
   TBranch        *b_PosIne;   //!
   TBranch        *b_TimeIne;   //!
   TBranch        *b_NSecIne;   //!
   TBranch        *b_FirstSec;   //!
   TBranch        *b_NTIneSec;   //!
   TBranch        *b_HitSecIne;   //!
   TBranch        *b_IdSecIne;   //!
   TBranch        *b_TrSecIne;   //!
   TBranch        *b_PSec;   //!

  
void SetChain(TChain *ch)
{
  fChain->SetMakeClass(0);

   ch->SetBranchAddress("RunNum", &RunNum, &b_RunNum);
   ch->SetBranchAddress("EveNum", &EveNum, &b_EveNum);
   ch->SetBranchAddress("NIncHits", &NIncHits, &b_NIncHits);
   ch->SetBranchAddress("IdInc", IdInc, &b_IdInc);
   ch->SetBranchAddress("IdParInc", IdParInc, &b_IdParInc);
   ch->SetBranchAddress("TrInc", TrInc, &b_TrInc);
   ch->SetBranchAddress("ParTrInc", ParTrInc, &b_ParTrInc);
   ch->SetBranchAddress("IntParInc", IntParInc, &b_IntParInc);
   ch->SetBranchAddress("RegInc", RegInc, &b_RegInc);
   ch->SetBranchAddress("PosInc", PosInc, &b_PosInc);
   ch->SetBranchAddress("PInc", PInc, &b_PInc);
   ch->SetBranchAddress("TimeInc", TimeInc, &b_TimeInc);
   ch->SetBranchAddress("NCalHits", &NCalHits, &b_NCalHits);
   ch->SetBranchAddress("WhCal", &WhCal, &b_WhCal);
   ch->SetBranchAddress("PosCal", &PosCal, &b_PosCal);
   ch->SetBranchAddress("EneCal", &EneCal, &b_EneCal);
   ch->SetBranchAddress("EqCal", &EqCal, &b_EqCal);
   ch->SetBranchAddress("NCHitPiz", &NCHitPiz, &b_NCHitPiz);
   ch->SetBranchAddress("WhCPiz", &WhCPiz, &b_WhCPiz);
   ch->SetBranchAddress("PosCPiz", &PosCPiz, &b_PosCPiz);
   ch->SetBranchAddress("EneCPiz", &EneCPiz, &b_EneCPiz);
   ch->SetBranchAddress("EqCPiz", &EqCPiz, &b_EqCPiz);
   ch->SetBranchAddress("NIneHits", &NIneHits, &b_NIneHits);
   ch->SetBranchAddress("TypeIne", TypeIne, &b_TypeIne);
   ch->SetBranchAddress("IdIne", IdIne, &b_IdIne);
   ch->SetBranchAddress("TrIne", TrIne, &b_TrIne);
   ch->SetBranchAddress("IdParIne", IdParIne, &b_IdParIne);
   ch->SetBranchAddress("IntParIne", IntParIne, &b_IntParIne);
   ch->SetBranchAddress("PIne", PIne, &b_PIne);
   ch->SetBranchAddress("PosIne", PosIne, &b_PosIne);
   ch->SetBranchAddress("TimeIne", TimeIne, &b_TimeIne);
   ch->SetBranchAddress("NSecIne", NSecIne, &b_NSecIne);
   ch->SetBranchAddress("FirstSec", FirstSec, &b_FirstSec);
   ch->SetBranchAddress("NTIneSec", &NTIneSec, &b_NTIneSec);
   ch->SetBranchAddress("HitSecIne", HitSecIne, &b_HitSecIne);
   ch->SetBranchAddress("IdSecIne", IdSecIne, &b_IdSecIne);
   ch->SetBranchAddress("TrSecIne", TrSecIne, &b_TrSecIne);
   ch->SetBranchAddress("PSec", PSec, &b_PSec);
}

};

#endif
