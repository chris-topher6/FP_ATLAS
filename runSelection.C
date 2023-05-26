#include "mini.h"
#include "fileHelper.h"
#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>

using namespace std;

int main(int argn, char *argv[]) {

  //if you want to run this script for several input files, it is useful to call the name of the file when executing this program
  if(argn==1){
    cout << "Please start runSelection.exe with added argument of file to be processed" << endl;
    return 1;
  }
  // path to the file to be studied + filename from argument called when executing file
  string inpath = string(argv[1]);
  TString filename = TString(inpath).ReplaceAll("/net/e4-nfs-home.e4.physik.tu-dortmund.de/home/photon/E4/Final/samples/", "");

  cout << "Processing " << filename << endl;

  // retrieve the tree from the file
  mini * tree = fileHelper::GetMiniTree(inpath);
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

  // create file to which the selection is going to be saved to
  TString outpath = "output_runSelection/" + TString(filename);
  outpath.ReplaceAll(".root", "_selected.root");
  TFile * newFile = new TFile(outpath, "RECREATE");
  
  // make a copy of the tree to be saved after selection
  TTree * newTree = tree->CloneTree();

  // Define parameters for efficiency
  int totalEvents=0;
  int passedEvents=0;
  int count_lep_n=0;  
  int count_jet_n=0;
  int count_jet_good=0; 
  int count_bjet=0;

  // now loop over the events in the tree
  for (int iEntry = 0; iEntry < nEntries; ++iEntry) {
    
    totalEvents++;

    // get entry no. iEntry and tell every 100000th event
    tree->GetEntry(iEntry);
    if ((iEntry+1)%1000000 == 0) {
      cout << "INFO processing the " << iEntry+1 << "th event" << endl;
    }

    //////////////////////////////////////////////////////
    // To do: Implement all required selection criteria //
    bool passCriteria = true;
    if (tree->lep_n != 1) { //lepton number = 1
      count_lep_n++;
      passCriteria = false;
    }
    if (tree->jet_n != 4) { //jet number = 4; Aaron: besser > 4
      count_jet_n++;
      passCriteria = false;
    }

    bool allJetsGood = true; //all jets are good; Aaron: nur 4 müssen gut sein
    for (int iJet = 0; iJet < static_cast<int>(tree->jet_n); ++iJet) {
      if (tree->jet_good[iJet] != 1) {
        allJetsGood = false;
        break; // Exit the loop if any jet is not good
      }
    }
    if (!allJetsGood) {
      count_jet_good++;
      passCriteria = false;
    }

    int bJetCount = 0; // 2 b-jets
    for (int iJet = 0; iJet < static_cast<int>(tree->jet_n); ++iJet) {
      if (tree->jet_MV1[iJet] > 0.78) {
        bJetCount++;
      }
    }
    if (bJetCount != 2) {
      count_bjet++;
      passCriteria = false;
    }
    //////////////////////////////////////////////////////
  
    // check all selection criteria and only save the event to the new
    // tree if all of them are true
    if (passCriteria) {
      passedEvents++;
      newTree->Fill();
    }

  }

  // efficiencies
  double eff_tot      = static_cast<double>(passedEvents)              / totalEvents;
  double eff_lep_n    = static_cast<double>(totalEvents-count_lep_n)   / totalEvents;
  double eff_jet_n    = static_cast<double>(totalEvents-count_jet_n)   / totalEvents;
  double eff_jet_good = static_cast<double>(totalEvents-count_jet_good)/ totalEvents;
  double eff_bjet     = static_cast<double>(totalEvents-count_bjet)    / totalEvents;


  // save efficiencies in .txt
  TString effpath = "output_runSelection/" + TString(filename);
  effpath.ReplaceAll(".root", "_efficiency.txt");

  ofstream outputFile(effpath.Data());

  if (outputFile.is_open()) {
    outputFile << "List of all Efficiencies" << endl;
    outputFile << "total Efficiency: " << eff_tot << endl;
    outputFile << "Efficiency lep_n: " << eff_lep_n << endl;
    outputFile << "Efficiency jet_n: " << eff_jet_n << endl;
    outputFile << "Efficiency jet_good: " << eff_jet_good << endl;
    outputFile << "Efficiency bjet:" << eff_bjet << endl;

    outputFile.close();
    cout << "Efficiencies saved to .txt file" << endl;
    }
  else {
    cout << "Unable to open file for writing" << endl;
  }

  // save new tree to file
  cout << "INFO saving new tree with " << newTree->GetEntries() << " entries" << endl;
  newFile->Write();
  gDirectory->Purge();
  newFile->Close();
  
  // end program happily
  delete newFile;
  return 0;
}
