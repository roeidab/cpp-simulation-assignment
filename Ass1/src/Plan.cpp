#include "Plan.h"
#include <iostream>
using namespace std;


Plan::Plan(const int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy, const vector<FacilityType> &facilityOptions)
: plan_id(planId),settlement(settlement),
  selectionPolicy(selectionPolicy),status(PlanStatus::AVALIABLE),facilities(),
   underConstruction(), facilityOptions(facilityOptions),
   life_quality_score(0), economy_score(0), environment_score(0)
{
}

Plan::Plan(const Plan& other):
plan_id(other.plan_id),
 settlement(other.settlement), selectionPolicy(other.selectionPolicy->clone()),
 status(other.status), facilities(), underConstruction(),facilityOptions(other.facilityOptions),
   life_quality_score(other.life_quality_score), economy_score(other.economy_score), environment_score(other.environment_score)
{
    for (const Facility* fac : other.facilities) {
        if (fac != nullptr) 
        {
            facilities.push_back(fac->clone()); 
        } 
    }
        
    for (const Facility* fac : other.underConstruction) {
        if (fac != nullptr) 
        {
            underConstruction.push_back(fac->clone()); 
        }
    } 
}

Plan::Plan(Plan&& other) noexcept:
plan_id(other.plan_id),
 settlement(other.settlement),selectionPolicy(other.selectionPolicy), status(other.status), 
  facilities(other.facilities), underConstruction(other.underConstruction),
   facilityOptions(other.facilityOptions),
   life_quality_score(other.life_quality_score), economy_score(other.economy_score), environment_score(other.environment_score)
{
    other.selectionPolicy = nullptr;
    other.facilities.clear();
    other.underConstruction.clear();
}

Plan::~Plan() {
        delete selectionPolicy; 
        for (Facility* facility : facilities) {
            delete facility; 
        }
        for (Facility* facility : underConstruction) {
            delete facility; 
        }
    };

Plan::Plan(const Plan& other,const Settlement &settlement)
:plan_id(other.plan_id),settlement(settlement),selectionPolicy(other.selectionPolicy->clone()),status(other.status)
,facilities(),underConstruction(),facilityOptions(other.facilityOptions),
life_quality_score(other.life_quality_score),economy_score(other.economy_score),environment_score(other.environment_score)
{
    for (const Facility* fac : other.facilities) {
        if (fac != nullptr) 
        {
            facilities.push_back(fac->clone()); 
        } 
    }
        
    for (const Facility* fac : other.underConstruction) {
        if (fac != nullptr) 
        {
            underConstruction.push_back(fac->clone()); 
        }
    }
}

const int Plan::getlifeQualityScore() const
{
    return life_quality_score;
}

const int Plan::getEconomyScore() const
{
    return economy_score;
}

const int Plan::getEnvironmentScore() const
{
    return environment_score;
}

void Plan::setSelectionPolicy(SelectionPolicy *selectionPolicy)
{
    if (this->selectionPolicy != nullptr) { // Free the old memory of policy
        delete this->selectionPolicy; 
    }
    this->selectionPolicy = selectionPolicy;  
}

void Plan::step()
{
    std::size_t conLimit = static_cast<std::size_t>(settlement.getConstructionLimit());
    
    while(underConstruction.size()< conLimit && status==PlanStatus::AVALIABLE){//select and add facilities as long as available 
        Facility* newfacility = new Facility(selectionPolicy->selectFacility(facilityOptions),settlement.getName());
        addFacility(newfacility);
    }
    //process facilities under construction
    for(int i = underConstruction.size()-1;i>=0;i--){
        FacilityStatus stat = underConstruction[i]->step();
        if(stat == FacilityStatus::OPERATIONAL){
            addFacility(underConstruction[i]);
            underConstruction.erase(underConstruction.begin()+i);
        }
    }
    if (underConstruction.size() == conLimit)
    {
        status = PlanStatus::BUSY; 
    }
    else{
        status = PlanStatus::AVALIABLE; 
    }
}

void Plan::closePrint()
{
    std::cout<<"PlanID: " << plan_id << std::endl;
    std::cout<<"SettlementName: " << settlement.getName() << std::endl;
    std::cout<<"LifeQualityScore: " << life_quality_score << std::endl;
    std::cout<<"EconomyScore: " << economy_score << std::endl;
    std::cout<<"EnvironmentScore: " << environment_score << std::endl;
}

void Plan::printStatus()
{
    string st;
    if (status==PlanStatus::AVALIABLE)
    {
        st =  "AVALIABLE";
    }
    else{
        st = "BUSY";
    }
    std::cout<<"PlanID: " << plan_id << std::endl;
    std::cout<<"SettlementName: " << settlement.getName() << std::endl;
    std::cout<<"PlanStatus: " << st << std::endl;
    std::cout<<"SelectionPolicy: " << selectionPolicy->stringShortPolicy() << std::endl;
    std::cout<<"LifeQualityScore: " << life_quality_score << std::endl;
    std::cout<<"EconomyScore: " << economy_score << std::endl;
    std::cout<<"EnvironmentScore: " << environment_score << std::endl;

    for (Facility* fac: facilities)
    {
        std::cout<< fac->toString() << endl;
    }
    for (Facility* fac: underConstruction)
    {
        std::cout<< fac->toString() << endl;
    }
}
    

const vector<Facility *> &Plan::getFacilities() const
{
    return this->facilities;
}

void Plan::addFacility(Facility *facility)
{
    if (facility->getStatus() == FacilityStatus::OPERATIONAL){
        facilities.push_back(facility);
        economy_score += facility->getEconomyScore();
        environment_score += facility->getEnvironmentScore();
        life_quality_score += facility->getLifeQualityScore();
    }
    else{
        underConstruction.push_back(facility);
    }
}

const string Plan::toString() const
{
    return string();
}

const int Plan::getPlanID() const
{
    return plan_id;
}

const string Plan::getPolicyString() const // returns "nve" "bal" /"eco" / "env"
{
    return selectionPolicy->stringShortPolicy();
}

const string Plan::getSettlementName() const
{
    return settlement.getName();
}

const SettlementType Plan::getSettlementType() const
{
    return settlement.getType();
}
