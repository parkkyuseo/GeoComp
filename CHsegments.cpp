//
// Created by kyuseo park on 3/13/16.
//

#include "CHsegments.h"
#include "ConvexHull.h"
#include <cmath>

using namespace std;

CHsegments::CHsegments()
{

}
//CHsegments::CHsegments(CHsegments& ch)
//{
//    upperH = ch.upperH;
//    lowerH = ch.lowerH;
//    upperV = ch.upperV;
//    lowerV = ch.lowerV;
//}

CHsegments::CHsegments(stack<Point>& upper, stack<Point>& lower)
{
    Point prev, next;

    // create the vector of segments.
    prev = upper.top();
    upperV.push_back(prev);

    //for tang
    int count = 0;
    p.v[count][0] = prev.x;
    p.v[count++][1] = prev.y;

    //

    upper.pop();
    int stackSize = upper.size();
    Segment s;
    for(int i = 0; i < stackSize; i++)
    {
        next = upper.top();
        upper.pop();
        s.p1 = prev;
        s.p2 = next;

        upperH.push_back(s);

        upperV.push_back(next);
        /*
         * this below is for tang
         */
        p.v[count][0] = next.x;
        p.v[count++][1] = next.y;

        /*
         *
         */

        prev = next;
    }

    prev = lower.top();
    lowerV.push_back(prev);
    lower.pop();
    stackSize = lower.size();
    for(int i = 0; i < stackSize; i++)
    {
        next = lower.top();
        lower.pop();
        s.p1 = prev;
        s.p2 = next;

        lowerH.push_back(s);

        lowerV.push_back(next);

        prev = next;
    }

    auto it = lowerV.cend(); it--; it--;
    while(it != lowerV.cbegin())
    {
        p.v[count][0] = it->x;
        p.v[count++][1] = it->y;

        it--;
    }
//    p.v[count][0] = it->x;
//    p.v[count++][1] = it->y;
//    cout << "(" << p.v[count-1][0] << " , " << p.v[count-1][1] << " )" << endl;
    p.n = count;

}
void CHsegments::fOut(int index)
{
    /*
     * for plotting the convex hull
     */
    string xPoints = "convexXPoints"+to_string(index)+".txt";
    string yPoints = "convexYPoints"+to_string(index)+".txt";
    ofstream convexX(xPoints);
    ofstream convexY(yPoints);
    /*
     *
     */

    Segment s;
    Point p;
    for(int i = 0 ; i < upperH.size(); i++ )
    {
        s = upperH.at(i);
        p = s.p1;
        convexX << p.x << endl;
        convexY << p.y << endl;
    }
    convexX << s.p2.x << endl;
    convexY << s.p2.y << endl;

    for(int i = 0; i < lowerH.size(); i++)
    {
        s = lowerH.at(i);
        p = s.p1;
        convexX << p.x << endl;
        convexY << p.y << endl;
    }
    convexX << s.p2.x << endl;
    convexY << s.p2.y << endl;


    convexX.close();
    convexY.close();
}
void CHsegments::fOut()
{
    /*
     * for plotting the convex hull
     */
    string xPoints = "convexXPoints.txt";
    string yPoints = "convexYPoints.txt";
    ofstream convexX(xPoints);
    ofstream convexY(yPoints);
    /*
     *
     */

    Segment s;
    Point p;
    for(int i = 0 ; i < upperH.size(); i++ )
    {
        s = upperH.at(i);
        p = s.p1;
        convexX << p.x << endl;
        convexY << p.y << endl;
    }
    convexX << s.p2.x << endl;
    convexY << s.p2.y << endl;

    for(int i = 0; i < lowerH.size(); i++)
    {
        s = lowerH.at(i);
        p = s.p1;
        convexX << p.x << endl;
        convexY << p.y << endl;
    }
    convexX << s.p2.x << endl;
    convexY << s.p2.y << endl;


    convexX.close();
    convexY.close();
}

