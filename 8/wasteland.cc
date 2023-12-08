#include <iostream>
#include <unordered_map>
#include <string>
#include <numeric>
#include "helpers.hpp"

std::unordered_map<std::string, unsigned int> instructionMap{
    {"L", 0},
    {"R", 1}
};

std::vector<unsigned int> getInstructions(std::string const & line)
{
    std::vector<unsigned int> output;

    for (auto c : line)
    {
        output.push_back(instructionMap.at(std::string{c}));
    }

    return output;
}

std::unordered_map<std::string, std::pair<std::string, std::string>> getMappings(std::vector<std::string> const & lines, std::string const & regex)
{
    std::unordered_map<std::string, std::pair<std::string, std::string>> output;

    std::regex rgx(regex.c_str());

    for (auto const & line : lines)
    {
        auto words_begin = std::sregex_iterator(line.begin(), line.end(), rgx);
        auto words_end = std::sregex_iterator();

        std::vector<std::string> matches;

        for (std::sregex_iterator i = words_begin; i != words_end; ++i)
        {
            matches.push_back(i->str());
        }

        if (matches.empty())
            continue;

        output.insert({matches[0], {matches[1], matches[2]}});

        // std::cout << "Inserted {" << matches[1] << ", " << matches[2] << "} at " << matches[0] << std::endl;
    }

    return output;
}

void solvePartOne(std::vector<unsigned int> const & instructions, std::unordered_map<std::string, std::pair<std::string, std::string>> const & mappings)
{
    std::string currentKey{"AAA"};
    unsigned int step{0};

    while ("ZZZ" != currentKey)
    {
        step++;

        unsigned int instruction = instructions[(step-1) % instructions.size()];
        // std::cout << "Instruction: " << instructions[(step-1) % instructions.size()] << std::endl;
        currentKey = instruction ? mappings.at(currentKey).second : mappings.at(currentKey).first;
    }

    std::cout << "Number of steps: " << step << std::endl;
}

void solvePartTwo(std::vector<unsigned int> const & instructions, std::unordered_map<std::string, std::pair<std::string, std::string>> const & mappings)
{
    std::vector<std::string> keys;

    std::cout << "Starting keys: ";
    for (auto const& [key, value]: mappings)
    {
        if ('A' == key[2])
        {
            keys.push_back(key);
            std::cout << key << " ";
        }
    }
    std::cout << std::endl;

    std::vector<unsigned int> stepsToZ;

    for (auto key : keys)
    {
        unsigned int step_test{0};

        auto currentKey = key;

        while ('Z' != currentKey[2])
        {
            step_test++;

            unsigned int instruction = instructions[(step_test-1) % instructions.size()];
            currentKey = instruction ? mappings.at(currentKey).second : mappings.at(currentKey).first;
        }
        std::cout << "Distance to first z instance for key " << key << ": " << step_test << std::endl;
        stepsToZ.push_back(step_test);
    }

    std::size_t solution{1};

    for (auto s : stepsToZ)
    {
        solution = std::lcm(solution, s);
    }

    std::cout << "Solution: " << solution << std::endl;
}

int main(int argc, const char ** argv)
{
    // TODO: Read in instructions (if match isn't found by the end of the instructions, repeat) (one line)
    // Then read in all the other lines
    // Format - map of pairs
    if (argc < 2)
    {
        std::cerr << "Please specify filename to read in!" << std::endl;
        return -1;
    }

    auto lines = getLinesInFile(argv[1]);

    auto instructions = getInstructions(lines[0]);

    // auto mappings = getMappings(slice(lines, 2, lines.size()), "[A-Z]+");

    // solvePartOne(instructionsPartOne, mappings);

    auto mappings = getMappings(slice(lines, 2, lines.size()), "[A-Z0-9]+");

    solvePartTwo(instructions, mappings);
}