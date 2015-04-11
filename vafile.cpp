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

        // The base case if it is smaller than first mark
        if (coordinate < base) {
            return 0;
        }

        // Computation of quantization using binary search
        int quantization = 0;
        while (last >= first) {
            int mid = (first + last) / 2;

            // Base case, when first and last are equal, we get the quantization
            if (last == first) {
                quantization = first;
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
      * Parse a line from a normal file and return the coordinates
      * @param line The line to parse
      * @return A vector representing the point
      */
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

    /**
      * Parse a line from a VAFile and return the coordinates and lineCount
      * @param line The line to parse
      * @return A pair of the vector as a bitset and the lineCount
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

    /**
      * Perform rangeQuery on the VAFile
      * @param point A vector representation of the query point
      * @param radius Query radius
      */
    void rangeQuery(std::vector<double> point, double radius) {
        // Memory map the file incase it is smaller than memory size
        long long fileSize = getFileSize(VAFILE);

    }
}
