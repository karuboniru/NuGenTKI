#include "TString.h"
#include "TH2D.h"
#include "TTree.h"
#include "TDirectory.h"
#include "TF1.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TLine.h"
#include "TSystem.h"
#include "TPaveStats.h"
#include "TLegend.h"

#include "style.h"

const bool kthin = false;//true;

void setYtitleNorm(TH1D *hist)
{
  TString ytit = hist->GetYaxis()->GetTitle();

  //ytit=ytit(0, ytit.First("("));
  //ytit += "(area normalized
  ytit.ReplaceAll("  ("," (");
  ytit.ReplaceAll(" (", "/#sigma (");
  ytit.ReplaceAll("cm^{2}","1");
  ytit.ReplaceAll("/nucleon","");
  hist->SetYTitle(ytit);
}

TH1D *getMINERvA0PI(const TString varname, TMatrixD *& cov)
{
  printf("\n\nget MINERvA 0PI\n\n");
  //TFile *fdata=new TFile("data/CC0piTKI_MINERvAPRL/official_release_data_fullCov/MINERvA_1805.05486_v2.root");
  //TFile *fdata=new TFile("data/CC0piTKI_MINERvAPRL/official_release_data_fullCov_deelastic/MINERvA_1805.05486_v3.root");
  TFile *fdata=new TFile("data/PhysRevD.101.092001.root");

  TList * tmplist = (TList*)fdata->Get(varname);
  if(!tmplist){
    if(varname=="IApN"){
      //old name
      tmplist = (TList*)fdata->Get("neutronmomentum");
    }
    if(!tmplist){
      cout<<"no tmplist "<<varname<<endl; exit(1);
    }
  }  
  TH1D * hh  = (TH1D*) tmplist->At(0);      
  if(!hh){
    cout<<"no 0pi hist! "<<varname<<endl; exit(1);
  }
  printf("check norm 0pi raw: %e\n", hh->Integral(0,100000,"width"));

  style::ResetStyle(hh);
  cov = (TMatrixD*) tmplist->At(2); 
  if(!cov){
    cout<<"no 0pi cov! "<<varname<<endl; exit(1);
  }
  return hh;
}

TH1D* getMINERvAPIZERO(const TString varname, TMatrixD *& cov)
{
  printf("\n\nget MINERvA PIZERO\n\n");
  TFile *fin = new TFile("data/PIZEROTKI_MINERvA.root");
  TList * lin = (TList*)fin->Get(varname);
  if(!lin){
    if(varname=="IApN"){
      //old name
      lin = (TList*)fin->Get("neutronmomentum");
    }
    if(!lin){
      cout<<"no list "<<varname<<endl; exit(1);
    }
  }

  TH1D * hh = (TH1D*) (lin->At(0));
  if(!hh){
    cout<<"no pi0 hist! "<<varname<<endl; exit(1);
  }
  printf("check norm pi0 raw (problematic with overflow bin--the bin width is taken from last bin instead of the overflow): %e\n", hh->Integral(0,100000,"width"));

  //TMatrixD * errm = (TMatrixD*) (fin->Get(varname)->At(2));
  cov = (TMatrixD*) (lin->At(2));//style::Hist2Matrix(errm, 1E76);
  if(!cov){
    cout<<"no pi0 cov! "<<varname<<endl; exit(1);
  }
  //hh->Draw();

  cout<<"check flows "<<hh->GetName()<<" "<<hh->GetBinContent(0)<<" "<<hh->GetBinContent(hh->GetNbinsX()+1)<<endl;

  style::ResetStyle(hh); 

  return hh;
}

