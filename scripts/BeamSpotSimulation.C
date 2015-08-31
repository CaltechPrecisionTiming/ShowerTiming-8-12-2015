#ifdef __MAKECINT__
#pragma link C++ class vector<vector<float> >+;
#endif

#include <iostream>
#include <fstream> 
#include <sstream>
#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TMath.h"
#include "TF1.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TVirtualFitter.h"
#include "TPaveText.h"
#include "TProfile.h"
#include "TLegend.h"
#include "TStyle.h"
#include "TColor.h"
#include "TGraphErrors.h"
#include "TLatex.h"



void SimulateBeamSpot( double resolution , int axis = 0) {

  TRandom3 *random = new TRandom3;  
  int NTrials = 50000;

  double beamspotSize = 2.0;
  if (axis == 0) beamspotSize = 1.7;

  TH1F *hist = new TH1F( "xpos", "; x [mm]; Number of Events", 100, -5, 5);

  for (int i=0 ; i< NTrials; ++i) {    
    double beamPosition = (random->Rndm() - 0.5)*beamspotSize; 
    double measuredPosition = beamPosition + random->Gaus(0, resolution);
    //cout << "trial " << i << " : " << measuredPosition << "\n";
    hist->Fill( measuredPosition );    
  }

  TCanvas *cv = new TCanvas("cv","cv",800,600);

  hist->Draw();

  TF1* myRes = new TF1("res","gaus");
  hist->Fit("res","gaus");
  
  cout << "Mean: " << myRes->GetParameter(1) << " +/- " << myRes->GetParError(1) << "\n";
  cout << "Sigma: " << myRes->GetParameter(2) << " +/- " << myRes->GetParError(2) << "\n";


  cv->SaveAs( "SimulateBeamSpot.gif" );

}

void BeamSpotSimulation() {

  //  SimulateBeamSpot(0.6, 0);
  SimulateBeamSpot(1.05, 1);

}
