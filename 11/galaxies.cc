#include <iostream>
#include <regex>
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

void duplicateColumns(std::vector<std::string> & lines)
{
    std::vector<std::string> columns(lines[0].size());

    // Create columns
    for (unsigned int i = 0; i < lines[0].size(); ++i)
    {
        for (unsigned int j = 0; j < lines.size(); ++j)
        {
            // I always want to append the ith element in a line to the ith column
            columns[i] += lines[j][i];

        }
    }

    // Now duplicate, reverse the order again and overwrite lines
    duplicateLines(columns);

    std::vector<std::string> rows(columns[0].size());

    // I need to fill the ith element in the jth row with the jth element from the ith row
    for (unsigned int i = 0; i < columns[0].size(); ++i)
    {
        for (unsigned int j = 0; j < columns.size(); ++j)
        {
            rows[i] += columns[j][i];
        }
    }

    lines = rows;
}

void solvePartOne(std::vector<std::string> & lines)
{
     // Duplicate all rows without galaxies first
    // Then slice through columns - and add elements to each row at the specific positions
    duplicateLines(lines);
    duplicateColumns(lines);

    // Then simply get the coordinates of the galaxies, find the vector between them and add x+y
    std::vector<helpers::Index> galaxies;
    for (int i = 0; i < lines.size(); ++i)
    {
        for (int j = 0; j < lines[i].size(); ++j)
        {
            if ('#' == lines[i][j])
                galaxies.push_back({i, j});
        }
    }

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

int main(int argc, const char ** argv)
{
    if (argc < 2)
    {
        std::cerr << "Please specify filename to read in!" << std::endl;
        return -1;
    }

    auto lines{helpers::getLinesInFile(argv[1])};


}