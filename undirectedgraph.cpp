#include <utility>
#include <unordered_map>
#include <fstream>
#include <queue>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cstring>
#include <string>
#include <vector>
#include <stdio.h>
#include <bits/stdc++.h>

using namespace std;

// Vertex Class
class Vertex {
    public: // constructor of Vertex
        Vertex() {}
        Vertex(string element): element(element) {}
        string getElem() const { return element; }
        string operator*() const { return element; } // Overloaded * operator
    private:
        string element;
};

//Edge Class
class Edge {
    public: // constructor of Edge
        Edge() {}
        Edge(Vertex v1, Vertex v2, string element): v1(v1), v2(v2), element(element) {}
        Vertex getFirst() const { return v1; }
        Vertex getSecond() const { return v2; }
        string getElem() const { return element; }
        string operator*() const { return element; } // Overloaded * operator

        //if first/second vertex of edge == v
        bool isIncidentOn(Vertex v){
            return ((getFirst().getElem() == v.getElem()) || (getSecond().getElem() == v.getElem())) ? true : false;
        }
        //if first/second vertex of edge == first/second vertex of f
        bool isAdjacentTo(Edge f){
            return ((getFirst().getElem() == f.getFirst().getElem()) || (getFirst().getElem() == f.getSecond().getElem()) || (getSecond().getElem() == f.getFirst().getElem()) || (getSecond().getElem() == f.getFirst().getElem())) ? true : false;
        }
        //return opposite of v on edge, return v if v not on edge
        Vertex opposite(Vertex v){
            if (getFirst().getElem()!=v.getElem() && getSecond().getElem()!=v.getElem()) {return v;}
            return (getFirst().getElem()==v.getElem()) ? getSecond() : getFirst();
        }
        //return vector{v1,v2}
        vector<Vertex> endVertices(){
            vector<Vertex> vList;
            vList.push_back(v1);
            vList.push_back(v2);
            return vList;
        }

    private:
        Vertex v1;
        Vertex v2;
        string element;
};

// Graph Class
class Graph {
    public:
        //graph consists of vectors storing edges and vertices
        vector<Edge> EdgeList;
        vector<Vertex> VertexList;
        Graph(string filename) {
            ifstream file(filename);
            string line;

            // Skip first line
            getline(file, line);
            while (getline(file, line)) {
                // read file for vertices and edges aimultaneously
                string v1_element, v2_element, element;
                stringstream stringstream(line);
                getline(stringstream, v1_element, '\t');
                getline(stringstream, v2_element, '\t');
                getline(stringstream, element, '\t');
                Vertex v1(v1_element);
                Vertex v2(v2_element);
                Edge e(v1, v2, element);

                // since reading vertices from edges lines, remove duplicate vertices
                int existed = 0;
                for (const Vertex& v : VertexList) {
                    if (v1.getElem() == v.getElem()){existed = 1; break;}
                    }
                if (existed == 0){
                    VertexList.push_back(v1);
                }
                existed = 0;
                for (const Vertex& v : VertexList) {
                    if (v2.getElem() == v.getElem()){existed = 1; break;}
                    }
                if (existed == 0){
                    VertexList.push_back(v2);
                }  

                EdgeList.push_back(e);
            }
        }

        //access vectors of edges and vertices
        vector<Vertex> getVertexList() const { return VertexList; }
        vector<Edge> getEdgeList() const { return EdgeList; }


        //insert vertex = push vertex to VertexList
        void insertVertex(Vertex v) {
            VertexList.push_back(v);
        }

        //insert edge
        void insertEdge(Edge e) {
            int exist = 0;
            for (const Edge& e1 : EdgeList) {
                //if both edge's elements already exist and connected
                if ((e1.getFirst().getElem() == e.getFirst().getElem() && e1.getSecond().getElem() == e.getSecond().getElem()) || (e1.getFirst().getElem() == e.getSecond().getElem() && e1.getSecond().getElem() == e.getFirst().getElem())){
                    exist = 1;
                    cout << "\nEdge insert failed: edge already exists." << endl;
                }
            }
            if (exist == 0){
                EdgeList.push_back(e);
                cout << "\nEdge inserted." << endl;
                
            }
        }

