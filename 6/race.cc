#include <string>
#include <regex>
#include <iostream>
#include "helpers.hpp"

std::vector<long long> getValue(std::string const & line)
{
    std::regex rgx("\\d+");

    auto numbers_begin = std::sregex_iterator(line.begin(), line.end(), rgx);
    auto numbers_end = std::sregex_iterator();

    std::string str{};

    for (std::sregex_iterator i = numbers_begin; i != numbers_end; ++i)
    {
        str.append(i->str());
    }

    std::cout << "String is: " << str << std::endl;

    return std::vector<long long>{std::stoll(str)};
}

// TODO: Can I do with this combinatorics somehow?
int main(int argc, const char ** argv)
{
    if (argc < 2)
    {
        std::cerr << "Please specify filename to read in!" << std::endl;
        return -1;
    }

    auto lines = getLinesInFile(argv[1]);

    // Part 1
    // auto times = getNumbersInLine(lines[0]);
    // auto distances = getNumbersInLine(lines[1]);

    // Part 2
    auto times = getValue(lines[0]);
    auto distances = getValue(lines[1]);

    if (times.size() != distances.size())
    {
        std::cerr << "Times and distances don't match!" << std::endl;
        return -1;
    }

    long long totalNumOfWaysToWin = 0;

    for (long long i = 0; i < times.size(); ++i)
    {
        long long numOfWaysToWinThisGame = 0;

        for (long long j = 0; j <= times[i]; ++j)
        {
            long long const finalDistance = (times[i] - j) * j;

            if (finalDistance > distances[i])
                numOfWaysToWinThisGame++;
        }

        if (0 == totalNumOfWaysToWin)
            totalNumOfWaysToWin = numOfWaysToWinThisGame;
        else
            totalNumOfWaysToWin *= numOfWaysToWinThisGame;
    }

    std::cout << "Product of number of ways to win: " << totalNumOfWaysToWin << std::endl;
}