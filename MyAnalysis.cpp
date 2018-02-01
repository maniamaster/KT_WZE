#include "MyAnalysis.h"

#include <iostream>
#include <string>
#include <cmath>

using std::cout;
using std::endl;
using std::string;
#include "TROOT.h" // ROOT classes used in this file.
#include "TCanvas.h" // Add other includes here if you use more.
#include "TFile.h"   
#include "TAxis.h"
#include "TStyle.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TF1.h"
#include "TMinuit.h"
#include "TColor.h"

bool trigger=true;
bool cut=true;

	

// This is an example class to get you going. It creates and
// fills two histograms with the transverse momentum of the
// two highest Pt muons in an event. These muons are pointed to in the
// data by index1 and index2 respectively, which are members
// of the Data class. These muons have some loose
// quality cuts applied, namely they must have a matched central track
// and be of at least medium quality.

MyAnalysis::MyAnalysis() : _numEvents(0), _eventsWithAtLeast2Muons(0),
    _hist_muon1_pt(0),
    _hist_muon2_pt(0),
    _hist_muon1_eta(0),
    _hist_muon2_eta(0),
    _hist_muon1_phi(0),
    _hist_muon2_phi(0),
    _hist_time_diff(0),
	_hist_Zmass(0)


{
    // Class constructor. Use it to initialise member variables,
    // not to create objects. Primitive types can be initialised in the
    // order they are declared in the class header file as above, more
    // complicated initialisations can be done in the body of the function.
}

MyAnalysis::~MyAnalysis()
{
    // Class destructor. If you have created any objects on the heap (with
    // the new operator) that are not ROOT objects, clean them up here if
    // you haven't already done it in End().
}

