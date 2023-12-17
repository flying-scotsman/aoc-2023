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

unsigned int computeLoad(std::vector<std::string> const & lines)
{
    unsigned int totalLoad{0};

    for (unsigned int i = 0; i < lines.size(); ++i)
    {
        unsigned int numOfRoundedRocks{0};

        for (char c : lines[i])
            if ('O' == c)
                numOfRoundedRocks++;

        totalLoad += numOfRoundedRocks * (lines.size() - i);
    }

    return totalLoad;
}

void solvePartOne(std::vector<std::string> const & lines)
{
    auto columns{helpers::pivotLines(lines)};

    rollRocks(columns);

    auto rows{helpers::pivotLines(columns)};

    std::cout << "Total load: " << computeLoad(rows) << std::endl;
}

void tiltNorth(std::vector<std::string> & lines)
{
    // tilt north - pivot like normal, roll, pivot back
    lines = helpers::pivotLines(lines);
    rollRocks(lines);
    lines = helpers::pivotLines(lines);
}

void tiltWest(std::vector<std::string> & lines)
{
    rollRocks(lines);
}

void tiltSouth(std::vector<std::string> & lines)
{
    // tilt south - reverse whole vector, pivot, roll, pivot back, reverse back
    std::reverse(lines.begin(), lines.end());
    lines = helpers::pivotLines(lines);
    rollRocks(lines);
    lines = helpers::pivotLines(lines);
    std::reverse(lines.begin(), lines.end());
}

void tiltEast(std::vector<std::string> & lines)
{
    // tilt east - reverse each string, roll, reverse each string back
    for (auto & l : lines)
        std::reverse(l.begin(), l.end());
    rollRocks(lines);
    for (auto & l : lines)
        std::reverse(l.begin(), l.end());
}

std::size_t getHashOfRocks(std::vector<std::string> const & lines)
{
    std::size_t hash{0};

    for (auto const & l : lines)
        hash ^= std::hash<std::string>{}(l);

    return hash;
}

void performCycle(std::vector<std::string> & lines)
{
    tiltNorth(lines);
    tiltWest(lines);
    tiltSouth(lines);
    tiltEast(lines);
}

void solvePartTwo(std::vector<std::string> & lines, int numOfCycles)
{
    std::vector<std::string> newLines = lines;
    std::size_t currentValue{getHashOfRocks(lines)};
    std::vector<std::size_t> hashValues{currentValue};

    auto indexOfHash = [&]() -> int
    {
        for (unsigned int i = 0; i < hashValues.size(); ++i)
            if (hashValues[i] == currentValue)
                return i;

        return -1;
    };

    int count = 0;
    int pos = 0;

    while (true)
    {
        performCycle(newLines);
        currentValue = getHashOfRocks(newLines);
        count++;
        pos = indexOfHash();

        if (indexOfHash() != -1)
            break;

        hashValues.push_back(currentValue);
    }

    int cycleTime = count - pos;

    // We find the position in the cycle of the desired number
    // Then add it to the start of the regular cycle, which is given by the value
    // of count relative to cycleTime
    int numOfCyclesEquiv = (numOfCycles % cycleTime) + cycleTime * (count / cycleTime);

    for (int i = 0; i < numOfCyclesEquiv; ++i)
        performCycle(lines);

    std::cout << "Total load: " << computeLoad(lines) << std::endl;
}

int main(int argc, const char ** argv)
{
    if (argc < 2)
    {
        std::cerr << "Please specify filename to read in!" << std::endl;
        return -1;
    }

    auto lines{helpers::getLinesInFile(argv[1])};

    // solvePartOne(lines);
    solvePartTwo(lines, std::stol(argv[2]));
}