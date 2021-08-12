#include "stdlib.h"

#include "TMatrixD.h"
#include "TCanvas.h"
#include "TDirectory.h"
#include "TF1.h"
#include "TFile.h"
#include "TGraph.h"
#include "TH2D.h"
#include "TH3D.h"
#include "TLegend.h"
#include "TPaveStats.h"
#include "TSpline.h"
#include "TString.h"
#include "TStyle.h"
#include "TSystem.h"
#include "TTree.h"
#include "HistIO.h"
#include "GeneratorUtils.h"

using namespace HistIO;
using namespace std;

void getHist(const TString fn, const TString tag, const int anaid)
{
  cout<<"please check "<<fn<<" "<<tag<<" "<<anaid<<endl;

  TList *lout=new TList;
  TFile * fin = new TFile(fn);
  TTree * tree = (TTree*) fin->Get("tree");
  SetTree(tree, anaid);

  TString nuExp;
  int nuPDG, tarA;
  const TString gen = GeneratorUtils::SetEnv(fn, nuExp, nuPDG, tarA);
  const double histnormFactor = GeneratorUtils::GetHistNormPerNucleus(fin, nuPDG, tarA, gen, anaid);

  //=======================================================================<

  //new, works for both GiBUU and GENIE
  /*
    enum mode{
    kALL=0,
    kQE=1,
    kRES=2,
    kDIS=3,
    k2P2H=4,
    kOTHER=5
  };
  */
  const TString modes[]={"_all","_qe","_res","_dis","_2p2h", "_other"};
  const int pdsmin[]={0,     1, 2, 3, 4, 5};
  const int pdsmax[]={10000, 1, 2, 3, 4, 5};
  const Int_t nmode = sizeof(modes)/sizeof(TString);

  ULong64_t minnp, maxnp;
  const bool isTopoTask = GeneratorUtils::SetTopoCut(anaid, minnp, maxnp);

  const TString npiDecomp[]={"","_0pi","_1pi","_Mpi","_other"};
  //only gointo sub-category for TopoTask
  const int nNpiCut = isTopoTask? (sizeof(npiDecomp)/sizeof(TString)) : 1;

  //=======================================================================<

  TMatrixD countFill(nmode+1, nNpiCut+1);
  for(int imode=0; imode<nmode; imode++){

    for(int inpi=0;  inpi< nNpiCut; inpi++){
    
      const TString npicut = npiDecomp[inpi];
      const TString modetag = modes[imode]+npicut;

      const vector<TString> hhs = SetHist(modetag, nuExp, anaid, lout);
      
      int iEvt=0;
      int nFill = 0;
      while(tree->GetEntry(iEvt++)){
        if(perweight>4000){
          printf("\n\n\nAlert!!  Filling Super weight!! %d %f skiping...\n\n\n", iEvt, perweight); 
          continue;
        }
        
        if(anaid == CC1piNpID){
          if(targetZ == 1){//no hydrogen for GFS1
            continue;
          }
        }
        
        if(evtMode<pdsmin[imode] || evtMode>pdsmax[imode]){
          continue;
        }

        if(!GeneratorUtils::CutNpi(npicut, npar)){
          continue;
        }

        if(isTopoTask){
          if(npar<minnp || npar>maxnp){
            continue;
          }
        }
        
        bool passwr = true;
        if(anaid==NUBAR1PI){
          if(Wrest>=1.8){
            passwr = false;
          }
        }

        FillHist(passwr, isTopoTask);

        nFill++;
      }
      countFill[imode][inpi] = nFill;
      cout<<"\n filling: modetag "<<modetag<<" "<<"imode "<<imode<<", "<<pdsmin[imode]<<" evtMode "<<pdsmax[imode]<<", countFill[imode][inpi] "<<countFill[imode][inpi]<<endl<<endl;
      
      //========= fill 2D and 3D
      if(!isTopoTask){
        
        TString basecut="&& (perweight < 4000)";
        if(anaid == CC1piNpID){
          basecut += "&& (targetZ!=1) ";//no hydrogen
          printf("\n\n************************************** Require NO Hydrogen in the basecut! %d \"%s\"\n\n\n", anaid, basecut.Data());
        }
        
        TString modecut;
        GeneratorUtils::SetStringBaseCut(modecut, pdsmin[imode], pdsmax[imode]);
        printf("\n\n************************************** modecut: \"%s\" mode %d %d basecut: \"%s\" np %lld %lld\n\n\n", modecut.Data(), pdsmin[imode], pdsmax[imode], basecut.Data(), minnp, maxnp);

        const int nhist = hhs.size();
        for(int ii=0; ii<nhist; ii++){
          printf("\nii %d hhs %s\n\n", ii, hhs[ii].Data());
          
          TString kincut;
          if(anaid==NUBAR1PI && (hhs[ii]!="Wrest")){
            kincut = "&& (Wrest<1.8)";
          }
          
          const TString tmpcut = Form("perweight * (%s %s %s) ", modecut.Data(), basecut.Data(), kincut.Data());
          TString varname=hhs[ii];
          varname.ReplaceAll("VS",":");  
          varname.ReplaceAll("TIMES","*");
          cout<<"test "<<ii<<" cut "<<tmpcut<<" name "<<hhs[ii]+modetag<<" varname "<<varname<<endl;
          const int ndraw = tree->Project(hhs[ii]+modetag,varname,tmpcut);
          cout<<"tmpcut "<<tmpcut<<" ndraw "<<ndraw<<endl<<endl;
        }//nhist end
      }//!isTopoTask end
      
    }//npiDecomp end
  }//imode end

  for(int ii=0; ii<nmode; ii++){
    for(int jj=1; jj<nNpiCut; jj++){
      countFill[ii][nNpiCut]+=countFill[ii][jj];
    }
    if(countFill[ii][0]){
      countFill[ii][nNpiCut] /= countFill[ii][0];
    }
  }
  for(int jj=0; jj<nNpiCut; jj++){
    for(int ii=1; ii<nmode; ii++){
      countFill[nmode][jj]+=countFill[ii][jj];
    }
    if(countFill[0][jj]){
      countFill[nmode][jj] /= countFill[0][jj];
    }
  }
  countFill.Print();

  //================================================ finalizing =========================================

  const int nh = lout->GetEntries();
  for(int ii=0; ii<nh; ii++){
    TH1D * htmp = (TH1D*) lout->At(ii);    
    const TString hname(htmp->GetName());
    if(hname.Contains("enu") || hname.Contains("Erecoil")){
      printf("For %s, no width normalization!\n", hname.Data());
      htmp->Scale(1/histnormFactor);
    }
    else{
      htmp->Scale(1/histnormFactor, "width");
    }
  }

  TFile * fout=new TFile(Form("outplot/outHist%s_A%d_ana%d_tag%s.root",nuExp.Data(), tarA, anaid, tag.Data()),"recreate");
  lout->Write();
  fout->Save();
  fout->Close();

  cout<<"done!"<<endl;
}

int main(int argc, char * argv[])
{
  //void getHist(const TString fn, const TString tag, const int anaid)
  getHist(argv[1], argv[2], atoi(argv[3]));

  return 0;
}
