#ifndef _ANAUTILS_
#define _ANAUTILS_

#include "ProcessUtils.h"

using namespace std;
using namespace ProcessUtils;

class AnaUtils
{
 public:
  static void DoFill(TTree *tt);
  static void Ini();

  static TChain * InputROOTFiles(const TString file, const TString tr);
  static void MainProceed();

 private:
  //DoFill
  static bool IsGood();
  static TLorentzVector GetBeamFullP();
  static void Calc();
};

TChain * AnaUtils::InputROOTFiles(const TString file, const TString tr)
{
  TChain *ch=0x0;

  if(file.Contains(".root")){
    ch = new TChain(tr);
    ch->Add(file);
  }
  else{
    ifstream fin(file);
    if(!fin){
      printf("AnaUtils::InputROOTFiles file not found \n%s\n\n",file.Data()); exit(1);
    }

    TString buff;
    while(fin.good()){
      fin>>buff;
      if(buff!=""){
        if(!buff.Contains(".root")){
          return 0x0;
        }
        else{
          if(!ch){
            ch = new TChain(tr);
          }
          ch->Add(buff);
        }
      }
    }
  }

  //const Int_t ent = ch->GetEntries(); //takes infinity time!!
  printf("\t%d trees!\n",ch->GetNtrees());

  return ch;
}

//=======================================================================================================
//Generator independent procedures
//=======================================================================================================

void AnaUtils::MainProceed()
{
  (*eventfullp) += (*lineFullMom);
  
  if(expmode==kMINERvA || expmode==kNONE){
    if(anamode == GFS){
      ProceedMINERvAGFS();
    }
    else if(anamode == LOWRECOIL){
      ProceedMINERvALOWRECOIL();
    }
    else if(anamode == NUBAR1PI){
      ProceedMINERvANUBAR1PI();
    }
    else if(anamode == MMECCQE){
      ProceedMINERvAMECCQE();
    }
    else if(anamode == CLR){
      ProceedCLR();
    }
    else if(anamode == RESPS){
      ProceedRESPS();
    }
    else if(anamode == NUGAS){
      ProceedNUGAS();
    }
    else if(anamode == GFSPIZERO){
      ProceedMINERvAGFSPIZERO();
    }
    else if(anamode == GFS0PI){
      ProceedMINERvAGFS0PI();
    }
    else if(anamode == TESTBEAM){
      ProceedTESTBEAM();
    }
  }
  else if(expmode==kT2K){
    if(anamode == GFS){
      ProceedT2KGFS();
    }
    else if(anamode == GFSEXP){
      ProceedT2KGFSEXP();
    }
  }
  else if(expmode==kDUNE){
    if(anamode == EXCL3){
      ProceedDUNEEXCL3();
    }
  }
}



void AnaUtils::DoFill(TTree *tt)
{
  SetNParticles();
  if(IsGood()){
    Calc();
    tt->Fill();
    static int count = 0;
    count++;
    if(count%10000==0){
      cout<<"Fill Count "<<count<<endl;
    }
  }

  //need to do it also before first event
  //do it outside
  //Ini();
}

void AnaUtils::Ini()
{
  IniTreeIO();

  IniGeneratorIO();

  IniProcessUtils();
}

