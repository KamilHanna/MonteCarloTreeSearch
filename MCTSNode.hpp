#ifndef MCTS_NODE_HPP
#define MCTS_NODE_HPP

#include <vector>
#include "Node.hpp"

using namespace std;

class MCTSNode {
private:
    vector<Node<int>> tour;        // The current partial tour
    MCTSNode* parent;              // The parent of this node
    vector<MCTSNode*> children;    // The children of this node
    int visits;                    // Number of visits
    double totalDistance;          // Total distance (for averaging)

public:
    // Constructor
    MCTSNode(const vector<Node<int>>& tour, MCTSNode* parent = nullptr);

    // Getters
    const vector<Node<int>>& getTour() const;
    MCTSNode* getParent() const;
    const vector<MCTSNode*>& getChildren() const;
    int getVisits() const;
    double getTotalDistance() const;

    // Setters
    void setTour(const vector<Node<int>>& tour);
    void setParent(MCTSNode* parent);
    void addChild(MCTSNode* child);
    void setVisits(int visits);
    void setTotalDistance(double distance);

    // Member functions
    void incrementVisits();
    void addToTotalDistance(double distance);
    double calculateAverageDistance() const;
};

#endif // MCTS_NODE_HPP
