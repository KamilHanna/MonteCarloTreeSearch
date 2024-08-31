#ifndef NODE_HPP
#define NODE_HPP


#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include "Node.cpp"

using namespace std;

template <typename T>

class Node {

    private:

    // The id of a node
    const int id;
    // The coordinates of a node
    T x;
    T y;

    public :

    //Constructor
    Node(int id, T x, T y);

    //Getters
    int getId() const;
    T getX() const;
    T getY() const;

    //Setters
    void setX(T x);
    void setY(T y);


    //Member functions
    T distanceTo(const Node<T>& other) const;
    vector<Node<T>> readNodesFromFile(const string& filename);
    
    };

#endif //NODE_HPP

