#ifndef __READGEANT4_H__
#define __READGEANT4_H__

//#include "GeneratorIO.h"

namespace ReadGEANT4
{
   // Declaration of leaf types
   vector<int>     *PDGcode;
   vector<int>     *interType;
   vector<double>  *massPart;
   vector<double>  *X;
   vector<double>  *Y;
   vector<double>  *Z;
   vector<double>  *Px;
   vector<double>  *Py;
   vector<double>  *Pz;
   vector<double>  *E;

   // List of branches
   TBranch        *b_PDGcode;   //!
   TBranch        *b_interType;   //!
   TBranch        *b_massPart;   //!
   TBranch        *b_X;   //!
   TBranch        *b_Y;   //!
   TBranch        *b_Z;   //!
   TBranch        *b_Px;   //!
   TBranch        *b_Py;   //!
   TBranch        *b_Pz;   //!
   TBranch        *b_E;   //!


void SetChain(TChain *ch)
{
  ch->SetMakeClass(0);
  // Set object pointer
   PDGcode = 0;
   interType = 0;
   massPart = 0;
   X = 0;
   Y = 0;
   Z = 0;
   Px = 0;
   Py = 0;
   Pz = 0;
   E = 0;

   ch->SetBranchAddress("PDGcode", &PDGcode, &b_PDGcode);
   ch->SetBranchAddress("interType", &interType, &b_interType);
   ch->SetBranchAddress("massPart", &massPart, &b_massPart);
   ch->SetBranchAddress("X", &X, &b_X);
   ch->SetBranchAddress("Y", &Y, &b_Y);
   ch->SetBranchAddress("Z", &Z, &b_Z);
   ch->SetBranchAddress("Px", &Px, &b_Px);
   ch->SetBranchAddress("Py", &Py, &b_Py);
   ch->SetBranchAddress("Pz", &Pz, &b_Pz);
   ch->SetBranchAddress("E", &E, &b_E);
}

};

//end of namespace
#endif
