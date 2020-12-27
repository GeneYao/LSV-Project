
#include "ext-lsv/cmos.h"
#include "ext-lsv/graph.h"
#include <iostream>

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

    nmos_net.embed();
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

    Graph mos_net(argv[2]);
    mos_net.dump();

    return 0;
}

}   /// end of namespace lsv
