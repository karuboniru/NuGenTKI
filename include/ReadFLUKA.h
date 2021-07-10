#ifndef __READFLUKA_H__
#define __READFLUKA_H__


const Int_t MAXHITS = 400000;
const Int_t MAXINEHITS = 400000;
const Int_t MXS = 500000;

namespace ReadFLUKA
{
   Int_t    RunNum;		// Run number
   Int_t    EveNum;		// Event number
   //
   Int_t    NIncHits;
   Int_t    IdInc[MAXHITS];
   Int_t    IdParInc[MAXHITS];
   Int_t    TrInc[MAXHITS];
   Int_t    IntParInc[MAXHITS];
   Int_t    RegInc[MAXHITS];
   Int_t    ParTrInc[MAXHITS];   // track id of the parent
   Float_t PosInc[MAXHITS][3];	// position of hit 
   Float_t PInc[MAXHITS][5];	//momentum/ energy of crossing particle
   Float_t TimeInc[MAXHITS];	// time
   //
   Int_t    NCalHits;			// number of energy "blobs" in EM calo
  //   Float_t EdepCal;			// total en deposited in LAR
  //  Float_t EdepECal;			// energy deposited in LAR by electrom. particles
   Int_t    WhCal[MAXHITS];		// calo part
   Float_t PosCal[MAXHITS][3];	// position of n-th blob
   Float_t EneCal[MAXHITS];	//
     Float_t EqCal[MAXHITS];	// 
  //   Float_t EdepCal;			// total en deposited in LAR
  //  Float_t EdepECal;			// energy deposited in LAR by electrom. particles
   Int_t    NCHitPiz;			// number of energy "blobs" in EM calo
   Int_t    WhCPiz[MAXHITS];		// calo part
   Float_t PosCPiz[MAXHITS][3];	// position of n-th blob
   Float_t EqCPiz[MAXHITS];	//
     Float_t EneCPiz[MAXHITS];	// 
  //   Float_t TimeCal[MAXHITS];	// time
  //
  Int_t   NIneHits;  // number of inelastic/decay
  Int_t    TypeIne[MAXINEHITS];  // int. type (fluka coding)
  Int_t    IdIne[MAXINEHITS];
  Int_t    TrIne[MAXINEHITS]; // track number
  Int_t    IdParIne[MAXINEHITS]; //original parent
  Int_t    IntParIne[MAXHITS]; // parent from last interaction
  Float_t PIne[MAXINEHITS][5];  //momentum
   Float_t PosIne[MAXINEHITS][3];	// position of hit 
   Float_t TimeIne[MAXINEHITS];	// time
  Int_t   NSecIne[MAXINEHITS];  //number of secondaries in this hit
  Int_t    FirstSec[MAXINEHITS]; //position of first secondary from this hit
  Int_t   NTIneSec;  //total number of secondaries
  Float_t PSec[MXS][5];  //momentum,total energy,
  Int_t    TrSecIne[MXS]; // track number
  Int_t   HitSecIne[MXS];  // hit where particle was produced
  Int_t    IdSecIne[MXS];  //

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
  ch->SetMakeClass(0);

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
