#include "Node.hpp"

//Constructor
template <typename state>
Node<state>::Node(int& id){
    setId(id);
    numberOfNodes++;
}

template<typename state>
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

// Setter for ID
template<typename state>
void Node<state>::setId(int& id) {
    this->id = id;
}

// Setter for visits
template<typename state>
void Node<state>::setVisits(int& visits) {
    this->visits = visits;
}

// Setter for total reward
template<typename state>
void Node<state>::setTotalReward(Real& totalReward) { 
    this->totalReward = totalReward;
 }

// Setter for parent
template<typename state>
void Node<state>::setParent(weak_ptr<Node<state>> parent) {
    this->parent = parent;
}

// Setter for children
template<typename state> 
void Node<state>::setChildren(vector<shared_ptr<Node<state>>> children) {
    this->children = children;
}

// Setter for untried actions
template<typename state> 
void Node<state>::setUntriedActions(vector<int>& untried_actions) {
    this->untried_actions = untried_actions;
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
    cout << "Number of untried actions: " << getUntriedActions().size() << endl;
}
