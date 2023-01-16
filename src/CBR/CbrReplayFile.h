#pragma once
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/vector.hpp>
#include "Metadata.h"
#include "CbrCase.h"
#include "AnnotatedReplay.h"


struct CommandActions
{
    std::string moveName;
    std::vector<int> inputs;
    std::string priorMove;
    std::vector<int> altInputs;
};


class CbrReplayFile {
private:
    //friend class boost::serialization::access;
    std::vector<CbrCase> cbrCase;
    std::vector<int> input;
    std::array<std::string, 2> characterName;
    std::array<int, 2> characterId;
    static std::array< std::string, 13> neutralActions;
    bool stateChangeTrigger = false;
    bool commandInputNeedsResolving = false;
 
    std::string inputResolveName = "";

public:
    template<class Archive>
    void serialize(Archive& a, const unsigned version) {
        a& cbrCase& characterName & input &  stateChangeTrigger & commandInputNeedsResolving & characterId;
    }
    CbrReplayFile();
    //p1Charname p2Charname
    CbrReplayFile::CbrReplayFile(std::string p1, std::string p2, int p1ID, int p2ID);
    CbrReplayFile::CbrReplayFile(std::array< std::string, 2> arr, std::array< int, 2> arrId);
    void CopyInput(std::vector<int>);
    void AddCase(CbrCase);
    CbrCase* getCase(int);
    std::vector<CbrCase>* getCaseBase();
    int getCaseBaseLength();
    int getInput(int);

    int MakeCaseBase(AnnotatedReplay*, std::string, int, int, int);
    bool CheckCaseEnd(int, Metadata, std::string, bool);
    bool CheckNeutralState(std::string);
    std::vector<int> DeconstructInput(int, bool);
    std::vector < std::vector<signed int>> CheckCommandExecution(int, std::vector<std::vector<int>>);
    std::vector<std::vector<int>> MakeInputArray(std::string, std::vector<CommandActions>, std::string);
    std::vector<CommandActions> FetchCommandActions(std::string);
    int instantLearning(AnnotatedReplay*, std::string);
    int CbrReplayFile::makeFullCaseBase(AnnotatedReplay*, std::string);
    bool checkDirectionInputs(int direction, int input);
    bool isDirectionInputs(int direction);
    std::array<int, 2>& CbrReplayFile::getCharIds();
};


