#include "MCTS.hpp"

//Constructor
MCTS::MCTS(Node<Portfolio<Real>>&& root, const int& NumberOfSimulations,
 const int& HorizontalScaling, const bool& finetuning, const int& EarlyStopping) : 
    root(move(root)), numberOfSimulations(NumberOfSimulations), treeWidth(11),
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
    cout << "Tree width: " << treeWidth << endl;
    cout << "Horizontal scaling: " << HorizontalScaling << endl;
    cout << "Finetuning: " << finetuning << endl;
    cout << "Early stopping: " << EarlyStopping << endl;
}

//Member functions

void MCTS::setBestChild(const Real& parentVisits) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int num_children = root.getChildren().size();
    int chunk_size = num_children / size; // Number of children each process handles
    int start_index = rank * chunk_size;
    int end_index = (rank == size - 1) ? num_children : start_index + chunk_size; // Handle remainder

    int local_bestChildIndex = -1;
    Real local_bestUCB1 = -std::numeric_limits<Real>::infinity();

    #pragma omp parallel for reduction(max:local_bestUCB1)
    for (int i = start_index; i < end_index; i++) {
        Real childUCB1 = root.getChildren()[i]->computeUCB1(parentVisits);
        
        if (childUCB1 > local_bestUCB1) {
            local_bestUCB1 = childUCB1;
            local_bestChildIndex = i; // relative to local chunk
        } 
    }

    // Create the MPI datatype for maxloc
    struct {
        Real val;
        int rank;
    } local_max, global_max;

    local_max.val = local_bestUCB1;
    local_max.rank = rank;

    // MPI Reduction (using the custom datatype)
    MPI_Allreduce(&local_max, &global_max, 1, MPI_FLOAT_INT, MPI_MAXLOC, MPI_COMM_WORLD); // or MPI_DOUBLE_INT if Real is double

    int global_bestChildIndex;
    if (global_max.rank != -1) { // Check if any process found a valid index
        global_bestChildIndex = (local_bestChildIndex == -1 ? -1 : local_bestChildIndex + start_index);
    } else {
        global_bestChildIndex = -1;
    }


    if (global_bestChildIndex != -1) {
        setRoot(move(*root.getChildren()[global_bestChildIndex]));
    }
}

void MCTS::select() {
    //Check if the root has children (if node is terminal)
    if(!root.getChildren().empty()) {
        //Select the child with the highest UCB1 value (maximizes the UCB1)
        Real parentVisits = std::log(root.getVisits());    
        setBestChild(parentVisits);
    }
    //root.getPortfolio().PortfolioInformation();
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
    //Perform Action 1 (Ajustments based on returns) & Action 2 (Adjustments based on risk)
    //Per each child for different sectors multiple times with different adjustment values.
    for (const auto& sector : Constraints::sectors) {
        for (int i = ScalabilityFactor; i < (treeWidth+ScalabilityFactor) && i < HorizontalExpansion; i++) {
            root.getChildren()[i]->getPortfolio().Action1(adj_val[(ScalabilityFactor-i)], sector);
            root.getChildren()[i]->getPortfolio().Action2(adj_val[(ScalabilityFactor-i)], sector);
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
}

void MCTS::startMCTS() {
    cout << "Initial fandomly initialized S&P 500 Index" << endl;
    root.getPortfolio().PortfolioInformation();

    while(numberOfSimulations--) {
        select();
        expand();
        simulate();
        backpropagate();
    }
    if(finetuning) {
        cout << "adjusted S&P 500 Index [Before fine Tuning]" << endl;
        root.getPortfolio().PortfolioInformation();
        //root.getPortfolio().Action3();
    }
        cout << "Final adjusted S&P 500 Index" << endl;
        root.getPortfolio().PortfolioInformation();
    
    
}


