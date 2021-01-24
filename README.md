# LSV-Project
tzyysang@gmail.com

geneyao0302@gmail.com

This is a repository of final project of the course **Logic Synthesis and Verification** at National Taiwan University.
It is cloned from https://github.com/NTU-ALComLab/LSV-PA

## Source Code
Source code are in **src/ext-lsv/**  
ABC_commnad interfaces are in **cmos.cpp**  
Major functions are in **graph.cpp**  

## How to comple
Please follow the standard make flow as in the LSV-PA, that is,
```
[LSV-project]$ make
```

## Run
Testcases and example runme script are in **lsv/project/**

An example runme script is in lsv/project
```
[LSV-project/lsv/project]$ source run.sh
```

It runs 1) planar netlist test, 2) nonplanar netlist test, 3) graph generation test

All implemented functions are incapsulated as ABC commands.

#### Generate new transistor netlist
```
#### example usage
abc> lsv_cmos_graph_gen2 test.nmos 6 0.6
```
test.nmos: output file name, generated netlist will be dumped into this file  
6: number of nodes in netlist, n >= 3  
0.6: vertex/edge ratio, 0 < r <= 1  

```
#### example outputs
Random graph generation 2 :
  # of vertices = 10
  ratio = 0.6
subdivision 1
subdivision 1
subdivision 2
subdivision 4
random edge 2, 1
subdivision 6
subdivision 7
random edge 3, 6
subdivision 4
random edge 0, 1
subdivision 10
random edge 3, 1
dump netlist to test.nmos
```

#### Dual network generation
```
abc> lsv_cmos_dual test.nmos test.output
```
test.nmos: input netlist
test.out: output netlist

#### Equivalence checking
```
abc> lsv_cmos2sop n test.nmos nmos.blif
abc> lsv_cmos2sop p pmos.output pmos.blif
abc> cec nmos.blif pmos.blif
```

The command lsv_cmos2sop convert our netlist to SOP/POS format with path enumaration algorithm.
Then cec is used to compare the two netlists.

