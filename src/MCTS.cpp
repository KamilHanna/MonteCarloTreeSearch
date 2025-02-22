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
void MCTS::setBestChild(const Real& parentVisits, const int& rank, const int& size) {
    int num_children = root.getChildren().size();
    int chunk_size = num_children / size;   // Number of children per process
    int start_index = rank * chunk_size;
    int end_index = (rank == size - 1) ? num_children : start_index + chunk_size;

    Real local_bestUCB1 = -std::numeric_limits<Real>::infinity();
    int local_bestChildIndex = -1;

    // Find the best child in the local chunk using OpenMP
    #pragma omp parallel
    {

        #pragma omp for reduction(max:local_bestUCB1)
        for (int i = start_index; i < end_index; i++) {
            Real childUCB1 = root.getChildren()[i]->computeUCB1(parentVisits);
            if (childUCB1 > local_bestUCB1) {
                local_bestUCB1 = childUCB1;
                local_bestChildIndex = i; // Absolute index
            }
        }
    }

    // Gather all local best UCB1 values and indices
    std::vector<Real> all_bestUCB1(size);
    std::vector<int> all_bestIndices(size);

    MPI_Gather(&local_bestUCB1, 1, MPI_DOUBLE, all_bestUCB1.data(), 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Gather(&local_bestChildIndex, 1, MPI_INT, all_bestIndices.data(), 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Rank 0 determines the global best child
    if (rank == 0) {
        Real global_bestUCB1 = -std::numeric_limits<Real>::infinity();
        int global_bestChildIndex = -1;

        for (int i = 0; i < size; i++) {
            if (all_bestUCB1[i] > global_bestUCB1) {
                global_bestUCB1 = all_bestUCB1[i];
                global_bestChildIndex = all_bestIndices[i];
            }
        }

        // Set the root to the best child
        if (global_bestChildIndex != -1) {
            setRoot(move(*root.getChildren()[global_bestChildIndex]));
        }
    }
}

void MCTS::select(const int& rank, const int& size) {
    //Check if the root has children (if node is terminal)
    if(!root.getChildren().empty()) {
        //Select the child with the highest UCB1 value (maximizes the UCB1)
        Real parentVisits = std::log(root.getVisits());    
        setBestChild(parentVisits, rank, size);
    }
    //root.getPortfolio().PortfolioInformation();
}

void MCTS::expand(const int& rank, const int& size) {
    int num_children = HorizontalExpansion; // Total number of children to create
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

void MCTS::simulate(const int& rank, const int& size) {
    int num_children = root.getChildren().size();
    int chunk_size = num_children / size;   // Number of children per process
    int start_index = rank * chunk_size;
    int end_index = (rank == size - 1) ? num_children : start_index + chunk_size;

    vector<Real> adj_val = generate_adjustment_values(HorizontalScaling);

    #pragma omp parallel
    {

        #pragma omp for
        for (int i = start_index; i < end_index; i++) {
            int sector_index = i / treeWidth; // Determine sector for this child
            const auto& sector = Constraints::sectors[sector_index];

            root.getChildren()[i]->getPortfolio().Action1(adj_val[i % HorizontalScaling], sector);
            root.getChildren()[i]->getPortfolio().Action2(adj_val[i % HorizontalScaling], sector);

            Real reward = root.getChildren()[i]->getPortfolio().simulatePerformance();
            root.getChildren()[i]->setTotalReward(reward);
        }
    }

    // Synchronize rewards across processes (if needed)
    MPI_Barrier(MPI_COMM_WORLD);
}


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

void MCTS::backpropagate(const int& rank, const int& size) {

    int num_children = root.getChildren().size();
    int chunk_size = num_children / size;   // Number of children per process
    int start_index = rank * chunk_size;
    int end_index = (rank == size - 1) ? num_children : start_index + chunk_size;

    Real local_totalReward = 0.0;

    #pragma omp parallel for reduction(+:local_totalReward)
    for (int i = start_index; i < end_index; i++) {
        Real reward = root.getChildren()[i]->getTotalReward();

        #pragma omp atomic
        local_totalReward += reward;

        #pragma omp critical
        root.getChildren()[i]->setVisits(root.getChildren()[i]->getVisits() + 1);
        
    }

    // Aggregate total reward across processes
    Real global_totalReward;
    MPI_Allreduce(&local_totalReward, &global_totalReward, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

    // Update root node
    if (rank == 0) {
        root.setTotalReward(root.getTotalReward() + global_totalReward);
        root.setVisits(root.getVisits() + 1);
    }
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
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        cout << "Initial S&P 500 Index" << endl;
        root.getPortfolio().PortfolioInformation();
    }

    // nbr of simulations per process
    int local_simulations = numberOfSimulations / size; 
    int remainder = numberOfSimulations % size;

    if (rank < remainder) local_simulations++;

    while (local_simulations--) {
        select(rank, size);
        EarlyStoppingMCTS();
        expand(rank, size);
        simulate(rank, size);
        backpropagate(rank, size);
    }

    if (rank == 0 && finetuning) {
        cout << "Adjusted S&P 500 Index [Before fine Tuning]" << endl;
        root.getPortfolio().PortfolioInformation();
    }

    if(finetuning){
        while (FTiterations--) {        
        select(rank, size);
        EarlyStoppingMCTS();
        expand_finetuning(rank, size);
        simulate_finetuning(rank, size);
        backpropagate(rank, size);
        }
    }

    if (rank == 0) {
        cout << "Final adjusted S&P 500 Index" << endl;
        root.getPortfolio().PortfolioInformation();
        root.getPortfolio().printWeights();
    }
}

