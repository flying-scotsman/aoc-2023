// TODO: Seems like the numbers are too big to fill vectors with
// For each map I can just remember the range using src and length
// Then find the difference between the seed and src and add it to the corresponding dst

#include <iostream>
#include <string>
#include <array>
#include <numeric>
#include <limits>
#include <algorithm>
#include "helpers.hpp"

std::vector<std::vector<long long>> getMapsFromLines(std::vector<std::string> const & lines)
{
    std::vector<std::vector<long long>> output {{}};

    std::regex rgx_t("[A-Za-z]+");
    std::regex rgx_d("\\d+");
    std::regex rgx_anything("\\w+");

    std::smatch text_match{};
    std::smatch any_match{};

    std::vector<bool> validVec{};

    for (auto const & line : lines)
    {
        // Whitespace
        if (!std::regex_search(line, any_match, rgx_anything))
        {
            // std::cout << "New vector at " << any_match.str() << std::endl;
            output.push_back({});  // New matching vector
            validVec.clear();
            continue;
        }

        // Text
        if (std::regex_search(line, text_match, rgx_t))
        {
            // std::cout << text_match.str() << ", " << output.size() << std::endl;
            continue;
        }

        // std::cout << "Found numbers" << std::endl;

        auto numbers_begin = std::sregex_iterator(line.begin(), line.end(), rgx_d);
        auto numbers_end = std::sregex_iterator();

        // 0 = dst, 1 = src, 2 = length
        std::vector<long long> values{};

        for (std::sregex_iterator i = numbers_begin; i != numbers_end; ++i)
            values.push_back(std::stoll(i->str()));

        std::vector<long long> & currentVec = output.back();

        // First resize vector if necessary
        if (currentVec.size() <= values[1])
        {
            currentVec.resize(values[1] + values[2]);
            validVec.resize(values[01] + values[2]);
        }

        // Fills the current vector from the required position with increasing values
        std::iota(currentVec.begin() + values[1], currentVec.begin() + values[1] + values[2], values[0]);
        std::fill(validVec.begin() + values[1], validVec.begin() + values[1] + values[2], true);

        // Fill up vector
        for (int i = 0; i < output.back().size(); ++i)
        {
            if (!validVec[i])
                output.back()[i] = i;

            // std::cout << i << ": " << output.back()[i] << std::endl;
        }
    }

    // std::cout << output.size() << std::endl;

    return output;
}

// Function returns a vector of vector of vectors
// Outer is item mapping
// Secondmost outer is range collection
// Inner is src, dst and length
// Then later I can iterate through the second layer to find if a number
// is between src and length, then find the difference and add it to dst
// If it's not in any range then I just use the number directly
std::vector<std::vector<std::vector<std::size_t>>> getMappings(std::vector<std::string> const & lines)
{
    std::vector<std::vector<std::vector<std::size_t>>> output{{}};

    std::regex rgx_t("[A-Za-z]+");
    std::regex rgx_d("\\d+");
    std::regex rgx_anything("\\w+");

    std::smatch text_match{};
    std::smatch any_match{};

    for (auto const & line : lines)
    {
        // Whitespace
        if (!std::regex_search(line, any_match, rgx_anything))
        {
            // std::cout << "New vector at " << any_match.str() << std::endl;
            output.push_back({});  // New matching vector
            continue;
        }

        // Text
        if (std::regex_search(line, text_match, rgx_t))
        {
            // std::cout << text_match.str() << ", " << output.size() << std::endl;
            continue;
        }

        auto numbers_begin = std::sregex_iterator(line.begin(), line.end(), rgx_d);
        auto numbers_end = std::sregex_iterator();

        // 0 = dst, 1 = src, 2 = length
        std::vector<std::size_t> values{};

        for (std::sregex_iterator i = numbers_begin; i != numbers_end; ++i)
        {
            values.push_back(std::stoll(i->str()));
        }

        // std::cout << "Adding values to maps: ";
        // for (auto const value : values)
        //     std::cout << value << " ";
        // std::cout << std::endl;

        output.back().push_back(values);
    }

    // std::cout << "No. of maps: " << output.size() << std::endl;

    return output;
}

void solvePartOne(std::vector<std::string> const & lines, std::vector<long long> seeds)
{
    auto maps = getMappings(helpers::slice(lines, 2, lines.size()));

    std::size_t lowestLocation = INT64_MAX;

    for (std::size_t seed : seeds)
    {
        std::size_t newVal = seed;

        for (auto const & map : maps)
        {
            if (0 == map.size())
                continue;

            for (auto const & range : map)
            {
                // std::cout << "Range size: " << range.size() << std::endl;
                if (newVal > range[1] + range[2] || newVal < range[1])
                {
                    // Not in this range, continue
                    // std::cout << newVal << " outside range of " << range[1] << " and " << range[1] + range[2] << std::endl;
                    continue;
                }

                // If it's in the range, find the difference and add it to dst
                newVal = newVal - range[1] + range[0];

                // std::cout << "NewVal: " << newVal << std::endl;

                break;
            }
        }

        if (newVal < lowestLocation)
            lowestLocation = newVal;
    }

    std::cout << "Lowest location: " << lowestLocation << std::endl;
}

void solvePartTwo(std::vector<std::string> const & lines, std::vector<long long> seeds)
{
    auto maps = getMappings(helpers::slice(lines, 2, lines.size()));

    // Part 2
    // This also won't work because the numbers are way too big
    // The smallest number will come from the smallest destination range, as long as I have the value
    // So I should sort the ranges based on their destination values
    // But this doesn't account for the different mappings... low is not necessarily good!
    for (auto & map : maps)
    {
        std::sort(map.begin(), map.end(), [](const std::vector<std::size_t> &a, const std::vector<std::size_t> &b)
        {
            return a[0] < b[0];  // 0 dst, 1 src, 2 length
        });
    }

    for (unsigned int i = 0; i < seeds.size(); i += 2)  // Iterate through ranges of seeds
    {
        // std::size_t start = seeds[i];
        // std::size_t end = seeds[i+1];  // We assume the number of seed values is even so that this is safe

        // for (auto const & map : maps)
        // {
        //     bool matchFound = false;
        //     for (auto const & range : map)
        //     {
        //         // For each value:
        //         // 1. Check if I have a seed in the range of the next source using the 4 given numbers
        //         // - if so, take the lowest available number



        //     }

        //     // 2. If there's no match, use the lowest available value
        //     if (!matchFound)
        //     {

        //     }
        // }

    }
}

int main(int argc, const char ** argv)
{
    if (argc < 2)
    {
        std::cerr << "Please specify filename to read in!" << std::endl;
        return -1;
    }

    auto lines = helpers::getLinesInFile(argv[1]);

    std::vector<long long> seeds = helpers::getNumbersInLine(lines[0]);

    // solvePartOne(lines, seeds);

    solvePartTwo(lines, seeds);
}