void MyAnalysis::Setup()
{
    // Called before the first event is processed. Create your
    // hitogram objects etc. here. Remember to add the appropriate
    // #include files in the class header or this file if you use
    // other ROOT classes.

	cout << "Setup called" << endl;
	if (_dataType == RealData)
		cout << "Running on data" << endl;
	else if (_dataType == ZMonteCarlo)
		cout << "Running on Z MC" << endl;
    else if (_dataType == WMonteCarlo)
        cout << "Running on W MC" << endl;
	else
		cout << "Unknown data format..." << endl;

	
	if (_dataType == ZMonteCarlo) trigger=false;

    // Set the graphics style
    gROOT->SetStyle("Plain");
    gStyle->SetOptFit();
    gStyle->SetTitleBorderSize(0);

    // Create histograms here (remember to declare the pointers in
    // the class header file).
    // Syntax: new TH1F( Name, Title, bins, min_value, max_value )
    // Don't forget to label the axes!
    _hist_muon1_pt = new TH1F( "m1_P_t", "First Muon P_{t}", 100, 0, 100 );
    if (_hist_muon1_pt != (TH1F *)0) // Check it got created properly
    {
        _hist_muon1_pt->GetXaxis()->SetTitle("P_{t} (GeV/c)");
        _hist_muon1_pt->GetYaxis()->SetTitle("Entries");
    }

    _hist_muon2_pt = new TH1F( "m2_P_t", "Second Muon P_{t}", 100, 0, 100 );
    if (_hist_muon2_pt != (TH1F *)0)
    {
        _hist_muon2_pt->GetXaxis()->SetTitle("P_{t} (GeV/c)");
        _hist_muon2_pt->GetYaxis()->SetTitle("Entries");
    }

    _hist_muon1_eta = new TH1F( "m1_eta", "First Muon eta", 100, -5, 5 );
    if (_hist_muon1_eta != (TH1F *)0) // Check it got created properly
    {
        _hist_muon1_eta->GetXaxis()->SetTitle("eta ");
        _hist_muon1_eta->GetYaxis()->SetTitle("Entries");
    }

    _hist_muon2_eta = new TH1F( "m2_eta", "Second Muon eta", 100, -5, 5);
    if (_hist_muon2_eta != (TH1F *)0)
    {
        _hist_muon2_eta->GetXaxis()->SetTitle("eta");
        _hist_muon2_eta->GetYaxis()->SetTitle("Entries");
    }

    _hist_eta_diff = new TH1F( "DeltaEta", "Delta eta", 100, 0, 7);
    if (_hist_eta_diff != (TH1F *)0)
    {
        _hist_eta_diff->GetXaxis()->SetTitle("Delta eta");
        _hist_eta_diff->GetYaxis()->SetTitle("Entries");
    }

    _hist_muon1_phi = new TH1F( "m1_phi", "First Muon phi", 100, -5, 5 );
    if (_hist_muon1_phi != (TH1F *)0) // Check it got created properly
    {
        _hist_muon1_phi->GetXaxis()->SetTitle("phi ");
        _hist_muon1_phi->GetYaxis()->SetTitle("Entries");
    }

    _hist_muon2_phi = new TH1F( "m2_phi", "Second phi", 100, -5, 5);
    if (_hist_muon2_phi != (TH1F *)0)
    {
        _hist_muon2_phi->GetXaxis()->SetTitle("phi");
        _hist_muon2_phi->GetYaxis()->SetTitle("Entries");
    }

    _hist_phi_diff = new TH1F( "deltaPhi", "Delta phi", 100, 0, 5);
    if (_hist_phi_diff != (TH1F *)0)
    {
        _hist_phi_diff->GetXaxis()->SetTitle("Delta phi");
        _hist_phi_diff->GetYaxis()->SetTitle("Entries");
    }


    _hist_time_diff = new TH1F( "time_diff", "timing difference", 100, -200, 200);
    if (_hist_time_diff != (TH1F *)0)
    {
        _hist_time_diff->GetXaxis()->SetTitle("time diff");
        _hist_time_diff->GetYaxis()->SetTitle("Entries");
    }

	_hist_Zmass = new TH1F( "inv_mass", "invariant Z mass", 50, 0, 200);
    if (_hist_Zmass != (TH1F *)0)
    {
        _hist_Zmass->GetXaxis()->SetTitle("Z mass [GeV]");
        _hist_Zmass->GetYaxis()->SetTitle("Entries");
    }

	_hist_nmu = new TH1F( "nmu", "number of muons", 5, -0.5, 4.5);
    if (_hist_nmu != (TH1F *)0)
    {
        _hist_nmu->GetXaxis()->SetTitle("number of muons");
        _hist_nmu->GetYaxis()->SetTitle("Entries");
    }
	_hist_pt5_1 = new TH1F( "pt5", "pt_5", 100, 0, 4);
    if (_hist_pt5_1 != (TH1F *)0)
    {
        _hist_pt5_1->GetXaxis()->SetTitle("pt_5 [GeV]");
        _hist_pt5_1->GetYaxis()->SetTitle("Entries");
    }

	_hist_ehalo_1 = new TH1F( "ehalo", "ehalo", 100, 0, 4);
    if (_hist_ehalo_1 != (TH1F *)0)
    {
        _hist_ehalo_1->GetXaxis()->SetTitle("ehalo [GeV]");
        _hist_ehalo_1->GetYaxis()->SetTitle("Entries");
    }

	_hist_ehalo_2 = new TH1F( "ehalo", "ehalo", 100, 0, 4);
    if (_hist_ehalo_2 != (TH1F *)0)
    {
        _hist_ehalo_2->GetXaxis()->SetTitle("ehalo [GeV]");
        _hist_ehalo_2->GetYaxis()->SetTitle("Entries");
    }

	_hist_eta_phi = new TH2F( "eta_phi", "eta phi", 100, 0, 4, 100, 0, 4);
    if (_hist_eta_phi != (TH2F *)0)
    {
        _hist_eta_phi->GetXaxis()->SetTitle("eta");
        _hist_eta_phi->GetYaxis()->SetTitle("phi");
    }

	_hist_chisqdof1 = new TH1F( "chisqdof1", "chisqdof 1", 100, 0, 5);
    if (_hist_chisqdof1 != (TH1F *)0)
    {
        _hist_chisqdof1->GetXaxis()->SetTitle("chisqdof1");
        _hist_chisqdof1->GetYaxis()->SetTitle("Entries");
    }

	_hist_chisqdof2 = new TH1F( "chisqdof2", "chisqdof 2", 100, 0, 5);
    if (_hist_chisqdof2 != (TH1F *)0)
    {
        _hist_chisqdof2->GetXaxis()->SetTitle("chisqdof2");
        _hist_chisqdof2->GetYaxis()->SetTitle("Entries");
    }
	//REJECTED:
    _hist_rj_muon1_pt = new TH1F( "m1_P_t", "First Muon P_{t}", 100, 0, 100 );
    if (_hist_rj_muon1_pt != (TH1F *)0) // Check it got created properly
    {
        _hist_rj_muon1_pt->GetXaxis()->SetTitle("P_{t} (GeV/c)");
        _hist_rj_muon1_pt->GetYaxis()->SetTitle("Entries");
        _hist_rj_muon1_pt->SetLineColor(2);
    }

    _hist_rj_muon2_pt = new TH1F( "m2_P_t", "Second Muon P_{t}", 100, 0, 100 );
    if (_hist_rj_muon2_pt != (TH1F *)0)
    {
        _hist_rj_muon2_pt->GetXaxis()->SetTitle("P_{t} (GeV/c)");
        _hist_rj_muon2_pt->GetYaxis()->SetTitle("Entries");
        _hist_rj_muon2_pt->SetLineColor(2);
    }

    _hist_rj_muon1_eta = new TH1F( "m1_eta", "First Muon eta", 100, -5, 5 );
    if (_hist_rj_muon1_eta != (TH1F *)0) // Check it got created properly
    {
        _hist_rj_muon1_eta->GetXaxis()->SetTitle("eta ");
        _hist_rj_muon1_eta->GetYaxis()->SetTitle("Entries");
        _hist_rj_muon1_eta->SetLineColor(2);
    }

    _hist_rj_muon2_eta = new TH1F( "m2_eta", "Second Muon eta", 100, -5, 5);
    if (_hist_rj_muon2_eta != (TH1F *)0)
    {
        _hist_rj_muon2_eta->GetXaxis()->SetTitle("eta");
        _hist_rj_muon2_eta->GetYaxis()->SetTitle("Entries");
        _hist_rj_muon2_eta->SetLineColor(2);
    }

    _hist_rj_eta_diff = new TH1F( "DeltaEta", "Delta eta", 100, 0, 7);
    if (_hist_rj_eta_diff != (TH1F *)0)
    {
        _hist_rj_eta_diff->GetXaxis()->SetTitle("Delta eta");
        _hist_rj_eta_diff->GetYaxis()->SetTitle("Entries");
        _hist_rj_eta_diff->SetLineColor(2);
    }

    _hist_rj_muon1_phi = new TH1F( "m1_phi", "First Muon phi", 100, -5, 5 );
    if (_hist_rj_muon1_phi != (TH1F *)0) // Check it got created properly
    {
        _hist_rj_muon1_phi->GetXaxis()->SetTitle("phi ");
        _hist_rj_muon1_phi->GetYaxis()->SetTitle("Entries");
        _hist_rj_muon1_phi->SetLineColor(2);
    }

    _hist_rj_muon2_phi = new TH1F( "m2_phi", "Second phi", 100, -5, 5);
    if (_hist_rj_muon2_phi != (TH1F *)0)
    {
        _hist_rj_muon2_phi->GetXaxis()->SetTitle("phi");
        _hist_rj_muon2_phi->GetYaxis()->SetTitle("Entries");
        _hist_rj_muon2_phi->SetLineColor(2);
    }

    _hist_rj_phi_diff = new TH1F( "deltaPhi", "Delta phi", 100, 0, 5);
    if (_hist_rj_phi_diff != (TH1F *)0)
    {
        _hist_rj_phi_diff->GetXaxis()->SetTitle("Delta phi");
        _hist_rj_phi_diff->GetYaxis()->SetTitle("Entries");
        _hist_rj_phi_diff->SetLineColor(2);
    }


    _hist_rj_time_diff = new TH1F( "time_diff", "timing difference", 100, -200, 200);
    if (_hist_rj_time_diff != (TH1F *)0)
    {
        _hist_rj_time_diff->GetXaxis()->SetTitle("time diff");
        _hist_rj_time_diff->GetYaxis()->SetTitle("Entries");
        _hist_rj_time_diff->SetLineColor(2);
    }

	_hist_rj_Zmass = new TH1F( "inv_mass", "invariant Z mass", 50, 0, 200);
    if (_hist_rj_Zmass != (TH1F *)0)
    {
        _hist_rj_Zmass->GetXaxis()->SetTitle("Z mass [GeV]");
        _hist_rj_Zmass->GetYaxis()->SetTitle("Entries");
        _hist_rj_Zmass->SetLineColor(2);
    }

	_hist_rj_nmu = new TH1F( "nmu", "number of muons", 5, -0.5, 4.5);
    if (_hist_rj_nmu != (TH1F *)0)
    {
        _hist_rj_nmu->GetXaxis()->SetTitle("number of muons");
        _hist_rj_nmu->GetYaxis()->SetTitle("Entries");
        _hist_rj_nmu->SetLineColor(2);
    }
	_hist_rj_pt5_1 = new TH1F( "pt5", "pt_5", 100, 0, 4);
    if (_hist_rj_pt5_1 != (TH1F *)0)
    {
        _hist_rj_pt5_1->GetXaxis()->SetTitle("pt_5 [GeV]");
        _hist_rj_pt5_1->GetYaxis()->SetTitle("Entries");
        _hist_rj_pt5_1->SetLineColor(2);    
}

	_hist_rj_ehalo_1 = new TH1F( "ehalo", "ehalo", 100, 0, 4);
    if (_hist_rj_ehalo_1 != (TH1F *)0)
    {
        _hist_rj_ehalo_1->GetXaxis()->SetTitle("ehalo [GeV]");
        _hist_rj_ehalo_1->GetYaxis()->SetTitle("Entries");
        _hist_rj_ehalo_1->SetLineColor(2); 
    }

	_hist_rj_ehalo_2 = new TH1F( "ehalo", "ehalo", 100, 0, 4);
    if (_hist_rj_ehalo_2 != (TH1F *)0)
    {
        _hist_rj_ehalo_2->GetXaxis()->SetTitle("ehalo [GeV]");
        _hist_rj_ehalo_2->GetYaxis()->SetTitle("Entries");
        _hist_rj_ehalo_2->SetLineColor(2);    
	}

	_hist_rj_eta_phi = new TH2F( "eta_phi", "eta phi", 100, 0, 4, 100, 0, 4);
    if (_hist_rj_eta_phi != (TH2F *)0)
    {
        _hist_rj_eta_phi->GetXaxis()->SetTitle("eta");
        _hist_rj_eta_phi->GetYaxis()->SetTitle("phi");
        _hist_rj_eta_phi->SetLineColor(2);
    }

	_hist_rj_chisqdof1 = new TH1F( "chisqdof1", "chisqdof 1", 100, 0, 5);
    if (_hist_rj_chisqdof1 != (TH1F *)0)
    {
        _hist_rj_chisqdof1->GetXaxis()->SetTitle("chisqdof1");
        _hist_rj_chisqdof1->GetYaxis()->SetTitle("Entries");
        _hist_rj_chisqdof1->SetLineColor(2);
    }

	_hist_rj_chisqdof2 = new TH1F( "chisqdof2", "chisqdof 2", 100, 0, 5);
    if (_hist_rj_chisqdof2 != (TH1F *)0)
    {
        _hist_rj_chisqdof2->GetXaxis()->SetTitle("chisqdof2");
        _hist_rj_chisqdof2->GetYaxis()->SetTitle("Entries");
        _hist_rj_chisqdof2->SetLineColor(2);
    }
}



