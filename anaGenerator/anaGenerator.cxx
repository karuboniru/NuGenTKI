#include "stdlib.h"

#include <iostream>
#include <fstream>

#include "TString.h"
#include "TLorentzVector.h"
#include "TMath.h"
#include "TTree.h"
#include "TF1.h"
#include "TFile.h"
#include "TGraph.h"
#include "TH2D.h"
#include "TChain.h"
#include "AnaUtils.h"
#include "ReadGENIE.h"
#include "ReadGEANT4.h"
#include "ReadFLUKA.h"
#include "style.h"

using namespace std;
using namespace ReadGENIE;
using namespace ReadFLUKA;

int gSmearBit = 0;

void FLUKAReadChain(TChain * ch, TTree * tout, TH1I * hcounter, TH1D * htargetM, const int nEntryToStop = -999)
{
  ReadFLUKA::SetChain(ch);

  int ientry = 0;
  int isInteractionCounter = 0;
  int isInelas = 0;
  int isTargetAtRest = 0;
  int zeroNucleiCounter = 0;
  int singleNucleiCounter = 0;
  int multiNucleiCounter = 0;
  int isAr40 = 0;
  
  while(ch->GetEntry(ientry)){
    if(ientry%100000==0){
      printf("myEntries %d\n", ientry);
    }
    
    if(nEntryToStop>0){
      if(ientry>=nEntryToStop){
        printf("\n\n\n************************  FLUKA Breaking after %d entries ***********************************************\n\n", nEntryToStop);
        break;
      }
    }

    //do it before the loop continues for any reason
    ientry++;
    hcounter->Fill(1);//any event
    
    //===========================================================================
    AnaUtils::Ini();

    const int tmpnp = ReadFLUKA::NSecIne[0];
    //printf("test0 PDGcode size %d\n", tmpnp);
    if(tmpnp==0){//skip non-interacting events
      continue;
    }
    isInteractionCounter++;
    hcounter->Fill(2);//interacting including elastic and inelastic

    const int nIne = ReadFLUKA::NIneHits;
    if(nIne==0){
      continue;
    }
    
    const int interType = ReadFLUKA::TypeIne[0];
    if(nIne!=1 || interType!=101){//expect to have only 1 inelastic interaction per event
      printf("strange nIne or interType %d %d\n", nIne, interType);
      if(interType==102){//seldom, but not sure what that is
        continue;
      }
      else{
        exit(1);
      }
    }
    isInelas++;
    hcounter->Fill(3);//only inelastic

    const int beam = ReadFLUKA::IdIne[0];
    if(beam!=211){
      printf("wrong beam! %d\n", beam); exit(1);
    }
    const TLorentzVector tmpBeamP(ReadFLUKA::PIne[0][0], ReadFLUKA::PIne[0][1], ReadFLUKA::PIne[0][2], ReadFLUKA::PIne[0][3]);
    if(fabs(tmpBeamP.M()-0.139571)>1E-5){
      printf("wrong beam mass! %f", tmpBeamP.M()); tmpBeamP.Print(); exit(1);
    }
    
    targetZ = 18;//need to read in from file, to-do
    int failCounter = 0;
    TLorentzVector totEvtP=-tmpBeamP;
    for(int ii=0; ii<tmpnp; ii++){
      const TLorentzVector tmpSecP(ReadFLUKA::PSec[ii][0], ReadFLUKA::PSec[ii][1], ReadFLUKA::PSec[ii][2], ReadFLUKA::PSec[ii][3]);
      totEvtP += tmpSecP;
      const bool kProceed = GeneratorIO::FLUKAProceed(ReadFLUKA::RunNum, ReadFLUKA::EveNum, &tmpBeamP, ReadFLUKA::IdSecIne[ii], &tmpSecP, gSmearBit);
      if(kProceed){
        AnaUtils::MainProceed();
      }
      else{
        failCounter++;
      }
    }//loop over particle

    /*    //test momentum conservation: totEvtP.Print(); exit(1);
      test evt 1 beamm 0.139571 pdg 111 imass 0.134977
      test evt 1 beamm 0.139571 pdg 211 imass 0.139571
      test evt 1 beamm 0.139571 pdg 2112 imass 0.939565
      test evt 1 beamm 0.139571 pdg 22 imass -0.000001
      test evt 1 beamm 0.139571 pdg 22 imass -0.000000
      test evt 1 beamm 0.139571 pdg -8981961 imass 36.285842
      (x,y,z,t)=(0.000000,0.000000,0.000000,37.215539) (P,eta,phi,E)=(0.000000,1.475926,1.165905,37.215539)
     */
    if(totEvtP.P()>1E-6){
      //printf("initial target not at rest run %d event %d\n", ReadFLUKA::RunNum, ReadFLUKA::EveNum); totEvtP.Print(); //exit(1);
      //test do not skip for nofsi: continue;
    }
    isTargetAtRest++;
    hcounter->Fill(4);
    
    htargetM->Fill(totEvtP.M());
    if(fabs(totEvtP.M()-37.2)>1){
      continue;
    }
    else if(fabs(totEvtP.M()-37.2)>2E-1){
      //unknown Ar-40!(x,y,z,t)=(-0.000000,0.000000,-0.000000,37.305117) (P,eta,phi,E)=(0.000000,-1.605495,2.266529,37.305117)
      printf("unknown Ar-40!"); totEvtP.Print();
      //test do not exit for nofsi: exit(1);
    }
    isAr40++;
    hcounter->Fill(5);
    
    if(failCounter==0){//all are processed and therefore there is no nuclei skipped.
      zeroNucleiCounter++;
      hcounter->Fill(6);
    }
    else if(failCounter==1){
      singleNucleiCounter++;
      hcounter->Fill(7);
    }
    else{//>=2
      multiNucleiCounter++;
      hcounter->Fill(8);
    }

    if(1){//fill all fail cases//if(failCounter==1){
      AnaUtils::DoFill(tout);
    }
    
  }//loop over event

  //static void PrintStat(const TString tag, const double oldsel, const double newn);
  style::PrintStat("All entries", ientry, ientry);
  style::PrintStat("Interactions", ientry, isInteractionCounter);
  style::PrintStat("Inelastic", isInteractionCounter, isInelas);
  style::PrintStat("Target at rest", isInelas, isTargetAtRest);
  style::PrintStat("Ar40", isTargetAtRest, isAr40);
  style::PrintStat("zero nuclei", isAr40, zeroNucleiCounter);
  style::PrintStat("single nuclei", isAr40, singleNucleiCounter);
  style::PrintStat("multi nuclei", isAr40, multiNucleiCounter);
}