bool AnaUtils::IsGood()
{
  if(anamode==GFS || anamode==GFSEXP || anamode==CLR){
    cout<<"need to think more! stop for the moment"<<endl; exit(1);
  }

  if(anamode==GFSPIZERO || anamode==GFS0PI){
    if(GetNBkgs()){
      return false;
    }

    //1mu
    if(GetNMuons()!=1){
      return false;
    }
    
    //Np at least one proton needed for baryon kinematic calculation
    if(GetNProtons()==0){
      return false;
    }

    if(anamode==GFSPIZERO){
      //1 or more pion
      if(GetNPions()==0){
        return false;
      }
    }
    else{
      //require 0 pi 
      if(GetNPions()){
        return false;
      }
    }

    return true;

    /*
    //1mu
    //0pi at least 1 p
    //1pi any p
    return (npar >= 101 && npar < 120);
    */
  }
  else if(anamode==RESPS){
    //1mu
    if(GetNMuons()!=1){
      return false;
    }

    //0 or 1pi
    if(GetNPions()>1){
      return false;
    }

    if(GetNBkgs()){
      return false;
    }

    //only care about RES
    if(evtMode!=2){
      return false;
    }

    return true;
  }
  else if(anamode==MMECCQE){
    if(GetNMuons()!=1){
      return false;
    }

    if(GetNBkgs()){
      return false;
    }

    return true;
  }
  else if(anamode==LOWRECOIL){
    return (GetNMuons()==1);
  }
  else if(anamode==NUBAR1PI){
    if(GetNMuons()!=1)
      return false;

    if(GetNPions()!=1)
      return false;

    if(GetNBkgs()){
      return false;
    }

    return true;
  }
  else if(anamode==NUGAS){
    if(GetNMuons()!=1)
      return false;
    
    if(targetZ!=1)
      return false;

    if(GetNBkgs())
      return false;

    return true;
    /*
    if(GetNBkgs()){
      return false;
    }

    if(GetNNeutrons()==1 && GetNProtons()==0 && GetNPions()==0)//antinu QE
      return true;

    if(GetNNeutrons()==0 && GetNProtons()==1 && GetNPions()==1)//RES
      return true;

    return false;
    */
  }
  else if(anamode==EXCL3){
    if(GetNBkgs()){
      return false;
    }

    //1mu
    if(GetNMuons()!=1){
      return false;
    }

    //1p
    if(GetNProtons()!=1){
      return false;
    }

    //1pi
    if(GetNPions()!=1){
      return false;
    }

    return true;
  }
  else if(anamode==TESTBEAM){
    //accept Npmpi
    //1p
    if(1)//accept all (nPion+nPiZero) ==1 && (nProton==1||nProton==2) )    
      return true;
    else
      return false;
  }
  else{
    printf("unknown anamode %d\n", anamode); exit(1);
  }
}

TLorentzVector AnaUtils::GetBeamFullP()
{
  if(beamfullp->E()>1E-3){//larger than 1 MeV
    return *beamfullp;
  }
  else{
  double beamMass = 0;
  static int kprintbeammass=1;
  if(anamode == TESTBEAM){
    beamMass = PionMass();

    if(kprintbeammass){
      printf("\n\n*********************** setting beam mass to %f ***********************\n\n", beamMass);
      kprintbeammass=0;
    }
  }

  const double pz = TMath::Sqrt(beamE*beamE-beamMass*beamMass);
  //printf("debug testgetbeam %f %f %f\n", beamMass, beamE, pz);
  
  TLorentzVector beamp(0,0,pz,beamE);

  return beamp;
  }
}


