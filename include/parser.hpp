#pragma once

#include "State_Space.hpp"

#include <istream>
#include <string>
#include <vector>

struct ParsedNodeSpec {
    std::string name;
    int run_mem{0};
    int output_mem{0};
    int time_cost{0};
    std::vector<std::string> inputs;
};

bool parseSimpleFormat(std::istream& in,
                       long& total_memory,
                       std::vector<ParsedNodeSpec>& nodes_out,
                       std::string& error);

bool parseExamplesFormat(std::istream& in,
                         long& total_memory,
                         std::vector<ParsedNodeSpec>& nodes_out,
                         std::string& error);

Problem buildProblem(long total_memory, const std::vector<ParsedNodeSpec>& specs);