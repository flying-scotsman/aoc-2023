#include <iostream>
#include <regex>
#include <string>
#include <unordered_map>
#include "helpers.hpp"

std::unordered_map<std::string, unsigned int> const limits{
    {"red", 12},
    {"green", 13},
    {"blue", 14}
};

struct MinimumRequiredNumbers
{
    unsigned int r = 0;
    unsigned int g = 0;
    unsigned int b = 0;
};

bool checkLineForValidity(std::string const & line)
{
    std::regex rgx("(\\d+)\\s*([a-zA-Z]+)");

    bool valid = true;

    auto words_begin = std::sregex_iterator(line.begin(), line.end(), rgx);
    auto words_end = std::sregex_iterator();

    for (std::sregex_iterator i = words_begin; i != words_end; ++i)
    {
        if (i->size() < 3)
            continue;

        if (!limits.contains(((*i)[2].str())))
            continue;

        valid = valid && limits.at((*i)[2].str()) >= std::stoi((*i)[1].str());
    }

    return valid;
}

MinimumRequiredNumbers checkMinNumber(std::string const & line)
{
    std::regex rgx("(\\d+)\\s*([a-zA-Z]+)");

    MinimumRequiredNumbers result{};

    auto words_begin = std::sregex_iterator(line.begin(), line.end(), rgx);
    auto words_end = std::sregex_iterator();

    for (std::sregex_iterator i = words_begin; i != words_end; ++i)
    {
        if (i->size() < 3)
            continue;

        if (!limits.contains(((*i)[2].str())))
            continue;

        std::string const col = (*i)[2].str();
        unsigned int const num = std::stoi((*i)[1].str());

        if (col == "red" && num > result.r)
            result.r = num;
        if (col == "green" && num > result.g)
            result.g = num;
        if (col == "blue" && num > result.b)
            result.b = num;
    }

    return result;
}

int main(int argc, const char ** argv)
{
    if (argc < 2)
    {
        std::cerr << "Please specify filename to read in!" << std::endl;
        return -1;
    }

    auto lines = helpers::getLinesInFile(argv[1]);

    unsigned int sum = 0;

    for (unsigned int i = 0; i < lines.size(); ++i)
    {
        // Part 1
        // if (checkLineForValidity(lines[i]))
        //     sum += (i + 1);

        // Part 2
        MinimumRequiredNumbers const minReqNums = checkMinNumber(lines[i]);
        sum += minReqNums.r * minReqNums.g * minReqNums.b;
    }

    std::cout << "Sum is " << sum << std::endl;
}