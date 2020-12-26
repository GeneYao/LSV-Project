#ifndef LSV_GRAPH_H
#define LSV_GRAPH_H

#include <vector>
#include <iostream>
#include <fstream>
#include <cassert>

namespace lsv
{
struct Node;
struct Edge
{
    /// an edge is a transistor
    int var;
    std::vector<int> vars;  /// use vars if no duplicated edges
    Node *n1, *n2;
    Edge( int v, Node* n1_, Node* n2_ ) : var(v), n1(n1_), n2(n2_) {}
    Edge( Node* n1_, Node* n2_ ) : var(0), n1(n1_), n2(n2_) {}
    Edge(): var(0), n1(nullptr), n2(nullptr) {}
};
struct Node
{
    /// a node is connected to many transistors
    int idx;
    std::vector<Edge*> edges;
    std::vector<Node*> neighbors;
    Node( int i=-1 ) : idx(i) {}
};
class Graph
{
    /// Node gnd has idx = 0, out has idx = 1
    Node *_gnd, *_out;
    std::vector<Node*> _nodes;
    std::vector<Edge*> _edges;
public:
    Graph()
    {
        _gnd = nullptr;
        _out = nullptr;
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
    }

    Node* gnd() { return _gnd; }
    Node* out() { return _out; }
    std::vector<Node*>& nodes() { return _nodes; }
    std::vector<Edge*>& edges() { return _edges; }

    void read_mos_network(char* input_file);
    void read_mos_network_no_dup(char* input_file);
    void dump(std::ostream& os=std::cout);
    Edge* find_edge(Node* n1, Node* n2);
};

}   /// end of namespace lsv

#endif
