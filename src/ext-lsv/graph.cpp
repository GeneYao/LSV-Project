
#include "ext-lsv/graph.h"

namespace lsv
{

void Graph::dump(std::ostream& os)
{
    os << "node: edges" << std::endl;
    for( Node* n : _nodes )
    {
        if( n )
        {
            os << " " << n->idx << " : ";
            for( Edge* e : n->edges ) os << e->var << " ";
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
    }

    _gnd = _nodes[0];
    _out = _nodes[1];

    ifs.close();
}

}   /// end of namespace lsv
