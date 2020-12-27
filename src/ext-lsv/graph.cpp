
#include "ext-lsv/graph.h"

namespace lsv
{

Node* neighbor(const Node* n, const Edge* e)
{
    if( n==e->n1 ) return e->n2;
    return e->n1;
}

bool is_neighbor(const Node* n1, const Node* n2)
{
    for( Node* n : n1->neighbors )
        if( n==n2 ) return true;
    return false;
}

std::ostream& operator<<(std::ostream& os, const Edge* e)
{
    if(e->var!=-1)
        os << e->var << " ";
    else
        for( int var : e->vars )
            os << var << " ";
    return os;
}
std::ostream& operator<<(std::ostream& os, const std::vector<Edge*> path)
{
    for( Edge* e : path )
        os << e << "-- ";
    return os;
}

bool Graph::find_cycle(Node* node)
{
    /// find a cycle through _out, put in path vector
    if( node )
    {
        if( node==_out && path.size()>2  )
        {
            return true;
        }
        else if( node->visited == 1 )
        {
            return false;
        }
        else
        {
            node->visited = 1;
            for( Edge* e : node->edges )
            {
                path.push_back(e);
                if( find_cycle( neighbor(node, e) ) ) return true;
                path.pop_back();
            }
        }
    }
    return true;
}

void Graph::initial_face()
{
    /// initialize two faces by the path
    for( auto* ptr : _faces ) delete ptr;
    _faces.clear();

    Face* f1 = new Face(0);
    Face* f2 = new Face(1);
    _faces.push_back(f1);
    _faces.push_back(f2);
    for( Edge* e : path )
    {
        e->f1 = f1;
        e->f2 = f2;
        f1->edges.push_back(e);
        f2->edges.push_back(e);
    }
    path.clear();
}

void Graph::embed()
{
    /// embed graph onto plane, generate all faces
    reset_workspace();
    find_cycle(_out);
    std::cout << path << std::endl;
    initial_face();

}

Edge* Graph::find_edge(const Node* n1, const Node* n2)
{
    /// check if edge (n1,n2) exists
    for( Edge* e : _edges )
        if( e && ((e->n1==n1 && e->n2==n2) || (e->n1==n2 && e->n2==n1)) ) return e;
    return nullptr;
}

Edge* Graph::add_edge(int var, Node* n1, Node* n2)
{
    /// add new edge (non-dup edge style)
    Edge* e = find_edge(n1, n2);
    if( e )
    {
        e->vars.push_back(var);
    }
    else
    {
        e = new Edge(n1,n2);
        e->vars.push_back(var);
        _edges.push_back(e);
        /// update node connectivity
        n1->edges.push_back(e);
        n1->neighbors.push_back(n2);
        n2->edges.push_back(e);
        n2->neighbors.push_back(n1);
    }
    return e;
}

void Graph::add_ext_edge()
{
    assert( _gnd!=nullptr && _out!=nullptr );
    _ext_edge = new Edge(_out,_gnd);
    _ext_edge->vars.push_back(0);

    _edges.push_back(_ext_edge);
    _out->edges.push_back(_ext_edge);
    _gnd->edges.push_back(_ext_edge);

    if( !is_neighbor(_out,_gnd) )
    {
        _out->neighbors.push_back(_gnd);
        _gnd->neighbors.push_back(_out);
    }
}

void Graph::dump(std::ostream& os)
{
    os << "node: edges" << std::endl;
    for( Node* n : _nodes )
    {
        if( n )
        {
            os << " " << n->idx << " : ";
            for( Edge* e : n->edges )
            {
                os << e << ", ";
            }
        }
        os << std::endl;
    }

    os << "node: neighbors" << std::endl;
    for( Node* n : _nodes )
    {
        if( n )
        {
            os << " " << n->idx << " : ";
            for( Node* neighbor : n->neighbors )
                os << neighbor->idx << " ";
        }
        os << std::endl;
    }

    os << "edge (n1 , n2) : vars : f1, f2" << std::endl;
    for( Edge* e : _edges )
    {
        if( e )
        {
            os << "( " << e->n1->idx << " , " << e->n2->idx << " ) : " ;
            os << e << ", ";
            os << ": ";
            if( e->f1 ) os << e->f1->idx << " , ";
            else os << "null , ";
            if( e->f2 ) os << e->f2->idx << "";
            else os << "null";
        }
        os << std::endl;
    }

    os << "face : edges" << std::endl;
    for( Face* f : _faces )
    {
        if( f )
        {
            os << " " << f->idx << " : " ;
            for( Edge* e : f->edges )
                os << e << ", ";
        }
        os << std::endl;
    }
}

void Graph::read_mos_network(const char* input_file)
{
    std::cout << "read transistor network file : " << input_file << std::endl;
    std::ifstream ifs(input_file);
    assert( ifs.is_open() );

    int num_node, num_edge;
    ifs >> num_node >> num_edge;

    _nodes.resize(num_node,nullptr);
    _edges.resize(num_edge,nullptr);

    for( int i=0; i<num_node; ++i )
    {
        _nodes[i] = new Node(i);
    }

    for( int i=0; i<num_edge; ++i )
    {
        char type;
        int var, n1, n2;
        ifs >> type >> var >> n1 >> n2;
        _edges[i] = new Edge(var, _nodes[n1], _nodes[n2]);
        _nodes[n1]->edges.push_back(_edges[i]);
        _nodes[n2]->edges.push_back(_edges[i]);
        _nodes[n1]->neighbors.push_back(_nodes[n2]);
        _nodes[n2]->neighbors.push_back(_nodes[n1]);
    }

    _gnd = _nodes[0];
    _out = _nodes[1];

    ifs.close();
}

void Graph::read_mos_network_no_dup(const char* input_file)
{
    std::cout << "read transistor network file : " << input_file << std::endl;
    std::ifstream ifs(input_file);
    assert( ifs.is_open() );

    int num_node, num_edge;
    ifs >> num_node >> num_edge;

    /// allocate for nodes
    _nodes.resize(num_node);
    for( int i=0; i<num_node; ++i )
        _nodes[i] = new Node(i);

    /// read all edges
    for( int i=0; i<num_edge; ++i )
    {
        char type;
        int var, n1, n2;
        ifs >> type >> var >> n1 >> n2;
        add_edge(var,_nodes[n1],_nodes[n2]);
    }

    _gnd = _nodes[0];
    _out = _nodes[1];

    ifs.close();
}

}   /// end of namespace lsv
