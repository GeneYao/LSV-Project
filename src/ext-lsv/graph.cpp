
#include "ext-lsv/graph.h"

namespace lsv
{

Edge* Graph::find_edge(Node* n1, Node* n2)
{
    /// check if edge (n1,n2) exists
    for( Edge* e : _edges )
        if( e && ((e->n1==n1 && e->n2==n2) || (e->n1==n2 && e->n2==n1)) ) return e;
    return nullptr;
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
                if(e->var!=0)
                    os << e->var << " ";
                else
                    for( int var : e->vars )
                        os << var << " ";
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

    os << "edge (n1 : n2) : vars" << std::endl;
    for( Edge* e : _edges )
    {
        if( e )
        {
            os << "( " << e->n1->idx << " : " << e->n2->idx << " ) : " ;
            if(e->var!=0)
                os << e->var << " ";
            else
                for( int var : e->vars )
                    os << var << " ";
        }
        os << std::endl;
    }
}

void Graph::read_mos_network(char* input_file)
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

void Graph::read_mos_network_no_dup(char* input_file)
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

        Node *node1 = _nodes[n1], *node2 = _nodes[n2];
        Edge* e = find_edge(node1,node2);
        if( e )
        {
            e->vars.push_back(var);
        }
        else
        {
            /// add new edge
            e = new Edge(node1,node2);
            e->vars.push_back(var);
            _edges.push_back(e);
            /// update node connectivity
            node1->edges.push_back(e);
            node1->neighbors.push_back(node2);
            node2->edges.push_back(e);
            node2->neighbors.push_back(node1);
        }
    }

    _gnd = _nodes[0];
    _out = _nodes[1];

    ifs.close();
}

}   /// end of namespace lsv