void AnaUtils::Calc()
{
  const TLorentzVector beamFullP=GetBeamFullP();
  const TLorentzVector lvq= beamFullP - (*PU4pScatter);

#if __OPENCALC__
  //---muon: 2
  {//use blocks to isolate calculations
    scattermomentum = PU4pScatter->P();
    scattertheta = PU4pScatter->Theta()*TMath::RadToDeg();
    
    //---W: 5
    //not used q3 = lvq.P();
    Q2 = -lvq.M2();
    TLorentzVector dummyP, dummyW;
    //assum proton target
    dummyP.SetXYZM(0,0,0,ProtonMass());
    dummyW = lvq+dummyP;
    Wrest = dummyW.M();
    xrest = Q2/2/(dummyP.Dot(lvq));
    q0 = lvq.E();
    q3 = lvq.P();
      
    dummyW = lvq+(*iniNfullp);
    Wtrue = dummyW.M();
    xBj = Q2/2/(iniNfullp->Dot(lvq));
  }

  {
    //---hadron: 8; tmphadronfullp is intermediate locally
    recoilmomentum = PU4pRecoil->P();
    recoiltheta = PU4pRecoil->Theta()*TMath::RadToDeg();
    mesonmomentum = PU4pMeson->P();
    mesontheta = PU4pMeson->Theta()*TMath::RadToDeg();
    //pionEk = PU4pMeson->E()-PionMass();
    mesonEk = Ekin(PU4pMeson, PionMass()); //only use experimental momentum
  }
  
  {
    //pretend hydrogen is carbon, same as experiment
    const int localZ = (targetZ==1 ? 6 : targetZ);
    const int localA = AnaFunctions::getTargetA(localZ);

    //GiBUU 4-momentum doesn't give physical mass; but using its given 4-momentum provides direct access to internal dynamics like pN
    const TLorentzVector tmphadronfullp = (*PU4pRecoil) + (*PU4pMeson);//this is more logical
    //this will cause delta<0 in TESTBEAM; only used for neutrino. 
    //to repeat old results TLorentzVector tmphadronfullp; tmphadronfullp.SetXYZT(PU4pRecoil->X()+PU4pMeson->X(), PU4pRecoil->Y()+PU4pMeson->Y(), PU4pRecoil->Z()+PU4pMeson->Z(), Energy(PU4pRecoil, ProtonMass())+Energy(PU4pMeson, PU4pMeson->P()>1E-10? PionMass():0));//need to use experimental momentum only
    
    baryonmomentum = tmphadronfullp.P();
    baryontheta = tmphadronfullp.Theta()*TMath::RadToDeg();
    baryonmass = tmphadronfullp.M();
    
    AnaFunctions::getCommonTKI(localA, localZ, &beamFullP, PU4pScatter, &tmphadronfullp, dalphat, dphit, dpt, dpTT, beamCalcP, IApN, recoilM, recoilP);

    double dummy = -999;
    AnaFunctions::getCommonTKI(localA, localZ, &beamFullP, 0x0,       eventfullp,  dummy, dummy, dummy,dummy, event_beamCalcP, event_IApN, event_recoilM, event_recoilP);

  }
  
  /*
    protonTT = PU4pRecoil->Vect().Dot(ztt);
    pionTT = PU4pMeson->Vect().Dot(ztt);
  */

  //dpTy = dpt * cos(dalphat*TMath::DegToRad());
#endif

#if __OPENCLR__
  const TVector3 ztt = (beamFullP.Vect().Cross(PU4pScatter->Vect())).Unit();    

  const double ares = -0.3; //0.2

  lrsign = 0;
  if(PU4pRecoil->E()>1E-10){
    //right is -1, left is +1
    lrsign = PU4pRecoil->Vect().Dot(ztt) > 0 ? -1 : 1;

    //just use the unweighted lrsign (original one)
    pseudosign = lrsign;
  }

  RESmass = -999;
  adlerPhi = -999;
  pseudoPhi = -999;
  w2 = 1;
  wpseudo2 = 1;
  if(RESpifullp->E()>1E-10 && RESnucleonfullp->E()>1E-10){
    const TLorentzVector ressum = (*RESpifullp) + (*RESnucleonfullp);
    RESmass = ressum.M();
    
    //double GetAdlerPhi(TLorentzVector nufull, TLorentzVector muonfull, TLorentzVector pifull, TLorentzVector nucleonfull, TLorentzVector iniNfull)
    adlerPhi = GetOneBoostAdlerPhi(beamFullP, *PU4pScatter, *RESpifullp, *RESnucleonfullp, *iniNfullp);

    /*
    //->test
    const double tbadler = GetTwoBoostAdlerPhi(beamFullP, *PU4pScatter, *RESpifullp, *RESnucleonfullp, *iniNfullp);
    if(fabs(adlerPhi-tbadler)>1e-10){
      printf("one-boost and two-boost not consistent!\n");
      exit(1);
    }
    exit(1);
    //<--
    */

    //by weighting the event with lrsign, the adlerPhi weight is not needed
    w2 = GetW2(adlerPhi, ares);
    lrsign *= w2;

    //============= repeat incorrect calculation in CLR paper first version =====>
    pseudoPhi = GetPseudoPhi(beamFullP, *PU4pScatter, *RESpifullp, *RESnucleonfullp);
    wpseudo2 = GetW2(pseudoPhi, ares);
    pseudosign *= wpseudo2;
  }

  cosNuIniNAngle = ((beamFullP.Vect()).Unit()).Dot((iniNfullp->Vect()).Unit());
  cosQIniNAngle = ((lvq.Vect()).Unit()).Dot((iniNfullp->Vect()).Unit());
#endif

#if __OPENNEUTRONFS__
  neutronFSmomentum = neutronFSfullp->P();
  neutronFStheta = neutronFSfullp->Theta()*TMath::RadToDeg();
#endif

#if __OPENDPTT__
  //add 20 MeV Cauchy smearing
  dpTT20s = dpTT+SmeardpTT(20);
  dpTT10s = dpTT+SmeardpTT(10);
  dpTT5s  = dpTT+SmeardpTT(5);
  dpTT2s  = dpTT+SmeardpTT(2);
#endif

#if __OPENMMECCQE__
  //scattercostheta = TMath::Cos( PU4pScatter->Theta()  );
  scatterpt = PU4pScatter->Pt();
  scatterpz = PU4pScatter->Pz();
  q2qe = GetTrueCCQEQ2(scattermomentum, PU4pScatter->Theta());
#endif 


}


#endif
