# MonteCarloTreeSearch
## Overview

This is an implementation of the Monte Carlo Tree Search Algorithm for Index optimization. 
the use case provided is for S&P 500 Index.

This is part of a project-work for Advanced Method for Scientific Computing course [@Polimi](https://www.polimi.it/)

### Author
Project developed by:
- [Kamil Hanna](https://github.com/kamilhanna)

### Problem description, Implementation decisions and Result analysis

Can be found into report - `documents/Report.pdf`

### File Structure

* `include` -  headers files where:
    * `Asset.hpp` - Defines the Asset object for storing asset information.
    * `Portfolio.hpp` - Defines the Portfolio object that manages assets.
    * `Constants.hpp` - Defines constant values used across the portfolio optimization process, such as the risk-free rate, risk aversion, and exploration constant for MCTS                              (UCB1). These constants help control parameters like risk preference and exploration/exploitation balance during portfolio optimization.
    * `Constraints.hpp` - Defines portfolio constraints, including sector weight limits, asset weight boundaries, and sector index boundaries for the S&P 500, to guide the                                 portfolio optimization process.
    * `Types.hpp` - Defines fundamental types  used in the project
    * `Utils.hpp` - Implements utility functions for file reading, user input handling, and other helper tasks in portfolio optimization.
    * `Node.hpp` - Defines the data structure for nodes in the MCTS algorithm
    * `MCTS.hpp` - Implements the core Monte Carlo Tree Search algorithm, includes the methods for node selection, expansion, simulation, and backpropagation.

* `src` - source files of headers implementation.
* `Python` - python libraries that generates the assets data.
* `Bibliography` - collection of reference papers.
* `documents` - implementation report.
* 

### Build dependencies

In order to build all targets the following software libraries must be installed:

* `OpenMP` - [Reference site](https://www.openmp.org/) - tested version: 11.2.x
* `MPI`    - [Reference site](https://www.open-mpi.org/) - tested version: 4.1.x

### How to build

In order to build the executable, from the root folder run the following commands:

```bash
$ mkdir build
$ cd build
$ cmake .. 
$ make
```
### How to run the program
```bash
$ ./MCTS_EXEC
```

## Runing the Algorithm

The MCTS algorithm for portfolio optimization is divided into two phases: **Phase I (Exploration)** and **Phase II (Finetuning)**. Below are the steps and inputs required to run the algorithm.

### Phase I: Exploration (Sector-Based Weights Adjustments (based on return and risk) + Merging Optimization

1. **Weight Initialization**  
   - Choose the method for initializing weights:
     - Enter `0` for **Equal Initialization** (based on sector constraints).
     - Enter `1` for **Market Cap-Based Initialization**.

2. **Number of Simulations**  
- Enter the desired number of simulations (the more, the better).  


3. **Horizontal Scaling Factor**  
- Horizontal scaling duplicates child nodes from the 11 sectors, applying weight adjustments with different values.  
- Enter the size of the adjustment values vector for horizontal scaling.


4. **TreeCut Optimization**  
- Enter a **TreeCut value**, which divides simulations and multiplies adjustments by a reduction factor.  
- Enter a **TreeCutReductionValue** between 0 and 1, which decreases as the tree goes deeper.


### Phase II: Finetuning (Overall weights adjustments based on returns correlation)
### [Not highly recommended]
1. **Enable Finetuning**  
- Enter `1` to enable finetuning, or `0` to skip this phase.

2. **Number of Finetuning Iterations**  
- Enter the number of iterations for finetuning (recommendation: no more than 3).

### Early Stopping Conditions

You can set thresholds for return and risk. If the portfolio meets these conditions, MCTS will stop early.

1. **Enable Early Stopping**  
- Enter `1` to enable early stopping, or `0` to disable.


2. **Set Early Stopping Thresholds**  
- Enter the early stopping return threshold in percentage.
- Enter the early stopping risk threshold in percentage.






