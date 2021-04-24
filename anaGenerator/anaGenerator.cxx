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

using namespace std;
using namespace ReadGENIE;

void GEANT4ReadChain(TChain * ch, TTree * tout, const int nEntryToStop = -999)
{
  const int tmpz = 18;
  printf("\nanaGenerator::GEANT4ReadChain setting targetZ as 18 for argon only!\n\n");

  ReadGEANT4::SetChain(ch);

  int ientry = 0;
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
    
    //===========================================================================
    AnaUtils::Ini();

    const int tmpnp = ReadGEANT4::PDGcode->size();
    for(int ii=0; ii<tmpnp; ii++){
      if((*ReadGEANT4::PDGcode)[ii]>1000000 || (*ReadGEANT4::PDGcode)[ii]>3000){//don't care remnant or strangeness 3122 is Lambda
        continue;
      }
      //printf("test ii %d %d %d\n", ii, tmpnp, (*ReadGEANT4::PDGcode)[ii]);

      if(GeneratorIO::GEANT4Proceed(ii, (*ReadGEANT4::interType)[ii], (*ReadGEANT4::PDGcode)[ii], (*ReadGEANT4::Px)[ii], (*ReadGEANT4::Py)[ii], (*ReadGEANT4::Pz)[ii], (*ReadGEANT4::E)[ii], tmpz)){
        AnaUtils::MainProceed();
      }
    }//loop over particle

    AnaUtils::DoFill(tout);
  }//loop over event
  
  cout<<"All entries "<<ientry<<endl;
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

    //printf("\ntest ientry %d tmpevent %d tmpprod %d tmpenu %f tmppw %f tmpnp %d\n", ientry, tmpevent, tmpprod, tmpenu, tmppw, tmpnp);
    
    AnaUtils::Ini();

    const bool isHydrogen = ecode.Contains("1000010010");//special case

    int idxIni = -999;
    int idxRESnucleon = -999;
    int idxRESpi = -999;
    int idxDelta = -999;
    for(int ii=1; ii<tmpnp; ii++){
      const int tmpid = StdHepPdg[ii];

      if( ( abs(tmpid) == 12 || abs(tmpid) == 14 || abs(tmpid) == 16 ) && StdHepFm[ii]!=0 ){//charm decay to neutrino
        continue;
      }

      if( abs(tmpid)==13 && StdHepFm[ii]!=0 ){//skip non CC muon
        continue;

      }

      if(tmpid>1000000000){//skip nucleus
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

void anaGenerator(const TString tag, const TString filelist, const int tmpana, const int nToStop=-999)
{
  cout<<"please check "<<tag<<" "<<filelist<<" "<<tmpana<<endl;

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
      GEANT4ReadChain(rootFileInput, tout, nToStop);
    }
    else{
      cout<<"No generator identified for GEANT4!! "<<filelist<<endl;
      exit(1);
    }
  }
  else{
    nrun = GiBUUReadFile(filelist, tout, nToStop);
  }


  //_________________________________________________________________________________________________
  //_________________________________________________________________________________________________

  fout->cd();

  if(hCCrate){
    hCCrate->Write();
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
  //void anaGenerator(const TString tag, const TString filelist, const int tmpana, const int nToStop)
  if(argc==4){
    anaGenerator(argv[1], argv[2], atoi(argv[3]));
  }
  else if(argc==5){
    anaGenerator(argv[1], argv[2], atoi(argv[3]), atoi(argv[4]));
  }
  else{
    printf("wrong argc %d\n", argc); return 1;
  }
  return 0;
}
