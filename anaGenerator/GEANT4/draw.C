void savedraw(const int iplot, TTree *t, TCanvas *c1, const TString var, const TString cut="1", const TString opt="hist")
{
  gStyle->SetOptStat("enoum");
  if(opt.Contains("same")){
    gPad->SetLogy(0);
  }
  else{
    gPad->SetLogy(1);
  }
  t->Draw(var, cut, opt);
  c1->Print(Form("outplot/outdraw/%splot%d.png",c1->GetName(), iplot));
}

void subdrawPiZero_OneP(TTree * t)
{
  TCanvas * c1 = new TCanvas("PiZero_OneP","",600,400);
  int iplot=0;
  const TString sig = "(totparcount-1000000*nGamma)==100001";
  savedraw(iplot++, t, c1, "nProton==1");
  savedraw(iplot++, t, c1, "nPiZero==1", "nProton==1");
  savedraw(iplot++, t, c1, "nPiZero==1 && nProton==1");
  savedraw(iplot++, t, c1, "nPion==0", "nPiZero==1 && nProton==1");
  savedraw(iplot++, t, c1, "nPion==0 && nPiZero==1 && nProton==1");
  savedraw(iplot++, t, c1, "nBkg==0", "nPion==0 && nPiZero==1 && nProton==1");
  savedraw(iplot++, t, c1, "nBkg==0 && nPion==0 && nPiZero==1 && nProton==1");//selection rate 3.6%
  savedraw(iplot++, t, c1, sig, "nBkg==0 && nPion==0 && nPiZero==1 && nProton==1");//purity 28%

  //https://www.wolframalpha.com/input/?i=argon-39+mass+in+gev //36.295028 GeV
  savedraw(iplot++, t, c1, "event_recoilM", sig+" && nBkg==0 && nPion==0 && nPiZero==1 && nProton==1");
  savedraw(iplot++, t, c1, "event_recoilM", "nBkg==0 && nPion==0 && nPiZero==1 && nProton==1 && abs(event_recoilM-36.295)<0.002");
  savedraw(iplot++, t, c1, sig, "nBkg==0 && nPion==0 && nPiZero==1 && nProton==1 && abs(event_recoilM-36.295)<0.002");//purity 99.99%

  //36.295 - 36.304
  //NOTE: need to draw smeared kinematic reconstructed recoilM, resolution should be much smaller than missing nucleon mass from neutron!
  savedraw(iplot++, t, c1, "recoilM", sig+" && nBkg==0 && nPion==0 && nPiZero==1 && nProton==1");
  savedraw(iplot++, t, c1, "recoilM", "nBkg==0 && nPion==0 && nPiZero==1 && nProton==1 && abs(recoilM-36.295)<0.009");
  savedraw(iplot++, t, c1, sig, "nBkg==0 && nPion==0 && nPiZero==1 && nProton==1 && abs(recoilM-36.295)<0.009");//purity 86.4%

  savedraw(iplot++, t, c1, "recoilP", "nBkg==0 && nPion==0 && nPiZero==1 && nProton==1 && abs(recoilM-36.295)<0.009");
  savedraw(iplot++, t, c1, "event_recoilP", "nBkg==0 && nPion==0 && nPiZero==1 && nProton==1 && abs(recoilM-36.295)<0.009","e same");
}


