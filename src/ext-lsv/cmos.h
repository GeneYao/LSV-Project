#ifndef LSV_CMOS_H
#define LSV_CMOS_H

#include "base/abc/abc.h"
#include "base/main/main.h"
#include "base/main/mainInt.h"
#include "ext-lsv/graph.h"

namespace lsv
{
    int CommandCmosDual(Abc_Frame_t* pAbc, int argc, char** argv);
    int CommandCmos2Sop(Abc_Frame_t* pAbc, int argc, char** argv);
    int CommandCmosGraphGen(Abc_Frame_t* pAbc, int argc, char** argv);  
    void Cmos2Sop(Graph* mos_net, bool isNmos , int argc, char** argv);
    void Search(Node* x, Node * t, Graph* mos_net ,std::vector<std::vector<Node*>>* all_path, std::vector<Node*>* path, std::vector<Node*>* seen);
    bool Stuck(Node* x, Node * t, Graph* mos_net , std::vector<Node*>* seen);
    void printPath(std::vector<Node*> path);
}

#endif
