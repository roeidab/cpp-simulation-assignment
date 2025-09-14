#include "Action.h"
#include "Simulation.h"
//Class: BaseAction 

BaseAction::BaseAction():errorMsg(),status()
{
}

ActionStatus BaseAction::getStatus() const
{
    return status;
}

void BaseAction::complete()
{
    status = ActionStatus::COMPLETED;
}

void BaseAction::error(string errorMsg) // Updates the error message and sets status to ERROR
{
    this->errorMsg = errorMsg;
    std::cout << "Error: " + errorMsg << std::endl;
    status = ActionStatus::ERROR;
}

const string &BaseAction::getErrorMsg() const
{
    return errorMsg;
}

const std::string BaseAction::ActionStatusToString(ActionStatus stat) const {
    switch (stat) {
        case ActionStatus::COMPLETED: 
            return "COMPLETED";
            break;
        case ActionStatus::ERROR: 
            return "ERROR";
            break;
        default:
            return "INVALID_STATUS"; // will never reach here, only 2 options for ActionStatus stat, added to remove warnings from compiler.
    }
}


//Class - SimulateStep : BaseAction
// This class shouldn't raise an error

SimulateStep::SimulateStep(const int numOfSteps): numOfSteps(numOfSteps)
{
}

void SimulateStep::act(Simulation &simulation) //Iteraes through all plans and execute step() method
{
    for (int i = 1; i <= numOfSteps; i++)
    {
        simulation.step();
    }
    complete();
    
}

const string SimulateStep::toString() const 
{
    string s = ("step " + std::to_string(numOfSteps) + " " + ActionStatusToString(this->getStatus()));
    return s;
}

SimulateStep *SimulateStep::clone() const
{
    return new SimulateStep(*this);
}

//Class - AddPlan: BaseAction
//Should result an error if the provided settlement name or policy doesn't exist

AddPlan::AddPlan(const string &settlementName, const string &selectionPolicy): settlementName(settlementName), selectionPolicy(selectionPolicy)
{

}

void AddPlan::act(Simulation &simulation) 
{
    SelectionPolicy* policy = simulation.createSelectionPolicy(selectionPolicy);
    if (simulation.isSettlementExists(settlementName) && policy != nullptr)
    {
        simulation.addPlan(simulation.getSettlement(settlementName),policy);
        this->complete();
    }
    else{ //Updates errorMsg if the provided settlement name or policy doesn't exists
        this->error("Cannot create this plan");
    }
}
    
const string AddPlan::toString() const
{
    return ("plan " + settlementName +" " + selectionPolicy + " " + ActionStatusToString(this->getStatus()));
}

AddPlan *AddPlan::clone() const
{
    return new AddPlan(*this);
}
//Class - AddSettlement:BaseAction
AddSettlement::AddSettlement(const string &settlementName, SettlementType settlementType):settlementName(settlementName),settlementType(settlementType)
{
}

void AddSettlement::act(Simulation &simulation)
{
    if(simulation.isSettlementExists(settlementName)){
        this->error("Settlement already exists");
    }
    else{
        Settlement* s = new Settlement(settlementName,settlementType);
        simulation.addSettlement(s);
        this->complete();
    }
}

AddSettlement *AddSettlement::clone() const
{
    return new AddSettlement(*this);
}

const string AddSettlement::toString() const
{
    return ("Settlement "+settlementName+" "+std::to_string(static_cast<int>(settlementType))+""+ActionStatusToString(this->getStatus()));
}

// Class -  AddFacility:BaseAction
// Should result an error if the facility name is already exist

AddFacility::AddFacility(const string &facilityName, const FacilityCategory facilityCategory,const int price,
 const int lifeQualityScore, const int economyScore, const int environmentScore) :
 facilityName(facilityName), facilityCategory(facilityCategory), price(price),
 lifeQualityScore(lifeQualityScore), economyScore(economyScore), environmentScore(environmentScore)
{
}

void AddFacility::act(Simulation &simulation)
{
    FacilityType fac = FacilityType(facilityName, facilityCategory, price, lifeQualityScore, economyScore, environmentScore);
    bool completed = simulation.addFacility(fac);
    if (!completed)
    {
        this->error("Facility already exists");
    }
    else
    {
        this->complete();
    }
}

