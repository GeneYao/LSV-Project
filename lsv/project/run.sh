../../abc -c "lsv_cmos_dual sp1.nmos sp1.output"
../../abc -c "lsv_cmos2sop n sp1.nmos sp1.nmos.blif"
../../abc -c "lsv_cmos2sop p sp1.pmos sp1.pmos.blif"
../../abc -c "cec sp1.nmos.blif sp1.pmos.blif"
../../abc -c "lsv_cmos2sop n nsp1.nmos nsp1.nmos.blif"
../../abc -c "lsv_cmos2sop p nsp1.pmos nsp1.pmos.blif"
../../abc -c "cec nsp1.nmos.blif nsp1.pmos.blif"
