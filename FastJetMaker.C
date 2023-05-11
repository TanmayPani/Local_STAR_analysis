#define FastJetMaker_cxx

#include "FastJetMaker.h"
#include "TStarEventClass/TStarTrack.h"
#include "TStarEventClass/TStarTower.h"

FastJetMaker::FastJetMaker(){
    jet_def.reset(new JetDefinition(antikt_algorithm, 0.4, BIpt2_scheme, Best));   
}

FastJetMaker::FastJetMaker(JetAlgorithm algo, float R, RecombinationScheme reco_sch){
    jet_def.reset(new JetDefinition(algo, R, reco_sch, Best));
    //cout<<jet_def->description()<<endl;
}

FastJetMaker::~FastJetMaker(){
     
}

void FastJetMaker::FastJetAreaMaker(AreaType area_type, float MaxRap){
    area_spec.reset(new GhostedAreaSpec(MaxRap));
	area_def.reset(new AreaDefinition(area_type, *area_spec));
    doJetAreas = true;
}

void FastJetMaker::InputForClustering(int i, short ch, float px, float py, float pz, float E){
    PseudoJet constit(px, py, pz, E);
    constit.set_user_info(new UserInfo(i, ch));
    full_event.push_back(move(constit));
}

void FastJetMaker::InputForClustering(int i, TStarTrack *trk){
    PseudoJet constit(trk->Px(), trk->Py(), trk->Pz(), trk->Pi0E());
    constit.set_user_info(new UserInfo(i, trk->Charge()));
    full_event.push_back(move(constit));
}

void FastJetMaker::InputForClustering(int i, TStarTower *tow){
    PseudoJet constit(tow->Px(), tow->Py(), tow->Pz(), tow->E());
    constit.set_user_info(new UserInfo(i, 0));
    full_event.push_back(move(constit));
}

//void FastJetMaker::InputForClustering(int i, short ch, float pt, float eta, float phi, float M){
//    float px = pt * cos(phi);
//    float py = pt * sin(phi);
//    float pz = pt * sinh(eta);
//    float E = sqrt(px*px + py*py + pz*pz + M*M);
//    PseudoJet constit(px, py, pz, E);
//    constit.set_user_info(new UserInfo(i, ch));
//    full_event.push_back(move(constit));
//}

vector<PseudoJet> FastJetMaker::GetJets(){
    jet_constituents = ConstituentSelector(full_event);
    full_event.clear();
    vector<PseudoJet> jets;
    if(!doJetAreas){
        CS.reset(new ClusterSequence(jet_constituents, *jet_def));
        jets = JetSelector(CS->inclusive_jets());
    }else{
        CS_Area.reset(new ClusterSequenceArea(jet_constituents, *jet_def, *area_def));
        jets = JetSelector(no_ghost(CS_Area->inclusive_jets()));
    }
    jet_constituents.clear();
    return jets;
}

vector<PseudoJet>* FastJetMaker::GetConstituents(PseudoJet *jet){
    jet_constituents.clear();
    jet_constituents =  jet->constituents();
    return static_cast<vector<PseudoJet>*>(&jet_constituents);
}

bool FastJetMaker::IsConstituentGhost(PseudoJet *constit){
    if(!doJetAreas)return false;
    else return !constit->has_user_info();
}

void FastJetMaker::Clear(){
    if(!full_event.empty())full_event.clear();
    if(!jet_constituents.empty())jet_constituents.clear();

    if(CS_Area)CS_Area.reset();  
    if(CS)CS.reset();
}

