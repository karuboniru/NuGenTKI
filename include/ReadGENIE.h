#ifndef __READGENIE_H__
#define __READGENIE_H__

#include "GeneratorIO.h"
#include <TObjString.h>

namespace ReadGENIE
{
  
  //500 is needed for Pb
  const int _ARRAYSIZE_ = 500;

  // Declaration of leaf types                                                                                                                                                                                                                                       
  TObjString      *gEvtCode;

  //Int_t           G2NeutEvtCode;
  Int_t           EvtNum;
  Double_t        EvtXSec;
  Double_t        EvtDXSec;
  Double_t        EvtWght;
  Double_t        missE; // used for NuWro

  Double_t        EvtProb;
  Double_t        EvtVtx[4];
  Int_t           StdHepN;
  Int_t           StdHepPdg[_ARRAYSIZE_];
  Int_t           StdHepStatus[_ARRAYSIZE_];

  Int_t           StdHepRescat[_ARRAYSIZE_];
  Double_t        StdHepX4[_ARRAYSIZE_][4];
  Double_t        StdHepP4[_ARRAYSIZE_][4];
  Double_t        StdHepPolz[_ARRAYSIZE_][3];
  Int_t           StdHepFd[_ARRAYSIZE_];

  Int_t           StdHepLd[_ARRAYSIZE_];
  Int_t           StdHepFm[_ARRAYSIZE_];
  Int_t           StdHepLm[_ARRAYSIZE_];

  // List of branches                                                                                                                                                                                                                                                
  TBranch        *b_gEvtCode;

  //TBranch        *b_G2NeutEvtCode;
  TBranch        *b_EvtNum;
  TBranch        *b_EvtXSec;
  TBranch        *b_EvtDXSec;
  TBranch        *b_EvtWght;
  TBranch        *b_missE;

  TBranch        *b_EvtProb;
  TBranch        *b_EvtVtx;
  TBranch        *b_StdHepN;
  TBranch        *b_StdHepPdg;
  TBranch        *b_StdHepStatus;

  TBranch        *b_StdHepRescat;
  TBranch        *b_StdHepX4;
  TBranch        *b_StdHepP4;
  TBranch        *b_StdHepPolz;
  TBranch        *b_StdHepFd;

