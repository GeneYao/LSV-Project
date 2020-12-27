#ifndef LSV_GRAPH_H
#define LSV_GRAPH_H

#include <vector>
#include <list>
#include <stack>
#include <iostream>
#include <fstream>
#include <cassert>

namespace lsv
{

struct Node;
struct Face;

struct Edge
{
    /// an edge is a transistor
    int var;
    int visited;
    std::vector<int> vars;  /// use vars if no duplicated edges
    Node *n1, *n2;
    Face *f1, *f2;
    Edge( int v, Node* n1_, Node* n2_ ) : var(v), n1(n1_), n2(n2_), f1(nullptr), f2(nullptr) {}
    Edge( Node* n1_, Node* n2_ ) : var(-1), n1(n1_), n2(n2_), f1(nullptr), f2(nullptr) {}
    Edge(): var(-1), n1(nullptr), n2(nullptr), f1(nullptr), f2(nullptr) {}
};

struct Node
{
    /// a node is connected to many transistors
    int idx;
    int visited;
    std::vector<Edge*> edges;
    std::vector<Node*> neighbors;
    Node( int i=-1 ) : idx(i) {}
};

struct Face
{
    int idx;
    std::list<Edge*> edges;
    Face(int i=-1) : idx(i) {}
};

class Graph
{
    /// Node gnd has idx = 0, out has idx = 1
    Node *_gnd, *_out;
    Edge *_ext_edge;
    std::vector<Node*> _nodes;
    std::vector<Edge*> _edges;
    std::vector<Face*> _faces;
    std::vector<Edge*> path;
public:
    Graph()
    {
        _gnd = nullptr;
        _out = nullptr;
        _ext_edge = nullptr;
    }
    Graph(char* input_file, bool dup_edge=false)
    {
        if( dup_edge )
            /// allow duplicated edges
            read_mos_network(input_file);
        else
            read_mos_network_no_dup(input_file);
    }
    ~Graph()
    {
        for( auto* ptr : _nodes ) delete ptr;
        for( auto* ptr : _edges ) delete ptr;
        for( auto* ptr : _faces ) delete ptr;
    }

    Node* gnd() { return _gnd; }
    Node* out() { return _out; }
    Edge* ext_edge() { return _ext_edge; }
    std::vector<Node*>& nodes() { return _nodes; }
    std::vector<Edge*>& edges() { return _edges; }
    std::vector<Face*>& faces() { return _faces; }

    void read_mos_network(const char* input_file);
    void read_mos_network_no_dup(const char* input_file);
    void dump(std::ostream& os=std::cout);
    Edge* find_edge(const Node* n1, const Node* n2);
    Edge* add_edge(int var, Node* n1, Node* n2);
    void add_ext_edge();
    void embed();
    bool find_cycle(Node* node);
    void initial_face();

    void clear_node_visited()
    {
        for( auto* ptr : _nodes )
            if( ptr ) ptr->visited = 0;
    }
    void clear_edge_visited()
    {
        for( auto* ptr : _edges )
            if( ptr ) ptr->visited = 0;
    }
    void reset_workspace()
    {
        clear_node_visited();
        clear_edge_visited();
        path.clear();
    }
};

std::ostream& operator<<(std::ostream& os, const std::vector<Edge*> path);
std::ostream& operator<<(std::ostream& os, const Edge* e);
Node* neighbor(const Node* n, const Edge* e);
bool is_neighbor(const Node* n1, const Node* n2);

}   /// end of namespace lsv

#endif
