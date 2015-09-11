//C++ INCLUDES
#include <iostream>
//ROOT INCLUDES
#include <TFile.h>
#include <TTree.h>
#include <TH1F.h>
#include <TF1.h>
#include <TCanvas.h>
#include <TDirectory.h>
#include <TLatex.h>
#include <TStyle.h>

//Axis
const float axisTitleSize = 0.06;
const float axisTitleOffset = .8;

const float axisTitleSizeRatioX   = 0.18;
const float axisLabelSizeRatioX   = 0.12;
const float axisTitleOffsetRatioX = 0.84;

const float axisTitleSizeRatioY   = 0.15;
const float axisLabelSizeRatioY   = 0.108;
const float axisTitleOffsetRatioY = 0.52;

//Margins
const float leftMargin   = 0.12;
const float rightMargin  = 0.05;
const float topMargin    = 0.07;
const float bottomMargin = 0.12;

void FitSpatialResolution( TString inputFile = "" , TString coord = "X", int nbins = 100 )
{
  if ( inputFile ==  "" )
    {
      std::cerr << "[ERROR]: Please provide a valid ROOT input file" << std::endl;
      return;
    }

  TFile* _f = new TFile( inputFile, "READ" );
  TTree* T = (TTree*)_f->Get("t");
  //-------------------------------------
  //d e f i n e   f i t   f u n c t i o n
  //-------------------------------------
  TF1* f = new TF1( "f", "[0]*(TMath::Erf((2*(x-[1])+[2])/([3]*TMath::Sqrt(8))) + TMath::Erf((2*([1]-x)+[2])/([3]*TMath::Sqrt(8))))", -1.8, 1.8 );
  //TF1* f = new TF1( "f", "[0]*(TMath::Erf((2*(x-[1])+[2])/([3]*TMath::Sqrt(8))) + TMath::Erf((2*([1]-x)+[2])/([3]*TMath::Sqrt(8)))) + [4]*exp(-0.5*((x-[1])/[5])**2)", -3., 3. );
  double norm = 2000;
  double a = 1.7;//[1]
  double mu = 0.0;//[2]
  double sigma = 0.5;//[3]
  
  f->SetParameter(0, norm);
  f->SetParameter(1, mu);
  f->SetParameter(2, a);
  //f->FixParameter(2, a);
  f->SetParameter(3, sigma);
  //f->SetParameter(4, 0.1*norm);
  //f->SetParameter(5, 4*sigma);
  
  //f->SetLineColor(kBlue);
  //---------------------------------------------------
  //S e l e c t i n g   c o o r d i n a t e  t o  f i t
  //---------------------------------------------------
  TString var;
  double xlow  = -5.;
  double xhigh = 5.;
  TString axisTitle, outName;
  if ( coord == "X" )
    {
      var = Form("coords[0]>>histo(%d,%f,%f)", nbins, xlow, xhigh);
      a = 1.7;
      axisTitle = "X [mm]";
      outName = "X_Resolution";
    }
  else if ( coord == "Y" )
    {
      var = Form("coords[1]>>histo(%d,%f,%f)", nbins, xlow, xhigh);
      a = 2.0;
      axisTitle = "Y [mm]";
      outName = "Y_Resolution";
    }
  else
    {
      std::cerr << "[ERROR]: coordinate not recognized, provide X or Y" << std::endl;
      return;
    }
  
  //--------------------------------------
  //F i x i n g  t r i g g e r   w i d t h
  //--------------------------------------
  f->FixParameter(2, a);
  //f->SetParLimits(2, 0.0, 300*a);
  //f->SetParLimits(5, 1.0, 400000);
  T->Draw(var, "", "goff");
  TH1F* histo = (TH1F*)gDirectory->Get("histo");
  histo->Fit(f,"RV");

  TCanvas* c = new TCanvas( "c", "c", 2119, 33, 800, 700 );
  c->SetHighLightColor(2);
  c->SetFillColor(0);
  c->SetBorderMode(0);
  c->SetBorderSize(2);
  c->SetLeftMargin( leftMargin );
  c->SetRightMargin( rightMargin );
  c->SetTopMargin( topMargin );
  c->SetBottomMargin( bottomMargin );
  c->SetFrameBorderMode(0);
  c->SetFrameBorderMode(0);

  gStyle->SetPaintTextFormat("4.3f");

  histo->SetLineWidth(2);
  histo->SetStats(0);
  histo->SetTitle("");
  histo->GetXaxis()->SetTitleSize( axisTitleSize );
  histo->GetXaxis()->SetTitleOffset( axisTitleOffset );
  histo->GetYaxis()->SetTitleSize( axisTitleSize );
  histo->GetYaxis()->SetTitleOffset( axisTitleOffset + 0.15);
  histo->SetTitle("");
  histo->SetXTitle(axisTitle);
  histo->SetYTitle( "entries / 0.25 mm" );
  
  histo->SetStats(0);
  histo->Draw();
  std::cout << "sigma: " << f->GetParameter(3) << std::endl;

  //------
  //Latex
  //------
  TString sigmaVal = TString( Form( "#sigma = %.2f #pm %.2f [mm]", f->GetParameter(3), f->GetParError(3) ) );
  TLatex latex;
  latex.SetNDC();
  latex.SetTextAngle(0);
  latex.SetTextColor(kBlack);    
  latex.SetTextFont(42);
  latex.SetTextAlign(31); 
  latex.SetTextSize(0.04);    
  latex.DrawLatex(0.93, 0.88, sigmaVal );
  c->SaveAs( outName+".pdf" );
  c->SaveAs( outName+".C" );
};
