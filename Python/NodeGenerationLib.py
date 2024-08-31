import numpy as np
import matplotlib.pyplot as plt
import json
import os

def generate_nodes(num_nodes, grid_size):
    # Randomly generate node coordinates within the grid size
    nodes = np.random.rand(num_nodes, 2) * grid_size
    return nodes

def save_nodes_to_file(nodes, filename):
    # Save node coordinates to a JSON file
    with open(filename, 'w') as file:
        json.dump(nodes.tolist(), file)

def plot_nodes(nodes, filename):
    
    # Get the current working directory
    pwd = os.getcwd()
    
    # Create the full path for saving the image
    full_path = os.path.join(pwd, filename)
    
    # Plot nodes and their connections
    plt.figure(figsize=(8, 8))
    
    # Scatter plot for the nodes
    plt.scatter(nodes[:, 0], nodes[:, 1], c='blue', marker='o')

    # Draw lines connecting each pair of nodes
    num_nodes = len(nodes)
    for i in range(num_nodes):
        for j in range(i + 1, num_nodes):
            plt.plot([nodes[i, 0], nodes[j, 0]], [nodes[i, 1], nodes[j, 1]], 'k--', linewidth=0.5)
    
    plt.title('Node Grid with Connections')
    plt.xlabel('X Coordinate')
    plt.ylabel('Y Coordinate')
    plt.grid(True)
    plt.savefig(filename)
    plt.show()

def plot_nodes_unconnected(nodes, filename):
    
    # Get the current working directory
    pwd = os.getcwd()
    
    # Create the full path for saving the image
    full_path = os.path.join(pwd, filename)
    
    # Plot nodes without any connections
    plt.figure(figsize=(8, 8))
    
    # Scatter plot for the nodes
    plt.scatter(nodes[:, 0], nodes[:, 1], c='blue', marker='o')
    
    plt.title('Node Grid without Connections')
    plt.xlabel('X Coordinate')
    plt.ylabel('Y Coordinate')
    plt.grid(True)
    plt.savefig(filename)
    plt.show()