

#include <iostream>
#include "Commands.h"
#include "Polygons.h"
#include "ScopeGuard.h"

int main(int argc, char* argv[])
{

    if (argc < 2)
    {
        return -1;
    }

    std::string filename = argv[1];

    std::ifstream in(filename);

    parfenov::startInterface(in);

    return 0;
}




