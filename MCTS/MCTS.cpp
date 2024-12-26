#include "MCTS.hpp"

//Constructor
MCTS::MCTS(Node<Portfolio<Real>>&& root, const int& NumberOfSimulations, const int& treeDepth) : 
    root(move(root)), numberOfSimulations(NumberOfSimulations), treeDepth(treeDepth) {}

//Getters

int MCTS::getNumberOfSimulations() const {
    return numberOfSimulations;
}

int MCTS::getTreeDepth() const {
    return treeDepth;
}

const Node<Portfolio<Real>> &MCTS::getRoot() const {
    return root;
}

//Setters

void MCTS::setNumberOfSimulations(const int& numberOfSimulations) {
    this->numberOfSimulations = numberOfSimulations;
}

void MCTS::setTreeDepth(const int& treeDepth) {
    this->treeDepth = treeDepth;
}

void MCTS::setRoot(Node<Portfolio<Real>>&& root) {
    this->root = move(root);
}

//Member functions

void MCTS::startMCTS() {
    for (int i = 0; i < numberOfSimulations; i++) {

        /*
         MCTSNode* select(MCTSNode* node){

    
        while (!node->isTerminal()) {
            if (!node->isFullyExpanded()) {
                return expand(node);
            } else {
                node = node->getBestChild();
            }
        }
        return node;

        */
        select();
        expand();
        simulate();
        backpropagate();
    }
}

Real MCTS::computeUCB1() const {
    return 0;
}

void MCTS::select() {
    return;
}

void MCTS::expand() {
    return;
}

void MCTS::simulate() {
    return;
}

void MCTS::backpropagate() {
    return;
}

void MCTS::MCTSInformation() const {
    cout << "Number of simulations: " << numberOfSimulations << endl;
    cout << "Tree depth: " << treeDepth << endl;
    cout << "Root node: " << root.getId() << endl;
}

