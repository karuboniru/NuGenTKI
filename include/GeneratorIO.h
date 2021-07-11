#ifndef __GENERATORIO_H__
#define __GENERATORIO_H__

#include "AnaFunctions.h"
#include "TreeIO.h"

using namespace AnaFunctions;
using namespace TreeIO;

namespace GeneratorIO
{
  //=======================================================================================================================
  //                                                  enum
  //=======================================================================================================================

  enum dtype{
    kNULL,
    kPION,
    kNUCLEON,
    kINI,
    kFINAL
  };
  
  enum mode{
    kALL=0,
    kQE=1,
    kRES=2,
    kDIS=3,
    k2P2H=4,
    kOTHER=5
  };
  
  enum particlebit{
    PROTONBIT  =1,
    PIONBIT    =10,
    MUONBIT    =100,
    KAONBIT    =1000,
    ELECTRONBIT=10000,
    PIZEROBIT  =100000,
    GAMMABIT   =1000000,
    NEUTRONBIT =10000000,
    BKGBIT     =100000000,
    NUCLEUSBIT =1000000000
  };
  
  //=======================================================================================================================
  //                                                  variables
  //=======================================================================================================================

  TLorentzVector * beamfullp= new TLorentzVector;

  //IsIniN
  TLorentzVector * iniNfullp = new TLorentzVector;
  
  //proceed
  TLorentzVector * RESpifullp = new TLorentzVector;//--> GENIE only
  TLorentzVector * RESnucleonfullp = new TLorentzVector;//--> GENIE only
  
  TLorentzVector *lineFullMom = new TLorentzVector;
  double CLR_lineKNsource;//--> GENIE only
  int lineCharge;
  
