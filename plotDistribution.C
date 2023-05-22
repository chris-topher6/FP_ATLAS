#include "fileHelper.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TStyle.h"
#include <iostream>
#include "string.h"
#include "TH1F.h"
#include "TLatex.h"
#include "TLorentzVector.h"
#include "TLegend.h"
#include "physicsHelper.h"


using namespace std;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// These functions might be useful. They can be found at the end of this script and don't have to be but can be altered. //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void SetStyle();
// Should be called before plotting to get a decent looking plot

TH1F * InitHist(TString varName,TString varUnit, int numberBins, float minBin, float maxBin, bool isData); 
// Helps you initialize a histogram so that it already ha the correct labels when plotted.
// varName is the variable name in the tuple or the name you pick for a derived physical variable. You should stick to it for the rest of the analysis. (e.g. lep_eta)
// 

void PlotHist(TString filename, TH1F * hist);
// Creates a file in with path name (e.g. "text.pdf") and plots histogram in it

 


int main(int argn, char *argv[]) {

  //if you want to run this script for several input files, it is useful to call the name of the file when executing this program
  if(argn==1){
    cout << "Please start plotDistribution.exe with added argument of file to be processed" << endl;
    return 1;
  }

  // path to the file to be studied, e.g.
  string path = string(argv[1]);

  // Extract the filename from the input file path
  string inputFileName(argv[1]);
  size_t pos = inputFileName.find_last_of('/');
  if (pos != string::npos) {
      inputFileName = inputFileName.substr(pos + 1);
  }

  // is the file a data file or not? setting this variable now might be useful
  bool isdata = false;
  if(inputFileName.find("data")!= string::npos){
    isdata = true;
  }

  // retrieve the tree from the file
  mini * tree = fileHelper::GetMiniTree(path);
  if (tree == 0) {
    cout << "ERROR tree is null pointer" << endl;
    return 1;
  }

  // check that the tree is not empty
  int nEntries = tree->GetEntries();
  cout << "INFO tree contains " << nEntries << " events" << endl;
  if (nEntries == 0) {
    cout << "ERROR tree contains zero events" << endl;
    return 1;
  }
  
  //////////////////////////////////////////////////////////////////////////////
  // To do: initialize histograms to be made
  TH1F * h_lep_pt  = InitHist("lep_pt","p_{T}(l) [MeV]",100,0,140.e3,isdata);
  TH1F * h_lep_eta = InitHist("lep_eta", "#eta(l)", 100, -2.5, 2.5, isdata);
  TH1F * h_lep_phi = InitHist("lep_phi", "#phi(l)", 100, -3.2, 3.2, isdata);
  TH1F * h_lep_E   = InitHist("lep_E", "#E(l)", 100, 0, 400.e3, isdata);

  TH1F * h_jet_pt  = InitHist("jet_pt","p_{T}(j) [MeV]",100,0,180.e3,isdata);
  TH1F * h_jet_eta = InitHist("jet_eta", "#eta(j)", 100, -2.5, 2.5, isdata);
  TH1F * h_jet_phi = InitHist("jet_phi", "#phi(j)", 100, -3.2, 3.2, isdata);
  TH1F * h_jet_E   = InitHist("jet_E", "E(l)", 100, 0, 450.e3, isdata);

  TH1F * h_jet_good_n = InitHist("jet_good_count", "Number of good Jets", 5, 0, 5, isdata);
  TH1F * h_bjet_n     = InitHist("bjet_count", "Number of b-Jets", 5, 0, 5, isdata);

  TH1F * h_jet_pt_max  = InitHist("jet_pt_max", "p_{T}(j_{max}) [MeV]", 100, 0, 180.e3, isdata);
  TH1F * h_jet_eta_max = InitHist("jet_eta_max", "#eta(j_{max})", 100, -2.5, 2.5, isdata);
  TH1F * h_jet_phi_max = InitHist("jet_phi_max", "#phi(j_{max})", 100, -3.2, 3.2, isdata);
  TH1F * h_jet_E_max   = InitHist("jet_E_max", "E(j_{max})", 100, 0, 450.e3, isdata);


  TH1F * h_met_et   = InitHist("met_et", "#p_{miss} [MeV]", 100, 0, 150.e3, isdata);
  TH1F * h_met_phi  = InitHist("met_phi", "#phi_{miss}", 100, -3.2, 3.2, isdata);

  TH1F * h_deltaPhi  = InitHist("deltaPhi", "#delta #phi(E_{T}^{miss},l)", 100, 0, 3.2, isdata);
  TH1F * h_inv_mass3 = InitHist("inv_mass3", "m_{inv}(3j)", 100, 0, 20000, isdata);
  TH1F * h_inv_mass  = InitHist("inv_mass", "m_{inv}", 100, 10000, 1.e6, isdata);
  TH1F * h_sys_eta   = InitHist("sys_eta", "#eta_{total}", 100, -2.5, 2.5, isdata);
  //////////////////////////////////////////////////////////////////////////////////


  // now loop over the events in the tree
  for (int iEntry = 0; iEntry < nEntries; ++iEntry) {

    // get entry no. iEntry and tell every 100th event
    tree->GetEntry(iEntry);
    if ((iEntry+1)%1000000 == 0) {
      cout << "INFO processing the " << iEntry+1 << "th event" << endl;
    }

    // For Monte Carlo, each event has to have a scale factor. 
    // The scale factors necessary can be found separately in the samples, but they have also been combined in the variable scaleFactor_COMBINED.
    float w = 1.;
    if (!isdata)
      w = tree->scaleFactor_COMBINED;
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Get variable or calculate it (s. instructions)    
    float met_et  = tree->met_et;
    float met_phi = tree->met_phi;
    // fill histograms
    h_met_et->Fill(met_et ,w);
    h_met_phi->Fill(met_phi,w);
    
    // leptons
    for (UInt_t iLep = 0; iLep < tree->lep_n; ++iLep) {
      float lep_pt  = tree->lep_pt[iLep];
      float lep_eta = tree->lep_eta[iLep];
      float lep_phi = tree->lep_phi[iLep];
      float lep_E   = tree->lep_E[iLep];
      //derived quantities
      float deltaPhi = 0;
      // calculate quantities
      float diff = fabs(lep_phi - met_phi);
      float diff_2pi = fabs(2 * M_PI - diff);
      deltaPhi = (diff < diff_2pi) ? diff : diff_2pi;

      // fill histograms
      h_lep_pt->Fill(lep_pt,w);
      h_lep_eta->Fill(lep_eta, w);
      h_lep_phi->Fill(lep_phi,w);
      h_lep_E->Fill(lep_E  ,w);
      h_deltaPhi->Fill(deltaPhi,w);
    }

    // jets
    int JetGood_n=0;
    int bjet_n = 0;
    float jet_pt_max = 0;
    float jet_eta_max = 0;
    float jet_phi_max = 0;
    float jet_E_max = 0;

    TLorentzVector jet1, jet2, jet3, jet4;
    jet1.SetPxPyPzE(0.0, 0.0, 0.0, 0.0);
    jet2.SetPxPyPzE(0.0, 0.0, 0.0, 0.0);
    jet3.SetPxPyPzE(0.0, 0.0, 0.0, 0.0);
    jet4.SetPxPyPzE(0.0, 0.0, 0.0, 0.0);
    TLorentzVector lepton;
    TLorentzVector metvect; 


    // Loop over jets in the event
    for (UInt_t iJet = 0; iJet < tree->jet_n; ++iJet) {
      bool isJetGood = (tree->jet_good[iJet] == 1); // Check the jet_good condition

      if (isJetGood) { // Increment jet_good count
        JetGood_n++;
      }
      float jet_pt  = tree->jet_pt[iJet];
      float jet_eta = tree->jet_eta[iJet];
      float jet_phi = tree->jet_phi[iJet];
      float jet_E   = tree->jet_E[iJet];
      float jet_MV1 = tree->jet_MV1[iJet];
      
      // fill histograms
      if(isJetGood){ //only good Jets
        h_jet_pt->Fill(jet_pt ,w);
        h_jet_eta->Fill(jet_eta,w);
        h_jet_phi->Fill(jet_phi,w);
        h_jet_E->Fill(jet_E  ,w);
      }
      if (jet_pt > jet_pt_max) { // only the jets with max pt
        jet_pt_max = jet_pt;
        jet_eta_max = jet_eta;
        jet_phi_max = jet_phi;
        jet_E_max = jet_E;
      }
      if (jet_MV1 > 0.7892) {
        bjet_n++;
      }

      // Compare jet_pt with the pT of the three largest jets
      if (jet_pt > jet1.Pt()) { // Update four-vector values
          jet4 = jet3;
          jet3 = jet2;
          jet2 = jet1;
          jet1.SetPtEtaPhiE(tree->jet_pt[iJet], tree->jet_eta[iJet], tree->jet_phi[iJet], tree->jet_E[iJet]);
      } else if (jet_pt > jet2.Pt()) {
          jet4 = jet3;
          jet3 = jet2;
          jet2.SetPtEtaPhiE(tree->jet_pt[iJet], tree->jet_eta[iJet], tree->jet_phi[iJet], tree->jet_E[iJet]);
      } else if (jet_pt > jet3.Pt()) {
          jet4 = jet3;
          jet3.SetPtEtaPhiE(tree->jet_pt[iJet], tree->jet_eta[iJet], tree->jet_phi[iJet], tree->jet_E[iJet]);
      } else if (jet_pt > jet4.Pt()) {
          jet4.SetPtEtaPhiE(tree->jet_pt[iJet], tree->jet_eta[iJet], tree->jet_phi[iJet], tree->jet_E[iJet]);
      }
    }
    h_jet_good_n->Fill(JetGood_n, w);
    h_bjet_n->Fill(bjet_n,w);
    if(jet_pt_max!=0){
      h_jet_pt_max->Fill(jet_pt_max ,w);
      h_jet_eta_max->Fill(jet_eta_max,w);
      h_jet_phi_max->Fill(jet_phi_max,w);
      h_jet_E_max->Fill(jet_E_max  ,w);
    }
    // Calculate m(3jets)
    TLorentzVector sumJets = jet1 + jet2 + jet3;
    Float_t inv_mass3 = sumJets.M();
    if (inv_mass3 != 0){
      h_inv_mass3->Fill(inv_mass3, w);
    }
    // Calculate m(4 jets, lep, v_lep)
    lepton.SetPtEtaPhiE(tree->lep_pt[0], tree->lep_eta[0], tree->lep_phi[0], tree->lep_E[0]);
    TLorentzVector* neutrino = physicsHelper
    ::Neutrino(metvect, lepton);
    TLorentzVector system = jet1;
    system += jet2;
    system += jet3;
    system += jet4;
    system += lepton;
    system += *neutrino;
    double inv_mass = system.M();
    double sys_eta = system.Eta();
    h_inv_mass->Fill(inv_mass,w);
    h_sys_eta->Fill(sys_eta);
    //
    ///////////////////////////////////////////////////////////////////////////////////////////
  }

  SetStyle();

  ///////////////////////////////////////////////////////////////////////////////////////////
  //To do: Use PlotHist to plot
  PlotHist("plots/pdfs/lep_pt.pdf",  h_lep_pt);
  PlotHist("plots/pdfs/lep_eta.pdf", h_lep_eta);
  PlotHist("plots/pdfs/lep_phi.pdf", h_lep_phi);
  PlotHist("plots/pdfs/lep_E.pdf",   h_lep_E  );

  PlotHist("plots/pdfs/jet_pt.pdf",  h_jet_pt );
  PlotHist("plots/pdfs/jet_eta.pdf", h_jet_eta);
  PlotHist("plots/pdfs/jet_phi.pdf", h_jet_phi);
  PlotHist("plots/pdfs/jet_E.pdf",   h_jet_E  );

  PlotHist("plots/pdfs/jet_good_n.pdf", h_jet_good_n);
  PlotHist("plots/pdfs/bjet_n.pdf", h_bjet_n);

  PlotHist("plots/pdfs/jet_pt_max.pdf",  h_jet_pt_max );
  PlotHist("plots/pdfs/jet_eta_max.pdf", h_jet_eta_max);
  PlotHist("plots/pdfs/jet_phi_max.pdf", h_jet_phi_max);
  PlotHist("plots/pdfs/jet_E_max.pdf",   h_jet_E_max  );

  PlotHist("plots/pdfs/met_et.pdf",  h_met_et );
  PlotHist("plots/pdfs/met_phi.pdf", h_met_phi);

  PlotHist("plots/pdfs/deltaPhi.pdf", h_deltaPhi);
  PlotHist("plots/pdfs/inv_mass3.pdf", h_inv_mass3);
  PlotHist("plots/pdfs/inv_mass.pdf", h_inv_mass);
  PlotHist("plots/pdfs/sys_eta.pdf", h_sys_eta);
  /////////////////////////////////////////////////////////////////////////////////////////////
  //You can now use fileHelper::SaveNewHist to save histograms
  
  // Create the name of the .root file based on the input filename
  pos = inputFileName.find_last_of('.');
  if (pos != string::npos) {
    inputFileName = inputFileName.substr(0, pos);
  }
  string outputRootFileName = "plots/" + inputFileName + "_plots.root";
  
  // Open the .root file for writing
  TFile* outputFile = new TFile(outputRootFileName.c_str(), "RECREATE");
  TDirectory* histDir = outputFile->mkdir("Histograms");
  histDir->cd();


  //Save histograms
  h_lep_pt->Write();
  h_lep_eta->Write();
  h_lep_phi->Write();
  h_lep_E->Write();
  h_jet_pt->Write();
  h_jet_eta->Write();
  h_jet_phi->Write();
  h_jet_E->Write();
  h_jet_good_n->Write();
  h_bjet_n->Write();
  h_jet_pt_max->Write();
  h_jet_eta_max->Write();
  h_jet_phi_max->Write();
  h_jet_E_max->Write();
  h_met_et->Write();
  h_met_phi->Write();
  h_deltaPhi->Write();
  h_inv_mass3->Write();
  h_inv_mass->Write();
  h_sys_eta->Write();


  // Schließe die Root-Datei
  outputFile->Close();

  // To end the program properly, delete all dynamic instances
  delete h_lep_pt;
  delete h_lep_eta;
  delete h_lep_phi;
  delete h_lep_E;
  delete h_jet_pt;
  delete h_jet_eta;
  delete h_jet_phi;
  delete h_jet_E;
  delete h_jet_pt_max;
  delete h_jet_eta_max;
  delete h_jet_phi_max;
  delete h_jet_E_max;  
  delete h_met_et;
  delete h_met_phi;
  delete h_deltaPhi;
  delete h_inv_mass3;
  delete h_sys_eta;
  delete tree;

  return 0;
}



