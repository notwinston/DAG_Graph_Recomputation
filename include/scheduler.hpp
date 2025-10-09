#pragma once

#include "model.hpp"
//Helper Functions
//Calculates the peak memory required to execute the given node after the given state
int calculateSequentialPeak(const ScheduleState& state, const Node& node_B, int impact_A);
//Checks if the first schedule is better than the second schedule, True if the first schedule is better
bool isBetterSchedule(const ScheduleState& state1, const ScheduleState& state2, long total_memory);
//Simulates the execution of the given node on the given state and returns an updated schedule state
Schedule_State executeNode(const Node& node, const Schedule_State& state);

//Memory Freeing Functions
std::unordered_set<std::string> getFreeableInputs(
    const Node& node,
    const ScheduleState& state,
    const std::unordered_map<std::string, std::unordered_set<std::string>>& dependencies);
    //spill handling functions
static bool trySpillLargest(Schedule_State& state); //..
static bool trySpillBest(const Problem& prob, Schedule_State& state); //...

//Schedulers
ScheduleState TopologicalSchedule(const Problem& prob);
ScheduleState NaiveSchedule(const Problem& prob);
Schedule_State HeuristicSchedule(const Problem& prob);
Schedule_State BeamSearchSchedule(const Problem& prob, size_t beamWidth, size_t maxExpansions);
Schedule_State greedySchedule(const Problem& prob);
Schedule_State dpGreedySchedule(const Problem& prob, size_t lookaheadDepth, size_t branchFactor);
ScheduleState dfsScheduleLimited(const Problem& prob, size_t maxExpansions, double timeLimitSeconds);