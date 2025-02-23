#include "MCTS.hpp"

//Constructor
MCTS::MCTS(Node<Portfolio<Real>>&& root, const int& NumberOfSimulations,
 const int& HorizontalScaling, const bool& finetuning, const int& FTiterations):
    root(move(root)), numberOfSimulations(NumberOfSimulations), treeWidth(Constants::treeWidth),
    HorizontalScaling(HorizontalScaling), finetuning(finetuning), FTiterations(FTiterations),
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

int MCTS::getFTiterations() const {
    return FTiterations;
}

int MCTS::getHorizontalExpansion() const {
    return HorizontalExpansion;
}

bool MCTS::getEarlyStopping() const {
    return EarlyStopping;
}

Real MCTS::getEarlyStoppingReturn() const {
    return early_stopping_return;
}

Real MCTS::getEarlyStoppingRisk() const {
    return early_stopping_risk;
}

int MCTS::getTreeCut() const {
    return TreeCut;
}

Real MCTS::getTreeCutReductionValue() const {
    return TreeCutReductionValue;
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

void MCTS::setFTiterations(const int& FTiterations) {
    this->FTiterations = FTiterations;
}

void MCTS::setHorizontalExpansion(const int& HorizontalExpansion) {
    this->HorizontalExpansion = HorizontalExpansion;
}

void MCTS::setEarlyStopping(const bool& EarlyStopping) {
    this->EarlyStopping = EarlyStopping;
}

void MCTS::setEarlyStoppingReturn(const Real& early_stopping_return) {
    this->early_stopping_return = early_stopping_return;
}

void MCTS::setEarlyStoppingRisk(const Real& early_stopping_risk) {
    this->early_stopping_risk = early_stopping_risk;
}

void MCTS::setTreeCut(const int& TreeCut) {
    this->TreeCut = TreeCut;
}

void MCTS::setTreeCutReductionValue(const Real& TreeCutReductionValue) {
    if(TreeCutReductionValue < 0 || TreeCutReductionValue > 1){
        throw invalid_argument("TreeCutReductionValue must be between 0 and 1");
    }
    this->TreeCutReductionValue = TreeCutReductionValue;
}

void MCTS::setRoot(Node<Portfolio<Real>>&& root) {
    this->root = move(root);
}

void MCTS::MCTSInformation() const {
    cout << "Number of simulations: " << numberOfSimulations << endl;
    cout << "Children per simulation: " << treeWidth*HorizontalScaling << endl;
    cout << "Horizontal scaling: " << HorizontalScaling << endl;
    cout << "Finetuning: " << finetuning << endl;
    cout << "Finetuning iterations: " << FTiterations << endl;
}

//Member functions
void MCTS::setBestChild(const Real& parentVisits) {
    int num_children = root.getChildren().size();

    Real bestUCB1 = -std::numeric_limits<Real>::infinity();
    int bestChildIndex = -1;

    // Finding the best child
    #pragma omp parallel
    {
        Real thread_bestUCB1 = -std::numeric_limits<Real>::infinity();
        int thread_bestChildIndex = -1;

        #pragma omp for
        for (int i = 0; i < num_children; i++) {
            Real childUCB1 = root.getChildren()[i]->computeUCB1(parentVisits);
            if (childUCB1 > thread_bestUCB1) {
                thread_bestUCB1 = childUCB1;
                thread_bestChildIndex = i; // Absolute index
            }
        }

        //Update the best child
        #pragma omp critical
        {
            if (thread_bestUCB1 > bestUCB1) {
                bestUCB1 = thread_bestUCB1;
                bestChildIndex = thread_bestChildIndex;
            }
        }
    }

    // Set the root to the best child
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
    //root.getPortfolio().PortfolioInformation();
}

void MCTS::expand() {
    int num_children = HorizontalExpansion; // Total number of children to create

    vector<shared_ptr<Node<Portfolio<Real>>>> all_children;
    all_children.reserve(num_children);

    // Create children in parallel
    #pragma omp parallel
    {
        vector<shared_ptr<Node<Portfolio<Real>>>> local_children;

        #pragma omp for
        for (int i = 0; i < num_children; i++) {
            Portfolio<Real> newPortfolio(root.getPortfolio());
            Node<Portfolio<Real>> childNode(move(newPortfolio));
            local_children.emplace_back(make_shared<Node<Portfolio<Real>>>(move(childNode)));
        }

        // Use a critical section to merge local_children into all_children
        #pragma omp critical
        {
            all_children.insert(all_children.end(), local_children.begin(), local_children.end());
        }
    }

    // Set children at the root
    root.setChildren(all_children);
}

/*
void MCTS::expand_finetuning(const int& rank, const int& size) {
    int num_children = HorizontalScaling; // Total number of children to create
    int chunk_size = num_children / size;   // Number of children per process
    int start_index = rank * chunk_size;
    int end_index = (rank == size - 1) ? num_children : start_index + chunk_size;

    vector<shared_ptr<Node<Portfolio<Real>>>> local_children;
    local_children.reserve(chunk_size);

    #pragma omp parallel
    {
        #pragma omp for
        for (int i = start_index; i < end_index; i++) {
            Portfolio<Real> newPortfolio(root.getPortfolio());
            Node<Portfolio<Real>> childNode(move(newPortfolio));

            #pragma omp critical
            {
                local_children.emplace_back(make_shared<Node<Portfolio<Real>>>(move(childNode)));
            }
        }
    }

    // Gather all children at the root process (rank 0)
    vector<shared_ptr<Node<Portfolio<Real>>>> all_children;
    if (rank == 0) {
        all_children.reserve(num_children);
    }

    // Gather the number of children from each process
    int local_size = local_children.size();
    vector<int> recv_counts(size);
    MPI_Gather(&local_size, 1, MPI_INT, recv_counts.data(), 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Gather the children
    if (rank == 0) {
        all_children.insert(all_children.end(), local_children.begin(), local_children.end());
        for (int i = 1; i < size; i++) {
            vector<shared_ptr<Node<Portfolio<Real>>>> temp_children(recv_counts[i]);
            MPI_Recv(temp_children.data(), recv_counts[i], MPI_UNSIGNED_LONG_LONG, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            all_children.insert(all_children.end(), temp_children.begin(), temp_children.end());
        }
    } else {
        MPI_Send(local_children.data(), local_size, MPI_UNSIGNED_LONG_LONG, 0, 0, MPI_COMM_WORLD);
    }

    // Set children at the root process
    if (rank == 0) {
        root.setChildren(all_children);
    }
}
*/
void MCTS::simulate(const int& remaining_simulations, const int& total_simulations) {
    int num_children = root.getChildren().size();
    vector<Real> adj_val = generate_adjustment_values(HorizontalScaling);

    // Simulate performance for all children in parallel
    #pragma omp parallel for
    for (int i = 0; i < num_children; i++) {
        int sector_index = i / treeWidth; // Determine sector for this child
        const auto& sector = Constraints::sectors[sector_index];

        if (TreeCut != 0) {
            Real reduction_factor = std::pow(TreeCutReductionValue, ((total_simulations - remaining_simulations) / (total_simulations / TreeCut)));
            root.getChildren()[i]->getPortfolio().Action1((adj_val[i % HorizontalScaling]) * reduction_factor, sector);
            root.getChildren()[i]->getPortfolio().Action2((adj_val[i % HorizontalScaling]) * reduction_factor, sector);
        } else {
            root.getChildren()[i]->getPortfolio().Action1(adj_val[i % HorizontalScaling], sector);
            root.getChildren()[i]->getPortfolio().Action2(adj_val[i % HorizontalScaling], sector);
        }

        Real reward = root.getChildren()[i]->getPortfolio().simulatePerformance();
        root.getChildren()[i]->setTotalReward(reward);
    }
}

/*
void MCTS::simulate_finetuning(const int& rank, const int& size) {
    int num_children = root.getChildren().size();
    int chunk_size = num_children / size;   // Number of children per process
    int start_index = rank * chunk_size;
    int end_index = (rank == size - 1) ? num_children : start_index + chunk_size;

    vector<Real> adj_val = generate_adjustment_values(HorizontalScaling);

    #pragma omp parallel
    {
        #pragma omp for
        for (int i = start_index; i < end_index; i++) {
            root.getChildren()[i]->getPortfolio().Action3(adj_val[i]);

            Real reward = root.getChildren()[i]->getPortfolio().simulatePerformance();
            root.getChildren()[i]->setTotalReward(reward);
        }
    }

    // Synchronize rewards across processes (if needed)
    MPI_Barrier(MPI_COMM_WORLD);
}
*/

void MCTS::backpropagate() {
    int num_children = root.getChildren().size();
    Real totalReward = 0.0;

    // Backpropagate rewards and update visits in parallel
    #pragma omp parallel for reduction(+:totalReward)
    for (int i = 0; i < num_children; i++) {
        Real reward = root.getChildren()[i]->getTotalReward();
        totalReward += reward;

        // Safely update the number of visits for each child
        #pragma omp critical
        {
            root.getChildren()[i]->setVisits(root.getChildren()[i]->getVisits() + 1);
        }
    }

    // Update root node
    root.setTotalReward(root.getTotalReward() + totalReward);
    root.setVisits(root.getVisits() + 1);
}


void MCTS::EarlyStoppingMCTS() {
    if(EarlyStopping){
        if (root.getPortfolio().computeAnnualizedReturn() >= early_stopping_return &&
            root.getPortfolio().computeVarianceRisk() <= early_stopping_risk) {
            cout << "--- Early stopping condition met ---" << endl;
            cout << "Exiting MCTS" << endl;
            cout << "Final S&P 500 Index" << endl;
            root.getPortfolio().PortfolioInformation();
            exit(0);
        }
    }
}

void MCTS::startMCTS() {
    
    cout << "Initial S&P 500 Index" << endl;
    root.getPortfolio().PortfolioInformation();
    
    int total_simulations = numberOfSimulations;
    // Normal MCTS Section
    while (numberOfSimulations--) {
        select();
        EarlyStoppingMCTS();
        expand();
        simulate(numberOfSimulations, total_simulations);
        backpropagate();
    }

    /*
    // Fine tuning Setion
    if (finetuning) {
        cout << "Adjusted S&P 500 Index [Before fine Tuning]" << endl;
        root.getPortfolio().PortfolioInformation();
        while (FTiterations--) {        
            select();
            EarlyStoppingMCTS();
            expand_finetuning();
            simulate_finetuning();
            backpropagate();
            }
    }
    */

    cout << "Final adjusted S&P 500 Index" << endl;
    root.getPortfolio().PortfolioInformation();
    root.getPortfolio().printWeights();
    
}