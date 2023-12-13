#include <iostream>
#include "helpers.hpp"

std::vector<std::vector<std::string>> getTextBlocks(std::vector<std::string> const & lines)
{
    std::vector<std::vector<std::string>> blocks{{}};

    for (auto const & line : lines)
    {
        if (line.empty())
        {
            blocks.push_back({});
            continue;
        }
        blocks.back().push_back(line);
    }

    return blocks;
}

std::vector<unsigned int> getColumnsLeftOfMirrorPlanes(std::vector<std::string> const & block)
{
    std::vector<unsigned int> mirrorPlanes;
    std::vector<bool> potentialMirrorPlanes(block[0].size()-1, true);

    for (std::string const & line : block)
    {
        for (int i = 0; i < potentialMirrorPlanes.size(); ++i)
        {
            int l = i;
            int r = i + 1;

            while (l >= 0 && r < line.size())
            {
                if (line[l] != line[r])
                {
                    potentialMirrorPlanes[i] = false;
                }

                l--;
                r++;
            }
        }
    }

    // Any surviving candidates are mirror planes
    // std::cout << "Mirror planes:" << std::endl;
    for (unsigned int i = 0; i < potentialMirrorPlanes.size(); ++i)
    {
        if (potentialMirrorPlanes[i])
        {
            // std::cout << i << std::endl;
            mirrorPlanes.push_back(i + 1); // Since we are counting planes and not
        }
    }

    return mirrorPlanes;
}

std::vector<unsigned int> getColumnsLeftOfMirrorPlanesWithSmudge(std::vector<std::string> const & block)
{
    std::vector<unsigned int> mirrorPlanes;
    std::vector<unsigned int> mirrorPlaneDefects(block[0].size()-1, 0);

    for (std::string const & line : block)
    {
        for (int i = 0; i < mirrorPlaneDefects.size(); ++i)
        {
            int l = i;
            int r = i + 1;

            while (l >= 0 && r < line.size())
            {
                if (line[l] != line[r])
                {
                    mirrorPlaneDefects[i]++;
                }

                l--;
                r++;
            }
        }
    }

    // Any planes where there's only one defect are our guys
    for (unsigned int i = 0; i < mirrorPlaneDefects.size(); ++i)
    {
        if (1 == mirrorPlaneDefects[i])
        {
            // std::cout << i << std::endl;
            mirrorPlanes.push_back(i + 1);
        }
    }

    return mirrorPlanes;
}

void solvePartOne(std::vector<std::string> const & lines)
{
    // First split the lines into blocks (patterns)
    auto blocks{getTextBlocks(lines)};
    unsigned int summary{0};
    for (auto const & block : blocks)
    {
        // Then for each line in the block we check the symmetry of each plane
        std::vector<unsigned int> mirrorPlanes{getColumnsLeftOfMirrorPlanes(block)};
        for (unsigned int const p : mirrorPlanes)
        {
            summary += p;
        }
        // Then do the same with pivoted rows
        std::vector<unsigned int> mirrorRows{getColumnsLeftOfMirrorPlanes(helpers::pivotLines(block))};
        for (unsigned int const p : mirrorRows)
        {
            summary += 100 * p;
        }
    }

    std::cout << "Part 1 summary: " << summary << std::endl;
}

void solvePartTwo(std::vector<std::string> const & lines)
{
    // First split the lines into blocks (patterns)
    auto blocks{getTextBlocks(lines)};
    unsigned int summary{0};
    for (auto const & block : blocks)
    {
        // Then for each line in the block we check the symmetry of each plane
        std::vector<unsigned int> mirrorPlanes{getColumnsLeftOfMirrorPlanesWithSmudge(block)};
        for (unsigned int const p : mirrorPlanes)
        {
            summary += p;
        }
        // Then do the same with pivoted rows
        std::vector<unsigned int> mirrorRows{getColumnsLeftOfMirrorPlanesWithSmudge(helpers::pivotLines(block))};
        for (unsigned int const p : mirrorRows)
        {
            summary += 100 * p;
        }
    }

    std::cout << "Part 2 summary: " << summary << std::endl;
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
    solvePartTwo(lines);
}