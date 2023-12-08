#include <iostream>
#include <unordered_map>
#include <string>
#include <nuemric>
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

    auto checkKeys = [&]() -> bool
    {
        // unsigned int count = 0;
        // bool allZ = true;
        for (auto const & key : keys)
        {
            if ('Z' != key[2])
            {
                // allZ = false;
                // continue;
                return false;
            }
            // count++;
        }
        return true;
        // if (3 < count)
        //     std::cout << count << " keys with Z!" << std::endl;
        // return allZ;
    };

    unsigned int step{0};

    // I could have a look at how long it takes for every key to get to Z by itself
    for (auto key : keys)
    {
        unsigned int step_test{0};
        std::vector<unsigned int> z_steps;

        auto currentKey = key;

        while (1000000 > step_test)
        {
            step_test++;

            unsigned int instruction = instructions[(step_test-1) % instructions.size()];
            currentKey = instruction ? mappings.at(currentKey).second : mappings.at(currentKey).first;
            // std::cout << "Current key: " << currentKey << std::endl;
            if ('Z' == currentKey[2])
            {
                z_steps.push_back(step_test);
            }
        }
        std::cout << "Z steps for key " << key << ": ";
        for (auto z_step : z_steps)
        {
            std::cout << z_step << " ";
        }
        std::cout << std::endl;
    }


    // while(!checkKeys())
    // {
    //     step++;

    //     unsigned int instruction = instructions[(step-1) % instructions.size()];

    //     for (auto & key : keys)
    //     {
    //         // auto oldKey = key;
    //         key = instruction ? mappings.at(key).second : mappings.at(key).first;
    //         // std::cout << "Changed " << oldKey << " to " << key << std::endl;
    //     }
    // }

    // std::cout << "Number of steps: " << step << std::endl;
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