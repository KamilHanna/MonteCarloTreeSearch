#include "Node.hpp"

//Constructor
template <typename state>
Node<state>::Node(state &&portfolio) : portfolio(move(portfolio)) {
    numberOfNodes++;
}

template <typename state>
Node<state>::~Node() {
    --numberOfNodes; // Decrement the static member in the destructor
}

// Getters 


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


// Setter for children
template<typename state> 
void Node<state>::setChildren(const vector<shared_ptr<Node<state>>> children) {
    this->children = children;
}

template<typename state>
Real Node<state>::computeUCB1(const Real& parentVisits) const {

    //If the node has never been visited, return infinity to encourage exploration
    if(visits == 0) {
        //we are not using this since we are not using the UCB1 formula for the root node.
        return std::numeric_limits<Real>::infinity();
    }

    //UCB1 formula: exploitation + exploration
    Real exploitation = totalReward / visits; //Average reward
    Real exploration = Constants::explorationConstant * std::sqrt(parentVisits / visits);

    return exploitation + exploration;
}

// Returns a node's information
template<typename state>
void Node<state>::NodeInformation() const {
    cout << "Number of visits: " << getVisits() << endl;
    cout << "Total reward: " << getTotalReward() << endl;
    cout << "Number of children: " << getChildren().size() << endl;
}

template class Node<Portfolio<Real>>;