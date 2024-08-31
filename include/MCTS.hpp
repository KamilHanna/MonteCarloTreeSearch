#ifndef MCTS_HPP
#define MCTS_HPP

#include "MCTSNode.hpp"
#include <cmath>
#include <limits>

class MCTS {
private:
    // The root node of the MCTS tree
    MCTSNode* root;  

public:
    MCTS(MCTSNode* root);
    MCTSNode* run(int iterations);
    
private:
    MCTSNode* select(MCTSNode* node);
    MCTSNode* expand(MCTSNode* node);
    double simulate(MCTSNode* node);
    void backpropagate(MCTSNode* node, double result);

    double calculateUCB1(MCTSNode* node, int totalVisits) const;
};

#endif // MCTS_HPP
