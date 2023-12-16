#include <iostream>
#include <regex>
#include <optional>
#include "helpers.hpp"
#include "tsl/ordered_map.h"

std::vector<std::string> getWords(std::string const & sequence)
{
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

struct Code
{
    std::string letters;
    unsigned int box;
    char op;
    std::optional<unsigned int> val;
};

Code getCode(std::string const & w)
{
    std::regex rgx{"[a-z]+"};
    std::smatch letters;

    if (!std::regex_search(w, letters, rgx))
    {
        std::cerr << "Couldn't find any letters in word!" << std::endl;
        return {};
    }

    unsigned int box = hashAlgorithm(letters.str());
    unsigned int op_pos = letters.position() + letters.str().size();
    char op = w[op_pos];
    std::optional<unsigned int> val{ std::nullopt };
    if ('=' == op)
        val = static_cast<unsigned int>(w[op_pos + 1] - '0');

    return {letters.str(), box, op, val};
}

void solvePartTwo(std::string const & sequence)
{
    // Need a struct of string, operation and number - can split up words from part 1 for this and do it on the fly
    // Box is given by running hashAlgorithm on letters
    // Can I use an actual hashmap? The problem is it isn't sorted by entering, a vector makes much more sense
    // But I need a fast lookup to remove elements
    // I can use tsl::ordered_map for this :)

    std::vector<tsl::ordered_map<std::string, unsigned int>> lensBoxes(256);

    auto words{getWords(sequence)};

    for (std::string const & w : words)
    {
        Code code{getCode(w)};

        auto & thisMap{lensBoxes[code.box]};

        if ('-' == code.op)
        {
            if (!thisMap.contains(code.letters))
            {
                // do nothing
                continue;
            }

            // remove key
            thisMap.erase(code.letters);
        }
        else if ('=' == code.op)
        {
            // assign to this value or add new one
            thisMap[code.letters] = code.val.value();
        }
    }

    unsigned int totalFocusingPower{0};
    for (unsigned int i = 0; i < lensBoxes.size(); ++i)
    {
        unsigned int j = 1;
        for (auto [key, value] : lensBoxes[i])
        {
            totalFocusingPower += (i + 1) * j * value;
            j++;
        }
    }

    std::cout << "Total focusing power: " << totalFocusingPower << std::endl;
}

int main(int argc, const char ** argv)
{
    if (argc < 2)
    {
        std::cerr << "Please specify filename to read in!" << std::endl;
        return -1;
    }

    auto lines{helpers::getLinesInFile(argv[1])};

    // solvePartOne(lines[0]);
    solvePartTwo(lines[0]);
}