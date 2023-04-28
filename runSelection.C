#include "mini.h"
#include "fileHelper.h"
#include <iostream>
#include <string>

using namespace std;

int main(int argn, char *argv[]) {

  //if you want to run this script for several input files, it is useful to call the name of the file when executing this program
  if(argn==1){
    cout << "Please start runSelection.exe with added argument of file to be processed" << endl;
    return 1;
  }
  // path to the file to be studied + filename from argument called when executing file
  string inpath = string(argv[1]);
  TString filename = TString(inpath).ReplaceAll("/net/e4-nfs-home.e4.physik.tu-dortmund.de/home/zprime/E4/Final/samples/", "");

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
  TString outpath = "output_runSelection/" + filename;
  outpath.ReplaceAll(".root", "_selected.root");
  TFile * newFile = new TFile(outpath, "RECREATE");
  
  // make a copy of the tree to be saved after selection
  TTree * newTree = tree->CloneTree();
    
   
  // now loop over the events in the tree
  for (int iEntry = 0; iEntry < nEntries; ++iEntry) {

    // get entry no. iEntry and tell every 100000th event
    tree->GetEntry(iEntry);
    if ((iEntry+1)%100000 == 0) {
      cout << "INFO processing the " << iEntry+1 << "th event" << endl;
    }

    //////////////////////////////////////////////////////
    // To do: Implement all required selection criteria //
    //////////////////////////////////////////////////////


    bool passCriteria = true;




    // check all selection criteria and only save the event to the new
    // tree if all of them are true
    if (passCriteria) {
      newTree->Fill();
    }
    
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
