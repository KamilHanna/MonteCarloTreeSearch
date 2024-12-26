#include "Node.hpp"

//Constructor
template <typename state>
Node<state>::Node(int& id, state &&portfolio) :id(id), portfolio(move(portfolio)) {
    numberOfNodes++;
}

template <typename state>
Node<state>::~Node() {
    --numberOfNodes; // Decrement the static member in the destructor
}

// Getters 

// Getter for ID
template<typename state>
int Node<state>::getId() const {
    return id;
}

// Getter for visits
template<typename state>
int Node<state>::getVisits() const {
    return visits;
}

// Getter for portfolio
template<typename state>
state &Node<state>::getPortfolio() {
    return portfolio;
}

// Getter for total reward
template<typename state>
Real Node<state>::getTotalReward() const {
    return totalReward;
}

// Getter for parent
template<typename state>
shared_ptr<Node<state>> Node<state>::getParent() const {
    return parent.lock();
}

// Getter for children
template<typename state>
vector<shared_ptr<Node<state>>> Node<state>::getChildren() const {
    return children;
}

// Getter for number of nodes
template<typename state>
int Node<state>::getNumberOfNodes() {
    return numberOfNodes;
}

//Setters

// Setter for visits
template<typename state>
void Node<state>::setVisits(const int& visits) {
    this->visits = visits;
}

// Setter for total reward
template<typename state>
void Node<state>::setTotalReward(const Real& totalReward) { 
    this->totalReward = totalReward;
 }

// Setter for portfolio
template<typename state>
void Node<state>::setPortfolio(state&& portfolio) {
    this->portfolio = move(portfolio);
}

// Setter for parent
template<typename state>
void Node<state>::setParent(const weak_ptr<Node<state>> parent) {
    this->parent = parent;
}

// Setter for children
template<typename state> 
void Node<state>::setChildren(const vector<shared_ptr<Node<state>>> children) {
    this->children = children;
}

// Returns a node's information
template<typename state>
void Node<state>::NodeInformation() const {
    cout << "Node ID: " << getId() << endl;
    cout << "Number of visits: " << getVisits() << endl;
    cout << "Total reward: " << getTotalReward() << endl;

    auto parentPtr = getParent();
    if (parentPtr) {
        cout << "Parent ID: " << parentPtr->getId() << endl;
    } else {
        cout << "This node is the root of the tree." << endl;
    }

    cout << "Number of children: " << getChildren().size() << endl;
}

template class Node<Portfolio<Real>>;