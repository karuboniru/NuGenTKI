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

const bool kthin=false;//true;//

TH1D* getMINERvA(const TString pint, const TString varname, TMatrixD *& cov)
{
  printf("\n\nget MINERvA %s\n\n", pint.Data());
  TFile *fin = 0x0;
  if(pint=="0pi"){
    fin = new TFile("data/PhysRevD.101.092001.root");
    //fin = new TFile("data/MINERvA_1805.05486_v3.root");//official_release_data_fullCov_deelastic
    //fin = new TFile("data/MINERvA_1805.05486_v2.root");//CC0piTKI_MINERvAPRL/official_release_data_fullCov
  }
  else if(pint=="pi0"){
    fin = new TFile("data/PIZEROTKI_MINERvA.root");
  }
  if(!fin){
    cout<<"no fin! "<<pint<<endl; exit(1);
  }

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
    cout<<"no hist"<<endl; exit(1);
  }
  hh=(TH1D*) hh->Clone("xsec");
  hh->SetDirectory(0);

  cov = (TMatrixD*) (lin->At(2));
  if(!cov){
    cout<<"no cov"<<endl; exit(1);
  }
  cov = (TMatrixD*) cov->Clone("cov");

  if( (pint=="pi0")&& (varname=="IApN"||varname=="dpTT") ){
    style::ScaleXaxis(hh, 1E-3);
    (*cov) *= 1E6;
  }

  fin->Close();
  delete fin;
  fin=0x0;

  cout<<"check flows "<<hh->GetName()<<" "<<hh->GetBinContent(0)<<" "<<hh->GetBinContent(hh->GetNbinsX()+1)<<endl;

  return hh;
}


TString getPIZEROvar(const TString var)
{
  if(var=="IApN"){
    return "pN";
  }
  else if(var=="dalphat"){
    return "dalphaT";
  }
  else if(var=="dpTT"){
    return "dpTT";
  }
  else{
    cout<<"pizerovar bad "<<var<<endl; exit(1);
  }

  return "";
}

void fixTitle(const int ivar, TH1D * hdata)
{
  const TString bads[]={"Me", "N", "deg.", "degrees"};
  const TString goods[]={"Ge", "n", "degree", "degree"};
  for(unsigned ii =0; ii<sizeof(bads)/sizeof(TString); ii++){
    TString xtmptit = hdata->GetXaxis()->GetTitle();
    xtmptit.ReplaceAll(bads[ii], goods[ii]);
    hdata->SetXTitle(xtmptit);
    TString ytmptit = hdata->GetYaxis()->GetTitle();
    ytmptit.ReplaceAll(bads[ii], goods[ii]);
    hdata->SetYTitle(ytmptit);
  }
}

