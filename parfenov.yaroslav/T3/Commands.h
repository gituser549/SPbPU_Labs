
#ifndef COMMANDS_H
#define COMMANDS_H

#include <iostream>
#include <fstream>
#include "Polygons.h"
#include <vector>
#include <string>

namespace parfenov
{

struct Command
{
    std::string data;
};

std::istream& operator>>(std::istream& in, Command& cmd);
bool doCommand(std::vector<Polygon>& polygons, const std::string& curCommand);
void startInterface(std::ifstream& in);

}

#endif
