#include <regex>
#include <string>
#include <iostream>
#include <unordered_set>
#include <unordered_map>
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

void solvePartTwo(std::vector<std::string> const & lines)
{
    std::unordered_map<unsigned int, unsigned int> count;
    std::unordered_map<unsigned int, unsigned int> copies;
    unsigned int totalCards{0};
    for (unsigned int i = 0; i < lines.size(); ++i)
    {
        std::string numbers{getJustTheNumbers(lines[i])};
        std::unordered_set<int> winners{getNumbers(numbers, true)};
        std::unordered_set<int> draw{getNumbers(numbers, false)};

        unsigned int matches{0};
        for (int n : draw)
        {
            if (winners.contains(n))
                matches++;
        }
        // I now copy each scratchcard that I have matches
        // So if I have 3 matches in the 2nd card I copy the 3rd, 4th and 5th card
        // Each card that gets copied gets its matches counted
        // So if I have 2 copies of the 2nd card, its matches get counted twice
        count[i] = 1;
        // // Now see if there are any copies
        count[i] = copies.contains(i)? count[i] + copies[i] : count[i];

        // Now accrue the copies according to the matches
        if (0 == matches)
            continue;

        for (unsigned int j = 0; j < matches; ++j)
        {
            unsigned int cardNumber = j + i + 1;
            copies[cardNumber] = copies.contains(cardNumber)? copies[cardNumber] + count[i] : count[i];
        }
    }

    for (auto [key, value] : count)
    {
        totalCards += value;
    }

    std::cout << "Total number of cards: " << totalCards << std::endl;
}

int main(int argc, const char ** argv)
{
    if (argc < 2)
    {
        std::cerr << "Please specify filename to read in!" << std::endl;
        return -1;
    }

    auto lines = helpers::getLinesInFile(argv[1]);

    // solvePartOne(lines);
    solvePartTwo(lines);
}