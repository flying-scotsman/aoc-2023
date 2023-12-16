#include <iostream>
#include <string_view>
#include "helpers.hpp"

void rollRocks(std::vector<std::string> & lines)
{
    for (std::string & s : lines)
    {
        unsigned int currentPos{0};
        unsigned int targetPos{0};

        while (s.size() > currentPos)
        {
            if ('.' == s[currentPos])
            {
                currentPos++;
                continue;
            }

            if ('#' == s[currentPos])
            {
                currentPos++;
                targetPos = currentPos;  // already incremented
                continue;
            }

            // We have a rounded rock
            std::swap(s[targetPos], s[currentPos]);
            targetPos++;
            currentPos++;
        }
    }
}

void solvePartOne(std::vector<std::string> const & lines)
{
    auto columns{helpers::pivotLines(lines)};

    rollRocks(columns);

    auto rows{helpers::pivotLines(columns)};

    unsigned int totalLoad{0};

    for (unsigned int i = 0; i < rows.size(); ++i)
    {
        unsigned int numOfRoundedRocks{0};

        for (char c : rows[i])
            if ('O' == c)
                numOfRoundedRocks++;

        totalLoad += numOfRoundedRocks * (rows.size() - i);
    }

    std::cout << "Total load: " << totalLoad << std::endl;
}

int main(int argc, const char ** argv)
{
    if (argc < 2)
    {
        std::cerr << "Please specify filename to read in!" << std::endl;
        return -1;
    }

    auto lines{helpers::getLinesInFile(argv[1])};

    solvePartOne(lines);
}