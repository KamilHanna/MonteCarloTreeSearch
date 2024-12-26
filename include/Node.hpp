#ifndef NODE_HPP
#define NODE_HPP

#include <iostream>
#include <vector>
#include <cmath>
#include <memory>
#include "Types.hpp"
#include "Portfolio.hpp"

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
    state portfolio;                                   // Portfolio of the node
    int visits = 0;                                    // Number of times the node has been visited
    Real totalReward = 0.0;                            // The total reward of the node
    weak_ptr<Node<state>> parent;                      // The parent of the current node
    vector<shared_ptr<Node<state>>> children;          // The children of the current node
    inline static int numberOfNodes = 0;               // Number of nodes created

public :
    //Constructor
    Node(int& id, state &&portfolio);
    //Destructor
    ~Node();

    //Getters
    int getId() const;
    int getVisits() const;
    state &getPortfolio();
    Real getTotalReward() const;
    shared_ptr<Node<state>> getParent() const;
    vector<shared_ptr<Node<state>>> getChildren() const;
    static int getNumberOfNodes();

    //Setters
    void setVisits(const int& visits);
    void setTotalReward(const Real& totalReward);
    void setPortfolio(state&& portfolio);
    void setParent(const weak_ptr<Node<state>> parent);
    void setChildren(const vector<shared_ptr<Node<state>>> children);

    //Member functions
    void NodeInformation() const; 
};

#endif //NODE_HPP

