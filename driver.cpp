/*
 * Copyright (c) 2015 <name of copyright holder>
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

// Configuration file
#include "config.h"

// Include the VPTree library
#include "vafile.h"

// Stream processing
#include <iostream>
#include <fstream>

// STL
#include <vector>
#include <iterator>

// Time
#include <chrono>

using namespace std;
using namespace VAFile;

void processQuery() {
    // Open the query file
    ifstream ifile(QUERYFILE);

    long query;

    // Loop over the entire file
    while (ifile >> query) {
        // Get the point from the file
        vector <double> point;
        double coordinate;
        for (long i = 0; i < DIMENSIONS; ++i) {
            ifile >> coordinate;
            point.push_back(coordinate);
        }

#ifdef OUTPUT
            cout << endl << query << " ";
            copy(point.begin(), point.end(), ostream_iterator<double>(cout, " "));
#endif

        if (query == 1) {
#ifdef OUTPUT
            cout << endl;
#endif

#ifdef TIME
            cout << query << " ";
            auto start = std::chrono::high_resolution_clock::now();
#endif

            pointQuery(point);

#ifdef TIME
            auto elapsed = std::chrono::high_resolution_clock::now() - start;
            long long microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
            cout << microseconds << endl;
#endif

        } else if (query == 2) {
            // Get the range
            double range;
            ifile >> range;

#ifdef OUTPUT
            cout << " " << range << endl;
#endif

#ifdef TIME
            cout << query << " ";
            auto start = std::chrono::high_resolution_clock::now();
#endif

            rangeQuery(point, range * 1.0);

#ifdef TIME
            auto elapsed = std::chrono::high_resolution_clock::now() - start;
            long long microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
            cout << microseconds << endl;
#endif
        } else if (query == 3) {
            // Get the number of points
            long long k;
            ifile >> k;

#ifdef OUTPUT
            cout << " " << k << endl;
#endif

#ifdef TIME
            cout << query << " ";
            auto start = std::chrono::high_resolution_clock::now();
#endif

            kNNQuery(point, k);

#ifdef TIME
            auto elapsed = std::chrono::high_resolution_clock::now() - start;
            long long microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
            cout << microseconds << endl;
#endif
        }
    }

    // Close the file
    ifile.close();
}

int main() {
    // build a new VAFILE only if the old one does not exist
    if (getFileSize(VAFILE) == 0)  {
        buildVAFile();
    }

    // Process the query file
    processQuery();

    return 0;
}
