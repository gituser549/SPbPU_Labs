
#include "Commands.h"
#include "ScopeGuard.h"
#include <sstream>
#include <regex>
#include <iomanip>

namespace parfenov
{

const std::regex ANY_NUMBER = std::regex("([0]|[1-9][0-9]*)");
const std::regex COMMANDS_AREA = std::regex("AREA (EVEN|ODD|MEAN|([1-9][0-9]*))");
const std::regex COMMANDS_MAX = std::regex("MAX (AREA|VERTEXES)");
const std::regex COMMANDS_MIN = std::regex("MIN (AREA|VERTEXES)");
const std::regex COMMANDS_COUNT = std::regex("COUNT (EVEN|ODD|([1-9][0-9]*))");
const std::regex COMMANDS_RMECHO = std::regex("RMECHO ([0]|[1-9][0-9]*) \\(([0]|[1-9][0-9]*);([0]|[1-9][0-9]*)\\)");
const std::regex COMMANDS_INTERSECTIONS = std::regex("INTERSECTIONS ([0]|[1-9][0-9]*) \\(([0]|[1-9][0-9]*);([0]|[1-9][0-9]*)\\)");

std::istream& operator>>(std::istream& in, Command& cmd)
{
    std::istream::sentry sentry(in);
    if (!sentry)
    {
        return in;
    }
    std::getline(in, cmd.data);
    return in;
}

bool doCommand(std::vector<Polygon>& polygons, const std::string& curCommand)
{
    std::ostream::sentry sentry(std::cout);
    if (!sentry)
    {
        return false;
    }
    ScopeGuard guard(std::cout);
    if (std::regex_match(curCommand, COMMANDS_AREA))
    {
        std::cout << std::fixed << std::setprecision(1);
        if (curCommand == "AREA EVEN")
        {
            std::cout << getAreaOfPolygons(polygons, "EVEN") << "\n";
        }
        else if (curCommand == "AREA ODD")
        {
            std::cout << getAreaOfPolygons(polygons, "ODD") << "\n";
        }
        else if (curCommand == "AREA MEAN")
        {
            if (polygons.empty())
            {
                std::cout << "<INVALID COMMAND>" << "\n";
                return false;
            }
            std::cout << getAreaMean(polygons) << "\n";
        }
        else
        {
            std::istringstream commandIn(curCommand.substr(5, curCommand.length() - 5));
            unsigned long int numPoints;
            commandIn >> numPoints;
            if (numPoints < 3)
            {
                std::cout << "<INVALID COMMAND>" << "\n";
                return false;
            }
            std::cout << getAreaFixedPointNum(polygons, numPoints) << "\n";
        }
    }
    else if (std::regex_match(curCommand, COMMANDS_MAX))
    {
        if (polygons.empty())
        {
            std::cout << "<INVALID COMMAND>" << "\n";
            return false;
        }
        if (curCommand == "MAX AREA")
        {
            std::cout << std::fixed << std::setprecision(1) << getMaxOrMinAreaOrPoints(polygons, "AREA", "MAX") << "\n";
        }
        else if (curCommand == "MAX VERTEXES")
        {
            std::cout << getMaxOrMinAreaOrPoints(polygons, "VERTEXES", "MAX") << "\n";
        }
    }
    else if (std::regex_match(curCommand, COMMANDS_MIN))
    {
        if (polygons.empty())
        {
            std::cout << "<INVALID COMMAND>" << "\n";
            return false;
        }
        if (curCommand == "MIN AREA")
        {
            std::cout << std::fixed << std::setprecision(1) << getMaxOrMinAreaOrPoints(polygons, "AREA", "MIN") << "\n";
        }
        else if (curCommand == "MIN VERTEXES")
        {
            std::cout << getMaxOrMinAreaOrPoints(polygons, "VERTEXES", "MIN") << "\n";
        }
    }
    else if (std::regex_match(curCommand, COMMANDS_COUNT))
    {
        if (curCommand == "COUNT EVEN")
        {
            std::cout << getCountEvenOrOddPts(polygons, "EVEN") << "\n";
        }
        else if (curCommand == "COUNT ODD")
        {
            std::cout << getCountEvenOrOddPts(polygons, "ODD") << "\n";
        }
        else
        {
            std::istringstream commandIn(curCommand.substr(6, curCommand.length() - 6));
            unsigned long int numPoints;
            commandIn >> numPoints;
            if (numPoints < 3)
            {
                std::cout << "<INVALID COMMAND>" << "\n";
                return false;
            }
            std::cout << getCountExactNumPts(polygons, numPoints) << "\n";
        }
    }
    else if (std::regex_match(curCommand, COMMANDS_RMECHO))
    {
        std::istringstream commandIn(curCommand.substr(7, curCommand.length() - 7));
        Polygon pol;
        commandIn >> pol;
        std::cout << eraseDuplicatesInARow(polygons, pol) << "\n";
    }
    else if (std::regex_match(curCommand, COMMANDS_INTERSECTIONS))
    {
        std::istringstream commandIn(curCommand.substr(14, curCommand.length() - 14));
        Polygon pol;
        commandIn >> pol;
        std::cout << areAnyIntersections(polygons, pol) << "\n";
    }
    else
    {
        std::cout << "<INVALID COMMAND>" << "\n";
        return false;
    }
    return true;
}

void startInterface(std::ifstream& in)
{
    std::vector<Polygon> polygons;
    std::copy_if(
                 std::istream_iterator<Polygon>(in),
                 std::istream_iterator<Polygon>(),
                 std::back_inserter(polygons),
                 [](Polygon polygon) { return polygon.points.size() > 0; }
                 );

    std::vector<Command> commands;
    std::copy_if(
                 std::istream_iterator<Command>(std::cin),
                 std::istream_iterator<Command>(),
                 std::back_inserter(commands),
                 [&polygons](Command command) { return doCommand(polygons, command.data); }
                 );
}

}
