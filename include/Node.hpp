#ifndef NODE_HPP
#define NODE_HPP

#include <iostream>
#include <vector>
#include <cmath>
#include <memory>
#include "Utils.hpp"

using namespace std;

/*
Some notes for the report :
weak_ptr since parent is not owned by the node..
shared_ptr since children are owned by the node... ( we don't want tor release...
(they also handle memory management automatically)
numberofnodes static (since it is for the class not for the object)
also getter for parent how it locks weak_ptr
*/

template <typename state>
class Node {

private:
    const int id;                                      // ID of a node
    int visits = 0;                                    // Number of times the node has been visited
    Real totalReward = 0.0;                            // The total reward of the node
    weak_ptr<Node<state>> parent;                      // The parent of the current node
    vector<shared_ptr<Node<state>>> children;          // The children of the current node
    vector<int> untried_actions;                       // The untried actions of the current node
    inline static int numberOfNodes = 0;               // Number of nodes created

    //shared_ptr<state> node_state; // Portfolio, represents current state of the node

public :
    //Constructor
    Node(int id);
    //Destructor
    ~Node();

    //Getters
    int getId() const;
    int getVisits() const;
    Real getTotalReward() const;
    shared_ptr<Node<state>> Node<state>::getParent() const;
    vector<shared_ptr<Node<state>>> getChildren() const;
    vector<int> getUntriedActions() const;
    static int getNumberOfNodes();

    //Setters
    void setId(int id);
    void setVisits(int visits);
    void setTotalReward(Real totalReward);
    void setParent(weak_ptr<Node<state>> parent);
    void setChildren(vector<shared_ptr<Node<state>>> children);
    void setUntriedActions(vector<int> untried_actions);

    //Member functions
    void NodeInformation() const;
    
    /*
    Add children.... Instead of setting children, we can add children to the vector of children

    also update visits..
    actions too.
    */
};

#endif //NODE_HPP

