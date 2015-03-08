void plotClosureTest(const TString &fileName = "HadTau_Closure.root",
		     const TString &graphicsFormat = "png") {
  gROOT->ProcessLine(".L HistStyle.h+");
  gROOT->ProcessLine(".L HistReader.h+");
  HistStyle::init();
  const int canSize = 600;

  // Get histograms from file
  const unsigned int kNDists = 3;
  TH1* hTrue[kNDists];
  TH1* hPred[kNDists];
  for(unsigned int i = 0; i < kNDists; ++i) {
    TString name = "";
    if(      i == 0 ) name = "Ht";
    else if( i == 1 ) name = "met";
    else if( i == 2 ) name = "NJets";
    hPred[i] = HistReader::get(fileName,"hPred"+name);
    hTrue[i] = HistReader::get(fileName,"hTrue"+name);
  }
  
  // Set style
  for(unsigned int i = 0; i < kNDists; ++i) {
    hPred[i]->SetMarkerStyle(20);
    hPred[i]->SetMarkerColor(kRed);
    hPred[i]->SetLineColor(hPred[i]->GetMarkerColor());
    hPred[i]->SetMarkerSize(0.9);
    hTrue[i]->SetLineColor(kBlue);
    hTrue[i]->GetYaxis()->SetTitleOffset(1);
   }

  // Ratio plots
  TH1* hRatioFrame[kNDists];
  TH1* hRatio[kNDists];
  for(unsigned int i = 0; i < kNDists; ++i) {
    hRatio[i] = static_cast<TH1*>(hPred[i]->Clone("Ratio"));
    hRatio[i]->Divide(hTrue[i]);
    hRatio[i]->GetYaxis()->SetTitle("N(predicted) / N(expected)");
    hRatio[i]->GetYaxis()->SetRangeUser(0,2);
    hRatio[i]->SetTitle("");
    hRatio[i]->SetStats(0);
    hRatio[i]->GetYaxis()->SetTitleSize(0.08);
    hRatio[i]->GetYaxis()->SetTitleOffset(0.5);
    hRatio[i]->GetYaxis()->SetLabelSize(0.07);

    hRatioFrame[i] = static_cast<TH1*>(hRatio[i]->Clone("RatioFrame"));
    for(int bin = 1; bin <= hRatioFrame[i]->GetNbinsX(); ++bin) {
      hRatioFrame[i]->SetBinContent(bin,1.);
    }
    hRatioFrame[i]->SetLineStyle(2);
    hRatioFrame[i]->SetLineWidth(2);
    hRatioFrame[i]->SetLineColor(hTrue[i]->GetLineColor());
  }

  // Create legend
  TLegend* leg = new TLegend(0.4,0.75,0.9,0.89);
  leg->SetBorderSize(0);
  leg->SetFillColor(0);
  leg->SetFillStyle(0);
  leg->SetTextFont(42);
  leg->SetTextSize(0.05);
  leg->AddEntry(hTrue[0],"MC Expectation","L");
  leg->AddEntry(hPred[0],"MC Prediction.","P");


  // Draw
  for(unsigned int i = 0; i < kNDists; ++i) {
    TString name = "";
    if(      i == 0 ) name = "HT";
    else if( i == 1 ) name = "MET";
    else if( i == 2 ) name = "NJets";

    TCanvas* can = new TCanvas(name,name,canSize,canSize);
    TPad *pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1.0);
    pad1->SetBottomMargin(0); // Upper and lower plot are joined
    pad1->Draw();             // Draw the upper pad: pad1
    pad1->cd();               // pad1 becomes the current pad
    
    hTrue[i]->Draw("HIST");
    hPred[i]->Draw("PE1same");
    leg->Draw("same");
    name = "hClosure"+name;
    gPad->SetLogy();
    
    can->cd();
    TPad *pad2 = new TPad("pad2", "pad2", 0, 0.01, 1, 0.3);
    pad2->SetTopMargin(0);
    pad2->SetBottomMargin(0.2);
    pad2->Draw();
    pad2->cd();       // pad2 becomes the current pad
    hRatioFrame[i]->Draw("HIST");
    hRatio[i]->SetMarkerSize(0.6);
    hRatio[i]->Draw("PE1same");

   can->SaveAs(name+"."+graphicsFormat);
   can->SaveAs(name+".pdf");
  }

cout<<hTrue[2]->GetEntries()<<" "<<hPred[2]->GetEntries()<<endl;
}
