#pragma once
#include "model.hpp"
#include <string>
#include <vector>
#include <istream>

//Parser Helper Functions & Structs
//intermediate structure to store the parsed node specs
struct ParsedNodeSpec {
    std::string name;
    int run_mem{0};
    int output_mem{0};
    int time_cost{0};
    std::vector<std::string> inputs;
};
//white space trimming
static inline std::string trim(const std::string& s);
//takes a string seperated by commas and splits it into a vector of strings
static inline std::vector<std::string> splitCommaList(const std::string& value)
//compiles the problem from the parsed node specs
Problem buildProblem(long total_memory, const std::vector<ParsedNodeSpec>& specs);

//Parser Functions
bool parseExamplesFormat(std::istream& in, long& total_memory,
                         std::vector<ParsedNodeSpec>& nodes_out,
                         std::string& error);

bool parseSimpleFormat(std::istream& in, long& total_memory,
                       std::vector<ParsedNodeSpec>& nodes_out,
                       std::string& error);

