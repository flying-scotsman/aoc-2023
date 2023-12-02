#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <optional>
#include <algorithm>
#include <vector>
#include <cstring>
#include "helpers.hpp"

std::vector<const char *> digits{
    "one",
    "two",
    "three",
    "four",
    "five",
    "six",
    "seven",
    "eight",
    "nine"
};

std::vector<char> characters{
    '1',
    '2',
    '3',
    '4',
    '5',
    '6',
    '7',
    '8',
    '9'
};

std::optional<char> getFirstDigitInString(std::string const & str, bool reverse = false)
{
    std::vector<unsigned> positions(digits.size());

    // How about saving a position to compare with? Starting with 0 and if you get to the end you have a match

    for (char const c: str)
    {
        if (std::isdigit(c))
            return c;  // Simple way out

        for (unsigned i = 0; i < digits.size(); ++i)
        {
            char const ref = reverse ? digits[i][strlen(digits[i]) - 1 - positions[i]] : digits[i][positions[i]];
            char const start = reverse ? digits[i][strlen(digits[i]) - 1] : digits[i][0];

            if (c != ref)
            {
                positions[i] = start == c ? 1 : 0;
                continue;
            }

            positions[i]++;

            if (strlen(digits[i]) == positions[i])
                return characters[i];
        }
    }

    return std::nullopt;
}

int main(int argc, const char ** argv)
{
    unsigned int totalSum = 0;

    if (argc < 2)
    {
        std::cerr << "Please specify filename to read in!" << std::endl;
        return -1;
    }

    std::vector<std::string> lines = getLinesInFile(argv[1]);

    // while(std::getline(file, line))
    for (auto line : lines)
    {
        auto const resultLeft = getFirstDigitInString(line);
        std::reverse(line.begin(), line.end());
        auto const resultRight = getFirstDigitInString(line, true);

        std::string lineSumStr{};

        if (resultLeft.has_value())
            lineSumStr.push_back(resultLeft.value());
        if (resultRight.has_value())
            lineSumStr.push_back(resultRight.value());

        if (!lineSumStr.empty())
            totalSum += std::stoi(lineSumStr);
    }

    std::cout << "Total sum is: " << totalSum << std::endl;
}
