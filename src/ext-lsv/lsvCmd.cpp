#include "base/abc/abc.h"
#include "base/main/main.h"
#include "base/main/mainInt.h"
#include "ext-lsv/cmos.h"

void init(Abc_Frame_t* pAbc)
{
    Cmd_CommandAdd(pAbc, "LSV", "lsv_cmos_dual", lsv::CommandCmosDual, 0);
    Cmd_CommandAdd(pAbc, "LSV", "lsv_cmos2sop", lsv::CommandCmos2Sop, 0);
}

void destroy(Abc_Frame_t* pAbc) {}

Abc_FrameInitializer_t frame_initializer = {init, destroy};

struct PackageRegistrationManager
{
    PackageRegistrationManager() { Abc_FrameAddInitializer(&frame_initializer); }
} lsvPackageRegistrationManager;
