// Plots muon PE response
// the graph of energy of muon beam vs mean PE hit distribution from qsim

#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include "TROOT.h"
#include "TStyle.h"
#include "TGaxis.h"
#include "TFile.h"
#include "TH1I.h"
#include "TTree.h"
#include "TChain.h"
#include "TCanvas.h"
#include "TPaveStats.h"
#include "TSystem.h"
#include "TGraphErrors.h"
#include "TMultiGraph.h"
#include "TF1.h"
#include "TLegend.h"

using namespace std;

void plot_graph_eVsMeanHit_mu(){
    //gROOT->Reset();
    //gStyle->SetOptStat("neMR");
    //gStyle->SetTitleYOffset(1.3);
    //gStyle->SetPadGridX(1);
    //gStyle->SetPadGridY(1);
    gStyle->SetOptFit();
    //TGaxis::SetMaxDigits(3);
    
    string config = "qsim_18";
    string particle = "mu";
    string geometry = "smFullscaleQsim";
    int color[] = {kBlack, kRed, kBlue, kGreen+2, kMagenta,};
    string fitFn = "pol3";

    vector<float> beamEnergy;
    vector<float> meanHits;
    vector<float> meanHitsError;
    vector<float> rms;
    vector<float> peakHit;
    vector<float> detRes;
    vector<string> inRootFileName;
    

    //string inFileDir = Form("/volatile/halla/moller12gev/sudip/qsim_rootfiles/%s/",config.c_str());
    string inFileDir = "/run/user/1000/gvfs/sftp:host=sudips-mbp.local/Users/sudip/test/qsim_rootfiles/qsim_18/";

    for (float iBeamEnergy=1; iBeamEnergy<=12; iBeamEnergy+=2){
        beamEnergy.push_back(iBeamEnergy);
    }

    TH1F *h_hitn[beamEnergy.size()];

    ofstream outfile;
    outfile.open("./output_files/eDist_muon.csv");
    outfile << "Energy,Mean,RMS,Res" << endl; 

    for (int i=0; i<beamEnergy.size(); i++){
        inRootFileName.push_back(Form("qsim_out_%.0fGeV_%s_10k_*.root", beamEnergy.at(i), geometry.c_str()));
        string inRootFile = inFileDir + inRootFileName.at(i);

        TChain * T = new TChain("T", "Chain");
        T->Add(inRootFile.c_str());
        //TFile *inFile = new TFile(inRootFile.c_str(), "READ");
        //TTree *T = (TTree*)inFile->Get("T");
        T->Draw(Form("hit.n>>hitn_%.0fGeV", beamEnergy.at(i)), "hit.pid==0", "goff");

        h_hitn[i] = (TH1F*)gDirectory->FindObject(Form("hitn_%.0fGeV", beamEnergy.at(i)));
        
        meanHits.push_back(h_hitn[i]->GetEntries()/20000);
        //meanHitsError.push_back(h_hitn[i]->GetMeanError());

        //peakHit.push_back(h_hitn[i]->GetMaximum());
        
        float rms = h_hitn[i]->GetRMS();

        detRes.push_back(rms/meanHits[i]);

        outfile << beamEnergy.at(i) << "," << meanHits[i] << "," << rms << "," << detRes[i] << endl;

        //cout << meanHits[i] << " " << peakHit[i] << endl;
        //cout << beamEnergy[i] << " " << detRes[i] << endl;

    }
    outfile.close();
/*
    // Draw beam energy vs mean PE
    TCanvas* c1 = new TCanvas("c1", "Energy vs mean PE");
    TGraphErrors *grMean = new TGraphErrors(beamEnergy.size(), &beamEnergy[0], &meanHits[0], 0, &meanHitsError[0]);
    grMean->SetTitle("Mean PE yield; Energy of e beam [GeV]; PE yield");
    grMean->SetMarkerStyle(20);
    grMean->SetTitle("Mean PE");
    grMean->SetDrawOption("AP");
    grMean->SetMarkerColor(kRed);
    grMean->Draw("AP");
    grMean->Fit(fitFn.c_str());

    TF1* fit = (TF1*)grMean->GetFunction(fitFn.c_str());
    fit->SetLineColor(kBlue);
    fit->Print();

    //cout << fit->Eval(0.855) << endl;

    TLegend *leg = new TLegend(0.1,0.8,0.4,0.9);
    leg->SetTextSize(0.04);
    leg->AddEntry(grMean, "Data points");
    leg->AddEntry(fit, "Pol2 best fit");
    leg->Draw();

    gPad->Update();
    TPaveStats *stat = (TPaveStats*)grMean->FindObject("stats");
    stat->SetX1NDC(0.1);
    stat->SetY1NDC(0.6);
    stat->SetX2NDC(0.4);
    stat->SetY2NDC(0.8);
    stat->Draw();

    //Save canvas
    gSystem->Exec(Form("mkdir -p plots/%s/",config.c_str()));
    c1->SaveAs(Form("./plots/%s/eVsMeanHit_%s_%s.pdf",config.c_str(), geometry.c_str(),particle.c_str()));

    // Draw beam energy vs peak PE
    //TCanvas* c2 = new TCanvas("c2", "Energy vs peak PE");
    //TGraph *grPeak = new TGraph(beamEnergy.size(), &beamEnergy[0], &peakHit[0]);
    //grPeak->SetMarkerStyle(30);
    //grPeak->SetTitle("Peak PE");
    //grPeak->SetDrawOption("AP");
    //grPeak->SetMarkerColor(kBlue);
    //grPeak->Draw();
    
    //// Draw beam energy vs det resolution
    //TCanvas* c3 = new TCanvas("c3", "Energy vs det resolution");
    //TGraph *grRes = new TGraph(beamEnergy.size(), &beamEnergy[0], &detRes[0]);
    //grRes->SetTitle("Electron beam energy vs det resolution; Beam energy [in GeV]; RMS/Mean");
    //grRes->Draw();

    //// Draw multigraph of beam energy vs mean and peak PE
    //TCanvas* c4 = new TCanvas("c4", "energy vs PE");
    //TMultiGraph *mg = new TMultiGraph("mg", "mg");
    //mg->SetTitle("Energy vs PE; ELectron beam energy [GeV]; PE count");
    //mg->Add(grMean, "lp");
    //mg->Add(grPeak, "cp");
    //mg->Draw("ap");
*/
}