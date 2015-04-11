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
#include <iterator>

// Math
#include <math.h>

namespace VAFile {
    /**
     * Get the size of a file.
     * @param filename The name of the file to check size for
     * @return The filesize, or 0 if the file does not exist.
     */
    long long getFileSize(const std::string& filename) {
        struct stat st;
        if(stat(filename.c_str(), &st) != 0) {
            return 0;
        }
        return (long long) st.st_size;
    }

    /**
      * Compute the quantization of a given coordinate using binary search
      * @param coordinate The coordinate
      * @return an integer quantization value
      */
    int quantize(double coordinate) {
        int first = 1;
        int last = ((int) pow(2, BITS)) - 1;
        double base = pow(2, -1 * BITS);

        // Base cases
        if (coordinate <= base) {
            return 0;
        }

        if (coordinate >= base * last) {
            return last;
        }

        // Computation of quantization using binary search
        int quantization = 0;
        while (last >= first) {
            int mid = (first + last) / 2;

            // Base case, when first and last are equal, we get the quantization
            if (first == last) {
                if (coordinate < first * base) {
                    quantization = first - 1;
                } else if (coordinate > first * base){
                    quantization = first + 1;
                } else {
                    quantization = first;
                }
                break;
            }

            // Update the bounds according to the value of the coordinate
            if (coordinate < mid * base) {
                last = mid - 1;
            } else {
                first = mid + 1;
            }
        }

        return quantization;
    }

    /**
      * Get the minimum distance between a point and grid
      * @param point The point as a vector<double>
      * @param grid The grid as a vector<bitset>
      * @return Minimum distance
      */
    double getMinDistance(std::vector<double> point, std::vector< std::bitset<BITS> > grid) {
        double base = pow(2, -1 * BITS);

        double minDistance = 0;
        for (int i = 0; i < DIMENSIONS; ++i) {
            double component = fabs(grid[i].to_ulong() * base - point[i]);
            minDistance += component * component;
        }

        return sqrt(minDistance);
    }

    /**
      * Get the quantized grid for a point
      * @param point The point as a vector<double>
      * @return grid The grid to which the point belongs as vector<bitset>
      */
    std::vector< std::bitset<BITS> > getQuantizedPoint(std::vector<double> point) {
        std::vector< std::bitset<BITS> > quantizedPoint;

        for (int i = 0; i < DIMENSIONS; ++i) {
            quantizedPoint.push_back(std::bitset<BITS>(quantize(point[i])));
        }

        return quantizedPoint;
    }

    /**
      * Parse a line from a normal file and return the coordinates
      * @param line The line to parse
      * @return A pair of the point as vector<double> and the string
      */
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

    /**
      * Parse a line from a VAFile and return the coordinates and lineCount
      * @param line The line to parse
      * @return A pair of the point as vector<bitset> and the lineCount
      */
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

        // Get the lineCount from the line
        long long lineCount;
        inputStream >> lineCount;

        // Return a pair of coordinates and lineCount
        return make_pair(coordinates, lineCount);
    }

    /**
      * Batch build a VAFile from a normal file
      */
    void batchBuild() {
        std::ifstream ifile(DATAFILE);
        std::ofstream ofile(VAFILE);

        // A pointer to the location of the point in the file
        long long lineCount = 1;

        // Read the file line by line
        for (std::string line; std::getline(ifile, line); ++lineCount) {
            // Parse the input line into coordinates and string
            std::pair< std::vector<double>, std::string> input = parseNormalLine(line);

            // Encode the line and print it out to the file
            // Create an outputStream which will be written to the VAfile
            std::ostringstream outputStream;

            // Now add the quantized point to the outputStream
            for (auto coordinate : input.first) {
                outputStream << std::bitset<BITS>(quantize(coordinate)) << " ";
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

    /**
      * Perform rangeQuery on the VAFile
      * @param point A vector representation of the query point
      * @param radius Query radius
      */
    void rangeQuery(std::vector<double> point, double radius) {
        // TODO: Memory map the file incase it is smaller than memory size

        std::vector< std::bitset<BITS> > grid = getQuantizedPoint(point);
    }
}