void GEANT4ReadChain(TChain * ch, TTree * tout, TH1I * hcounter, TH1D * htargetM, const int nEntryToStop = -999)
{
  ReadGEANT4::SetChain(ch);

  int ientry = 0;
  int isInteractionCounter = 0;
  int isInelas = 0;
  int zeroNucleiCounter = 0;
  int singleNucleiCounter = 0;
  int multiNucleiCounter = 0;
  int isTestBeam = 0;
  int isTargetAtRest = 0;
  int isAr40 = 0;
 
  while(ch->GetEntry(ientry)){
    if(ientry%100000==0){
      printf("myEntries %d\n", ientry);
    }

    if(nEntryToStop>0){
      if(ientry>=nEntryToStop){
        printf("\n\n\n************************  GEANT4 Breaking after %d entries ***********************************************\n\n", nEntryToStop);
        break;
      }
    }

    //do it before the loop continues for any reason
    ientry++;
    hcounter->Fill(1);//any event
    
    //===========================================================================
    AnaUtils::Ini();

    const int tmpnp = ReadGEANT4::interType->size();
    //printf("test0 PDGcode size %d\n", tmpnp);
    if(tmpnp==0){//skip non-interacting events
      continue;
    }
    isInteractionCounter++;
    hcounter->Fill(2);//interacting including elastic and inelastic
    
    const bool interType = (*ReadGEANT4::interType)[0];
    if(!interType){
      continue;
    }
    isInelas++;
    hcounter->Fill(3);//only inelastic

    targetZ = (*ReadGEANT4::targetZ)[0];
    int failCounter = 0;
    TLorentzVector totEvtP;
    TLorentzVector beamP;
    for(int ii=0; ii<tmpnp; ii++){
      //printf("test all ientry %d ii %d/%d interType %d pdg %d x %f y %f z %f e %f\n", ientry, ii, tmpnp, (*ReadGEANT4::interType)[ii], (*ReadGEANT4::PDGcode)[ii], (*ReadGEANT4::Px)[ii], (*ReadGEANT4::Py)[ii], (*ReadGEANT4::Pz)[ii], (*ReadGEANT4::E)[ii]);
      TLorentzVector tmpSecP((*ReadGEANT4::Px)[ii], (*ReadGEANT4::Py)[ii], (*ReadGEANT4::Pz)[ii], (*ReadGEANT4::E)[ii]);
      tmpSecP *= 1E-3;//input unit is MeV
      
      if(ii==0){
        beamP = tmpSecP;
        totEvtP = -beamP;//beam
      }
      else{
        totEvtP += tmpSecP;
      }
      
      const bool kProceed = GeneratorIO::GEANT4Proceed(ReadGEANT4::EventID, interType, (*ReadGEANT4::PDGcode)[ii], &tmpSecP, gSmearBit);
      if(kProceed){
        AnaUtils::MainProceed();
      }
      else{
        if(ii!=0){//beam particle at [0]
          failCounter++;
        }
      }
    }//loop over particle
    
    if(fabs(beamP.M()-0.139571)>1E-5 && fabs(beamP.M()-0.000511)>1E-5){
      //printf("***************************** ERROR EventID %d wrong beam mass not piplus or electron! %f ", ReadGEANT4::EventID, beamP.M()); beamP.Print(); //exit(1);
      continue;
    }
    isTestBeam++;
    hcounter->Fill(4);

    if(totEvtP.P()>3E-2){
      //printf("***************************** ERROR EventID %d initial target not at rest", ReadGEANT4::EventID); totEvtP.Print(); //exit(1);
      continue;
    }
    isTargetAtRest++;
    hcounter->Fill(5);
    
    htargetM->Fill(totEvtP.M());
    if(fabs(totEvtP.M()-37.2)>1){
      continue;
    }
    if(fabs(totEvtP.M()-37.2)>2E-1){
      printf("***************************** ERROR EventID %d unknown Ar-40 mass! %f", ReadGEANT4::EventID, totEvtP.M()); totEvtP.Print(); exit(1);
    }
    isAr40++;
    hcounter->Fill(6);
    
    if(failCounter==0){//all are processed and therefore there is no nuclei skipped.
      //nuclei below GEANT4 tracking threshold are not saved. These can be helium and deutron, checked by looking at event_recoilM for AstarA<0
      //now skipping all A<20 nuclei, can be quite often
      zeroNucleiCounter++;
      hcounter->Fill(7);
      /*
      printf("anaGenerator bad failCounter %d\n", failCounter);
      for(int kk=0; kk<tmpnp; kk++){
        printf("test failCounter ientry %d kk %d/%d interType %d pdg %d x %f y %f z %f e %f\n", ientry, kk, tmpnp, (*ReadGEANT4::interType)[kk], (*ReadGEANT4::PDGcode)[kk], (*ReadGEANT4::Px)[kk], (*ReadGEANT4::Py)[kk], (*ReadGEANT4::Pz)[kk], (*ReadGEANT4::E)[kk]);
      }
      */
      //exit(1);
    }
    else if(failCounter==1){
      singleNucleiCounter++;
      hcounter->Fill(8);
    }
    else{//>=2
      multiNucleiCounter++;
      hcounter->Fill(9);
    }

    //hcounter->Fill(4);//only no nuclei or 1 nuclei, reject multi-nuclei, i.e. breakup

    /*
    if( (TreeIO::AstarPDG%1000)/10 == 39){
      for(int kk=0; kk<tmpnp; kk++){
        printf("test Cl/Ar39 ientry %d kk %d/%d interType %d pdg %d x %f y %f z %f e %f\n", ientry, kk, tmpnp, (*ReadGEANT4::interType)[kk], (*ReadGEANT4::PDGcode)[kk], (*ReadGEANT4::Px)[kk], (*ReadGEANT4::Py)[kk], (*ReadGEANT4::Pz)[kk], (*ReadGEANT4::E)[kk]);
      }
    }
test Cl/Ar39 ientry 190499 kk 0/6 interType 1 pdg 211 x 431.087456 y 339.823754 z 767.199259 e 953.619995
test Cl/Ar39 ientry 190499 kk 1/6 interType 1 pdg 2212 x -426.430196 y -277.459398 z 291.563354 e 1106.431195
test Cl/Ar39 ientry 190499 kk 2/6 interType 1 pdg 22 x -1.171083 y 3.678978 z 1.844162 e 4.278697
test Cl/Ar39 ientry 190499 kk 3/6 interType 1 pdg 22 x 0.180470 y 0.222846 z 0.031460 e 0.288478
test Cl/Ar39 ientry 190499 kk 4/6 interType 1 pdg 22 x -0.019291 y 0.173310 z 0.068713 e 0.187430
test Cl/Ar39 ientry 190499 kk 5/6 interType 1 pdg 1000170390 x -1.026815 y -66.109551 z 69.926137 e 36289.920122

test Cl/Ar39 ientry 189 kk 0/5 interType 1 pdg 111 x 35.345009 y -599.181520 z 472.315098 e 775.608147
test Cl/Ar39 ientry 189 kk 1/5 interType 1 pdg 2212 x 171.966526 y 573.838089 z 649.373338 e 1288.760157
test Cl/Ar39 ientry 189 kk 2/5 interType 1 pdg 22 x -1.951103 y -0.375384 z -1.299820 e 2.374289
test Cl/Ar39 ientry 189 kk 3/5 interType 1 pdg 22 x 0.708164 y 0.604643 z 0.428463 e 1.025022
test Cl/Ar39 ientry 189 kk 4/5 interType 1 pdg 1000180390 x -209.351213 y 28.246665 z 9.275568 e 36286.457472
    */
    
    if(1){//fill all fail cases//if(failCounter==1){
      AnaUtils::DoFill(tout);
    }
  }//loop over event

  //static void PrintStat(const TString tag, const double oldsel, const double newn);
  style::PrintStat("All entries", ientry, ientry);
  style::PrintStat("Interactions", ientry, isInteractionCounter);
  style::PrintStat("Inelastic", isInteractionCounter, isInelas);
  style::PrintStat("Pi+/electron beam", isInelas, isTestBeam);
  style::PrintStat("Target at rest", isTestBeam, isTargetAtRest);
  style::PrintStat("Ar40", isTargetAtRest, isAr40);
  style::PrintStat("zero nuclei", isAr40, zeroNucleiCounter);
  style::PrintStat("single nuclei", isAr40, singleNucleiCounter);
  style::PrintStat("multi nuclei", isAr40, multiNucleiCounter);

}

