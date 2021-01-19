
import sys
import networkx as nx
import matplotlib.pyplot as plt

if len(sys.argv) >= 2:
    file_name = sys.argv[1]
else:
    print('Usage: plot.py netlist.nmos')
    exit()

G = nx.Graph()

file = open(file_name, 'r')

num_v = 0
num_e = 0
line_num = 0
for line in file:
    current = line.split(" ")
    if line_num == 0:
        num_v = int(current[0])
        num_e = int(current[1])
        G.add_nodes_from( range(num_v) )
    else:
        n1 = int(current[2])
        n2 = int(current[3])
        G.add_edge(n1,n2)
    line_num = line_num + 1

print( list(G.nodes) )
print( list(G.edges) )

plt.figure()
nx.draw(G, with_labels=True)
plt.show()
exit()
