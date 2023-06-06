#include <iostream>
#include <string>
#include "TMath.h"
#include "TH1F.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TStyle.h"
#include "TLegend.h"


using namespace std;

float chisquareNBins(TH1F * data, TH1F * mc);

void SetStyle();

int main(int argc, char* argv[]){	// console input example: ./chiSquare.exe output_runSelection/data.root output_runSelection/mc.root

	// At this point, you want to first test the agreement between data and monte carlo.
	// After that, you want to create a limit plot
	// You should be able to do most of it on your own by now, but here are some usefull things:

	// The function chisquareNBins(TH1F * data, TH1F * mc) returns the chi square value that characterizes the aggreement between a data histogram and a monte carlo histogramm
	// It prints the degrees of freedom in this comparison, which might be useful information

	//////////////////////////////////////////////////////////////////
	// Do do: 1. quantify data-mc agreement
	//		  2. calculate limits to the cross section of Z' production
	//
	////////////////////////////////////////////////////////////////////////////

    // Get the file paths from the command-line arguments
    string dataFilePath = argv[1];
    string mcFilePath = argv[2];

	// extract string for pdf name
    size_t lastSlashIndex = mcFilePath.find_last_of("/");
    string fileName = mcFilePath.substr(lastSlashIndex + 1);
    size_t firstDotIndex = fileName.find_first_of(".");
    string extractedString = fileName.substr(0, firstDotIndex);

    // Open the data file
    TFile* dataFile = new TFile(dataFilePath.c_str(), "READ");
    if (!dataFile || dataFile->IsZombie())
    {
        cout << "Error opening data file: " << dataFilePath << endl;
        return 1;
    }

    // Get the data histogram from the file
    TH1F* dataHistogram = static_cast<TH1F*>(dataFile->Get("inv_mass"));
    TDirectoryFile *dir_data = (TDirectoryFile*)dataFile->Get("Histograms");
	dataHistogram = (TH1F*)dir_data->Get("inv_mass");
    if (!dataHistogram)
    {
        cout << "Error retrieving data histogram from the file." << endl;
        return 1;
    }

    // Open the Monte Carlo file
    TFile* mcFile =  new TFile((mcFilePath).c_str(), "READ");

    if (!mcFile || mcFile->IsZombie())
    {
        cout << "Error opening Monte Carlo file: " << mcFilePath << endl;
        return 1;
    }

    // Get the Monte Carlo histogram from the file
    TH1F* mcHistogram = static_cast<TH1F*>(mcFile->Get("mcsum"));

    if (!mcHistogram)
    {
        cout << "Error retrieving Monte Carlo histogram from the file." << endl;
        return 1;
    }

	cout << "It lives!" << endl;



	// Once you finished calculating your limits, you want to plot them
	// Start with arrays of the mass, the expected cross section and the limits you calculated
	// The mass array has already be filled for you in order to remind you of the use of arrays
	float mass[11] = {400.,500.,750.,1000.,1250.,1500.,1750.,2000.,2250.,2500.,3000};
	float expectedXsec[11] = {1.1e2, 8.2e1, 2.0e1, 5.5, 1.9, 8.3e-1, 3.0e-1, 1.4e-1, 6.7e-2, 3.5e-2, 1.2e-2};
	float limitXsec[11];
	float chiSquare = chisquareNBins(dataHistogram, mcHistogram);
	int dof = dataHistogram->GetNbinsX() - 1;  // #Freiheitsgrade= #Bins - 1
	// Berechne das kritische Chi-Quadrat-Quantil für 95% Konfidenz und Freiheitsgrade
	float criticalValue = TMath::ChisquareQuantile(0.95, dof);
	for (int i = 0; i < 11; i++) {
	  limitXsec[i] = expectedXsec[i] * chiSquare / criticalValue;
	}


	// Create a canvas
	SetStyle();
	TCanvas * c_limits = new TCanvas("c_limits", "canvas for limit plot", 1);
	// If you want to, use a logarithmic y axis 
  	c_limits->SetLogy();

	// Create a TGraph for the expected cross section
	TGraph * g_expected = new TGraph(11, mass, expectedXsec);
	g_expected->SetLineColor(kBlue);

	//Create a TGraph with you limits
	TGraph * g_limits = new TGraph(11, mass, limitXsec);
	
	//The TH1D is only to have axes to you plot
	TH1D * h_helper = new TH1D("h_helper", "just an empty helper histogram", 1, 400., 3000.);
	h_helper->SetMaximum(270);
	h_helper->GetXaxis()->SetTitle("m_{Z\'} [GeV]"); 
  	h_helper->GetYaxis()->SetTitle("#sigma_{Z'}#timesBR(Z'#rightarrow t#bar{t}) [pb]"); // don't forget the axis titles !
  	h_helper->Draw("p");

	// create a legend
	TLegend * l = new TLegend(0.35, 0.7, 0.9, 0.8, "");
  	l->SetFillColor(0);
  	l->SetBorderSize(0);
  	l->SetTextSize(0.04);
  	l->SetTextAlign(12);
  	l->AddEntry(g_expected, "Expected #sigma_{Z'}#timesBR(Z'#rightarrow t#bar{t})", "l");
  	l->AddEntry(g_limits, "Observed 95% CL upper limit (100 pb^{-1})", "l");
  	
  	g_expected->Draw("l SAME"); 
  	g_limits->Draw("l SAME");
  	l->Draw();
  	c_limits->SetLogy();
  	c_limits->Print(("limits/limits_" + extractedString + ".pdf").c_str());
	

	return 0;

}


float chisquareNBins(TH1F * data, TH1F * mc){

	float chisquare_test = 0.0;

	int nbins = data->GetSize();
	int nbinsused = 0;
	for(int i = 1; i < nbins; i++){
		float n_data = data->GetBinContent(i);
		float n_mc = mc->GetBinContent(i);
		if(n_mc != 0.){
			chisquare_test = chisquare_test + (n_data-n_mc)*(n_data-n_mc)/n_mc;
			nbinsused++;
		}
	}


	double ndf= double(nbinsused-1);

	cout << "The number of degrees of freedom is " << ndf << endl;
	return chisquare_test;

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
