#include "helpers.hpp"
#include <iostream>
#include <map>
#include <unordered_map>

std::unordered_map<char, unsigned int> CardOrder{
    {'A', 0},
    {'K', 1},
    {'Q', 2},
    {'T', 3},
    {'9', 4},
    {'8', 5},
    {'7', 6},
    {'6', 7},
    {'5', 8},
    {'4', 9},
    {'3', 10},
    {'2', 11},
    {'J', 12}
};

std::unordered_map<char, unsigned int> getCards(std::string const str, bool joker = false)
{
    std::unordered_map<char, unsigned int> cards;
    unsigned int numOfJokers{0};

    for (char c : str)
    {
        if ('J' == c && joker)
        {
            numOfJokers++;
            continue;
        }
        cards[c] = cards.contains(c)? cards[c] + 1 : 1;
    }

    if (joker)
    {
        // Add the number of jokers to each card
        for (auto & [key, value] : cards)
        {
            value += numOfJokers;
        }

        if (cards.empty())
        {
            // All jokers
            cards['J'] = 5;
        }
    }

    return cards;
}

std::pair<char, unsigned int> getMaxOccurrences(std::unordered_map<char, unsigned int> const & cards)
{
    std::pair<char, unsigned int> maxOccurrences{'_', 0};

    for (auto const [key, value] : cards)
    {
        if (value < maxOccurrences.second)
            continue;

        maxOccurrences = {key, value};
    }

    return maxOccurrences;
}

// Five of a kind, where all five cards have the same label: AAAAA
// Four of a kind, where four cards have the same label and one card has a different label: AA8AA
// Full house, where three cards have the same label, and the remaining two cards share a different label: 23332
// Three of a kind, where three cards have the same label, and the remaining two cards are each different from any other card in the hand: TTT98
// Two pair, where two cards share one label, two other cards share a second label, and the remaining card has a third label: 23432
// One pair, where two cards share one label, and the other three cards have a different label from the pair and each other: A23A4
// High card, where all cards' labels are distinct: 23456
bool comparePokerHands(std::string a, std::string b, bool joker = false)
{
    std::unordered_map<char, unsigned int> a_cards{getCards(a, joker)};
    std::unordered_map<char, unsigned int> b_cards{getCards(b, joker)};

    // If there are fewer entries in one, that hand is stronger
    if (a_cards.size() != b_cards.size())
        return a_cards.size() < b_cards.size();

    std::pair<char, unsigned int> maxOccurrencesA{getMaxOccurrences(a_cards)};
    std::pair<char, unsigned int> maxOccurrencesB{getMaxOccurrences(b_cards)};

    // If the maximum number of occurrences in one is more than the other, that hand is stronger
    if (maxOccurrencesA.second != maxOccurrencesB.second)
        return maxOccurrencesA.second > maxOccurrencesB.second;

    // Same number of different cards, same number of max. occurrences - same hand
    // Now the first card takes precedence, then the second and so on

    // std::cout << "Now arbitrating for " << a << " and " << b << std::endl;

    for (unsigned int i = 0; i < a.size(); ++i)
    {
        if (CardOrder[a[i]] != CardOrder[b[i]])
            return CardOrder[a[i]] < CardOrder[b[i]];
    }

    // The hands are identical, now it depends on the sort implementation as to whether order is preserved
    std::cout << "Identical hands! " << a << " " << b << std::endl;
    return true;
}

struct HandStrengthCmp
{
    bool operator()(std::string a, std::string b) const
    {
        return comparePokerHands(a, b);
    }
};

struct HandStrengthCmpWithJokers
{
    bool operator()(std::string a, std::string b) const
    {
        return comparePokerHands(a, b, true);
    }
};

std::map<std::string, int, HandStrengthCmp> getRankedHands(std::vector<std::string> const & lines)
{
    std::map<std::string, int, HandStrengthCmp> hands{};

    for (auto const & line : lines)
    {
        unsigned int const pos = line.find(" ");
        hands.insert({line.substr(0, pos), std::stoi(line.substr(pos))});
    }

    return hands;
}

std::map<std::string, int, HandStrengthCmpWithJokers> getRankedHandsWithJokers(std::vector<std::string> const & lines)
{
    std::map<std::string, int, HandStrengthCmpWithJokers> hands{};

    for (auto const & line : lines)
    {
        unsigned int const pos = line.find(" ");
        hands.insert({line.substr(0, pos), std::stoi(line.substr(pos))});
    }

    return hands;
}

void solvePartOne(std::vector<std::string> const & lines)
{
    // Map of string to int with custom sort operator to determine hand strength.
    auto rankedHands{getRankedHands(lines)};

    std::size_t totalWinnings{0};
    int rank = rankedHands.size();

    for (auto const & [key, value] : rankedHands)
    {
        if (rank == 0)
            break;

        totalWinnings += value * rank;
        rank--;
    }

    std::cout << "Total winnings part 1: " << totalWinnings << std::endl;
}

void solvePartTwo(std::vector<std::string> const & lines)
{
    // Map of string to int with custom sort operator to determine hand strength.
    auto rankedHands{getRankedHandsWithJokers(lines)};

    std::size_t totalWinnings{0};
    int rank = rankedHands.size();

    std::cout << "Hands ranked in order:" << std::endl;
    for (auto const & [key, value] : rankedHands)
    {
        std::cout << key << std::endl;
        if (rank == 0)
            break;

        totalWinnings += static_cast<std::size_t>(value) * static_cast<std::size_t>(rank);
        rank--;
    }

    std::cout << "Total winnings part 2: " << totalWinnings << std::endl;
}

int main(int argc, const char ** argv)
{
    if (argc < 2)
    {
        std::cerr << "Please specify filename to read in!" << std::endl;
        return -1;
    }

    auto lines = helpers::getLinesInFile(argv[1]);

    // solvePartOne(lines);
    solvePartTwo(lines);
}