//////////////////////////////////////////////////////////////////////////////////////////
////// Functions that can but do not have to be uses: ////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////



TH1F * InitHist(TString varName,TString varUnit, int numberBins, float minBin, float maxBin, bool isData){
  TH1F *hist = new TH1F(varName,varName,numberBins,minBin,maxBin);
  hist->SetTitle(";"+varUnit+";Events");
  hist->GetYaxis()->SetTitleOffset(1.3);
  hist->Sumw2(false);
  if(isData){
    hist->Sumw2(true);
  }
  return hist;
}


void PlotHist(TString filename, TH1F * hist){
	TCanvas * canv = new TCanvas("canv","Canvas for histogram",1);
  hist->Draw("HIST+E");
  canv->Print(filename);
  cout << "INFO: " << filename << " has been created" << endl;
  delete canv;
}

void Plot2Hist(TString filename, TString varUnit, TH1F * hist1, TH1F * hist2) {
  TCanvas * canv = new TCanvas("canv","Canvas for histograms",1);
  canv->SetLeftMargin(.12);
  canv->SetRightMargin(.1);
  
  hist1->Draw("HIST");

  hist1->SetTitle(";"+varUnit+";Events");
  hist1->GetYaxis()->SetTitleOffset(1);

  hist2->Draw("HIST SAME");

  TLegend * l = new TLegend(0.5, 0.75, 0.86, 0.9, "");
  l->SetFillColor(0);
  l->SetBorderSize(1);
  l->SetTextSize(0.04);
  l->SetTextAlign(12);
  l->AddEntry(hist1, "Add description", "l");
  l->AddEntry(hist2, "Add description here", "l");
  l->Draw();

  ///////////////////////////////////////////
  // Histograms can be normalized to unit area by calling 
  // hist->Scale(1./hist->Integral()) before plotting
  // In case you decide to do that, you can use the following lines to label your plots
  ////////////////////////////////////////////////////////////////////////////////////////
  //TLatex * t = new TLatex(0.93,0.55,"#bf{Normalized to unit area}");
  //t->SetTextAngle(90);
  //t->SetNDC();
  //t->SetTextSize(.04);
  //t->DrawClone();

  canv->Print(filename);
  cout << "INFO: " << filename << " has been created" << endl;
  delete canv;
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
  gStyle->SetMarkerStyle(20);
  gStyle->SetMarkerSize(1.2);
  gStyle->SetHistLineWidth(2);
  gStyle->SetOptTitle(0);
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(0);
}
