#include "Simulation.h"
//rule of 5
Simulation::Simulation(const string &configFilePath):isRunning(false),planCounter(0),actionsLog(),plans(),settlements(),facilitiesOptions()
{
    std::ifstream configFile(configFilePath);
    string l;
    while (std::getline(configFile, l))
    {
        vector<string> parsedArgs =Auxiliary::parseArguments(l);
        if(parsedArgs.at(0) == "settlement"){ //create a new settlement in the heap, using the second and third argument, converting the 3rd from string to int and then stlmntType.
            Settlement* newSettlement = new Settlement(parsedArgs.at(1),static_cast<SettlementType>(std::stoi(parsedArgs.at(2))));
            settlements.push_back(newSettlement);
        }
        else if(parsedArgs.at(0) == "facility"){
            FacilityType f = FacilityType(parsedArgs.at(1),static_cast<FacilityCategory>(std::stoi(parsedArgs.at(2))), std::stoi(parsedArgs.at(3)), std::stoi(parsedArgs.at(4)), std::stoi(parsedArgs.at(5)), std::stoi(parsedArgs.at(6)));
            facilitiesOptions.push_back(f);
        }
        else if(parsedArgs.at(0) == "plan"){
            Plan p = Plan(planCounter,getSettlement(parsedArgs.at(1)),createSelectionPolicy(parsedArgs.at(2)),facilitiesOptions);
            plans.push_back(p);
            planCounter++; //push counter by one
        }
        else{
            //do nothing
        }
    }
    
}
Simulation::Simulation(const Simulation &other)//copy constructor
:isRunning(other.isRunning),planCounter(other.planCounter),actionsLog(),plans(),settlements(),facilitiesOptions()
{
    //deep copy 
    for(auto& settlement : other.settlements){
        settlements.push_back(new Settlement(*settlement));
    }
    for(auto& action : other.actionsLog){
        actionsLog.push_back(action->clone());
    }
    for(auto& plan: other.plans){
        string name = plan.getSettlementName();
        Plan p = Plan(plan,Auxiliary::getSettlment(settlements,name));
        plans.push_back(p);
    }
    //shallow copy
    for(auto& fac: other.facilitiesOptions){
        facilitiesOptions.push_back(fac);
    }
}
Simulation::~Simulation()//destructor
{
    //delete all resources
    for(auto& action: actionsLog){
        delete action;
    }
    for(auto& settlement: settlements){
        delete settlement;
    }
}
Simulation &Simulation::operator=(const Simulation &other)//copy assginment operator
{   
    if(this!=&other){
        isRunning = other.isRunning;
        planCounter = other.planCounter;
        //delete all current  resources
        for(auto& action: actionsLog){
            delete action;
        }
        for(auto& settlement: settlements){
            delete settlement;
        }
        settlements.clear();
        actionsLog.clear();
        facilitiesOptions.clear();
        plans.clear();
        
        //deep copy 
        for(auto& settlement : other.settlements){
            settlements.push_back(new Settlement(*settlement));
        }
        for(auto& action : other.actionsLog){
            actionsLog.push_back(action->clone());
        }
        for(auto& plan: other.plans){
            string name = plan.getSettlementName();
            Plan p = Plan(plan,Auxiliary::getSettlment(settlements,name));
            plans.push_back(p);
        }
        //shallow copy
        for(auto& fac: other.facilitiesOptions){
            facilitiesOptions.push_back(fac);
        }
    }
    return *this;
}
Simulation::Simulation(Simulation &&other)  // move construcor
:isRunning(other.isRunning), planCounter(other.planCounter),
actionsLog(std::move(other.actionsLog)), plans(std::move(other.plans)), settlements(std::move(other.settlements)), facilitiesOptions(other.facilitiesOptions)
{
    other.settlements.clear();
    other.actionsLog.clear();
    other.plans.clear();
    other.facilitiesOptions.clear();
}
Simulation &Simulation::operator=(Simulation &&other) noexcept //move assignment operator
{
    if(this!=&other){
        isRunning = other.isRunning;
        planCounter = other.planCounter;

        settlements = std::move(other.settlements);
        actionsLog = std::move(other.actionsLog);
        plans = std::move(other.plans);
        facilitiesOptions = std::move(other.facilitiesOptions);

        other.settlements.clear();
        other.actionsLog.clear();
        other.plans.clear();
        other.facilitiesOptions.clear();
    }
    return *this;
}

