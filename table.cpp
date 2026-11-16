#include "table.h"

#include <format>
#include <iostream>

using namespace std;

Table::Table(vector<string> headers, int width) : headers_(move(headers)), width_(width) {}

void Table::printHeader() const
{
    printRow(headers_);
    cout << string(headers_.size() * width_, '-') << '\n';
}

void Table::printRow(const vector<string>& cells) const
{
    for (const string& cell : cells)
        cout << format("{:>{}}", cell, width_);
    cout << '\n';
}

string formatMs(double ms)
{
    if (ms >= 1.0)
        return format("{:.3f}", ms);
    return format("{:.5f}", ms);
}