CHsegments CHsegments::Merge(CHsegments& b)
{
    int aSt,aMid,aEnd, tempAEnd,tempASt;
    int bSt,bMid,bEnd, tempBEnd,tempBSt;

    aSt = 0, aEnd = upperV.size()-1, aMid = (aEnd + aSt)/2; tempAEnd = aEnd, tempASt = aSt;
    bSt = 0, bEnd = b.upperV.size()-1, bMid = (bEnd + bSt)/2; tempBEnd = bEnd, tempBSt = bSt;

    Point upperAPoint, upperBPoint, aPrev, aNext, bPrev, bNext, lowerAPoint, lowerBPoint;
    upperAPoint = upperV.at(aMid);
    aPrev = upperV.at(aMid==0?0:aMid-1);
    aNext = upperV.at(aMid==aEnd?aEnd:aMid+1);
    upperBPoint = b.upperV.at(bMid);
    bPrev = b.upperV.at(bMid==0?0:bMid-1);
    bNext = b.upperV.at(bMid==bEnd?bEnd:bMid+1);

    // compute upper tangent
    bool done = true;
    while(done)
    {
        done = false;
        while(ConvexHull::orientation(upperAPoint,upperBPoint,bPrev) > 0)
        {
            cout << "while1" << endl;
            if(tempBSt == tempBEnd)
            {
                tempBSt = 0, tempBEnd = b.upperV.size()-1;
            }
            tempBEnd = bMid;
            bMid = floor(double(bMid + tempBSt)/2);
            upperBPoint = b.upperV.at(bMid);
            bPrev = b.upperV.at(bMid==0?0:bMid-1);
            bNext = b.upperV.at(bMid==bEnd?bEnd:bMid+1);

            done = true;
        }
        while(ConvexHull::orientation(upperAPoint,upperBPoint,bNext) > 0)
        {
            cout << "while2" << endl;
            if(tempBSt == tempBEnd)
            {
                tempBSt = 0, tempBEnd = b.upperV.size()-1;
            }
            tempBSt = bMid;
            bMid = ceil(double(tempBEnd + bMid)/2);
            upperBPoint = b.upperV.at(bMid);
            bPrev = b.upperV.at(bMid==0?0:bMid-1);
            bNext = b.upperV.at(bMid==bEnd?bEnd:bMid+1);

            done = true;
        }

        while(ConvexHull::orientation(upperBPoint,upperAPoint,aPrev) < 0)
        {
            cout << "while3" << endl;
            if(tempAEnd == tempASt)
            {
                tempASt = 0, tempAEnd = upperV.size()-1;
            }
            tempAEnd = aMid;
            aMid = floor(double(aMid + tempASt)/2);
            upperAPoint = upperV.at(aMid);
            aPrev = upperV.at(aMid==0?0:aMid-1);
            aNext = upperV.at(aMid==aEnd?aEnd:aMid+1);

            done = true;
        }
        while(ConvexHull::orientation(upperBPoint,upperAPoint,aNext) < 0)
        {
            cout << "while4" << endl;
            if(tempAEnd == tempASt)
            {
                tempASt = 0, tempAEnd = upperV.size()-1;
            }
            tempASt = aMid;
            aMid = ceil(double(tempAEnd + aMid)/2);
            upperAPoint = upperV.at(aMid);
            aPrev = upperV.at(aMid==0?0:aMid-1);
            aNext = upperV.at(aMid==aEnd?aEnd:aMid+1);

            done = true;
        }

    }

    aSt = 0, aEnd = lowerV.size()-1, aMid = (aEnd + aSt)/2; tempASt = aSt, tempAEnd = aEnd;
    bSt = 0, bEnd = b.lowerV.size()-1, bMid = (bEnd + bSt)/2; tempBSt = bSt, tempBEnd = bEnd;
    lowerAPoint = lowerV.at(aMid);
    aPrev = lowerV.at(aMid==0?0:aMid-1);
    aNext = lowerV.at(aMid==aEnd?aEnd:aMid+1);
    lowerBPoint = b.lowerV.at(bMid);
    bPrev = b.lowerV.at(bMid==0?0:bMid-1);
    bNext = b.lowerV.at(bMid==bEnd?bEnd:bMid+1);

    // compute lower tanget
    done = true;
    while(done)
    {
        done = false;

//        aSt = 0, aEnd = lowerV.size()-1;
//        bSt = 0, bEnd = b.lowerV.size()-1;
        while(ConvexHull::orientation(lowerAPoint,lowerBPoint,bPrev) < 0)
        {
            cout << "while5" << endl;
            cout << "("<<lowerAPoint.x << "," << lowerAPoint.y << ") (" << lowerBPoint.x << "," << lowerBPoint.y << ")" << endl;
            if(tempBSt == tempBEnd)
            {
                tempBSt = 0, tempBEnd = b.lowerV.size()-1;
            }
            tempBEnd = bMid;
            bMid = floor(double(bMid + tempBSt)/2);
            lowerBPoint = b.lowerV.at(bMid);
            bPrev = b.lowerV.at(bMid==0?0:bMid-1);
            bNext = b.lowerV.at(bMid==bEnd?bEnd:bMid+1);
            done = true;
        }
        while(ConvexHull::orientation(lowerAPoint,lowerBPoint,bNext) < 0)
        {
            cout << "while6" << endl;

            cout << "("<<lowerAPoint.x << "," << lowerAPoint.y << ") (" << lowerBPoint.x << "," << lowerBPoint.y << ")" << endl;
            if(tempBSt == tempBEnd)
            {
                tempBSt = 0, tempBEnd = b.lowerV.size()-1;
            }
            tempBSt = bMid;
            bMid = ceil(double(tempBEnd + bMid)/2);
            lowerBPoint = b.lowerV.at(bMid);
            bPrev = b.lowerV.at(bMid==0?0:bMid-1);
            bNext = b.lowerV.at(bMid==bEnd?bEnd:bMid+1);
            done = true;
        }

        while(ConvexHull::orientation(lowerBPoint,lowerAPoint,aPrev) > 0)
        {
            cout << "while7" << endl;
            cout << "("<<lowerAPoint.x << "," << lowerAPoint.y << ") (" << lowerBPoint.x << "," << lowerBPoint.y << ")" << endl;
            if(tempAEnd == tempASt)
            {
                tempASt = 0, tempAEnd = lowerV.size()-1;
            }
            tempAEnd = aMid;
            aMid = floor(double(aMid + tempASt)/2);
            lowerAPoint = lowerV.at(aMid);
            aPrev = lowerV.at(aMid==0?0:aMid-1);
            aNext = lowerV.at(aMid==aEnd?aEnd:aMid+1);
            done = true;
        }
        while(ConvexHull::orientation(lowerBPoint,lowerAPoint,aNext) > 0)
        {
            cout << "while8" << endl;
            cout << "("<<lowerAPoint.x << "," << lowerAPoint.y << ") (" << lowerBPoint.x << "," << lowerBPoint.y << ")" << endl;
            if(tempAEnd == tempASt)
            {
                tempASt = 0, tempAEnd = lowerV.size()-1;
            }
            tempASt = aMid;
            aMid = ceil(double(tempAEnd + aMid)/2);
            lowerAPoint = lowerV.at(aMid);
            aPrev = lowerV.at(aMid==0?0:aMid-1);
            aNext = lowerV.at(aMid==aEnd?aEnd:aMid+1);
            done = true;
        }

    }
    cout << "DONE" << endl;

    // for return
    CHsegments ch;

    // for upper side
    for(int i = 0; i < b.upperH.size(); i++)
    {
        if(b.upperH.at(i).p1 != upperBPoint)
        {
            ch.upperH.push_back(b.upperH.at(i));
            ch.upperV.push_back(b.upperH.at(i).p1);
        }
        else
        {
            Segment s;
            s.p1 = upperBPoint;
            s.p2 = upperAPoint;
            ch.upperH.push_back(s);
            ch.upperV.push_back(upperBPoint);
            ch.upperV.push_back(upperAPoint);
            break;
        }
    }
    int j;
    for(j = 0 ; j < upperH.size(); j++)
    {
        if(upperH.at(j).p1 == upperAPoint)
        {
            break;
        }
    }
    for(; j < upperH.size(); j++)
    {
        ch.upperH.push_back(upperH.at(j));
        ch.upperV.push_back((upperH.at(j).p2));
    }

    // for lower side
    for(int i = 0 ; i < b.lowerH.size(); i++)
    {
        if(b.lowerH.at(i).p1 != lowerBPoint)
        {
            ch.lowerH.push_back(b.lowerH.at(i));
            ch.lowerV.push_back(b.lowerH.at(i).p1);
        }
        else
        {
            Segment s;
            s.p1 = lowerBPoint;
            s.p2 = lowerAPoint;
            ch.lowerH.push_back(s);
            ch.lowerV.push_back(lowerBPoint);
            ch.lowerV.push_back(lowerAPoint);
            break;
        }
    }
    for(j = 0 ; j < lowerH.size(); j++)
    {
        if(lowerH.at(j).p1 == lowerAPoint)
        {
            break;
        }
    }
    for(; j < lowerH.size(); j++)
    {
        ch.lowerH.push_back(lowerH.at(j));
        ch.lowerV.push_back((lowerH.at(j).p2));
    }

    return ch;

}



CHsegments::~CHsegments()
{
    upperH.clear();
    lowerH.clear();

}