void GENIEReadChain(TChain * ch, TTree * tout, TH1F * &hCCrate, const int nEntryToStop = -999)
{
  ReadGENIE::SetChain(ch);

  int ientry = 0;
  while(ch->GetEntry(ientry)){
    if(ientry%100000==0){
      printf("myEntries %d\n", ientry);
    }

    if(nEntryToStop>0){
      if(ientry>=nEntryToStop){
        printf("\n\n\n************************  GENIE Breaking after %d entries ***********************************************\n\n", nEntryToStop);
        break;
      }
    }

    //do it before the loop continues for any reason
    ientry++;

    //===========================================================================

    const TString ecode=gEvtCode->GetString();
    if(ecode.Contains("Weak[NC]")){//skip NC
      continue;
    }
    else if(!ecode.Contains("Weak[CC]")){
      printf("not cc!!! %s\n", ecode.Data()); exit(1);
    }

    if(!hCCrate){
      hCCrate = new TH1F("hCCrate","", 100000, 0, 100);//1MeV per bin
    }

    const double tmpenu = StdHepP4[0][3];
    hCCrate->Fill(tmpenu);

    const int tmpevent = EvtNum;
    const int tmpprod= -999;//not needing G2NeutEvtCode anymore, no need to modify GENIE code; abs(G2NeutEvtCode);
    const double tmppw = 1;//to-do EvtXSec;

    const int tmpnp = StdHepN;

    //printf("\ntestbug ientry %d tmpevent %d tmpprod %d tmpenu %f tmppw %f tmpnp %d\n", ientry, tmpevent, tmpprod, tmpenu, tmppw, tmpnp);
    
    AnaUtils::Ini();

    const bool isHydrogen = ecode.Contains("1000010010");//special case

    int idxIni = -999;
    int idxRESnucleon = -999;
    int idxRESpi = -999;
    int idxDelta = -999;
    //0 is beam, 1 is target nucleus
    for(int ii=2; ii<tmpnp; ii++){
      const int tmpid = StdHepPdg[ii];

      //printf("testbug ii %d/%d pdg %d precut\n", ii, tmpnp, tmpid);

      if( ( abs(tmpid) == 12 || abs(tmpid) == 14 || abs(tmpid) == 16 ) && StdHepFm[ii]!=0 ){//charm decay to neutrino
        continue;
      }

      if( abs(tmpid)==13 && StdHepFm[ii]!=0 ){//skip non CC muon
        continue;
      }

      const GeneratorIO::dtype IniOrFinaltype = ReadGENIE::GetIniOrFinalType(ii, isHydrogen, tmpevent, tmpid);
      const GeneratorIO::dtype RESdtype = ReadGENIE::GetRESType(ii, ecode, IniOrFinaltype, idxIni, idxDelta, idxRESnucleon, idxRESpi);
      const double tmpKNsrc = ReadGENIE::GetKNsource(ii, IniOrFinaltype, idxDelta); //proton 1; pion -1      
      const bool isOK = (IniOrFinaltype!=GeneratorIO::kNULL || RESdtype!=GeneratorIO::kNULL);

      if(isOK){
        const double tmpmom1 = StdHepP4[ii][0];
        const double tmpmom2 = StdHepP4[ii][1];
        const double tmpmom3 = StdHepP4[ii][2];
        const double tmptote = StdHepP4[ii][3];

        //test
        //const TVector3 tmpvec(tmpmom1, tmpmom2, tmpmom3); printf("test particle %d %f %f %f %f %d mom %f theta %f\n", ii, tmpmom1, tmpmom2, tmpmom3, tmptote, tmpid, tmpvec.Mag(), tmpvec.Theta()*TMath::RadToDeg());

        //if(ecode.Contains("QES")){ cout<<"test event "<<tmpevent<<" ii "<<ii<<" IniOrFinaltype "<<IniOrFinaltype<<" idxIni "<<idxIni<<" ecode "<<ecode<<" status "<<StdHepStatus[ii]<<" pdg "<<StdHepPdg[ii]<<" scat "<<StdHepRescat[ii]<<" StdHepFd "<<StdHepFd[ii]<<" StdHepLd "<<StdHepLd[ii]<<" RESdtype "<<RESdtype<<" "<<endl;}

        //printf("testbug ii %d/%d pdg %d postcut\n", ii, tmpnp, tmpid);
        
        if(GeneratorIO::GENIEProceed(IniOrFinaltype, RESdtype, ecode, tmpevent, tmpprod, tmpenu, tmppw, tmpmom1, tmpmom2, tmpmom3, tmptote, tmpid, tmpKNsrc)){
          //if(ecode.Contains("QES")){printf("test now do Main IniOrFinaltype %d\n", IniOrFinaltype); ch->Show(ientry);}
          AnaUtils::MainProceed();
        }

      }//isOK

    }//loop over particle

    AnaUtils::DoFill(tout);
  }//loop over event

  cout<<"All entries "<<ientry<<endl;
}

