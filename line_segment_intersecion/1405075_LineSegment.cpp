#include<bits/stdc++.h>
#include<fstream>
#include<vector>
using namespace std;

#define eps 1e-10
#define inf 1e18

double X, Y;
ofstream outfile;

int checkZero(double x){
    if(fabs(x) < eps) return 0;
    else if (x < 0) return -1;
    return 1;
}

struct Point{
    double x, y;

    Point(){}

    Point(double a, double b){
        x = a;
        y = b;
    }

    bool operator==(const Point& u) const{
        return checkZero(x - u.x) == 0 && checkZero(y - u.y) == 0;
    }

    bool operator!=(const Point& u) const{
        return !(*this == u);
    }

    bool operator<(const Point& u) const{
        return checkZero(y - u.y) < 0 || (checkZero(y - u.y) == 0 && checkZero(x-u.x) > 0);
    }

    bool operator>(const Point& u) const{
        return u < *this;
    }

    Point operator+(const Point& u){
        return Point(x + u.x, y + u.y);
    }

    Point operator-(const Point& u){
        return Point(x - u.x, y - u.y);
    }

    Point operator*(const double u){
        return Point(x * u, y * u);
    }

    Point operator/(const double u){
        return Point(x / u, y / u);
    }

    double operator*(const Point& u){
        return x * u.y - y * u.x;
    }

};

double dotProduct(Point p, Point q){
    return p.x*q.x + p.y*q.y;
}

double crossProduct(Point p, Point q){
    return p.x*q.y - p.y*q.x;
}

double magnitude(Point p){
    return sqrt(dotProduct(p, p));
}

bool onSegment(Point p, Point a, Point b){
    Point x = a - p;
    Point y = b - p;

    return checkZero(crossProduct(x, y)) == 0
        && checkZero(dotProduct(x, y)) < 0;
}

bool haveIntersection(Point x1, Point y1, Point x2, Point y2) {
    double c1, c2, c3, c4;
    c1 = crossProduct(y1-x1, x2-x1);
    c2 = crossProduct(y1-x1, y2-x1);
    c3 = crossProduct(y2-x2, x1-x2);
    c4 = crossProduct(y2-x2,y1-x2);

    return checkZero(c1)*checkZero(c2) < 0 && checkZero(c3)*checkZero(c4) < 0;
}

bool checkIntersection(Point p, Point v, Point q, Point w, Point& o){
    if (checkZero(crossProduct(v, w)) == 0) return false;

    Point u = p - q;
    double k = crossProduct(w, u) / crossProduct(v, w);
    o = p + v * k;

    return true;
}

class Segment{
public:
    Point startPoint, endPoint;
    int id;

    Segment(){}

    Segment(Point a, Point b) {
        startPoint = a;
        endPoint = b;

        if(startPoint < endPoint) swap(startPoint, endPoint);
    }

    bool operator==(const Segment& s) const{
        return (startPoint == s.startPoint) && (endPoint == s.endPoint);
    }

    double nearestX(Segment s) const{
        double top = (s.startPoint.x - s.endPoint.x) * Y - (s.startPoint.x * s.endPoint.y - s.endPoint.x * s.startPoint.y);
        double bottom = s.startPoint.y - s.endPoint.y;

        if(checkZero(bottom) == 0) return X + eps;
        else return top / bottom;
    }

    bool operator<(const Segment& s) const{
        return nearestX(*this) < nearestX(s);
    }
};

vector<Segment> Q;
vector<Point> intersections;
set<Segment> T;
priority_queue<Point> eventPoints;
map<Point, vector<Segment> > upperPoints;

void findNewEvent(Segment left, Segment right, Point p){
        if(!haveIntersection(left.startPoint, left.endPoint, right.startPoint, right.endPoint)) return;

        Point i;
        checkIntersection(left.startPoint, left.endPoint - left.startPoint, right.startPoint, right.endPoint - right.startPoint, i);

        if(i < p) eventPoints.push(i);
}

