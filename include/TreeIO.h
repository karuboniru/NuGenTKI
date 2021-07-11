#ifndef __TREEIO_H__
#define __TREEIO_H__

//all __OPEN__ tag only show up in Calc
#define __OPENCALC__ 1
#define __OPENCLR__ 0
#define __OPENNEUTRONFS__ 0
#define __OPENDPTT__ 0
#define __OPENMMECCQE__ 0

using namespace std;

namespace TreeIO
{

  //=======================================================================================================================
  //                                                  enum
  //=======================================================================================================================
  enum analysis{
    GFS       =0,  
    LOWRECOIL =1,
    NUBAR1PI  =2,
    GFSEXP    =3,
    MMECCQE   =4,
    CLR       =5,
    RESPS     =6,
    GFSPIZERO =7,
    NUGAS     =8,
    GFS0PI    =9,
    EXCL3 = 10,
    TESTBEAM = 11
  };
  
  enum experiment{
    kNONE,
    kMINERvA,
    kT2K,
    kDUNE
  };
  
  //--- this, global control, not saved, no Ini
  analysis anamode;
  experiment expmode;
  
  //=======================================================================================================================
  //                                                  variables
  //=======================================================================================================================
  
  int iniNcharge;//--> GiBUU only  
  double perweight;//--> GiBUU only
  int filecount;//--> GiBUU only
  int run;//--> GiBUU only
  int prod;//--> GiBUU only
  
  int event;
  int targetZ;
  int evtMode;
  double beamE;

  ULong64_t npar; //coded bit to count selected particles. For the internal bits, see particlebit in GeneratorIO.h.
  ULong64_t totparcount; //same coded bit as npar but for ALL particles. If npar == totparcount, the n<Particle> below are the true level particle counts, otherwise they are only for the selected ones.
  int nProton;
  int nNuclei;
  int nPion;
  int nPiZero;
  int nGamma;
  int nNeutron;
  int nBkg;
  int nElectron;

  int AstarPDG; //Astar is the final-state nucleus that has A > targetA/2, namely the nucleus remnant. targetA is the initial nucleus A.
  int AstarA;

  double CLR_KNsrc;
  
  double MMECCQE_Erecoil;

  int LOWRECOIL_parbit;
  double LOWRECOIL_Eav;
  double LOWRECOIL_q3;

  double TESTBEAM_gammaE;//all energy of the gammas, need to be divied by nGamma to have the mean energy per gamma.

  //--- Calc, all will be set by = no need to Ini()
#if __OPENCALC__
  double muonmomentum;
  double muontheta;
  
  double Wrest;
  double Wtrue;
  double xBj;
  double xrest;
  double Q2;
  
  double baryonmomentum;
  double baryontheta;
  double baryonmass;

  double protonmomentum;//in case of TESTBEAM, all protons are added together
  double protontheta;
  double pionmomentum;//in case of TESTBEAM, all pi+ and pi0 are added together
  double piontheta;
  double pionEk;
  
  double dpt;
  double dphit;
  double dalphat;
  double dpTT;
  double beamCalcP;  //using pions and protons only, without knowing beam momentum, calculate via Mx assumption. Idea from J. Sobczyk
  double IApN;       //using pions and protons only, without knowing beam momentum, calculate via Mx assumption. Idea from J. Sobczyk. This is the recoilP under the IA assumption
  double recoilM;    //using pions and protons only, using beam momentum, direct, model-independent, calcuated recoil nucleus mass
  double recoilP;    //using pions and protons only, using beam momentum, direct, model-independent, calcuated recoil nucleus momentum
  double event_beamCalcP; //event_ means the calculation us all particles, including those not selected in Proceed()
  double event_IApN;      //event_ means the calculation us all particles, including those not selected in Proceed()
  double event_recoilM;   //event_ means the calculation us all particles, including those not selected in Proceed()
  double event_recoilP;   //event_ means the calculation us all particles, including those not selected in Proceed()
  
 /*
   double muoncostheta;
   double protonTT;
   double pionTT;
   double dpTy;
   double calcEnu;
   double xCCH;
   double energyCCH;
 */
#endif 
  
#if __OPENCLR__
  double RESmass;
  double adlerPhi;
  double lrsign;
  double w2;
  double pseudoPhi;
  double wpseudo2;
  double pseudosign;
  double cosNuIniNAngle;
  double cosQIniNAngle;
#endif
  
#if __OPENNEUTRONFS__
  double neutronFSmomentum;
  double neutronFStheta;
#endif
  
#if __OPENDPTT__
  double dpTT20s;
  double dpTT10s;
  double dpTT5s;
  double dpTT2s;
#endif
  
#if __OPENMMECCQE__
  double q2qe;
  double muonpt;
  double mupz;
#endif

