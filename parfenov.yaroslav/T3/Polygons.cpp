
#include "Polygons.h"
#include <sstream>
#include <regex>
#include <algorithm>
#include <numeric>
#include <functional>

namespace parfenov
{

const std::regex COORD_PAIR = std::regex("\\(([0]|[1-9][0-9]*);([0]|[1-9][0-9]*)\\)");

using namespace std::placeholders;

bool Point::operator==(const Point& pt) const
{
    return x == pt.x && y == pt.y;
}

bool Polygon::operator==(const Polygon& pol) const
{
    bool result = true;
    if (points.size() != pol.points.size())
    {
        result = false;
    }
    else
    {
        result = std::equal(
                            points.cbegin(),
                            points.cend(),
                            pol.points.cbegin(),
                            std::bind(std::equal_to<Point>(), _1, _2)
                            );
    }
    return result;
}


int getFirstCoord(std::string coords)
{
    auto itEnd = std::find_if(
                              coords.cbegin() + 1,
                              coords.cend(),
                              std::bind(std::equal_to<char>(), _1, ';')
                              );
    std::string subStr;
    std::copy(
              coords.cbegin(),
              itEnd,
              std::back_inserter(subStr)
              );

    return stoi(subStr.substr(1, subStr.length() - 1));
}

int getSecondCoord(std::string coords)
{
    auto itBegin = std::find_if(
                                coords.cbegin() + 1,
                                coords.cend(),
                                std::bind(std::equal_to<char>(), _1, ';')
                                );
    std::string subStr;
    std::copy(
              itBegin,
              coords.cend(),
              std::back_inserter(subStr)
              );

    return stoi(subStr.substr(1, subStr.length() - 1));
}

std::istream& operator>>(std::istream& in, Polygon& fig)
{
    std::istream::sentry sentry(in);
    if (!sentry)
    {
        return in;
    }
    long unsigned int numPoints = 0;
    in >> numPoints;
    in.ignore();

    std::string gotten;
    std::getline(in, gotten);

    std::istringstream line(gotten);

    std::vector<std::string> strs;

    std::copy(
              std::istream_iterator<std::string>(line),
              std::istream_iterator<std::string>(),
              std::back_inserter(strs)
              );

    auto isError = [](std::string str) { return !std::regex_match(str, COORD_PAIR); };

    long errors = 0;
    errors = std::count_if(
                           strs.cbegin(),
                           strs.cend(),
                           isError
                           );

    if (strs.size() != numPoints)
    {
        errors++;
    }

    fig.points.clear();
    if (!errors)
    {
        std::transform(
                       strs.cbegin(),
                       strs.cend(),
                       std::back_inserter(fig.points),
                       [](const std::string& str) { return Point{ getFirstCoord(str), getSecondCoord(str) }; }
                       );
    }

    if (fig.points.size() < 3)
    {
        fig.points.clear();
    }

    return in;
}

double getAreaOfPolygon(const Polygon& pol)
{

    double ans = 0;
    ans = 0.5 * std::abs(std::inner_product(
                                            pol.points.cbegin(),
                                            pol.points.cend() - 1,
                                            pol.points.cbegin() + 1,
                                            0.0,
                                            std::plus<>(),
                                            [](const Point& fir, const Point& sec) { return (fir.x * sec.y) - (sec.x * fir.y); }
                                            )
                         + (pol.points.back().x * pol.points.front().y) - (pol.points.front().x * pol.points.back().y)
                         );
    return ans;
}


double getAreaOfPolygons(const std::vector<Polygon>& polygons, const std::string& signal)
{
    double ans = 0;
    if (signal == "EVEN" || signal == "ODD")
    {
        std::vector<Polygon> neededPols;
        if (signal == "EVEN")
        {
            std::copy_if(
                         polygons.cbegin(),
                         polygons.cend(),
                         std::back_inserter(neededPols),
                         [](const Polygon& polygon) { return polygon.points.size() % 2 == 0; }
                         );
        }
        else if (signal == "ODD")
        {
            std::copy_if(
                         polygons.cbegin(),
                         polygons.cend(),
                         std::back_inserter(neededPols),
                         [](const Polygon& polygon) { return polygon.points.size() % 2 != 0; }
                         );
        }

        ans = std::accumulate(
                              neededPols.cbegin(),
                              neededPols.cend(),
                              0.0,
                              [](double res, const Polygon& pol) { return res + getAreaOfPolygon(pol); }
                              );

    }
    else
    {
        ans = -1;
    }
    return ans;
}

double getAreaMean(const std::vector<Polygon>& pols)
{
    if (pols.empty())
    {
        return 0;
    }
    double ans = (getAreaOfPolygons(pols, "EVEN") + getAreaOfPolygons(pols, "ODD")) / pols.size();
    return ans;
}

double getAreaFixedPointNum(const std::vector<Polygon>& polygons, const unsigned long int& pointNum)
{
    double ans = 0;
    std::vector<Polygon> neededPols;
    std::copy_if(
                 polygons.cbegin(),
                 polygons.cend(),
                 std::back_inserter(neededPols),
                 [&pointNum](const Polygon& polygon) { return polygon.points.size() == pointNum; }
                 );
    ans = std::accumulate(
                          neededPols.cbegin(),
                          neededPols.cend(),
                          0.0,
                          [](double res, const Polygon& pol) { return res + getAreaOfPolygon(pol); }
                          );
    return ans;
}

double getMaxOrMinAreaOrPoints(const std::vector<Polygon>& polygons, const std::string areaOrVerts, std::string minOrMax)
{
    double ans = 0;
    if (areaOrVerts == "AREA")
    {
        std::vector<double> areas;
        std::transform(
                       polygons.cbegin(),
                       polygons.cend(),
                       std::back_inserter(areas),
                       std::bind(&getAreaOfPolygon, _1)
                       );
        if (minOrMax == "MAX")
        {
            ans = *std::max_element(areas.cbegin(), areas.cend());
        }
        else if (minOrMax == "MIN")
        {
            ans = *std::min_element(areas.cbegin(), areas.cend());
        }
    }
    else if (areaOrVerts == "VERTEXES")
    {
        std::vector<double> vertexes;
        std::transform(
                       polygons.cbegin(),
                       polygons.cend(),
                       std::back_inserter(vertexes),
                       [](const Polygon& pol) { return pol.points.size(); }
                       );
        if (minOrMax == "MAX")
        {
            ans = *std::max_element(vertexes.cbegin(), vertexes.cend());
        }
        else if (minOrMax == "MIN")
        {
            ans = *std::min_element(vertexes.cbegin(), vertexes.cend());
        }
    }
    return ans;
}

long getCountEvenOrOddPts(const std::vector<Polygon>& polygons, const std::string& signal)
{
    long ans = 0;
    if (signal == "EVEN")
    {
        ans = std::count_if(
                            polygons.cbegin(),
                            polygons.cend(),
                            [](const Polygon& pol) { return pol.points.size() % 2 == 0; }
                            );
    }
    else if (signal == "ODD")
    {
        ans = std::count_if(
                            polygons.cbegin(),
                            polygons.cend(),
                            [](const Polygon& pol) { return pol.points.size() % 2 != 0; }
                            );
    }
    else
    {
        ans = -1;
    }

    return ans;
}

long getCountExactNumPts(const std::vector<Polygon>& polygons, const unsigned long int& numVerts)
{
    long ans = 0;
    ans = std::count_if(
                        polygons.cbegin(),
                        polygons.cend(),
                        [&numVerts](const Polygon& pol) { return pol.points.size() == numVerts; }
                        );
    return ans;
}

long eraseDuplicatesInARow(std::vector<Polygon>& polygons, const Polygon& toErase)
{
    auto newEndIter = std::unique(
                                  polygons.begin(),
                                  polygons.end(),
                                  [&toErase](Polygon& fir, Polygon& sec) { return fir == sec && fir == toErase; }
                                  );
    long numRemoved = polygons.size() - std::distance(polygons.begin(), newEndIter);
    polygons.erase(newEndIter, polygons.end());
    return numRemoved;
}

bool isPtInsideSeg(const Point& pt1, const Point& pt2, const Point& pt3)
{
    return (pt3.x >= std::min(pt1.x, pt2.x) && pt3.x <= std::max(pt1.x, pt2.x)) &&
    (pt3.y >= std::min(pt1.y, pt2.y) && pt3.y <= std::max(pt1.y, pt2.y));
}

bool areIntersectPoints(const Point& pt1, const Point& pt2, const Point& pt3, const Point& pt4)
{
    int k1 = (pt1.x - pt3.x) * (pt4.y - pt3.y) - (pt1.y - pt3.y) * (pt4.x - pt3.x),
    k2 = (pt2.x - pt3.x) * (pt4.y - pt3.y) - (pt2.y - pt3.y) * (pt4.x - pt3.x);
    int k3 = (pt3.x - pt1.x) * (pt2.y - pt1.y) - (pt3.y - pt1.y) * (pt2.x - pt1.x),
    k4 = (pt4.x - pt1.x) * (pt2.y - pt1.y) - (pt4.y - pt1.y) * (pt2.x - pt1.x);

    bool firInSeg1 = isPtInsideSeg(pt3, pt4, pt1),
    secInSeg1 = isPtInsideSeg(pt3, pt4, pt2),
    thiInSeg2 = isPtInsideSeg(pt1, pt2, pt3),
    fouInSeg2 = isPtInsideSeg(pt1, pt2, pt4);

    return (k1 * k2 < 0 && k3 * k4 < 0) || (k1 == 0 && firInSeg1)
    || (k2 == 0 && secInSeg1) || (k3 == 0 && thiInSeg2) || (k4 == 0 && fouInSeg2);

}

bool haveIntersectionsPolTwoPoints(const Point& pt1, const Point& pt2, const Polygon& longerPol)
{
    std::vector<bool> innerResIns;
    std::transform(
                   longerPol.points.cbegin(),
                   longerPol.points.cend() - 1,
                   longerPol.points.cbegin() + 1,
                   std::back_inserter(innerResIns),
                   std::bind(&areIntersectPoints, pt1, pt2, _1, _2)
                   );
    return std::any_of(
                       innerResIns.cbegin(),
                       innerResIns.cend(),
                       std::bind(std::equal_to<bool>(), _1, true)
                       );
}

bool haveIntersectionsPolygons(const Polygon& pol1, const Polygon& pol2)
{
    Polygon shorterPol = std::min(
                                  pol1,
                                  pol2,
                                  [](const Polygon& pol1, const Polygon& pol2)
                                  { return pol1.points.size() < pol2.points.size(); }
                                  );

    Polygon longerPol = (pol1 == shorterPol) ? pol2 : pol1;

    shorterPol.points.push_back(*(shorterPol.points.cbegin()));
    longerPol.points.push_back(*(longerPol.points.cbegin()));

    std::vector<bool> resIns;
    std::transform(
                   shorterPol.points.cbegin(),
                   shorterPol.points.cend() - 1,
                   shorterPol.points.cbegin() + 1,
                   std::back_inserter(resIns),
                   std::bind(&haveIntersectionsPolTwoPoints, _1, _2, longerPol)
                   );

    return std::any_of(
                       resIns.cbegin(),
                       resIns.cend(),
                       std::bind(std::equal_to<bool>(), _1, true)
                       );
}

long areAnyIntersections(const std::vector<Polygon>& pols, const Polygon& pol)
{
    std::vector<bool> res;
    std::transform(
                   pols.cbegin(),
                   pols.cend(),
                   std::back_inserter(res),
                   std::bind(&haveIntersectionsPolygons, pol, _1)
                   );

    return std::count_if(
                         res.cbegin(),
                         res.cend(),
                         std::bind(std::equal_to<bool>(), _1, true)
                         );
}

}
