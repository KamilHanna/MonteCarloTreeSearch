#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <iostream>
#include <iomanip>
#include <string>
#include "Node.hpp"
#include <variant>
#include <vector>
#include <list>
#include <map>

using namespace std;
using VariantType = variant<string, int, Tiny, Real,bool>;

class Logger {

private:
    int width;                                                          // Width of the terminal box
    string header;                                                      // Header of the terminal box
    vector<pair<string, vector<pair<string, VariantType>>>> DataLine;   // Data lines of the terminal box

    public:
    // Constructor
    Logger(const int& width);

    // Getters
    int getWidth() const ;
    string getHeader() const;
    vector<pair<string, vector<pair<string, VariantType>>>> getDataLine() const;

   // Setters
    void setWidth(const int& name);
    void setHeader(const string& header);
    void setDataLine(const vector<pair<string, vector<pair<string, VariantType>>>>& DataLine);

    // Member functions
    void printHorizontalLine(const string& left, const string& middle, const string& right);
    void printHeaderLine(const string& text);
    void printDataSection(const string& text);
    void printContentLine(const string& left, const VariantType& right);
    void printDisplayBox();
 
};



#endif // LOGGER_HPP