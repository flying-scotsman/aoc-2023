#pragma once

#include <vector>
#include <string>
#include <fstream>

std::vector<std::string> getLinesInFile(std::string const & filename)
{
    std::fstream file{};
    std::vector<std::string> output{};

    file.open(filename, std::ios::in);

    if (!file.is_open())
    {
        std::cerr << "Couldn't open file!" << std::endl;
        return output;
    }

    for(std::string line; std::getline(file, line);)
    {
        output.push_back(line);
    }

    return output;
}