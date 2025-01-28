#include "MCTS.hpp"

//Constructor
MCTS::MCTS(Node<Portfolio<Real>>&& root, const int& NumberOfSimulations, const int& treeWidth,
 const int& HorizontalScaling, const bool& finetuning, const int& EarlyStopping) : 
    root(move(root)), numberOfSimulations(NumberOfSimulations), treeWidth(treeWidth),
    HorizontalScaling(HorizontalScaling), finetuning(finetuning), EarlyStopping(EarlyStopping),
    HorizontalExpansion(treeWidth*HorizontalScaling){}

//Getters

int MCTS::getNumberOfSimulations() const {
    return numberOfSimulations;
}

int MCTS::getTreeWidth() const {
    return treeWidth;
}

int MCTS::getHorizontalScaling() const {
    return HorizontalScaling;
}

bool MCTS::getFinetuning() const {
    return finetuning;
}

int MCTS::getEarlyStopping() const {
    return EarlyStopping;
}

int MCTS::getHorizontalExpansion() const {
    return HorizontalExpansion;
}

const Node<Portfolio<Real>> &MCTS::getRoot() const {
    return root;
}


//Setters

void MCTS::setNumberOfSimulations(const int& numberOfSimulations) {
    this->numberOfSimulations = numberOfSimulations;
}

void MCTS::setTreeWidth(const int& treeDepth) {
    this->treeWidth = treeDepth;
}

void MCTS::setHorizontalScaling(const int& HorizontalScaling) {
    this->HorizontalScaling = HorizontalScaling;
}

void MCTS::setFinetuning(const bool& finetuning) {
    this->finetuning = finetuning;
}

void MCTS::setEarlyStopping(const int& EarlyStopping) {
    this->EarlyStopping = EarlyStopping;
}

void MCTS::setHorizontalExpansion(const int& HorizontalExpansion) {
    this->HorizontalExpansion = HorizontalExpansion;
}

void MCTS::setRoot(Node<Portfolio<Real>>&& root) {
    this->root = move(root);
}

void MCTS::MCTSInformation() const {
    cout << "Number of simulations: " << numberOfSimulations << endl;
    cout << "Tree depth: " << treeWidth << endl;
}

//Member functions

void MCTS::setBestChild(const Real& parentVisits) {
    int bestChildIndex = -1;
    Real bestUCB1 = -std::numeric_limits<Real>::infinity();

    for (int i = 0; i < root.getChildren().size(); i++) {
        Real childUCB1 = root.getChildren()[i]->computeUCB1(parentVisits);
        if (childUCB1 > bestUCB1) {
            bestUCB1 = childUCB1;
            bestChildIndex = i;
        }
    }

    if (bestChildIndex != -1) {
        setRoot(move(*root.getChildren()[bestChildIndex]));
    }
}

void MCTS::select() {
    //Check if the root has children (if node is terminal)
    if(!root.getChildren().empty()) {
        //Select the child with the highest UCB1 value (maximizes the UCB1)
        Real parentVisits = std::log(root.getVisits());    
        setBestChild(parentVisits);
    }
    root.getPortfolio().PortfolioInformation();
}

void MCTS::expand() {
    //Create children for the root node
    vector<shared_ptr<Node<Portfolio<Real>>>> children;
    children.reserve(HorizontalExpansion); //11 Sectors * the scaling factor

    //Copying portfolio of the root node to the children node.
    Portfolio<Real> newPortfolio(root.getPortfolio());

    for (int i = 0; i < HorizontalExpansion; i++) {
        Portfolio<Real> newPortfolio(root.getPortfolio());
        Node<Portfolio<Real>> childNode(move(newPortfolio));
        children.emplace_back(make_shared<Node<Portfolio<Real>>>(move(childNode)));
    }
    
    root.setChildren(children);
}

void MCTS::simulate() {

    //Generating the adjustment values
    vector<Real> adj_val = generate_adjustment_values(HorizontalScaling);

    int ScalabilityFactor = 0;
    //Perform Action 1 for each child for different sectors multiple times with different adjustment values.
    for (const auto& sector : Constraints::sectors) {
        for (int i = ScalabilityFactor; i < (treeWidth+ScalabilityFactor) && i < HorizontalExpansion; i++) {
            root.getChildren()[i]->getPortfolio().Action1(adj_val[(ScalabilityFactor-i)], sector);
            //children[i]->getPortfolio().Action1(adj_val[(ScalabilityFactor-i)], sector);
            //cout << "Sharpe Ratio :" <<children[i]->getPortfolio().computeSharpeRatio() << endl;
        }
        ScalabilityFactor += treeWidth;
    }

    for(int i = 0; i < root.getChildren().size(); i++) {
        root.getChildren()[i]->setTotalReward(root.getChildren()[i]->getPortfolio().simulatePerformance());
    }
}


void MCTS::backpropagate() {
    for(int i = 0; i < root.getChildren().size(); i++) {
        root.getChildren()[i]->setTotalReward(root.getChildren()[i]->getPortfolio().simulatePerformance());
        root.setTotalReward(root.getTotalReward() + root.getChildren()[i]->getTotalReward());
        root.getChildren()[i]->setVisits((root.getChildren()[i]->getVisits() + 1));
    }
    root.setVisits((root.getVisits() + 1));
    //cout << "visits root" << root.getVisits() << endl;
}

void MCTS::startMCTS() {
    while(numberOfSimulations--) {
        select();
        expand();
        simulate();
        backpropagate();
    }
}




