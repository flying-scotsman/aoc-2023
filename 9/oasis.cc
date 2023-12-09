#include <iostream>
#include <regex>
#include <cstdint>
#include "helpers.hpp"

std::vector<std::vector<std::int64_t>> getSequences(std::vector<std::string> const & lines)
{
    std::vector<std::vector<std::int64_t>> output;

    std::regex rgx{"-?[0-9]+"};

    for (auto const & line : lines)
    {
        output.push_back({});

        auto numbers_begin = std::sregex_iterator(line.begin(), line.end(), rgx);
        auto numbers_end = std::sregex_iterator();

        // std::cout << "Adding sequence: ";

        for (std::sregex_iterator i = numbers_begin; i != numbers_end; ++i)
        {
            output.back().push_back(std::stoll(i->str()));
            // std::cout << std::stoll(i->str()) << " ";
        }

        // std::cout << std::endl;
    }

    return output;
}

bool checkForZeroes(std::vector<std::int64_t> const & seq)
{
    for (std::int64_t const el : seq)
    {
        if (0 != el)
            return false;
    }
    return true;
}

std::vector<std::vector<std::int64_t>> getDiffSequences(std::vector<std::int64_t> const & seq)
{
    std::vector<std::vector<std::int64_t>> diffSequences;
    diffSequences.push_back(seq);

    // auto const & lastSeq{sequences.back()};

    while(!checkForZeroes(diffSequences.back()))
    {
        std::vector<std::int64_t> newSeq;

        // Make differences of elements until they're all zero
        // std::cout << "Adding sequence: ";
        for (unsigned int i = 0; i < diffSequences.back().size() - 1; ++i)
        {
            newSeq.push_back(diffSequences.back()[i+1] - diffSequences.back()[i]);
            // std::cout << diffSequences.back()[i+1] - diffSequences.back()[i] << " ";
        }
        // std::cout << std::endl;

        diffSequences.push_back(newSeq);
    }

    return diffSequences;
}

void solvePartOne(std::vector<std::vector<std::int64_t>> & sequences)
{
    std::int64_t total{0};
    for (auto & seq : sequences)
    {
        auto diffSequences{getDiffSequences(seq)};

        if (diffSequences.size() < 3)
        {
            std::cerr << "diffSequences must have at least 3 levels!" << std::endl;
            return;
        }

        // Then add one more of the same to the next vector up,
        // and then add the last element of the vector before to the end of each one until
        // there's none left
        // Only need to start from the 3rd level from the bottom
        for (int i = diffSequences.size() - 3; i > -1; i--)
        {
            int a = diffSequences[i].back();
            int b = diffSequences[i+1].back();
            diffSequences[i].push_back(b + a);
        }

        seq.push_back(diffSequences[0].back());
        // std::cout << "Adding " << diffSequences[0].back() << " to the sequence." << std::endl;
        total += diffSequences[0].back();
    }

    std::cout << "Total: " << total << std::endl;
}

void solvePartTwo(std::vector<std::vector<std::int64_t>> & sequences)
{
    std::int64_t total{0};

    for (auto & seq : sequences)
    {
        auto diffSequences{getDiffSequences(seq)};

        if (diffSequences.size() < 3)
        {
            std::cerr << "diffSequences must have at least 3 levels!" << std::endl;
            return;
        }

        for (int i = diffSequences.size() - 3; i > -1; i--)
        {
            int a = diffSequences[i].front();
            int b = diffSequences[i+1].front();
            diffSequences[i].insert(diffSequences[i].begin(), a - b);
            // std::cout << i << ": " << a << ", " << b << std::endl;
        }

        seq.push_back(diffSequences[0].front());
        // std::cout << "Adding " << diffSequences[0].front() << " to the sequence." << std::endl;
        total += diffSequences[0].front();
    }

    std::cout << "Total: " << total << std::endl;
}

int main(int argc, const char ** argv)
{
    if (argc < 2)
    {
        std::cerr << "Please specify filename to read in!" << std::endl;
        return -1;
    }

    auto lines{getLinesInFile(argv[1])};

    auto sequences{getSequences(lines)};

    // solvePartOne(sequences);

    solvePartTwo(sequences);
}