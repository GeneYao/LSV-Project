echo "==== Planar Netlist Test ====\n"
../../abc -c "lsv_cmos_dual nsp3.nmos pmos.output"
../../abc -c "lsv_cmos2sop n nsp3.nmos nmos.blif"
../../abc -c "lsv_cmos2sop p pmos.output pmos.blif"
../../abc -c "cec nmos.blif pmos.blif"
echo "\n==== Non-planar Test ====\n"
../../abc -c "lsv_cmos_dual non1.nmos pmos.output"
../../abc -c "lsv_cmos2sop n non1.nmos nmos.blif"
../../abc -c "lsv_cmos2sop n pmos.output pmos.blif"
../../abc -c "cec nmos.blif pmos.blif"
echo "\n==== Graph Gen Test ====\n"
../../abc -c "lsv_cmos_graph_gen2 test.nmos 10 0.6"

#python3 plot.py test.output
