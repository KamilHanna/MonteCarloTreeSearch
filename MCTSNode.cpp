#include "MCTSNode.hpp"

// Constructor using setters
MCTSNode::MCTSNode(const vector<Node<int>>& tour, MCTSNode* parent) {
    setTour(tour);
    setParent(parent);
    setVisits(0);
    setTotalDistance(0.0);
}

// Getters
const vector<Node<int>>& MCTSNode::getTour() const {
    return tour;
}

MCTSNode* MCTSNode::getParent() const {
    return parent;
}

const vector<MCTSNode*>& MCTSNode::getChildren() const {
    return children;
}

int MCTSNode::getVisits() const {
    return visits;
}

double MCTSNode::getTotalDistance() const {
    return totalDistance;
}

// Setters
void MCTSNode::setTour(const vector<Node<int>>& tour) {
    this->tour = tour;
}

void MCTSNode::setParent(MCTSNode* parent) {
    this->parent = parent;
}

void MCTSNode::addChild(MCTSNode* child) {
    children.push_back(child);
}

void MCTSNode::setVisits(int visits) {
    this->visits = visits;
}

void MCTSNode::setTotalDistance(double distance) {
    this->totalDistance = distance;
}

// Other member functions
void MCTSNode::incrementVisits() {
    setVisits(visits + 1);
}

void MCTSNode::addToTotalDistance(double distance) {
    setTotalDistance(totalDistance + distance);
}

double MCTSNode::calculateAverageDistance() const {
    return visits > 0 ? totalDistance / visits : 0.0;
}
