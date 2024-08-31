from NodeGenerationLib import *
import os
import sys
    
# Number of nodes to generate, passed as a command-line argument (Called from c++)
num_nodes =int(sys.argv[1])

grid_size = 10*num_nodes
    
nodes = generate_nodes(num_nodes, grid_size)
    
save_nodes_to_file(nodes, 'nodes.json')

#Plotting the nodes without a connection.
plot_nodes_unconnected(nodes, 'nodes_unconnected.png')

#Plotting the nodes with a connection.
plot_nodes(nodes, 'nodes_connected.png')



