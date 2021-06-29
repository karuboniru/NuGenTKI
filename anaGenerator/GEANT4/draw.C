void savedraw(const int iplot, TTree *t, TCanvas *c1, const TString var, const TString cut="1")
{
  gPad->SetLogy();
  t->Draw(var, cut);
  c1->Print(Form("outplot/outdraw/%splot%d.png",c1->GetName(), iplot));
}

void subdrawPiZero_OneP(TTree * t)
{
  TCanvas * c1 = new TCanvas("PiZero_OneP","",600,400);
  int iplot=0;
  savedraw(iplot++, t, c1, "nProton==1");
  savedraw(iplot++, t, c1, "nPiZero==1", "nProton==1");
  savedraw(iplot++, t, c1, "nPiZero==1 && nProton==1");
  savedraw(iplot++, t, c1, "nPion==0", "nPiZero==1 && nProton==1");
  savedraw(iplot++, t, c1, "nPion==0 && nPiZero==1 && nProton==1");
  savedraw(iplot++, t, c1, "nBkg==0", "nPion==0 && nPiZero==1 && nProton==1");
  savedraw(iplot++, t, c1, "nBkg==0 && nPion==0 && nPiZero==1 && nProton==1");//selection rate 3.6%
  savedraw(iplot++, t, c1, "(totparcount-1000000*nGamma)==100001", "nBkg==0 && nPion==0 && nPiZero==1 && nProton==1");//purity 28%
  savedraw(iplot++, t, c1, "event_recoilM","(totparcount-1000000*nGamma)==100001 && nBkg==0 && nPion==0 && nPiZero==1 && nProton==1");
}


void subdrawPion_OneP(TTree * t)
{
  TCanvas * c1 = new TCanvas("Pion_OneP","",600,400);
  int iplot=0;
  savedraw(iplot++, t, c1, "nProton==1");
  savedraw(iplot++, t, c1, "nPion==1", "nProton==1");
  savedraw(iplot++, t, c1, "nPion==1 && nProton==1");
  savedraw(iplot++, t, c1, "nPiZero==0", "nPion==1 && nProton==1");
  savedraw(iplot++, t, c1, "nPiZero==0 && nPion==1 && nProton==1");
  savedraw(iplot++, t, c1, "nBkg==0", "nPiZero==0 && nPion==1 && nProton==1");
  savedraw(iplot++, t, c1, "nBkg==0 && nPiZero==0 && nPion==1 && nProton==1");//selection rate 8.3%
  savedraw(iplot++, t, c1, "(totparcount-1000000*nGamma)==11", "nBkg==0 && nPiZero==0 && nPion==1 && nProton==1");//purity 30%
  savedraw(iplot++, t, c1, "event_recoilM","(totparcount-1000000*nGamma)==11 && nBkg==0 && nPiZero==0 && nPion==1 && nProton==1");
}


void subdrawPiZero_TwoP(TTree * t)
{
  TCanvas * c1 = new TCanvas("PiZero_TwoP","",600,400);
  int iplot=0;
  savedraw(iplot++, t, c1, "nProton==2");
  savedraw(iplot++, t, c1, "nPiZero==1", "nProton==2");
  savedraw(iplot++, t, c1, "nPiZero==1 && nProton==2");
  savedraw(iplot++, t, c1, "nPion==0", "nPiZero==1 && nProton==2");
  savedraw(iplot++, t, c1, "nPion==0 && nPiZero==1 && nProton==2");
  savedraw(iplot++, t, c1, "nBkg==0", "nPion==0 && nPiZero==1 && nProton==2");
  savedraw(iplot++, t, c1, "nBkg==0 && nPion==0 && nPiZero==1 && nProton==2");//selection rate 1.9%
  savedraw(iplot++, t, c1, "(totparcount-1000000*nGamma)==100002", "nBkg==0 && nPion==0 && nPiZero==1 && nProton==2");//purity 0.94%
  savedraw(iplot++, t, c1, "event_recoilM","(totparcount-1000000*nGamma)==100002 && nBkg==0 && nPion==0 && nPiZero==1 && nProton==2");
}


void subdrawPion_TwoP(TTree * t)
{
  TCanvas * c1 = new TCanvas("Pion_TwoP","",600,400);
  int iplot=0;
  savedraw(iplot++, t, c1, "nProton==2");
  savedraw(iplot++, t, c1, "nPion==1", "nProton==2");
  savedraw(iplot++, t, c1, "nPion==1 && nProton==2");
  savedraw(iplot++, t, c1, "nPiZero==0", "nPion==1 && nProton==2");
  savedraw(iplot++, t, c1, "nPiZero==0 && nPion==1 && nProton==2");
  savedraw(iplot++, t, c1, "nBkg==0", "nPiZero==0 && nPion==1 && nProton==2");
  savedraw(iplot++, t, c1, "nBkg==0 && nPiZero==0 && nPion==1 && nProton==2");//selection rate 2.6%
  savedraw(iplot++, t, c1, "(totparcount-1000000*nGamma)==12", "nBkg==0 && nPiZero==0 && nPion==1 && nProton==2");//purity 2.75%
  savedraw(iplot++, t, c1, "event_recoilM","(totparcount-1000000*nGamma)==12 && nBkg==0 && nPiZero==0 && nPion==1 && nProton==2");
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
