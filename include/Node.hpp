#include <string>
#include <vector>
//Need seperate node class for BB search with peak and impact
class Node {
    private:
        std::string name_; //name of the operation
        std::vector<std::string> input_names_;
        int run_mem_; //memory required to run the operation's computation process
        int output_mem_; //memory required to store the output of the operation's computation process
        int time_cost_; //time required to run the operation's computation process
    public:
        Node() : run_mem_(0), output_mem_(0), time_cost_(0) {}
        Node(std::string name, std::vector<std::string> inputs, int run_mem, int output_mem, int time_cost) : name_(std::move(name)), input_names_(std::move(inputs)), run_mem_(run_mem), output_mem_(output_mem), time_cost_(time_cost)
        //getters
        const std::string& getName() const { return name_; }
        const std::vector<std::string>& getInputs() const { return input_names_; }
        int getRunMem() const { return run_mem_; }
        int getOutputMem() const { return output_mem_; }
        int getTimeCost() const { return time_cost_; }
};

//Current state of the schedule
struct Schedule_State {
    std::vector<std::string> execution_order;
    std::vector<bool> recompute_flags; // true if this step is a recomputation of a previously executed node, Denotes if the current node is recomputed
    int current_memory{0};
    int memory_peak{0};
    int total_time{0};
    std::unordered_set<std::string> computed; // Denotes the nodes that have been computed
    std::unordered_map<std::string, int> output_memory; // Denotes the memory required to store the output of the nodes
}

struct Problem {
    long total_memory{0}; // Total memory available
    std::unordered_map<std::string, Node> nodes; // Denotes the nodes in the problem
    //Edges
    std::unordered_map<std::string, std::unordered_set<std::string>> dependencies; // input -> consumers // Denotes the dependencies between the nodes
    std::unordered_map<std::string, std::vector<std::string>> successors; // node -> consumers list // Denotes the successors of the nodes
};
