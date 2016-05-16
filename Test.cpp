//
// Created by kyuseo park on 4/23/16.
//

#include "Test.h"

using namespace std;
#define numberOfPoints 1000
#define iterateCount 100
#define numberOfCases 4

bool isEqual(Point a, Point b)
{
    return a==b;
}
bool lessFunction (Point a, Point b)
{
    return a<b;
}
bool Point::operator<(const Point &operand)
{
    if (this->x < operand.x)
        return true;

    else if (this->x == operand.x && this->y < operand.y)
        return true;
    else
        return false;
}
bool Point::operator==(const Point &operand)
{
    if ((this->x == operand.x) && (this->y == operand.y))
        return true;
    else
        return false;
}
bool Point::operator!=(const Point &operand)
{
    if ((this->x != operand.x) || (this->y != operand.y))
        return true;
    else
        return false;
}
void MyConvexHull(const int index, const int PointsNumberInThreads, const vector<Point>& pointList, promise<CHsegments>&& p, int pointNum)
{
    string fileName = to_string(pointNum)+"takenTimeThread"+to_string(index)+".txt";
    ofstream timeOut(fileName, ios::app);

    clock_t begin,end;
    int firstIndex = index * PointsNumberInThreads;
    int lastIndex = firstIndex + PointsNumberInThreads;
    vector<Point>::const_iterator first = pointList.cbegin() + firstIndex;
    vector<Point>::const_iterator last;
    if((pointList.size() - firstIndex) > PointsNumberInThreads )
    {
        last = pointList.cbegin() + lastIndex;
    }
    else
    {
        last = pointList.cbegin() + pointList.size();
    }

    vector<Point> points(first, last);


    ConvexHull ch;
    begin = clock();
    CHsegments chs = ch.convexHull(points);
    end = clock();
    timeOut << (((float)(end-begin))/CLOCKS_PER_SEC)<<endl;
    timeOut.close();
    p.set_value(chs);

}

void TestConvexHull::start()
{

    clock_t begin,end;
    const int NumberOfThreads = thread::hardware_concurrency() ?: 2;
    int NumberOfPoints = 0;

    for(int j = 0; j< numberOfCases ; j++) {

        NumberOfPoints = numberOfPoints * pow(10,j);

        for(int i = 0 ; i < iterateCount; i++) {

            /*
             * Threads variables
             */
            vector<thread> t;
            vector<future<CHsegments> > futures;
            vector<CHsegments> chSegments;

            double tempNum = (double) (NumberOfPoints / NumberOfThreads);
            int PointsNumberInThreads = ceil(tempNum);

            vector<Point> pointList;
            Point p;
            srand((unsigned int) time(NULL));
            for (int i = 0; i < NumberOfPoints; i++) {
                int randomx = rand() % NumberOfPoints + 1;
                int randomy = rand() % NumberOfPoints + 1;

                p.x = randomx;
                p.y = randomy;
                pointList.push_back(p);
            }




            // points in the vector pointList needed to be sorted by lexicographical order
            begin = clock();
            sort(pointList.begin(), pointList.end(), lessFunction);
            vector<Point>::iterator pos;
            pos = unique(pointList.begin(), pointList.end(), isEqual);
            pointList.erase(pos, pointList.end());
            end = clock();
            cout << "sort : " << (((float)(end-begin))/CLOCKS_PER_SEC)<<endl;

            for (int i = 0; i < NumberOfThreads; i++) {
                promise<CHsegments> promises;
                futures.push_back(promises.get_future());
                t.push_back(thread(MyConvexHull, i, PointsNumberInThreads, pointList, std::move(promises),NumberOfPoints));
            }

            for_each(t.begin(), t.end(), std::mem_fn(&std::thread::join));

            for (int i = 0; i < futures.size(); i++) {
                chSegments.push_back(futures.at(i).get());
            }



            for (int i = 0; i < chSegments.size(); i++) {
                chSegments.at(i).fOut(i);
            }





            string fileName = to_string(NumberOfPoints)+"takenTimeSerial.txt";
            ofstream timeOut(fileName, ios::app);

            ConvexHull ch;
            begin = clock();
            CHsegments chs = ch.convexHull(pointList);
            end = clock();
            chs.fOut();


            timeOut << (((float) (end - begin)) / CLOCKS_PER_SEC) << endl;
            timeOut.close();
            t.clear();
            futures.clear();
            pointList.clear();
            chSegments.clear();



        }


        ifstream* timeForThreads = new ifstream[NumberOfThreads];


        for(int i = 0 ; i < NumberOfThreads; i++) {
            timeForThreads[i].open((to_string(NumberOfPoints)+"takenTimeThread"+to_string(i)+".txt"));
        }

        ifstream s(to_string(NumberOfPoints)+"takenTimeSerial.txt");

        double time[NumberOfThreads];
        double stime=0;
        double in = 0.0;

        for(int i = 0 ; i < NumberOfThreads; i++) {

            while(timeForThreads[i] >> in){
                time[i] += in;
            }
            in = 0.0;
        }

        while(s >> in)
        {
            stime += in;
        }

        cout << "*********************************************" << endl;
        cout << "Case "<< j << ": Generate " << NumberOfPoints << " and iterate "<< iterateCount << " times" << endl;
        cout << "*********************************************" << endl;

        cout << "During " << iterateCount << " iterations generate " << NumberOfPoints << " points each time and measuring the time each thread took" << endl;

        for(int i = 0 ; i < NumberOfThreads; i++)
        {
            cout << "thread " << i << " total time = " << time[i] << endl;
        }

        cout << "serial version total time = " << stime << endl << endl;


        cout << "Average time taken for each thread and serial version" << endl;

        for(int i = 0 ; i < NumberOfThreads; i++)
        {
            cout << "thread " << i << " avg time = " << time[i]/iterateCount << endl;
        }

        cout << "serial version avg time = " << stime/iterateCount << endl << endl;


        double avgThreadtime = 0.0;
        for(int i = 0 ; i < NumberOfThreads; i++) {
            avgThreadtime += (time[i]/iterateCount);
        }
        avgThreadtime = avgThreadtime / NumberOfThreads;

        cout << "Average time taken by all threads = " << avgThreadtime << endl;
        cout << "serial version avg time = " << stime/iterateCount << endl << endl;


        string fileName = "finalResultPoint"+to_string(NumberOfPoints)+".txt";
        ofstream final(fileName);

        final << avgThreadtime << endl;
        final << stime/iterateCount << endl;

        for(int i = 0 ; i < NumberOfThreads; i++) {
            timeForThreads[i].close();
        }
        delete [] timeForThreads;

        final.close();
    }

    // since the time taken by each thread is appended to the corresponding txt file, the contents should be removed after all. Otherwise, the result made by previous run would be added to the current run.
    for(int i = 0 ; i < numberOfCases; i++) {
        int pointNum = numberOfPoints * pow(10,i);
        string fileName;
        for(int j = 0 ; j < NumberOfThreads; j++) {
            fileName = to_string(pointNum)+"takenTimeThread"+to_string(j)+".txt";
            ofstream remove(fileName);
            remove.close();
        }
        fileName = to_string(pointNum)+"takenTimeSerial.txt";
        ofstream remove(fileName);
        remove.close();
    }

}