/*
 * Copyright (c) 2015 Srijan R Shetty
 * Author: Srijan R Shetty <srijan.shetty+code@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

// The configuration file
#include "config.h"

// The header file
#include "vafile.h"

// File IO
#include <fstream>
#include <iostream>
#include <sstream>

// STL
#include <string>
#include <bitset>
#include <vector>

// Math
#include <math.h>

namespace VAFile {
    // Helper function to get fileSize
    long long getFileSize(std::string FileName) {
        std::ifstream ifile;
        ifile.open (FileName.c_str(), std::ios::binary );
        ifile.seekg (0, std::ios::end);
        return ifile.tellg();
    }

    // Function to compute the quantization of a given coordinate
    // using binary search
    int quantize(double coordinate) {
        // This is the base
        int first = 1;
        int last = ((int) pow(2, BITS)) - 1;
        double base = pow(2, -1 * BITS);

        // The base case if it is smaller than first mark
        if (coordinate < base) {
            return 0;
        }

        // The value of quantization returned
        int quant = 0;

        while (last >= first) {
            // Compute the mid point
            int mid = (first + last) / 2;

            // Base case, when first and last are equal exit
            if (last == first) {
                quant = first;
                break;
            }

            // Now we compare the coordinate with values
            if (coordinate < mid * base) {
                last = mid - 1;
            } else {
                first = mid + 1;
            }
        }

        return quant;
    }

    // Parse a line from a normal file and return the coorinates
    std::vector<double> parseNormalLine(std::string line) {
        // Create a stringstream from the input line
        std::istringstream inputStream(line);

        // Read each coordinate from the stream and create a vector
        double coordinate;
        std::vector<double> coordinates;
        for (int i = 0; i < DIMENSIONS; ++i) {
            inputStream >> coordinate;
            coordinates.push_back(coordinate);
        }

        // Return a pair
        return coordinates;
    }

    // Parse a line from a VAFile and return the coordinates and lineCount
    std::pair< std::vector< std::bitset<BITS> >, long long> parseVALine(std::string line) {
        // Create a stringstream from the input line
        std::istringstream inputStream(line);

        // Read each coordinate from the stream and create a vector
        std::bitset<BITS> coordinate;
        std::vector< std::bitset<BITS> > coordinates;
        for (int i = 0; i < DIMENSIONS; ++i) {
            inputStream >> coordinate;
            coordinates.push_back(coordinate);
        }

        long long lineCount;
        inputStream >> lineCount;

        // Return a pair of coordinates and lineCount
        return make_pair(coordinates, lineCount);
    }

    // Function to build a VAFile for a given DATAFILE
    void batchBuild() {
        std::ifstream ifile(DATAFILE);
        std::ofstream ofile(VAFILE);

        // A pointer to the location of the point in the file
        long long lineCount = 1;

        // Read the file line by line
        for (std::string line; std::getline(ifile, line); ++lineCount) {
            // Parse the input line into coordinates and string
            std::vector<double> coordinates = parseNormalLine(line);

            // Encode the line and print it out to the file
            // Create an outputStream which will be written to the VAfile
            std::ostringstream outputStream;

            // Now add the quantized point to the outputStream
            for (auto coordinate : coordinates) {
                std::bitset<BITS> quant(quantize(coordinate));
                outputStream << quant << " ";
            }

            // Now add the lineCount
            outputStream << lineCount;

            // Push this line to the file
            ofile << outputStream.str() << std::endl << std::flush;
        }

        // Close open files
        ifile.close();
        ofile.close();
    }

    void rangeQuery(std::vector<double> point, double radius) {
        std::cout << getFileSize(VAFILE) << std::endl;
    }
}