TH1D * norm2Shape(const TH1D *hh, const TMatrixD * rawcov)
{
  printf("\n\n============================== norm2Shape ====================\n");

  style::PrintHist(hh, Form("%s old", hh->GetName()));
  
  int hb0=-999, hll=-999;
  style::GetHistB0L(hh, 1E-100, hb0, hll);
  printf("norm2Shape b0 %d l %d\n", hb0, hll);

  const int hbuff = hb0-1;
  const int nbins = hll;

  const int mbuff = hbuff+1;

  TMatrixDSym tmpcov(nbins);
  for(int ii=0; ii<nbins; ii++){
    for(int jj=0; jj<nbins; jj++){
      tmpcov(ii,jj)=(*rawcov)(ii+mbuff, jj+mbuff);
    }
  }
  rawcov->Print();
  tmpcov.Print();

  TH1D * tmph=(TH1D*) hh->Clone(Form("tmp%s", hh->GetName()));
  double newbins[nbins+2];
  for(int ii=0; ii<=nbins+1; ii++){
    newbins[ii] = hh->GetXaxis()->GetBinUpEdge(ii+hbuff);
  }
  tmph->SetBins(nbins, newbins);
  tmph->Scale(0);
  for(int ii=0; ii<=nbins+1; ii++){
    if( (ii+hbuff)<=(hh->GetNbinsX()+1) ){
      tmph->SetBinContent(ii, hh->GetBinContent(ii+hbuff));
      tmph->SetBinError(ii, hh->GetBinError(ii+hbuff));
    }
  }

  style::PrintHist(tmph, Form("%s new", tmph->GetName()));

  //==================
  TH1D * hout =0x0;
  TMatrixDSym * dummy = style::ExtractShapeOnlyCovar(&tmpcov, tmph, hout);
  //hout=(TH1D*) tmph->Clone(Form("noshape%s", tmph->GetName()));
  delete dummy;
  delete tmph;
  //==================
  setYtitleNorm(hout);

  const double norm = hout->Integral(0,100000,"width");
  hout->Scale(1./norm);

  cout<<"check "<<hout->GetName()<<" norm sys "<<norm<<endl;

  return hout;
}