int GiBUUReadFile(const TString filelist, TTree * tout, const int nFileToStop)
{
  TString tmpfl4target(filelist);
  tmpfl4target.ToUpper();
  int tmptargetZ = 6;
  if(tmpfl4target.Contains("HYDROGEN")){
    tmptargetZ = 1;
  }

  ifstream fl;
  fl.open(filelist);
  if(!fl.is_open()){
    printf("filelist %s not opened!\n", filelist.Data()); exit(1);
  }

  int filecount=0;
  int totlinecount=0;
  int totnrun = 0;

  string singlefile;
  while(getline(fl,singlefile)){
    if(filecount%10==0){
      cout<<"File count "<<filecount<<" total line count "<<totlinecount<<" total nrun "<<totnrun<<endl;
    }

    if(nFileToStop>0){
      if(filecount>=nFileToStop){
        printf("\n\n\n************************  GiBUU Breaking after %d files ***********************************************\n\n", nFileToStop);
        break;
      }
    }

    filecount++;
  //====================================== core loop ======================================
  ifstream elf;
  elf.open(singlefile.c_str());
  if(!elf.is_open()){
    printf("singlefile %s not opened!\n", singlefile.c_str()); exit(1);
  }

  string fline;
  AnaUtils::Ini();
  getline(elf, fline);//after reading even the last line it is still not eof
  while(!elf.eof()){
    TString tmpline(fline);
    totlinecount++;
    if(tmpline.Contains("Event")){//simply ignore it
      getline(elf, fline);
      continue;
    }
    
    double tmppw;
    double tmpmom1, tmpmom2, tmpmom3, tmptote;                                                                                                     
    double tmppos1, tmppos2, tmppos3;//position
    int tmprun, tmpevent, tmpid, tmpcharge, tmpprod;
    double tmpenu;
    
    sscanf(tmpline.Data(),"%d %d %d %d %lf %lf %lf %lf %lf %lf %lf %lf %*d %d %lf", &tmprun, &tmpevent, &tmpid, &tmpcharge, &tmppw, &tmppos1, &tmppos2, &tmppos3, &tmptote, &tmpmom1, &tmpmom2, &tmpmom3, /*&history,*/ &tmpprod, &tmpenu);
    
    if(GeneratorIO::GiBUUProceed(filecount, tmprun, tmpevent, tmpcharge, tmpprod, tmpenu, tmppw, tmpmom1, tmpmom2, tmpmom3, tmptote, tmpid, tmptargetZ)){
      AnaUtils::MainProceed();
    }

    getline(elf, fline);

    bool kChangeEvent = false;
    bool kChangeRun = false;
    if(elf.eof()){
      kChangeEvent = true;
      kChangeRun = true;
    }
    else{
    //only read in new event to check for event switching
      int nextRun, nextEvent;
      sscanf(fline.c_str(),"%d %d", &nextRun,  &nextEvent);
      if(nextEvent!=tmpevent){
        kChangeEvent = true;
      }
      if(nextRun!=tmprun){
        kChangeRun = true;
      }
    }

    if(kChangeEvent){
      AnaUtils::DoFill(tout);
      AnaUtils::Ini();
    }

    if(kChangeRun){
      totnrun++;
    }

  }
  elf.close();
  //============================================= end core ==============================================
  }

  cout<<"Final file count "<<filecount<<" total line count "<<totlinecount<<" total nrun "<<totnrun<<endl;
  return totnrun;
}
void NuWroReadChain(TChain *ch, TTree *tout, TH1F *&hCCrate, const int nEntryToStop = 10)
{
  cout << "precessing NuWro Chain" << endl;

  ReadGENIE::SetChain(ch); // use same code as readGENIE

  int ientry = 0;
  while (ch->GetEntry(ientry))
  {
    // EvtNum = ientry;
    if (ientry % 100000 == 0)
    {
      printf("myEntries %d\n", ientry);
    }

    if (nEntryToStop > 0)
    {
      if (ientry >= nEntryToStop)
      {
        printf("\n\n\n************************  NuWro Breaking after %d entries ***********************************************\n\n", nEntryToStop);
        break;
      }
    }

    // do it before the loop continues for any reason
    ientry++;
    // cout << "ientry " << ientry << endl;
    //====================================== core loop ======================================

    //===========================================================================
    // ignore the CC check here, We should disable NC in neut/nuwro side.
    // or check ecode here, if NC, ignore it
    // checking NC is not a good idea, since checking ecode is complex.
    //===========================================================================
    const TString ecode_str = gEvtCode->GetString();
    const int code = ecode_str.Atoi();
    // if(ecode.Contains("Weak[NC]")){//skip NC
    //   continue;
    // }
    // else if(!ecode.Contains("Weak[CC]")){
    //   printf("not cc!!! %s\n", ecode.Data()); exit(1);
    // }
    if (!hCCrate)
    {
      hCCrate = new TH1F("hCCrate", "", 100000, 0, 100); // 1MeV per bin
    }

    const double tmpenu = StdHepP4[0][3];
    hCCrate->Fill(tmpenu);
    beamfullp->SetXYZT(StdHepP4[0][0], StdHepP4[0][1], StdHepP4[0][2], StdHepP4[0][3]);

    beamE = StdHepP4[0][3];

    const int tmpevent = EvtNum;
    const int tmpprod = -999; // not needing G2NeutEvtCode anymore, no need to modify GENIE code; abs(G2NeutEvtCode);
    // const double tmppw = 1;   //to-do EvtXSec;
    npar = 0;
    const int tmpnp = StdHepN;

    // printf("\ntest ientry %d tmpevent %d tmpprod %d tmpenu %f tmppw %f tmpnp %d\n", ientry, tmpevent, tmpprod, tmpenu, tmppw, tmpnp);

    AnaUtils::Ini();
    int targetid = StdHepPdg[1];
    // cout << "targetid = " << targetid << endl;
    // const bool isHydrogen = targetid == 1000010010; //special case

    // int idxIni = -999;
    // int idxRESnucleon = -999;
    // int idxRESpi = -999;
    // int idxDelta = -999;
    bool muon_found = false;
    iniNfullp->SetXYZT(0, 0, 0, 0);

    for (int ii = 0; ii < tmpnp; ii++)
    {
      const int tmpid = StdHepPdg[ii];

      if ((abs(tmpid) == 12 || abs(tmpid) == 14 || abs(tmpid) == 16) && ii >= 2)
      {
        // charm decay to neutrino
        // dirty hack
        continue;
      }
      if (abs(tmpid) == 13 && muon_found)
      {
        // skip non CC muon
        // dirty hack, should be fixed in the future
        continue;
      }
      if (abs(tmpid) == 13)
      {
        muon_found = true;
      }

      if (tmpid > 1000000000 && ii > 2)
      { // skip nucleus
        continue;
      }
      if (StdHepStatus[ii] == 2) // ignore before fsi particle
        continue;
      const GeneratorIO::dtype IniOrFinaltype = StdHepStatus[ii] == 0 ? GeneratorIO::kINI : GeneratorIO::kFINAL;
      // const GeneratorIO::dtype RESdtype = ReadGENIE::GetRESTypeNuWro(ii, code, IniOrFinaltype, idxIni, idxDelta, idxRESnucleon, idxRESpi);
      // cout << "found restype" << RESdtype << endl;
      // const double tmpKNsrc = ReadGENIE::GetKNsource(ii, IniOrFinaltype, idxDelta); //proton 1; pion -1
      // cout << "tmpKNsrc:" << tmpKNsrc << endl;

      const double tmpmom1 = StdHepP4[ii][0];
      const double tmpmom2 = StdHepP4[ii][1];
      const double tmpmom3 = StdHepP4[ii][2];
      const double tmptote = StdHepP4[ii][3];

      // test
      // const TVector3 tmpvec(tmpmom1, tmpmom2, tmpmom3); printf("test particle %d %f %f %f %f %d mom %f theta %f\n", ii, tmpmom1, tmpmom2, tmpmom3, tmptote, tmpid, tmpvec.Mag(), tmpvec.Theta()*TMath::RadToDeg());

      // if(ecode.Contains("QES")){ cout<<"test event "<<tmpevent<<" ii "<<ii<<" IniOrFinaltype "<<IniOrFinaltype<<" idxIni "<<idxIni<<" ecode "<<ecode<<" status "<<StdHepStatus[ii]<<" pdg "<<StdHepPdg[ii]<<" scat "<<StdHepRescat[ii]<<" StdHepFd "<<StdHepFd[ii]<<" StdHepLd "<<StdHepLd[ii]<<" RESdtype "<<RESdtype<<" "<<endl;}

      if (GeneratorIO::NuWroProceed(code, tmpevent, tmpprod, tmpenu, tmpmom1, tmpmom2, tmpmom3, tmptote, tmpid, targetid, IniOrFinaltype, EvtWght, missE))
      {
        // if (ii>=2)
        AnaUtils::MainProceed();
      } // isOK

      // loop over particle

      // AnaUtils::DoFill(tout);
    } // loop over event
    AnaUtils::DoFill(tout);
    // cout << "All entries " << ientry << endl;
  }
}

