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

// To get the fileSize
#include <sys/stat.h>

// Stream Processing
#include <fstream>
#include <iostream>
#include <sstream>

// STL
#include <string>
#include <bitset>
#include <vector>
#include <queue>
#include <iterator>

// Math
#include <cmath>

namespace VAFile {
    // To keep a track of the number of objects
    long long objectCount = 0;

    long long getFileSize(const std::string& filename) {
        struct stat st;
        if(stat(filename.c_str(), &st) != 0) {
            return 0;
        }
        return (long long) st.st_size;
    }

    int quantize(double coordinate) {
        int first = 1;
        int last = ((int) pow(2, BITS)) - 1;
        double base = pow(2, -1 * BITS);

        // Coordinates towards the start
        if (coordinate <= base) {
            return 0;
        }

        // Coordinates towards the end
        if (coordinate >= base * last) {
            return last;
        }

        // Computation of quantization using binary search
        while (last >= first) {
            int mid = (first + last) / 2;

            // We check if the point is in the vicinity
            if (coordinate >= ((mid - 1) * base) && coordinate < (mid * base)) {
                return mid - 1;
            } else if  (coordinate >= (mid * base) && coordinate < ((mid + 1) * base)) {
                return mid;
            }

            // Update the bounds according to the value of the coordinate
            if (coordinate < mid * base) {
                last = mid - 1;
            } else {
                first = mid + 1;
            }
        }

        return 0;
    }

    double getMinDistance(std::vector<double> point, std::vector< std::bitset<BITS> > grid) {
        double base = pow(2, -1 * BITS);

        double minDistance = 0;
        for (int i = 0; i < DIMENSIONS; ++i) {
            double component = std::abs(grid[i].to_ulong() * base - point[i]);
            minDistance += component * component;
        }

        return std::sqrt(minDistance);
    }

    double getDistance(std::vector<double> point1, std::vector<double> point2) {
        double minDistance = 0;

        for (int i = 0; i < DIMENSIONS; ++i) {
            double component = (point1[i] - point2[i]) * (point1[i] - point2[i]);
            minDistance += component * component;
        }

        return std::sqrt(minDistance);
    }

    std::vector< std::bitset<BITS> > getQuantizedPoint(std::vector<double> point) {
        std::vector< std::bitset<BITS> > quantizedPoint;

        for (int i = 0; i < DIMENSIONS; ++i) {
            quantizedPoint.push_back(std::bitset<BITS>(quantize(point[i])));
        }

        return quantizedPoint;
    }

    std::pair< std::vector<double>, std::string > parseNormalLine(std::string line) {
        // Create a stringstream from the input line
        std::istringstream inputStream(line);

        // Read each coordinate from the stream and create a vector
        double coordinate;
        std::vector<double> coordinates;
        for (int i = 0; i < DIMENSIONS; ++i) {
            inputStream >> coordinate;
            coordinates.push_back(coordinate);
        }

        // Get the string
        std::string dataString;
        inputStream >> dataString;

        // Return a pair
        return make_pair(coordinates, dataString);
    }

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

        // Get the fileIndex from the line
        long long fileIndex;
        inputStream >> fileIndex;

        // Return a pair of coordinates and fileIndex
        return make_pair(coordinates, fileIndex);
    }

    void writeNormalFile(std::vector<double> point, std::string dataString, long long fileIndex) {
        // Encode the line and print it out to the file
        // Create an outputStream which will be written to the VAfile
        std::ostringstream outputStream;

        // Now add the quantized point to the outputStream
        for (auto coordinate : point) {
            outputStream << coordinate << " ";
        }

        // Now add the dataString
        outputStream << dataString;

        // Push this line to the file
        std::ofstream ofile(OBJECTBASE + std::to_string(fileIndex));
        ofile << outputStream.str() << std::endl << std::flush;
        ofile.close();
    }

    void writeVALine(std::vector<double> point, long long fileIndex, std::ofstream& ofile) {
        // Encode the line and print it out to the file
        // Create an outputStream which will be written to the VAfile
        std::ostringstream outputStream;

        // Now add the quantized point to the outputStream
        for (auto coordinate : point) {
            outputStream << std::bitset<BITS>(quantize(coordinate)) << " ";
        }

        // Now add the fileIndex
        outputStream << fileIndex;

        // Push this line to the file
        ofile << outputStream.str() << std::endl << std::flush;
    }

    void batchBuild() {
        std::ifstream ifile(DATAFILE);
        std::ofstream ofile(VAFILE);

        // Read the file line by line
        for (std::string line; std::getline(ifile, line); ++objectCount) {
            // Parse the input line into coordinates and string
            auto input = parseNormalLine(line);

            // We create a new file for the object and store is there
            writeNormalFile(input.first, input.second, objectCount);

            // Write the VALine to the file
            writeVALine(input.first, objectCount, ofile);
        }

        // Close open files
        ifile.close();
        ofile.close();
    }

    void rangeQuery(std::vector<double> point, double radius) {
        // TODO: Memory map the file incase it is smaller than memory size
        std::ifstream ifile(VAFILE);

        // Filter and search paradigm, so we need a queue
        std::queue<long long> fileIndices;

        // Loop over the entire VAFile and match
        for(std::string line; std::getline(ifile, line);) {
            auto VAPair = parseVALine(line);
            double minDistance = getMinDistance(point, VAPair.first);

            // If we cannot prune the grid, we add it to the queue
            if (minDistance <= radius) {
                fileIndices.push(VAPair.second);
            }
        }

        // The work of this file is over
        ifile.close();

        // Now we loop over the entire non pruned nodes and perform full computation
        while (!fileIndices.empty()) {
            // Get the current file index
            auto fileIndex = fileIndices.front();
            fileIndices.pop();

            // Open the file
            std::ifstream ifile(OBJECTBASE + std::to_string(fileIndex));

            // get the point
            std::string line;
            std::getline(ifile, line);
            auto dataPair = parseNormalLine(line);

            // compute the acutal distance
            if (getDistance(dataPair.first, point) <= radius) {
#ifdef OUTPUT
                std::cout << dataPair.second << std::endl;
#endif
            }

            ifile.close();
        }

    }
}
