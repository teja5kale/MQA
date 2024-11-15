#include "Translator.h"
#include "STLReader.h"
#include "Triangulation.h"
#include "DATWriter.h"
#include <iostream>

// Function definiton to translate STL file to DAT format
void Translator::translateSTLtoDAT(const std::string& inputFilePath, const std::string& outputFilePath) {
    Triangulation tri;
    STLReader reader;
    
    // Read data from the input STL file into the triangulation data structure
    reader.read(inputFilePath, tri);
    
    DATWriter writer;
    
    // Write the triangulation data to the output DAT file
    writer.fileWriter(outputFilePath, tri.triangulizationDataStructure(), tri.getUniqueVertices());
}
