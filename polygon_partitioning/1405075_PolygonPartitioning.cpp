#include<bits/stdc++.h>
#include<fstream>
using namespace std;

#define START 0
#define SPLIT 1
#define END 2
#define MERGE 3
#define REGULAR 4

#define LEFT 66
#define RIGHT 99

#define zero 1e-10


struct point{

    double x, y;
};


struct vertex{

    int id;
    point coord;
    int type;
    int chain;

    vertex(){}

    vertex(double x, double y, int id){
        this->id = id;
        coord = {x, y};
        type = NULL;
        chain = NULL;
    }
};


struct edge{

    int id;
    vertex startVertex, endVertex;
    vertex helperVertex;

    edge(){}

    edge(vertex u, vertex v, int id){

        this->id = id;
        startVertex = u;
        endVertex = v;

        if(endVertex.coord.y > startVertex.coord.y) swap(startVertex, endVertex);

        else if(fabs(endVertex.coord.y - startVertex.coord.y) <= zero)
            if(endVertex.coord.x < startVertex.coord.x) swap(startVertex, endVertex);
    }

    bool operator<(const edge &e) const{

        if(fabs(startVertex.coord.y - e.startVertex.coord.y) <= zero){
            return startVertex.coord.x < e.startVertex.coord.x;
        }

        if(startVertex.coord.y > e.startVertex.coord.y){
            double x = (e.startVertex.coord.y - startVertex.coord.y)
                        * (startVertex.coord.x - endVertex.coord.x) / (startVertex.coord.y - endVertex.coord.y)
                        + startVertex.coord.x;
            return (x < e.startVertex.coord.x);
        }

        else{
            double x = (startVertex.coord.y - e.startVertex.coord.y)
                        * (e.startVertex.coord.x - e.endVertex.coord.x) / (e.startVertex.coord.y - e.endVertex.coord.y)
                        + e.startVertex.coord.x;
            return (startVertex.coord.x < x);
        }
    }
};


bool yComparator(vertex v1, vertex v2){

    if(fabs(v1.coord.y - v2.coord.y) <= zero) return v1.coord.x > v2.coord.x;
    else return v1.coord.y < v2.coord.y;
}


double dotProduct(point p, point q){

    return p.x*q.x + p.y*q.y;
}


double crossProduct(point p, point q){

    return p.x*q.y-p.y*q.x;
}

//true if on RIGHT side
bool checkSide(vertex minusV, vertex V, vertex plusV){

    if(minusV.coord.x * (V.coord.y - plusV.coord.y)
       - V.coord.x * (minusV.coord.y - plusV.coord.y)
       + plusV.coord.x * (minusV.coord.y - V.coord.y) > 0) return true;

    return false;
}


void setVertexType(vertex &minusV, vertex &V, vertex &plusV){

    if(V.coord.y > minusV.coord.y && V.coord.y > plusV.coord.y){
        if(checkSide(minusV, V, plusV)) V.type = START;
        else V.type = SPLIT;
    }

    else if(V.coord.y < minusV.coord.y && V.coord.y < plusV.coord.y){
        if(checkSide(minusV, V, plusV)) V.type = END;
        else V.type = MERGE;
    }

    else V.type = REGULAR;
}


vector<vertex> vertices;
vector<edge> edges;

priority_queue <vertex, vector<vertex>, bool (*)(vertex, vertex)> verticesPQ(yComparator);
set<edge> tree;
vector<pair<int,int> > diagonals;


bool checkInterior(vertex v){

    int n = vertices.size();
    vertex prevV = vertices[(v.id-1+n)%n];

    if(prevV.coord.y > v.coord.y) return true;
    else if(prevV.coord.y < v.coord.y) return false;
    else{
        if(prevV.coord.x < v.coord.x) return true;
        return false;
    }
}

void setChainType(){

    int yMAX = -99999;
    int yMIN = 99999;
    int topVertexId, bottomVertexId;

    for(int i = 0; i < vertices.size(); i++){
        if(vertices[i].coord.y > yMAX){
            yMAX = vertices[i].coord.y;
            topVertexId = vertices[i].id;
        }
        if(vertices[i].coord.y < yMIN){
            yMIN = vertices[i].coord.y;
            bottomVertexId = vertices[i].id;
        }
    }

    for(int i = 0; i < vertices.size(); i++){
        if(i >= min(topVertexId, bottomVertexId) && i <= max(topVertexId, bottomVertexId)){
            vertices[i].chain = LEFT;
        }
        else vertices[i].chain = RIGHT;
    }

}


void constructPolygon(){

    ifstream infile;
    infile.open("1405075_input2.txt");

    int n;
    infile >> n;

    for(int i = 0; i < n; i++){
        double x, y;
        infile >> x >> y;
        vertex temp(x, y, i);
        vertices.push_back(temp);
    }

    infile.close();

    for(int i = 0; i < vertices.size(); i++){
        setVertexType(vertices[i], vertices[(i+1)%vertices.size()],
                       vertices[(i+2)%vertices.size()]);

        edge temp(vertices[i], vertices[(i+1)%vertices.size()], i);
        edges.push_back(temp);
    }

    setChainType();

    for(int i = 0; i < vertices.size(); i++){
        vertex temp = vertices[i];
        verticesPQ.push(temp);
    }
}


