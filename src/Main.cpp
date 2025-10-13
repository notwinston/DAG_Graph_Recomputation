#include "parser.hpp"
#include "scheduler.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace {

bool loadProblemFromFile(const std::string& path, Problem& out_prob) {
	std::ifstream file(path);
	if (!file) {
		std::cerr << "Failed to open input file: " << path << "\n";
		return false;
	}

	std::stringstream buffer;
	buffer << file.rdbuf();
	std::string contents = buffer.str();

	long total_memory = 0;
	std::vector<ParsedNodeSpec> specs;
	std::string error;

	{
		std::istringstream in(contents);
		if (!parseExamplesFormat(in, total_memory, specs, error)) {
			std::istringstream in_simple(contents);
			specs.clear();
			if (!parseSimpleFormat(in_simple, total_memory, specs, error)) {
				std::cerr << "Failed to parse " << path << ": " << error << "\n";
				return false;
			}
		}
	}

	out_prob = buildProblem(total_memory, specs);
	return true;
}

void printSchedule(const Schedule_State& schedule) {
	std::cout << "Execution order (" << schedule.execution_order.size() << " steps):\n";
	for (std::size_t i = 0; i < schedule.execution_order.size(); ++i) {
		std::cout << "  [" << i << "] " << schedule.execution_order[i];
		if (i < schedule.recompute_flags.size() && schedule.recompute_flags[i]) {
			std::cout << " (recompute)";
		}
		std::cout << "\n";
	}
	std::cout << "Total time: " << schedule.total_time << "\n";
	std::cout << "Memory peak: " << schedule.memory_peak << "\n";
}

} // namespace

int main(int argc, char** argv) {
	std::string input_path = "input/example1.txt";
	if (argc > 1) {
		input_path = argv[1];
	}

	Problem prob;
	if (!loadProblemFromFile(input_path, prob)) {
		return 1;
	}

	std::cout << "Loaded problem with " << prob.nodes.size() << " nodes and total memory "
	          << prob.total_memory << "\n";

	Schedule_State schedule = greedySchedule(prob);

	if (schedule.execution_order.size() != prob.nodes.size()) {
		std::cerr << "Warning: schedule only executed " << schedule.execution_order.size()
		          << " of " << prob.nodes.size() << " nodes.\n";
	}

	printSchedule(schedule);

	return 0;
}

