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
#include <cmath>
#include <fstream>

using namespace std;

void SetStyle();
void PlotStack(TString path, TString varUnit, THStack * mc, TH1F * data, TLegend * legend);

vector<float> LoadScalingFactors(const string& filename) {
    vector<float> scalingFactors;
    ifstream file(filename);
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            float factor = stof(line);
            scalingFactors.push_back(factor);
        }
        file.close();
    } else {
        cout << "Fehler beim Öffnen der Datei: " << filename << endl;
    }
    return scalingFactors;
}

struct Simulation_info {
    double xsec;
    int events;
};


int main(int argc, char* argv[]) {
  ////////////////////////////////////////////////////////////////////////////////
  // This file can guide you through the process of combining monte carlo and data
  ///////////////////////////////////////////////////////////////////////////////

  string inv_mass = argv[1];

  // Let's say you want to have all the relevant plots in one file, this is how you create it
  TFile * analysis = new TFile("analysis_inv_mass.root", "RECREATE");

  // Deklarationen für for Schleife über zprime
  vector<string> zprime_masses= {
    "400",
    "500",
    "750",
    "1000",
    "1250",
    "1500",
    "1750",
    "2000",
    "2250",
    "2500",
    "3000",
  };

  // Werte für Berechnungen der Skalierungsfaktoren
  int L = 1000;
  vector<Simulation_info> simulation_info = {
      {29.41, 922521},    //diboson
      {52.47, 1468942},   //singletop
      {252.82, 7847944},  //ttbar
      {36214, 66536222},  //wjet
      {2516.2, 37422926}, //zjet
      {110.0, 100000},    //zprime400
      {82.0, 100000},     //zprime500
      {20.0, 100000},     //zprime750
      { 5.5, 100000},     //zprime1000
      { 1.9, 100000},     //zprime1250
      { 0.83, 100000},    //zprime1500
      { 0.3, 100000},     //zprime1750  
      { 0, 100000},       //zprime2000
      { 0.067, 100000},   //zprime2250
      { 0.035, 100000},   //zprime2500
      { 0.012, 100000}    //zprime3000
  };
  vector<double> w;
  for (const auto& info : simulation_info) {
    double weight = L * info.xsec / info.events;
    w.push_back(weight);
  }
  
  
  vector<string> leptons = {"el", "mu"};
  unsigned int idx1 = 0;

  for (const auto& zprime_mass : zprime_masses){

    //Skalierungsfaktoren
    float scaleFactor_diboson    = w[0]; 
    float scaleFactor_singletop  = w[1];
    float scaleFactor_ttbar      = w[2];
    float scaleFactor_wjets      = w[3];
    float scaleFactor_zjets      = w[4];
    float scaleFactor_zprime     = w[5+idx1];
    cout << "diboson  : " << scaleFactor_diboson   << endl;
    cout << "singletop: " << scaleFactor_singletop << endl;
    cout << "ttbar    : " << scaleFactor_ttbar     << endl;
    cout << "wjets    : " << scaleFactor_wjets     << endl;
    cout << "zjets    : " << scaleFactor_zjets     << endl;
    cout << "zprime   : " << scaleFactor_zprime    << endl;    
    idx1++;

    for (const auto& lepton : leptons) {   // for Schleife über el und mu

      // now you have to get the histograms you want to use. If you have saved them in another file beforehand, you can get them this way:
      TFile * file_histogram_data = new TFile(("/net/e4-nfs-home.e4.physik.tu-dortmund.de/home/photon/Breitfeld_Knospe_26-04-23/plots/data." + lepton + "_selected_plots.root").c_str());
      TH1F * h_data = (TH1F*)file_histogram_data->Get(inv_mass.c_str());
      TDirectoryFile *dir_data = (TDirectoryFile*)file_histogram_data->Get("Histograms");
      if (dir_data) {
        h_data = (TH1F*)dir_data->Get(inv_mass.c_str());
        if (h_data) {
          // alles erfolgreich geladen
        } else {
          cout << "Histogramm 'inv_mass' wurde nicht im Verzeichnis 'Histograms' gefunden!" << endl;
        }
      } else {
        cout << "Verzeichnis 'Histograms' wurde nicht im ROOT-File gefunden!" << endl;
      }

      // Fehlerbalken
      h_data->SetBinErrorOption(TH1::kPoisson);


    // Initialisiere Histogramme

      TFile * file_histogram_zprime = new TFile(("/net/e4-nfs-home.e4.physik.tu-dortmund.de/home/photon/Breitfeld_Knospe_26-04-23/plots/zprime" + zprime_mass + "." + lepton + "_selected_plots.root").c_str());
      TH1F * h_zprime = (TH1F*)file_histogram_zprime->Get(inv_mass.c_str());
      TDirectoryFile *dir_zprime = (TDirectoryFile*)file_histogram_zprime->Get("Histograms");
      h_zprime = (TH1F*)dir_zprime->Get(inv_mass.c_str());

      TFile * file_histogram_diboson = new TFile(("/net/e4-nfs-home.e4.physik.tu-dortmund.de/home/photon/Breitfeld_Knospe_26-04-23/plots/diboson." + lepton + "_selected_plots.root").c_str());
      TH1F * h_diboson = (TH1F*)file_histogram_diboson->Get(inv_mass.c_str());
      TDirectoryFile *dir_diboson = (TDirectoryFile*)file_histogram_diboson->Get("Histograms");
      h_diboson = (TH1F*)dir_diboson->Get(inv_mass.c_str());

      TFile * file_histogram_singletop = new TFile(("/net/e4-nfs-home.e4.physik.tu-dortmund.de/home/photon/Breitfeld_Knospe_26-04-23/plots/singletop." + lepton + "_selected_plots.root").c_str());
      TH1F * h_singletop = (TH1F*)file_histogram_singletop->Get(inv_mass.c_str());
      TDirectoryFile *dir_singletop = (TDirectoryFile*)file_histogram_singletop->Get("Histograms");
      h_singletop = (TH1F*)dir_singletop->Get(inv_mass.c_str());

      TFile * file_histogram_ttbar = new TFile(("/net/e4-nfs-home.e4.physik.tu-dortmund.de/home/photon/Breitfeld_Knospe_26-04-23/plots/ttbar." + lepton + "_selected_plots.root").c_str());
      TH1F * h_ttbar = (TH1F*)file_histogram_ttbar->Get(inv_mass.c_str());
      TDirectoryFile *dir_ttbar = (TDirectoryFile*)file_histogram_ttbar->Get("Histograms");
      h_ttbar = (TH1F*)dir_ttbar->Get(inv_mass.c_str());

      TFile * file_histogram_wjets = new TFile(("/net/e4-nfs-home.e4.physik.tu-dortmund.de/home/photon/Breitfeld_Knospe_26-04-23/plots/wjets." + lepton + "_selected_plots.root").c_str());
      TH1F * h_wjets = (TH1F*)file_histogram_wjets->Get(inv_mass.c_str());
      TDirectoryFile *dir_wjets = (TDirectoryFile*)file_histogram_wjets->Get("Histograms");
      h_wjets = (TH1F*)dir_wjets->Get(inv_mass.c_str());

      TFile * file_histogram_zjets = new TFile(("/net/e4-nfs-home.e4.physik.tu-dortmund.de/home/photon/Breitfeld_Knospe_26-04-23/plots/zjets." + lepton + "_selected_plots.root").c_str());
      TH1F * h_zjets = (TH1F*)file_histogram_zjets->Get(inv_mass.c_str());
      TDirectoryFile *dir_zjets = (TDirectoryFile*)file_histogram_zjets->Get("Histograms");
      h_zjets = (TH1F*)dir_zjets->Get(inv_mass.c_str());

      //If you want to scale the histogram, use Scale(float factor)
      //If you want to adjust the bin width, use Rebin(int number_of_bins_to_be_merged)

      h_zprime->Scale(scaleFactor_zprime);
      h_diboson->Scale(scaleFactor_diboson);
      h_singletop->Scale(scaleFactor_singletop);
      h_ttbar->Scale(scaleFactor_ttbar);
      h_wjets->Scale(scaleFactor_wjets);
      h_zjets->Scale(scaleFactor_zjets);


      //You should set a different fill color for each process using SetFillColor(Color_t fcolor); examples for fcolor are kRed, kGreen, kYellow etc. 
      //  You can also add integer like kRed+2 to change the shade
      h_zprime->SetFillColor(kRed);
      h_diboson->SetFillColor(kBlue);
      h_singletop->SetFillColor(kCyan);
      h_ttbar->SetFillColor(kMagenta);
      h_wjets->SetFillColor(kGreen);
      h_zjets->SetFillColor(kYellow);

      //You might also want to change the line color using e.g. SetLineColor(kBlack)
      h_zprime->SetLineColor(kBlack);
      h_diboson->SetLineColor(kBlack);
      h_singletop->SetLineColor(kBlack);
      h_ttbar->SetLineColor(kBlack);
      h_wjets->SetLineColor(kBlack);
      h_zjets->SetLineColor(kBlack);

      //You should add a legend to be able to distinguish the different processes
      TLegend *leg = new TLegend(0.7,0.6,0.85,0.9);
      leg->SetFillColor(0);
      leg->SetBorderSize(0);
      leg->SetTextSize(0.035);
      leg->AddEntry(h_data,"data", "f");
      leg->AddEntry(h_zprime, ("zprime" + zprime_mass).c_str(), "f");
      leg->AddEntry(h_diboson, "diboson", "f");
      leg->AddEntry(h_singletop, "singletop", "f");
      leg->AddEntry(h_ttbar, "ttbar", "f");  
      leg->AddEntry(h_wjets, "wjets", "f");
      leg->AddEntry(h_zjets, "zjets", "f");

      //To plot several MC histograms, use THStack. At this point you should be able to figure out its use by looking it up online. 
      //For further analysis, you should however combine them to a new TH1F

      THStack *mcStack = new THStack("Stack", "Stacked MC");

      mcStack->Add(h_ttbar);
      mcStack->Add(h_singletop); 
      mcStack->Add(h_diboson);
      mcStack->Add(h_wjets);
      mcStack->Add(h_zjets);
      mcStack->Add(h_zprime);   

      // und hier nochmal nur die Summe ohne THStack, damit wir wirklich die Summe aus allen Simulationen haben
      int nbins = 100;  // Anzahl der Bins
      double xmin = 0.0;  // Untere Grenze des Binning-Bereichs
      double xmax = 1000000.0;  // Obere Grenze des Binning-Bereichs

      // Erstellen des Summen-Histogramms
      TH1F *h_sum = new TH1F("Sum", "Sum of MC", nbins, xmin, xmax);

      // Berechnung der Summe der gestapelten Simulationen
      for (int i = 1; i <= nbins; i++)
      {
          double sumContent = 0.0;
          for (int j = 0; j < mcStack->GetHists()->GetEntries(); j++)
          {
              TH1F *hist = dynamic_cast<TH1F *>(mcStack->GetHists()->At(j));
              sumContent += hist->GetBinContent(i);
          }
          h_sum->SetBinContent(i, sumContent);
      }
      h_sum->Draw();


      //For histograms of data, you can use the following commands to change the attributes of the histobramm
      h_data->SetLineWidth(1);
      h_data->SetLineColor(kBlack);
      h_data->SetMarkerStyle(7);
      h_data->SetMarkerSize(1);
      h_data->SetMarkerColor(kBlack);

     //For plotting data and stacked MC, you can use the function PlotStack at the end of this file 
      TCanvas *c = new TCanvas("c", "Stacked Plot");
      mcStack->Draw("HIST");
      leg->Draw("SAME");

      //For all objects you want to write to the analysis file, call Write(), e.gl
      analysis->cd();
      mcStack->Write(("mcStack_" + zprime_mass + "_" + lepton).c_str());
      h_sum->Write(("h_sum_" + zprime_mass + "_" + lepton).c_str());      

      // Save the canvas as a PDF file
      PlotStack("stackedPlots/stackedPlot_" + inv_mass + "_" + zprime_mass + "_" + lepton + ".pdf", "GeV", mcStack, h_data, leg);

      //End the programm properly by deleting all dynamic instances
      file_histogram_data->Close();
      file_histogram_zprime->Close();
      delete file_histogram_data;
      delete file_histogram_zprime;
      delete file_histogram_diboson;
      delete file_histogram_singletop;
      delete file_histogram_ttbar;
      delete file_histogram_wjets;
      delete file_histogram_zjets;
      delete leg;
      delete c;

    }
  }
  analysis->Close();
  delete analysis;

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
