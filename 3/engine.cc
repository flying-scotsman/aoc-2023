// TODO: Use regex to find numbers, get their positions and then
// search neighbouring substrings for things that aren’t numbers or decimal points,
// if there’s a hit then save the number.

#include <regex>
#include <string>
#include <iostream>
#include "helpers.hpp"

bool searchSubstringForSymbols(std::string const str, unsigned int const numPos, unsigned int const numLength)
{
    if (str.empty())
        return false;

    // Form substring
    unsigned int const start = numPos > 0 ? numPos - 1 : 0;
    unsigned int const end = numPos + numLength >= str.length() ? str.length() - 1 : numPos + numLength;
    std::string const strToMatch = str.substr(start, end - start + 1);

    std::regex symbols("[^a-zA-Z0-9.]");

    std::smatch matches{};

    return std::regex_search(strToMatch, matches, symbols);
}

unsigned int getLineSum(
    std::string const line,
    std::string const prevLine = "",
    std::string const nextLine = "")
{
    std::regex rgx("\\d+");

    auto numbers_begin = std::sregex_iterator(line.begin(), line.end(), rgx);
    auto numbers_end = std::sregex_iterator();

    unsigned int lineSum = 0;

    for (std::sregex_iterator i = numbers_begin; i != numbers_end; ++i)
    {
        // Look in all three relevant lines at relevant positions for symbols
        bool matchValid =
            searchSubstringForSymbols(line, i->position(), i->str().size())
            || searchSubstringForSymbols(prevLine, i->position(), i->str().size())
            || searchSubstringForSymbols(nextLine, i->position(), i->str().size());

        if (matchValid)
            lineSum += std::stoi(i->str());
    }

    return lineSum;
}

std::vector<unsigned int> getAdjacentNumberMatchesInLine(
    std::string const line,
    unsigned int const start,
    unsigned int const end)
{
    std::vector<unsigned int> output{};

    if (line.empty())
        return output;

    std::regex rgx("\\d+");

    auto numbers_begin = std::sregex_iterator(line.begin(), line.end(), rgx);
    auto numbers_end = std::sregex_iterator();

    for (std::sregex_iterator i = numbers_begin; i != numbers_end; ++i)
    {
        if (i->position() > end)
            continue;

        if (i->position() + i->str().size() - 1 < start)
            continue;

        output.push_back(std::stoi(i->str()));
    }

    return output;
}

unsigned int getGearRatioOfLine(
    std::string const line,
    std::string const prevLine = "",
    std::string const nextLine = "")
{
    std::regex rgx("\\*");

    auto matches_begin = std::sregex_iterator(line.begin(), line.end(), rgx);
    auto matches_end = std::sregex_iterator();

    unsigned int gearRatio = 0;

    for (std::sregex_iterator i = matches_begin; i != matches_end; ++i)
    {
        // Search all relevant lines for exactly two neighbouring numbers
        // Remember that any part of the number can be in the relevant range
        unsigned int const start = i->position() > 0 ? i->position() - 1 : 0;
        unsigned int const end = i->position() < line.size() - 1 ? i->position() + 1 : line.size() - 1;

        auto const numbersInPrevLine = getAdjacentNumberMatchesInLine(prevLine, start, end);
        auto const numbersInLine = getAdjacentNumberMatchesInLine(line, start, end);
        auto const numbersInNextLine = getAdjacentNumberMatchesInLine(nextLine, start, end);

        if (numbersInPrevLine.size() + numbersInLine.size() + numbersInNextLine.size() != 2)
            continue;

        unsigned int gearRatioTemp = 1;

        for (unsigned int const n : numbersInPrevLine)
            gearRatioTemp *= n;
        for (unsigned int const n : numbersInLine)
            gearRatioTemp *= n;
        for (unsigned int const n : numbersInNextLine)
            gearRatioTemp *= n;

        gearRatio += gearRatioTemp;
    }

    return gearRatio;
}

int main(int argc, const char ** argv)
{
    if (argc < 2)
    {
        std::cerr << "Please specify filename to read in!" << std::endl;
        return -1;
    }

    auto lines = getLinesInFile(argv[1]);

    unsigned int totalSum = 0;

    for (unsigned int i = 0; i < lines.size(); ++i)
    {
        if (0 == i)
        {
            // Part 1
            // totalSum += getLineSum(lines[i], "", lines[i+1]);

            // Part 2
            totalSum += getGearRatioOfLine(lines[i], "", lines[i+1]);
        }
        else if (lines.size() - 1 == i)
        {
            // Part 1
            // totalSum += getLineSum(lines[i], lines[i-1], "");

            // Part 2
            totalSum += getGearRatioOfLine(lines[i], lines[i-1], "");
        }
        else
        {
            // Part 1
            // totalSum += getLineSum(lines[i], lines[i-1], lines[i+1]);

            // Part 2
            totalSum += getGearRatioOfLine(lines[i], lines[i-1], lines[i+1]);
        }
    }

    std::cout << "Total sum: " << totalSum << std::endl;
}
