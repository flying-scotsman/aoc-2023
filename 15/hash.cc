#include <iostream>
#include <regex>
#include "helpers.hpp"

std::vector<std::string> getWords(std::string const & sequence)
{
    // - Regex on absolutely anything but comma or newline
    std::vector<std::string> words{{}};

    for (char c : sequence)
    {
        if (',' == c)
        {
            words.push_back("");
            continue;
        }

        words.back() += c;
    }

    // for (auto w : words)
    //     std::cout << w << std::endl;

    return words;
}

unsigned int hashAlgorithm(std::string const & word)
{
    unsigned int currentValue{0};

    for (char c : word)
    {
        currentValue += static_cast<unsigned int>(c);
        currentValue *= 17;
        currentValue %= 256;
    }

    return currentValue;
}

void solvePartOne(std::string const & sequence)
{
    auto words{getWords(sequence)};

    unsigned int total{0};

    for (std::string const & w : words)
    {
        total += hashAlgorithm(w);
    }

    std::cout << "Total part 1: " << total << std::endl;
}

int main(int argc, const char ** argv)
{
    if (argc < 2)
    {
        std::cerr << "Please specify filename to read in!" << std::endl;
        return -1;
    }

    auto lines{helpers::getLinesInFile(argv[1])};

    solvePartOne(lines[0]);
}