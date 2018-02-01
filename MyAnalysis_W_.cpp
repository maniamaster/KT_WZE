#include "MyAnalysis_W_.h"

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
int trigAB=0;
int trigA=0;
//A :indep
	

// This is an example class to get you going. It creates and
// fills two histograms with the transverse momentum of the
// two highest Pt muons in an event. These muons are pointed to in the
// data by index1 and index2 respectively, which are members
// of the Data class. These muons have some loose
// quality cuts applied, namely they must have a matched central track
// and be of at least medium quality.

MyAnalysis::MyAnalysis() : _numEvents(0), _eventsWithAtLeast2Muons(0),
    _hist_muon1_pt(0),
    _hist_muon1_eta(0),
    _hist_muon1_phi(0)

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


    _hist_muon1_eta = new TH1F( "m1_eta", "First Muon eta", 100, -5, 5 );
    if (_hist_muon1_eta != (TH1F *)0) // Check it got created properly
    {
        _hist_muon1_eta->GetXaxis()->SetTitle("eta ");
        _hist_muon1_eta->GetYaxis()->SetTitle("Entries");
    }



    _hist_muon1_phi = new TH1F( "m1_phi", "First Muon phi", 100, -5, 5 );
    if (_hist_muon1_phi != (TH1F *)0) // Check it got created properly
    {
        _hist_muon1_phi->GetXaxis()->SetTitle("phi ");
        _hist_muon1_phi->GetYaxis()->SetTitle("Entries");
    }




	_hist_Wmass = new TH1F( "inv_mass", "invariant transverse W mass", 50, 0, 200);
    if (_hist_Wmass != (TH1F *)0)
    {
        _hist_Wmass->GetXaxis()->SetTitle("W transverse mass [GeV]");
        _hist_Wmass->GetYaxis()->SetTitle("Entries");
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



	_hist_chisqdof1 = new TH1F( "chisqdof1", "chisqdof 1", 100, 0, 5);
    if (_hist_chisqdof1 != (TH1F *)0)
    {
        _hist_chisqdof1->GetXaxis()->SetTitle("chisqdof1");
        _hist_chisqdof1->GetYaxis()->SetTitle("Entries");
    }

	//REJECTED:
    _hist_rj_muon1_pt = new TH1F( "m1_P_t", "First Muon P_{t}", 100, 0, 100 );
    if (_hist_rj_muon1_pt != (TH1F *)0) // Check it got created properly
    {
        _hist_rj_muon1_pt->GetXaxis()->SetTitle("P_{t} (GeV/c)");
        _hist_rj_muon1_pt->GetYaxis()->SetTitle("Entries");
        _hist_rj_muon1_pt->SetLineColor(2);
    }


    _hist_rj_muon1_eta = new TH1F( "m1_eta", "First Muon eta", 100, -5, 5 );
    if (_hist_rj_muon1_eta != (TH1F *)0) // Check it got created properly
    {
        _hist_rj_muon1_eta->GetXaxis()->SetTitle("eta ");
        _hist_rj_muon1_eta->GetYaxis()->SetTitle("Entries");
        _hist_rj_muon1_eta->SetLineColor(2);
    }



    _hist_rj_muon1_phi = new TH1F( "m1_phi", "First Muon phi", 100, -5, 5 );
    if (_hist_rj_muon1_phi != (TH1F *)0) // Check it got created properly
    {
        _hist_rj_muon1_phi->GetXaxis()->SetTitle("phi ");
        _hist_rj_muon1_phi->GetYaxis()->SetTitle("Entries");
        _hist_rj_muon1_phi->SetLineColor(2);
    }




	_hist_rj_Wmass = new TH1F( "inv_mass", "invariant transverse W mass", 50, 0, 200);
    if (_hist_rj_Wmass != (TH1F *)0)
    {
        _hist_rj_Wmass->GetXaxis()->SetTitle("W transverse mass [GeV]");
        _hist_rj_Wmass->GetYaxis()->SetTitle("Entries");
        _hist_rj_Wmass->SetLineColor(2);
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



	_hist_rj_chisqdof1 = new TH1F( "chisqdof1", "chisqdof 1", 100, 0, 5);
    if (_hist_rj_chisqdof1 != (TH1F *)0)
    {
        _hist_rj_chisqdof1->GetXaxis()->SetTitle("chisqdof1");
        _hist_rj_chisqdof1->GetYaxis()->SetTitle("Entries");
        _hist_rj_chisqdof1->SetLineColor(2);
    }

   
	_hist_met = new TH1F( "met", "missing transverse energy", 100, 0, 200);
    if (_hist_met != (TH1F *)0)
    {
        _hist_met->GetXaxis()->SetTitle("met [GeV]");
        _hist_met->GetYaxis()->SetTitle("Entries");
    }

	_hist_rj_met = new TH1F( "met", "missing transverse energy", 100, 0, 200);
    if (_hist_rj_met != (TH1F *)0)
    {
        _hist_rj_met->GetXaxis()->SetTitle("met [GeV]");
        _hist_rj_met->GetYaxis()->SetTitle("Entries");
        _hist_rj_met->SetLineColor(2);
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
	double MT;
	double MET;
	MET=sqrt((theData.MET_met_x-theData.MU_px[theData.index1])*(theData.MET_met_x-theData.MU_px[theData.index1])+(theData.MET_met_y-theData.MU_py[theData.index1])*(theData.MET_met_y-theData.MU_py[theData.index1]));
	MT=sqrt((MET+theData.MU_pt[theData.index1])*(MET+theData.MU_pt[theData.index1])-(-theData.MET_met_x)*(-theData.MET_met_x)-(theData.MET_met_y)*(theData.MET_met_y));

    _numEvents++; // Add 1 to the event counter

    // Fill histograms

	//FOR W: 
    //if (theData.index1 != -1 && theData.index2 == -1) {// index1 >= 0 for a real muon

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
        //_hist_muon1_pt->Fill( theData.MU_pt[theData.index1] );



//<<<<<<<<<<<<<<<<<<<<<<<<<<Trigger EFF<<<<<<<<<<<<<<<<<<<<<<<<<<<

//trigAB
if ((theData.TRIG_Independant==1)&&(theData.MU_pt[theData.index1]>20) && (theData.MU_ehalo[theData.index1]<1.5) && (theData.MU_chisqdof[theData.index1]<2 )){
	trigA+=1;
	if ((theData.TRIG_MUW_W_L2M3_TRK10==1)){
		trigAB+=1;	
	}
}


	

		//Mit cuts:
		if (cut==true){
			//(old cuts for Z) if (((theData.MU_q[theData.index1]==1 && theData.MU_q[theData.index2]==-1)||(theData.MU_q[theData.index1]==-1 && theData.MU_q[theData.index2]==+1)) && ((trigger==false)||(theData.TRIG_MUW_W_L2M3_TRK10==1)) && (abs(theData.MU_timeA[theData.index1]-theData.MU_timeA[theData.index2])<10) && (theData.MU_pt[theData.index2]>15) && (theData.MU_pt[theData.index1]>30) && (theData.MU_chisqdof[theData.index1] < 2) && (theData.MU_chisqdof[theData.index2] < 2) && (theData.MU_ehalo[theData.index1] < 2.5) && (theData.MU_ehalo[theData.index2] < 2.5)){
			if ( ((trigger==false)||(theData.TRIG_MUW_W_L2M3_TRK10==1)) && (theData.MU_pt[theData.index1]>20) && (theData.MU_ehalo[theData.index1]<1.5) && (theData.MU_chisqdof[theData.index1]<2 ) && (MET>20) &&(MET<60) && ((theData.index1 != -1 && theData.index2 == -1)||(theData.index1 == -1 && theData.index2 != -1))){
				_hist_muon1_pt->Fill( theData.MU_pt[theData.index1]);
				_hist_muon1_eta->Fill( theData.MU_eta[theData.index1]);
				_hist_muon1_phi->Fill( theData.MU_phi[theData.index1]);
				_hist_nmu->Fill( theData.MU_Nmu);
				_hist_ehalo_1->Fill( theData.MU_ehalo[theData.index1]);
				_hist_Wmass->Fill(MT);
				_hist_chisqdof1->Fill( theData.MU_chisqdof[theData.index1]);
				_hist_pt5_1->Fill(theData.MU_pt_5[theData.index1]);
				_hist_met->Fill(MET);
			}			
			else {
				//rejected:
				_hist_rj_muon1_pt->Fill( theData.MU_pt[theData.index1]);
				_hist_rj_muon1_eta->Fill( theData.MU_eta[theData.index1]);
				_hist_rj_muon1_phi->Fill( theData.MU_phi[theData.index1]);
				_hist_rj_nmu->Fill( theData.MU_Nmu);
				_hist_rj_pt5_1->Fill( theData.MU_pt_5[theData.index1]);
				_hist_rj_ehalo_1->Fill( theData.MU_ehalo[theData.index1]);
				_hist_rj_Wmass->Fill(MT);
				_hist_rj_chisqdof1->Fill( theData.MU_chisqdof[theData.index1]);
				_hist_rj_met->Fill(MET);
			}
	}
	else{	
	//no cuts but trigger:
		if((trigger==false)||(theData.TRIG_MUW_W_L2M3_TRK10==1)){			
			_hist_muon1_pt->Fill( theData.MU_pt[theData.index1]);
			_hist_muon1_eta->Fill( theData.MU_eta[theData.index1]);
			_hist_muon1_phi->Fill( theData.MU_phi[theData.index1]);
			_hist_nmu->Fill( theData.MU_Nmu);
			_hist_pt5_1->Fill( theData.MU_pt_5[theData.index1]);
			_hist_ehalo_1->Fill( theData.MU_ehalo[theData.index1]);
			_hist_Wmass->Fill(MT);
			_hist_chisqdof1->Fill( theData.MU_chisqdof[theData.index1]);
			_hist_met->Fill(MET);
			}
		}
}		
 //}


void MyAnalysis::End()
{
    // Called after the last event has been processed. Do your
    // post processing analysis here.

	std::cout << "End called" << std::endl;

	string name ="";
	if (_dataType == RealData)
		name = name + "Wdata_";
	if (_dataType == WMonteCarlo)
		name = name + "wmc_";
	if (cut==true)
		name = name + "cut_";
	if (cut==false)
		name = name + "uncut_";

    // Draw histograms: Make a canvas to draw them on and set the background colour to white
	if (cut==false){
    TCanvas *canvas_Wmass = new TCanvas( "wmass", "Wmass" );
	_hist_Wmass->Draw();
    TCanvas *canvas_muon1_pt = new TCanvas( "m1pts", "muon1_pt" );
    _hist_muon1_pt->Draw(); // Draw histogram
    TCanvas *canvas_ehalo_1 = new TCanvas( "ehalo1", "ehalo_1" );
	_hist_ehalo_1->Draw();
    TCanvas *canvas_chisqdof1 = new TCanvas( "chisqdof1", "chisqdof1" );
	_hist_chisqdof1->Draw();
	TCanvas *canvas_pt5 =new TCanvas( "pt5", "pt5");
	_hist_pt5_1->Draw();
	TCanvas *canvas_met =new TCanvas( "met", "met");
	_hist_met->Draw();
	
    canvas_muon1_pt->SaveAs((name+"muonpt_1.pdf").c_str());
    canvas_ehalo_1->SaveAs((name+"ehalo1.pdf").c_str());
    canvas_chisqdof1->SaveAs((name+"chisqdof1.pdf").c_str());
    canvas_Wmass->SaveAs((name+"wmass.pdf").c_str());
	canvas_pt5->SaveAs((name+"pt5.pdf").c_str());
	canvas_met->SaveAs((name+"met.pdf").c_str());


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
		
	    TCanvas *canvas_Wmass = new TCanvas( "wmass", "Wmass" );
		canvas_Wmass->Divide(2,1);
		canvas_Wmass->cd(1);
		_hist_Wmass->Draw();
		canvas_Wmass->cd(2);
		_hist_rj_Wmass->Draw();
	    TCanvas *canvas_muon1_pt = new TCanvas( "m1pts", "muon1_pt" );
		canvas_muon1_pt->Divide(2,1);
		canvas_muon1_pt->cd(1);
	    _hist_muon1_pt->Draw(); // Draw histogram
		canvas_muon1_pt->cd(2);
	    _hist_rj_muon1_pt->Draw(); // Draw histogram
	    TCanvas *canvas_ehalo_1 = new TCanvas( "ehalo1", "ehalo_1" );
		canvas_ehalo_1->Divide(2,1);
		canvas_ehalo_1->cd(1);
	    _hist_ehalo_1->Draw(); // Draw histogram
		canvas_ehalo_1->cd(2);
	    _hist_rj_ehalo_1->Draw(); // Draw histogram
	    TCanvas *canvas_chisqdof1 = new TCanvas( "chisqdof1", "chisqdof1" );
		canvas_chisqdof1->Divide(2,1);
		canvas_chisqdof1->cd(1);
	    _hist_chisqdof1->Draw(); // Draw histogram
		canvas_chisqdof1->cd(2);
	    _hist_rj_chisqdof1->Draw(); // Draw histogram
		TCanvas *canvas_pt5 = new TCanvas( "pt5", "pt5" );
		canvas_pt5->Divide(2,1);
		canvas_pt5->cd(1);
	    _hist_pt5_1->Draw(); // Draw histogram
		canvas_pt5->cd(2);
	    _hist_rj_pt5_1->Draw(); // Draw histogram

		TCanvas *canvas_met = new TCanvas( "met", "met" );
		canvas_met->Divide(2,1);
		canvas_met->cd(1);
	    _hist_met->Draw(); // Draw histogram
		canvas_met->cd(2);
	    _hist_rj_met->Draw(); // Draw histogram
		



    canvas_muon1_pt->SaveAs((name+"muonpt_1.pdf").c_str());
    canvas_ehalo_1->SaveAs((name+"ehalo1.pdf").c_str());
    canvas_chisqdof1->SaveAs((name+"chisqdof1.pdf").c_str());
    canvas_Wmass->SaveAs((name+"wmass.pdf").c_str());
	canvas_pt5->SaveAs((name+"pt5.pdf").c_str());
	canvas_met->SaveAs((name+"met.pdf").c_str());

	}

///triggereff:
double triggereff = 0;
triggereff = double(trigAB)/double(trigA) ;

std::cout<<"The Trigger Efficiency is "<< triggereff <<std::endl;

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
        what = "Wdata";
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
