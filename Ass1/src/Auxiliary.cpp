#include "Auxiliary.h"
/*
This is a 'static' method that receives a string(line) and returns a vector of the string's arguments.

For example:
parseArguments("settlement KfarSPL 0") will return vector with ["settlement", "KfarSPL", "0"]

To execute this method, use Auxiliary::parseArguments(line)
*/
std::vector<std::string> Auxiliary::parseArguments(const std::string& line) {
    std::vector<std::string> arguments;
    std::istringstream stream(line);
    std::string argument;

    while (stream >> argument) {
        arguments.push_back(argument);
    }

    return arguments;
}

Settlement &Auxiliary::getSettlment(std::vector<Settlement *> settlements, std::string settlementName)
{
    for(auto& settlement: settlements){//iterate over the vector of settlements and compare the names to the given one.
        if(settlement->getName()==settlementName)
            return *settlement;
    }
    return *settlements.at(0);// for not having compilation error, we assume the func will get legal settlement name
}
