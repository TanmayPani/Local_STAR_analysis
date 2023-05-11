//Wrapper class for using the FastJet library...
//Uses ROOT6 classes and compiler setup for a few functionalities, but can be made ROOT independent easily

//Load fastjet libraries and main fastjet folders
R__ADD_INCLUDE_PATH(/opt/homebrew/Cellar/fastjet/3.4.0_1/include)
R__LOAD_LIBRARY(/opt/homebrew/Cellar/fastjet/3.4.0_1/lib/libfastjet.dylib)

#ifndef FastJetMaker_h
#define FastJetMaker_h

#include "fastjet/ClusterSequenceArea.hh"
#include "fastjet/Selector.hh"

using namespace fastjet;

class TStarTrack;
class TStarTower;

class FastJetMaker{
    public:
        FastJetMaker(); //default constructor, antikt, R=0.4, BIpt2 recombination
        FastJetMaker(JetAlgorithm algo, float R, RecombinationScheme reco_sch);
        virtual ~FastJetMaker();

        void FastJetAreaMaker(AreaType area_type, float MaxRap); //Will define an area for the jet cone, 
        //Calculating areas will increase computation time by a LOT, do this only for data and detector level
        //NO AREA NEEDED for particle level from MC simulations

        inline void InputForClustering(PseudoJet constit){full_event.push_back(move(constit));}
        inline void InputForClustering(int i, short ch, float px, float py, float pz, float E);
        inline void InputForClustering(int i, TStarTrack* trk);
        inline void InputForClustering(int i, TStarTower* trk);
        //inline void InputForClustering(int i, short ch, float pt, float eta, float phi, float M);

        vector<PseudoJet> GetJets(); //returns clustered vector<PseudoJet> of jets 
        vector<PseudoJet>* GetConstituents(PseudoJet* jet); //returns a pointer to the vector<PseudoJet> for constituents for a jet

        //Kinematic cuts for constituents, can add more as needed...
        void SetConstituentPtMin(float pt)     {ConstituentSelector = ConstituentSelector * SelectorPtMin(pt);}
        void SetConstituentPtMax(float pt)     {ConstituentSelector = ConstituentSelector * SelectorPtMax(pt);}
        void SetConstituentEtaMin(float eta)   {ConstituentSelector = ConstituentSelector * SelectorEtaMin(eta);}
        void SetConstituentEtaMax(float eta)   {ConstituentSelector = ConstituentSelector * SelectorEtaMax(eta);}
        void SetConstituentAbsEtaMax(float eta){ConstituentSelector = ConstituentSelector * SelectorAbsEtaMax(eta);}

        //Kinematic cuts for jets, can add more as needed...
        void SetJetPtMin(float pt)     {JetSelector = JetSelector * SelectorPtMin(pt);}
        void SetJetPtMax(float pt)     {JetSelector = JetSelector * SelectorPtMax(pt);}
        void SetJetEtaMin(float eta)   {JetSelector = JetSelector * SelectorEtaMin(eta);}
        void SetJetEtaMax(float eta)   {JetSelector = JetSelector * SelectorEtaMax(eta);}
        void SetJetAbsEtaMax(float eta){JetSelector = JetSelector * SelectorAbsEtaMax(eta);}

        void MakerDescription(){
            cout<<"Jet definition: "<<jet_def->description()<<endl;
            if(area_def)cout<<"Jet area definition: "<<area_def->description()<<endl;
            cout<<"Constituent selection: "<<ConstituentSelector.description()<<endl;
            cout<<"Jet selection: "<<JetSelector.description()<<endl;
        }

        bool IsConstituentGhost(PseudoJet *constit);

        void Clear();
    
    private:
        unique_ptr<JetDefinition> jet_def = nullptr; 
        //JetDefinition jet_def;
        bool doJetAreas = false;
        unique_ptr<AreaDefinition> area_def = nullptr;
        unique_ptr<GhostedAreaSpec> area_spec = nullptr;

        unique_ptr<ClusterSequenceArea> CS_Area = nullptr;
	    unique_ptr<ClusterSequence> CS = nullptr;

        Selector no_ghost = !SelectorIsPureGhost(); 
        Selector JetSelector = !SelectorIsZero();
        Selector ConstituentSelector = !SelectorIsZero(); 

        vector<PseudoJet> full_event;
        vector<PseudoJet> jet_constituents;

    //protected:
        //vector<PseudoJet> jets; //Final jets of the events are contained here 
};

class UserInfo: public PseudoJet::UserInfoBase{//User Info class to include non-kinematic details of the particles into the pseudojet objects...
public:
    UserInfo(const int indx, const short & ch):_charge(ch), _index(indx){}
    short getIndex() const {return _index;}
    short getCharge() const {return _charge;}
protected:
    int _index;
    short _charge;
};

#endif
