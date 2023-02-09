// gminus2_status
//
// This macro generats a summary plot of muon g-2 experimental data,
// the standard model prediction(s), and the projected sensitivy of
// the J-PARC muon g-2/EDM experiment.

TGraphErrors* graphData( Double_t, Double_t, Double_t, Int_t, Int_t, Int_t);

void status(){

  gStyle->SetOptStat(0);
  gStyle->SetTickLength(0.00, "Y");
  gStyle->SetLabelOffset(100, "Y");
  gStyle->SetLabelSize(0.045);
  gStyle->SetTitleFont(12, "X");  
  gStyle->SetTitleXOffset( 1.0 );
  gStyle->SetTitleXSize( 0.065 );
  gStyle->SetTitleAlign(12);
  gStyle->SetEndErrorSize( 5 );
  gStyle->SetTextSize( 0.04 );
  

  // The white paper 2020, Physics Reports 887, 1-166 (2020).
  Double_t  amuSM  = 116591810e-11;
  Double_t eamuSM  = 43e-11;

  // BNL E821 final report, Bennett G.W., et al., Phys. Rev. D, 73,072003 (2006).
  Double_t  amuBNL = 116592089e-11; 
  Double_t eamuBNL = 63e-11;

  // FNAL Run 1, B. Abi et al. Phys. Rev. Lett. 126, 141801 (2021).  
  Double_t  amuFNAL= 116592040e-11;
  Double_t eamuFNAL= 54e-11;

  // Experimental average, B. Abi et al. Phys. Rev. Lett. 126, 141801 (2021).    
  Double_t  amuAVE = 116592061e-11;
  Double_t eamuAVE = 41e-11;

  // J-PARC TDR, PTEP 2019, 5, 053C02 (2019).
  Double_t  amuJP  = amuAVE;
  Double_t eamuJP  = amuJP * 450e-9;
  
  Double_t  amuOff = 116590000e-11; // a common offset used in FNAL Run 1 paper.

  const Int_t N = 5;
  TGraphErrors* g[ N ];
  Double_t scale   = 1e+9;
  g[ 0 ] = graphData( (amuSM  - amuOff) * scale, eamuSM   * scale, 70., 20, kGreen +3, kSolid );
  g[ 1 ] = graphData( (amuBNL - amuOff) * scale, eamuBNL  * scale, 80., 21, kBlue  +1, kSolid );
  g[ 2 ] = graphData( (amuFNAL- amuOff) * scale, eamuFNAL * scale, 60., 21, kBlue    , kSolid);
  g[ 3 ] = graphData( (amuAVE - amuOff) * scale, eamuAVE  * scale, 20., 21, kCyan+2  , kSolid );
  g[ 4 ] = graphData( (amuJP  - amuOff) * scale, eamuJP   * scale, 20., 20, kRed     , kDotted);

  TBox * boxSM = new TBox( (amuSM  - amuOff - eamuSM) * scale, 4., (amuSM  - amuOff + eamuSM) * scale, 99.);
  boxSM->SetFillColor( kGreen -10 );

  TBox * boxAVE = new TBox( (amuAVE - amuOff - eamuAVE) * scale, 4., (amuAVE - amuOff + eamuAVE) * scale, 99.);
  boxAVE->SetFillColor( kCyan -10 );
  
  TCanvas * c = new TCanvas("","", 800, 450 );
  gPad->SetBottomMargin(0.15);
    
  TH2D* hframe = new TH2D("hframe","",100, 17, 22, 100, 0., 100.);
  hframe->SetXTitle("a_{#mu} #times 10^{9} - 1165900");
 
  hframe->Draw();
  boxSM->Draw();
  boxAVE->Draw();
  g[ 0 ]->Draw("ps");
  g[ 1 ]->Draw("ps");
  g[ 2 ]->Draw("ps");
  g[ 4 ]->Draw("ps");

  TLatex * latex = new TLatex();
  latex->SetTextAlign(12);
  latex->DrawLatex(17.1, 70., "#splitline{#splitline{White}{Paper}}{(2020)}");
  latex->DrawLatex(21.6, 80., "#splitline{BNL}{(2006)}");
  latex->DrawLatex(21.3, 60., "#splitline{FNAL}{(2021)}");  
  latex->SetTextColor( kRed );
  latex->DrawLatex(21.3, 20., "#splitline{J-PARC}{(projection)}");  
  latex->SetTextColor( kCyan -6);
  latex->DrawLatex(20.3, 92., "Experiment");
  latex->SetTextColor( kGreen -6);
  latex->DrawLatex(17.85, 92., "#splitline{Standard}{Model}");
  
  c->Print("gminus2_status.pdf");
  
}

TGraphErrors* graphData( Double_t x, Double_t ex, Double_t y, Int_t imarker, Int_t icolor, Int_t lstyle){

  TGraphErrors * g = new TGraphErrors( 1 );
  
  g->SetPoint( 0,  x, y );
  g->SetPointError( 0, ex, 0.);
  g->SetMarkerSize( 1.7 );
  g->SetMarkerStyle( imarker );
  g->SetMarkerColor( icolor );
  g->SetLineWidth( 2 );
  g->SetLineColor( icolor );
  g->SetLineStyle( lstyle );
  
  return (g);
}
