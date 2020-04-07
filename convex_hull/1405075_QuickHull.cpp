#include<iostream>
#include<bits/stdc++.h>

#define MIN 0
#define MAX 999999
#define PI 3.14159265

using namespace std;

struct point{
    float x, y;
    float angle;
};

struct angleComparator{
    inline bool operator() (const point& p1, const point& p2){
        return (p1.angle < p2.angle);
    }
};

struct xyComparator{
    inline bool operator() (const point& p1, const point& p2){
        return (p1.x < p2.x && p1.y < p2.y);
    }
};

class ConvexHull{

private:
    int totalPoints;
    vector<point> points;
    vector<point> convexHull;
    float area;

public:
    ConvexHull(int n){
        totalPoints = n;
        area = 0.0;
    }


    ~ConvexHull(){
    }


    float getAngle(float x, float y){
        return atan2(y, x) * 180 / PI;
    }


    void addPoint(float x, float y){
        point p = {x, y, getAngle(x, y)};
        points.push_back(p);
    }


    float getCrossProduct(point p1, point p2, point p){
        return (p2.x - p1.x) * (p.y - p1.y) - (p.x - p1.x) * (p2.y - p1.y);
    }


    float findArea(point p1, point p2, point p3){
        float A, B, C, S;

        A = sqrt(((p2.x-p1.x) * (p2.x-p1.x)) + ((p2.y-p1.y) * (p2.y-p1.y)));
        B = sqrt(((p3.x-p2.x) * (p3.x-p2.x)) + ((p3.y-p2.y) * (p3.y-p2.y)));
        C = sqrt(((p1.x-p3.x) * (p1.x-p3.x)) + ((p1.y-p3.y) * (p1.y-p3.y)));
        S = (A + B + C) / 2;

        return sqrt(S * (S - A) * (S - B) * (S - C));
    }


    void sortPoints(){
        sort(convexHull.begin(), convexHull.end(), angleComparator());
    }


    void quickHull(){

        point minPoint = points[0];
        point maxPoint = points[0];

        for(int i = 1; i < totalPoints; i++){

            if(points[i].x < minPoint.x) minPoint = points[i];
            if(points[i].x > maxPoint.x) maxPoint = points[i];
        }

        convexHull.push_back(minPoint);
        convexHull.push_back(maxPoint);

        //cout<<"MIN "<<minPoint.x<<" "<<minPoint.y<<endl;
        //cout<<"MAX "<<maxPoint.x<<" "<<maxPoint.y<<endl;

        vector<point> s1, s2;

        for(int i = 0; i < totalPoints; i++){

            if (getCrossProduct(minPoint, maxPoint, points[i]) > 0.0) s1.push_back(points[i]);
            else if (getCrossProduct(minPoint, maxPoint, points[i]) < 0.0) s2.push_back(points[i]);
        }

        findHull(s1, minPoint, maxPoint);
        findHull(s2, maxPoint, minPoint);
    }


    void findHull(vector<point> s, point p, point q){

        if (s.size() == 0) return;

        float maxDistance = 0.0;
        int farPoint;

        for(int i = 0; i < s.size(); i++){

            float distance = abs(getCrossProduct(p, q, s[i]));

            if (distance > maxDistance){
                maxDistance = distance;
                farPoint = i;
            }
        }
        convexHull.push_back(s[farPoint]);

        area = area + findArea(p, q, s[farPoint]);

        //cout<<"FAR "<<s[farPoint].x<<" "<<s[farPoint].y<<endl;

        vector<point> s1, s2;

        for(int i = 0; i < s.size(); i++){

            if (getCrossProduct(p, s[farPoint], s[i]) > 0.0) s1.push_back(s[i]);
            if (getCrossProduct(s[farPoint], q, s[i]) > 0.0) s2.push_back(s[i]);

        }

        findHull(s1, p, s[farPoint]);
        findHull(s2, s[farPoint], q);
    }


    void writeResults(){

        ofstream file;
        file.open("1405075_output.txt");

        cout << "Total corner points on the Convex Hull = " << convexHull.size() << endl << endl;

        sortPoints();

        for (int i = 0; i < convexHull.size(); i++){

            file << convexHull[i].x << " " << convexHull[i].y << endl;
            cout << i + 1 << ". (" << convexHull[i].x << ", " << convexHull[i].y << ")" << endl;
            //cout << "Angle: " << convexHull[i].angle << endl;
        }
        file << convexHull[0].x << " " << convexHull[0].y << endl;

        cout << endl << "Area = " << area << " sq units" << endl;

        file.close();
    }

};


int main(){

    int totalPoints;

    ifstream file;
    file.open("1405075_input1.txt");

    file >> totalPoints;
    ConvexHull convexHull = ConvexHull(totalPoints);

    for (int i = 0; i < totalPoints; i++){
        float x, y;
        file >> x >> y;

        convexHull.addPoint(x, y);

    }
    file.close();

    convexHull.quickHull();
    convexHull.writeResults();

    return 0;
}
