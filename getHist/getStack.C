void getStack(const TString fin, const TString anaid, const TString tag, const TString var)
{
  const TString dir= ("outStack/"+anaid);
  gSystem->Exec("mkdir -p "+dir);

  cout<<" fin "<<fin<<" anaid "<<anaid<<" tag "<<tag<<" var "<<var<<endl;
  TFile::Open(fin);

  TList *lout=new TList;

  if(var.Contains("VS")){
    TH2D * hall=(TH2D*) gDirectory->Get(var+"_all"); 
    TH2D * hqe=(TH2D*) gDirectory->Get(var+"_qe"); 
    TH2D * hres=(TH2D*) gDirectory->Get(var+"_res");
    TH2D * hdis=(TH2D*) gDirectory->Get(var+"_dis");
    TH2D * h2p2h=(TH2D*) gDirectory->Get(var+"_2p2h");
    TH2D * hother=(TH2D*) gDirectory->Get(var+"_other");

    TH2D *hh[]={hall, hqe, hres, hdis, h2p2h}; 
    const int nh=5;
    for(int ii=0; ii<nh; ii++){
      hh[ii]->Scale(1./13.); printf("Scaling to per nucleon in CH,  C and H should have been full nucleus and combined !!\n");
      lout->Add(hh[ii]);
    }
    TCanvas *cc=new TCanvas;
    hall->Draw("colz");
    cc->Print(Form("outStack/%s/%s_%s.eps", anaid.Data(), var.Data(), tag.Data()));
    cc->Print(Form("outStack/%s/%s_%s.png", anaid.Data(), var.Data(), tag.Data()));
  }
  else{
    TH1D * hall=(TH1D*) gDirectory->Get(var+"_all");   
    TH1D * hqe=(TH1D*) gDirectory->Get(var+"_qe");     
    TH1D * hres=(TH1D*) gDirectory->Get(var+"_res");   
    TH1D * hdis=(TH1D*) gDirectory->Get(var+"_dis");   
    TH1D * h2p2h=(TH1D*) gDirectory->Get(var+"_2p2h"); 
    TH1D * hother=(TH1D*) gDirectory->Get(var+"_other"); 
    if(hother){
      if(hother->GetEntries()){
        printf("\n\n\n\n\n%s has entry! Is it COH?? \n\n\n\n\n\n", hother->GetName()); exit(1);
      }
      else{
        cout<<hother->GetName()<<" good "<<hother->GetEntries()<<endl;
      }
    }

    TH1D * hqe_0pi=(TH1D*) gDirectory->Get(var+"_qe_0pi");
    TH1D * hqe_1pi=(TH1D*) gDirectory->Get(var+"_qe_1pi");
    TH1D * hqe_Mpi=(TH1D*) gDirectory->Get(var+"_qe_Mpi");
    TH1D * hres_0pi=(TH1D*) gDirectory->Get(var+"_res_0pi");
    TH1D * hres_1pi=(TH1D*) gDirectory->Get(var+"_res_1pi");
    TH1D * hres_Mpi=(TH1D*) gDirectory->Get(var+"_res_Mpi");
    TH1D * hdis_0pi=(TH1D*) gDirectory->Get(var+"_dis_0pi");
    TH1D * hdis_1pi=(TH1D*) gDirectory->Get(var+"_dis_1pi");
    TH1D * hdis_Mpi=(TH1D*) gDirectory->Get(var+"_dis_Mpi");
    TH1D * h2p2h_0pi=(TH1D*) gDirectory->Get(var+"_2p2h_0pi");
    TH1D * h2p2h_1pi=(TH1D*) gDirectory->Get(var+"_2p2h_1pi");
    TH1D * h2p2h_Mpi=(TH1D*) gDirectory->Get(var+"_2p2h_Mpi");

    /*
    TH1D *hh[]={hall, hqe, hres, hdis, h2p2h};
    TString tit[]={"all", "QE","RES", "DIS", "2p2h"};
    const Int_t cols[]={kBlack, kRed-3,  kGray, kMagenta, kOrange};
    */

    TH1D *hh[]={   hall,  hqe_0pi,  hqe_1pi,  hqe_Mpi,  hres_0pi,  hres_1pi,  hres_Mpi,  hdis_0pi,  hdis_1pi,  hdis_Mpi,  h2p2h_0pi,  h2p2h_1pi,  h2p2h_Mpi};
    TString tit[]={"all", "QE", "QE 1#pi^{0}", "QE multi-#pi^{0}", "RES", "RES 1#pi^{0}", "RES multi-#pi^{0}", "DIS", "DIS 1#pi^{0}", "DIS multi-#pi^{0}", "2p2h", "2p2h 1#pi^{0}", "2p2h multi-#pi^{0}"};
    const Int_t cols[]={kBlack, kRed-3,  kGray, kMagenta, kOrange, kBlue, kGreen+3, kRed-3,  kGray, kMagenta, kOrange, kBlue, kGreen+3};

    THStack * stk = new THStack(var,tag); lout->Add(stk);
 
    TLegend *lg=new TLegend(0.7, 0.5, 0.9,0.9); lg->SetName("lg"); lout->Add(lg);
    lg->SetFillStyle(0);
    
    const int nh = sizeof(hh)/sizeof(TH1D*);
    for(int ii=0; ii<nh; ii++){
      
      //scale for CH target!
      //hh[ii]->Scale(1./12.); printf("Scaling to per nucleon in C,  C should have been full nucleus and combined !!\n");
      if (fin.Contains("NuWro")) // For NuWro, xsec given is already per nucleon
      {
        hh[ii]->Scale(12./13.); printf("Scaling to per nucleon in CH,  C and H should have been per nucleon and combined !!\n");
      }else{
        hh[ii]->Scale(1./13.); printf("Scaling to per nucleon in CH,  C and H should have been full nucleus and combined !!\n");
      }
      lout->Add(hh[ii]);
      //hh[ii]->SetLineWidth(0.5);
      hh[ii]->SetLineStyle(kSolid);
      hh[ii]->SetLineColor(cols[ii]);
      hh[ii]->SetFillColor(cols[ii]);
      hh[ii]->SetFillStyle(1001);
      hh[ii]->SetTitle(Form("%s %s",tit[ii].Data(), hh[ii]->GetTitle()));
      hh[ii]->SetName(Form("%s%s",var.Data(), tit[ii].Data()));
      if(ii){
        stk->Add(hh[ii]);
      }
      lg->AddEntry(hh[ii], tit[ii], "fl");
    }

    TCanvas *cc=new TCanvas;
    //stk->Draw("hist C");
    stk->Draw("hist");
    stk->GetXaxis()->SetTitle(var);
    stk->SetTitle(hall->GetTitle());
    gStyle->SetTitleX(0.5);
    //stk->Draw("hist C");
    stk->Draw("hist");
    hall->SetLineWidth(3);
    hall->SetLineColor(kBlack);
    hall->SetFillStyle(0);
    hall->SetMinimum(0);
    hall->Draw("same");
    printf("total integrated xsec %s %s %e\n", anaid.Data(), var.Data(), hall->Integral(0,10000,"width"));
    lg->Draw();
    cc->Print(Form("outStack/%s/%s_%s.eps", anaid.Data(), var.Data(), tag.Data()));
    cc->Print(Form("outStack/%s/%s_%s.png", anaid.Data(), var.Data(), tag.Data()));
  }

  TFile *fout=new TFile(Form("%s/%s_%s.root", dir.Data(), var.Data(), tag.Data()),"recreate");
  TDirectory *ld=gDirectory->mkdir(tag);
  ld->cd();
  lout->Write();
  fout->Save();
  fout->Close();    
}
