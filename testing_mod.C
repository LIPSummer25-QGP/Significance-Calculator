#include <TFile.h>
#include <TTree.h>
#include <TCanvas.h>
#include <TH1F.h>
#include <TBox.h>
#include <TLegend.h>
#include <iostream>
#include <cmath>
#include <TStyle.h>
#include <TLine.h>
#include <TMarker.h>

#include "ACCSEL.h"

void testing_mod() {

// MC FILEs
const char * files[] = {
   // "/lstore/cms/hlegoinha/Bmeson/MC_DATA/MC_ppRef_Bmeson/Bu_phat5_Bfinder.root",     //ppRef                         
   // "/lstore/cms/hlegoinha/Bmeson/MC_DATA/MC_ppRef_Bmeson/Bd_phat5_Bfinder.root"         //ppRef
   // "/lstore/cms/hlegoinha/Bmeson/MC_DATA/MC_ppRef_Bmeson/Bs_phat5_Bfinder.root"
   "/lstore/cms/u25lekai/Bmeson/MC/ppRef/Bu_phat5_Bfinder.root" //ppRef New
   //"/lstore/cms/u25lekai/Bmeson/MC/ppRef/Bd_phat5_Bfinder.root" //ppRef New to be replaced again
   // "/lstore/cms/hlegoinha/X3872/MC_DATA/prompt_PSI2S_to_Jpsi_pipi_phat5_Bfinder.root" //dados MC PSI2S
   // "/lstore/cms/hlegoinha/X3872/MC_DATA/prompt_X3872_to_Jpsi_Rho_phat5_Bfinder.root" //dados MC X
           
};

const char* files_data={
    "/lstore/cms/hlegoinha/Bmeson/MC_DATA/DATA_ppref_Bmeson/DATA_ppref_Bmeson.root" // Dados completos
   // "/lstore/cms/hlegoinha/X3872/MC_DATA/DATA_ppRef_X3872.root" // Real data
    //"/user/u/u25pedrochan/HiForestMINIAOD_ppRefData_100.root" // Real data
};

//VARIABLES
//VARIABLES
const char * variables[] = {"Balpha",/* "BQvalueuj"   ,*/  "Bcos_dtheta", "BtrkPtimb", "Bchi2cl",  "Btrk1dR", "Btrk2dR",     "Btrk1Pt",    "Btrk2Pt", "Bnorm_svpvDistance_2D", "Bnorm_svpvDistance" , "Bnorm_trk1Dxy" ,  "Bnorm_trk2Dxy"};
const double ranges[][2] = {{0,3.15},/*      {0,2.5}  ,*/     {0.9, 1}  ,      {0,1},    {0.05,1},  {0,4.5},  {0,2} ,     {0.5, 10} ,     {0.5, 10},   {0,85},                     {0,85}   ,        {-22,22}  ,          {-22,22} };
int SELplots = 0; //mudar para 1 com ruído e descomentar a linha acima

//const char * variables[] = {"Bmass"/*,  "Btktkmass" , "Bpt", "By", "nSelectedChargedTracks"*/};//comentar o Btkmass
//const double ranges[][2] = { {5 , 6}/*,{0,2.5} ,{5, 50}, {-2.4, 2.4}, {0,150}*/};
//VARIABLES
//VARIABLES

/////////////////////////////////  ///////////////////////////  ////////////////

TString cutlevel = ""; // "_RAW", "_ACC", "_SEL", "_TRG", "", 

/////////////////////////////////  ///////////////////////////  ///////////

TString path_to_file = "";

const int nVars = sizeof(variables)/sizeof(variables[0]);

for (int ifile = 0; ifile < sizeof(files)/sizeof(files[0]); ++ifile) {
    path_to_file = Form("%s", files[ifile]);
    //path_to_file = Form("/eos/user/h/hmarques/MC_ppRef_Bmeson/MC_ppRef_Bmeson/%s_Bfinder.root", files[ifile]);

     

    TFile *file = TFile::Open(path_to_file.Data());
    TFile *file_data = TFile::Open(files_data);
    // Get the trees from the file
    TTree *treeMix;
    TTree *treedata;
    if (path_to_file.Contains("Bs")){
        file->GetObject("Bfinder/ntphi", treeMix);
        file_data->GetObject("Bfinder/ntphi", treedata);
    } else if (path_to_file.Contains("Bd")){
        file->GetObject("Bfinder/ntKstar", treeMix);
        file_data->GetObject("Bfinder/ntKstar", treedata);
    } else if (path_to_file.Contains("Bu")){
        file->GetObject("Bfinder/ntKp", treeMix);
        file_data->GetObject("Bfinder/ntKp", treedata); 
    } else if ((path_to_file.Contains("Rho"))|| (path_to_file.Contains("PSI2S"))){
        file->GetObject("Bfinder/ntmix", treeMix);
        file_data->GetObject("Bfinder/ntmix", treedata);
    }



    std::cout << "\n" << "Entries in treeMix: " << treeMix->GetEntries() << std::endl;
    std::cout << "\n" << "Entries in treedata: " << treedata->GetEntries() << std::endl;

    for (int i = 0; i < nVars; ++i) {
        TString var = variables[i];

        if(path_to_file.Contains("Bu") && ((var.Contains("trk2") || var.Contains("Ptimb")))) continue; // B+ has less one track!

        // Create a canvas to draw the histograms
        TCanvas *canvas = new TCanvas("canvas", "", 600, 600);
        canvas->SetLeftMargin(0.15);
        canvas->SetTopMargin(0.05);
        canvas->SetRightMargin(0.05); 

        double hist_Xhigh      = ranges[i][1];
        double hist_Xlow       = ranges[i][0];
        int hist_Nbin          = 50000 ;
        if (var == "nSelectedChargedTracks") {
            hist_Nbin = hist_Xhigh - hist_Xlow;
        } 
        double bin_length_MEV  = (hist_Xhigh - hist_Xlow) / hist_Nbin;
        if(SELplots){ hist_Nbin = 50; }
        
        TString Xlabel ;
        if (var == "Bmass"){ 
            if (path_to_file.Contains("Bs")){
                Xlabel = "m_{J/#Psi K^{+} K^{-}} [GeV/c^{2}]";
            } else if (path_to_file.Contains("Bd")){
                Xlabel = "m_{J/#Psi K^{+} #pi^{-}} [GeV/c^{2}]";
            } else if (path_to_file.Contains("Bu")) {
                Xlabel = "m_{J/#Psi K^{+}} [GeV/c^{2}]";
            } else if (path_to_file.Contains("PSI2S")){
                Xlabel="m_{J/#Psi#pi^{+}#pi^{-}} [GeV/c^{2}]";
            } else if (path_to_file.Contains("Rho")){
                Xlabel="m_{J/#Psi#rho} [GeV/c^{2}]";
            }
        } else if (var == "Bpt"){ 
            Xlabel = "p_{T} [GeV/c]";
        } else { 
            Xlabel = var.Data();
        }

        // Create histograms
        TH1F *hist_SIG = new TH1F("hist_SIG"      , Form("; %s; Entries / %.3f ", Xlabel.Data(), bin_length_MEV) , hist_Nbin, hist_Xlow ,hist_Xhigh); 
        TH1F *hist_BKG = new TH1F("hist_BKG"      , Form("; %s; Entries / %.3f ", Xlabel.Data(), bin_length_MEV) , hist_Nbin, hist_Xlow ,hist_Xhigh);
        TH1F *hist     = new TH1F("hist"          , Form("; %s; Entries / %.3f ", Xlabel.Data(), bin_length_MEV) , hist_Nbin, hist_Xlow ,hist_Xhigh);
        TH1F *hist_SIG_WT   = new TH1F("hist_SIG_WT"  , Form("; %s; Entries / %.3f ", Xlabel.Data(), bin_length_MEV) , hist_Nbin, hist_Xlow ,hist_Xhigh);        
        TH1F *hist_SIG_BOTH = new TH1F("hist_SIG_BOTH", Form("; %s; Entries / %.3f ", Xlabel.Data(), bin_length_MEV) , hist_Nbin, hist_Xlow ,hist_Xhigh);        

        
        /////////////////////////////////////////////////////////////////////////////////////////////////////////
        //SELECT THE acc + presel CUT 

        TString dirNAME = "";
        TString Final = "1";      
        TString trgmatches = TRGmatching.Data();   //TRG matching only in ppRef
        TString ACCcuts = "" ;
        TString SELcuts = "" ;

        if (path_to_file.Contains("Bu")){
            ACCcuts    = ACCcuts_ppRef_Bu.Data(); //ppRef
            SELcuts    = SELcuts_ppRef_Bu.Data(); //ppRef
            if (path_to_file.Contains("PbPb")) { 
                ACCcuts = ACCcuts_PbPb_Bu.Data();
                SELcuts = SELcuts_PbPb_Bu.Data();
                trgmatches = "1";
            }
        }
        else {
            ACCcuts    = ACCcuts_ppRef.Data(); //ppRef
            SELcuts    = SELcuts_ppRef.Data(); //ppRef
            if (path_to_file.Contains("PbPb")) { 
                ACCcuts = ACCcuts_PbPb.Data();
                SELcuts = SELcuts_PbPb.Data();
                trgmatches = "1";
            }
        }

        TString cut = "";
        if (cutlevel == "_RAW")       {cut = Form(" %s "                   ,FIDreg.Data());}                                                              //RAW (inside fid reg only)
        else if (cutlevel == "_ACC")  {cut = Form(" %s && %s "             ,FIDreg.Data(), ACCcuts.Data());}                                              //ACC
        else if (cutlevel == "_SEL")  {cut = Form(" %s && %s && %s "       ,FIDreg.Data(), ACCcuts.Data(), SELcuts.Data());}                              //SEL
        else if (cutlevel == "_TRG")  {cut = Form(" %s && %s && %s && %s " ,FIDreg.Data(), ACCcuts.Data(), SELcuts.Data(), trgmatches.Data());}           //TRG
        else if (cutlevel == ""){
            if (!SELplots) {dirNAME  = "";}
            cut = Form(" %s && %s && %s && %s", ACCcuts.Data(), SELcuts.Data(), trgmatches.Data(), Final.Data());                   //Final
        }
        else{
            std::cerr << "Invalid cut level specified: " << cutlevel << std::endl;
            return;
        }                                                                                                 

        TString sepcCASES = "1";
        if (path_to_file.Contains("Bs")){ 
            sepcCASES = "abs(Btktkmass - 1.019455) < 0.015"; // phi meson mass cut
            treedata->Draw(Form("%s >> hist_BKG", var.Data()), Form("((Bmass < 5.289) || (Bmass > 5.449)) && %s && %s", cut.Data(), sepcCASES.Data()));
        } else if (path_to_file.Contains("Bd")){ 
            sepcCASES = "abs(Btktkmass - 0.89594) < 0.25"; // Kstar meson mass cut
        }
        //treeMix->Draw(Form("%s >> hist_SIG", var.Data()), Form(" Bmass>3.6 && Bmass<4 && Bchi2cl>0.02 && BQvalueuj<0.2 && %s && %s && %s", isMCsignal.Data(), cut.Data(), sepcCASES.Data()));  // SIG
        if (path_to_file.Contains("Bd")){ 
           // treeMix->Draw(Form("%s >> hist_SIG_WT"  , var.Data()), Form(" (Bgen == 41000) && %s && %s", cut.Data(), sepcCASES.Data()));                              // WT component
	       treedata->Draw(Form("%s >> hist_BKG" , var.Data()), Form("Bmass>5.1 && Bmass<5.5 && Bchi2cl>0.003 && Bnorm_svpvDistance_2D>3.9914 && ((Bmass < 5.204911988) || (Bmass > 5.356368012)) && %s && %s", cut.Data(), sepcCASES.Data())); // BKG -- (notice the *!* in the first %s)
               treeMix->Draw(Form("%s >> hist_SIG_BOTH", var.Data()), Form("Bmass>5.1 && Bmass<5.5 && Bchi2cl>0.003 && Bnorm_svpvDistance_2D>3.9914 && Bnorm_svpvDistance>2 && (%s || Bgen==41000) && %s && %s", isMCsignal.Data(), cut.Data(), sepcCASES.Data()));  // SIG + WT
        } else if(path_to_file.Contains("Bu")) {
            treeMix->Draw(Form("%s >> hist_SIG", var.Data()), Form("Bnorm_svpvDistance>2 && Bnorm_svpvDistance_2D>4 && Bchi2cl>0.003 && %s && %s && %s", isMCsignal.Data(), cut.Data(), sepcCASES.Data()));
            treedata->Draw(Form("%s >> hist_BKG", var.Data()), Form("Bmass<5.8 && Bnorm_svpvDistance_2D>4 && (Bmass > 5.380091232) && Bchi2cl>0.003 && %s && %s", cut.Data(), sepcCASES.Data())); // BKG -- (notice the *!* in the first %s)
        } else if(path_to_file.Contains("Rho")) {
            treeMix->Draw(Form("%s >> hist_SIG", var.Data()), Form(" Bmass>3.6 && Bmass<4 && Bchi2cl>0.02 && BQvalueuj<0.2 && %s && %s && %s", isMCsignal.Data(), cut.Data(), sepcCASES.Data()));
            treedata->Draw(Form("%s >> hist_BKG", var.Data()), Form("((Bmass<3.65788)||(Bmass>3.71698 && Bmass<3.83576) || (Bmass>3.91064)) && Bmass>3.6 && Bmass<4 && Bchi2cl>0.02 && BQvalueuj<0.2 && %s", cut.Data()));
        } else if(path_to_file.Contains("PSI2S")) {
            treedata->Draw(Form("%s >> hist_BKG", var.Data()), Form("Bnorm_svpvDistance<1.635 && ((Bmass < 3.65826) || ((Bmass > 3.71562) && (Bmass < 3.83176)) || (Bmass > 3.91336)) && %s", cut.Data()));
        }
         


//treeMix->Draw(Form("%s >> hist"    , var.Data()), Form(" %s && %s", cut.Data(), sepcCASES.Data()) );                          // ALL 

        
        //SELECT THE acc + presel CUT 
        ///////////////////////////////////////////////////////////////////////////////////////////////////////// 

        // Customize the Histograms
        hist->SetLineColor(kBlack);
        hist->SetLineWidth(2);

        hist_SIG->SetLineColor(kOrange+7);
        hist_SIG->SetFillColor(kOrange+7);    
        //hist_SIG->SetFillStyle(3001); 
        //hist_SIG->SetLineWidth(2);
        //hist_SIG->SetLineStyle(2);

        hist_SIG_BOTH->SetLineColor(kOrange+7);
        hist_SIG_BOTH->SetFillColor(kOrange+7);
        hist_SIG_WT->SetLineColor(kOrange+7);
        hist_SIG_WT->SetFillColor(kOrange+7);    

        hist_BKG->SetLineColor(kBlue);
        hist_BKG->SetFillColor(kBlue);     
        hist_BKG->SetFillStyle(3358);
        //hist_BKG->SetLineStyle(2);
        //hist_BKG->SetLineWidth(2);

        //hist_BKG->SetMarkerStyle(20); // Circle marker
        //hist_BKG->SetMarkerSize(.8); // Bigger dots
        // Customize the Histograms
 
        // Set display titles (for legend and stat box)
        hist_SIG->SetName("MC_SIG");  // <--- This affects the stat box label

        hist_BKG->SetName("DATA_BKG");  // <--- Also affects the stat box

        hist_SIG_BOTH->SetName("MC_SIG");


         // <--- This affects the stat box label

         // <--- Also affects the stat box

        


       if (SELplots == 1) { // NORMALIZE
              double int_sig     = hist_SIG->Integral();
              double int_bkg     = hist_BKG->Integral();
              double int_sig_wt  = hist_SIG_BOTH->Integral();

         if (int_sig > 0 || int_sig_wt > 0){     
                hist_SIG->Scale(1.0 / int_sig);
                hist_BKG->Scale(1.0 / int_bkg);
                hist_SIG_BOTH->Scale(1.0 / int_sig_wt); 
          }
        }




       // if(SELplots==1){ //NORMALIZE
       //     double nEntries_sig = hist_SIG->GetEntries();
       //     double nEntries_bkg = hist_BKG->GetEntries();
      //      double nEntries_sig_WT = hist_SIG_BOTH->GetEntries();

            //double nEntries = hist->GetEntries();
      //      if (nEntries_sig > 0) {
      //          hist_SIG->Scale(1.0 / nEntries_sig);
      //          hist_BKG->Scale(1.0 / nEntries_bkg);
      //          hist_SIG_BOTH->Scale(1.0 / nEntries_sig_WT);
      //      }
      //  }

        if(1){// set the y-axis maximum if needed
            Double_t     max_val = TMath::Max(hist->GetMaximum(), TMath::Max(hist_BKG->GetMaximum(), hist_SIG->GetMaximum()));
            if(SELplots) {
                if (path_to_file.Contains("Bd")) {
                    max_val = TMath::Max( hist_BKG->GetMaximum(), hist_SIG_BOTH->GetMaximum()) ;
                    hist_SIG_BOTH->SetMaximum(max_val * 1.1);  // Increase the max range to give some space
                } else {
                    max_val = TMath::Max( hist_SIG->GetMaximum(), hist_BKG->GetMaximum());
                    hist_SIG->SetMaximum(max_val * 1.1);  // Increase the max range to give some space
                }
                hist_BKG->SetMaximum(max_val * 1.1); 
            } else {
                hist_SIG->SetMaximum(max_val * 1.1);  // Increase the max range to give some space
                hist_BKG->SetMaximum(max_val * 1.1);
            }
        }

        // Draw the histograms
        hist->SetStats(0);
        if (SELplots && path_to_file.Contains("Bd")){
            hist_SIG_BOTH->Draw("HIST");
        } else {
            hist_SIG->Draw("HIST");
            if (path_to_file.Contains("Bd")) {
                hist_SIG_WT->Draw("HIST SAMES");
            }
        }
        hist_BKG->Draw("HIST SAMES");
      

        if(!SELplots) hist->Draw("HIST SAME");
        gPad->Update();

        // Move and color the stat boxes
        TPaveStats *st_bkg = (TPaveStats*)hist_BKG->GetListOfFunctions()->FindObject("stats");
        if (st_bkg) {
            st_bkg->SetTextColor(kBlue);
            st_bkg->SetLineColor(kBlue); 
            st_bkg->SetX1NDC(0.75);
            st_bkg->SetX2NDC(0.95);
            st_bkg->SetY1NDC(0.85);
            st_bkg->SetY2NDC(0.95);
            st_bkg->Draw();
        }
        TPaveStats *st_sig = (TPaveStats*)hist_SIG->GetListOfFunctions()->FindObject("stats");
        if (st_sig) {
            st_sig->SetTextColor(kOrange+7);
            st_sig->SetLineColor(kOrange+7);
            st_sig->SetX1NDC(0.75);
            st_sig->SetX2NDC(0.95);
            st_sig->SetY1NDC(0.75);
            st_sig->SetY2NDC(0.85);
            st_sig->Draw();
        }
        TPaveStats *st_sigWT = (TPaveStats*)hist_SIG_BOTH->GetListOfFunctions()->FindObject("stats");
        if (st_sigWT) {
            st_sigWT->SetTextColor(kOrange+7);
            st_sigWT->SetLineColor(kOrange+7);
            st_sigWT->SetX1NDC(0.75);
            st_sigWT->SetX2NDC(0.95);
            st_sigWT->SetY1NDC(0.75);
            st_sigWT->SetY2NDC(0.85);
            st_sigWT->Draw();
        }        
        // LATEX text
        if(0){
            double Nsignal = hist_SIG->GetEntries();
            double Nbkg = hist_BKG->GetEntries();
            double significance = (Nbkg > 0) ? Nsignal / sqrt(Nbkg) : 0;

            TLatex latex;
            latex.SetNDC();
            latex.SetTextSize(0.022);
            latex.SetTextColor(kOrange+7); // Same as hist_SIG
            latex.DrawLatex(0.18, 0.82, Form("N_{sig} = %.0f", Nsignal));
            latex.SetTextColor(kBlue);     // Same as hist_BKG
            latex.DrawLatex(0.18, 0.85, Form("N_{bkg} = %.0f", Nbkg));
        }

        // Add a legend
        auto legend = new TLegend(0.15, 0.7, 0.25, 0.9);
        legend->AddEntry(hist_SIG, "MC SIG", "l");
        legend->AddEntry(hist_BKG, "DATA BKG", "l");          

        // fs, fb
        double f_s = -1;
        double f_b = -1;
   if (path_to_file.Contains("Bu")){
     //f_s = 1;
     //f_b = 1;
     // f_s = 12.173	;
     // f_b = 0.484;  
     //f_s = 11.989;
     //f_b = 0.666;
     //f_s = 0.802;
     //f_b = 0.712;
     f_s =11.638;
     f_b =0.513;
    } else if (path_to_file.Contains("Bd")){
      f_s = 4.396;
      f_b = 0.612;	  
    } else if (path_to_file.Contains("Bs")){
      f_s = 1;
      f_b = 1;
    } else if (path_to_file.Contains("Rho")){
      f_s = 0.370;
      f_b = 0.300;
    } else if (path_to_file.Contains("PSI2S")){
      f_s = 1;
      f_b = 1;
    }

       


        // Save the canvas as an image
        if(path_to_file.Contains("Bd")){
         const int nbins = hist_SIG_BOTH->GetNbinsX();
         std::vector<double> cuts_right, sig_right;
         std::vector<double> cuts_left, sig_left;

         double maxSig1 = -1;
         double bestCut1 = -1;
         double B_Cut1 = -1;
         double S_Cut1 = -1;
         TString bestDirection1 = "";

    // === Left-side cut: cut < x  ===
         for (int ibin = 1; ibin <= nbins; ++ibin) {
          double cut = hist_SIG_BOTH->GetBinLowEdge(ibin);
          double S = hist_SIG_BOTH->Integral(ibin, nbins);
          double B = hist_BKG->Integral(ibin, nbins);
          double significance = (S>0) ? (S*f_s) / sqrt((S*f_s) + (B*f_b)) : 0;

          /*
          std::cout << "  B = " << B << std::endl;
          std::cout << "  S    = " << S << std::endl;
          std::cout << "  Significance1 = " << significance << std::endl;
          std::cout << "  Cut Direction1    = " << "cut < x" << std::endl;
          std::cout << "  Cut Value1        = " << cut << std::endl;
          */
          cuts_right.push_back(cut);
          sig_right.push_back(significance);

          if (significance > maxSig1) {
              maxSig1 = significance;
              bestCut1 = cut;
              S_Cut1 = S;
              B_Cut1 = B;
              bestDirection1 = "cut < x";
        }
    }
/*
        std::cout << "  Max Significance1 = " << maxSig1 << std::endl;
        std::cout << "  Cut Direction1    = " << bestDirection1 << std::endl;
        std::cout << "  Cut Value1        = " << bestCut1 << std::endl;
*/
       
        double maxSig2 = -1;
        double bestCut2= -1;
        double B_Cut2 = -1;
        double S_Cut2 = -1;
        TString bestDirection2="";
    // === Right-side cut: x < cut ===
        for (int ibin = 1; ibin <= nbins; ++ibin) {
          double cut = hist_SIG_BOTH->GetBinLowEdge(ibin + 1);  // upper edge
          double S = hist_SIG_BOTH->Integral(1, ibin);
          double B = hist_BKG->Integral(1, ibin);
          double significance = (S>0) ? (S*f_s) / sqrt((S*f_s) + (B*f_b)) : 0;
          /*
          std::cout << "  B = " << B << std::endl;
          std::cout << "  S    = " << S << std::endl;
          std::cout << "  Significance2 = " << significance << std::endl;
          std::cout << "  Cut Direction2    = " << "x > cut" << std::endl;
          std::cout << "  Cut Value2        = " << cut << std::endl;
          */
          cuts_left.push_back(cut);
          sig_left.push_back(significance);

          if (significance > maxSig2) {
              maxSig2 = significance;
              bestCut2 = cut;
              S_Cut2 = S;
              B_Cut2 = B;
              bestDirection2 = "x < cut";
        }
    }

/*
        std::cout << "  Max Significance2 = " << maxSig2 << std::endl;
        std::cout << "  Cut Direction    = " << bestDirection2 << std::endl;
        std::cout << "  Cut Value        = " << bestCut2 << std::endl;
*/
        double maxSig_both=-1;
        double bestCut_both=-1;
        TString bestDirection="";
        if(maxSig1>maxSig2){
          maxSig_both=maxSig1;
          bestCut_both=bestCut1;
          bestDirection= "cut < x";
        } else {
          maxSig_both=maxSig2;
          bestCut_both=bestCut2;
          bestDirection= "x < cut";
        }
        std::cout << "  Variable  = " << var.Data() << std::endl;
        std::cout << "  B = " << B_Cut1 << std::endl;
        std::cout << "  S    = " << S_Cut1 << std::endl;
        std::cout << "  Max Significance1 = " << maxSig1 << std::endl;
        std::cout << "  Cut Direction1    = " << bestDirection1 << std::endl;
        std::cout << "  Cut Value1        = " << bestCut1 << std::endl;


        std::cout << "  B = " << B_Cut2 << std::endl;
        std::cout << "  S    = " << S_Cut2 << std::endl;
        std::cout << "  Max Significance2 = " << maxSig2 << std::endl;
        std::cout << "  Cut Direction    = " << bestDirection2 << std::endl;
        std::cout << "  Cut Value        = " << bestCut2 << std::endl;

        std::cout << "\n=== Best Significance ===" << std::endl;
        std::cout << "  Max Significance = " << maxSig_both << std::endl;
        std::cout << "  Cut Direction    = " << bestDirection << std::endl;
        std::cout << "  Cut Value        = " << bestCut_both << std::endl;
         if (0) { // NORMALIZE
              double int_sig     = hist_SIG->Integral();
              double int_bkg     = hist_BKG->Integral();
              double int_sig_wt  = hist_SIG_BOTH->Integral();

          if (int_sig > 0 || int_sig_wt > 0){     
                hist_SIG->Scale(1.0 / int_sig);
                hist_BKG->Scale(1.0 / int_bkg);
                hist_SIG_BOTH->Scale(1.0 / int_sig_wt); 
          }
        }
    
        // Draw the histograms
 /*       hist->SetStats(0);
        if (SELplots && path_to_file.Contains("Bd")){
            hist_SIG_BOTH->Draw("HIST");
        } else {
            hist_SIG->Draw("HIST");
        }
        hist_BKG->Draw("HIST SAMES");
*/

        double x_cut = bestCut_both;        
        double y_min = 0;
        double y_max = hist_BKG->GetMaximum();

        TLine* line = new TLine(x_cut, y_min, x_cut, y_max);
        line->SetLineColor(kRed);
        line->SetLineWidth(2);
        line->SetLineStyle(2);  // dashed line
        line->Draw("SAME");

        TString particleNAME = "Bu";
        TString systemNAME = "SG_ppRef_";
        if (path_to_file.Contains("Bs")){
            particleNAME = "Bs";
        } else if (path_to_file.Contains("Bd")){
            particleNAME = "Bd";
        } else if (path_to_file.Contains("Rho")){
            particleNAME = "X3872";
        } else if (path_to_file.Contains("PSI2S")){
            particleNAME = "PSI2S";
        }
        if (path_to_file.Contains("PbPb"))  { systemNAME = "SG_PbPb_";}


 /* // === Plotting ===
    TCanvas* c = new TCanvas("c", "", 900, 700);
    c->SetGrid();

    double xMin_R = cuts_right.front();
    double xMax_R = cuts_right.back();
    double xMin_L = cuts_left.front();
    double xMax_L = cuts_left.back();

    double yMax_R = *std::max_element(sig_right.begin(), sig_right.end());

    TGraph* gRight = new TGraph(cuts_right.size(), &cuts_right[0], &sig_right[0]);
    gRight->SetLineColor(kBlue);
    gRight->SetLineWidth(2);
    gRight->SetTitle("");
    gRight->GetXaxis()->SetTitle(Form("Cut Value, %s", var.Data()));
    gRight->GetYaxis()->SetTitle("Significance");

    gRight->GetXaxis()->SetTitleSize(0.045);
    gRight->GetXaxis()->SetTitleOffset(1.1);
    gRight->GetYaxis()->SetTitleSize(0.045);
    gRight->GetYaxis()->SetTitleOffset(1.2);


    TGraph* gLeft = new TGraph(cuts_left.size(), &cuts_left[0], &sig_left[0]);
    gLeft->SetLineColor(kRed);
    gLeft->SetLineWidth(2);

    gRight->GetXaxis()->SetLimits(xMin_R, xMax_R);
    gRight->GetYaxis()->SetRangeUser(0, maxSig_both*1.1);
    gLeft->GetXaxis()->SetLimits(xMin_L, xMax_L);
    gLeft->GetYaxis()->SetRangeUser(0, maxSig_both*1.1);

    gRight->Draw("AL");
    gLeft->Draw("L SAME");

// draw a vertical dashed line at the best cut
TLine* bestLine = new TLine(bestCut_both, 0, bestCut_both, maxSig_both*1.1);
bestLine->SetLineColor(kOrange+7);
bestLine->SetLineStyle(2);
bestLine->SetLineWidth(2);
bestLine->Draw("SAME");

// draw a marker at the maximum-significance point
TMarker* bestMark = new TMarker(bestCut_both, maxSig_both, 29);
bestMark->SetMarkerColor(kMagenta);
bestMark->SetMarkerSize(1.2);
bestMark->Draw("SAME");

    auto legend = new TLegend(0.6, 0.75, 0.88, 0.88);
    legend->SetHeader(particleNAME, "");
    legend->AddEntry(gRight, "Cut: x > value", "l");
    legend->AddEntry(gLeft,  "Cut: x < value", "l");
// make the text smaller
legend->SetTextSize(0.025);  
legend->SetTextFont(62);

// shrink & move the legend box by tweaking its NDC corners
legend->SetX1NDC(0.65);  // left
legend->SetX2NDC(0.85);  // right
legend->SetY1NDC(0.80);  // bottom
legend->SetY2NDC(0.90);  // top

// optionally remove the fill to make it less obtrusive
legend->SetFillStyle(0);  
legend->SetBorderSize(0);

    legend->Draw();

        c->SaveAs(Form("./%s%s%s_%s%s.png", dirNAME.Data(), systemNAME.Data() , var.Data(), particleNAME.Data(), cutlevel.Data()));*/
        
    // === Plotting: RIGHT-hand cut (cut < x) ===
TCanvas* cRight = new TCanvas("cRight", "Right-side Cut: cut < x", 900, 700);
cRight->SetGrid();

TGraph* gRight = new TGraph(cuts_right.size(), &cuts_right[0], &sig_right[0]);
gRight->SetLineColor(kBlue);
gRight->SetLineWidth(2);
gRight->SetTitle(Form(""));
gRight->GetXaxis()->SetTitle(Form("Cut Value, (%s)", var.Data()));
gRight->GetYaxis()->SetTitle("Significance");
gRight->GetXaxis()->SetTitleSize(0.045);
gRight->GetXaxis()->SetTitleOffset(1.1);
gRight->GetYaxis()->SetTitleSize(0.045);
gRight->GetYaxis()->SetTitleOffset(1.2);
gRight->GetYaxis()->SetRangeUser(0, maxSig_both * 1.1);
gRight->Draw("AL");

// Best marker & line for RIGHT cut
if (bestDirection == "cut < x") {
    TLine* bestLineRight = new TLine(bestCut1, 0, bestCut1, maxSig1 * 1.1);
    bestLineRight->SetLineColor(kOrange + 7);
    bestLineRight->SetLineStyle(2);
    bestLineRight->SetLineWidth(2);
    bestLineRight->Draw("SAME");

    TMarker* bestMarkRight = new TMarker(bestCut1, maxSig1, 29);
    bestMarkRight->SetMarkerColor(kMagenta);
    bestMarkRight->SetMarkerSize(1.5);
    bestMarkRight->Draw("SAME");
}
auto legendRight = new TLegend(0.6, 0.15, 0.88, 0.28);
legendRight->SetHeader(particleNAME, "");

legendRight->AddEntry(gRight,Form( "Cut: x > cut value"), "l");
legendRight->AddEntry(gRight,Form( "BestCut: x > %.4f", bestCut1), "l");
legendRight->AddEntry(gRight,Form( "MaxS = %.4f",maxSig1), "l");

// make the text smaller
legendRight->SetTextSize(0.035);  
legendRight->SetTextFont(62);

// shrink & move the legend box by tweaking its NDC corners
legendRight->SetX1NDC(0.65);  // left
legendRight->SetX2NDC(0.15);  // right
legendRight->SetY1NDC(0.80);  // bottom
legendRight->SetY2NDC(0.28);  // top

// optionally remove the fill to make it less obtrusive
//legendRight->SetFillStyle(0);  
//legendRight->SetBorderSize(0);

legendRight->Draw();

// Save the canvas as an image
cRight->SaveAs(Form("./%s%s%s_%s%s_RIGHT.png", dirNAME.Data(), systemNAME.Data(), var.Data(), particleNAME.Data(), cutlevel.Data()));

// === Plotting: LEFT-hand cut (x < cut) ===
TCanvas* cLeft = new TCanvas("cLeft", "Left-side Cut: x < cut", 900, 700);
cLeft->SetGrid();

TGraph* gLeft = new TGraph(cuts_left.size(), &cuts_left[0], &sig_left[0]);
gLeft->SetLineColor(kRed);
gLeft->SetLineWidth(2);
gLeft->SetTitle(Form(""));
gLeft->GetXaxis()->SetTitle(Form("Cut Value (%s)", var.Data()));
gLeft->GetYaxis()->SetTitle("Significance");
gLeft->GetXaxis()->SetTitleSize(0.045);
gLeft->GetXaxis()->SetTitleOffset(1.1);
gLeft->GetYaxis()->SetTitleSize(0.045);
gLeft->GetYaxis()->SetTitleOffset(1.2);
gLeft->GetYaxis()->SetRangeUser(0, maxSig_both * 1.1);
gLeft->Draw("AL");

// Best marker & line for LEFT cut
if (bestDirection == "x < cut") {
    TLine* bestLineLeft = new TLine(bestCut2, 0, bestCut2, maxSig2 * 1.1);
    bestLineLeft->SetLineColor(kOrange + 7);
    bestLineLeft->SetLineStyle(2);
    bestLineLeft->SetLineWidth(2);
    bestLineLeft->Draw("SAME");

    TMarker* bestMarkLeft = new TMarker(bestCut2, maxSig2, 29);
    bestMarkLeft->SetMarkerColor(kMagenta);
    bestMarkLeft->SetMarkerSize(1.5);
    bestMarkLeft->Draw("SAME");
}
auto legendLeft = new TLegend(0.6, 0.15, 0.88, 0.28);
legendLeft->SetHeader(particleNAME, "");
legendLeft->AddEntry(gLeft,Form( "Cut: x < cut value"), "l");
legendLeft->AddEntry(gLeft,Form( "BestCut: x < %.3f", bestCut2), "l");
legendLeft->AddEntry(gLeft,Form( "MaxS = %.3f",maxSig2), "l");

// make the text smaller
legendLeft->SetTextSize(0.035);  
legendLeft->SetTextFont(62);

// shrink & move the legend box by tweaking its NDC corners
legendLeft->SetX1NDC(0.65);  // left
legendLeft->SetX2NDC(0.15);  // right
legendLeft->SetY1NDC(0.80);  // bottom
legendLeft->SetY2NDC(0.28);  // top

// optionally remove the fill to make it less obtrusive
//legendLeft->SetFillStyle(0);  
//legendLeft->SetBorderSize(0);

legendLeft->Draw();

cLeft->SaveAs(Form("./%s%s%s_%s%s_LEFT.png", dirNAME.Data(), systemNAME.Data(), var.Data(), particleNAME.Data(), cutlevel.Data()));


    
    } else {
        const int nbins = hist_SIG->GetNbinsX();
        std::vector<double> cuts_right, sig_right;
        std::vector<double> cuts_left, sig_left;

        double maxSig1 = -1;
        double bestCut1 = -1;
        double S_Cut1 = -1;
        double B_Cut1 = -1;
        TString bestDirection1 = "";

    // === Left-side cut: cut < x  ===
        for (int ibin = 1; ibin <= nbins; ++ibin) {
          double cut = hist_SIG->GetBinLowEdge(ibin);
          double S = hist_SIG->Integral(ibin, nbins);
          double B = hist_BKG->Integral(ibin, nbins);
          double significance = (S  > 0) ? (S*f_s) / sqrt((S*f_s) + (B*f_b)) : 0;

          /*
          std::cout << "  B = " << B << std::endl;
          std::cout << "  S    = " << S << std::endl;            
          std::cout << "  Significance1 = " << significance << std::endl;
          std::cout << "  Cut Direction1    = " << "cut < x" << std::endl;
          std::cout << "  Cut Value1        = " << cut << std::endl;
          */
          cuts_right.push_back(cut);
          sig_right.push_back(significance);

          if (significance > maxSig1) {
              maxSig1 = significance;
              bestCut1 = cut;
              S_Cut1 = S;
              B_Cut1 = B;
              bestDirection1 = "cut < x";
        }
    }
/*
        std::cout << "  Max Significance1 = " << maxSig1 << std::endl;
        std::cout << "  Cut Direction1    = " << bestDirection1 << std::endl;
        std::cout << "  Cut Value1        = " << bestCut1 << std::endl;
*/
       double maxSig2 = -1;
       double bestCut2= -1;
       double S_Cut2 = -1;
       double B_Cut2 = -1;
       TString bestDirection2="";
    // === Right-side cut: x < cut ===
        for (int ibin = 1; ibin <= nbins; ++ibin) {
          double cut = hist_SIG->GetBinLowEdge(ibin + 1);  // upper edge
          double S = hist_SIG->Integral(1, ibin);
          double B = hist_BKG->Integral(1, ibin);
          double significance = (S  > 0) ? (S*f_s) / sqrt((S*f_s) + (B*f_b)) : 0;
          /*
          std::cout << "  B = " << B << std::endl;
          std::cout << "  S    = " << S << std::endl;
          std::cout << "  Significance2 = " << significance << std::endl;
          std::cout << "  Cut Direction2    = " << "x > cut" << std::endl;
          std::cout << "  Cut Value2        = " << cut << std::endl;
          */
          cuts_left.push_back(cut);
          sig_left.push_back(significance);

          if (significance > maxSig2) {
              maxSig2 = significance;
              bestCut2 = cut;
              bestDirection2 = "x < cut";
              S_Cut2 = S;
              B_Cut2 = B;
        }
    }
/*
        std::cout << "  Max Significance2 = " << maxSig2 << std::endl;
        std::cout << "  Cut Direction    = " << bestDirection2 << std::endl;
        std::cout << "  Cut Value        = " << bestCut2 << std::endl;
*/
        double maxSig_both=-1;
        double bestCut_both=-1;
        TString bestDirection="";
        if(maxSig1>maxSig2){
          maxSig_both=maxSig1;
          bestCut_both=bestCut1;
          bestDirection= "cut < x";
        } else {
          maxSig_both=maxSig2;
          bestCut_both=bestCut2;
          bestDirection= "x < cut";
        }
    // === Plotting ===
       /* TCanvas* c = new TCanvas("c", "Significance Scan", 900, 700);
        c->SetGrid();

        TGraph* gRight = new TGraph(cuts_right.size(), &cuts_right[0], &sig_right[0]);
        gRight->SetLineColor(kBlue);
        gRight->SetLineWidth(2);
        gRight->SetTitle("Significance Scan;Cut Value;S / sqrt(S + B)");

        TGraph* gLeft = new TGraph(cuts_left.size(), &cuts_left[0], &sig_left[0]);
        gLeft->SetLineColor(kRed);
        gLeft->SetLineWidth(2);

        gRight->Draw("AL");
        gLeft->Draw("L SAME");

        auto legend = new TLegend(0.6, 0.75, 0.88, 0.88);
        legend->AddEntry(gRight, "Cut: x > value", "l");
        legend->AddEntry(gLeft,  "Cut: x < value", "l");
        legend->Draw();*/
        std::cout << "  Variable  = " << var.Data() << std::endl;
        std::cout << "  B = " << B_Cut1 << std::endl;
        std::cout << "  S    = " << S_Cut1 << std::endl;  
        std::cout << "  Max Significance1 = " << maxSig1 << std::endl;
        std::cout << "  Cut Direction1    = " << bestDirection1 << std::endl;
        std::cout << "  Cut Value1        = " << bestCut1 << std::endl;


        std::cout << "  B = " << B_Cut2 << std::endl;
        std::cout << "  S    = " << S_Cut2 << std::endl;
        std::cout << "  Max Significance2 = " << maxSig2 << std::endl;
        std::cout << "  Cut Direction    = " << bestDirection2 << std::endl;
        std::cout << "  Cut Value        = " << bestCut2 << std::endl;

        std::cout << "\n=== Best Significance ===" << std::endl;
        std::cout << "  Max Significance = " << maxSig_both << std::endl;
        std::cout << "  Cut Direction    = " << bestDirection << std::endl;
        std::cout << "  Cut Value        = " << bestCut_both << std::endl;

        if (0) { // NORMALIZE
              double int_sig     = hist_SIG->Integral();
              double int_bkg     = hist_BKG->Integral();
              double int_sig_wt  = hist_SIG_BOTH->Integral();

          if (int_sig > 0 || int_sig_wt > 0){     
                hist_SIG->Scale(1.0 / int_sig);
                hist_BKG->Scale(1.0 / int_bkg);
                hist_SIG_BOTH->Scale(1.0 / int_sig_wt); 
          }
        }

        // Draw the histograms
      /*  hist->SetStats(0);
        if (SELplots && path_to_file.Contains("Bd")){
            hist_SIG_BOTH->Draw("HIST");
        } else {
            hist_SIG->Draw("HIST");
        }
        hist_BKG->Draw("HIST SAMES");
*/

        double x_cut = bestCut_both;        
        double y_min = 0;
        double y_max = hist_BKG->GetMaximum();

        TLine* line = new TLine(x_cut, y_min, x_cut, y_max);
        line->SetLineColor(kRed);
        line->SetLineWidth(2);
        line->SetLineStyle(2);  // dashed line
        line->Draw("SAME");

        TString particleNAME = "Bu";
        TString systemNAME = "SG_ppRef_";
        if (path_to_file.Contains("Bs")){
            particleNAME = "Bs";
        } else if (path_to_file.Contains("Bd")){
            particleNAME = "Bd";
        } else if (path_to_file.Contains("Rho")){
            particleNAME = "X3872";
        } else if (path_to_file.Contains("PSI2S")){
            particleNAME = "PSI2S";
        }

        if (path_to_file.Contains("PbPb"))  { systemNAME = "SG_PbPb_";}



 /* // === Plotting ===
    TCanvas* c = new TCanvas("c", "", 900, 700);
    c->SetGrid();

    double xMin_R = cuts_right.front();
    double xMax_R = cuts_right.back();
    double xMin_L = cuts_left.front();
    double xMax_L = cuts_left.back();

    double yMax_R = *std::max_element(sig_right.begin(), sig_right.end());

    TGraph* gRight = new TGraph(cuts_right.size(), &cuts_right[0], &sig_right[0]);
    gRight->SetLineColor(kBlue);
    gRight->SetLineWidth(2);
    gRight->SetTitle("");
    gRight->GetXaxis()->SetTitle(Form("Cut Value, %s", var.Data()));
    gRight->GetYaxis()->SetTitle("Significance");

    gRight->GetXaxis()->SetTitleSize(0.045);
    gRight->GetXaxis()->SetTitleOffset(1.1);
    gRight->GetYaxis()->SetTitleSize(0.045);
    gRight->GetYaxis()->SetTitleOffset(1.2);


    TGraph* gLeft = new TGraph(cuts_left.size(), &cuts_left[0], &sig_left[0]);
    gLeft->SetLineColor(kRed);
    gLeft->SetLineWidth(2);

    gRight->GetXaxis()->SetLimits(xMin_R, xMax_R);
    gRight->GetYaxis()->SetRangeUser(0, maxSig_both*1.1);
    gLeft->GetXaxis()->SetLimits(xMin_L, xMax_L);
    gLeft->GetYaxis()->SetRangeUser(0, maxSig_both*1.1);


    gRight->Draw("AL");
    gLeft->Draw("L SAME");



// draw a vertical dashed line at the best cut 
TLine* bestLine = new TLine(bestCut_both, 0, bestCut_both, maxSig_both*1.1);
bestLine->SetLineColor(kOrange+7);
bestLine->SetLineStyle(2);
bestLine->SetLineWidth(2);
bestLine->Draw("SAME");

// draw a marker at the maximum-significance point
TMarker* bestMark = new TMarker(bestCut_both, maxSig_both, 29);
bestMark->SetMarkerColor(kMagenta);
bestMark->SetMarkerSize(1.2);
bestMark->Draw("SAME");
  

    auto legend = new TLegend(0.6, 0.75, 0.88, 0.88);
    legend->SetHeader(particleNAME, "");
    legend->AddEntry(gRight, "Cut: x > value", "l");
    legend->AddEntry(gLeft,  "Cut: x < value", "l");
// make the text smaller
legend->SetTextSize(0.025);
legend->SetTextFont(62);

// shrink & move the legend box by tweaking its NDC corners
legend->SetX1NDC(0.65);  // left
legend->SetX2NDC(0.85);  // right
legend->SetY1NDC(0.80);  // bottom
legend->SetY2NDC(0.90);  // top

// optionally remove the fill to make it less obtrusive
legend->SetFillStyle(0);
legend->SetBorderSize(0);
legend->Draw();

        c->SaveAs(Form("./%s%s%s_%s%s.png", dirNAME.Data(), systemNAME.Data() , var.Data(), particleNAME.Data(), cutlevel.Data()));*/
        
    // === Plotting: RIGHT-hand cut (cut < x) ===
TCanvas* cRight = new TCanvas("cRight", "Right-side Cut: cut < x", 900, 700);
cRight->SetGrid();

TGraph* gRight = new TGraph(cuts_right.size(), &cuts_right[0], &sig_right[0]);
gRight->SetLineColor(kBlue);
gRight->SetLineWidth(2);
gRight->SetTitle(Form(""));
gRight->GetXaxis()->SetTitle(Form("Cut Value, (%s)", var.Data()));
gRight->GetYaxis()->SetTitle("Significance");
gRight->GetXaxis()->SetTitleSize(0.045);
gRight->GetXaxis()->SetTitleOffset(1.1);
gRight->GetYaxis()->SetTitleSize(0.045);
gRight->GetYaxis()->SetTitleOffset(1.2);
gRight->GetYaxis()->SetRangeUser(0, maxSig_both * 1.1);
gRight->Draw("AL");

// Best marker & line for RIGHT cut
if (bestDirection == "cut < x") {
    TLine* bestLineRight = new TLine(bestCut1, 0, bestCut1, maxSig1 * 1.1);
    bestLineRight->SetLineColor(kOrange + 7);
    bestLineRight->SetLineStyle(2);
    bestLineRight->SetLineWidth(2);
    bestLineRight->Draw("SAME");

    TMarker* bestMarkRight = new TMarker(bestCut1, maxSig1, 29);
    bestMarkRight->SetMarkerColor(kMagenta);
    bestMarkRight->SetMarkerSize(1.5);
    bestMarkRight->Draw("SAME");
}
auto legendRight = new TLegend(0.6, 0.15, 0.88, 0.28);
legendRight->SetHeader(particleNAME, "");
legendRight->AddEntry(gRight,Form( "Cut: x > cut value"), "l");
legendRight->AddEntry(gRight,Form( "BestCut: x > %.3f", bestCut1), "l");
legendRight->AddEntry(gRight,Form( "MaxS = %.3f",maxSig1), "l");
// make the text smaller
legendRight->SetTextSize(0.035);  
legendRight->SetTextFont(62);

// shrink & move the legend box by tweaking its NDC corners
legendRight->SetX1NDC(0.65);  // left
legendRight->SetX2NDC(0.15);  // right
legendRight->SetY1NDC(0.80);  // bottom
legendRight->SetY2NDC(0.28);  // top

// optionally remove the fill to make it less obtrusive
//legendRight->SetFillStyle(0);  
//legendRight->SetBorderSize(0);

legendRight->Draw();

// Save the canvas as an image
cRight->SaveAs(Form("./%s%s%s_%s%s_RIGHT.png", dirNAME.Data(), systemNAME.Data(), var.Data(), particleNAME.Data(), cutlevel.Data()));

// === Plotting: LEFT-hand cut (x < cut) ===
TCanvas* cLeft = new TCanvas("cLeft", "Left-side Cut: x < cut", 900, 700);
cLeft->SetGrid();

TGraph* gLeft = new TGraph(cuts_left.size(), &cuts_left[0], &sig_left[0]);
gLeft->SetLineColor(kRed);
gLeft->SetLineWidth(2);
gLeft->SetTitle(Form(""));
gLeft->GetXaxis()->SetTitle(Form("Cut Value (%s)", var.Data()));
gLeft->GetYaxis()->SetTitle("Significance");
gLeft->GetXaxis()->SetTitleSize(0.045);
gLeft->GetXaxis()->SetTitleOffset(1.1);
gLeft->GetYaxis()->SetTitleSize(0.045);
gLeft->GetYaxis()->SetTitleOffset(1.2);
gLeft->GetYaxis()->SetRangeUser(0, maxSig_both * 1.1);
gLeft->Draw("AL");

// Best marker & line for LEFT cut
if (bestDirection == "x < cut") {
    TLine* bestLineLeft = new TLine(bestCut2, 0, bestCut2, maxSig2 * 1.1);
    bestLineLeft->SetLineColor(kOrange + 7);
    bestLineLeft->SetLineStyle(2);
    bestLineLeft->SetLineWidth(2);
    bestLineLeft->Draw("SAME");

    TMarker* bestMarkLeft = new TMarker(bestCut2, maxSig2, 29);
    bestMarkLeft->SetMarkerColor(kMagenta);
    bestMarkLeft->SetMarkerSize(1.5);
    bestMarkLeft->Draw("SAME");
}
auto legendLeft = new TLegend(0.6, 0.15, 0.88, 0.28);
legendLeft->SetHeader(particleNAME, "");

legendLeft->AddEntry(gLeft,Form( "Cut: x < cut value"), "l");
legendLeft->AddEntry(gLeft,Form( "BestCut: x < %.3f", bestCut2), "l");
legendLeft->AddEntry(gLeft,Form( "MaxS = %.3f",maxSig2), "l");
// make the text smaller
legendLeft->SetTextSize(0.035);  
legendLeft->SetTextFont(62);

// shrink & move the legend box by tweaking its NDC corners
legendLeft->SetX1NDC(0.65);  // left
legendLeft->SetX2NDC(0.15);  // right
legendLeft->SetY1NDC(0.80);  // bottom
legendLeft->SetY2NDC(0.28);  // top

// optionally remove the fill to make it less obtrusive
//legendLeft->SetFillStyle(0);  
//legendLeft->SetBorderSize(0);

legendLeft->Draw();

cLeft->SaveAs(Form("./%s%s%s_%s%s_LEFT.png", dirNAME.Data(), systemNAME.Data(), var.Data(), particleNAME.Data(), cutlevel.Data()));


      }

      //  c->SaveAs("significance_scan_both.pdf");
        delete hist_SIG;
        delete hist_SIG_WT;
        delete hist_SIG_BOTH;
        delete hist_BKG;
        delete hist;
        delete canvas;
        
 
}
        // Clean up
       
       
        
}

}



int main() {
    testing_mod();
    return 0;
}
