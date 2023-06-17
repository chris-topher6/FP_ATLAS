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
#include "THStack.h"

using namespace std;

float chisquareNBins(TH1F * data, TH1F * mc);
bool CheckBinning(TH1F * data, TH1F *mc);
float scaleHistogramUpUntilThreshold(TH1F *dataHistogram, TH1F *mcHistogram_BG, const string& masses, const string& lepton, const string& inv_mass, const float& xsec, int dof, float threshold);
float scaleHistogramDownUntilThreshold(TH1F *dataHistogram, TH1F *mcHistogram_BG, const string& masses, const string& lepton, const string& inv_mass, const float& xsec, int dof, float threshold);
float scaleHistogramUpUntilThreshold2(TH1F *dataHistogram, TH1F *mcHistogram_BG, const string& masses, const string& lepton, const string& inv_mass, const float& xsec, int dof, float threshold);


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

	string dataFilePath = ("plots/data." + lepton + "_selected_plots.root").c_str();
	string mcFilePath   = "stackedPlots/analysis_" + inv_mass + ".root";

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
    vector<float> xsec = {
        110.0 , 
        82.0  , 
        20.0  , 
         5.5  , 
         1.9  ,  
         0.83 ,  
         0.3  ,    
         0.14 ,  
         0.067,  
         0.035,  
         0.012   
    };

	float mass[11] = {400.,500.,750.,1000.,1250.,1500.,1750.,2000.,2250.,2500.,3000};
	float expectedXsec[11] = {1.1e2, 8.2e1, 2.0e1, 5.5, 1.9, 8.3e-1, 3.0e-1, 1.4e-1, 6.7e-2, 3.5e-2, 1.2e-2};
	float limitXsec[11];

	for (size_t m = 0; m < masses.size(); m++){

        // für die Skalierung noch mal das Z' um es auf die MC ohne Z' zu stacken

   		// Get the Monte Carlo histogram from the file
	    TH1F* mcHistogram_BG   = static_cast<TH1F*>(mcFile->Get(("h_bg_" + masses[m] + "_" + lepton).c_str()));
	    TH1F* mcHistogram_BGZ  = static_cast<TH1F*>(mcFile->Get(("h_bgZ_" + masses[m] + "_" + lepton).c_str()));


    	if (!mcHistogram_BG or !mcHistogram_BGZ){
    	    cout << "Error retrieving Monte Carlo histogram from the file." << endl;
    	    return 1;
    	}

        // Background only
		float chiSquare = chisquareNBins(dataHistogram, mcHistogram_BG);
		int dof = dataHistogram->GetNbinsX() - 1;  // #Freiheitsgrade= #Bins - 1
    	float pValue =  TMath::Prob(chiSquare, dof);
		if (pValue < 2.7e-3 && pValue > 5.7e-7){
			cout << "Evidence for Zprime" << endl;
		}
		else if (pValue < 5.7e-7) {
			cout << "Observation of Zprime" << endl;
		}

        // Background + Z'
		chiSquare = chisquareNBins(dataHistogram, mcHistogram_BGZ);
    	pValue =  TMath::Prob(chiSquare, dof);
        cout << "pValue (unskaliert): " << pValue;

        // Funktionen zum auf und ab skalieren bis zum 0.95 Konfidenzlevel
        if(pValue>0.05){
            limitXsec[m] = xsec[m]*scaleHistogramUpUntilThreshold(dataHistogram, mcHistogram_BG, masses[m], lepton, inv_mass, xsec[m], dof, 0.05);
        }
        else if(pValue<0.05){
            limitXsec[m] = xsec[m]*scaleHistogramDownUntilThreshold(dataHistogram, mcHistogram_BG, masses[m], lepton, inv_mass, xsec[m], dof, 0.05);
        }
        cout << "mass: " << masses[m] << "\t limit: " << limitXsec[m] << endl;

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
    g_limits->SetLineColor(kBlack);

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

float scaleHistogramUpUntilThreshold(TH1F *dataHistogram, TH1F *mcHistogram_BG, const string& masses, const string& lepton, const string& inv_mass, const float& xsec, int dof, float threshold) {
    float pValue = 1.0;
    float scale = 0.01;
    float scaleIncrement = 0.0001;
    int loop_count = 0;

    while ((pValue > threshold) && (scale < 100)) {
        TFile *file_histogram_zprime = new TFile(("/net/e4-nfs-home.e4.physik.tu-dortmund.de/home/photon/Breitfeld_Knospe_26-04-23/plots/zprime" + masses + "." + lepton + "_selected_plots.root").c_str());
        TH1F *h_zprime = (TH1F *)file_histogram_zprime->Get(inv_mass.c_str());
        TDirectoryFile *dir_zprime = (TDirectoryFile *)file_histogram_zprime->Get("Histograms");
        h_zprime = (TH1F *)dir_zprime->Get(inv_mass.c_str());

        THStack *mcStack = new THStack("Stack", "Stacked MC");
        TH1F *mcHistogram_scaled = new TH1F("scaledBGZprime", "scaledBGZ", 1, 1, 1);
        mcHistogram_scaled->SetBins(dataHistogram->GetNbinsX(), dataHistogram->GetXaxis()->GetXmin(), dataHistogram->GetXaxis()->GetXmax());

        // scale
        h_zprime->Scale(scale * xsec);
        mcStack->Add(mcHistogram_BG);
        mcStack->Add(h_zprime);
        int nbins = dataHistogram->GetSize();
        for (int i = 1; i <= nbins; i++) {
            double sumContent = 0.0;
            for (int j = 0; j < mcStack->GetHists()->GetEntries(); j++) {
                TH1F *hist = dynamic_cast<TH1F *>(mcStack->GetHists()->At(j));
                sumContent += hist->GetBinContent(i);
            }
            mcHistogram_scaled->SetBinContent(i, sumContent);
        }
        float chiSquare = chisquareNBins(dataHistogram, mcHistogram_scaled);
        pValue = TMath::Prob(chiSquare, dof);

        scale += scaleIncrement;
        scaleIncrement *= 1.01;  // Erhöhe Inkrementgröße logarithmisch

        // Dateien schließen
        file_histogram_zprime->Close();
        delete file_histogram_zprime;

        loop_count++;
        if (loop_count == 10000) {
            cout << "Schleife vorzeitig abgebrochen" << endl;
            break;
        }
    }
    cout << "\t pValue (skaliert): " << pValue << endl;

    return scale;
}


float scaleHistogramDownUntilThreshold(TH1F *dataHistogram, TH1F *mcHistogram_BG, const string& masses, const string& lepton, const string& inv_mass, const float& xsec, int dof, float threshold) {
    float pValue = 0.0;
    float scale = 0.1;
    float scaleIncrement = 0.001;
    int loop_count = 0;

    while ((pValue < threshold) && (loop_count < 10000)) {
        TFile *file_histogram_zprime = new TFile(("/net/e4-nfs-home.e4.physik.tu-dortmund.de/home/photon/Breitfeld_Knospe_26-04-23/plots/zprime" + masses + "." + lepton + "_selected_plots.root").c_str());
        TH1F *h_zprime = (TH1F *)file_histogram_zprime->Get(inv_mass.c_str());
        TDirectoryFile *dir_zprime = (TDirectoryFile *)file_histogram_zprime->Get("Histograms");
        h_zprime = (TH1F *)dir_zprime->Get(inv_mass.c_str());

        THStack *mcStack = new THStack("Stack", "Stacked MC");
        TH1F *mcHistogram_scaled = new TH1F("scaledBGZprime", "scaledBGZ", 1, 1, 1);
        mcHistogram_scaled->SetBins(dataHistogram->GetNbinsX(), dataHistogram->GetXaxis()->GetXmin(), dataHistogram->GetXaxis()->GetXmax());

        // scale
        h_zprime->Scale(scale * xsec);
        mcStack->Add(mcHistogram_BG);
        mcStack->Add(h_zprime);
        int nbins = dataHistogram->GetSize();
        for (int i = 1; i <= nbins; i++) {
            double sumContent = 0.0;
            for (int j = 0; j < mcStack->GetHists()->GetEntries(); j++) {
                TH1F *hist = dynamic_cast<TH1F *>(mcStack->GetHists()->At(j));
                sumContent += hist->GetBinContent(i);
            }
            mcHistogram_scaled->SetBinContent(i, sumContent);
        }
        float chiSquare = chisquareNBins(dataHistogram, mcHistogram_scaled);
        pValue = TMath::Prob(chiSquare, dof);

        scale -= scaleIncrement;
        scaleIncrement *= 0.99;  // Verringere Inkrementgröße logarithmisch

        // Dateien schließen
        file_histogram_zprime->Close();
        delete file_histogram_zprime;

        if (pValue > threshold) {
            break;
        }
        loop_count++;

        if (loop_count == 1000) {
            cout << "Schleife vorzeitig abgebrochen, scale: " << scale << endl;
            scaleHistogramUpUntilThreshold2(dataHistogram, mcHistogram_BG, masses, lepton, inv_mass, xsec, dof, 0.05);
            break;
        }

    }
    cout << "\t pValue (skaliert): " << pValue << endl;

    return scale;
}

float scaleHistogramUpUntilThreshold2(TH1F *dataHistogram, TH1F *mcHistogram_BG, const string& masses, const string& lepton, const string& inv_mass, const float& xsec, int dof, float threshold) {
    cout << "gehe in scaleHistogramUpUntilThreshold2" << endl;
    float pValue = 1.0;
    float scale = 0.01;
    float scaleIncrement = 0.0001;
    int loop_count = 0;

    while ((pValue < threshold) && (scale < 100)) {
        TFile *file_histogram_zprime = new TFile(("/net/e4-nfs-home.e4.physik.tu-dortmund.de/home/photon/Breitfeld_Knospe_26-04-23/plots/zprime" + masses + "." + lepton + "_selected_plots.root").c_str());
        TH1F *h_zprime = (TH1F *)file_histogram_zprime->Get(inv_mass.c_str());
        TDirectoryFile *dir_zprime = (TDirectoryFile *)file_histogram_zprime->Get("Histograms");
        h_zprime = (TH1F *)dir_zprime->Get(inv_mass.c_str());

        THStack *mcStack = new THStack("Stack", "Stacked MC");
        TH1F *mcHistogram_scaled = new TH1F("scaledBGZprime", "scaledBGZ", 1, 1, 1);
        mcHistogram_scaled->SetBins(dataHistogram->GetNbinsX(), dataHistogram->GetXaxis()->GetXmin(), dataHistogram->GetXaxis()->GetXmax());

        // scale
        h_zprime->Scale(scale * xsec);
        mcStack->Add(mcHistogram_BG);
        mcStack->Add(h_zprime);
        int nbins = dataHistogram->GetSize();
        for (int i = 1; i <= nbins; i++) {
            double sumContent = 0.0;
            for (int j = 0; j < mcStack->GetHists()->GetEntries(); j++) {
                TH1F *hist = dynamic_cast<TH1F *>(mcStack->GetHists()->At(j));
                sumContent += hist->GetBinContent(i);
            }
            mcHistogram_scaled->SetBinContent(i, sumContent);
        }
        float chiSquare = chisquareNBins(dataHistogram, mcHistogram_scaled);
        pValue = TMath::Prob(chiSquare, dof);

        scale += scaleIncrement;
        scaleIncrement *= 1.01;  // Erhöhe Inkrementgröße logarithmisch

        // Dateien schließen
        file_histogram_zprime->Close();
        delete file_histogram_zprime;

        loop_count++;
        if (loop_count == 10000) {
            cout << "Schleife vorzeitig abgebrochen" << endl;
            break;
        }
    }
    cout << "\t pValue (skaliert): " << pValue <<endl;

    return scale;
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
