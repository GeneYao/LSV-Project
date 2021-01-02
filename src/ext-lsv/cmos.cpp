
#include "ext-lsv/cmos.h"
#include "ext-lsv/graph.h"
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <numeric>

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

static void HelpCommandCmosGraphGen()
{
    Abc_Print(-2, "usage: lsv_cmos_graph_gen [vertex_num] [max_degree] output_file\n");
    Abc_Print(-2, "\t       generate ramdom graph for cmos netlist\n");
    Abc_Print(-2, "\t-h    : print the command usage\n");
}

int CommandCmosGraphGen(Abc_Frame_t* pAbc, int argc, char** argv)
{
    int c;
    Extra_UtilGetoptReset();
    while ((c = Extra_UtilGetopt(argc, argv, "h")) != EOF) {
    switch (c) {
        case 'h':
            HelpCommandCmosGraphGen();
            return 1;
        default:
            HelpCommandCmosGraphGen();
            return 1;
    }
    }

    int d, n;
    std::cout<<"Random graph generation: ";
    n= strtol(argv[1], NULL, 10);
    std::cout<<"\nThe graph has "<<n<<" vertices";
    d = strtol(argv[2], NULL, 10);
    std::cout<<"\nand each vertex has maximum " << d <<" degrees.\n";
    GenRandomGraphs(n, d, argc, argv);

    return 0;
}

void GenRandomGraphs(int vertex_num, int max_degree, int argc, char** argv)
{
    std::vector<std::vector<int>> adj_mat(vertex_num, std::vector<int>(vertex_num));
    std::vector<int> vertex_degree(vertex_num);
    int d = 0, n = vertex_num;
    for (int i = 0; i < n; i++) {
        d = max_degree; // 2;
        vertex_degree[i] = rand()%d;
        if (vertex_degree[i] % 2 == 1) {
            vertex_degree[i] += 1;
        }
        if (vertex_degree[i] == 0) {
            vertex_degree[i] = 2;
        }
    }
    std::cout << "vertex_degree: "; 
    for (int i = 0; i < n; i++) {
        std::cout << vertex_degree[i] << " ";
    }
    std::cout << std::endl;
    for(int i = 0; i<n; i++) {
        for(int j = 0; j < n; j++){
            adj_mat[i][j] = 0;
        }
    }
    while (std::accumulate(vertex_degree.begin(), vertex_degree.end(), 0) > 0) {
    //while (vertex_degree[i] > 0 && vertex_degree[j] > 0) {
    //for (int k = 0; k < max_degree / vertex_num + 1; k++) {
        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j < n; j++) {
                if (rand() % 100 > 50 && vertex_degree[i] > 0 && vertex_degree[j] > 0) {
                    vertex_degree[i]--; vertex_degree[j]--;
                    adj_mat[i][j] += 1; adj_mat[j][i] += 1;
                }
            }
        }
    }
    std::cout << std::endl << std::setw(3) << " ";
    for (int i = 0; i < n; i++) {
        std::cout << std::setw(3) << "(" << i << ")";
    }
    std::cout << std::endl << std::endl;
    for (int i = 0; i < n; i++) {
        std::cout << std::setw(4) << "(" << i << ")";
        for (int j = 0; j < n; j++) {
            std::cout << std::setw(5) << adj_mat[i][j];
        }
        std::cout << std::endl;
    }
    /*
    int i, j, edge[edge_num][2], count;
    i = 0;
    //Assign random values to the number of vertex and edges of the graph, Using rand().
    while(i < edge_num) {
        edge[i][0] = rand()%vertex_num+1;
        edge[i][1] = rand()%vertex_num+1;
        //Print the connections of each vertex, irrespective of the direction.
        if(edge[i][0] == edge[i][1])
            continue;
        else {
            for(j = 0; j < i; j++) {
                if((edge[i][0] == edge[j][0] &&
                edge[i][1] == edge[j][1]) || (edge[i][0] == edge[j][1] &&
                edge[i][1] == edge[j][0]))
                i--;
            }
        } 
        i++;
    }
    std::cout<<"\nThe generated random graph is: ";
    for(i = 0; i < vertex_num; i++) {
        count = 0;
        std::cout<<"\n\t"<<i+1<<"-> { ";
        for(j = 0; j < edge_num; j++) {
            if(edge[j][0] == i+1)
            {
                std::cout<<edge[j][1]<<" ";
                count++;
            } 
            else if(edge[j][1] == i+1)
            {
                std::cout<<edge[j][0]<<" ";
                count++;
            } 
            else if(j== edge_num-1 && count == 0)
                std::cout<<"Isolated Vertex!"; //Print “Isolated vertex” for the vertex having no degree.
      }
      std::cout<<" }\n";
   }
   */ 
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
