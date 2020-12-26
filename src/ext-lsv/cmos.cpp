
#include "ext-lsv/cmos.h"
#include "ext-lsv/graph.h"
#include <iostream>
#include <vector>
#include <algorithm>

namespace lsv
{

static void HelpCommandCmosDual()
{
    Abc_Print(-2, "usage: lsv_cmos_dual [-h] input_file output_file\n");
    Abc_Print(-2, "\t        generate dual network from transistor netlists\n");
    Abc_Print(-2, "\t-h    : print the command usage\n");
}

int CommandCmosDual(Abc_Frame_t* pAbc, int argc, char** argv)
{
    int c;
    Extra_UtilGetoptReset();
    while ((c = Extra_UtilGetopt(argc, argv, "h")) != EOF) {
    switch (c) {
        case 'h':
            HelpCommandCmosDual();
            return 1;
        default:
            HelpCommandCmosDual();
            return 1;
    }
    }

    if( argc!=3 )
    {
        HelpCommandCmosDual();
        return 1;
    }

    Graph nmos_net(argv[1],true);
    nmos_net.dump();

    return 0;
}

static void HelpCommandCmos2Sop()
{
    Abc_Print(-2, "usage: lsv_cmos2sop [-h] [n/p] input_file output_file\n");
    Abc_Print(-2, "\t        conver mos netlist to blif\n");
    Abc_Print(-2, "\t-h    : print the command usage\n");
}

int CommandCmos2Sop(Abc_Frame_t* pAbc, int argc, char** argv)
{
    int c;
    bool isNmos = false;
    Extra_UtilGetoptReset();
    while ((c = Extra_UtilGetopt(argc, argv, "h")) != EOF) {
    switch (c) {
        case 'h':
            HelpCommandCmos2Sop();
            return 1;
        default:
            HelpCommandCmos2Sop();
            return 1;
    }
    }

    if (*argv[1] == 'n') isNmos = true;

    Graph mos_net(argv[2]);
    mos_net.dump();

    Cmos2Sop(&mos_net, isNmos);

    return 0;
}

void Cmos2Sop(Graph* mos_net, bool isNmos)
{
    //mos_net->dump();
    std::vector<std::vector<Node*>> all_path;
    std::vector<Node*> path;
    std::vector<Node*> seen;
    
    // source and sink
    Node *s = mos_net->gnd();
    Node *t = mos_net->out();
    path.push_back(s);

    // Search all path
    Search(s, t, mos_net ,&all_path, &path, &seen);

    // Turn all path into boolean expression

    // dump blif
    
}

void Search(Node* x, Node * t, Graph* mos_net ,std::vector<std::vector<Node*>>* all_path, std::vector<Node*>* path, std::vector<Node*>* seen)
{
    //char temp[256];
    //path->push_back(x);
    if (x->idx == t->idx) {
        std::cout << "----terminate----\n";
        all_path->push_back(*path);
        printPath(*path);
        return;
    }
    
    seen->clear();
    for (Node *n:*path) {
        seen->push_back(n);
    }
    //printf("Seen: ");
    //printPath(*seen);

    if (Stuck(x, t, mos_net, seen)) return;
    //std::cout << "----test----\n";
    for (Node *n: x->neighbors) {
        //std::cout << "x neighbor: " << n->idx << std::endl;
        if (std::count(path->begin(), path->end(), n) == 0) {
            path->push_back(n);
            //std::cout << "----Search Begin----\n";
            //printPath(*path);
            Search(n, t, mos_net , all_path, path, seen);
            //std::cin.get(temp,256);
            //std::cout << "----Search End----\n";
            //printPath(*path);
            path->pop_back();
            //printPath(*path);
            //std::cin >> temp;
        }
    }

}

bool Stuck(Node* x, Node * t, Graph* mos_net , std::vector<Node*>* seen)
{
    //printf("-----stuck---\n");
    //std::cout << "stuck n: " << x->idx << std::endl; 
    if (x->idx == t->idx) return false;
    //printf("Seen: ");
    //printPath(*seen);

    for (Node* n:x->neighbors) {
        bool isSeen = false;
        //std::cout << "x neighbor: " << n->idx << std::endl;
        for (Node *s:*seen) {
            //std::cout << "s: " << s->idx << ", n: " << n->idx << std::endl;
            if (s->idx == n->idx) {
                //printf("QQ\n");
                isSeen = true;
                break;
            }
        }
        if (!isSeen){
            seen->push_back(n);
            if (! Stuck(n, t, mos_net, seen)) return false;
        }
    }
    printf("TRUE\n"); 
    return true;
}

void printPath(std::vector<Node*> path)
{
    for (Node *n:path) {
        std::cout << n->idx << " ";       
    }
    std::cout << std::endl;
}

}   /// end of namespace lsv
