../../abc -c "lsv_cmos_dual nsp2.nmos pmos.output"
../../abc -c "lsv_cmos2sop n nsp2.nmos nmos.blif"
../../abc -c "lsv_cmos2sop p pmos.output pmos.blif"
../../abc -c "cec nmos.blif pmos.blif"
