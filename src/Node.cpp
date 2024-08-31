#include "Node.hpp"


using namespace std;

template <typename T>
Node<T>::Node(int id, T x, T y){
    setID(id);
    setX(x);
    setY(y);
}

// Getters 

// Getter for ID
template<typename T>
int Node<T>::getId() const {
    return id;
}

// Getter for x coordinate
template<typename T>
T Node<T>::getX() const {
    return x;
}


// Getter for y coordinate
template<typename T>
T Node<T>::getY() const {
    return y;
}

//Setters

// Setter for x
template<typename T>
void Node<T>::setX(T x) {
    this->x = x;
}

// Setter for y
template<typename T>
void Node<T>::setY(T y) {
    this->y = y;
}

// Method to calculate the distance to another node (Distance between two nodes)
template<typename T>
T Node<T>::distanceTo(const Node<T>& other) const {
    return sqrt(pow(x - other.getX(), 2) + pow(y - other.getY(), 2));
}



// Method to read nodes from a file

template <typename T>
vector<Node<T>> Node<T>::readNodesFromFile(const string& filename) {
    vector<Node<T>> nodes;
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return nodes;
    }

    int id;
    T x, y;

    while (file >> id >> x >> y) {
        nodes.push_back(Node<T>(id, x, y));
    }

    file.close();
    return nodes;
}

/*
template<typename T>
vector<Node<T>>::readNodesFromFile(const std::string& filename){
    vector<Node<T>> nodes;
    ifstream file(filename);

    if (file.is_open()) {
        int id;
        T x, y;
        while (file >> id >> x >> y) {
            nodes.emplace_back(id, x, y);
        }
        file.close();
    } else {
        cerr << "Unable to open file: " << filename << endl;
    }

    return nodes;



/*
int main() {
    // Example usage
    std::vector<Node> nodes = Node::readNodesFromFile("nodes.txt");

    // Print the nodes
    for (const Node& node : nodes) {
        std::cout << "Node " << node.id << ": (" << node.x << ", " << node.y << ")\n";
    }

    return 0;
}

*/