void Simulation::start()
{
    open();
    string line;
    while(isRunning){ //loop that waits for next command, tokenizes it and acts accordingly
        std::getline(std::cin,line);
        vector<string> parsedArgs = Auxiliary::parseArguments(line);
        string cmd = parsedArgs.at(0);
        if(cmd == "step"){ 
           SimulateStep* s = new SimulateStep(std::stoi((parsedArgs.at(1))));
           s->act(*this);
           addAction(s);
        }
        else if(cmd == "plan"){
            AddPlan* a = new AddPlan(parsedArgs.at(1),parsedArgs.at(2));
            a->act(*this);
            addAction(a);
        }
        else if(cmd == "settlement"){
            AddSettlement* a = new AddSettlement(parsedArgs.at(1),static_cast<SettlementType>(std::stoi(parsedArgs.at(2))));
            a->act(*this);
            addAction(a);
        }
        else if(cmd == "facility"){
            AddFacility* a = new AddFacility(parsedArgs.at(1),static_cast<FacilityCategory>(std::stoi(parsedArgs.at(2))), std::stoi(parsedArgs.at(3)), std::stoi(parsedArgs.at(4)), std::stoi(parsedArgs.at(5)), std::stoi(parsedArgs.at(6)));
            a->act(*this);
            addAction(a);
        }
        else if(cmd == "planStatus"){
            PrintPlanStatus* p = new PrintPlanStatus(std::stoi(parsedArgs.at(1)));
            p->act(*this);
            addAction(p);
        }
        else if(cmd == "changePolicy"){
            ChangePlanPolicy* c = new ChangePlanPolicy(std::stoi(parsedArgs.at(1)),parsedArgs.at(2));
            c->act(*this);
            addAction(c);
        }
        else if(cmd == "log"){
            PrintActionsLog* p = new PrintActionsLog();
            p->act(*this);
            addAction(p);
        }
        else if(cmd == "close"){
            Close* c = new Close();
            addAction(c);//add so it will get deleted
            c->act(*this);
        }
        else if(cmd == "backup"){
            BackupSimulation* b = new BackupSimulation();
            addAction(b);
            b->act(*this);
        }
        else if(cmd == "restore"){
            RestoreSimulation* r = new RestoreSimulation();
            r->act(*this);
            addAction(r);
        }
        else{
            //do nothing
        }

    }
}
void Simulation::addPlan(const Settlement &settlement, SelectionPolicy *selectionPolicy)
{
    Plan p(planCounter,settlement,selectionPolicy,facilitiesOptions);
    plans.push_back(p);
    planCounter++;
}
void Simulation::addAction(BaseAction *action)
{
    actionsLog.push_back(action);
}
bool Simulation::addSettlement(Settlement *settlement)
{
    if(isSettlementExists(settlement->getName())){
        return false; //if already exists return false
    }
    else{
        settlements.push_back(settlement);
        return true; //return true if added
    }
}
bool Simulation::addFacility(FacilityType facility)
{
    for(FacilityType& fac: facilitiesOptions){
        if(fac.getName()==facility.getName())
            return false;
    }
    facilitiesOptions.push_back(facility);
    return true;
}
bool Simulation::isSettlementExists(const string &settlementName)
{
    for(auto& settlements: settlements){
        if(settlements->getName()==settlementName)
            return true;
    }
    return false;
}
Settlement &Simulation::getSettlement( const string &name)
{
    for(auto& settlement: settlements){//iterate over the vector of settlements and compare the names to the given one.
        if(settlement->getName()==name)
            return *settlement;
    }
    return *settlements.at(0);// for not having compilation error, we assume the func will get legal settlement name
}

Plan &Simulation::getPlan(const int planID)
{
    for(auto& plan: plans){
        if(plan.getPlanID()==planID) 
            return plan;
    }
    return plans.at(0); // for not having compilation error, we assume the func will get legal planID
}
void Simulation::step()
{
    for(auto& p:plans){
        p.step();
    }
}

void Simulation::close()
{
    isRunning = false;
    for(auto& p: plans){
        p.closePrint();
    }
}
void Simulation::open()
{
    isRunning =true;
    std::cout << "Simulation opened" << std::endl;
}

SelectionPolicy* Simulation:: createSelectionPolicy(const string &policy){
    if(policy == "nve"){
        return new NaiveSelection();
    }
    else if(policy == "bal"){
        return new BalancedSelection(0,0,0);
    }
    else if(policy == "eco"){
        return new EconomySelection();
    }
    else if(policy == "env"){
        return new SustainabilitySelection();
    }
    else
        return nullptr;
}

const bool Simulation::isPlanExists(const int planID) const
{
    for(auto& plan: plans){
        if(plan.getPlanID()==planID) 
            return true;
    }
    return false;
}

void Simulation::printLog()
{
    for(auto& action: actionsLog){
        std::cout << action->toString()<<std::endl;
    }
}
