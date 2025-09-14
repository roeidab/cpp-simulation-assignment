#include "Settlement.h"

Settlement::Settlement(const string &name, SettlementType type):name(name),type(type),constructionLimit(static_cast<int>(type)+1)
{}

const string &Settlement::getName() const
{
    return name;
}

SettlementType Settlement::getType() const
{
    return type;
}

const string Settlement::toString() const
{
    string typeS;
    switch (type)
    {
    case SettlementType::VILLAGE:
        typeS = "Village";
        break;
    case SettlementType::CITY:
        typeS = "City";
        break;
    case SettlementType::METROPOLIS:
        typeS = "Metropolis";
        break;
    default:
        break;
    }
    return "name: "+name+", type:"+ typeS;
}

const int Settlement::getConstructionLimit() const
{
    return constructionLimit;
}