int mcchi2(const int opt, const TString sgen, const TString pint)
{
  const TString varname[]={"muonmomentum","muontheta","protonmomentum","protontheta","dalphat","dphit","dpt", "IApN", "dpTT"};
  
  const int ivar = opt;

  TH1D * hdata = 0x0;
  TMatrixD *dataCov=0x0;
  hdata = getMINERvA(pint, varname[ivar], dataCov);  
  if(!hdata){
    cout<<"no data "<<varname[ivar]<<endl; 
    return 1;
  }

  style::ResetStyle(hdata); 
  hdata->SetLineColor(kBlack);//1012);
  hdata->SetLineWidth(2); 
  hdata->SetLineStyle(kSolid);
  hdata->SetMarkerStyle(21);
  hdata->SetMarkerSize(1);//2);
  hdata->SetMarkerColor(kBlack);//1015);//hdata->GetLineColor());

  fixTitle(ivar, hdata);

  const TString pi0v = getPIZEROvar(varname[ivar]);
  vector<TString> filetag, legtag;
  TString filehead, leghead, histname, histnameNuWro, histnameGENIE;
  if(sgen=="PRD"){
    filehead = "";
    filetag.push_back("data/nuisance/allnuisance/nuwro_CCnumi_CH_GFG_noFSI_"+pi0v);
    filetag.push_back("data/nuisance/allnuisance/nuwro_CCnumi_CH_GFG_"+pi0v);
    filetag.push_back("data/NuWro_JS/xianguo_august2020_effSF");

    //const TString tmpg = "NuWro ";
    //leghead="";

    const TString tmpg = "";
    leghead="NuWro";

    histname = Form("MINERvA_CCNpi0Mp_CH_XSec_1D%s_nu_MC", pi0v.Data());
    if(ivar==4){
      histnameNuWro = "alphaTpiN";
    }
    else if(ivar==7){
      histnameNuWro = "pn";
    }
    else{
      histnameNuWro = "dpTT";
    }
    
    legtag.push_back(tmpg+"RFG w/o FSI");
    legtag.push_back(tmpg+"RFG w/ FSI");
    legtag.push_back(tmpg+"ESF w/ FSI");
  }
  else if(sgen=="nuisancenuwro"){
    const bool kmix = 0;

    filehead = "";
    filetag.push_back("data/nuisance/allnuisance/nuwro_CCnumi_CH_GFG_noFSI_"+pi0v);
    filetag.push_back("data/nuisance/allnuisance/nuwro_CCnumi_CH_GFG_"+pi0v);
    //if(!kmix) filetag.push_back("data/nuisance/allnuisance/nuwro_CCnumi_CH_BRGFG_"+pi0v);
    //filetag.push_back("data/nuisance/allnuisance/nuwro_CCnumi_CH_LFG_"+pi0v);
    filetag.push_back("data/NuWro_JS/xianguo_august2020_effSF");
    if(kmix){
      //test_not_to_use filetag.push_back("data/nuisance/allnuisance/LocalFGNievesQEAndMEC_LFGHNFSI_"+pi0v);  
    }

    const TString tmpg = kmix?"NuWro ":"";
    leghead=kmix?"":"NuWro";    

    histname = Form("MINERvA_CCNpi0Mp_CH_XSec_1D%s_nu_MC", pi0v.Data());
    //histnameNuWro = filetag[3](filetag[3].First("/")+1, 1000)+"nu/"+varname[ivar];
    if(ivar==4){
      histnameNuWro = "alphaTpiN";
    }
    else if(ivar==7){
      histnameNuWro = "pn";
    }
    else{
      histnameNuWro = "dpTT";
    }
    
    legtag.push_back(tmpg+"RFG w/o FSI");
    legtag.push_back(tmpg+"RFG w/ FSI");
    //if(!kmix) legtag.push_back(tmpg+"BR-RFG w/ FSI");
    //legtag.push_back(tmpg+"LFG w/ FSI");
    legtag.push_back(tmpg+"ESF w/ FSI");
    if(kmix){
      if(filetag.back().Contains("nuwroplaceholder")){
        legtag.push_back("NuWro JS");
      }
      else{
        //test_not_to_use legtag.push_back("GENIE LFG hN");
      }
    }
  }
  else if(sgen=="nuisancegenie"){
    filehead = "data/nuisance/allnuisance/";
    filetag.push_back("DefaultPlusMECWithNC_Std_"+pi0v);
    filetag.push_back("DefaultPlusMECWithNC_HNFSI_"+pi0v);
    filetag.push_back("LocalFGNievesQEAndMEC_LFG_"+pi0v);
    filetag.push_back("LocalFGNievesQEAndMEC_LFGHNFSI_"+pi0v);

    leghead="GENIE";
    histname = Form("MINERvA_CCNpi0Mp_CH_XSec_1D%s_nu_MC", pi0v.Data());
    legtag.push_back("BR-RFG hA");
    legtag.push_back("BR-RFG hN");
    legtag.push_back("LFG hA");
    legtag.push_back("LFG hN");
  }
  else if(sgen=="oobgibuu"){
    if(pint=="0pi"){
      //filehead = "data/MINERvALEGiBUUGFS0PIa9t1nuCarbon";
      //filehead = "data/GFS0PIMINERvAGiBUULE_CarbonOnly";
      filehead = "data/GFS0PIMINERvAGiBUULE_newE";
    }
    else if(pint=="pi0"){
      //filehead = "data/MINERvALEGiBUUGFSPIZEROa7t4nuCarbon";
      //filehead = "data/GFSPIZEROMINERvAGiBUULE_CarbonOnly";
      filehead = "data/GFSPIZEROMINERvAGiBUULE_newE";
    }
    filetag.push_back("");

    leghead="GiBUU";
    histname = filehead(filehead.First("/")+1, 1000)+"/"+varname[ivar];
    legtag.push_back("");
  }
  else if(sgen=="oobnofsigibuu"){
    if(pint=="0pi"){
      filehead = "data/GFS0PIMINERvAGiBUUNOFSIPiCut2Fixed";
      //filehead = "data/GFS0PIMINERvAGiBUUNoFSI4kEns";
    }
    else if(pint=="pi0"){
      filehead = "data/GFSPIZEROMINERvAGiBUUNOFSIPiCut2Fixed";
      //filehead = "data/GFSPIZEROMINERvAGiBUUNoFSI4kEns";
    }
    filetag.push_back("");

    leghead="GiBUU w/o FSI";
    histname = filehead(filehead.First("/")+1, 1000)+"/"+varname[ivar];
    legtag.push_back("");
  }
  else if(sgen=="geniev3oob"){
    if(pint=="0pi"){
      filehead = "data/GFS0PIMINERvAGENIE_v3OOB_LE_CarbonOnly";
    }
    else if(pint=="pi0"){
      filehead = "data/GFSPIZEROMINERvAGENIE_v3OOB_LE_CarbonOnly";
    }
    filetag.push_back("");

    leghead="GENIE v3.0.6";
    histname = filehead(filehead.First("/")+1, 1000)+"/"+varname[ivar];
    legtag.push_back("");
  }
  else if(sgen=="geniev2DC"){
    if(pint=="0pi"){
      filehead = "data/GFS0PIMINERvAGENIE_v2DC_LE_CarbonOnly";
    }
    else if(pint=="pi0"){
      filehead = "data/GFSPIZEROMINERvAGENIE_v2DC_LE_CarbonOnly";
    }
    filetag.push_back("");

    leghead="GENIE v2.12.10-DC";
    histname = filehead(filehead.First("/")+1, 1000)+"/"+varname[ivar];
    legtag.push_back("");
  }
  else if(sgen=="genieV2RG"){
    if(pint=="0pi"){
      filehead = "data/GFS0PIMINERvAGENIE_V2RG_LE_CarbonOnly";
    }
    else if(pint=="pi0"){
      filehead = "data/GFSPIZEROMINERvAGENIE_V2RG_LE_CarbonOnly";
    }
    filetag.push_back("");

    leghead="GENIE v2.12.10-RG";
    histname = filehead(filehead.First("/")+1, 1000)+"/"+varname[ivar];
    legtag.push_back("");
  }
  else if(sgen=="geniev2TJ"){
    if(pint=="0pi"){
      filehead = "data/GFS0PIMINERvAGENIEv2TJ_LE_CarbonOnly";
    }
    else if(pint=="pi0"){
      filehead = "data/GFSPIZEROMINERvAGENIEv2TJ_LE_CarbonOnly";
    }
    filetag.push_back("");

    leghead="GENIE v2.12.10-TJ";
    histname = filehead(filehead.First("/")+1, 1000)+"/"+varname[ivar];
    legtag.push_back("");
  }
  else{
    cout<<"wrong sgen 1 "<<sgen<<endl; exit(1);
  }

  const unsigned nhist=filetag.size();
  vector<TH1D *> hmc;
  vector<THStack *> smc;
  //const int stkcols[]={1002, 1009, 1005, 1008, 1011, 1014, 1017};
  const int stkcols[]={1014, 1003, 1002, 1009, 1010, 1007, 1012, 1005, 1011, 1008, 1014, 1017};
  const int lincols[]={1015, 1002, 1009, 1008, 1011, 1014, 1017};

  for(unsigned ii=0; ii<nhist; ii++){
    TFile * fmc = 0x0;
    TH1D *htmp = 0x0;
    THStack * stmp = 0x0;

    fmc = new TFile(Form("%s%s.root", filehead.Data(),  filetag[ii].Data()));
    if(!fmc){
      cout<<"no fmc!"<<endl; exit(1);
    }
    htmp = dynamic_cast<TH1D*> (fmc->Get(histname));
    stmp = dynamic_cast<THStack*> (fmc->Get(histname));
    if( !htmp){
      htmp = dynamic_cast<TH1D*> (fmc->Get(histnameNuWro));
      if(!htmp){
        if(!stmp){
          stmp = dynamic_cast<THStack*> (fmc->Get(histnameGENIE));
        }
        if(stmp){//also can be GiBUU here
          htmp = style::GetStackedSum(stmp);
        }
      }

      if(!htmp){
        cout<<"no hmc! "<<histname<<" "<<histnameNuWro<<" "<<histnameGENIE<<endl;
        gDirectory->ls();
        exit(1);
      }
    }
    else{
      if(!sgen.Contains("oob")&&varname[ivar]!="dalphat"){
        style::ScaleXaxis(htmp, 1E-3);
      }
    }

    style::ResetStyle(htmp);
    htmp->SetLineColor(style::GetColor(lincols[ii]));  
    htmp->SetLineWidth(3);

    if(legtag[ii].Contains("GENIE")){
      htmp->SetLineStyle(kDashed);
      htmp->SetLineWidth(5);
    }

    TH1D * hcopy = (TH1D*)(htmp->Clone(Form("h%d",ii)));
    hcopy->SetDirectory(0);
    hmc.push_back(hcopy);
    delete htmp;
    htmp = 0x0;

    if(stmp){
      style::ResetStyle(stmp);
      style::SetStackColorCB(stmp, stkcols, 3);
      THStack * scopy = (THStack*)(stmp->Clone(Form("s%d",ii)));
      //scopy->SetDirectory(0);
      smc.push_back(scopy);
      delete stmp;
      stmp=0x0;
    }
    
    if(fmc){
    fmc->Close();
    delete fmc;
    fmc=0x0;
    }

    int noff=1, xnbin = hdata->GetNbinsX();
    if(pint=="0pi"){
      noff=2;
      if(varname[ivar]=="IApN")
        xnbin -= 2;
      else if(varname[ivar]=="dalphat")
        xnbin -= 1;
      else{
        cout<<"wrong varname "<<varname[ivar]<<endl; exit(1);
      }
    }
    const double dummyunit = 1E-42;

    TH1D * hfit = (TH1D*)hmc[ii]->Clone("hfit");//(TH1D*) fmc->Get(Form("MINERvA_CCNpi0Mp_CH_XSec_1D%s_nu_MC", pi0v.Data()));

    for(int ii=0; ii<=hfit->GetNbinsX()+1; ii++){
      printf("test bin %d, %f %f %e %e\n", ii, hfit->GetXaxis()->GetBinLowEdge(ii), hfit->GetXaxis()->GetBinUpEdge(ii), hfit->GetBinContent(ii), hfit->GetBinError(ii));
    }

    const double fchi2 = style::GetChi2(hdata, *dataCov, noff, xnbin, hfit, dummyunit);
    printf("Check %s %d %s chi2 %e\n", hfit->GetName(), ii, pi0v.Data(), fchi2);
    if(smc.size()){
      leghead += Form("  #chi^{2}=%.0f", fchi2);
    }
    else{
      legtag[ii] += Form("  #chi^{2}=%.0f", fchi2);
    }
    delete hfit;
    hfit=0x0;
  }

  //_________________________________________________________________________________
  //_________________________________________________________________________________  
  const Double_t currentLeft=kthin?0.17:0.12; 
  const Double_t currentTop=0.08; 
  const Double_t currentRight=kthin?0.045:0.037;
  const Double_t currentBottom=0.17;

  const bool kleft = (opt==1 || opt==3 || opt==4 || opt==8);

  TCanvas *cc=new TCanvas("cc","cc",kthin?300:600,400);
  style::PadSetup(cc);
  
  //cc->GetCanvas()->SetGrayscale();
  gPad->SetLeftMargin(currentLeft);//square
  gPad->SetRightMargin(currentRight);//0.03//square
  gPad->SetTopMargin(currentTop);
  gPad->SetBottomMargin(currentBottom);

  TH1D * hfirst = hdata;

  double xmin=-999, xmax=-999;

  if(varname[ivar]=="dalphat"){
    xmin = 0;
    xmax = 180;
  }
  else if(varname[ivar]=="dpTT"){
    xmax = 400/1E3;
    xmin = -xmax;
  }
  else if(varname[ivar]=="IApN"){
    xmin = 0;
    xmax = 800/1E3;
  }
  else{
    printf("varname error %d %s\n", ivar, varname[ivar].Data()); exit(1);
  }

  hfirst->GetXaxis()->SetRangeUser(xmin, xmax);

  double amax = 14E-3;
  double pmax = (sgen.Contains("gibuu")? 3.6 : 7.1);
  //need it to have overhead space to hold the asymmetry inset plot
  if(opt==8 && pint=="pi0"){
    pmax = 7.1;
  }
  if(pint=="0pi"){
    pmax *= 1.7;
    amax *= 1.7;
  }

 if(sgen.Contains("nofsi")||sgen.Contains("genie")){
   pmax = (pint=="0pi"?8.3:5.5);
 }

  const double hm[]={1.2, 0.235, 5.5, 8, amax, 0.151, 6.5, pmax, pmax, 5};
  hfirst->SetMaximum(hm[opt]*1e-39);
  hfirst->SetMinimum(0);

  //const double x0=kleft?0.15:0.7;
  //const double x0=kleft?0.14:0.53;//square
  const double x0=kleft?(kthin?0.2:0.14):(sgen.Contains("gibuu")&&!sgen.Contains("nofsi")?(kthin?0.6:0.7): (kthin?0.45:0.47));//square
  const double xsize=0.42;
  TLegend *lg = new TLegend(x0, 0.55, x0+xsize, 0.89);
  if(leghead.Length()){
    lg->SetHeader(leghead);
  }

  style::ResetStyle(lg,0.18,0.68); lg->SetTextAlign(12); //lg->SetBorderSize(1);

  //need to draw data first before THStack
  hdata->Draw("X1 E0");

  const double modThres = 0.01;
  //const TString dopt="same hist";
  const TString dopt="same histC";
  //const TString dopt="same histL";

  for(unsigned ii=0; ii<nhist; ii++){
    if(smc.size()==nhist){
      smc[ii]->Draw(dopt);
      style::AddStackToLegend(smc[ii], lg, legtag[ii], modThres);
    }
    else{
      if(legtag[ii].Contains("w/o FSI")||legtag[ii].Contains("BR-RFG hA")||sgen=="oobgibuu"){
        hmc[ii]->SetFillColor(style::GetColor(lincols[ii]));
        hmc[ii]->SetLineColor(style::GetColor(1014));
        // hmc[ii]->SetFillStyle(3544);
        hmc[ii]->SetLineWidth(3);
        hmc[ii]->Draw(dopt);
        lg->AddEntry(hmc[ii],legtag[ii],"fb");
      }
      else{
        hmc[ii]->Draw(dopt);
        lg->AddEntry(hmc[ii],legtag[ii],"pl");
      }
    }
  }

  TString pleg;
  if(pint=="0pi"){
    pleg+="QE-like ";
  }
  else if(pint=="pi0"){
    pleg+="#pi^{0} ";
  }

  lg->AddEntry(hdata,pleg+"Data","pl");

  hdata->Draw("same X1 E0");
  hdata->Draw("same axis");

  lg->Draw();
  
  if((sgen.Contains("oobgibuu")||sgen.Contains("nuisancenuwro")||sgen.Contains("PRD"))&&pint.Contains("pi0")){
    const double xsublable = kleft?(opt==4?0.75:(kthin?0.8:0.89)):(kthin?0.2:0.15);
    const double ysublable = /*(opt==8&&sgen.Contains("gibuu"))?0.48:*/0.83;
    TLatex * lt1 = 0x0;
    if(opt==4 && kthin){
      lt1= new TLatex(xsublable, ysublable, sgen.Contains("gibuu")?"(d)":"(c)");
    }
    else{
      lt1= new TLatex(xsublable, ysublable, sgen.Contains("gibuu")?"(b)":"(a)");
    }
    style::ResetStyle(lt1);
    lt1->Draw();
  }

  const TString tag(sgen+pint);
  cc->Print(Form("outplot/final/mcchi2minerva%d%s.eps", opt, tag.Data()));
  cc->Print(Form("outplot/final/mcchi2minerva%d%s.png", opt, tag.Data()));

  return 0;
}

