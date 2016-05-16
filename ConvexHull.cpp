//
// Created by kyuseo park on 3/14/16.
//

#include "ConvexHull.h"


    ConvexHull::ConvexHull() { }

    // to see if it turns clockwise or counterclockwise
    int ConvexHull::orientation(const Point &p, const Point &q, const Point &r) {
        int val = (q.x - p.x) * (r.y - p.y) - (q.y - p.y) * (r.x - p.x);

        if (val == 0)
            return 0; // colinear
        return (val < 0) ? -1 : 1; // right or left
    }

    // to find next to top in a stack
    Point ConvexHull::secondTop(stack<Point> &S) {
        Point res;
        if (S.size() < 2) {
            res = S.top();
        }
        else {
            Point p = S.top();
            S.pop();
            res = S.top();
            S.push(p);
        }

        return res;
    }

    CHsegments ConvexHull::convexHull(const vector<Point> &points)
    {
        CHsegments result;

        if(points.size()>1)
        {


            Point leftMostPoint, rightMostPoint, nextPoint;

            stack<Point> upperH, lowerH;

            vector<Point>::const_iterator itr;

            itr = points.cbegin();


            leftMostPoint.x = (*itr).x;
            leftMostPoint.y = (*itr).y;

            itr = points.cend();
            itr--;
            rightMostPoint.x = (*itr).x;
            rightMostPoint.y = (*itr).y;

            itr = points.cbegin();
            itr++;

            upperH.push(leftMostPoint);
            lowerH.push(leftMostPoint);


            while ((itr) < points.cend())
            {
                nextPoint.x = (*itr).x;
                nextPoint.y = (*itr).y;
                itr++;

                if (orientation(leftMostPoint, rightMostPoint, nextPoint) >= 0) // if the next point is above the imaginary line
                {
                    while (orientation(secondTop(upperH), upperH.top(), nextPoint) > 0) // if it is left turn
                    {
                        upperH.pop();
                    }
                    upperH.push(nextPoint);
                }

                else // if (orientation(leftMostPoint, rightMostPoint, nextPoint) < 0)
                {
                    while (orientation(secondTop(lowerH), lowerH.top(), nextPoint) < 0) // if it is right turn
                    {
                        lowerH.pop();
                    }
                    lowerH.push(nextPoint);
                }
            }


            // with the very last point,
            // it should be doing this while loop again and pop if it is the right turn with the last point. And then push the last point.
            while (orientation(secondTop(lowerH), lowerH.top(), nextPoint) < 0)
            {
                lowerH.pop();
            }
            lowerH.push(nextPoint);

            CHsegments result1(upperH, lowerH);
            result = result1;
        }
        else{
            stack<Point> upperH, lowerH;
            upperH.push(points.at(0));
            lowerH.push(points.at(0));
            CHsegments result1(upperH, lowerH);
            result = result1;
        }


        return result;
    }
