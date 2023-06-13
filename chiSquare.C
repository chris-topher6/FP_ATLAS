#include <iostream>
#include <string>
#include "TMath.h"
#include "TH1F.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TStyle.h"
#include "TLegend.h"
#include <fstream>


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

	if (argc < 3) {
    	cout << "Zu wenige Argumente. Bitte geben Sie einen Integer-Wert an." << endl;
    	return 1;  
  	}

    // Get the file paths from the command-line arguments
	string lepton = argv[1];
	string inv_mass = argv[2];
	//int scale_zprime_xsec = stoi(argv[3]);

	ofstream outputFile1("limits/Evidence_"    + inv_mass + "_" + lepton + ".txt");
	ofstream outputFile2("limits/Observation_" + inv_mass + "_" + lepton + ".txt");
	if (!outputFile1 or !outputFile2) {
        std::cout << "Fehler beim Öffnen der Datei." << std::endl;
        return 1;
    }
	outputFile1 << "Evidence"	  << endl << "p-value,	mass,	xsec" << endl;
	outputFile2 << "Observation"  << endl << "p-value,	mass,	xsec" << endl;


//	vector<double> scale_zprime_xsecs = {0.2, 0.4, 0.6, 0.8, 1.0, 1.2, 1.4, 1.6, 1.8, 2.0};
	vector<double> scale_zprime_xsecs = {
										0.7,
										0.8,
										0.9,
										1,
										1.1,
										1.2,
										1.3,
										1.4,
										1.5,
										2.2,
										2.4,
										2.6,
										2.8,
										3.5,
										3.6,
										3.7,
										3.8,
										3.9,
										4,
										4.5,
										5.5,
										6,
										6.5,
										7,
										7.5,
										8,
										8.5,
										9,
										9.5,
										11,
										11.5,
										12,
										13,
										14,
										15,
										16,
										17,
										18,
										19,
										20,
										21,
										22,
										23,
										24,
										25,
										26,
										27,
										28,
										29,
										30,
										31,
										32,
										33,
										34,
										35,
										36,
										37,
										38,
										39,
										40,
										50,
										51,
										52,
										53,
										54,
										55,
										70,
										80,
										100,
										110,
										130,
										135,
										140,
										145,
										150,
										180,
										200,
										240,
										270,
										280,
										300,
										325,
										400,
										525,
										600,
										710,
										720,
										730,
										740,
										750,
										850,
										1000,
										1150,
										1200,
										1275,
										1300,
										1750,
										1800,
										1950,
										2000,
										2750,
										3000,
										3300,
										3400,
										3500,
										4000,
										4250,
										4500,
										4750,
										5250,
										5500,
										5750,
										6500,
										9000};
	vector<string> loadedMasses_Evidence;
	vector<string> loadedMasses_Observation;

	for (const auto& scale_zprime_xsec : scale_zprime_xsecs) {

	string dataFilePath = ("plots/data." + lepton + "_selected_plots.root").c_str();
	string mcFilePath   = "stackedPlots/analysis_" + to_string(scale_zprime_xsec) + "_" + inv_mass + ".root";

    // Open the data file
    TFile* dataFile = new TFile(dataFilePath.c_str(), "READ");
    if (!dataFile || dataFile->IsZombie())
    {
        cout << "Error opening data file: " << dataFilePath << endl;
        return 1;
    }

    // Get the data histogram from the file
    TH1F* dataHistogram = static_cast<TH1F*>(dataFile->Get(inv_mass.c_str()));
    TDirectoryFile *dir_data = (TDirectoryFile*)dataFile->Get("Histograms");
	dataHistogram = (TH1F*)dir_data->Get(inv_mass.c_str());
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


	vector<string> masses= {
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

	float mass[11] = {400.,500.,750.,1000.,1250.,1500.,1750.,2000.,2250.,2500.,3000};
	float expectedXsec[11] = {1.1e2, 8.2e1, 2.0e1, 5.5, 1.9, 8.3e-1, 3.0e-1, 1.4e-1, 6.7e-2, 3.5e-2, 1.2e-2};
	float limitXsec[11];

	for (size_t m = 0; m < masses.size(); m++){
   		// Get the Monte Carlo histogram from the file
	    TH1F* mcHistogram  = static_cast<TH1F*>(mcFile->Get(("h_sum_" + masses[m] + "_" + lepton).c_str()));

    	if (!mcHistogram){
    	    cout << "Error retrieving Monte Carlo histogram from the file." << endl;
    	    return 1;
    	}

		float chiSquare = chisquareNBins(dataHistogram, mcHistogram);
		int dof = dataHistogram->GetNbinsX() - 1;  // #Freiheitsgrade= #Bins - 1

		// pValue
    	float pValue =  TMath::Prob(chiSquare, dof);

    	if (pValue < 2.7e-3 && pValue > 5.7e-7) {
			if (find(loadedMasses_Evidence.begin(), loadedMasses_Evidence.end(),masses[m]) == loadedMasses_Evidence.end()) {
    	    	outputFile1 << pValue << "\t" << masses[m] << "\t" << scale_zprime_xsec << endl;
				loadedMasses_Evidence.push_back(masses[m]);
			}
    	} else if (pValue < 5.7e-7) {
			if (find(loadedMasses_Observation.begin(), loadedMasses_Observation.end(),masses[m]) == loadedMasses_Observation.end()) {
	    	    outputFile2 << pValue << "\t" << masses[m] << "\t" << scale_zprime_xsec << endl;
				loadedMasses_Observation.push_back(masses[m]);
			}
		}
		else{
		}
		
		// limit
		float criticalValue = TMath::ChisquareQuantile(0.95, dof);
		limitXsec[m] = expectedXsec[m] * chiSquare / criticalValue;
		// cout << "limit: " << limitXsec[m] << endl;

	}

	if(scale_zprime_xsec == 1.0){
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
	  	h_helper->GetYaxis()->SetTitle("#sigma_{Z'}#timesBR(Z' #rightarrow t#bar{t}) [pb]"); // don't forget the axis titles !
	  	h_helper->Draw("p");

		// create a legend
		TLegend * l = new TLegend(0.35, 0.7, 0.9, 0.8, "");
	  	l->SetFillColor(0);
	  	l->SetBorderSize(0);
	  	l->SetTextSize(0.04);
	  	l->SetTextAlign(12);
	  	l->AddEntry(g_expected, "Expected #sigma_{Z'}#timesBR(Z' #rightarrow t#bar{t})", "l");
	  	l->AddEntry(g_limits, "Observed 95% CL upper limit (100 pb^{-1})", "l");

	  	g_expected->Draw("l SAME"); 
	  	g_limits->Draw("l SAME");
	  	l->Draw();
	  	c_limits->SetLogy();
	  	c_limits->Print(("limits/limits_" + inv_mass + "_" + lepton + ".pdf").c_str());
	}
	}

	for (const auto& mass : loadedMasses_Evidence) {
	    cout << "loadedMasses_Evidence: " << mass << endl; 
	}
		for (const auto& mass : loadedMasses_Observation) {
	    cout << "loadedMasses_Observation: " << mass << endl; 
	}

	outputFile1.close();
	outputFile2.close();
	
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


	// double ndf= double(nbinsused-1);

	// cout << "The number of degrees of freedom is " << ndf << endl;
	return chisquare_test;

}

bool CheckBinning(TH1F* hist1, TH1F* hist2)
{
    if (hist1->GetNbinsX() != hist2->GetNbinsX())
    {
        cout << "Error: Number of bins is different between the histograms." << endl;
        return false;
    }

    for (int i = 1; i <= hist1->GetNbinsX(); i++)
    {
        if (hist1->GetBinLowEdge(i) != hist2->GetBinLowEdge(i) || hist1->GetBinWidth(i) != hist2->GetBinWidth(i))
        {
            cout << "Error: Binning is different between the histograms." << endl;
            return false;
        }
    }

    return true;
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
