#include "State_Space.hpp"
#include <algorithm>
#include <limits>
#include <iostream>
#include <unordered_set>

// Simple greedy scheduler implementation.
// Picks any ready node (all inputs satisfied) with smallest output memory and executes it.

namespace {

// return true if the node's inputs are available (either external or already computed)
bool isReady(const Problem& prob, const Schedule_State& state, const Node& node) {
	for (const auto& inp : node.getInputs()) {
		auto it = prob.nodes.find(inp);
		if (it != prob.nodes.end()) {
			if (state.computed.find(inp) == state.computed.end()) return false;
		}
	}
	return true;
}

// Free outputs of inputs to `executed` when none of their consumers remain uncomputed.
void freeFinishedInputs(const Problem& prob, Schedule_State& state, const Node& executed) {
	for (const auto& inp : executed.getInputs()) {
		auto it_node = prob.nodes.find(inp);
		if (it_node == prob.nodes.end()) continue; // external input

		bool allDone = true;
		auto it_cons = prob.dependencies.find(inp);
		if (it_cons != prob.dependencies.end()) {
			for (const auto& consumer : it_cons->second) {
				if (state.computed.find(consumer) == state.computed.end()) { allDone = false; break; }
			}
		}
		if (allDone) {
			auto itm = state.output_memory.find(inp);
			if (itm != state.output_memory.end()) {
				state.current_memory -= itm->second;
				state.output_memory.erase(itm);
			}
		}
	}
}

} // namespace

// public symbol expected by the project header: Schedule_State greedySchedule(const Problem& prob)
Schedule_State greedySchedule(const Problem& prob) {
	Schedule_State state;

	size_t total = prob.nodes.size();
	while (state.computed.size() < total) {
		// collect ready nodes not yet computed
		std::vector<const Node*> ready;
		ready.reserve(32);
		for (const auto& kv : prob.nodes) {
			const std::string& name = kv.first;
			if (state.computed.find(name) != state.computed.end()) continue;
			const Node& n = kv.second;
			if (isReady(prob, state, n)) ready.push_back(&n);
		}

		if (ready.empty()) {
			std::cerr << "greedySchedule: no ready nodes available. Remaining nodes: " << (total - state.computed.size()) << "\n";
			break;
		}

		// choose node with smallest output_mem, tie-break by run mem then name
		const Node* best = ready.front();
		for (const Node* n : ready) {
			if (n->getOutputMem() < best->getOutputMem()) best = n;
			else if (n->getOutputMem() == best->getOutputMem()) {
				if (n->getRunMem() < best->getRunMem()) best = n;
				else if (n->getRunMem() == best->getRunMem() && n->getName() < best->getName()) best = n;
			}
		}

		// simulate execution
		state.execution_order.push_back(best->getName());
		state.recompute_flags.push_back(false);
		state.total_time += best->getTimeCost();

		int outm = best->getOutputMem();
		if (outm > 0) {
			state.current_memory += outm;
			state.output_memory[best->getName()] = outm;
			if (state.current_memory > state.memory_peak) state.memory_peak = state.current_memory;
		}

		state.computed.insert(best->getName());

		freeFinishedInputs(prob, state, *best);
	}

	return state;

}

