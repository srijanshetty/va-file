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

// config
#include "config.h"

// STL
#include <vector>
#include <bitset>
#include <string>

namespace VAFile {
    /**
     * Get the size of a file.
     * @param filename The name of the file to check size for
     * @return The filesize, or 0 if the file does not exist.
     */
    long long getFileSize(const std::string& filename);

    /**
      * Compute the quantization of a given coordinate using binary search
      * @param coordinate The coordinate
      * @return an integer quantization value
      */
    int quantize(double coordinate);

    /**
      * Get the quantized grid for a point
      * @param point The point as a vector<double>
      * @return grid The grid to which the point belongs as vector<bitset>
      */
    std::vector< std::bitset<BITS> > getGrid(std::vector<double> point);

    /**
      * Get the minimum distance between a point and grid
      * @param point The point as a vector<double>
      * @param grid The grid as a vector<bitset>
      * @return Minimum distance
      */
    double getMinDistance(std::vector<double> point, std::vector< std::bitset<BITS> > grid);

    /**
      * Get the minimum distance between a point and grid
      * @param point1 The point as a vector<double>
      * @param point2 The second point as vector<double>
      * @return distance
      */
    double getDistance(std::vector<double> point1, std::vector<double> point2);

    /**
      * Check if two objects are equal
      * @param object1 The first object
      * @param object2 The second object
      * @return bool
      */
    bool equal(std::vector<double> object1, std::vector<double> object2);
    bool equal(std::vector< std::bitset<BITS> > object1, std::vector< std::bitset<BITS> > object2);

    /**
      * Parse a line from a normal file and return the coordinates
      * @param line The line to parse
      * @return A pair of the point as vector<double> and the string
      */
    std::pair< std::vector<double>, std::string > parseNormalLine(std::string line);

    /**
      * Parse a line from a VAFile and return the coordinates and lineCount
      * @param line The line to parse
      * @return A pair of the point as vector<bitset> and the lineCount
      */
    std::pair< std::vector< std::bitset<BITS> >, long long> parseVALine(std::string line);

    /**
      * Create a new file and store the vector, dataString
      * @param point The point as vector<double> to write
      * @param dataString the data string
      * @param fileCount index of file to write to
      * @return A pair of the point as vector<bitset> and the lineCount
      */
    void writeNormalFile(std::vector<double> point, std::string dataString, long long fileCount);

    /**
      * Write a vector and lineCount to the VAFile
      * @param point The point as vector<double> to write
      * @param lineCount the data
      * @param file The file to write to
      * @return A pair of the point as vector<bitset> and the lineCount
      */
    void writeVALine(std::vector<double> point, long long lineCount, std::ofstream& ofile);

    /**
     * Build a VAFile from a normal file
     */
    void buildVAFile();

    /**
     * Perform pointQuery on the VAFile
     * @param point A vector representation of the query point
     */
    void pointQuery(std::vector<double> point);

    /**
     * Perform rangeQuery on the VAFile
     * @param point A vector representation of the query point
     * @param radius Query radius
     */
    void rangeQuery(std::vector<double> point, double radius);

    /**
     * Perform kNNQuery on the VAFile
     * @param point A vector representation of the query point
     * @param k no of nearest neighbours
     */
    void kNNQuery(std::vector<double> point, long long k);
}
