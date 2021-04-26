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
    GEANT4PIPLUSKE1GEV = 11
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
  
  //--- GeneratorIO: Ini 10
  int iniNcharge;//--> GiBUU only
  
  double perweight;
  int filecount;//--> GiBUU only
  int run;//--> GiBUU only
  int event;
  int targetZ;
  int prod;
  int AstarPDG;
  double lineBeamEnergy;
  
  int LOWRECOIL_parbit;
  
  int evtMode;
  
  //--- ProcessUtils: Ini 6
  int npar;
  int totparcount;
  
  double CLR_KNsrc;
  
  double MMECCQE_Erecoil;
  double LOWRECOIL_Eav;
  double LOWRECOIL_q3;
  
  //--- Calc, all will be set by = no need to Ini()
#if __OPENCALC__
  double muonmomentum;
  double muontheta;
  
  double Wrest;
  double Wtrue;
  double xBj;
  double xrest;
  double Q2;
  
  double protonmomentum;
  double protontheta;
  double pionmomentum;
  double piontheta;
  double pionEk;
  double baryonmomentum;
  double baryontheta;
  double baryonmass;
  
  double dpt;
  double dphit;
  double dalphat;
  double neutronmomentum;
  double dpTT;
  double pBeam;
  double Mx; 
  
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
  /*//original

  npar=0;
  totparcount=0;
  iniNcharge = -999;
  parbit=0;

  iniNfullp->SetXYZT(0,0,0,0);
  RESpifullp->SetXYZT(0,0,0,0);
  RESnucleonfullp->SetXYZT(0,0,0,0);
  muonfullp->SetXYZT(0,0,0,0);
  protonfullp->SetXYZT(0,0,0,0);
  neutronFSfullp->SetXYZT(0,0,0,0);
  pionfullp->SetXYZT(0,0,0,0);
  baryonfullp->SetXYZT(0,0,0,0);

  Eav = 0;
  Erecoil = 0;
  KNsrc = 0;
   */

  //--- GeneratorIO: Ini 10
  iniNcharge = -999;

  perweight = -999;
  filecount = -999;
  run = -999;
  event = -999;
  targetZ = -999;
  prod = -999;
  AstarPDG = -999;
  lineBeamEnergy = -999;

  //has to be 0
  LOWRECOIL_parbit=0;

  evtMode = -999;

  //--- ProcessUtils: Ini 6
  npar=0;
  totparcount=0;

  CLR_KNsrc = 0;

  MMECCQE_Erecoil = 0;
  LOWRECOIL_Eav = 0;

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

  tout->Branch("iniNcharge",&iniNcharge);

  tout->Branch("perweight",&perweight);
  tout->Branch("filecount",&filecount);
  tout->Branch("run",&run);
  tout->Branch("event",&event);
  tout->Branch("targetZ",&targetZ);
  tout->Branch("prod",&prod);
  tout->Branch("AstarPDG",&AstarPDG);
  tout->Branch("enu",&lineBeamEnergy);

  tout->Branch("evtMode",&evtMode);

  tout->Branch("npar",&npar);
  tout->Branch("totparcount",&totparcount);

  //---
  if(anamode==MMECCQE){
    tout->Branch("MMECCQE_Erecoil",&MMECCQE_Erecoil);
  }
  else if(anamode==LOWRECOIL){
    tout->Branch("LOWRECOIL_parbit",&LOWRECOIL_parbit);
    tout->Branch("LOWRECOIL_q3",&LOWRECOIL_q3);
    tout->Branch("LOWRECOIL_Eav",&LOWRECOIL_Eav);
  }
  else if(anamode==CLR||anamode==RESPS){
    tout->Branch("CLR_KNsrc",&CLR_KNsrc);
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
  tout->Branch("muonmomentum",&muonmomentum);
  tout->Branch("muontheta",&muontheta);

  tout->Branch("Wtrue",&Wtrue);
  tout->Branch("Wrest",&Wrest);
  tout->Branch("xBj",&xBj);
  tout->Branch("xrest",&xrest);
  tout->Branch("Q2",&Q2);

  tout->Branch("protonmomentum",&protonmomentum);
  tout->Branch("protontheta",&protontheta);
  tout->Branch("pionmomentum",&pionmomentum);
  tout->Branch("piontheta",&piontheta);
  tout->Branch("pionEk",&pionEk);
  tout->Branch("baryonmomentum",&baryonmomentum);
  tout->Branch("baryontheta",&baryontheta);
  tout->Branch("baryonmass",&baryonmass);

  tout->Branch("dpt",&dpt);
  tout->Branch("dphit",&dphit);
  tout->Branch("dalphat",&dalphat);
  tout->Branch("neutronmomentum",&neutronmomentum);  
  tout->Branch("dpTT",&dpTT);
  tout->Branch("pBeam",&pBeam);
  tout->Branch("Mx",&Mx);
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
