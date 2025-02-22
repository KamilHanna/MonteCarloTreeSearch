#include "Logger.hpp"


//Constructor 
Logger::Logger(const int& width) : width(width){}


// Getters
int Logger::getWidth() const {
    return width;
}

string Logger::getHeader() const {
    return header;
}


vector<pair<string, vector<pair<string, VariantType>>>> Logger::getDataLine() const {
    return DataLine;
}


// Setters

void Logger::setWidth(const int& width) {
    this->width = width;
}   

void Logger::setHeader(const string& header) {
    this->header = header;
}


void Logger::setDataLine(const vector<pair<string, vector<pair<string, VariantType>>>>& DataLine) {
    this->DataLine = DataLine;
}

// Member functions

// Function to print a horizontal line with box-drawing symbols
void Logger::printHorizontalLine(const string& left, const string& middle, const string& right) {
    cout << left << string(width - 2, middle[0]) << right << endl;
}

// Function to print a centered header line
void Logger::printHeaderLine(const string& text) {
    int padding = (width - text.length() - 2) / 2;
    cout << "║" << string(padding, ' ') << text << string(padding, ' ') << "║" << endl;
}

// Function to print a data section title
void Logger::printDataSection(const string& text) {
    int padding = (width - text.length() - 4) / 2;
    cout << "║" << string(padding, '-') << " " << text << " " << string(padding, '-') << "║" << endl;
}

 // Function to print a content line
 void Logger::printContentLine(const string& left, const VariantType& right) {
    string rightStr = visit([](auto&& arg) -> string {
        using T = decay_t<decltype(arg)>;
        if constexpr (is_same_v<T, string>) {
            return arg;
        } else if constexpr (is_same_v<T, int> || is_same_v<T, double>) {
            return to_string(arg);
        } else if constexpr (is_same_v<T, bool>) {
            return arg ? "true" : "false";
        } else {
            return "";
        }
    }, right);

    cout << "║  " << left << string(width - left.length() - rightStr.length() - 6, ' ') << rightStr << "  ║" << endl;
}

// Function to print the display terminal box
void Logger::printDisplayBox() {
    printHorizontalLine("╔", "=", "╗");
    printHeaderLine(header);

    for (const auto& dataLine : DataLine) {
        printDataSection(dataLine.first);
        for (const auto& [left, right] : dataLine.second) {
            printContentLine(left, right);
        }
    }

    printHorizontalLine("╚", "=", "╝");
}



  


