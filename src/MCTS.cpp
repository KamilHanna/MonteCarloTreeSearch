#include "MCTS.hpp"
#include <random>

// Constructor
MCTS::MCTS(MCTSNode* root) : root(root) {}

// Run MCTS for a given number of iterations
MCTSNode* MCTS::run(int iterations) {
    for (int i = 0; i < iterations; ++i) {
        MCTSNode* leaf = select(root);
        MCTSNode* child = expand(leaf);
        double result = simulate(child);
        backpropagate(child, result);
    }
    // Return the best child of the root node
    return select(root);
}

// Selection: Traverse the tree to find the most promising leaf node
MCTSNode* MCTS::select(MCTSNode* node) {
    while (!node->getChildren().empty()) {
        double bestUCB1 = -std::numeric_limits<double>::infinity();
        MCTSNode* bestChild = nullptr;

        for (MCTSNode* child : node->getChildren()) {
            double ucb1 = calculateUCB1(child, node->getVisits());
            if (ucb1 > bestUCB1) {
                bestUCB1 = ucb1;
                bestChild = child;
            }
        }

        node = bestChild;
    }
    return node;
}

// Expansion: Expand the selected node by adding one or more children
MCTSNode* MCTS::expand(MCTSNode* node) {
    // Assume we add one child representing the next step in the tour
    vector<Node<int>> newTour = node->getTour();
    // Add the next city to the tour (this is simplified; in reality, you'd need to choose the next city carefully)
    // For simplicity, we'll assume a placeholder function getNextCity() which you would need to implement
    Node<int> nextCity = getNextCity(newTour);  // Implement this function based on your specific needs
    newTour.push_back(nextCity);

    MCTSNode* child = new MCTSNode(newTour, node);
    node->addChild(child);
    return child;
}

// Simulation: Perform a random rollout from the expanded node
double MCTS::simulate(MCTSNode* node) {
    vector<Node<int>> tour = node->getTour();
    // Simulate the completion of the tour randomly or with a heuristic
    while (tour.size() < TOTAL_NUMBER_OF_CITIES) {  // Replace with your total number of cities
        Node<int> nextCity = getRandomCity(tour);  // Implement this function to get a random city not yet visited
        tour.push_back(nextCity);
    }
    // Calculate the total distance of the tour
    return calculateTotalDistance(tour);  // Implement this function to calculate the distance of the tour
}

// Backpropagation: Update the nodes with the result of the simulation
void MCTS::backpropagate(MCTSNode* node, double result) {
    while (node != nullptr) {
        node->incrementVisits();
        node->addToTotalDistance(result);
        node = node->getParent();
    }
}

// Calculate UCB1 value
double MCTS::calculateUCB1(MCTSNode* node, int totalVisits) const {
    if (node->getVisits() == 0) {
        return std::numeric_limits<double>::infinity();  // Encourage exploration of unvisited nodes
    }
    double averageDistance = node->calculateAverageDistance();
    double explorationTerm = sqrt(2.0 * log(totalVisits) / node->getVisits());
    return -averageDistance + explorationTerm;  // We minimize distance, so we negate it
}