  //SetID
  bool lineIsBkgParticle;
  bool lineIsMMECCQEBkg;
  double lineMass;
  int linePID;
  int lineRawID; 
  int globalMuonCharge;
  
void IniGeneratorIO()
{
  beamfullp->SetXYZT(0,0,0,0);

  iniNfullp->SetXYZT(0,0,0,0);

  RESpifullp->SetXYZT(0,0,0,0);
  RESnucleonfullp->SetXYZT(0,0,0,0);

  lineFullMom->SetXYZT(0,0,0,0);
  CLR_lineKNsource = -999;
  lineCharge = -999;

  lineIsBkgParticle = true;
  lineIsMMECCQEBkg = true;
  lineMass = -999;
  linePID = -999;
  lineRawID = -999;
  globalMuonCharge = -999;
}

//=======================================================================================================================
//                                                  GiBUU
//=======================================================================================================================

bool GiBUUIsIniN(const double tmppw, const double tmpmom1, const double tmpmom2, const double tmpmom3, const double tmptote, const int tmpcharge)
{
  if(fabs(tmppw)<1E-10){
    //iniNfullp->SetXYZM(tmpmom1, tmpmom2, tmpmom3, lineCharge? ProtonMass() : NeutronMass());
    iniNfullp->SetXYZT(tmpmom1, tmpmom2, tmpmom3, tmptote);//bare mass might not work in the potential
    iniNcharge = tmpcharge;

    return true;
  }
  else{
    return false;  
  }
}

int GiBUUAntiProtonID()
{
  const int tmpid = -1;
  const int tmpcharge = -1;
  return (9990000 + tmpid)*tmpcharge;
}
 
void GiBUUSetID(const int tmpid, const double tmptote)
{
  //tmpid, lineCharge -> lineIsBkgParticle, lineMass, linePID, globalMuonCharge, LOWRECOIL_parbit, lineRawID

  lineIsBkgParticle = false;
  ///minerva/app/users/xlu/cmtuser/Minerva_v10r8p12/Ana/AnalysisFramework/External/GENIEXSecExtract/apps/runXSecLooper_transverseCCQETwoTrack.cpp
  /*
    const int apd = fabs(pdg);
           if( apd == 13 ) genie_n_muons++;
           else if( apd == 22 || apd == 11 )  genie_n_photons++;
           else if( apd == 2212 ) { genie_n_protons++;  genie_n_nucleons++; }
           else if( apd == 2112 ) { genie_n_neutrons++; genie_n_nucleons++; }
           else if( apd == 211 )  { genie_n_pions++;    genie_n_mesons++; }
           else if( apd == 111 )  { genie_n_pi_zeros++; genie_n_photons++; }
           else if( apd == 411 || apd == 421 || apd == 431 ) { genie_n_charms++; genie_n_mesons++; }
           else if( apd == 321 || apd == 311 || apd == 310 || apd == 130 ) { genie_n_kaons++; genie_n_mesons++; }
           else if( apd > 3000 && apd < 5000 ) genie_n_heavy_baryons++;
           else genie_n_others++;

         }

         if( genie_n_muons         == 1 &&
             genie_n_mesons        == 0 &&
             genie_n_heavy_baryons == 0 &&
             genie_n_photons       == 0 &&
             genie_n_protons       != 0  ) {
           signal = true;
         }
   */
  //all mesons (pi, ka, charmed, all in GiBUU meson) + photon (including electron) + all heavy baryons (all s, c, b) , https://gibuu.hepforge.org/trac/wiki/ParticleIDs
  if( 
     ( tmpid>100 && tmpid<200 ) || 
     ( tmpid==999 || tmpid==901 ) || 
     ( abs(tmpid)>=32 && abs(tmpid)<=61 ) //tmpid<0 can be antiparticles
      ){
    lineIsBkgParticle = true;
  }
  
  //allow electron and photon below 10MeV
  lineIsMMECCQEBkg = false;
  if(
     ( tmpid>100 && tmpid<200 ) ||
     ( tmpid==999 && tmptote > 0.01 ) ||
     ( tmpid>=32 && tmpid<=61 )
     ){
    lineIsMMECCQEBkg = true;
  }

  lineMass = 0;
  linePID = 0;
  lineRawID = (9990000 + tmpid)*lineCharge;

  ////E_av = \sum KE_p + \sum KE_piplus + \sum KE_piminus + sum E_electrons + \sum E_Kplus + \sum E_kminus + sum E_pizero + sum E_gamma  
  if(lineCharge){
    if(tmpid == 902){
      lineMass = MuonMass(); 
      linePID = MUONBIT;

      //no need to reset muon charge for event, should be the same for the whole sample
      if(globalMuonCharge == -999){
        globalMuonCharge = lineCharge;
      }
      else if(globalMuonCharge != lineCharge){
        cout<<"\n\n ************** Muon charge not consistent! "<<globalMuonCharge<<" "<<lineCharge<<" "<<event<<endl<<endl;
        exit(1);
      }

    }
    else if(tmpid==1){
      //there are in fact quite some antiprotons in DIS (mode=34)
      /*
      if(lineCharge<0){
        cout<<"strange antiproton!!! "<<tmpline<<endl;
      }
      */
      if(lineCharge>0){
        lineMass = ProtonMass(); 
        linePID = PROTONBIT;
        LOWRECOIL_parbit += PROTONBIT;
      }
    }
    else if(tmpid==101){
      lineMass = PionMass();
      linePID = PIONBIT;
      LOWRECOIL_parbit += PIONBIT;
    }
    else if(tmpid==110||tmpid==111){
      lineMass = KaonMass();
      linePID = KAONBIT;
      LOWRECOIL_parbit += KAONBIT;
    }
    else if(tmpid==901){
      lineMass = ElectronMass();
      linePID = ELECTRONBIT;
      LOWRECOIL_parbit += ELECTRONBIT;
    }
  }//all charged
  else{
    if(abs(tmpid) == 1){//include antineutron
      lineMass = NeutronMass();
      linePID = NEUTRONBIT;
      LOWRECOIL_parbit += NEUTRONBIT;
    }
    else if(tmpid==101){
      lineMass = PiZeroMass();
      linePID = PIZEROBIT;
      LOWRECOIL_parbit += PIZEROBIT;
    }
    else if(tmpid==999){
      linePID = GAMMABIT;
      LOWRECOIL_parbit += GAMMABIT;
    }
  }

}

void SetGiBUUMode()
{
  //const TString modes[]={"all","qe","res","dis","2p2h", "other"};
  //const TString cuts[]={"1","(prod==1)", "(prod>=2 && prod<=33)", "(prod==34)", "(prod==35)", "(prod>=36)"};
  const int outmode[]={kQE, kRES, kDIS, k2P2H};
  const int pdsmin[] ={1, 2,  34, 35};
  const int pdsmax[] ={1, 33, 34, 35};
  const Int_t nmode = sizeof(outmode)/sizeof(int);

  evtMode=kOTHER;
  for(int ii=0; ii<nmode; ii++){
    if(prod>=pdsmin[ii] && prod<=pdsmax[ii]){
      evtMode=outmode[ii];
      return;
    }
  }
}

bool GiBUUProceed(const int tmpfilecount, const int tmprun, const int tmpevent, const int tmpcharge, const int tmpprod, const double tmpenu, const double tmppw, const double tmpmom1, const double tmpmom2, const double tmpmom3, const double tmptote, const int tmpid, const int tmptargetZ)
{
  if(GiBUUIsIniN(tmppw, tmpmom1, tmpmom2, tmpmom3, tmptote, tmpcharge)){
    return false;
  }
  else{
    //only save non-0 perweight so that the whole event after this is not affected!
    perweight = tmppw;
  
    //lineFullMom->SetXYZM(tmpmom1, tmpmom2, tmpmom3, mass);
    lineFullMom->SetXYZT(tmpmom1, tmpmom2, tmpmom3, tmptote);  //tmptote needed for internal calculation of q, W

    filecount = tmpfilecount;
    run = tmprun;
    event = tmpevent;
    targetZ = tmptargetZ;

    prod = tmpprod;

    beamE = tmpenu;

    lineCharge = tmpcharge;
    GiBUUSetID(tmpid, tmptote);
    SetGiBUUMode();
    
    //have to stay after IsIniN, because should not do it if it is initial
    //MainProceed();
    return true;
  }
}


//=======================================================================================================================
//                                                  GENIE
//=======================================================================================================================

void SetGENIEMode(const TString code)
{
  evtMode = kOTHER;

  if(code.Contains("QES")){
    evtMode = kQE;
  }
  else if(code.Contains("RES")){
    evtMode = kRES;
  }
  else if(code.Contains("DIS")){
    evtMode = kDIS;
  }
  else if(code.Contains("MEC")){
    evtMode = k2P2H;
  }
  else if(!code.Contains("COH") && !code.Contains("IMD")){
    printf("GeneratorIO::SetGENIEMode unknown evtMode %s\n", code.Data()); exit(1);
  }

  return;

  /*  
  //ignroe the following

  //genie mode from NEUT code
  //https://internal.dunescience.org/doxygen/neut__code__from__rootracker_8C.html

  //if      (is_qel && !is_charm && is_cc && is_nu           ) evtype =   1;
  //if (is_dis && W_gt_2 && is_cc && is_nu   ) evtype =  26;
  
  // else if (is_nu    && is_cc && is_p && np==1 && nn==0 && npip==1 && npim==0 && npi0==0 && neKL==0) evtype =  11;
  //  462        else if (is_nu    && is_cc && is_n && np==1 && nn==0 && npip==0 && npim==0 && npi0==1 && neKL==0) evtype =  12;
  //  463        else if (is_nu    && is_cc && is_n && np==0 && nn==1 && npip==1 && npim==0 && npi0==0 && neKL==0) evtype =  13;

  //all work-in-progress, can't be tursted yet!!
  const int outmode[]={kQE, kRES, kDIS, k2P2H};
  const int pdsmin[] ={1, 11, 26, 35};
  const int pdsmax[] ={1, 13, 26, 35};
  const Int_t nmode = sizeof(outmode)/sizeof(int);

  //prod has to be unsigned!
  evtMode=kOTHER;
  for(int ii=0; ii<nmode; ii++){
    if(prod>=pdsmin[ii] && prod<=pdsmax[ii]){
      evtMode=outmode[ii];
      return;
    }
  }
  */
}

void SetGENIETarget(const TString evtcode)
{
    targetZ = -999;

    if(evtcode.Contains("1000060120")){
      targetZ = 6;
    }
    else if(evtcode.Contains("1000010010")){
      targetZ = 1;
    }
    else if(evtcode.Contains("1000180400")){
      //argon
      //Atomic number18
      //Standard atomic weight (±)39.948(1)[1]
      targetZ = 18;
    }
    else if(evtcode.Contains("1000080160")){
      //oxygen
      targetZ = 8;
    }
    else if(evtcode.Contains("1000260560")){
      //Fe
      //Atomic number26
      //Standard atomic weight (±)55.845(2)[1]
      targetZ = 26;
    }
    else if(evtcode.Contains("1000822070")){
      //Pb
      //Atomic number82
      //Standard atomic weight (±)207.2(1)[1]
      targetZ = 82;
    }
    else{
      printf("unknown tmptargetZ @%s@\n", evtcode.Data());exit(1);
    }
}

void GEANT4FLUKASetID(const int pdg, const int eventID)
{
  lineCharge = (pdg>0) ? 1 : -1;

  if(abs(pdg)>9000){
    linePID = NUCLEUSBIT;
  }
  else if(abs(pdg)==211){
    linePID = PIONBIT;
    lineMass = PionMass();
  }
  else if(pdg==2212){
    linePID = PROTONBIT;
    lineMass = ProtonMass();
  }
  else if(pdg==111){
    lineCharge = 0;
    linePID = PIZEROBIT;
    lineMass = PiZeroMass();
  }
  else if(pdg==2112){
    lineCharge = 0;
    linePID = NEUTRONBIT;
    lineMass = NeutronMass();
  }
  else if(pdg==22){
    lineCharge = 0;
    linePID = GAMMABIT;
  }
  else if(pdg==321){
    linePID = KAONBIT;
  }
  else if(pdg==130||pdg==310||pdg==311){
    lineCharge = 0;
    linePID = KAONBIT;
  }
  else if(pdg>3000){//strangeness and charm etc.  3122 is Lambda
    linePID = KAONBIT;
  }
  else if(abs(pdg)==11){
    linePID = ELECTRONBIT;
  }
  else if(abs(pdg)==13){
    linePID = MUONBIT;
  }
  else{
    printf("GeneratorIO::GEANT4FLUKASetID not known pdg %d eventID %d\n", pdg, eventID); exit(1);
  }
}

void GENIESetID(const int pdg, const double tmptote)
{
  //GiBUU: tmpid, lineCharge -> lineIsBkgParticle, lineMass, linePID, globalMuonCharge, LOWRECOIL_parbit, lineRawID
  //GENIE: pdg -> lineCharge,   lineIsBkgParticle, lineMass, linePID, globalMuonCharge, LOWRECOIL_parbit, lineRawID

  const int tmppdg = abs(pdg);
  ///minerva/app/users/xlu/cmtuser/Minerva_v10r8p12/Ana/AnalysisFramework/External/GENIEXSecExtract/apps/runXSecLooper_transverseCCQETwoTrack.cpp  
  /*
          if( apd == 13 ) genie_n_muons++;
           else if( apd == 22 || apd == 11 )  genie_n_photons++;
           else if( apd == 2212 ) { genie_n_protons++;  genie_n_nucleons++; }
           else if( apd == 2112 ) { genie_n_neutrons++; genie_n_nucleons++; }
           else if( apd == 211 )  { genie_n_pions++;    genie_n_mesons++; }
           else if( apd == 111 )  { genie_n_pi_zeros++; genie_n_photons++; }
           else if( apd == 411 || apd == 421 || apd == 431 ) { genie_n_charms++; genie_n_mesons++; }
           else if( apd == 321 || apd == 311 || apd == 310 || apd == 130 ) { genie_n_kaons++; genie_n_mesons++; }
           else if( apd > 3000 && apd < 5000 ) genie_n_heavy_baryons++;
           else genie_n_others++;

         }

         if( genie_n_muons         == 1 &&
             genie_n_mesons        == 0 &&
             genie_n_heavy_baryons == 0 &&
             genie_n_photons       == 0 &&
             genie_n_protons       != 0  ) {
           signal = true;
         }
   */
  lineIsBkgParticle = false;

  //ignore nuclei
  if(tmppdg>=5000 && tmppdg<1E9){
    printf("GeneratorIO::GENIESetID bad pdg %d\n", pdg); exit(1);
  }

  //two quarks (meson) 3 digits http://pdg.lbl.gov/2007/reviews/montecarlorpp.pdf  The general form is a 7–digit number: ±n nr nL nq1 nq2 nq3 nJ
  if( 
     (tmppdg>99 && tmppdg<1000) || 
     ( tmppdg == 22 || tmppdg == 11 ) ||
     ( tmppdg > 3000 && tmppdg < 5000 )
     //diquark !
     || ( tmppdg == 2103 ) || ( tmppdg == 2203 )
      ){
    lineIsBkgParticle = true;
  }

  //allow electron and photon below 10MeV
  lineIsMMECCQEBkg = false;
  if(
     (tmppdg>99 && tmppdg<1000) ||
     ( tmppdg == 22 && tmptote > 0.01 ) ||
     ( tmppdg > 3000 && tmppdg < 5000 )
     ){
    lineIsMMECCQEBkg = true;
  }

  lineCharge = (pdg>0? 1 : -1);

  linePID = 0;
  lineMass = 0;
  lineRawID = tmppdg;

  if(tmppdg==13){
    lineCharge *= -1;
    linePID = MUONBIT;
    lineMass = MuonMass();

    //no need to reset muon charge for event, should be the same for the whole sample                                                                                                                                                                       
    if(globalMuonCharge == -999){
      globalMuonCharge = lineCharge;
    }
    else if(globalMuonCharge != lineCharge){
      cout<<"\n\n ************** Muon charge not consistent! "<<globalMuonCharge<<" "<<lineCharge<<endl<<endl;
      exit(1);
    }
  }
  else if(tmppdg==11){
    lineCharge *= -1;
    linePID = ELECTRONBIT;
    lineMass = ElectronMass();
    LOWRECOIL_parbit += ELECTRONBIT;
  }
  else if(tmppdg==2212){
    linePID = PROTONBIT;
    lineMass = ProtonMass();
    LOWRECOIL_parbit += PROTONBIT;
  }
  else if(tmppdg==211){
    linePID = PIONBIT;
    lineMass = PionMass();
    LOWRECOIL_parbit += PIONBIT;
  }
  else if(tmppdg==321){
    linePID = KAONBIT;
    lineMass = KaonMass();
    LOWRECOIL_parbit += KAONBIT;
  }
  //neutrals now
  else if(tmppdg==111){
    lineCharge = 0;
    linePID = PIZEROBIT;
    lineMass = PiZeroMass();
    LOWRECOIL_parbit += PIZEROBIT;
  }
  else if(tmppdg==130||tmppdg==310||tmppdg==311){
    lineCharge = 0;
    linePID = KAONBIT;
    LOWRECOIL_parbit += KAONBIT;
  }
  else if(tmppdg==2112){
    lineCharge = 0;
    linePID = NEUTRONBIT;
    lineMass = NeutronMass();
    LOWRECOIL_parbit += NEUTRONBIT;
  }
  else if(tmppdg==22){
    lineCharge = 0;
    linePID = GAMMABIT;
    LOWRECOIL_parbit += GAMMABIT;
  }
}

bool GEANT4Proceed(const int eventID, const int tmpMode, const int tmppdg, const double tmppx, const double tmppy, const double tmppz, const double tmpE)
{
  //missing iniN-->
  //<---

  if(beamE<0){//not initialized, should correspond to [0] of particle vectors
    beamfullp->SetXYZT(tmppx, tmppy, tmppz, tmpE);//input unit is MeV
    (*beamfullp) *= 1E-3;
    
    beamE = beamfullp->E();
    
    event = eventID;
    //interType==1 is inelastic scattering, =2 is elastic scattering, no other values
    evtMode = tmpMode;

    //skip beam particle
    return false;
  }

  /*
  if(tmppdg>1000000){//should encounter once per event
    if(AstarPDG==-999){
      AstarPDG = tmppdg;
    }
    else{
      const int oldA = AstarPDG%1000;
      const int newA = tmppdg%1000;
      if(newA>oldA){//take the highest
        //printf("GeneratorIO::GEANT4Proceed replacing AstarPDg %d by %d, oldA %d newA %d\n", AstarPDG, tmppdg, oldA, newA);
        AstarPDG = tmppdg;
      }
    }
    return false;
  }
  */

  if(tmppdg>1000000){
    const int lineA = (tmppdg%1000)/10;
    const int targetA = AnaFunctions::getTargetA(targetZ);
    //only skip Astar
    if(lineA> (targetA/2)){
      if(AstarPDG==-999){
        AstarPDG = tmppdg;
        AstarA = lineA;
      }
      else{
        printf("GeneratorIO::GEANT4Proceed multiple large nuclei! %d %d %d %d\n", targetA, lineA, AstarPDG, tmppdg);
        exit(1);
      }
      return false;
    }
  }
  
  lineFullMom->SetXYZT(tmppx, tmppy, tmppz, tmpE);//input unit is MeV
  (*lineFullMom) *= 1E-3;

  GEANT4FLUKASetID(tmppdg, eventID);
  return true;
}

bool FLUKAProceed(const int evt, const TLorentzVector *tmpbeam, const int tmppdg, const TLorentzVector *tmpsecondary)
{
  //printf("test EveNum %d beamm %f secondary pdg %d Px %f Py %f Pz %f E %f M %f\n", evt, beamfull->M(), tmppdg, tmpfull->Px(), tmpfull->Py(), tmpfull->Pz(), tmpfull->E(), tmpfull->M());

  if(beamE<0){
    (*beamfullp) = (*tmpbeam);

    beamE = beamfullp->E();

    event = evt;
  }

  if( abs(tmppdg) > 9000 ){
    if(tmpsecondary->M() < 1){
      printf("too light nucleus!! %d %f\n", tmppdg, tmpsecondary->M()); tmpsecondary->Print(); exit(1);
    }

    return false;//skip all nuclei

    /*
    const int targetA = AnaFunctions::getTargetA(targetZ);
    if(tmpsecondary->M() > targetA/2){//only skipping Astar 

    }
    */
  }

  (*lineFullMom) = (*tmpsecondary);

  GEANT4FLUKASetID(tmppdg, evt);
  
  return true;
}

bool GENIEProceed(const dtype IniOrFinaltype, const dtype RESdtype, const TString code, const int tmpevent, const int tmpprod, const double tmpenu, const double tmppw, const double tmpmom1, const double tmpmom2, const double tmpmom3, const double tmptote, const int tmpid, const double tmpKNsrc)
{
  if(IniOrFinaltype==kINI){
    iniNfullp->SetXYZT(tmpmom1, tmpmom2, tmpmom3, tmptote);
    return false;
  }
  else{
    //both RESpi and RESnucleon can be final-state particles
    if(RESdtype==kPION){
      RESpifullp->SetXYZT(tmpmom1, tmpmom2, tmpmom3, tmptote);
    }
    else if(RESdtype==kNUCLEON){
      RESnucleonfullp->SetXYZT(tmpmom1, tmpmom2, tmpmom3, tmptote);
    }

    //can be neither ini or final, like intermediate delta
    if(IniOrFinaltype==kFINAL){

      SetGENIETarget(code);

      if(tmpid>1000000){
        const int lineA = (tmpid%1000)/10;
        const int targetA = AnaFunctions::getTargetA(targetZ);
        //only skip Astar
        if(lineA> (targetA/2)){
          if(AstarPDG==-999){
            AstarPDG = tmpid;
            AstarA = lineA;
            //printf("testbug %d %d %d %d %s\n", lineA, targetA, tmpZ, tmpid, code.Data());
          }
          else{
            printf("GeneratorIO::GENIEProceed multiple large nuclei! %d %d %d %d\n", targetA, lineA, AstarPDG, tmpid);
            exit(1);
          }
          return false;
        }
      }

      perweight = tmppw;
      lineFullMom->SetXYZT(tmpmom1, tmpmom2, tmpmom3, tmptote);
      
      event = tmpevent;
      
      prod = tmpprod;
      
      beamE = tmpenu;      
      CLR_lineKNsource = tmpKNsrc;
      
      GENIESetID(tmpid, tmptote);
      SetGENIEMode(code);
      
      //MainProceed();
      return true;
    }
  }

  return false;

  //lineCharge,   lineIsBkgParticle, lineMass, linePID, globalMuonCharge, LOWRECOIL_parbit
  //printf("======> test evtMode %d targetZ %d event %d prod %d enu %f perweight %f --- lineCharge %d lineIsBkgParticle %d lineMass %f linePID %d globalMuonCharge %d LOWRECOIL_parbit %d -- npar %d\n", evtMode, targetZ, event, prod, enu, perweight, lineCharge, lineIsBkgParticle, lineMass, linePID, globalMuonCharge, LOWRECOIL_parbit, npar);
  /*
  cout<<"Muon"<<endl;
  muonfullp->Print();
  cout<<"Proton"<<endl;
  protonfullp->Print();
  cout<<"Pion"<<endl;
  pionfullp->Print();
  */
}

//end of namespace
}

#endif
