#ifndef DataQA_h
#define DataQA_h

#include "FastJetMaker.h"

class TStarEvent;

class DataQA {
public :
   string run_list = "";
   string file_path = ""; 
   string runNumber = "";

   FastJetMaker *fjMaker = nullptr;

   unique_ptr<TFile> _File = nullptr; 
   unique_ptr<TTree> _Tree = nullptr;  

   int MaxFiles = 2;

   TH1F *hJetPt;
   TH1F *hJetEta;
   TH1F *hJetPhi;

   // Declaration of leaf types
   TStarEvent       *Event;
   // List of branches
   TBranch        *b_Event;   //!

   DataQA();
   virtual ~DataQA();
   void DeclareHistograms();
   void LoopOverRuns();
   void LoopOverEvents();
   void WriteOutput();
};

#endif

#ifdef DataQA_cxx
DataQA::DataQA(){
   run_list = "../DATAFILES/AuAu_200_production_mid_2014_GOOD.list"; 
   file_path = "../DATAFILES/AuAu_200_MidLumi2014_100Runs/EventTree_";

   fjMaker = new FastJetMaker(antikt_algorithm, 0.4, BIpt2_scheme); 
   fjMaker->SetConstituentPtMin(2.0);
   fjMaker->SetConstituentAbsEtaMax(1.0);
   fjMaker->SetJetPtMin(10.0);
   fjMaker->SetJetAbsEtaMax(0.6);
   fjMaker->FastJetAreaMaker(active_area_explicit_ghosts, 1.2);
   fjMaker->MakerDescription();
}

DataQA::~DataQA(){  
   if(fjMaker)delete fjMaker;
}

void DataQA::WriteOutput(){
   unique_ptr<TFile> _Out = make_unique<TFile>("Output_AuAu200_MidLumi2014_100Runs.root", "RECREATE");
   _Out->cd();
   hJetPt->Write();
   hJetEta->Write();
   hJetPhi->Write();
   _Out->Write();
   _Out->Close();
}

#endif // #ifdef DataQA_cxx
