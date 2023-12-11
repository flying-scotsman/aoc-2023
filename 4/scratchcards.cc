#include <regex>
#include <string>
#include <iostream>
#include <unordered_set>
#include "helpers.hpp"

std::string getJustTheNumbers(std::string const & line)
{
    std::regex rgx{"Card\\s+[0-9]+\\:"};
    std::smatch match;

    if (std::regex_search(line, match, rgx))
    {
        return line.substr(match.position()+match.str().size());
    }

    return {};
}

std::unordered_set<int> getNumbers(std::string const & line, bool winners)
{
    unsigned int pos{0};
    for (unsigned int i = 0; i < line.size(); ++i)
    {
        if ('|' == line[i])
        {
            pos = i;
            break;
        }
    }

    if (0 == pos)
    {
        std::cerr << "Couldn't find |!" << std::endl;
        return {};
    }

    std::string substring = winners? line.substr(0, pos) : line.substr(pos);

    std::regex rgx{"[0-9]+"};

    auto numbers_begin = std::sregex_iterator(substring.begin(), substring.end(), rgx);
    auto numbers_end = std::sregex_iterator();

    std::unordered_set<int> output{};

    for (std::sregex_iterator i = numbers_begin; i != numbers_end; ++i)
    {
        output.insert(std::stoi(i->str()));
    }

    return output;
}

void solvePartOne(std::vector<std::string> const & lines)
{
    // For each line, regex up to : and delete
    // Then get all numbers up to |
    // Then get all numbers after |
    // Then count the number of matches
    unsigned int totalScore{0};
    for (auto const & line : lines)
    {
        std::string numbers{getJustTheNumbers(line)};
        std::unordered_set<int> winners{getNumbers(numbers, true)};
        std::unordered_set<int> draw{getNumbers(numbers, false)};

        unsigned int matches{0};
        for (int n : draw)
        {
            if (winners.contains(n))
                matches++;
        }
        if (0 < matches)
            totalScore += std::pow(2, matches-1);
    }

    std::cout << "Total score: " << totalScore << std::endl;
}

int main(int argc, const char ** argv)
{
    if (argc < 2)
    {
        std::cerr << "Please specify filename to read in!" << std::endl;
        return -1;
    }

    auto lines = helpers::getLinesInFile(argv[1]);

    solvePartOne(lines);
}