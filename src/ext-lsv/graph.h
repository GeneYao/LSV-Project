#ifndef LSV_GRAPH_H
#define LSV_GRAPH_H

#include <vector>
#include <deque>
#include <list>
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
    int embedded;
    std::vector<Edge*> edges;
    std::vector<Node*> neighbors;
    Node( int i=-1 ) : idx(i) {}
};

struct Face
{
    int idx;
    int node_idx;
    std::list<Edge*> edges;
    Face(int i=-1) : idx(i), node_idx(-1) {}
};

class Graph
{
    /// Node gnd has idx = 0, out has idx = 1
    Node *_gnd, *_out;
    Edge *_ext_edge;
    std::vector<Node*> _nodes;
    std::vector<Edge*> _edges;
    std::vector<Face*> _faces;
    std::deque<Edge*> path;
    Node *path_front, *path_back;
    int extra_node_idx, node_idx;
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

    /// access members
    Node* gnd() { return _gnd; }
    Node* out() { return _out; }
    Edge* ext_edge() { return _ext_edge; }
    std::vector<Node*>& nodes() { return _nodes; }
    std::vector<Edge*>& edges() { return _edges; }
    std::vector<Face*>& faces() { return _faces; }

    /// functions in graph.cpp
    void read_mos_network(const char* input_file);
    void read_mos_network_no_dup(const char* input_file);
    void dump(std::ostream& os=std::cout);
    Edge* find_edge(const Node* n1, const Node* n2);
    Edge* add_edge(int var, Node* n1, Node* n2);
    Edge* find_extendable_edge( Node* node );
    void add_ext_edge();
    void embed();
    bool find_cycle(Node* node);
    void initial_face();
    void embed_all_edges();
    void extend_path();
    void slice_by_path();
    Face* find_face( Node* node, Node* target, Face* current_face, std::deque<Edge*>& path );
    void dump_dual(const char* output_file);
    void get_dual(std::ofstream& ofs, Edge* edge);

    /// inline shorthands
    void clear_node_visited()
    {
        for( auto* ptr : _nodes )
            if( ptr ) ptr->visited = 0;
    }
    void clear_node_embedded()
    {
        for( auto* ptr : _nodes )
            if( ptr ) ptr->embedded = 0;
    }
    void clear_edge_visited()
    {
        for( auto* ptr : _edges )
            if( ptr ) ptr->visited = 0;
    }
    void reset_workspace()
    {
        clear_node_embedded();
        clear_node_visited();
        clear_edge_visited();
        path.clear();
    }
    int contracted_size()
    {
        int size = 0;
        for( Edge* e : _edges )
        {
            size += e->vars.size()-1;
        }
        return size;
    }
};

/// utilities
std::ostream& operator<<(std::ostream& os, const std::deque<Edge*> path);
std::ostream& operator<<(std::ostream& os, const Edge* e);
Node* neighbor(const Node* n, const Edge* e);
bool is_neighbor(const Node* n1, const Node* n2);

}   /// end of namespace lsv

#endif
