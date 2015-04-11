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

// The header file
#include "vafile.h"

// File IO
#include <fstream>
#include <iostream>
#include <sstream>

namespace VAFile {

    // Function to build the VAFile
    void batchBuild() {
        std::ifstream ifile(DATAFILE);

        // Read the file line by line
        std::string line;
        while(std::getline(ifile, line)) {
            // Create a stringstream from the input line
            std::istringstream inputStream(line);

            // Read the point from the stream
            double coordinate;
            std::vector<double> coordinates;
            for (int i = 0; i < DIMENSIONS; ++i) {
                inputStream >> coordinate;
                coordinates.push_back(coordinate);
            }
            Point objectPoint = Point(coordinates);

            // Read the dataString from the file
            std::string dataString;
            inputStream >> dataString;

        }
    }
}
