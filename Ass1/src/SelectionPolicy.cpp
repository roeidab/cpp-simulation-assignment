#include "SelectionPolicy.h"
#include <limits.h>
//Class Naive Selection
NaiveSelection::NaiveSelection():lastSelectedIndex(-1)
{
    
}

NaiveSelection::NaiveSelection(int i):lastSelectedIndex(i)
{
}

const FacilityType &NaiveSelection::selectFacility(const vector<FacilityType> &facilitiesOptions)
{
    //iterates over to next index
    int newindex = (lastSelectedIndex+1)%facilitiesOptions.size();
    lastSelectedIndex = newindex;
    return facilitiesOptions.at(newindex);
    //returns a reference to facility at the new index
}

const string NaiveSelection::toString() const
{
    return "Naive selection, Last index ="+lastSelectedIndex;
}

const string NaiveSelection::stringShortPolicy() const
{
    return "nve";
}

NaiveSelection *NaiveSelection::clone() const
{
    NaiveSelection* e = new NaiveSelection(this->lastSelectedIndex);
    return e;
}

//Class Balanced Selection
BalancedSelection::BalancedSelection(int LifeQualityScore, int EconomyScore, int EnvironmentScore):LifeQualityScore(LifeQualityScore),EconomyScore(EconomyScore),EnvironmentScore(EnvironmentScore)
{

}

const FacilityType &BalancedSelection::selectFacility(const vector<FacilityType> &facilitiesOptions)
{   
    int index = 0;
    int lowestDistance = INT_MAX;//every value will be lower than it.
    int lowestindex = 0 ;
    for(FacilityType f: facilitiesOptions){
        //calculate new scores
        int Lifescore = LifeQualityScore+f.getLifeQualityScore();
        int Ecoscore = EconomyScore+f.getEconomyScore();
        int Enviscore = EnvironmentScore+f.getEnvironmentScore();
        //calculate distance
        int maxscore = std::max(std::max(Lifescore,Ecoscore),Enviscore);
        int minscore = std::min(std::min(Lifescore,Ecoscore),Enviscore);
        if(abs(maxscore-minscore)<lowestDistance){
            lowestDistance = abs(maxscore-minscore);
            lowestindex = index; //if lower than the lowest update values
        }
        index++;
    }
    //update the scores
    this->EconomyScore += facilitiesOptions.at(lowestindex).getEconomyScore();
    this->EnvironmentScore += facilitiesOptions.at(lowestindex).getEnvironmentScore();
    this->LifeQualityScore += facilitiesOptions.at(lowestindex).getLifeQualityScore();
    //return the first appearance of the lowest distance
    return facilitiesOptions.at(lowestindex);
}

const string BalancedSelection::stringShortPolicy() const
{
    return "bal";
}

const string BalancedSelection::toString() const
{
    return "Balanced selection, Life quality score: "+std::to_string(LifeQualityScore)+
    ", Economy score: "+std::to_string(EconomyScore)+
    ", Environment Score: "+std::to_string(EnvironmentScore);
}

BalancedSelection *BalancedSelection::clone() const
{
    BalancedSelection* e = new BalancedSelection(LifeQualityScore,EconomyScore,EnvironmentScore);
    return e;
}

//Class Economy Selection
EconomySelection::EconomySelection():lastSelectedIndex(-1)
{
    
}

EconomySelection::EconomySelection(int i):lastSelectedIndex(i)
{
}

const FacilityType &EconomySelection::selectFacility(const vector<FacilityType> &facilitiesOptions)
{
    bool found = false;
    int index= lastSelectedIndex;
    while(!found){
        index = (index+1)%(facilitiesOptions.size());
        FacilityType curr = facilitiesOptions[index];
        found = (curr.getCategory()==FacilityCategory::ECONOMY);
    }
    lastSelectedIndex=index;
    return facilitiesOptions[index];
}

const string EconomySelection::stringShortPolicy() const
{
    return "eco";
}

const string EconomySelection::toString() const
{
    return "EconomySelection";
}

EconomySelection *EconomySelection::clone() const
{
    return new EconomySelection(*this);
}

//Class Sustainability Selection
SustainabilitySelection::SustainabilitySelection():lastSelectedIndex(-1)
{
}

SustainabilitySelection::SustainabilitySelection(int i):lastSelectedIndex(i)
{

}

const FacilityType &SustainabilitySelection::selectFacility(const vector<FacilityType> &facilitiesOptions)
{
    int newindex = (lastSelectedIndex+1)%facilitiesOptions.size();
    bool found = false;
    while(!found){//iterate over the facilities of the enviroment category
        if(facilitiesOptions.at(newindex).getCategory()==FacilityCategory::ENVIRONMENT){
            found = true;
            lastSelectedIndex = newindex;
        }
        else{
            //iterate one more step and reset to the start of the list if nessecary
            newindex = (newindex+1)%facilitiesOptions.size();
        }
        
    }
    return facilitiesOptions.at(newindex);
}

const string SustainabilitySelection::stringShortPolicy() const
{
    return "env";
}


const string SustainabilitySelection::toString() const
{
    return "Sustainability selection, Last index ="+lastSelectedIndex;
}

SustainabilitySelection *SustainabilitySelection::clone() const
{
    SustainabilitySelection* e = new SustainabilitySelection(lastSelectedIndex);
    return e;
}


