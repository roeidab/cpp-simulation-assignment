#pragma once
#include <string>
#include <vector>
#include "Facility.h"
#include "Plan.h"
#include "Settlement.h"
#include "Auxiliary.h"
#include <fstream> //to parse the file
#include "Action.h" 

using std::string;
using std::vector;

class BaseAction;
class SelectionPolicy;

class Simulation {
    public:
        Simulation(const string &configFilePath);
        Simulation(const Simulation& other);//copy constructor
        ~Simulation(); //destructor
        Simulation& operator=(const Simulation& other); //copy assginment operator
        Simulation(Simulation&& other); //move construcor
        Simulation& operator=(Simulation&& other) noexcept; //move assignment operator

        void start();
        void addPlan(const Settlement &settlement, SelectionPolicy *selectionPolicy);
        void addAction(BaseAction *action);
        bool addSettlement(Settlement *settlement);
        bool addFacility(FacilityType facility);
        bool isSettlementExists(const string &settlementName);
        Settlement &getSettlement(const string &settlementName);
        Plan &getPlan(const int planID);
        void step();
        void close();
        void open();

        //added methods
        SelectionPolicy* createSelectionPolicy(const string &policy);
        const bool isPlanExists(const int planID) const;
        void printLog();

    private:
        bool isRunning; //Indicates whether the simulation should be ended
        int planCounter; //For assigning unique plan IDs
        vector<BaseAction*> actionsLog;
        vector<Plan> plans;
        vector<Settlement*> settlements;
        vector<FacilityType> facilitiesOptions;

};