void MyAnalysis::Event(const Data &theData)
{
    // Called every event with new data stored in theData.
    // theData is a reference to a Data object. Don't try to
    // change it as it is provided for you to use. The variables
    // contained in the Data object are declared in Data.h

    // Remember that you can add other functions to this class to 
    // carry out specific tasks. To do that, declare them in the
    // class header file and implement them in this file. Use the
    // existing example function as a guide.
	TLorentzVector mu1;
	TLorentzVector mu2;
	TLorentzVector Z;

    _numEvents++; // Add 1 to the event counter

    // Fill histograms
    //if (theData.index1 != -1 && theData.index2 != -1) {// index1 >= 0 for a real muon

        // To access a quantity stored in the data file
        // you need to use the name of the variable and prefix it
        // with the name of the Data object that it's stored in.
        // For example, to access the number of good muons in the
        // event, which is stored in the variable MU_Nmu,
        // you would use theData.MU_Nmu. In the example below
        // we want the transverse momentum of the highest Pt
        // muon in the event. To help you, the variables
        // index1 and index2 are already set to point to the
        // entry in the muons array that contains the highest
        // and second highest Pt good muons. index1 and index2
        // are set automatically, you don't need to try to set them
        // yourself. They are set to -1 if no such muon exists.
		mu1.SetPtEtaPhiM(theData.MU_pt[theData.index1],theData.MU_eta[theData.index1],theData.MU_phi[theData.index1],0.105);
		mu2.SetPtEtaPhiM(theData.MU_pt[theData.index2],theData.MU_eta[theData.index2],theData.MU_phi[theData.index2],0.105);
		Z=mu1+mu2;
        //_hist_muon1_pt->Fill( theData.MU_pt[theData.index1] );

		//Mit cuts:
		if (cut==true){
			if (((theData.MU_q[theData.index1]==1 && theData.MU_q[theData.index2]==-1)||(theData.MU_q[theData.index1]==-1 && theData.MU_q[theData.index2]==+1)) && ((trigger==false)||(theData.TRIG_MUW_W_L2M3_TRK10==1)) && (abs(theData.MU_timeA[theData.index1]-theData.MU_timeA[theData.index2])<10) && (theData.MU_pt[theData.index2]>15) && (theData.MU_pt[theData.index1]>20) && (theData.MU_chisqdof[theData.index1] < 2) && (theData.MU_chisqdof[theData.index2] < 2) && (theData.MU_ehalo[theData.index1] < 1.5) && (theData.MU_ehalo[theData.index2] < 1.5)&&(theData.index1 != -1 && theData.index2 != -1)){
				_hist_muon1_pt->Fill( theData.MU_pt[theData.index1]);
				_hist_muon2_pt->Fill( theData.MU_pt[theData.index2]);
				_hist_muon1_eta->Fill( theData.MU_eta[theData.index1]);
				_hist_muon2_eta->Fill( theData.MU_eta[theData.index2]);
				_hist_muon1_phi->Fill( theData.MU_phi[theData.index1]);
				_hist_muon2_phi->Fill( theData.MU_phi[theData.index2]);
				_hist_eta_diff->Fill(abs(theData.MU_eta[theData.index1]-theData.MU_eta[theData.index2]));
				double deltaphi=abs(theData.MU_phi[theData.index1]-theData.MU_phi[theData.index2]);
				if (deltaphi>=M_PI) deltaphi=2*M_PI-deltaphi;
				_hist_phi_diff->Fill(deltaphi);
				_hist_nmu->Fill( theData.MU_Nmu);
				_hist_pt5_1->Fill( theData.MU_pt_5[theData.index1]);
				_hist_ehalo_1->Fill( theData.MU_ehalo[theData.index1]);
				_hist_ehalo_2->Fill( theData.MU_ehalo[theData.index2]);
				_hist_Zmass->Fill(Z.M());
				_hist_time_diff->Fill( theData.MU_timeA[theData.index1]-theData.MU_timeA[theData.index2]);
				_hist_chisqdof1->Fill( theData.MU_chisqdof[theData.index1]);
				_hist_chisqdof2->Fill( theData.MU_chisqdof[theData.index2]);
				_hist_eta_phi->Fill(abs(theData.MU_eta[theData.index1]-theData.MU_eta[theData.index2]),deltaphi);
			}			
			else {
				//rejected:
				_hist_rj_muon1_pt->Fill( theData.MU_pt[theData.index1]);
				_hist_rj_muon2_pt->Fill( theData.MU_pt[theData.index2]);
				_hist_rj_muon1_eta->Fill( theData.MU_eta[theData.index1]);
				_hist_rj_muon2_eta->Fill( theData.MU_eta[theData.index2]);
				_hist_rj_muon1_phi->Fill( theData.MU_phi[theData.index1]);
				_hist_rj_muon2_phi->Fill( theData.MU_phi[theData.index2]);
				_hist_rj_eta_diff->Fill(abs(theData.MU_eta[theData.index1]-theData.MU_eta[theData.index2]));
				double deltaphi=abs(theData.MU_phi[theData.index1]-theData.MU_phi[theData.index2]);
				if (deltaphi>=M_PI) deltaphi=2*M_PI-deltaphi;
				_hist_rj_phi_diff->Fill(deltaphi);
				_hist_rj_nmu->Fill( theData.MU_Nmu);
				_hist_rj_pt5_1->Fill( theData.MU_pt_5[theData.index1]);
				_hist_rj_ehalo_1->Fill( theData.MU_ehalo[theData.index1]);
				_hist_rj_ehalo_2->Fill( theData.MU_ehalo[theData.index2]);
				_hist_rj_Zmass->Fill(Z.M());
				_hist_rj_time_diff->Fill( theData.MU_timeA[theData.index1]-theData.MU_timeA[theData.index2]);
				_hist_rj_chisqdof1->Fill( theData.MU_chisqdof[theData.index1]);
				_hist_rj_chisqdof2->Fill( theData.MU_chisqdof[theData.index2]);
				_hist_rj_eta_phi->Fill(abs(theData.MU_eta[theData.index1]-theData.MU_eta[theData.index2]),deltaphi);
			}
	}
	else{	
	//no cuts but trigger:
		if((trigger==false)||(theData.TRIG_MUW_W_L2M3_TRK10==1)){			
			_hist_muon1_pt->Fill( theData.MU_pt[theData.index1]);
			_hist_muon2_pt->Fill( theData.MU_pt[theData.index2]);
			_hist_muon1_eta->Fill( theData.MU_eta[theData.index1]);
			_hist_muon2_eta->Fill( theData.MU_eta[theData.index2]);
			_hist_muon1_phi->Fill( theData.MU_phi[theData.index1]);
			_hist_muon2_phi->Fill( theData.MU_phi[theData.index2]);
			_hist_eta_diff->Fill(abs(theData.MU_eta[theData.index1]-theData.MU_eta[theData.index2]));
			double deltaphi=abs(theData.MU_phi[theData.index1]-theData.MU_phi[theData.index2]);
			if (deltaphi>=M_PI) deltaphi=2*M_PI-deltaphi;
			_hist_phi_diff->Fill(deltaphi);
			_hist_nmu->Fill( theData.MU_Nmu);
			_hist_pt5_1->Fill( theData.MU_pt_5[theData.index1]);
			_hist_ehalo_1->Fill( theData.MU_ehalo[theData.index1]);
			_hist_ehalo_2->Fill( theData.MU_ehalo[theData.index2]);
			_hist_Zmass->Fill(Z.M());
			_hist_time_diff->Fill( theData.MU_timeA[theData.index1]-theData.MU_timeA[theData.index2]);
			_hist_chisqdof1->Fill( theData.MU_ehalo[theData.index1]);
			_hist_chisqdof2->Fill( theData.MU_ehalo[theData.index2]);
			_hist_eta_phi->Fill(abs(theData.MU_eta[theData.index1]-theData.MU_eta[theData.index2]),deltaphi);
		}
	}
		
   // }
}