  TBranch        *b_StdHepLd;
  TBranch        *b_StdHepFm;
  TBranch        *b_StdHepLm;

void SetChain(TChain *ch)
{
  //only 0 works!!                                                                                                                                                                                                                                                   
  ch->SetMakeClass(0);
  gEvtCode = new TObjString;

  ch->SetBranchAddress("EvtCode", &gEvtCode, &b_gEvtCode);

  //ch->SetBranchAddress("G2NeutEvtCode", &G2NeutEvtCode, &b_G2NeutEvtCode);
  ch->SetBranchAddress("EvtNum", &EvtNum, &b_EvtNum);
  ch->SetBranchAddress("EvtXSec", &EvtXSec, &b_EvtXSec);
  ch->SetBranchAddress("EvtDXSec", &EvtDXSec, &b_EvtDXSec);
  ch->SetBranchAddress("EvtWght", &EvtWght, &b_EvtWght);
  ch->SetBranchAddress("missE", &missE, &b_missE);

  ch->SetBranchAddress("EvtProb", &EvtProb, &b_EvtProb);
  ch->SetBranchAddress("EvtVtx", EvtVtx, &b_EvtVtx);
  ch->SetBranchAddress("StdHepN", &StdHepN, &b_StdHepN);
  ch->SetBranchAddress("StdHepPdg", StdHepPdg, &b_StdHepPdg);
  ch->SetBranchAddress("StdHepStatus", StdHepStatus, &b_StdHepStatus);

  ch->SetBranchAddress("StdHepRescat", StdHepRescat, &b_StdHepRescat);
  ch->SetBranchAddress("StdHepX4", StdHepX4, &b_StdHepX4);
  ch->SetBranchAddress("StdHepP4", StdHepP4, &b_StdHepP4);
  ch->SetBranchAddress("StdHepPolz", StdHepPolz, &b_StdHepPolz);
  ch->SetBranchAddress("StdHepFd", StdHepFd, &b_StdHepFd);

  ch->SetBranchAddress("StdHepLd", StdHepLd, &b_StdHepLd);
  ch->SetBranchAddress("StdHepFm", StdHepFm, &b_StdHepFm);
  ch->SetBranchAddress("StdHepLm", StdHepLm, &b_StdHepLm);
}

GeneratorIO::dtype GetIniOrFinalType(const int ii, const bool isHydrogen, const int tmpevent, const int tmpid)
{
  //https://github.com/luketpickering/NeutToRooTracker
  //If the StdHepStatus != 1 then you didn't see that particle. Always check the StdHepStatus.
  //Int_t StdHepStatus[$i<$StdHepN] //Status code of particle i --- 0 == incoming, 11 == incoming nucleus (if not using -E), 1 == good final state, anything else == intermediate or unseeable particle.
  GeneratorIO::dtype IniOrFinaltype = GeneratorIO::kNULL;
  if(isHydrogen){
    if(ii==1){//for hydrogen, initial nucleon at ii=1
      IniOrFinaltype = GeneratorIO::kINI;
    }
  }
  else{
    if(StdHepStatus[ii]==11){//initial nucleon has status 11
      IniOrFinaltype = GeneratorIO::kINI;
    }
  }
  
  if(IniOrFinaltype==GeneratorIO::kINI){
    //tmpid can be 2000000201 for MEC
    if(!isHydrogen && StdHepFm[ii]!=1){
      printf("wrong initial nucleon event %d -- %d %d\n", tmpevent, StdHepFm[ii], tmpid); exit(1);
    }
  }

  //important to check status==1 for all cases
  if(StdHepStatus[ii]==1){
    IniOrFinaltype=GeneratorIO::kFINAL;
  }
  
  if(StdHepStatus[ii]==2){
    const double bit9 = tmpid/1E9;
    if(bit9<=1 || bit9 >=2){//there is large pid like 2000000001 with status 12, nu:14;tgt:1000060120;N:2212;q:1(v);proc:Weak[CC],DIS; 2000000001 12
      printf("ReadGENIE GetIniOrFinalType wrong id %d %f status %d\n", tmpid, bit9, StdHepStatus[ii]);exit(1);
    }
    IniOrFinaltype=GeneratorIO::kFINAL;
  }    

  return IniOrFinaltype;
}

GeneratorIO::dtype GetRESType(const int ii, const TString ecode, const GeneratorIO::dtype IniOrFinaltype, int & idxIni, int & idxDelta, int & idxRESnucleon, int & idxRESpi)
{
  GeneratorIO::dtype RESdtype = GeneratorIO::kNULL;
  if(ecode.Contains("RES")){
    //particle list ordered by history; idx settings will be used for next loop
    if(IniOrFinaltype==GeneratorIO::kINI){//initial nucleon
      idxIni = ii;
    }
    else if(//resonance
            StdHepFm[ii]==idxIni 
            && StdHepStatus[ii]!=13 //13 has no daughters
            ){
          
      if(StdHepStatus[ii]!=3){
        printf("\n\n\nGENIE RES resonance is not status 3! Strange!\n\n\n"); exit(1);
      }
      if( (StdHepLd[ii]-StdHepFd[ii])!=1 ){
        printf("\n\n\nGENIE RES resonance daughters not next to ech other! Strange!\n\n\n"); exit(1);
      }

      //cout<<"test event "<<tmpevent<<" ii "<<ii<<" idxIni "<<idxIni<<" ecode "<<ecode<<" status "<<StdHepStatus[ii]<<" pdg "<<StdHepPdg[ii]<<" scat "<<StdHepRescat[ii]<<" StdHepFd "<<StdHepFd[ii]<<" StdHepLd "<<StdHepLd[ii]<<" "<<endl;
      
      idxDelta = ii;

      const int tmppdg = StdHepPdg[StdHepFd[ii]];
      if(tmppdg>999){//nucleon
        idxRESnucleon = StdHepFd[ii];
        idxRESpi    = StdHepLd[ii];
      }
      else{
        idxRESpi    = StdHepFd[ii];
        idxRESnucleon = StdHepLd[ii];
      }
    }
    else if(ii == idxRESpi){
      RESdtype = GeneratorIO::kPION;
    }
    else if(ii == idxRESnucleon){
      RESdtype = GeneratorIO::kNUCLEON;
    }
  }

  return RESdtype;

  //test-->
  /*      
          if(RESdtype>GeneratorIO::kNULL){
          cout<<"test2 event "<<tmpevent<<" ii "<<ii<<" idxIni "<<idxIni<<" ecode "<<ecode<<" status "<<StdHepStatus[ii]<<" pdg "<<StdHepPdg[ii]<<" scat "<<StdHepRescat[ii]<<" StdHepFd "<<StdHepFd[ii]<<" StdHepLd "<<StdHepLd[ii]<<" RESdtype "<<RESdtype<<" "<<endl;
          }
  */
  //<--
}

int GetKNsource(const int ii, const GeneratorIO::dtype IniOrFinaltype, const int idxDelta)
{
  //printf("testjj %d %d %d %d\n", IniOrFinaltype, GeneratorIO::kFINAL, RESdtype, GeneratorIO::kNUCLEON);

  double tmpKNsrc = 0; //proton 1; pion -1

  if(IniOrFinaltype==GeneratorIO::kFINAL && StdHepPdg[ii]==2212 &&idxDelta>=0){//final-state proton
    int cursor = ii;
    int motherid=StdHepFm[cursor];
        
    while(motherid>idxDelta){
      cursor = motherid;
      motherid = StdHepFm[cursor];
    }
    
    if(motherid<idxDelta){
      printf("\n\n\ntmpKNsrc not found %d %d\n\n\n", motherid, idxDelta); exit(1);
    }
    else{
      if(StdHepPdg[cursor]>999){//nucleon
        tmpKNsrc = 1;
      }
      else{//pion
        tmpKNsrc = -1;
      }
    }
    
  }

  //cout<<"test3 event "<<tmpevent<<" ii "<<ii<<" idxIni "<<idxIni<<" ecode "<<ecode<<" status "<<StdHepStatus[ii]<<" pdg "<<StdHepPdg[ii]<<" scat "<<StdHepRescat[ii]<<" StdHepFd "<<StdHepFd[ii]<<" StdHepLd "<<StdHepLd[ii]<<" RESdtype "<<RESdtype<<" idxDelta "<<idxDelta<<" tmpKNsrc "<<tmpKNsrc<<" "<<endl;     if(tmpevent==15)   exit(1);
      
  return tmpKNsrc;
}

//end of namespace
}
#endif
