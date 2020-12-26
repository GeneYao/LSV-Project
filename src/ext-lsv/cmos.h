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
    void Cmos2Sop(Graph* mos_net, bool isNmos);
}

#endif
