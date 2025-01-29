#ifndef MCTS_HPP
#define MCTS_HPP

#include <Node.hpp>
#include <Utils.hpp>

#include <cmath>
#include <limits>
#include <memory>
#include <omp.h>
#include <mpi.h>

class MCTS {
private:

    int numberOfSimulations;
    int treeWidth;
    int HorizontalScaling;
    int HorizontalExpansion;
    bool finetuning;
    int EarlyStopping;
    Node<Portfolio<Real>> root;

public:

    //Constructor
    MCTS(Node<Portfolio<Real>>&& root, const int& NumberOfSimulations,
    const int& HorizontalScaling, const bool& finetuning, const int& EarlyStopping);

    //Getters
    int getNumberOfSimulations() const;
    int getTreeWidth() const;
    int getHorizontalScaling() const;
    bool getFinetuning() const;
    int getEarlyStopping() const;
    int getHorizontalExpansion() const;
    const Node<Portfolio<Real>> &getRoot() const;


    //Setters
    void setNumberOfSimulations(const int& numberOfSimulations);
    void setTreeWidth(const int& treeDepth);
    void setHorizontalScaling(const int& HorizontalScaling);
    void setFinetuning(const bool& finetuning);
    void setEarlyStopping(const int& EarlyStopping);
    void setHorizontalExpansion(const int& HorizontalExpansion);
    void setRoot(Node<Portfolio<Real>>&& root);


    
    //Member functions
    void setBestChild(const Real& parentVisits);
    void select();
    void expand();     
    void simulate();
    void backpropagate();
    void startMCTS();

    void MCTSInformation() const;
};

#endif // MCTS_HPP