void anaGenerator(const TString tag, const TString filelist, const int tmpana, const int nToStop=-999, const int smearBit=0)
{
  gSmearBit = smearBit;

  cout<<"please check "<<tag<<" "<<filelist<<" "<<tmpana<<" smearBit "<<gSmearBit<<endl;

  TreeIO::experiment kExp = TreeIO::kNONE;
  if(filelist.Contains("MINERvA")){
    kExp = TreeIO::kMINERvA;
  }
  else if(filelist.Contains("T2K")){
    kExp = TreeIO::kT2K;
  }
  else if(filelist.Contains("DUNE")){
    kExp = TreeIO::kDUNE;
  }
  else{
    cout<<"\n\nnon-experimental filelist "<<filelist<<endl<<endl;
  }

  TFile *fout=new TFile(Form("outplot/outAna%d_%s.root", tmpana, tag.Data()),"recreate");
  TTree * tout = TreeIO::GetTree((analysis)tmpana, kExp);

  //_________________________________________________________________________________________________
  //_________________________________________________________________________________________________
  TH1F * hCCrate = 0x0;
  TH1I * hcounter = 0x0;
  TH1D * htargetM = 0x0;
  int nrun = -999;
  if(filelist.Contains("GENIE")){
    TChain * rootFileInput = AnaUtils::InputROOTFiles(filelist, "gRooTracker");
    if(rootFileInput){
      GENIEReadChain(rootFileInput, tout, hCCrate, nToStop);
    }
    else{
      cout<<"No generator identified for GENIE!! "<<filelist<<endl;
      exit(1);
    }
  }
  else if(filelist.Contains("GEANT4")){
    TChain * rootFileInput = AnaUtils::InputROOTFiles(filelist, "PartInfo");
    if(rootFileInput){
      hcounter=new TH1I("hcounter","",20,0,20);
      htargetM = new TH1D("htargetM","",1600,-1,40);
      GEANT4ReadChain(rootFileInput, tout, hcounter, htargetM, nToStop);
    }
    else{
      cout<<"No generator identified for GEANT4!! "<<filelist<<endl;
      exit(1);
    }
  }
  else if(filelist.Contains("FLUKA")){
    TChain * rootFileInput = AnaUtils::InputROOTFiles(filelist, "HitsTree");
    if(rootFileInput){
      hcounter=new TH1I("hcounter","",20,0,20);
      htargetM = new TH1D("htargetM","",1600,-1,40);
      FLUKAReadChain(rootFileInput, tout, hcounter, htargetM, nToStop);
    }
    else{
      cout<<"No generator identified for FLUKA!! "<<filelist<<endl;
      exit(1);
    }
  }else if(filelist.Contains("NuWro")){
    TChain * rootFileInput = AnaUtils::InputROOTFiles(filelist, "nRooTracker");
    if(rootFileInput){
      NuWroReadChain(rootFileInput, tout, hCCrate, nToStop);
    }
    else{
      cout<<"No generator identified for NuWro!! "<<filelist<<endl;
      exit(1);
    }
  }
  else{
    nrun = GiBUUReadFile(filelist, tout, nToStop);
  }


  //_________________________________________________________________________________________________
  //_________________________________________________________________________________________________

  fout->cd();

  TH1 * hhs[]={hCCrate, hcounter, htargetM};
  for(unsigned int ii=0; ii<sizeof(hhs)/sizeof(TH1*); ii++){
    if(hhs[ii]){
      printf("Saving hist %d %s\n", ii, hhs[ii]->GetName());
      hhs[ii]->Write();
    }
  }

  TTree *theader = new TTree("header","header");
  theader->Branch("nrun",&nrun);
  theader->Fill();
  theader->Write();

  tout->Write();

  fout->Save();
  fout->Close();

  return;
}

int main(int argc, char* argv[])
{
  //void anaGenerator(const TString tag, const TString filelist, const int tmpana, const int nToStop=-999, const int smearBit=0)
  if(argc==4){
    anaGenerator(argv[1], argv[2], atoi(argv[3]));
  }
  else if(argc==5){//last is nstop
    anaGenerator(argv[1], argv[2], atoi(argv[3]), atoi(argv[4]));
  }
  else if(argc==6){//last is smearbit
    anaGenerator(argv[1], argv[2], atoi(argv[3]), atoi(argv[4]), atoi(argv[5]));
  }
  else{
    printf("wrong argc %d\n", argc); return 1;
  }
  return 0;
}
