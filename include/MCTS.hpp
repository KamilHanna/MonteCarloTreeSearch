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
    int FTiterations;
    Node<Portfolio<Real>> root;
    bool EarlyStopping;
    Real early_stopping_return;
    Real early_stopping_risk;

public:

    //Constructor
    MCTS(Node<Portfolio<Real>>&& root, const int& NumberOfSimulations,
    const int& HorizontalScaling, const bool& finetuning, const int& FTiterations);

    //Getters
    int getNumberOfSimulations() const;
    int getTreeWidth() const;
    int getHorizontalScaling() const;
    bool getFinetuning() const;
    int getFTiterations() const;
    int getHorizontalExpansion() const;
    bool getEarlyStopping() const;
    Real getEarlyStoppingReturn() const;
    Real getEarlyStoppingRisk() const;
    const Node<Portfolio<Real>> &getRoot() const;


    //Setters
    void setNumberOfSimulations(const int& numberOfSimulations);
    void setTreeWidth(const int& treeDepth);
    void setHorizontalScaling(const int& HorizontalScaling);
    void setFinetuning(const bool& finetuning);
    void setFTiterations(const int& FTiterations);
    void setHorizontalExpansion(const int& HorizontalExpansion);
    void setEarlyStopping(const bool& EarlyStopping);
    void setEarlyStoppingReturn(const Real& early_stopping_return);
    void setEarlyStoppingRisk(const Real& early_stopping_risk);
    void setRoot(Node<Portfolio<Real>>&& root);


    
    //Member functions
    void setBestChild(const Real& parentVisits,const int& rank, const int& size);
    void select(const int& rank, const int& size);
    void expand(const int& rank, const int& size);     
    void simulate(const int& rank, const int& size);
    void backpropagate(const int& rank, const int& size);
    void expand_finetuning(const int& rank, const int& size);
    void simulate_finetuning(const int& rank, const int& size);
    void EarlyStoppingMCTS();
    void startMCTS();

    void MCTSInformation() const;
};

#endif // MCTS_HPP

