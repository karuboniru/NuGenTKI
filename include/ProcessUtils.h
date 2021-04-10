#ifndef __PROCESSUTILS_H__
#define __PROCESSUTILS_H__

#include "GeneratorIO.h"

using namespace GeneratorIO;

namespace ProcessUtils
{

  //=======================================================================================================================
  //                                                  variables
  //=======================================================================================================================
  TLorentzVector * muonfullp= new TLorentzVector;
  TLorentzVector * protonfullp= new TLorentzVector;
  TLorentzVector * pionfullp= new TLorentzVector;
  TLorentzVector * neutronFSfullp= new TLorentzVector;  

void IniProcessUtils()
{
  muonfullp->SetXYZT(0,0,0,0);
  protonfullp->SetXYZT(0,0,0,0);
  pionfullp->SetXYZT(0,0,0,0);
  neutronFSfullp->SetXYZT(0,0,0,0);
}

//=======================================================================================================================
//                                                  Is
//=======================================================================================================================
bool IsMuon()
{
  return (linePID==MUONBIT);
}

bool IsProton()
{
  return (linePID==PROTONBIT);
}

bool IsNeutron()
{
  return (linePID==NEUTRONBIT);
}

bool IsPion()
{
  return (linePID==PIONBIT);
}

bool IsKaon()
{
  return (linePID==KAONBIT);
}

bool IsElectron()
{
  return (linePID==ELECTRONBIT);
}

bool IsPiZero()
{
  return (linePID==PIZEROBIT);
}

bool IsGamma()
{
  return (linePID==GAMMABIT);
}

bool IsBKG()
{
  return (npar>=BKGBIT);
}

void AddABit(int &testnp, const int TESTBIT)
{
  const int ntest = (testnp%(10*TESTBIT)) /TESTBIT;
  if(ntest<8){
    testnp += TESTBIT;
  }
  else{
    //no need for actual count of number of particle. As long as it tells between 0 and non-0, and record the leading kinematics
    //printf("AnaUtils::AddABit bit overflow! %d %d %d\n", testnp, TESTBIT, ntest); exit(1);
  }
}

//=======================================================================================================================
//                                                  GetN
//=======================================================================================================================

int GetNProtons()
{
  //because of adding bit is in the if() of new leading proton, the nprotons is only such new leading protons in FV
  return (npar%(10*PROTONBIT)) /PROTONBIT;
}

int GetNNeutrons()
{
  return (npar%(10*NEUTRONBIT)) /NEUTRONBIT;
}

int GetNMuons()
{
  return (npar%(10*MUONBIT)) /MUONBIT;
}

int GetNPions()
{
  return (npar%(10*PIONBIT)) /PIONBIT;
}

//=======================================================================================================================
//                                                  Proceed
//=======================================================================================================================
void ProceedT2KGFS()
{
  const double tmpmom = lineFullMom->P();
  const double tmptheta = lineFullMom->Theta()*TMath::RadToDeg();
  //const double tmpEk = lineFullMom->E()-lineFullMom->M();
  const double tmpEk = Ekin(lineFullMom, lineMass);//only use experiment 3 momentum

  if(
     IsMuon()
     && tmpmom> 0.25 && tmptheta< 126.87 //cos > -0.6
     ){//mu- or +
    (*muonfullp)=(*lineFullMom);
    npar+= MUONBIT;
  }
  else if(
          IsProton()
          ){
    if(
       tmpmom>protonfullp->P()
       && tmpmom> 0.45 && tmpmom< 1.0 && tmptheta< 66.42 //cos > 0.4
       ){
      (*protonfullp)=(*lineFullMom);
      npar += PROTONBIT;
      CLR_KNsrc = CLR_lineKNsource;
    }
  }
  else if(
          IsPion() && (lineCharge+globalMuonCharge) == 0
          //the following is updated to PRC
          && tmpEk > 0.075 && tmpEk < 0.4
          && tmptheta<70
          ){//pion
        
    (*pionfullp)=(*lineFullMom);
    npar += PIONBIT;
  }
  else if(lineIsBkgParticle){//all mesons https://gibuu.hepforge.org/trac/wiki/ParticleIDs                                                                                                                             
    npar += BKGBIT;
  }
}

void ProceedT2KGFSEXP()
{
  const double tmpmom = lineFullMom->P();
  const double tmptheta = lineFullMom->Theta()*TMath::RadToDeg();

  if(
     IsMuon()
     && tmpmom> 0.25 && tmpmom< 7 && tmptheta< 70
     ){//mu- or +
    (*muonfullp)=(*lineFullMom);
    npar+= MUONBIT;
  }
  else if(
          IsProton()
          ){
    if(
       tmpmom>protonfullp->P()
       && tmpmom> 0.45 && tmpmom< 1.2 && tmptheta< 70
       ){
      (*protonfullp)=(*lineFullMom);
      npar += PROTONBIT;
      CLR_KNsrc = CLR_lineKNsource;
    }
  }
  else if(
          IsPion() && (lineCharge+globalMuonCharge) == 0
          && tmpmom> 0.15 && tmpmom< 1.2 && tmptheta< 70
          ){//pion
    (*pionfullp)=(*lineFullMom);
    npar += PIONBIT;
  }
  else if(lineIsBkgParticle){//all mesons https://gibuu.hepforge.org/trac/wiki/ParticleIDs
    npar += BKGBIT;
  }
}

void ProceedMINERvAMECCQE()
{
  const double tmptheta = lineFullMom->Theta()*TMath::RadToDeg();
  const double tmpmupz = lineFullMom->Pz();

  if(
     IsMuon()
     && tmpmupz>1.5 && tmptheta<20
     ){
    (*muonfullp)=(*lineFullMom);
    npar+= MUONBIT;
  }
  else if(
          IsProton()
          ){
    MMECCQE_Erecoil += Ekin(lineFullMom, lineMass);
  }
  else if(lineIsMMECCQEBkg){
    npar += BKGBIT;
  }
}

void ProceedCLR()
{
  const double tmpmom = lineFullMom->P();
  const double tmptheta = lineFullMom->Theta()*TMath::RadToDeg();

  if(
     IsMuon()
     && tmpmom> 1.5 && tmpmom< 10 && tmptheta<20
     ){
    (*muonfullp)=(*lineFullMom);
    npar+= MUONBIT;
  }
  else if(
          IsProton()
          ){
    if(
       tmpmom>protonfullp->P()
       && tmpmom> 0.45 && tmptheta<70
       ){
      (*protonfullp)=(*lineFullMom);
      npar += PROTONBIT;
      CLR_KNsrc = CLR_lineKNsource;
    }
  }
  else if(linePID == PIONBIT || linePID == PIZEROBIT){//jus tag, no use of momentum for calculation
    npar += PIONBIT;
  }
  else if(lineIsBkgParticle){
    if(globalMuonCharge == -999){
      //printf("globalMuonCharge not set before others! %d %d npar %d event %d\n", linePID, globalMuonCharge, npar, event); exit(1);
      printf("globalMuonCharge not set before others! %d %d\n", linePID, globalMuonCharge); exit(1);
    }
    npar += BKGBIT;
  }
}

void ProceedRESPS()
{
  const double tmpmom = lineFullMom->P();
  const double tmptheta = lineFullMom->Theta()*TMath::RadToDeg();

  if(
     IsMuon()
     && tmpmom> 1.5 && tmpmom< 10 && tmptheta<20
     ){
    (*muonfullp)=(*lineFullMom);
    npar+= MUONBIT;
  }
  else if(
          IsProton()
          ){
    if(
       tmpmom>protonfullp->P()
       ){
      (*protonfullp)=(*lineFullMom);
      npar += PROTONBIT;
      CLR_KNsrc = CLR_lineKNsource;
    }
  }
  else if(linePID == PIONBIT || linePID == PIZEROBIT){//jus tag, no use of momentum for calculation
    (*pionfullp)=(*lineFullMom);
    npar += PIONBIT;
  }
  else if(lineIsBkgParticle){
    if(globalMuonCharge == -999){
      //printf("globalMuonCharge not set before others! %d %d npar %d event %d\n", linePID, globalMuonCharge, npar, event); exit(1);
      printf("globalMuonCharge not set before others! %d %d\n", linePID, globalMuonCharge); exit(1);
    }
    npar += BKGBIT;
  }
}

void ProceedNUGAS()
{
  const double tmpmom = lineFullMom->P();

  if(
     IsMuon()
     ){
    (*muonfullp)=(*lineFullMom);
    npar+= MUONBIT;
  }
  else if(
          IsProton()
          ){
    if(
       tmpmom>protonfullp->P()
       ){
      (*protonfullp)=(*lineFullMom);
    }
    npar += PROTONBIT;
  }
  else if(
          IsPion() && (lineCharge+globalMuonCharge) == 0
          ){
    (*pionfullp)=(*lineFullMom);
    npar += PIONBIT;
  }
  else if(
          IsNeutron()
          ){
    if(
       tmpmom>neutronFSfullp->P()
       ){
      (*neutronFSfullp)=(*lineFullMom);
    }
    npar += NEUTRONBIT;
  }
  else if(lineIsBkgParticle){//all mesons https://gibuu.hepforge.org/trac/wiki/ParticleIDs
    if(globalMuonCharge == -999){
      //printf("globalMuonCharge not set before others! %d %d npar %d event %d\n", linePID, globalMuonCharge, npar, event); exit(1);
      printf("globalMuonCharge not set before others! %d %d\n", linePID, globalMuonCharge); exit(1);
    }
    npar += BKGBIT;
  }
  else{
    if(linePID!=0){//can happen to GiBUU
      //printf("mysterious particle!! %d %d npar %d event %d\n", linePID, globalMuonCharge, npar, event); exit(1);
      printf("mysterious particle!! %d %d\n", linePID, globalMuonCharge); exit(1);
    }
  }
}

void ProceedMINERvAGFS()
{
  const double tmpmom = lineFullMom->P();
  const double tmptheta = lineFullMom->Theta()*TMath::RadToDeg();
  //const double tmpEk = lineFullMom->E()-lineFullMom->M();
  const double tmpEk = Ekin(lineFullMom, lineMass);//only use experiment 3 momentum

  if(
     //all muons, in fact just one charge, depending on the input
     IsMuon()
     ){
    totparcount+= MUONBIT;

    if(tmpmom> 1.5 && tmpmom< 10 && tmptheta<20){//mu+ or mu-
      (*muonfullp)=(*lineFullMom);
      npar+= MUONBIT;
    }
  }
  else if(
          IsProton()
          ){
    AddABit(totparcount, PROTONBIT);
    
    if(
       tmpmom> 0.45 && tmpmom< 1.2 && tmptheta<70
       ){
      AddABit(npar, PROTONBIT);
      
      if(
         tmpmom>protonfullp->P()
         ){
        (*protonfullp)=(*lineFullMom);
        CLR_KNsrc = CLR_lineKNsource;
      }
    }
  }
  else if(//this structure only work won't work for 0pi selection!, check with: git diff 057314b6fe12798de8a68c916c68373684aa7ed7 src/Generator/AnaUtils.h
          IsPion() && (lineCharge+globalMuonCharge) == 0
          ){
    AddABit(totparcount, PIONBIT);
    
    //&& tmpmom> 0.2 && tmpmom< 4.0 
    if(tmpEk > 0.075 && tmpEk < 0.4 
       && tmptheta<70
       ){//pion
      (*pionfullp)=(*lineFullMom);
      AddABit(npar, PIONBIT);
    }
  }
  else if(lineIsBkgParticle){//all mesons https://gibuu.hepforge.org/trac/wiki/ParticleIDs                                                                                                                                                                           
    if(globalMuonCharge == -999){
      //printf("globalMuonCharge not set before others! %d %d npar %d event %d\n", linePID, globalMuonCharge, npar, event); exit(1);
      printf("globalMuonCharge not set before others! %d %d npar %d\n", linePID, globalMuonCharge, npar); exit(1);
    }
    AddABit(totparcount, BKGBIT);
    AddABit(npar, BKGBIT);
  }
}


void ProceedDUNEEXCL3()
{
  const double tmpmom = lineFullMom->P();
  const double KEthres = 3E-3; //in GeV

  //muon 3 MeV KE = 25 MeV momentum
  //https://www.wolframalpha.com/input/?i=sqrt%28%28105.65837%2B3%29%5E2-105.65837%5E2%29
  if(
     IsMuon() && tmpmom> EkinToP(MuonMass(), KEthres)
     ){
    totparcount += MUONBIT;

    npar+= MUONBIT;
    (*muonfullp)=(*lineFullMom);
  }
  //proton 3 MeV KE = 75 MeV momentum
  //https://www.wolframalpha.com/input/?i=sqrt%28%28938.272%2B3%29%5E2-938.272%5E2%29
  else if(
          IsProton() && tmpmom > EkinToP(ProtonMass(), KEthres)
          ){
    AddABit(totparcount, PROTONBIT);

    AddABit(npar, PROTONBIT);
      
    if(
       tmpmom>protonfullp->P()
       ){
      (*protonfullp)=(*lineFullMom);
    }
  }
  //pion 3 MeV KE = 29 MeV momentum
  //https://www.wolframalpha.com/input/?i=sqrt%28%28139.570%2B3%29%5E2-139.570%5E2%29
  else if(
          IsPion() && (lineCharge+globalMuonCharge) == 0 && tmpmom > EkinToP(PionMass(), KEthres)
          ){
    AddABit(totparcount, PIONBIT);

    AddABit(npar, PIONBIT);

    if(
       tmpmom>pionfullp->P()
       ){
      (*pionfullp)=(*lineFullMom);
    }
  }
  else if(lineIsBkgParticle || IsNeutron() || lineRawID==-9990001 || IsMuon() || IsProton() || IsPion() ){//neutron and also antiproton are background; mu/p/pi entering here are also non-exclusive background---they are below threshold and need to be subtracted in real measurement
    AddABit(totparcount, BKGBIT);
    AddABit(npar, BKGBIT);
  }
  else{
    //printf("DUNEEXCL3 strange background! linePID %d lineRawID %d file %d run %d event %d\n", linePID, lineRawID, filecount, run, event); exit(1);
    printf("DUNEEXCL3 strange background! linePID %d lineRawID %d\n", linePID, lineRawID); exit(1);
  }
}

void ProceedMINERvAGFS0PI()
{
  const double tmpmom = lineFullMom->P();
  const double tmptheta = lineFullMom->Theta()*TMath::RadToDeg();

  if(
     //all muons, in fact just one charge, depending on the input
     IsMuon()
     ){
    totparcount+= MUONBIT;

    if(tmpmom> 1.5 && tmpmom< 10 && tmptheta<20){//mu+ or mu-
      (*muonfullp)=(*lineFullMom);
      npar+= MUONBIT;
    }
  }
  else if(
          IsProton()
          ){
    AddABit(totparcount, PROTONBIT);
    
    if(
       tmpmom> 0.45 && tmpmom< 1.2 && tmptheta<70
       ){
      AddABit(npar, PROTONBIT);
      
      if(
         tmpmom>protonfullp->P()
         ){
        (*protonfullp)=(*lineFullMom);
        CLR_KNsrc = CLR_lineKNsource;
      }
    }
  }
  else if(lineIsBkgParticle){//all mesons https://gibuu.hepforge.org/trac/wiki/ParticleIDs                                                                                                                                                                           
    if(globalMuonCharge == -999){
      printf("globalMuonCharge not set before others! %d %d npar %d lineRawID %d event %d\n", linePID, globalMuonCharge, npar, lineRawID, event); exit(1);
    }
    AddABit(totparcount, BKGBIT);
    AddABit(npar, BKGBIT);
  }
  else if(!IsNeutron() && lineRawID!=-9990001){//allow antiproton
    printf("GFS0PI strange background! linePID %d lineRawID %d\n", linePID, lineRawID); exit(1);
  }
}

void ProceedMINERvAGFSPIZERO()
{
  const double tmpmom = lineFullMom->P();
  const double tmptheta = lineFullMom->Theta()*TMath::RadToDeg();

  if(
     IsMuon()
     ){
    totparcount += MUONBIT;

    if(
       tmpmom> 1.5 && tmpmom< 20 && tmptheta<25
       ){
      (*muonfullp)=(*lineFullMom);
      npar+= MUONBIT;
    }
  }
  else if(
          IsProton()
          ){
    AddABit(totparcount, PROTONBIT);

    if(
       tmpmom> 0.45
       ){
      AddABit(npar, PROTONBIT);

      if(
         tmpmom>protonfullp->P()
         ){
        (*protonfullp)=(*lineFullMom);
        CLR_KNsrc = CLR_lineKNsource;
      }
    }
  }
  else if(
          IsPiZero()
          ){
    AddABit(totparcount, PIONBIT);
    AddABit(npar, PIONBIT);

    if(
       tmpmom>pionfullp->P()
       ){
      (*pionfullp)=(*lineFullMom);
    }
  }
  else if(lineIsBkgParticle){
    AddABit(totparcount, BKGBIT);
    AddABit(npar, BKGBIT);
  }
  else if(!IsNeutron()){
    if(lineRawID!= -9990001){//GiBUU has large antiproton
      //printf("GFSPIZERO strange background! linePID %d lineRawID %d run %d event %d\n", linePID, lineRawID, run, event); exit(1);
      printf("GFSPIZERO strange background! linePID %d lineRawID %d\n", linePID, lineRawID); exit(1);
    }
  }
}

void AddEav(const bool kprint=false)
{
  //E_av = \sum KE_p + \sum KE_piplus + \sum KE_piminus + sum E_electrons + \sum E_Kplus + \sum E_kminus + sum E_pizero + sum E_gamma 
  double mass = -999;
  const double EPS = 1e-10;  //to ensure positivity

  //const double energy = lineFullMom->E();//experimetal energy deposit -> will have negative Eav
  const double energy = Energy(lineFullMom, lineMass);

  if(IsProton()){
    mass = ProtonMass();
  }
  else if(IsPion()){
    mass = PionMass();
  }
  else if(IsKaon() || IsElectron() || IsPiZero() || IsGamma()){
    mass = EPS;
  }
  else{//ignore all other particles
    return;
  }

  if(mass>0){
    if(energy<mass){
      printf("AnaUtils::AddEav negative kinematic energy! id %d charge %d energy %f mass %f %f LOWRECOIL_Eav %f\n", linePID, lineCharge, energy, mass, energy-mass, LOWRECOIL_Eav); exit(1);
    }

    LOWRECOIL_Eav += (energy - mass) ;

    if(kprint){
      printf("id %d charge %d energy %f mass %f %f LOWRECOIL_Eav %f\n", linePID, lineCharge, energy, mass, energy-mass, LOWRECOIL_Eav);
    }
  }
}

void ProceedMINERvALOWRECOIL()
{
  const double tmpmom = lineFullMom->P();
  const double tmptheta = lineFullMom->Theta()*TMath::RadToDeg();
  const double tmpE = lineFullMom->E();

  //static void AddEav(const int id, const int charge, const double energy);
  //static int icounter=0;
  AddEav();//(icounter++)<50);

  //add Lineenu cut in case of GENIE; for GiBUU the cut is granted by flux cut in job card
  if(IsMuon() && tmptheta<20 &&
     lineenu > 2 && lineenu < 6
     ){
    double kk = -999;
    if(lineCharge<0){//nu
      kk = tmpE;
    }
    else{//nubar
      kk = tmpmom;
    }
    if(kk > 1.5){
      (*muonfullp)=(*lineFullMom);

      const TLorentzVector dummyNu(0,0,lineenu, lineenu);
      const TLorentzVector lvq= dummyNu - (*muonfullp);
      LOWRECOIL_q3 = lvq.P();

      //only save below 0.8 to save space and time
      if(LOWRECOIL_q3<0.8){      
        npar+= MUONBIT;
      }
    }
  }
  //subsample
  else if(
          IsProton()
          ){
    if(
       tmpmom>protonfullp->P()
       && tmpmom> 0.45 && tmpmom< 1.2 && tmptheta<70
       ){
      (*protonfullp)=(*lineFullMom);
      npar += PROTONBIT;
      CLR_KNsrc = CLR_lineKNsource;
    }
  }
  else if(
          IsPion() && (lineCharge+globalMuonCharge) == 0
          && tmpmom> 0.2 && tmpmom< 4.0 && tmptheta<70
          ){//pion
    (*pionfullp)=(*lineFullMom);
    npar += PIONBIT;
  }
  else if(lineIsBkgParticle){//all mesons https://gibuu.hepforge.org/trac/wiki/ParticleIDs
    npar += BKGBIT;
  }
}

void ProceedMINERvANUBAR1PI()
{
  /*
    1) antineutrinos in the Tracker, so CH target
    2) charged-current
->    3) exact 1pi- (no other mesons, allow any number of nucleons)
    4) kinematic cuts:
->    a) 1.5 < Ev < 10 GeV
    b) W < 1.8 GeV (W plot without the W cut of course)
->    c) muon theta < 25 degrees
  */

  const double tmptheta = lineFullMom->Theta()*TMath::RadToDeg();
  const double tmpmom = lineFullMom->P();

  //add Lineenu cut in case of GENIE; for GiBUU the cut is granted by flux cut in job card 
  if(IsMuon() && tmptheta<25 &&
     lineenu > 1.5 && lineenu < 10
     ){
    (*muonfullp)=(*lineFullMom);
    npar+= MUONBIT;
  }
  //->save any leading proton also, not affecting selection
  else if(
          IsProton()
          ){
    if(tmpmom>protonfullp->P()){
      (*protonfullp)=(*lineFullMom);
      npar += PROTONBIT;
      CLR_KNsrc = CLR_lineKNsource;
    }
  }  
  //<-
  else if(
          IsPion() && (lineCharge+globalMuonCharge) == 0
          ){//pion
    (*pionfullp)=(*lineFullMom);
    npar += PIONBIT;
  }
  else if(lineIsBkgParticle){//all mesons https://gibuu.hepforge.org/trac/wiki/ParticleIDs
    if(globalMuonCharge == -999){
      printf("globalMuonCharge not set before others! %d %d\n", linePID, globalMuonCharge); exit(1);
    }
    npar += BKGBIT;
  }
}

//=======================================================================================================================
//                                                  end of namespace
//=======================================================================================================================
}

#endif
