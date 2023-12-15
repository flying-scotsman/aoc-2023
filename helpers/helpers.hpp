#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <regex>

namespace helpers
{

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

struct Index
{
    int r = -1;
    int c = -1;

    Index operator+(Index const & other)
    {
        return {r + other.r, c + other.c};
    }

    bool operator==(Index const & other) const
    {
        return r == other.r && c == other.c;
    }

    Index operator-(Index const & other)
    {
        return {r - other.r, c - other.c};
    }
};

std::vector<std::string> pivotLines(std::vector<std::string> const & lines)
{
    std::vector<std::string> pivotedLines(lines[0].size());

    // Create pivotedLines
    for (unsigned int i = 0; i < lines[0].size(); ++i)
    {
        for (unsigned int j = 0; j < lines.size(); ++j)
        {
            // I always want to append the ith element in a line to the ith column
            pivotedLines[i] += lines[j][i];
        }
    }

    return pivotedLines;
}

std::size_t factorial(std::size_t const n)
{
    return n != 0? n * factorial(n - 1) : 1;
}

std::size_t binomialCoefficient(std::size_t const n, std::size_t const k)
{
    if (n < k)
        return 0;

    return factorial(n) / (factorial(n - k) * factorial(k));
}

}