AddFacility *AddFacility::clone() const
{
    return new AddFacility(*this);
}

const string AddFacility::toString() const
{
    return "facility " + facilityName + " " + std::to_string(static_cast<int>(facilityCategory)) 
               + " " + std::to_string(price) 
               + " " + std::to_string(lifeQualityScore) 
               + " " + std::to_string(economyScore) 
               + " " + std::to_string(environmentScore) 
               + " " + ActionStatusToString(this->getStatus());
}

// Class - PrintPlanStatus

PrintPlanStatus::PrintPlanStatus(int planId): planId(planId)
{
}

void PrintPlanStatus::act(Simulation &simulation)
{
    if (simulation.isPlanExists(planId))
    {
        (simulation.getPlan(planId)).printStatus();
        this->complete();
    }
    else
    {
        this->error("Plan doesn't exists");
    }
    
}

PrintPlanStatus *PrintPlanStatus::clone() const
{
    return new PrintPlanStatus(*this);
}

const string PrintPlanStatus::toString() const
{
    return "planStatus "
     + std::to_string(static_cast<int>(planId)) +
     " " + ActionStatusToString(this->getStatus());;
}

// Class - ChangePlanPolicy: BaseAction

ChangePlanPolicy::ChangePlanPolicy(const int planId, const string &newPolicy): planId(planId), newPolicy(newPolicy)
{
}

void ChangePlanPolicy::act(Simulation &simulation)
{
    if (simulation.isPlanExists(planId))
    {
        Plan& p = simulation.getPlan(planId);
        string oldPolicy = p.getPolicyString();
        if (oldPolicy != newPolicy)
        {
            SelectionPolicy* policy =  simulation.createSelectionPolicy(newPolicy);
            p.setSelectionPolicy(policy);
            this->complete();
            string s =  "planID: " + std::to_string(planId)
               + "\n previousPolicy: " + oldPolicy 
               + "\n newPolicy: " + newPolicy;
            std::cout<< s <<std::endl;
        }
    }
    else
    {
        this->error("Cannot change selection policy");
    }
}

ChangePlanPolicy *ChangePlanPolicy::clone() const
{
    return new ChangePlanPolicy(*this);
}

const string ChangePlanPolicy::toString() const
{
   return "changePolicy "+std::to_string(planId) +" "+ newPolicy +" " +ActionStatusToString(this->getStatus());
}

// Class - PrintActionsLog:BaseAction

PrintActionsLog::PrintActionsLog()
{
}

void PrintActionsLog::act(Simulation &simulation)
{
    simulation.printLog();
    this->complete();
}

PrintActionsLog *PrintActionsLog::clone() const
{
    return new PrintActionsLog(*this);
}

const string PrintActionsLog::toString() const
{
    return "PrintActionsLog " + this->ActionStatusToString(this->getStatus());
}

// Class - Close: BaseAction

Close::Close()
{
}

void Close::act(Simulation &simulation)
{
    simulation.close();
    this->complete();
}

Close *Close::clone() const
{
    return new Close(*this);
}

const string Close::toString() const
{
    return "close " + this->ActionStatusToString(this->getStatus());
}

// Class - BackupSimulation: BaseAction
BackupSimulation::BackupSimulation()
{
}

void BackupSimulation::act(Simulation &simulation)
{
    if(backup!=nullptr){
        delete backup;
        backup = nullptr;
    }
    backup = new Simulation(simulation);
    this->complete();
}

BackupSimulation *BackupSimulation::clone() const
{
    return new BackupSimulation(*this);
}

const string BackupSimulation::toString() const
{
    return "backup " + this->ActionStatusToString(this->getStatus());
}

// Class - RestoreSimulation:BaseAction
RestoreSimulation::RestoreSimulation()
{
}

void RestoreSimulation::act(Simulation &simulation)
{
    if (backup != nullptr)
    {
        simulation = *backup;
        this->complete();
    }
    else
        error("No backup available");
}

RestoreSimulation *RestoreSimulation::clone() const
{
    return new RestoreSimulation(*this);
}

const string RestoreSimulation::toString() const
{
    return "restore " + this->ActionStatusToString(this->getStatus());
}
