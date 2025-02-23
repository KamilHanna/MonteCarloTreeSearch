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
    int TreeCut;
    Real TreeCutReductionValue;

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
    int getTreeCut() const;
    Real getTreeCutReductionValue() const;
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
    void setTreeCut(const int& TreeCut);
    void setTreeCutReductionValue(const Real& TreeCutReductionValue);
    void setRoot(Node<Portfolio<Real>>&& root);


    
    //Member functions
    void setBestChild(const Real& parentVisits);
    void select();
    void expand();     
    void simulate(const int& remaining_simulations, const int& total_simulations);
    void backpropagate();
    void setBestChild_finetuning(const Real& parentVisits);
    void select_finetuning();
    void expand_finetuning();
    void simulate_finetuning();
    void EarlyStoppingMCTS();
    void startMCTS();

    void MCTSInformation() const;
};

#endif // MCTS_HPP