void subdrawPion_OneP(TTree * t)
{
  TCanvas * c1 = new TCanvas("Pion_OneP","",600,400);
  int iplot=0;
  const TString sig = "(totparcount-1000000*nGamma)==11";
  savedraw(iplot++, t, c1, "nProton==1");
  savedraw(iplot++, t, c1, "nPion==1", "nProton==1");
  savedraw(iplot++, t, c1, "nPion==1 && nProton==1");
  savedraw(iplot++, t, c1, "nPiZero==0", "nPion==1 && nProton==1");
  savedraw(iplot++, t, c1, "nPiZero==0 && nPion==1 && nProton==1");
  savedraw(iplot++, t, c1, "nBkg==0", "nPiZero==0 && nPion==1 && nProton==1");
  savedraw(iplot++, t, c1, "nBkg==0 && nPiZero==0 && nPion==1 && nProton==1");//selection rate 8.3%
  savedraw(iplot++, t, c1, sig, "nBkg==0 && nPiZero==0 && nPion==1 && nProton==1");//purity 30%

  //https://www.wolframalpha.com/input/?i=chlorine-39+mass+in+gev //36.2984698
  savedraw(iplot++, t, c1, "event_recoilM", sig +" && nBkg==0 && nPiZero==0 && nPion==1 && nProton==1");
  savedraw(iplot++, t, c1, "event_recoilM","nBkg==0 && nPiZero==0 && nPion==1 && nProton==1 && abs(event_recoilM-36.298)<0.04");
  savedraw(iplot++, t, c1, sig,"nBkg==0 && nPiZero==0 && nPion==1 && nProton==1 && abs(event_recoilM-36.298)<0.04");//purity 99.98%
  
  //36.295 - 36.309
  savedraw(iplot++, t, c1, "recoilM", sig +" && nBkg==0 && nPiZero==0 && nPion==1 && nProton==1");
  savedraw(iplot++, t, c1, "recoilM","nBkg==0 && nPiZero==0 && nPion==1 && nProton==1 && abs(recoilM-36.298)<0.011");
  savedraw(iplot++, t, c1, sig,"nBkg==0 && nPiZero==0 && nPion==1 && nProton==1 && abs(recoilM-36.298)<0.011");//purity 97.1

  savedraw(iplot++, t, c1, "recoilP","nBkg==0 && nPiZero==0 && nPion==1 && nProton==1 && abs(recoilM-36.298)<0.011");
  savedraw(iplot++, t, c1, "event_recoilP","nBkg==0 && nPiZero==0 && nPion==1 && nProton==1 && abs(recoilM-36.298)<0.011", "e same");
}


void subdrawPiZero_TwoP(TTree * t)
{
  //NOTE:something special with pi0 2p!!
  
  TCanvas * c1 = new TCanvas("PiZero_TwoP","",600,400);
  int iplot=0;
  const TString sig = "(totparcount-1000000*nGamma)==100002";
  savedraw(iplot++, t, c1, "nProton==2");
  savedraw(iplot++, t, c1, "nPiZero==1", "nProton==2");
  savedraw(iplot++, t, c1, "nPiZero==1 && nProton==2");
  savedraw(iplot++, t, c1, "nPion==0", "nPiZero==1 && nProton==2");
  savedraw(iplot++, t, c1, "nPion==0 && nPiZero==1 && nProton==2");
  savedraw(iplot++, t, c1, "nBkg==0", "nPion==0 && nPiZero==1 && nProton==2");
  savedraw(iplot++, t, c1, "nBkg==0 && nPion==0 && nPiZero==1 && nProton==2");//selection rate 1.9%
  savedraw(iplot++, t, c1, sig, "nBkg==0 && nPion==0 && nPiZero==1 && nProton==2");//purity 0.94%

  //https://www.wolframalpha.com/input/?i=chlorine-38+mass+in+gev //35.3669778
  savedraw(iplot++, t, c1, "event_recoilM",sig+" && nBkg==0 && nPion==0 && nPiZero==1 && nProton==2");
  savedraw(iplot++, t, c1, "event_recoilM","nBkg==0 && nPion==0 && nPiZero==1 && nProton==2 && abs(event_recoilM-35.367)<0.003");
  savedraw(iplot++, t, c1, sig,"nBkg==0 && nPion==0 && nPiZero==1 && nProton==2 && abs(event_recoilM-35.367)<0.003");//purity 1

  savedraw(iplot++, t, c1, "recoilM",sig+" && nBkg==0 && nPion==0 && nPiZero==1 && nProton==2");
  savedraw(iplot++, t, c1, "recoilM","nBkg==0 && nPion==0 && nPiZero==1 && nProton==2 && abs(recoilM-35.367)<0.009");
  //savedraw(iplot++, t, c1, sig,"nBkg==0 && nPion==0 && nPiZero==1 && nProton==2 && abs(recoilM-35.367)<0.009");//55.7%
  savedraw(iplot++, t, c1, sig,"nBkg==0 && nPion==0 && nPiZero==1 && nProton==2 && abs(recoilM-35.367)<0.007");//99.43%

  savedraw(iplot++, t, c1, "recoilP","nBkg==0 && nPion==0 && nPiZero==1 && nProton==2 && abs(recoilM-35.367)<0.007");
  savedraw(iplot++, t, c1, "event_recoilP","nBkg==0 && nPion==0 && nPiZero==1 && nProton==2 && abs(recoilM-35.367)<0.007","same e");
}


