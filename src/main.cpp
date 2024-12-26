#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <vector>
#include <string>
#include <chrono>

#include "Node.hpp"
#include "Portfolio.hpp"
#include "Asset.hpp"
#include "Utils.hpp"

using namespace std;
/*
Real testSolver(Real deltaT, index_t dim) {
    // define T & deltaT  & Re
    const Real T = 0.1;
    const Real Re = 4000;
    // Define physical size of the problem (just for simplicity)
    const Real phy_dim = 1.0;

    logger.openSection("Running the TestSolver");
    logger.printValue(5, "Final T", T);
    logger.printValue(5, "dT", deltaT);
    logger.printValue(5, "Re num", Re);
    logger.printValue(5, "Phy dim", to_string(phy_dim) + " x " + to_string(phy_dim) + " x " + to_string(phy_dim));

    // Define number of nodes for each axis
    const index_t nx = dim;
    const index_t ny = dim;
    const index_t nz = dim;

    // Define physical size of the problem for each axis
    const Real sx = phy_dim / real(nx);
    const Real sy = phy_dim / real(ny);
    const Real sz = phy_dim / real(nz);

    // Define spacing
    Vector spacing = {sx, sy, sz};

    // Define nodes
    std::array<index_t, 3> nodes = {nx, ny, nz};

    // define the mesh:
    Grid<STANDARD> model(nodes, spacing, 1);
    logger.printTitle("Grid created");
    logger.printValue(5, "nodes", to_string(model.nx) + " x " + to_string(model.ny) + " x " + to_string(model.nz));
    logger.printValue(5, "ghosts", model.gp);

    // initialize the mesh
    // Define initial velocity function
    auto initialVel = [](Real x, Real y, Real z) -> Vector {
        return {ExactSolution<STANDARD>::u(x, y, z, 0), ExactSolution<STANDARD>::v(x, y, z, 0), ExactSolution<STANDARD>::w(x, y, z, 0)};
    };

    // Define initial pressure function
    // For the moment it does not work so do not care about it
    auto initialPres = [](Real x, Real y, Real z) -> Real {
        return x + y + z;
    };

    chrono_sect(initT,
                model.initGrid(initialVel, initialPres);
    );
    logger.printTitle("Grid initialized", initT);

    // Define test boundary condition
    Condition<STANDARD>::Mapper testBCMapper = [](Grid<STANDARD> &grid,
                                                  const TFunction &fun, Real currentTime) {

        const Real sdx = grid.sdx;
        const Real sdy = grid.sdy;
        const Real sdz = grid.sdz;

        using Sol = ExactSolution<STANDARD>;

        // apply on face with x constant
        for (index_t j = 0; j < grid.ny; j++)
            for (index_t k = 0; k < grid.nz; k++) {
                Real y = real(j) * grid.dy;
                Real z = real(k) * grid.dz;

                Real x = 0;
                // On x = 0 for ghost point we have exact for U, other approximate
                grid(U, -1, j, k) = Sol::u(x, y + sdy, z + sdz, currentTime);
                grid(V, -1, j, k) = 2 * Sol::v(x, y + grid.dy, z + sdz, currentTime)
                                    - grid(V, 0, j, k);
                grid(W, -1, j, k) = 2 * Sol::w(x, y + sdy, z + grid.dz, currentTime)
                                    - grid(W, 0, j, k);


                x = real(grid.nx) * grid.dx;
                // On x = phy_dim for domain point we have exact for U
                grid(U, grid.nx - 1, j, k) = Sol::u(x, y + sdy, z + sdz, currentTime);
                // For ghost point we have useless U, other approximate
                grid(U, grid.nx, j, k) = 0;
                grid(V, grid.nx, j, k) = 2 * Sol::v(x, y + grid.dy, z + sdz, currentTime)
                                         - grid(V, grid.nx - 1, j, k);
                grid(W, grid.nx, j, k) = 2 * Sol::w(x, y + sdy, z + grid.dz, currentTime)
                                         - grid(W, grid.nx - 1, j, k);
            }

        // apply on face with y constant
        for (index_t i = 0; i < grid.nx; i++)
            for (index_t k = 0; k < grid.nz; k++) {
                Real x = real(i) * grid.dx;
                Real z = real(k) * grid.dz;

                Real y = 0;
                // On y = 0 for ghost point we hae exact for V, other approximate
                grid(U, i, -1, k) = 2 * Sol::u(x + grid.dx, y, z + sdz, currentTime)
                                    - grid(U, i, 0, k);
                grid(V, i, -1, k) = Sol::v(x + sdx, y, z + sdz, currentTime);
                grid(W, i, -1, k) = 2 * Sol::w(x + sdx, y, z + grid.dz, currentTime)
                                    - grid(W, i, 0, k);

                y = real(grid.ny) * grid.dx;
                // On y = phy_dim for domain point we have exact for V
                grid(V, i, grid.ny - 1, k) = Sol::v(x + sdx, y, z + sdz, currentTime);
                // For ghost points we have useless V, other approximate
                grid(U, i, grid.ny, k) = 2 * Sol::u(x + grid.dx, y, z + sdz, currentTime)
                                         - grid(U, i, grid.ny - 1, k);
                grid(V, i, grid.ny, k) = 0;
                grid(W, i, grid.ny, k) = 2 * Sol::w(x + sdx, y, z + grid.dz, currentTime)
                                         - grid(W, i, grid.ny - 1, k);
            }

        // apply on face with z constant
        for (index_t i = 0; i < grid.nx; i++)
            for (index_t j = 0; j < grid.ny; j++) {
                Real x = real(i) * grid.dx;
                Real y = real(j) * grid.dy;

                Real z = 0;
                // On z = 0 for ghost point we have exact for W, other approximate
                grid(U, i, j, -1) = 2 * Sol::u(x + grid.dx, y + sdy, z, currentTime)
                                    - grid(U, i, j, 0);
                grid(V, i, j, -1) = 2 * Sol::v(x + sdx, y + grid.dy, z, currentTime)
                                    - grid(V, i, j, 0);
                grid(W, i, j, -1) = Sol::w(x + sdx, y + sdy, z, currentTime);

                z = real(grid.nz) * grid.dz;
                // On z = phy_dim for domain point we have exact for W
                grid(W, i, j, grid.nz - 1) = Sol::w(x + sdx, y + sdy, z, currentTime);
                // For ghost points we gave useless W, other interpolate
                grid(U, i, j, grid.nz) = 2 * Sol::u(x + grid.dx, y + sdy, z, currentTime)
                                         - grid(U, i, j, grid.nz - 1);
                grid(V, i, j, grid.nz) = 2 * Sol::v(x + sdx, y + grid.dy, z, currentTime)
                                         - grid(V, i, j, grid.nz - 1);
                grid(W, i, j, grid.nz) = 0;

            }
    };

    // Define test condition function
    TFunction zero = [](Real x, Real y, Real z, Real t) {
        return 0;
    };

    // Define test condition
    Condition<STANDARD> inletBoundary(testBCMapper, zero);

    // Define boundary conditions
    Boundaries<STANDARD> boundaries;

    // Add condition to boundaries
    boundaries.addCond(inletBoundary);
    logger.printTitle("Boundary condition set");

    // Define Buffers for RK method
    Grid<STANDARD> Y2(model.nodes, model.spacing, model.gp);
    Grid<STANDARD> Y3(model.nodes, model.spacing, model.gp);

    logger.printTitle("Buffers created");

    // Time
    Real currentTime = 0.0;

    // last iteration l2Norm capture
    Real l2Norm = 0.0;

    // Performance variables
    Real nNodes = real(model.nx * model.ny * model.nz);
    Real perf;

    // Printing variables
    index_t iter = 0;
    index_t printIt = 100; // prints every n iterations

    logger.printTitle("Start computation");
    logger.openTable({"Iter", "ts", "l2", "rkT", "l2T", "TxN"});
    chrono_sect(compT,
                code_span(
                        boundaries.apply(model, currentTime);
                        while (currentTime < T) {
                            // call RK (obtain model at currentTime + dt)
                            chrono_sect(rkTime,
                                        code_span(
                                                rungeKutta(model, Y2, Y3, Re, deltaT, currentTime, boundaries);
                                                currentTime += deltaT;
                                        )
                            );


                            if (!(iter % printIt) || currentTime >= T) // prints every n iteration or if is the last one
                            {
                                chrono_sect(l2Time,
                                            code_span(
                                                    l2Norm = computeL2Norm<STANDARD>(model, currentTime);
                                            )
                                );
                                perf = rkTime / nNodes;
                                logger.printTableValues(iter, {currentTime, l2Norm, rkTime, l2Time, perf});
                            }
                            iter++;
                        }
                )
    );
    logger.closeTable();
    logger.printTitle("End of computation", compT);
    logger.closeSection();
    logger.empty();
    return l2Norm;
}
*/

