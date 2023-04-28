#include "fileHelper.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TStyle.h"
#include <iostream>
#include "string.h"
#include "TH1F.h"
#include "THStack.h"
#include "TLatex.h"
#include "TLegend.h"

using namespace std;

void SetStyle();
void PlotStack(TString path, TString varUnit, THStack * mc, TH1F * data, TLegend * legend);

int main() {
  ////////////////////////////////////////////////////////////////////////////////
  // This file can guide you through the process of combining monte carlo and data
  ///////////////////////////////////////////////////////////////////////////////

  // Let's say you want to have all the relevant plots in one file, this is how you create it
  TFile * analysis = new TFile("analysis.root", "RECREATE");

  // now you have to get the histograms you want to use. If you have saved them in another file beforehand, you can get them this way:
  TFile * file_histogram1 = new TFile("name_of_file_where_you_saved_histogram1.root");
  TH1F * h_histogram1 = (TH1F*)file_histogram1->Get("name_of_variable");

  //If you want to scale the histogram, use Scale(float factor)
  //If you want to adjust the bin width, use Rebin(int number_of_bins_to_be_merged)

  //You should set a different fill color for each process using SetFillColor(Color_t fcolor); examples for fcolor are kRed, kGreen, kYellow etc. 
  //  You can also add integer like kRed+2 to change the shade

  //You might also want to change the line color using e.g. SetLineColor(kBlack)
  
  //You should add a legend to be able to distinguish the different processes
  TLegend *leg = new TLegend(0.7,0.6,0.85,0.9);
  //leg->SetFillColor(0);
  //leg->SetBorderSize(0);
  //leg->SetTextSize(0.035);
  //leg->AddEntry(h_histogram1,"Name of the process", "f");

  //To plot several MC histograms, use THStack. At this point you should be able to figure out its use by looking it up online. 
  //For further analysis, you should however combine them to a new TH1F
    
  //For histograms of data, you can use the following commands to change the attributes of the histobramm
  //h_data->SetLineWidth(0);
  //h_data->SetLineColor(kBlack);
  //h_data->SetMarkerStyle(7);
  //h_data->SetMarkerSize(1);
  //h_data->SetMarkerColor(kBlack);

  //For plotting data and stacked MC, you can use the function PlotStack at the end of this file 

  analysis->cd();
  //For all objects you want to write to the analysis file, call Write(), e.gl
  h_histogram1->Write();

  //End the programm properly by deleting all dynamic instances
  file_histogram1->Close();
  delete file_histogram1;
  delete leg;
  analysis->Close();



  return 0;
}

void SetStyle() {
  gStyle->SetFrameBorderMode(0);
  gStyle->SetFrameFillColor(0);
  gStyle->SetCanvasBorderMode(0);
  gStyle->SetCanvasColor(0);
  gStyle->SetPadBorderMode(0);
  gStyle->SetPadColor(0);
  gStyle->SetStatColor(0);
  gStyle->SetPadTopMargin(0.05);
  gStyle->SetPadRightMargin(0.05);
  gStyle->SetPadBottomMargin(0.15);
  gStyle->SetPadLeftMargin(0.15);
  gStyle->SetTitleXOffset(1.3);
  gStyle->SetTitleYOffset(1.3);
  gStyle->SetTextSize(0.05);
  gStyle->SetLabelSize(0.05,"x");
  gStyle->SetTitleSize(0.05,"x");
  gStyle->SetLabelSize(0.05,"y");
  gStyle->SetTitleSize(0.05,"y");
  gStyle->SetLabelSize(0.05,"z");
  gStyle->SetTitleSize(0.05,"z");
  gStyle->SetHistLineWidth(1);
  gStyle->SetOptTitle(0);
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(0);
}


void PlotStack(TString filename, TString varUnit, THStack* mc, TH1F * data, TLegend * legend) {
  SetStyle();
	TCanvas * canv = new TCanvas("canv","Canvas for histogram",1);
  canv->SetLeftMargin(.12);
  canv->SetRightMargin(.1);
  data->Draw("E1");
  data->SetMinimum(0);
  mc->Draw("hist SAME");
  data->Draw("E1 SAME");
  mc->SetTitle(";"+varUnit+";Events");
  mc->GetYaxis()->SetTitleOffset(1);
  data->Draw("E1 SAME");
  legend->Draw();
  canv->Print(filename);
  delete canv;
}






