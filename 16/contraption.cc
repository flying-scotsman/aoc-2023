#include "helpers.hpp"
#include <iostream>
#include <unordered_map>
#include <unordered_set>

using Tiles = std::unordered_map<helpers::Index, unsigned int>;
using TilesSeen = std::unordered_set<helpers::IndexIncrement>;

std::vector<helpers::IndexIncrement> getNextIndices(char const c, helpers::IndexIncrement indexInc)
{
    std::vector<helpers::IndexIncrement> output;
    int row = indexInc.index.r;
    int col = indexInc.index.c;
    // get next indices based on value of character and direction approached from
    switch (indexInc.direction)
    {
    case 0: // coming from below
        if ('.' == c || '|' == c)
        {
            // keep going straight up
            output.push_back({{row - 1, col}, 0});
        }
        else if ('-' == c)
        {
            // head out both ways
            output.push_back({{row, col - 1}, 1});
            output.push_back({{row, col + 1}, 2});
        }
        else if ('\\' == c)
        {
            // head out left
            output.push_back({{row, col - 1}, 1});
        }
        else if ('/' == c)
        {
            // head out right
            output.push_back({{row, col + 1}, 2});
        }

        break;

    case 1: // coming from the right
        if ('.' == c || '-' == c)
        {
            // head out left
            output.push_back({{row, col - 1}, 1});
        }
        else if ('|' == c)
        {
            // head out both ways
            output.push_back({{row - 1, col}, 0});
            output.push_back({{row + 1, col}, 3});
        }
        else if ('/' == c)
        {
            // head down
            output.push_back({{row + 1, col}, 3});
        }
        else if ('\\' == c)
        {
            // head up
            output.push_back({{row - 1, col}, 0});
        }

        break;

    case 2: // coming from the left
        if ('.' == c || '-' == c)
        {
            // head out right
            output.push_back({{row, col + 1}, 2});
        }
        else if ('|' == c)
        {
            // head out both ways
            output.push_back({{row - 1, col}, 0});
            output.push_back({{row + 1, col}, 3});
        }
        else if ('/' == c)
        {
            // head up
            output.push_back({{row - 1, col}, 0});
        }
        else if ('\\' == c)
        {
            // head down
            output.push_back({{row + 1, col}, 3});
        }

        break;

    case 3: // coming from above
        if ('.' == c || '|' == c)
        {
            // keep going straight down
            output.push_back({{row + 1, col}, 3});
        }
        else if ('-' == c)
        {
            // head out both ways
            output.push_back({{row, col - 1}, 1});
            output.push_back({{row, col + 1}, 2});
        }
        else if ('\\' == c)
        {
            // head out right
            output.push_back({{row, col + 1}, 2});
        }
        else if ('/' == c)
        {
            // head out left
            output.push_back({{row, col - 1}, 1});
        }

        break;

    default:
        break;
    }

    return output;
}

// This modifies the energizedTiles map
// It may be more sensible to return the number of cells but the information gets lost
void traversePath(helpers::IndexIncrement const indexInc, std::vector<std::string> const & lines, Tiles & energizedTiles, TilesSeen & tilesSeen)
{
    if (tilesSeen.contains(indexInc) || 0 > indexInc.index.r || lines[0].size() <= indexInc.index.r || 0 > indexInc.index.c || lines.size() <= indexInc.index.c)
        return;

    energizedTiles[indexInc.index] += 1;  // Another hit
    tilesSeen.insert(indexInc);

    auto nextIndices = getNextIndices(lines[indexInc.index.r][indexInc.index.c], indexInc);

    for (helpers::IndexIncrement const idx : nextIndices)
    {
        traversePath(idx, lines, energizedTiles, tilesSeen);
    }
}

void solvePartOne(std::vector<std::string> const & lines)
{
    Tiles energizedTiles{};
    TilesSeen tilesSeen{};

    // We come into the top left (0, 0) from the left (position 2)
    traversePath({{0, 0}, 2}, lines, energizedTiles, tilesSeen);

    std::cout << "Number of energized tiles: " << energizedTiles.size() << std::endl;
}

void solvePartTwo(std::vector<std::string> const & lines)
{
    // Try the brute force way
    std::size_t maxTiles{0};

    int direction = 3; // come from the top
    int r = 0;
    int c = 0;

    auto checkForMax = [&]()
    {
        Tiles energizedTiles{};
        TilesSeen tilesSeen{};

        // We come into the top left (0, 0) from the left (position 2)
        traversePath({{r, c}, direction}, lines, energizedTiles, tilesSeen);

        maxTiles = std::max(maxTiles, energizedTiles.size());
    };

    for (c = 0; c < lines[0].size(); ++c)
    {
        checkForMax();
    }

    // Come from the left
    direction = 2;
    c = 0;
    r = 0;

    for (r = 0; r < lines.size(); ++r)
    {
        checkForMax();
    }

    // Come from the bottom
    direction = 0;
    r = lines.size() - 1;
    c = 0;

    for (c = 0; c < lines[0].size(); ++c)
    {
        checkForMax();
    }

    // Come from the right
    direction = 1;
    c = lines[0].size() - 1;
    r = 0;

    for (r = 0; r < lines.size(); ++r)
    {
        checkForMax();
    }

    std::cout << "Maximum number of energized tiles: " << maxTiles << std::endl;
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