 //--- EOF

void IniTreeIO()
{
  //--- GeneratorIO: Ini 10
  iniNcharge = -999;
  perweight = -999;
  filecount = -999;
  run = -999;
  prod = -999;
  
  event = -999;
  targetZ = -999;
  evtMode = -999;
  beamE = -999;

  npar=0;
  totparcount=0;
  nProton = -999;
  nNuclei = -999;
  nPion = -999;
  nPiZero = -999;
  nGamma = -999;
  nNeutron = -999;
  nBkg = -999;
  nElectron = -999;

  AstarPDG = -999;
  AstarA = -999;

  CLR_KNsrc = 0;

  MMECCQE_Erecoil = 0;

  //has to be 0
  LOWRECOIL_parbit=0;
  LOWRECOIL_Eav = 0;

  //has to start from 0
  TESTBEAM_gammaE = 0;
  //--- EOF
}

//=======================================================================================================================
//                                                  functions
//=======================================================================================================================

TTree * GetTree(const analysis ana, const experiment exp)
{
  anamode = ana;
  expmode = exp;
  cout<<"TTree * TreeIO::GetTree anamode "<<anamode<<" expmode "<<expmode<<endl;

  //============
  TTree * tout = new TTree("tree","tree");
  //const Int_t spl = 1;

  if(anamode!=TESTBEAM){
    tout->Branch("iniNcharge",&iniNcharge);
    tout->Branch("perweight",&perweight);
    tout->Branch("filecount",&filecount);
    tout->Branch("run",&run);
    tout->Branch("prod",&prod);
  }

  tout->Branch("event",&event);
  tout->Branch("targetZ",&targetZ);
  tout->Branch("evtMode",&evtMode);
  tout->Branch("beamE",&beamE);
  
  tout->Branch("npar",&npar);
  tout->Branch("totparcount",&totparcount);
  tout->Branch("nProton",&nProton);
  tout->Branch("nNuclei",&nNuclei);
  tout->Branch("nPion",&nPion);
  tout->Branch("nPiZero",&nPiZero);
  tout->Branch("nGamma",&nGamma);
  tout->Branch("nNeutron",&nNeutron);
  tout->Branch("nBkg",&nBkg);
  tout->Branch("nElectron",&nElectron);

  tout->Branch("AstarPDG",&AstarPDG);
  tout->Branch("AstarA",&AstarA);

  //---
  if(anamode==CLR||anamode==RESPS){
    tout->Branch("CLR_KNsrc",&CLR_KNsrc);
  }
  else if(anamode==MMECCQE){
    tout->Branch("MMECCQE_Erecoil",&MMECCQE_Erecoil);
  }
  else if(anamode==LOWRECOIL){
    tout->Branch("LOWRECOIL_parbit",&LOWRECOIL_parbit);
    tout->Branch("LOWRECOIL_q3",&LOWRECOIL_q3);
    tout->Branch("LOWRECOIL_Eav",&LOWRECOIL_Eav);
  }
  else if(anamode==TESTBEAM){
    tout->Branch("TESTBEAM_gammaE",&TESTBEAM_gammaE);
  }

  /*
    tout->Branch("muonfullp","TLorentzVector", &muonfullp, 128000, spl);
    tout->Branch("protonfullp","TLorentzVector", &protonfullp, 128000, spl);
    tout->Branch("pionfullp","TLorentzVector", &pionfullp, 128000, spl);
    tout->Branch("baryonfullp","TLorentzVector", &baryonfullp, 128000, spl);
  */

  //tout->Branch("muoncostheta",&muoncostheta);
  //tout->Branch("xCCH",&xCCH);
  //tout->Branch("energyCCH",&energyCCH);
  //tout->Branch("dpTy",&dpTy);
  //tout->Branch("calcEnu",&calcEnu);
  //tout->Branch("protonTT",&protonTT);
  //tout->Branch("pionTT",&pionTT);
    
#if __OPENCALC__

  if(anamode!=TESTBEAM){
    tout->Branch("muonmomentum",&muonmomentum);
    tout->Branch("muontheta",&muontheta);
    
    tout->Branch("Wtrue",&Wtrue);
    tout->Branch("Wrest",&Wrest);
    tout->Branch("xBj",&xBj);
    tout->Branch("xrest",&xrest);
    tout->Branch("Q2",&Q2);
    
    tout->Branch("baryonmomentum",&baryonmomentum);
    tout->Branch("baryontheta",&baryontheta);
    tout->Branch("baryonmass",&baryonmass);
  }
  
  tout->Branch("protonmomentum",&protonmomentum);
  tout->Branch("protontheta",&protontheta);
  tout->Branch("pionmomentum",&pionmomentum);
  tout->Branch("piontheta",&piontheta);
  tout->Branch("pionEk",&pionEk);

  tout->Branch("dpt",&dpt);
  tout->Branch("dphit",&dphit);
  tout->Branch("dalphat",&dalphat);
  tout->Branch("dpTT",&dpTT);
  tout->Branch("beamCalcP",&beamCalcP);
  tout->Branch("IApN",&IApN);
  tout->Branch("recoilM",&recoilM);
  tout->Branch("recoilP",&recoilP);
  tout->Branch("event_beamCalcP",&event_beamCalcP);
  tout->Branch("event_IApN",&event_IApN);
  tout->Branch("event_recoilM",&event_recoilM);
  tout->Branch("event_recoilP",&event_recoilP);
#endif

#if __OPENCLR__
  tout->Branch("RESmass",&RESmass);
  tout->Branch("adlerPhi",&adlerPhi);
  tout->Branch("lrsign",&lrsign);
  tout->Branch("w2",&w2);
  
  tout->Branch("pseudoPhi",&pseudoPhi);
  tout->Branch("pseudosign",&pseudosign);
  tout->Branch("wpseudo2",&wpseudo2);
  tout->Branch("cosNuIniNAngle",&cosNuIniNAngle);
  tout->Branch("cosQIniNAngle",&cosQIniNAngle);
#endif
  
#if __OPENNEUTRONFS__
  tout->Branch("neutronFSmomentum",&neutronFSmomentum);
  tout->Branch("neutronFStheta",&neutronFStheta);
#endif
  
#if __OPENDPTT__
  tout->Branch("dpTT20s",&dpTT20s);
  tout->Branch("dpTT10s",&dpTT10s);
  tout->Branch("dpTT5s",&dpTT5s);
  tout->Branch("dpTT2s",&dpTT2s);
#endif

#if __OPENMMECCQE__
  tout->Branch("q2qe",&q2qe);
  tout->Branch("mupz",&mupz);
  tout->Branch("muonpt",&muonpt);
#endif

  return tout;
}

//end of namespace
}

#endif