int draw(const int opt = 0)
{
  const TString varname[]={"muonmomentum","muontheta","protonmomentum","protontheta","dalphat","dphit","dpt", "IApN", "dpTx", "dpTy"};
  
  const int ivar = opt;

  TMatrixD * data1Cov = 0x0;
  TMatrixD *DATA2Cov=0x0;
  const TH1D * hdata1 = getMINERvA0PI(varname[ivar], data1Cov);
  const TH1D * hDATA2 = getMINERvAPIZERO(varname[ivar], DATA2Cov);  

  TH1D * h1draw = norm2Shape(hdata1, data1Cov);
  TH1D * h2DRAW = norm2Shape(hDATA2, DATA2Cov);    
  if(varname[ivar]=="IApN"||varname[ivar]=="dpt"){
    style::ScaleXaxis(h2DRAW, 1E-3);
  }

  //_________________________________________________________________________________
  //_________________________________________________________________________________  


  const Double_t currentLeft=kthin?0.17:0.12; 
  const Double_t currentTop=0.08; 
  const Double_t currentRight=kthin?0.045:0.037;
  const Double_t currentBottom=0.17;

  const bool kleft = (opt==1 || opt==3 || opt==4);

  TCanvas *cc=dynamic_cast<TCanvas *> (gDirectory->Get("cc"));
  if(!cc){
    cc=new TCanvas("cc","cc",kthin?300:600,400);
    style::PadSetup(cc);

    //cc->GetCanvas()->SetGrayscale();
    gPad->SetLeftMargin(currentLeft);//square
    gPad->SetRightMargin(currentRight);//0.03//square
    gPad->SetTopMargin(currentTop);
    gPad->SetBottomMargin(currentBottom);
  }

  TH1D * hfirst = h1draw;

  if(ivar==7){
    TString xtmptit = hfirst->GetXaxis()->GetTitle();
    xtmptit.ReplaceAll("N","n");
    hfirst->SetXTitle(xtmptit);
    TString ytmptit = hfirst->GetYaxis()->GetTitle();
    ytmptit.ReplaceAll("N","n");
    hfirst->SetYTitle(ytmptit);
  }
  if(ivar==4){
    TString xtmptit = hfirst->GetXaxis()->GetTitle();
    xtmptit.ReplaceAll("degrees","degree");
    hfirst->SetXTitle(xtmptit);
    TString ytmptit = hfirst->GetYaxis()->GetTitle();
    ytmptit.ReplaceAll("degrees","degree");
    hfirst->SetYTitle(ytmptit);
  }

  const double hm[]={1.2, 0.235, 5.5, 75E-3, 13E-3, 0.07, 3.2, 3.2, 7.1, 5};
  hfirst->SetMaximum(hm[opt]);
  hfirst->SetMinimum(0);

  double xmin=-999, xmax=-999;
  if(varname[ivar]=="dalphat"){
    xmin = 0;
    xmax = 180;
  }
  else if(varname[ivar]=="dphit"){
    xmin = 0;
    xmax = 180;
  }
  else if(varname[ivar]=="IApN"){
    xmin = 0;
    xmax = 0.8;//1;
  }
  else if(varname[ivar]=="dpt"){
    xmin = 0;
    xmax = 1;
  }
  else{
    printf("varname error %d %s\n", ivar, varname[ivar].Data()); exit(1);
  }

  hfirst->GetXaxis()->SetRangeUser(xmin, xmax);

  //const double x0=kleft?0.15:0.7;
  //const double x0=kleft?0.2:0.67;//square
  const double x0=kleft?(kthin?0.27:0.2):(kthin?0.5:0.6);//square

  TLegend *lg = new TLegend(x0, 0.65, x0+0.34, 0.88);
  lg->SetHeader("MINERvA LE data");
  style::ResetStyle(lg,0.18,0.68);
  lg->SetTextAlign(12); //lg->SetBorderSize(1);

  //lg->AddEntry(h1draw,"CC 0#pi #it{N}p","pl");
  lg->AddEntry(h1draw,"CC QE-like #it{N}p","pl");
  lg->AddEntry(h2DRAW,"CC #it{M}#pi^{0} #it{N}p","pl");

  const int col1 = style::GetColor(1003);
  const int col2 = style::GetColor(1011);  

  h1draw->SetMarkerStyle(20);
  h1draw->SetMarkerSize(1);  
  h1draw->SetMarkerColor(col1); 
  h1draw->SetLineStyle(kSolid);
  h1draw->SetLineWidth(2); 
  h1draw->SetLineColor(col1); 

  h2DRAW->SetMarkerStyle(25);
  h2DRAW->SetMarkerSize(2);  
  h2DRAW->SetMarkerColor(col2); 
  h2DRAW->SetLineStyle(kSolid);
  h2DRAW->SetLineWidth(3); 
  h2DRAW->SetLineColor(col2); 

  hfirst->Draw("X1 E0 axis");  

  h1draw->Draw("same X1 E0");
  h2DRAW->Draw("same X1 E0");
  
  hfirst->Draw("sameaxis");
  
  printf("check norm h1 final: %e\n", h1draw->Integral(0,100000,"width"));   
  printf("check norm h2 final: %e\n", h2DRAW->Integral(0,100000,"width"));   

  lg->Draw();

  TLatex * lt1 = new TLatex(kthin?0.2:0.15, 0.83, opt==4?"(b)":"(a)");
  style::ResetStyle(lt1, 0.7);
  lt1->Draw();
  
  const TString tag;
  cc->Print(Form("outplot/final/minerva%d%s.eps", opt, tag.Data()));
  cc->Print(Form("outplot/final/minerva%d%s.png", opt, tag.Data()));

  return 0;
}

int main()
{
  style::SetGlobalStyle();
  //style::fgkYTitleOffset=1.6;//1.2;//square
  

  style::SetGlobalStyle();
  
  style::fgkTextSize = 0.07; //0.05;
  style::fgkTitleSize = 0.07;
  style::fgkYTitleOffset = kthin?1.08:0.75;
  style::fgkXTitleOffset = 1.1;

  vector<int> opts;
  opts.push_back(4);
  //opts.push_back(5);
  //opts.push_back(6);
  opts.push_back(7);
  for(unsigned ii=0; ii<opts.size(); ii++){
    draw(opts[ii]);
  }

  return 0;
}
