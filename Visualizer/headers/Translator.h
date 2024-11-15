#pragma once
#include <string>

// Translator class to handle file translation from STL to DAT format
class Translator {
public:
    // Member function that Translates the given STL file to a DAT file
    void translateSTLtoDAT(const std::string& inputFilePath, const std::string& outputFilePath);
};


