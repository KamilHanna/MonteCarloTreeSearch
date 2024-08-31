#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
#include <algorithm>
#include <random>
#include <memory>

using namespace std;


struct TSPState {
    vector<int> route;
    vector<int> unvisitedCities;
    double totalDistance = 0.0;

    TSPState(int numCities) {
        for (int i = 0; i < numCities; ++i) {
            unvisitedCities.push_back(i);
        }
    }
};

struct MCTSNode {
    TSPState state;
    int visits = 0;
    double totalDistance = 0.0;
    vector<shared_ptr<MCTSNode>> children;
    MCTSNode* parent = nullptr;

    MCTSNode(const TSPState& state, MCTSNode* parent = nullptr) : state(state), parent(parent) {}

    double ucb1(double exploration_constant) const {
        if (visits == 0) {
            return numeric_limits<double>::infinity();
        }
        return (totalDistance / visits) + exploration_constant * sqrt(log(parent->visits) / visits);
    }
};

// Function to expand the node by generating children nodes
void expandNode(MCTSNode* node, const vector<City>& cities) {
    for (int city : node->state.unvisitedCities) {
        TSPState newState = node->state;
        if (!newState.route.empty()) {
            newState.totalDistance += distance(cities[newState.route.back()], cities[city]);
        }
        newState.route.push_back(city);
        newState.unvisitedCities.erase(remove(newState.unvisitedCities.begin(), newState.unvisitedCities.end(), city), newState.unvisitedCities.end());
        node->children.push_back(make_shared<MCTSNode>(newState, node));
    }
}

// Function to perform a random simulation (play-out)
double simulate(const TSPState& state, const vector<City>& cities) {
    TSPState simulationState = state;

    // Shuffle remaining cities to simulate random route completion
    random_device rd;
    mt19937 g(rd());
    shuffle(simulationState.unvisitedCities.begin(), simulationState.unvisitedCities.end(), g);

    // Complete the route
    for (int city : simulationState.unvisitedCities) {
        if (!simulationState.route.empty()) {
            simulationState.totalDistance += distance(cities[simulationState.route.back()], cities[city]);
        }
        simulationState.route.push_back(city);
    }

    // Add distance from last city back to the starting city
    simulationState.totalDistance += distance(cities[simulationState.route.back()], cities[simulationState.route.front()]);

    return simulationState.totalDistance;
}

// MCTS algorithm for TSP
TSPState MCTS(const vector<City>& cities, int numSimulations) {
    int numCities = cities.size();
    TSPState initialState(numCities);
    auto root = make_shared<MCTSNode>(initialState);

    for (int i = 0; i < numSimulations; ++i) {
        auto node = root.get();

        // Selection
        while (!node->children.empty()) {
            node = max_element(node->children.begin(), node->children.end(),
                                [](const shared_ptr<MCTSNode>& a, const shared_ptr<MCTSNode>& b) {
                                    return a->ucb1(1.41) < b->ucb1(1.41);
                                })->get();
                                [](const shared_ptr<MCTSNode>& a, const shared_ptr<MCTSNode>& b) {
                                    return a->ucb1(1.41) < b->ucb1(1.41);
                                ).get();
        }

        // Expansion
        if (!node->state.unvisitedCities.empty()) {
            expandNode(node, cities);
        }

        // Simulation
        double simulatedDistance = simulate(node->state, cities);

        // Backpropagation
        while (node) {
            node->visits++;
            node->totalDistance += simulatedDistance;
            node = node->parent;
        }
    }

    // Best route selection
    return *max_element(root->children.begin(), root->children.end(),
                        [](const shared_ptr<MCTSNode>& a, const shared_ptr<MCTSNode>& b) {
                            return a->totalDistance / a->visits > b->totalDistance / b->visits;
                        }).state;
}

int main() {
    vector<City> cities = {{0, 0}, {1, 5}, {4, 1}, {7, 6}, {9, 4}}; // Example cities
    int numSimulations = 1000;

    TSPState bestRoute = MCTS(cities, numSimulations);

    cout << "Best route found: ";
    for (int city : bestRoute.route) {
        cout << city << " ";
    }
    cout << endl;
    cout << "Total distance: " << bestRoute.totalDistance << endl;

    return 0;
}
