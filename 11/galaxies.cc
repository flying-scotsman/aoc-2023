#include <iostream>
#include <regex>
#include <unordered_set>
#include "helpers.hpp"

void duplicateLines(std::vector<std::string> & lines)
{
    std::regex rgx{"\\#"};
    std::smatch match{};

    lines.reserve(lines.size() * 2);  // If every line is duplicated

    auto it = lines.begin();

    while (it != lines.end())
    {
        if (std::regex_search(*it, match, rgx))
        {
            it++;
            continue;
        }

        // No galaxy - add this content to lines
        lines.insert(it, *it);
        it += 2;  // Skip inserted line too
    }
}

std::unordered_set<unsigned int> getEmptyLines(std::vector<std::string> const & lines)
{
    std::unordered_set<unsigned int> output;

    std::regex rgx{"\\#"};
    std::smatch match{};

    for (unsigned int i = 0; i < lines.size(); ++i)
    {
        if (std::regex_search(lines[i], match, rgx))
            continue;

        // No galaxy - remember this id
        output.insert(i);
    }

    return output;
}

std::unordered_set<unsigned int> getEmptyColumns(std::vector<std::string> const & lines)
{
    std::vector<std::string> columns{helpers::pivotLines(lines)};

    return getEmptyLines(columns);
}

void duplicateColumns(std::vector<std::string> & lines)
{
    std::vector<std::string> columns{helpers::pivotLines(lines)};

    duplicateLines(columns);

    lines = helpers::pivotLines(columns);
}

std::vector<helpers::Index> getGalaxies(std::vector<std::string> const & lines)
{
    std::vector<helpers::Index> galaxies;

    for (int i = 0; i < lines.size(); ++i)
    {
        for (int j = 0; j < lines[i].size(); ++j)
        {
            if ('#' == lines[i][j])
                galaxies.push_back({i, j});
        }
    }

    return galaxies;
}

void solvePartOne(std::vector<std::string> & lines)
{
     // Duplicate all rows without galaxies first
    // Then slice through columns - and add elements to each row at the specific positions
    duplicateLines(lines);
    duplicateColumns(lines);

    // Then simply get the coordinates of the galaxies, find the vector between them and add x+y
    std::vector<helpers::Index> galaxies{getGalaxies(lines)};

    std::size_t distanceSum{0};

    for (int i = 0; i < galaxies.size(); ++i)
    {
        for (int j = i + 1; j < galaxies.size(); ++j)
        {
            helpers::Index diffIdx = galaxies[j] - galaxies[i];
            distanceSum += std::abs(diffIdx.r) + std::abs(diffIdx.c);
        }
    }

    std::cout << "Total distance sum: " << distanceSum << std::endl;
}

int universeExpander(int a, int b, std::vector<helpers::Index> const & galaxies,
    std::unordered_set<unsigned int> const & emptyLines,
    unsigned int const expansionFactor)
{
    unsigned int numOfEmptyLines{0};
    for (int k = std::min(a, b);
         k < std::max(a, b);
         ++k)
    {
        if (emptyLines.contains(k))
            numOfEmptyLines++;
    }

    int diff = std::abs(a - b) - numOfEmptyLines
        + numOfEmptyLines * expansionFactor;

    return diff;
}

void solvePartTwo(std::vector<std::string> const & lines)
{
    std::unordered_set<unsigned int> emptyRows = getEmptyLines(lines);
    std::unordered_set<unsigned int> emptyColumns = getEmptyColumns(lines);

    std::vector<helpers::Index> galaxies{getGalaxies(lines)};

    std::size_t distanceSum{0};

    for (int i = 0; i < galaxies.size(); ++i)
    {
        for (int j = i + 1; j < galaxies.size(); ++j)
        {
            distanceSum += universeExpander(galaxies[i].r, galaxies[j].r, galaxies, emptyRows, 1000000) +
                universeExpander(galaxies[i].c, galaxies[j].c, galaxies, emptyColumns, 1000000);
        }
    }

    std::cout << "Total distance sum: " << distanceSum << std::endl;
}

int main(int argc, const char ** argv)
{
    if (argc < 2)
    {
        std::cerr << "Please specify filename to read in!" << std::endl;
        return -1;
    }

    auto lines{helpers::getLinesInFile(argv[1])};

    solvePartTwo(lines);
}