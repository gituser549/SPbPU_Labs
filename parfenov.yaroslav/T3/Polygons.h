
#ifndef POLYGONS_H
#define POLYGONS_H

#include <iostream>
#include <vector>
#include <string>

namespace parfenov
{

struct Point
{
    int x, y;
    bool operator==(const Point& pt) const;
};

struct Polygon
{
    std::vector<Point> points;
    bool operator==(const Polygon& pol) const;
};

int getFirstCoord(std::string coords);
int getSecondCoord(std::string coords);
std::istream& operator>>(std::istream& in, Polygon& fig);
double getAreaOfPolygon(const Polygon& pol);
double getAreaOfPolygons(const std::vector<Polygon>& polygons, const std::string& signal);
double getAreaMean(const std::vector<Polygon>& pols);
double getAreaFixedPointNum(const std::vector<Polygon>& polygons, const unsigned long int& pointNum);
double getMaxOrMinAreaOrPoints(const std::vector<Polygon>& polygons, const std::string areaOrVerts, std::string minOrMax);
long getCountEvenOrOddPts(const std::vector<Polygon>& polygons, const std::string& signal);
long getCountExactNumPts(const std::vector<Polygon>& polygons, const unsigned long int& numVerts);
long eraseDuplicatesInARow(std::vector<Polygon>& polygons, const Polygon& toErase);
bool isPtInsideSeg(const Point& pt1, const Point& pt2, const Point& pt3);
bool areIntersectPoints(const Point& pt1, const Point& pt2, const Point& pt3, const Point& pt4);
bool haveIntersectionsPolTwoPoints(const Point& pt1, const Point& pt2, const Polygon& longerPol);
bool haveIntersectionsPolygons(const Polygon& pol1, const Polygon& pol2);
long areAnyIntersections(const std::vector<Polygon>& pols, const Polygon& pol);

}


#endif
