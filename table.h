#pragma once

#include <string>
#include <vector>

// оця штука для друкування таблицб
class Table
{
public:
    explicit Table(std::vector<std::string> headers, int width = 13);

    void printHeader() const;
    void printRow(const std::vector<std::string>& cells) const;

private:
    std::vector<std::string> headers_;
    int width_;
};

// оце дає більше чисел після коми для малих і менше для великиих
std::string formatMs(double ms);