        //erase a vertex from VertexList and erase edges with it as their elem in EdgeList
        void eraseVertex(const Vertex* v) {
            EdgeList.erase(remove_if(EdgeList.begin(), EdgeList.end(), [&](Edge& e) {
                return e.getFirst().getElem() == v->getElem() || e.getSecond().getElem() == v->getElem();
            }), EdgeList.end());
            VertexList.erase(remove_if(VertexList.begin(), VertexList.end(), [&](Vertex& u) {
                return u.getElem() == v->getElem();
            }), VertexList.end());
        }

        //iter through EdgeList, compare two elem pairs to find, remove edge
        void eraseEdge(const Edge* e) {
            for (auto iter = EdgeList.begin(); iter != EdgeList.end();) {
                if (iter->getFirst().getElem() == e->getFirst().getElem() && iter->getSecond().getElem() == e->getSecond().getElem() && iter->getElem() == e->getElem()) {
                    iter = EdgeList.erase(iter);
                    return;}
                else {++iter;}
            }
        }

        //iter through EdgeList, find all edges with vertex, return vector of edges
        vector<Edge> incidentEdges(Vertex v) const {
            vector<Edge> result;
            for (auto iter = EdgeList.begin(); iter != EdgeList.end(); ++iter) {
                if (iter->getFirst().getElem() == v.getElem() || iter->getSecond().getElem() == v.getElem()) {
                    result.push_back(*iter);
                }
            }
            return result;
        }

        //iter EdgeList, check if two vertices are vertices of one edge
        bool areAdjacent(Vertex v1, Vertex v2) const {
            for (auto iter = EdgeList.begin(); iter != EdgeList.end(); ++iter) {
                if ((iter->getFirst().getElem() == v1.getElem() && iter->getSecond().getElem() == v2.getElem()) ||
                    (iter->getFirst().getElem() == v2.getElem() && iter->getSecond().getElem() == v1.getElem())) {
                                    return true; }}}

        vector<Vertex> findPath(Graph g, Vertex start, Vertex end) {
            // Keep track of visited VertexList and their parents
            unordered_map<string, Vertex> parent;
            unordered_map<string, bool> visited;

            // Mark the start vertex as visited and enqueue it
            visited[start.getElem()] = true;
            queue<Vertex> q;
            q.push(start);
            // Perform BFS
            while (!q.empty()) {
                Vertex current = q.front();
                q.pop();
                vector<Edge> Edges = g.incidentEdges(current);

                // if start end adjacent, ignore their edge cause path >= 3 vertices
                for (const Edge& e : Edges) {
                    if ((e.getFirst().getElem()==start.getElem() && e.getSecond().getElem()==end.getElem()) || (e.getSecond().getElem()==start.getElem() && e.getFirst().getElem()==end.getElem())){
                        for (auto iter = Edges.begin(); iter != Edges.end();) {
                            if (iter->getFirst().getElem() == e.getFirst().getElem() && iter->getSecond().getElem() == e.getSecond().getElem() && iter->getElem() == e.getElem()){
                                iter = Edges.erase(iter);
                                break;}
                            else {++iter;}}
                        break;
                    }
                }

                //  if current this vertice, find edge with this vertice to reach next vertice until reaching end
                for (const Edge& e : Edges) {
                    Vertex next = (e.getFirst().getElem() == current.getElem()) ? e.getSecond() : e.getFirst();
                    if (!visited[next.getElem()]) {
                        visited[next.getElem()] = true;
                        parent[next.getElem()] = current;
                        q.push(next);
                        if (next.getElem() == end.getElem()) {
                            // after reaching end, build path backwards
                            vector<Vertex> path;
                            Vertex v = end;
                            while (v.getElem() != start.getElem()) {
                                path.push_back(v);
                                v = parent[v.getElem()];
                            }
                            path.push_back(start);
                            //end->start switch to start->end
                            reverse(path.begin(), path.end());
                            return path;
                        }
                    }
                }
            }
            // If no path was found
            cout <<"No path was found." <<endl;
            return vector<Vertex>();}      
};

