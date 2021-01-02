
#include "ext-lsv/cmos.h"
#include "ext-lsv/graph.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>

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

    Graph nmos_net(argv[1]);
    nmos_net.add_ext_edge();

    if( nmos_net.embed()==0 )
    {
        nmos_net.dump();
        nmos_net.dump_dual(argv[2]);
    }
    else
    {
        std::cout << "Error: " << argv[1] << " : netlist is non-planar" << std::endl;
    }
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

    Cmos2Sop(&mos_net, isNmos, argc, argv);

    return 0;
}

void Cmos2Sop(Graph* mos_net, bool isNmos, int argc, char** argv)
{
    //mos_net->dump();
    //char buff[256];
    std::vector<std::vector<Node*>> all_path;
    std::vector<Node*> path;
    std::vector<Node*> seen;
    std::vector<std::vector<int>> all_edge_path;
    std::vector<int> edge_path;
    // source and sink
    Node *s = mos_net->gnd();
    Node *t = mos_net->out();
    path.push_back(s);

    // Search all path
    Search(s, t, mos_net ,&all_path, &path, &seen);

    // Turn all path into boolean expression
    std::cout << "----print all path (Node) ----\n";
    int i = 0;
    for (auto p: all_path) {
        std::cout << "path " << i << ": ";
        for (Node *n: p) {
            std::cout << n->idx << " ";
        }
        std::cout << std::endl;
        i++;
    }

    std::cout << "----print all path (Edge) ----\n";
    int cur_idx, next_idx;
    std::vector<std::vector<int>> temp;
    std::vector<std::vector<int>> current_all_path;
    for (auto p: all_path) {
        edge_path.clear();
        current_all_path.clear();
        for (int i = 0; i < p.size()-1; i++) {
            cur_idx = p[i]->idx;
            next_idx = p[i+1]->idx;
            for (Edge* e: p[i]->edges) {
                if ((e->n1->idx == cur_idx && e->n2->idx == next_idx) || (e->n2->idx == cur_idx && e->n1->idx == next_idx)) {
                    if (p[i]->idx == s->idx) {
                        for (int var : e->vars) {
                            edge_path.push_back(var);
                            current_all_path.push_back(edge_path);
                            edge_path.pop_back();
                        }
                    }
                    else {
                        temp = current_all_path;
                        for (auto e_p: temp) {
                            current_all_path.erase(current_all_path.begin());
                            for (int var : e->vars) {
                                e_p.push_back(var);
                                current_all_path.push_back(e_p);
                                e_p.pop_back();
                            }
                        }
                    }
                }
            }
        }
        for (auto p : current_all_path) {
            all_edge_path.push_back(p);
        }
    }

    int count = 0;
    for (auto e_p: all_edge_path) {
        std::cout << "path " << count << ": ";
        for (int e: e_p) {
            std::cout << e << " ";
        }
        std::cout << std::endl;
        count++;
    }


    // dump blif
    std::ofstream of;
    of.open(argv[3]);
    of << ".model " << argv[3] << std::endl;
    of << ".inputs ";
    for (Edge* e: mos_net->edges()) {
        for (int var: e->vars) {
            of << "n" << var << " ";
        }
    }
    of << std::endl;
    of << ".outputs o" << std::endl;
    if (isNmos == 1) {
        count = 0;
        for (auto e_p: all_edge_path) {
            of << ".names ";
            for (int e: e_p) {
                of << "n" << e << " ";
            }
            of << "nn" << count << std::endl;
            for (int i=0; i<e_p.size(); ++i) {
                of << "1";
            }
            of << " 1" << std::endl;
            count++;
        }
        count = 0;
        of << ".names ";
        for (auto e_p: all_edge_path) {
            of << "nn" << count << " ";
            count++;
        }
        of << "o" << std::endl;
        for (int i = 0; i < all_edge_path.size(); i++) {
            for (int j = 0; j < all_edge_path.size(); j++) {
                if (i == j) of << "1";
                else of << "-";
            }
            of << " 1" << std::endl;
        }
    }
    else {
        count = 0;
        for (auto e_p: all_edge_path) {
            of << ".names ";
            for (int e: e_p) {
                of << "n" << e << " ";
            }
            of << "nn" << count << std::endl;
            for (int i = 0; i < e_p.size(); i++) {
                for (int j = 0; j < e_p.size(); j++) {
                    if (i == j) of << "1";
                    else of << "-";
                }
                of << " 1" << std::endl;
            }
            count++;
        }
        count = 0;
        of << ".names ";
        for (auto e_p: all_edge_path) {
            of << "nn" << count << " ";
            count++;
        }
        of << "o" << std::endl;
        for (int i = 0; i < all_edge_path.size(); i++) {
            of << "1";
        }
        of << " 1" << std::endl;
    }


}

void Search(Node* x, Node * t, Graph* mos_net ,std::vector<std::vector<Node*>>* all_path, std::vector<Node*>* path, std::vector<Node*>* seen)
{
    if (x->idx == t->idx) {
        all_path->push_back(*path);
        return;
    }

    seen->clear();
    for (Node *n:*path) {
        seen->push_back(n);
    }

    if (Stuck(x, t, mos_net, seen)) return;
    for (Node *n: x->neighbors) {
        if (std::count(path->begin(), path->end(), n) == 0) {
            path->push_back(n);
            Search(n, t, mos_net , all_path, path, seen);
            path->pop_back();
        }
    }

}

bool Stuck(Node* x, Node * t, Graph* mos_net , std::vector<Node*>* seen)
{
    if (x->idx == t->idx) return false;

    for (Node* n:x->neighbors) {
        bool isSeen = false;
        for (Node *s:*seen) {
            if (s->idx == n->idx) {
                isSeen = true;
                break;
            }
        }
        if (!isSeen){
            seen->push_back(n);
            if (! Stuck(n, t, mos_net, seen)) return false;
        }
    }
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