void handleEventPoint(Point p) {
    X = p.x;
    Y = p.y;
    vector<Segment> U, C, L;

    if(upperPoints.count(p)) {
        U = upperPoints[p];
    }

    Segment s = Segment(p, Point(p.x - 1, p.y + 1));
    auto it = T.lower_bound(s);

    Segment left, right;

    auto prev = it;

    while(true){
        if(prev == T.begin()) break;
        prev--;
        if(prev->endPoint == p) L.push_back(*prev);
        else if(onSegment(p, prev->startPoint, prev->endPoint)) C.push_back(*prev);
        else break;
    }

    auto next = it;
    while(true){
        if(next == T.end()) break;
        if(next->endPoint == p) L.push_back(*next);
        else if(onSegment(p, next->startPoint, next->endPoint)) C.push_back(*next);
        else break;

        next++;
    }

    if(L.size() + C.size() + U.size() > 1){
        intersections.push_back(p);
        cout << "Point: ";
        cout << p.x << " " << p.y;
        outfile << p.x << " " << p.y << endl;
        cout << ", Lines: ";

        vector<Segment>::iterator itr;
        for (itr=L.begin();itr<L.end();itr++) {
            cout<<(*itr).id<<" ";
        }
        for (itr=C.begin();itr<C.end();itr++) {
            cout<<(*itr).id<<" ";
        }
        for (itr=U.begin();itr<U.end();itr++) {
            cout<<(*itr).id<<" ";
        }
        cout<<endl;
    }

    Y = p.y + eps;
    vector<Segment>::iterator itr;
    for (itr=L.begin();itr<L.end();itr++) {
        T.erase(*itr);
        }
    for (itr=C.begin();itr<C.end();itr++) {
        T.erase(*itr);
    }
    for (itr=U.begin();itr<U.end();itr++) {
        T.erase(*itr);
    }


    Y = p.y - eps;
    for (itr=U.begin();itr<U.end();itr++) {
        T.insert(*itr);
    }
    for (itr=C.begin();itr<C.end();itr++) {
        T.insert(*itr);
    }


    if(U.size() + C.size() == 0){
        Y = p.y;

        auto it = T.lower_bound(s);
        if(it != T.end() && it != T.begin()){
            Segment s_r = *it;
            Segment s_l = *--it;
            findNewEvent(s_l, s_r, p);
        }
    }
    else{
        Y = p.y - eps;
        vector<Segment> temp;
        for(Segment s : U) temp.push_back(s);
        for(Segment s : C) temp.push_back(s);
        sort(temp.begin(), temp.end());
        Segment left = temp[0];
        Segment right = temp.back();

        assert(T.find(left) != T.end());
        assert(T.find(right) != T.end());

        auto pt = T.lower_bound(left);
        auto qt = T.upper_bound(right);

        if(pt != T.begin()) {
            pt--;
            findNewEvent(left, *pt, p);
        }
        if(qt != T.end()) {
            findNewEvent(right, *qt, p);
        }
    }
}

void findIntersections(){
    vector<Segment>:: iterator itr;

    for (itr = Q.begin(); itr < Q.end(); itr++) {
        Segment s = *itr;
        eventPoints.push(s.startPoint);
        eventPoints.push(s.endPoint);
        upperPoints[s.startPoint].push_back(s);
    }

    while(!eventPoints.empty()){
        Point p = eventPoints.top();
        while(!eventPoints.empty() && eventPoints.top() == p) eventPoints.pop();
        handleEventPoint(p);
    }
}

int main(){
    int n;
    X = 0;
    Y = 0;
    ifstream infile;
    infile.open("1405075_input2.txt");

    infile >> n;
    for(int i = 0; i < n; i++){
        double px, py, qx, qy;
        infile >> px >> py >> qx >> qy;

        Point p = Point(px, py);
        Point q = Point(qx, qy);

        Q.push_back(Segment(p, q));
        Q[i].id = i + 1;
    }
    infile.close();

    outfile.open("1405075_output.txt");
    findIntersections();
    outfile.close();
}