void MyAnalysis::End()
{
    // Called after the last event has been processed. Do your
    // post processing analysis here.

	std::cout << "End called" << std::endl;

	string name ="";
	if (_dataType == RealData)
		name = name + "data_";
	if (_dataType == ZMonteCarlo)
		name = name + "zmc_";
	if (cut==true)
		name = name + "cut_";
	if (cut==false)
		name = name + "uncut_";

    // Draw histograms: Make a canvas to draw them on and set the background colour to white
	if (cut==false){
    TCanvas *canvas_Zmass = new TCanvas( "zmass", "Zmass" );
	_hist_Zmass->Draw();
	     TF1 *MyFit = new TF1("Breit Wigner","[0]*(((x*x-[1]*[1])^2)+([1]*[1]*[2]*[2]))^(-1)", 30,150);
	      MyFit->SetParameter(0, 1);
	      MyFit->SetParameter(1, 100);
	      MyFit->SetParameter(2, 3);
	      _hist_Zmass->Fit(MyFit,"R");
    TCanvas *canvas_muon1_pt = new TCanvas( "m1pts", "muon1_pt" );
    _hist_muon1_pt->Draw(); // Draw histogram
    TCanvas *canvas_muon2_pt = new TCanvas( "m2pts", "muon2_pt" );
    _hist_muon2_pt->Draw(); // Draw histogram
    TCanvas *canvas_eta_diff = new TCanvas( "etadiff", "eta_diff" );
    _hist_eta_diff->Draw(); // Draw histogram
    TCanvas *canvas_phi_diff = new TCanvas( "phidiff", "phi_diff" );
    _hist_phi_diff->Draw(); // Draw histogram
    TCanvas *canvas_eta_phi= new TCanvas( "etaphi", "eta_phi_diff" );
	_hist_eta_phi->Draw();
    TCanvas *canvas_ehalo_1 = new TCanvas( "ehalo1", "ehalo_1" );
	_hist_ehalo_1->Draw();
    TCanvas *canvas_ehalo_2 = new TCanvas( "ehalo2", "ehalo_2" );
	_hist_ehalo_2->Draw();
    TCanvas *canvas_time_diff = new TCanvas( "timediff", "time_diff" );
	_hist_time_diff->Draw();
    TCanvas *canvas_chisqdof1 = new TCanvas( "chisqdof1", "chisqdof1" );
	_hist_chisqdof1->Draw();
    TCanvas *canvas_chisqdof2 = new TCanvas( "chisqdof2", "chisqdof2" );
	_hist_chisqdof2->Draw();
	
    canvas_muon1_pt->SaveAs((name+"muonpt_1.pdf").c_str());
    canvas_muon2_pt->SaveAs((name+"muonpt_2.pdf").c_str());
    canvas_eta_diff->SaveAs((name+"etadiff.pdf").c_str());
    canvas_phi_diff->SaveAs((name+"phidiff.pdf").c_str());
    canvas_eta_phi->SaveAs((name+"etaphi.pdf").c_str());
    canvas_ehalo_1->SaveAs((name+"ehalo1.pdf").c_str());
    canvas_ehalo_2->SaveAs((name+"ehalo2.pdf").c_str());
    canvas_time_diff->SaveAs((name+"timediff.pdf").c_str());
    canvas_chisqdof1->SaveAs((name+"chisqdof1.pdf").c_str());
    canvas_chisqdof2->SaveAs((name+"chisqdof2.pdf").c_str());
    canvas_Zmass->SaveAs((name+"zmass.pdf").c_str());


     ////////////////////////////////////
     // Fit the second histogram
     // *TF1: Fit function class
     //   1st arg: name
     //   2nd arg: function, [0], [1], etc: parameters to be varied during the procedure
     //   3rd and 4th arg: fit range
     // *histogram->Fit:
     //   1st argiment: fit function
     //   2nd argument: option, e,g,: R: use range of fit function
     ///////////////////////////////////
	}
	else{
	//CUT:
		
	    TCanvas *canvas_Zmass = new TCanvas( "zmass", "Zmass" );
		canvas_Zmass->Divide(2,1);
		canvas_Zmass->cd(1);
	     TF1 *MyFit = new TF1("Breit Wigner","[0]*(((x*x-[1]*[1])^2)+([1]*[1]*[2]*[2]))^(-1)", 30,150);
	      MyFit->SetParameter(0, 1);
	      MyFit->SetParameter(1, 100);
	      MyFit->SetParameter(2, 3);
	      _hist_Zmass->Fit(MyFit,"R");
		_hist_Zmass->Draw();
		canvas_Zmass->cd(2);
		_hist_rj_Zmass->Draw();
	    TCanvas *canvas_muon1_pt = new TCanvas( "m1pts", "muon1_pt" );
		canvas_muon1_pt->Divide(2,1);
		canvas_muon1_pt->cd(1);
	    _hist_muon1_pt->Draw(); // Draw histogram
		canvas_muon1_pt->cd(2);
	    _hist_rj_muon1_pt->Draw(); // Draw histogram
	    TCanvas *canvas_muon2_pt = new TCanvas( "m2pts", "muon2_pt" );
		canvas_muon2_pt->Divide(2,1);
		canvas_muon2_pt->cd(1);
	    _hist_muon2_pt->Draw(); // Draw histogram
		canvas_muon2_pt->cd(2);
	    _hist_rj_muon2_pt->Draw(); // Draw histogram
	    TCanvas *canvas_eta_diff = new TCanvas( "etadiff", "eta_diff" );
		canvas_eta_diff->Divide(2,1);
		canvas_eta_diff->cd(1);
	    _hist_eta_diff->Draw(); // Draw histogram
		canvas_eta_diff->cd(2);
	    _hist_rj_eta_diff->Draw(); // Draw histogram
	    TCanvas *canvas_phi_diff = new TCanvas( "phidiff", "phi_diff" );
		canvas_phi_diff->Divide(2,1);
		canvas_phi_diff->cd(1);
	    _hist_phi_diff->Draw(); // Draw histogram
		canvas_phi_diff->cd(2);
	    _hist_rj_phi_diff->Draw(); // Draw histogram
	    TCanvas *canvas_eta_phi= new TCanvas( "etaphi", "eta_phi_diff" );
		canvas_eta_phi->Divide(1,2);
		canvas_eta_phi->cd(1);
		_hist_eta_phi->Draw();
		canvas_eta_phi->cd(2);
		_hist_rj_eta_phi->Draw();
	    TCanvas *canvas_ehalo_1 = new TCanvas( "ehalo1", "ehalo_1" );
		canvas_ehalo_1->Divide(2,1);
		canvas_ehalo_1->cd(1);
	    _hist_ehalo_1->Draw(); // Draw histogram
		canvas_ehalo_1->cd(2);
	    _hist_rj_ehalo_1->Draw(); // Draw histogram
	    TCanvas *canvas_ehalo_2 = new TCanvas( "ehalo2", "ehalo_2" );
		canvas_ehalo_2->Divide(2,1);
		canvas_ehalo_2->cd(1);
	    _hist_ehalo_2->Draw(); // Draw histogram
		canvas_ehalo_2->cd(2);
	    _hist_rj_ehalo_2->Draw(); // Draw histogram
	    TCanvas *canvas_time_diff = new TCanvas( "timediff", "time_diff" );
		canvas_time_diff->Divide(2,1);
		canvas_time_diff->cd(1);
	    _hist_time_diff->Draw(); // Draw histogram
		canvas_time_diff->cd(2);
	    _hist_rj_time_diff->Draw(); // Draw histogram
	    TCanvas *canvas_chisqdof1 = new TCanvas( "chisqdof1", "chisqdof1" );
		canvas_chisqdof1->Divide(2,1);
		canvas_chisqdof1->cd(1);
	    _hist_chisqdof1->Draw(); // Draw histogram
		canvas_chisqdof1->cd(2);
	    _hist_rj_chisqdof1->Draw(); // Draw histogram
	    TCanvas *canvas_chisqdof2 = new TCanvas( "chisqdof2", "chisqdof2" );
		canvas_chisqdof2->Divide(2,1);
		canvas_chisqdof2->cd(1);
	    _hist_chisqdof2->Draw(); // Draw histogram
		canvas_chisqdof2->cd(2);
	    _hist_rj_chisqdof2->Draw(); // Draw histogram
		



    canvas_muon1_pt->SaveAs((name+"muonpt_1.pdf").c_str());
    canvas_muon2_pt->SaveAs((name+"muonpt_2.pdf").c_str());
    canvas_eta_diff->SaveAs((name+"etadiff.pdf").c_str());
    canvas_phi_diff->SaveAs((name+"phidiff.pdf").c_str());
    canvas_eta_phi->SaveAs((name+"etaphi.pdf").c_str());
    canvas_ehalo_1->SaveAs((name+"ehalo1.pdf").c_str());
    canvas_ehalo_2->SaveAs((name+"ehalo2.pdf").c_str());
    canvas_time_diff->SaveAs((name+"timediff.pdf").c_str());
    canvas_chisqdof1->SaveAs((name+"chisqdof1.pdf").c_str());
    canvas_chisqdof2->SaveAs((name+"chisqdof2.pdf").c_str());
    canvas_Zmass->SaveAs((name+"zmass.pdf").c_str());

	}


// hier war der FIT<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


    // The code below is an example of how to save your histograms
    // in a file with a different name depending on which data-set
    // you ran over.

    // Make the name of the file
    string histname("histograms-");
    string what;
    switch (_dataType)
    {
    case RealData:
        what = "data";
        break;
    case ZMonteCarlo:
        what = "zmc";
        break;
    case WMonteCarlo:
        what = "wmc";
        break;
    default:
        what = "unknown";
    }
    histname = histname + what + ".root";

    // Create the root file that will store your histograms
    TFile fHists( histname.c_str(), "recreate" ); 
    
    // Write the histograms to the file
    // Remember to add any you want to save here
    _hist_muon1_pt->Write(); 
    _hist_muon2_pt->Write();

    // Close the file
    fHists.Close(); 

    // Create pdf version of the histograms on the canvas.
    // You can save in various file formats depending on the
    // file extension chosen. A useful one for your writeup might
    // be .png as you can include these directly in word.
    string theoutputfile = what+".pdf";
    //canvas->SaveAs(theoutputfile.c_str()); // Save histograms as pdf
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    //canvas->SaveAs(theoutputfile.c_str()); // Save histograms as pdf
    // Output any useful information
    cout << endl << "Number of events processed: " << _numEvents << endl<<endl;

    // Note - don't delete your histograms or canvases (or other ROOT
    // objects) here or you won't be able to see them in root.
}
