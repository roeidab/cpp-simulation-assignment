#pragma once
#include <vector>
#include "Facility.h"
#include "Settlement.h"
#include "SelectionPolicy.h"
using std::vector;

enum class PlanStatus {
    AVALIABLE,
    BUSY,
};

class Plan {
    public:
        Plan(const int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy, const vector<FacilityType> &facilityOptions);
        Plan(const Plan& other); // Copy-constructor
        ~Plan(); // Destructor
        Plan& operator=(const Plan& other)=delete; //Copy-Assignment operator
        Plan(Plan&& other) noexcept; // Move constructor
        Plan& operator=(const Plan&& other)=delete; //Move-assignment operator
        //added constructor
        Plan(const Plan& other,const Settlement &settlement);

        const int getlifeQualityScore() const;
        const int getEconomyScore() const;
        const int getEnvironmentScore() const;
        void setSelectionPolicy(SelectionPolicy *selectionPolicy);
        void step();
        void printStatus();
        const vector<Facility*> &getFacilities() const;
        void addFacility(Facility* facility);
        const string toString() const;

        //added
        const int getPlanID() const;
        const string getPolicyString() const;
        const string getSettlementName() const;
        const SettlementType getSettlementType() const;
        const PlanStatus getPlanStatus() const;
        void closePrint();
        
    private:
        int plan_id;
        const Settlement &settlement;
        SelectionPolicy *selectionPolicy; //What happens if we change this to a reference?
        PlanStatus status;
        vector<Facility*> facilities; // Facilities that are operational
        vector<Facility*> underConstruction; // Facilities that UNDER_CONSTRUCTIONS
        const vector<FacilityType> &facilityOptions;
        int life_quality_score, economy_score, environment_score;
};