int main()
{
  style::SetGlobalStyle();
  
  style::fgkTextSize = 0.07; //0.05;
  style::fgkTitleSize = 0.07;
  style::fgkYTitleOffset = kthin?1.08:0.75;
  style::fgkXTitleOffset = 1.1;

  vector<int> opt; 
  opt.push_back(4);
  opt.push_back(7);
  opt.push_back(8);

  vector<TString> sgen;

  sgen.push_back("PRD");
  sgen.push_back("oobgibuu");
  //sgen.push_back("geniev3oob");
  //sgen.push_back("geniev2DC");
  //sgen.push_back("genieV2RG");
  //sgen.push_back("geniev2TJ");
  //sgen.push_back("oobnofsigibuu");
  //sgen.push_back("nuisancenuwro");
  //sgen.push_back("nuisancegenie");

  vector<TString> pint;
  pint.push_back("0pi");
  pint.push_back("pi0");

  for(unsigned io=0; io<opt.size(); io++){
    for(unsigned is=0; is<sgen.size(); is++){
      for(unsigned ik=0; ik<pint.size(); ik++){
        if(pint[ik]=="0pi"
           &&
           (opt[io]==8||sgen[is]=="nuisancenuwro"||sgen[is]=="nuisancegenie"||sgen[is]=="PRD")){
          continue;
        }
        mcchi2(opt[io], sgen[is], pint[ik]);
      }
    }
  }

  return 0;
}