void subdrawPion_TwoP(TTree * t)
{
  TCanvas * c1 = new TCanvas("Pion_TwoP","",600,400);
  int iplot=0;
  const TString sig = "(totparcount-1000000*nGamma)==12";
  savedraw(iplot++, t, c1, "nProton==2");
  savedraw(iplot++, t, c1, "nPion==1", "nProton==2");
  savedraw(iplot++, t, c1, "nPion==1 && nProton==2");
  savedraw(iplot++, t, c1, "nPiZero==0", "nPion==1 && nProton==2");
  savedraw(iplot++, t, c1, "nPiZero==0 && nPion==1 && nProton==2");
  savedraw(iplot++, t, c1, "nBkg==0", "nPiZero==0 && nPion==1 && nProton==2");
  savedraw(iplot++, t, c1, "nBkg==0 && nPiZero==0 && nPion==1 && nProton==2");//selection rate 2.6%
  savedraw(iplot++, t, c1, sig, "nBkg==0 && nPiZero==0 && nPion==1 && nProton==2");//purity 2.75%

  //https://www.wolframalpha.com/input/?i=sulphur-38+mass+in+gev //35.3699147
  savedraw(iplot++, t, c1, "event_recoilM",sig+" && nBkg==0 && nPiZero==0 && nPion==1 && nProton==2");
  savedraw(iplot++, t, c1, "event_recoilM","nBkg==0 && nPiZero==0 && nPion==1 && nProton==2 && abs(event_recoilM-35.370)<0.006");
  savedraw(iplot++, t, c1, sig,"nBkg==0 && nPiZero==0 && nPion==1 && nProton==2 && abs(event_recoilM-35.370)<0.006");//purity 1

  savedraw(iplot++, t, c1, "recoilM",sig+" && nBkg==0 && nPiZero==0 && nPion==1 && nProton==2");
  savedraw(iplot++, t, c1, "recoilM","nBkg==0 && nPiZero==0 && nPion==1 && nProton==2 && abs(event_recoilM-35.370)<0.011");
  savedraw(iplot++, t, c1, sig,"nBkg==0 && nPiZero==0 && nPion==1 && nProton==2 && abs(event_recoilM-35.370)<0.011");//purity 1

  savedraw(iplot++, t, c1, "recoilP","nBkg==0 && nPiZero==0 && nPion==1 && nProton==2 && abs(event_recoilM-35.370)<0.011");
  savedraw(iplot++, t, c1, "event_recoilP","nBkg==0 && nPiZero==0 && nPion==1 && nProton==2 && abs(event_recoilM-35.370)<0.011","same e");
}

void draw()
{
  const TString sin = "outplot/outAna11_GEANT4CHARGEDBEAM_Ar_10000xstat_HASthreshold_Allisgood_AllsaveGEANT4CHARGEDBEAM_Filelist_GEANT4_Pion_v010000xstat.root";

  TFile * fin = new TFile(sin);
  TTree * tree = (TTree*)fin->Get("tree");
  subdrawPiZero_OneP(tree);
  subdrawPion_OneP(tree);
  subdrawPiZero_TwoP(tree);
  subdrawPion_TwoP(tree);
}