void handleStartVertex(vertex v){

    tree.insert(edges[v.id]);
    edges[v.id].helperVertex = v;

//    cout<<"EDGE: "<<edges[v.id].id<<"  VERTEX: "<<v.id<<endl;
}


void handleSplitVertex(vertex v){

    edge temp(v, v, v.id);
    set<edge>::iterator it = tree.lower_bound(temp);
    it--;

    vertex helper = edges[it->id].helperVertex;

    diagonals.push_back(make_pair(v.id, helper.id));

//    cout<<"EDGE: "<<edges[it->id].id<<"  VERTEX: "<<v.id<<endl;
    edges[it->id].helperVertex = v;

    tree.insert(edges[v.id]);
    edges[v.id].helperVertex = v;
//    cout<<"EDGE: "<<edges[v.id].id<<"  VERTEX: "<<v.id<<endl;
}


void handleEndVertex(vertex v){

    int n = vertices.size();
    vertex helper = edges[(v.id-1+n)%n].helperVertex;

    if(helper.type == MERGE){
        diagonals.push_back(make_pair(v.id, helper.id));
    }
    tree.erase(edges[(v.id-1+n)%n]);
}


void handleMergeVertex(vertex v){

    int n = vertices.size();
    vertex helper = edges[(v.id-1+n)%n].helperVertex;

    if(helper.type == MERGE){
        diagonals.push_back(make_pair(v.id, helper.id));
    }
    tree.erase(edges[(v.id-1+n)%n]);

    edge temp(v, v, v.id);
    set<edge>::iterator it = tree.lower_bound(temp);
    it--;

    helper = edges[it->id].helperVertex;

    if(helper.type == MERGE){
        diagonals.push_back(make_pair(v.id, helper.id));
    }
    edges[it->id].helperVertex = v;

//    cout<<"EDGE: "<<edges[it->id].id<<"  VERTEX: "<<v.id<<endl;
}


void handleRegularVertex(vertex v){

    int n = vertices.size();
    vertex helper = edges[(v.id-1+n)%n].helperVertex;

    if(checkInterior(v)){
        if(helper.type == MERGE){
            diagonals.push_back(make_pair(v.id, helper.id));
        }
        tree.erase(edges[(v.id-1+n)%n]);

        tree.insert(edges[v.id]);
        edges[v.id].helperVertex = v;

//    cout<<"EDGE: "<<edges[v.id].id<<"  VERTEX: "<<v.id<<endl;
    }

    else{
        edge temp(v, v, v.id);
        set<edge>::iterator it = tree.lower_bound(temp);
        it--;

        helper = edges[it->id].helperVertex;

        if(helper.type == MERGE){
            diagonals.push_back(make_pair(v.id, helper.id));
        }
        edges[it->id].helperVertex = v;

//    cout<<"EDGE: "<<edges[it->id].id<<"  VERTEX: "<<v.id<<endl;
    }
}


void makeMonotone(){

    while(!verticesPQ.empty()){
        vertex temp = verticesPQ.top();
        verticesPQ.pop();

        switch(temp.type){
            case START:
                handleStartVertex(temp);
                break;
            case SPLIT:
                handleSplitVertex(temp);
                break;
            case END:
                handleEndVertex(temp);
                break;
            case MERGE:
                handleMergeVertex(temp);
                break;
            case REGULAR:
                handleRegularVertex(temp);
                break;
            default:
                break;
        }
    }
}


stack<vertex> S;

void TriangulateMonotonePolygon(){

    sort(vertices.begin(), vertices.end(), yComparator);
    S.push(vertices[0]);
    S.push(vertices[1]);

    for(int i = 2; i < vertices.size(); i++){
        vertex temp = S.top();

        if(vertices[i].chain != temp.chain){

            while(!S.empty()){
                vertex v = S.top();

                if(S.size() > 1) diagonals.push_back(make_pair(temp.id, vertices[i].id));
                S.pop();

                S.push(vertices[i-1]);
                S.push(vertices[i]);
            }
        }

        else{
            S.pop();

            while(!S.empty()){

                if(vertices[i].chain == LEFT){
                    vertex v = S.top();

                    if(v.coord.x < vertices[i].coord.x) diagonals.push_back(make_pair(v.id, vertices[i].id));
                    S.pop();

                    if(S.size() == 1) S.push(v);

                }

                else if(vertices[i].chain == RIGHT){
                    vertex v = S.top();

                    if(v.coord.x > vertices[i].coord.x) diagonals.push_back(make_pair(v.id, vertices[i].id));
                    S.pop();

                    if(S.size() == 1) S.push(v);

                }
            }

        }
    }
}


int main(){

    constructPolygon();
    makeMonotone();

    ofstream outfile;
    outfile.open("1405075_output.txt");

    cout << "Diagonals: " <<endl;
    for(int i = 0; i < diagonals.size(); i++){
        int firstVertex = diagonals[i].first;
        int secondVertex = diagonals[i].second;

        cout << firstVertex << " " << secondVertex << endl;
        outfile << vertices[firstVertex].coord.x << " " << vertices[firstVertex].coord.y <<endl;
        outfile << vertices[secondVertex].coord.x << " " << vertices[secondVertex].coord.y <<endl;
    }

    outfile.close();

    return 0;
}

