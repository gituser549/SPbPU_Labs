

#include <iostream>
#include "Commands.h"
#include "Polygons.h"
#include "ScopeGuard.h"
#include <string>

int main(int argc, char* argv[])
{

    if (argc < 2)
    {
        std::cout << "You should use ./lab <filename>" << "\n";
        return EXIT_FAILURE;
    }

    std::string filename = argv[1];

    std::ifstream in(filename);

    if (!in)
    {
        std::cout << "Incorrect file" << "\n";
        return EXIT_FAILURE;
    }

    parfenov::startInterface(in);

    return EXIT_SUCCESS;
}




