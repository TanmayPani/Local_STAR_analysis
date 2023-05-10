#define DataQA_cxx

#include "DataQA.h"
#include "FastJetMaker.h"
#include "TStarEventClass/TStarEvent.h"
#include "TStarEventClass/TStarTrack.h"

void DataQA::DeclareHistograms(){
   hJetPt = new TH1F("hJetPt", "Jet Pt", 60, 0, 60);
}

void DataQA::Loop(FastJetMaker *fj = nullptr){

   DeclareHistograms(); 

   if(!fj){
      cout<<"FastJetMaker not supplied, making one here..."<<endl;
      fj = new FastJetMaker(antikt_algorithm, 0.4, BIpt2_scheme); //Define FastJet JetDefinition
	   fj->SetConstituentPtMin(2.0);
	   fj->SetConstituentAbsEtaMax(1.0);
	   fj->SetJetPtMin(10.0);
	   fj->SetJetAbsEtaMax(0.6);
      fj->FastJetAreaMaker(active_area_explicit_ghosts, 1.2);
   }

   int NJets = 0;

//   for (int ievt=0; ievt<_Chain->GetEntriesFast(); ievt++){    
   for (int ievt=50000; ievt<60000; ievt++){    
      _Chain->GetEntry(ievt);
      fj->EmptyAllVectors();

      //if(!Event->IsHT2())continue;

      _Tracks = static_cast<TClonesArray*>(Event->GetTracks());

      for(int itrk = 0; itrk < _Tracks->GetEntriesFast(); itrk++){
         TStarTrack *trk = static_cast<TStarTrack*>(_Tracks->At(itrk));
         //hTr_Pt->Fill(trk->Pt());
         fj->InputForClustering(itrk, trk->Charge(), trk->Px(), trk->Py(), trk->Pz(), trk->Pi0E());

         //delete trk;
      }

      _Towers = static_cast<TClonesArray*>(Event->GetTowers());

      for(int itow = 0; itow < _Towers->GetEntriesFast(); itow++){
         TStarTower *tow = static_cast<TStarTower*>(_Towers->At(itow));
         fj->InputForClustering(itow, 0, tow->Px(), tow->Py(), tow->Pz(), tow->E());

         //delete tow;
      }

      Jets = fj->GetClusteredJets();

      NJets += Jets->size();

      for(int ijet = 0; ijet < Jets->size(); ijet++){
         PseudoJet *_jet = static_cast<PseudoJet*>(&(Jets->at(ijet)));

         cout<<_jet->perp()<<endl;

         hJetPt->Fill(_jet->perp());

         //delete _jet;
      }

      if(ievt%1000==0)cout<<"Event #: "<<ievt+1<<" NJets: "<<NJets<<endl;

   }

   hJetPt->Draw("EHIST");

}
