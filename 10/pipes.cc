#include <iostream>
#include <regex>
#include <cstdint>
#include "helpers.hpp"

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
};

// Position
//  0
// 1 2
//  3
struct IndexIncrement
{
    Index index{};
    int direction = -1;  // this refers to the position of this index relative to the previous one
};

Index getIndexOfS(std::vector<std::string> const & lines)
{
    for (int r = 0; r < lines.size(); ++r)
    {
        for (int c = 0; c < lines[r].size(); ++c)
        {
            if ('S' == lines[r][c])
                return {r, c};
        }
    }

    std::cerr << "No S in the input!" << std::endl;
    return {};
}

// Position
//  0
// 1 2
//  3

// | is a vertical pipe connecting north and south.
// - is a horizontal pipe connecting east and west.
// L is a 90-degree bend connecting north and east.
// J is a 90-degree bend connecting north and west.
// 7 is a 90-degree bend connecting south and west.
// F is a 90-degree bend connecting south and east.
// . is ground; there is no pipe in this tile.
bool isConnected(char const c, unsigned int relPos)
{
    switch (relPos)
    {
    case 0:
        return c == '|' || c == '7' || c == 'F';
    case 1:
        return c == '-' || c == 'L' || c == 'F';
    case 2:
        return c == '-' || c == 'J' || c == '7';
    case 3:
        return c == '|' || c == 'J' || c == 'L';
    default:
        std::cerr << "Provided invalid relPos!" << std::endl;
        break;
    }

    return false;
}

Index getDiffIndex(unsigned int relPos, Index const relIndex)
{
    switch (relPos)
    {
    case 0:
        return (Index{-1, 0} + relIndex);
    case 1:
        return (Index{0, -1} + relIndex);
    case 2:
        return (Index{0, 1} + relIndex);
    case 3:
        return (Index{1, 0} + relIndex);
    default:
        std::cerr << "Provided invalid relPos!" << std::endl;
        break;
    }

    return {};
}

// | is a vertical pipe connecting north and south.
// - is a horizontal pipe connecting east and west.
// L is a 90-degree bend connecting north and east.
// J is a 90-degree bend connecting north and west.
// 7 is a 90-degree bend connecting south and west.
// F is a 90-degree bend connecting south and east.
// . is ground; there is no pipe in this tile.
IndexIncrement getNextIndex(char const c, IndexIncrement const indexInc)
{
    switch (c)
    {
    case '|':
        if (indexInc.direction == 0)
            return {Index{-1, 0} + indexInc.index, 0};
        else if (indexInc.direction == 3)
            return {Index{1, 0} + indexInc.index, 3};
        break;
    case '-':
        if (indexInc.direction == 1)
            return {Index{0, -1} + indexInc.index, 1};
        else if (indexInc.direction == 2)
            return {Index{0, 1} + indexInc.index, 2};
        break;
    case 'L':
        if (indexInc.direction == 3)
            return {Index{0, 1} + indexInc.index, 2};
        else if (indexInc.direction == 1)
            return {Index{-1, 0} + indexInc.index, 0};
        break;
    case 'J':
        if (indexInc.direction == 3)
            return {Index{0, -1} + indexInc.index, 1};
        else if (indexInc.direction == 2)
            return {Index{-1, 0} + indexInc.index, 0};
        break;
    case '7':
        if (indexInc.direction == 0)
            return {Index{0, -1} + indexInc.index, 1};
        else if (indexInc.direction == 2)
            return {Index{1, 0} + indexInc.index, 3};
        break;
    case 'F':
        if (indexInc.direction == 0)
            return {Index{0, 1} + indexInc.index, 2};
        else if (indexInc.direction == 1)
            return {Index{1, 0} + indexInc.index, 3};
        break;
    default:
        break;
    }

    std::cerr << "Something has gone wrong here..." << std::endl;
    return {};
}

bool checkIndexValidity(Index const index, std::size_t const numRows, std::size_t const numCols)
{
    return index.r > 0 && index.r < numRows && index.c > 0 && index.c < numCols;
}

IndexIncrement findNextLinkToS(std::vector<std::string> const & lines, Index const s)
{
    for (unsigned int i = 0; i < 4; ++i)
    {
        Index diffIdx{getDiffIndex(i, s)};

        if(!checkIndexValidity(diffIdx, lines.size(), lines[0].size()))
            continue;

        if (isConnected(lines[diffIdx.r][diffIdx.c], i))
            return {diffIdx, static_cast<int>(i)};
    }

    return {};
}

void solvePartOne(std::vector<std::string> const & lines)
{
    Index s{getIndexOfS(lines)};

    IndexIncrement nextLink{findNextLinkToS(lines, s)};

    std::vector<Index> links{nextLink.index};

    while(nextLink.index != s)
    {
        // The character at nextLink.index combined with the direction we came from tells us where to go next
        // e.g. if it's J and prevPosition is 2, then we go up
        // Can I get around just hardcoding all the options? Or is there a better way?
        // Don't think so. There's only two options for each character anyway
        nextLink = getNextIndex(lines[nextLink.index.r][nextLink.index.c], nextLink);
        links.push_back(nextLink.index);
        // std::cout << "Next link: " << nextLink.index.r << ", " << nextLink.index.c << std::endl;
    }

    std::cout << "Farthest number of steps away from S: " << links.size() / 2 << std::endl;
}

void solvePartTwo(std::vector<std::string> const & lines)
{
    Index s{getIndexOfS(lines)};

    IndexIncrement nextLink{findNextLinkToS(lines, s)};

    std::vector<Index> links{nextLink.index};

    while(nextLink.index != s)
    {
        nextLink = getNextIndex(lines[nextLink.index.r][nextLink.index.c], nextLink);
        links.push_back(nextLink.index);
    }

    // Could use even/odd to determine for each point whether it's in the loop
    // Better solution: use Pick's theorem and shoelace formula to get the area and then the number of points
    std::size_t b = links.size();  // number of points on the boundary

    // Compute the area using the shoelace formula
    std::int64_t A{0};

    for (std::size_t i = 0; i < links.size(); ++i)
    {
        // Special case for links.size() - 1
        if (links.size() - 1 == i)
        {
            A += links[i].r * links[0].c - links[i].c * links[0].r;
            // std::cout << "Adding " << links[i].r * links[0].c - links[i].c * links[0].r << " to A" << std::endl;
        }
        else
        {
            A += links[i].r * links[i+1].c - links[i].c * links[i+1].r;
            // std::cout << "Adding " << links[i].r * links[i+1].c - links[i].c * links[i+1].r << " to A" << std::endl;
        }
    }

    std::cout << "Area of polygon: " << std::abs(A/2) << std::endl;

    std::int64_t i = std::abs(A/2) + 1 - b/2;

    std::cout << "Number of interior points: " << i << std::endl;
}

int main(int argc, const char ** argv)
{
    if (argc < 2)
    {
        std::cerr << "Please specify filename to read in!" << std::endl;
        return -1;
    }

    auto lines{getLinesInFile(argv[1])};

    // solvePartOne(lines);
    solvePartTwo(lines);
}