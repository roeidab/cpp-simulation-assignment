#pragma once
#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <fstream>
#include "Settlement.h"
class Auxiliary{
    public:
        static std::vector<std::string> parseArguments(const std::string& line);
        static Settlement& getSettlment(std::vector<Settlement*> f,std::string settlementName);
};
