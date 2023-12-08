#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <regex>

std::vector<std::string> getLinesInFile(std::string const & filename)
{
    std::fstream file{};
    std::vector<std::string> output{};

    file.open(filename, std::ios::in);

    if (!file.is_open())
    {
        std::cerr << "Couldn't open file!" << std::endl;
        return output;
    }

    for(std::string line; std::getline(file, line);)
    {
        output.push_back(line);
    }

    return output;
}

std::vector<long long> getNumbersInLine(std::string const & line)
{
    std::regex rgx("\\d+");

    auto numbers_begin = std::sregex_iterator(line.begin(), line.end(), rgx);
    auto numbers_end = std::sregex_iterator();

    std::vector<long long> output{};

    for (std::sregex_iterator i = numbers_begin; i != numbers_end; ++i)
    {
        output.push_back(std::stoll(i->str()));
    }

    return output;
}

template<typename T>
std::vector<T> slice(std::vector<T> const &v, int m, int n)
{
    auto first = v.cbegin() + m;
    auto last = v.cbegin() + n + 1;
 
    std::vector<T> vec(first, last);
    return vec;
}