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
  TFile * file_histogram_data = new TFile("/net/e4-nfs-home.e4.physik.tu-dortmund.de/home/photon/Breitfeld_Knospe_26-04-23/plots/data.el_selected_plots.root");
  TH1F * h_data = (TH1F*)file_histogram_data->Get("inv_mass");
  TDirectoryFile *dir_data = (TDirectoryFile*)file_histogram_data->Get("Histograms");
  if (dir_data) {
    h_data = (TH1F*)dir_data->Get("inv_mass");
    if (h_data) {
      // alles erfolgreich geladen
    } else {
      cout << "Histogramm 'inv_mass' wurde nicht im Verzeichnis 'Histograms' gefunden!" << endl;
    }
  } else {
    cout << "Verzeichnis 'Histograms' wurde nicht im ROOT-File gefunden!" << endl;
  }

//  file_histogram_data->ls();  // Zeige den Inhalt des ROOT-Files an


  TFile * file_histogram_zprime1000 = new TFile("/net/e4-nfs-home.e4.physik.tu-dortmund.de/home/photon/Breitfeld_Knospe_26-04-23/plots/zprime1000.el_selected_plots.root");
  TH1F * h_zprime1000 = (TH1F*)file_histogram_zprime1000->Get("inv_mass");
  TDirectoryFile *dir_zprime1000 = (TDirectoryFile*)file_histogram_zprime1000->Get("Histograms");
  h_zprime1000 = (TH1F*)dir_zprime1000->Get("inv_mass");

  TFile * file_histogram_diboson = new TFile("/net/e4-nfs-home.e4.physik.tu-dortmund.de/home/photon/Breitfeld_Knospe_26-04-23/plots/diboson.el_selected_plots.root");
  TH1F * h_diboson = (TH1F*)file_histogram_diboson->Get("inv_mass");
  TDirectoryFile *dir_diboson = (TDirectoryFile*)file_histogram_diboson->Get("Histograms");
  h_diboson = (TH1F*)dir_diboson->Get("inv_mass");

  TFile * file_histogram_singletop = new TFile("/net/e4-nfs-home.e4.physik.tu-dortmund.de/home/photon/Breitfeld_Knospe_26-04-23/plots/singletop.el_selected_plots.root");
  TH1F * h_singletop = (TH1F*)file_histogram_singletop->Get("inv_mass");
  TDirectoryFile *dir_singletop = (TDirectoryFile*)file_histogram_singletop->Get("Histograms");
  h_singletop = (TH1F*)dir_singletop->Get("inv_mass");

  TFile * file_histogram_ttbar = new TFile("/net/e4-nfs-home.e4.physik.tu-dortmund.de/home/photon/Breitfeld_Knospe_26-04-23/plots/ttbar.el_selected_plots.root");
  TH1F * h_ttbar = (TH1F*)file_histogram_ttbar->Get("inv_mass");
  TDirectoryFile *dir_ttbar = (TDirectoryFile*)file_histogram_ttbar->Get("Histograms");
  h_ttbar = (TH1F*)dir_ttbar->Get("inv_mass");

  TFile * file_histogram_wjets = new TFile("/net/e4-nfs-home.e4.physik.tu-dortmund.de/home/photon/Breitfeld_Knospe_26-04-23/plots/wjets.el_selected_plots.root");
  TH1F * h_wjets = (TH1F*)file_histogram_wjets->Get("inv_mass");
  TDirectoryFile *dir_wjets = (TDirectoryFile*)file_histogram_wjets->Get("Histograms");
  h_wjets = (TH1F*)dir_wjets->Get("inv_mass");

  TFile * file_histogram_zjets = new TFile("/net/e4-nfs-home.e4.physik.tu-dortmund.de/home/photon/Breitfeld_Knospe_26-04-23/plots/zjets.el_selected_plots.root");
  TH1F * h_zjets = (TH1F*)file_histogram_zjets->Get("inv_mass");
  TDirectoryFile *dir_zjets = (TDirectoryFile*)file_histogram_zjets->Get("Histograms");
  h_zjets = (TH1F*)dir_zjets->Get("inv_mass");

  //If you want to scale the histogram, use Scale(float factor)
  //If you want to adjust the bin width, use Rebin(int number_of_bins_to_be_merged)

  float N_ges = h_data->GetEntries();
  cout << "N_ges: "<< N_ges << endl;
  cout << "scale Factors:" << endl;

  float N_zprime1000 = 588.4834999999999;
  float scaleFactor_zprime1000= N_zprime1000 / N_ges;
  cout << "zprime1000: "<< scaleFactor_zprime1000 << endl;

  float N_diboson = 9.99984115; 
  float scaleFactor_diboson = N_diboson / N_ges;
  cout << "diboson: "<< scaleFactor_diboson << endl;

  float N_singletop = 875.189106;
  float scaleFactor_singletop = N_singletop / N_ges;  
  cout << "singletop: "<< scaleFactor_singletop << endl;

  float N_ttbar = 22541.254226;
  float scaleFactor_ttbar = N_ttbar / N_ges;
  scaleFactor_ttbar = 0.025;  
  cout << "ttbar: "<< scaleFactor_ttbar << endl;

  float N_wjets = 1164.3670136;
  float scaleFactor_wjets= N_wjets / N_ges;
  cout << "wjets: "<< scaleFactor_wjets << endl;

  float N_zjets = 471.0779316;
  float scaleFactor_zjets= N_zjets / N_ges;
  cout << "zjets: "<< scaleFactor_zjets << endl;
  


  h_zprime1000->Scale(scaleFactor_zprime1000);
  h_diboson->Scale(scaleFactor_diboson);
  h_singletop->Scale(scaleFactor_singletop);
  h_ttbar->Scale(scaleFactor_ttbar);
  h_wjets->Scale(scaleFactor_wjets);
  h_zjets->Scale(scaleFactor_zjets);


  //You should set a different fill color for each process using SetFillColor(Color_t fcolor); examples for fcolor are kRed, kGreen, kYellow etc. 
  //  You can also add integer like kRed+2 to change the shade
  h_zprime1000->SetFillColor(kRed+1);
  h_diboson->SetFillColor(kBlue+1);
  h_singletop->SetFillColor(kCyan+1);
  h_ttbar->SetFillColor(kMagenta+1);
  h_wjets->SetFillColor(kGreen+1);
  h_zjets->SetFillColor(kYellow+1);

  //You might also want to change the line color using e.g. SetLineColor(kBlack)
  h_zprime1000->SetLineColor(kBlack);
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
  leg->AddEntry(h_zprime1000, "zprime", "f");
  leg->AddEntry(h_diboson, "diboson", "f");
  leg->AddEntry(h_singletop, "singletop", "f");
  leg->AddEntry(h_ttbar, "ttbar", "f");  
  leg->AddEntry(h_wjets, "wjets", "f");
  leg->AddEntry(h_zjets, "zjets", "f");

  //To plot several MC histograms, use THStack. At this point you should be able to figure out its use by looking it up online. 
  //For further analysis, you should however combine them to a new TH1F
    
  THStack *mcStack = new THStack("Stack", "Stacked MC");

  mcStack->Add(h_diboson);
  mcStack->Add(h_singletop); 
  mcStack->Add(h_ttbar);
  mcStack->Add(h_wjets);
  mcStack->Add(h_zjets);
  mcStack->Add(h_zprime1000);   

  //For histograms of data, you can use the following commands to change the attributes of the histobramm
  h_data->SetLineWidth(0);
  h_data->SetLineColor(kBlack);
  h_data->SetMarkerStyle(7);
  h_data->SetMarkerSize(1);
  h_data->SetMarkerColor(kBlack);

  //For plotting data and stacked MC, you can use the function PlotStack at the end of this file 

  analysis->cd();
  //For all objects you want to write to the analysis file, call Write(), e.gl
  h_data->Write();
  h_zprime1000->Write();

  PlotStack("output.pdf", "GeV", mcStack, h_data, leg);

  //End the programm properly by deleting all dynamic instances
  file_histogram_data->Close();
  file_histogram_zprime1000->Close();
  delete file_histogram_data;
  delete file_histogram_zprime1000;
  delete file_histogram_diboson;
  delete file_histogram_singletop;
  delete file_histogram_ttbar;
  delete file_histogram_wjets;
  delete file_histogram_zjets;
  delete leg;
  analysis->Close();

  // hier beginnt mein Code
  //TString inputDirectory = "~/Breitfeld_Knospe_26-04-23/output_runSelection";
  //TString outputDirectory = "~/E4/Final/samples/";
  //
  //// Array mit den Dateinamen der .root-Dateien
  //TString files[] = {"data.el.root", "data.mu.root", "ttbar.el.root", "ttbar.mu.root", "zprime1000.el.root", "zprime1000.mu.root"};
  //
  //// Schleife über die Dateien
  //for (TString file : files)  {
  //  // Konstruiere den vollständigen Pfad für die Eingabe- und Ausgabedateien
  //  TString inputFile = inputDirectory + "/" + file;
  //  TString outputFile = outputDirectory + "/" + file;
  //
  //  // Ausführung von plotDistribution für jede Datei
  //  TString command = "./plotDistribution.exe " + inputFile + " " + outputFile;
  //  int result = system(command.Data());
  //  if (result != 0)
  //  {
  //    cout << "Fehler beim Ausführen von plotDistribution für Datei: " << file << endl;
  //    // Hier könntest du geeignete Fehlerbehandlung durchführen
  //  }
  //}
  //PlotStack("output.root", "m", mc, data, leg);
  // hier endet mein Code

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






