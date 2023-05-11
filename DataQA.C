#define DataQA_cxx

#include "DataQA.h"
#include "TStarEventClass/TStarEvent.h"
#include "TStarEventClass/TStarTrack.h"
#include "TStarEventClass/TStarTower.h"

void DataQA::DeclareHistograms(){
   hJetPt = new TH1F("hJetPt", "Jet Pt", 60, 0, 60);
   hJetEta = new TH1F("hJetEta", "Jet #eta", 20, -1., 1.);
   hJetPhi = new TH1F("hJetPhi", "Jet #phi", 60, 0., twopi);
}

void DataQA::LoopOverRuns(){
   DeclareHistograms();

   ifstream runList(run_list.c_str(), ios::in);
   if(!runList){
      cerr<<"ERROR: Unable to open input list file '"<<run_list<<"'."<<endl;
      return;
   }
      
   int count = 0;
   while(getline(runList, runNumber) && count < MaxFiles){
      cout<<"processing file #"<<count+1<<" ("<< file_path<<runNumber<<".root)..."<<endl;  

      _File.reset(new TFile((file_path+runNumber+".root").c_str(), "READ"));
      if(_File->IsZombie()){
         cerr<<"ERROR: Unable to open TTree TFile '"<<file_path<<runNumber<<".root'"<<endl;
         _File->Close();
         continue; 
      }

      cout<<"File not Zombie :) Getting TTree from file..."<<endl;

      _Tree.reset((TTree*)_File->Get("Event_Info"));
      if(!_Tree){
         cerr<<"ERROR: Can't obtain TTree!"<<endl;
      }

      _Tree->SetBranchAddress("Events", &Event, &b_Event);

      LoopOverEvents();

      count++;

      if(_Tree)_Tree.reset();
      if(_File->IsOpen())_File->Close();
      if(_File)_File.reset();
   }

   WriteOutput();
}

void DataQA::LoopOverEvents(){
   int NEvents = _Tree->GetEntriesFast();

   cout<<"Analyzing "<<NEvents<<" events from Run with ID:"<<runNumber<<endl;

   int NRunJets = 0;

   for (int ievt=0; ievt < NEvents; ievt++){ 
      _Tree->GetEntry(ievt);

      if(!Event->IsHT2())continue;

      //cout<<"Filling tracks into FastJetMaker..."<<endl;
      TClonesArray* _Tracks = static_cast<TClonesArray*>(Event->GetTracks());
      for(int itrk = 0; itrk < _Tracks->GetEntriesFast(); itrk++){
         TStarTrack* _track = static_cast<TStarTrack*>(_Tracks->At(itrk));
         fjMaker->InputForClustering(itrk, _track);
      }
      //cout<<"Filling towers into FastJetMaker..."<<endl;
      TClonesArray* _Towers = static_cast<TClonesArray*>(Event->GetTowers());
      for(int itow = 0; itow < _Towers->GetEntriesFast(); itow++){
         TStarTower* _tower = static_cast<TStarTower*>(_Towers->At(itow));
         fjMaker->InputForClustering(itow, _tower);
      }
      //cout<<"Getting clustered jets..."<<endl;

      vector<PseudoJet> Jets = fjMaker->GetJets();

      //cout<<"Running over jets..."<<endl;
      int NJets = Jets.size();
      if(NJets < 1) continue;
      
      for(PseudoJet& _jet : Jets){
         //cout<<_jet.perp()<<endl;
         hJetPt->Fill(_jet.perp());
         hJetEta->Fill(_jet.eta());
         hJetPhi->Fill(_jet.phi());
      }
      //cout<<"Ran over "<<NJets<<" jets"<<endl;
      NRunJets += NJets;

      //cout<<"clearing fjMaker..."<<endl;
      fjMaker->Clear(); 

      if(ievt%1000==0)cout<<"On Event #: "<<ievt+1<<endl;
   }
   cout<<"Clustered "<<NRunJets<<" jets from run with ID: "<<runNumber<<endl; 
}
