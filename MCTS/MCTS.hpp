#ifndef MCTS_HPP
#define MCTS_HPP

#include <Node.hpp>

#include <cmath>
#include <limits>

class MCTS {
private:

    int numberOfSimulations;
    int treeDepth;
    Node<Portfolio<Real>> root;

public:

    //Constructor
    MCTS(Node<Portfolio<Real>>&& root, const int& NumberOfSimulations, const int& treeDepth);

    //Getters
    int getNumberOfSimulations() const;
    int getTreeDepth() const;
    const Node<Portfolio<Real>> &getRoot() const;

    //Setters
    void setNumberOfSimulations(const int& numberOfSimulations);
    void setTreeDepth(const int& treeDepth);
    void setRoot(Node<Portfolio<Real>>&& root);

    //Member functions
    void startMCTS();
    Real computeUCB1() const;
    void select();
    void expand();
    void simulate();
    void backpropagate();

    void MCTSInformation() const;
};

#endif // MCTS_HPP