int main(int argc, char *argv[]) {

    vector<Asset> assets = readStockAndCorrelations(10,
     "/home/viken/Desktop/MCTS_AMSC/MonteCarloTreeSearch/Python/Stocks.csv", 
     "/home/viken/Desktop/MCTS_AMSC/MonteCarloTreeSearch/Python/correlation_matrix.csv");

    // Output the data
   // for (const auto& asset : assets) {
     //   asset.AssetInformation();
     //   cout << "CORR"<< asset.getCorrelation(2) << endl;
    //}

    vector<Real> weights(Asset::getNumberOfAssets());

    Portfolio<Real> myPortfolio(move(assets), move(weights));
    myPortfolio.initializeWeights();

    int i = 1;
    Node<Portfolio<Real>> myNode(i,move(myPortfolio));
   

    myNode.getPortfolio().initializeWeights();

    Real y = myNode.getPortfolio().simulatePerformance();
    Real y2 = myNode.getPortfolio().computePortfolioValue();
    
    cout <<"Portfolio Performance :" << y << endl;

    cout << "Portfolio Value :" << y2 << endl;

    myNode.getPortfolio().printWeights();
    

/*
    // Create a Portfolio object

    vector<Asset> assets = {myAsset, myAsset2};
    vector<Real> weights = {0.5, 0.5};

    Portfolio<Real> myPortfolio(assets, weights);
    
    int i = 1;
    Node<Portfolio<Real>> myNode(i);
    
    */
    return 0;
}

