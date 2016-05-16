//
// Created by kyuseo park on 3/13/16.
//

#ifndef CH_CHSEGMENTS_H
#define CH_CHSEGMENTS_H
#include <vector>
#include <stack>
#include <iostream>
#include <fstream>
struct Point {
    double x;
    double y;

    bool operator < (const Point& operand);
    bool operator ==(const Point& operand);
    bool operator !=(const Point& operand);
};

struct Segment {
    Point p1;
    Point p2;
};
#define MAXPTS 2000
using namespace std;
typedef double COORD;
typedef COORD POINT[2];
typedef struct Polygon
{
    int n, ccw, st, end, tent, wrap;
    POINT v[MAXPTS];

}Polygon;

class CHsegments
{
public:
    CHsegments();

    CHsegments(stack<Point>& upper, stack<Point>& lower);


    ~CHsegments();

    /*
     * Output
     */

    void fOut(int index);
    void fOut();

    CHsegments Merge(CHsegments& b);

    Polygon* getPoly()
    {
        return &p;
    }

private:
    vector<Segment> upperH;
    vector<Segment> lowerH;
    vector<Point> upperV, lowerV;

    Polygon p;


};
#endif //CH_CHSEGMENTS_H
