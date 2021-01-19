../../abc -c "lsv_cmos_dual nsp1.nmos pmos.output"
../../abc -c "lsv_cmos2sop n nsp1.nmos nmos.blif"
../../abc -c "lsv_cmos2sop p pmos.output pmos.blif"
../../abc -c "cec nmos.blif pmos.blif"
../../abc -c "lsv_cmos_dual non1.nmos pmos.output"
#echo "===== Graph Gen Test ====="
#../../abc -c "lsv_cmos_graph_gen 5 10 n test.nmos"
../../abc -c "lsv_cmos_graph_gen2 test.nmos 20 0.6"
../../abc -c "lsv_cmos_dual test.nmos test.output"
../../abc -c "lsv_cmos2sop n test.nmos test_n.blif"
../../abc -c "lsv_cmos2sop p test.output test_p.blif"
../../abc -c "cec test_n.blif test_p.blif"

#python3 plot.py test.nmos
