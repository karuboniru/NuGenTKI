#ifndef __GENERATORUTILS_H__
#define __GENERATORUTILS_H__

#include "GENIEUtils.h"

namespace GeneratorUtils
{

int GetNpi(const ULong64_t npar)
{
  //all requiring at least 1 proton
  if(npar>=101 &&npar<=109){
    return 0; //0pi
  }
  else if(npar>=111 && npar<=119){
    return 1; //1pi
  }
  else if(npar>=121 && npar<=199){
    return 2;
  }
  else{
    return 3;
  }
}

bool CutNpi(const TString cuttag, const ULong64_t npar)
{
  if(cuttag==""){
    return true;
  }
  else{
    const int npi = GetNpi(npar);

    if(cuttag.Contains("_0pi")){
      return npi==0;
    }
    else if(cuttag.Contains("_1pi")){
      return npi==1;
    }
    else if(cuttag.Contains("_Mpi")){
      return npi==2;
    }
    else{
      return npi==3;
    }
  }
}

void SetStringBaseCut(TString & modecut, const int minmode, const int maxmode)
{
  //old, only works for GiBUU
  /*
    https://gibuu.hepforge.org/Documentation/code/analysis/neutrinoAnalysis_f90.html#robo1063
    contains info on the very first neutrino-interaction with the nucleus:

    1: nucleon (QE)
    2-31: non-strange baryon resonance (as in IdTable)
    32: pi neutron-background (e.g. nu + n -> mu + pi+ + n)
    33: pi proton-background (e.g. nu + n -> mu + pi0 + p)
    34: DIS
    35: 2p2h QE
    36: 2p2h Delta
    37: two pion background

  */
  //const TString cuts[]={"1","(prod==1)", "(prod>=2 && prod<=33)", "(prod==34)", "(prod==35)", "(prod>=36)"};
  //const int pdsmin[]={0,   1, 2,  34, 35, 36};
  //const int pdsmax[]={1000,1, 33, 34, 35, 1000};

  //now use new one
  modecut = Form("(evtMode>=%d && evtMode<=%d)", minmode, maxmode);
}

bool SetTopoCut(const int anaid, ULong64_t & minnp, ULong64_t & maxnp)
{
  minnp = -999;
  maxnp = -999;
  if(anaid == CC0piNpID){
    minnp = 101;
    maxnp = 109;
  }
  else if(anaid == CC1piNpID){
    minnp = 111;
    maxnp = 119;
  }
  else if(anaid == CCMpiNpID){
    minnp = 111;
    maxnp = 199;
  }
  else if(anaid == CCEXCL3H1 || anaid == CCEXCL3H4){
    minnp = 111;
    maxnp = 111;
  }
  else if(anaid == LOWRECOIL){
    //do nothing
  }
  else if(anaid == LOWRECOIL0piNp){
    minnp = 101;
    maxnp = 109;
  }
  else if(anaid == NUBAR1PI){
    //do nothing
  }
  else if(anaid == MMECCQE){
    //do nothing
  }
  else{
    printf("anaid not known! %d\n", anaid); exit(1);
  }

  return (minnp>0);
}

TString SetEnv(const TString fn, TString & nuExp, int & nuPDG, int & tarA)
{
  if(fn.Contains("MINERvA"))
    nuExp = "MINERvA";
  else if(fn.Contains("T2K"))
    nuExp = "T2K";
  else if(fn.Contains("DUNE"))
    nuExp = "DUNE";
  else{
    nuExp="NONE";
  }
  cout<<"nuExp "<<nuExp<<endl;

  TString tmpfilename(fn); tmpfilename.ToUpper();
  if(tmpfilename.Contains("NUBAR")){
    nuPDG = -14;
  }
  else{
    nuPDG = 14;
  }
  cout<<"nuPDG "<<nuPDG<<endl;

  if(tmpfilename.Contains("HYDROGEN")){
    tarA = 1;
  }
  else if(tmpfilename.Contains("CH")){
    tarA = 13;
  }
  else{
    tarA = 12;
  }
  printf("\n\n****************************************** Using tarA %d **********************************************\n\n\n", tarA);

  TString gen;
  if(fn.Contains("GENIE")){
    if(tarA!=13){
      printf("******************************************* GENIE only working for tarA=13! atm %d *****************************\n", tarA); exit(1);
    }

    gen="GENIE";
  }
  else if(fn.Contains("GiBUU")){
    if(tarA!=1 && tarA!=12){
      printf("******************************************* GiBUU absolutely can't have other tarA than 1 or 12!! atm %d **************************************\n", tarA);exit(1);
    }

    gen="GiBUU";
  } else if (fn.Contains("NuWro"))
  {
    gen="NuWro";
  }
  else{
    printf("wrong generator name in file name! %s\n", fn.Data());
    exit(1);
  }

  return gen;
}

double GetHistNormPerNucleus(TFile * fin, const int nuPDG, const int tarA, const TString gen, const int anaid)
{
  //
  //normalization confirmed by benchmark
  //

  int GiBUUnormFactor=-999;
  //update GiBUUnormFactor from header tree
  TTree * theader = (TTree*) fin->Get("header");
  if(theader){
    int tmpanr;
    theader->SetBranchAddress("nrun", &tmpanr);
    theader->GetEntry(0);
    if(tmpanr>0){
      GiBUUnormFactor=tmpanr;
    }
    if(tarA==1&&anaid==CCEXCL3H4){
      printf("************************************** scaling GiBUU nor by 1/4 for Hydrogen in CCEXCL3H4!!! *************************\n");
      GiBUUnormFactor *= 0.25;
    }
  }
  cout<<"GiBUUnormFactor "<<GiBUUnormFactor<<endl;

  double GENIEnormFactor = -999;
  TH1F * GENIEhCCrate=(TH1F*) fin->Get("hCCrate");
  if(GENIEhCCrate){
    //make it flux*Nnucleon
    GENIEnormFactor = GENIEUtils::GENIEgetFluxIntegral(GENIEhCCrate, nuPDG, tarA);
  }
  cout<<"GENIEnormFactor "<<GENIEnormFactor<<endl;

  double histnormFactor=1;
  if(gen=="GENIE"){
    //GENIE spline same xsec unit as GiBUU
    /*
      root [8] tot_cc->GetYaxis()->GetTitle()
      (const char* 0x98a2358)"#sigma_{nuclear} (10^{-38} cm^{2})"
    */
    //not-to-use histnormFactor=GENIEnormFactor*tarA;//spline xsec is for each nucleus, now norm with tarA gives xsec per nucleon

    histnormFactor = 1E38 * GENIEnormFactor;//spline xsec is for each nucleus, norm as per nucleus, same as GiBUU now!
  }
  else if(gen=="GiBUU"){
    //https://gibuu.hepforge.org/Documentation2017/code/analysis/neutrinoAnalysis_f90.html#robo1052
    /*
      For eventtype=5 and process_ID=CC and NC: 10^{-38} cm^2/GeV 
      All x-sec per particle (1/A) 
    */

    histnormFactor = 1E38 * GiBUUnormFactor/tarA;//perweight is for per nucleon, norm with 1/tarA gives xsec per nucleus so that can be added up with other nuclei, then get xsec per nucleon in doGetStack.sh
  }
  else{
    printf("wrong generator name! %s\n", gen.Data()); exit(1);
  }
  cout<<gen<<" histnormFactor "<<histnormFactor<<endl;

  return histnormFactor;
}

//end of namespace
}

#endif
