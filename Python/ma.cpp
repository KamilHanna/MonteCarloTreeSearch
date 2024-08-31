#include <cstdlib>
#include <string>

int main() {

    int num_nodes = 100;

    // Construct the command to execute the Python script
    std::string command = "python3 main.py " + std::to_string(num_nodes);

    // Execute the command
    system(command.c_str());

    return 0;
}
