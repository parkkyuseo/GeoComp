//
// Created by kyuseo park on 3/10/16.
//

#ifndef CH_CONVEXHULL_H
#define CH_CONVEXHULL_H

#include "CHsegments.h"


using namespace std;

class ConvexHull {

public:

    ConvexHull();// { }

    // to see if it turns clockwise or counterclockwise
    int static orientation(const Point& p, const Point& q, const Point& r);

    // to find next to top in a stack
    Point secondTop(stack<Point> &S);


    CHsegments convexHull(const vector<Point>& points);

};
#endif //CH_CONVEXHULL_H
