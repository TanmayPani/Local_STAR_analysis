#ifndef DataQA_h
#define DataQA_h

class TStarEvent;
class FastJetMaker;

class DataQA {
public :
   string run_list = "";
   string file_path = "";
   TChain *_Chain = nullptr;   //!pointer to the analyzed TTree or TChain
   TFile *file = nullptr;

   TClonesArray *_Tracks = nullptr;
   TClonesArray *_Towers = nullptr;

   vector<PseudoJet>* Jets = nullptr;

   static const int MaxFiles = 100;

   TH1F *hEventStats;
   TH1F *hTrackStats;
   TH1F *hTowerStats;

   TH1F *hJetPt;
   TH1F *hJetEta;
   TH1F *hJetPhi;

   TH1F *tempEventStats[MaxFiles];
   TH1F *tempTrackStats[MaxFiles];
   TH1F *tempTowerStats[MaxFiles];

   TList *_EventStatsList;
   TList *_TrackStatsList;
   TList *_TowerStatsList;

   // Declaration of leaf types
   TStarEvent       *Event;
   // List of branches
   TBranch        *b_Event;   //!

   DataQA();
   virtual ~DataQA();
   void BuildChain();
   void DeclareHistograms();
   void Loop(FastJetMaker *fj = nullptr); 
   //void DeclareHistograms();
};

#endif

#ifdef DataQA_cxx
DataQA::DataQA(){
   run_list = "../DATAFILES/AuAu_200_production_mid_2014_GOOD.list"; 
   file_path = "../DATAFILES/AuAu_200_MidLumi2014_100Runs/EventTree_";

   BuildChain(); 
}

DataQA::~DataQA(){
   if(_Tracks)delete _Tracks;
   if(_Towers)delete _Towers;
   if(Jets)delete Jets;
   if(_Chain)delete _Chain;
}

void DataQA::BuildChain(){
   ifstream runList(run_list.c_str(), ios::in);
   if(!runList){
      cerr<<"ERROR: Unable to open input list file '"<<run_list<<"'."<<endl;
      return;
   }

   _Chain = new TChain("AuAu_200_MidLumi2014_100Runs");

   _EventStatsList = new TList();
   _TrackStatsList = new TList();
   _TowerStatsList = new TList();

   string runNumber;
   int count = 0;
   while(getline(runList, runNumber) && count < MaxFiles){
      _Chain->Add((file_path+runNumber+".root?#Event_Info").c_str());
      file = new TFile((file_path+runNumber+".root").c_str());

      if(file->IsZombie()){
         cerr<<"ERROR: Unable to open TTree TFile '"<<file_path<<runNumber<<".root'"<<endl;
         file->Close();
         delete file;
         continue; 
      } 

      tempEventStats[count] = (TH1F*)file->Get("hEventStats");
      tempTrackStats[count] = (TH1F*)file->Get("hTrackStats");
      tempTowerStats[count] = (TH1F*)file->Get("hTowerStats");

      tempEventStats[count]->SetName(Form("hEventStats_%i", count));
      tempTrackStats[count]->SetName(Form("hTrackStats_%i", count));
      tempTowerStats[count]->SetName(Form("hTowerStats_%i", count));

      if (tempEventStats[count] && tempTrackStats[count] && tempTowerStats[count]) { 
         _EventStatsList->Add(tempEventStats[count]);
         _TrackStatsList->Add(tempTrackStats[count]);
         _TowerStatsList->Add(tempTowerStats[count]);
      }else{
         cerr << "ERROR: Unable to retrieve histograms from file '" << file_path << runNumber << ".root'" << endl;
      }

      count++;

      cout<<"processed file "<<count<<": "<< file_path<<runNumber<<".root"<<endl;     

      //delete file;
   }
   _Chain->SetBranchAddress("Events", &Event, &b_Event);

   cout<<"Total number of entries in the chain: "<<_Chain->GetEntries()<<endl;

   hEventStats = (TH1F*)tempEventStats[0]->Clone("_hEventStats");
   hEventStats->Reset();
   hEventStats->Merge(_EventStatsList);

   cout<<"Number of good events: "<<hEventStats->GetBinContent(8)<<endl;

   hTrackStats = (TH1F*)tempTrackStats[0]->Clone("_hTrackStats");
   hTrackStats->Reset();
   hTrackStats->Merge(_TrackStatsList);

   hTowerStats = (TH1F*)tempTowerStats[0]->Clone("_hTowerStats");
   hTowerStats->Reset();
   hTowerStats->Merge(_TowerStatsList);

   file->Close();
   delete file;

}

#endif // #ifdef DataQA_cxx