int main(){
    string fname;
    char choice[1000];
    cout << "Hello!\nEnter the file name: ";
    getline(cin, fname);
    Graph g(fname); // create object Graph named g
    cout << "\nThank you. Your graph is ready.\n\n------------\nWhat would you like to do?\n------------\n\n1. Find edges incident on a vertex\n2. Find a path in the graph\n3. Insert an edge\n4. Erase a vertex\n\n";
    cout << "Your choice: ";
    try {cin >> choice;} 
    catch(...){throw invalid_argument("Invalid option. Goodbye!");}

    if (strcmp(choice,"1")==0){
        string element_input;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); //flush
        cout << "Label/Element of the vertex? ";
        getline(cin, element_input);
        cout << endl;
        int existed = 0;

        //iter VertexList, find vertex to call incidentEdges
        for (const Vertex& vertex_input : g.getVertexList()) {
            if (element_input == vertex_input.getElem()){
                existed = 1;
                vector<Edge> incidentEdgess = g.incidentEdges(vertex_input);
                for (const Edge& e : incidentEdgess) {
                    if (e.getFirst().getElem() != element_input){cout << element_input << " to " << e.getFirst().getElem() << " is " << e.getElem() << endl;}
                    else{cout << element_input << " to " << e.getSecond().getElem() << " is " << e.getElem() << endl;}
                }
            }
        }
        if (existed == 0){cout << "Vertex doesn't exist." << endl;}
    }

    else if (strcmp(choice,"2")==0){
        string start_input;
        string end_input;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); //flush
        cout << "\nLabel/Element of first vertex? \t";
        getline(cin, start_input);
        cout << "\nLabel/Element of second vertex? ";
        getline(cin, end_input);
        vector<Vertex> path = g.findPath(g, start_input, end_input); //second para start, third para end

        // Print the path
        for (const Vertex& v : path) {
            if (v.getElem() != end_input){cout << v.getElem() << " to ";}
            else{cout << v.getElem() << endl;}
        }
    }

    else if (strcmp(choice,"3")==0){
        string vertex1_input;
        string vertex2_input;
        string element;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "\nLabel/Element of first vertex? \t";
        getline(cin, vertex1_input);
        cout << "\nLabel/Element of second vertex? ";
        getline(cin, vertex2_input);
        cout << "\nLabel/Element of the edge? ";
        getline(cin, element);

        //edge not inserted if either vertex unfound
        int exist1 = 0, exist2 = 0;
        for (const Vertex& v : g.getVertexList()) {
            if (vertex1_input == v.getElem()){exist1=1;}
            if (vertex2_input == v.getElem()){exist2=1;}}
        if (exist1 == 0 || exist2 == 0 ){
            cout << "\nEdge insert failed: Vertices do not exist." << endl;
        }
        else{ //create 2 Vertex objs to create one Edge obj to insert
            Vertex vertex1(vertex1_input);
            Vertex vertex2(vertex2_input);
            Edge edging(vertex1, vertex2, element);
            g.insertEdge(edging);
        }
    }


    else if (strcmp(choice,"4")==0){
        string removed;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); //flush
        cout << "\nLabel/Element of the vertex to be removed? ";
        getline(cin, removed);

        //iter VertexList, find vertex to call eraseVertex
        int exist3 = 0;
        for (const Vertex& v : g.getVertexList()) {
            if (removed == v.getElem()){exist3=1; g.eraseVertex(&v);}}
        if (exist3==1){cout << "Vertex removed."<<endl;}
        else{cout << "Removal cancelled. Vertex not found."<<endl;}
    }

    else{throw invalid_argument("Invalid option. Goodbye!");}

    return 0;
}