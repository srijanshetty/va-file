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
#include "linear.h"

// We will use some utilities from vafile
#include "vafile.h"

// Stream Processing
#include <fstream>
#include <iostream>

// STL
#include <string>
#include <vector>
#include <queue>

namespace LinearArray {
    // Store the file as a linear array
    std::vector< std::pair<std::vector<double>, std::string> > linearArray;

    void buildLinearArray() {
        std::ifstream ifile(DATAFILE);
        std::ofstream ofile(VAFILE);

        // Read the file line by line
        for (std::string line; std::getline(ifile, line); ) {
            // Parse the input line into coordinates and string
            auto input = VAFile::parseNormalLine(line);

            // Write the VALine to the file
            linearArray.push_back(input);
        }

        // Close open files
        ifile.close();
        ofile.close();
    }

    void pointQuery(std::vector<double> point) {
        // Call rangeQuery with a zero radius
        rangeQuery(point, 0);
    }

    void rangeQuery(std::vector<double> point, double radius) {

        // Loop over the entire array and print point on match
        for(auto arrayEntry : linearArray) {
            if (VAFile::getDistance(point, arrayEntry.first) <= radius) {
#ifdef OUTPUT
                std::cout << arrayEntry.second << std::endl;
#endif
            }
        }
    }

    void kNNQuery(std::vector<double> point, long long k) {
        // Comparator for the priority queue
        class comparator {
            public:
                bool operator() (std::pair< std::string, double> &p1, std::pair<std::string, double> &p2) {
                    return p1.second < p2.second;
                }
        };

        // Maintain a priority queue for the k nearest neighbours
        std::priority_queue< std::pair< std::string, double>,
            std::vector< std::pair< std::string, double> >, comparator > nearestNeighbours;

        // Loop over the entire array and push to queue on match
        for(auto arrayEntry : linearArray) {
            double minDistance = VAFile::getDistance(point, arrayEntry.first);

            // If the queue is empty, we push elements into it
            if ((long long) nearestNeighbours.size() < k) {
                nearestNeighbours.push(std::make_pair(arrayEntry.second, minDistance));
            } else {
                // The pruning distance is the maximum distance of any point in the queue
                // Any element which is closer than the elements in the queue is pushed
                // onto the queue

                // Otherwise check the pruning distance criterion
                if (minDistance < nearestNeighbours.top().second) {
                    // We don't need the top now
                    nearestNeighbours.pop();

                    // Push the new element
                    nearestNeighbours.push(std::make_pair(arrayEntry.second, minDistance));
                }
            }
        }

        // Now we loop over the neighbours and print them
        while (!nearestNeighbours.empty()) {
#ifdef OUTPUT
            std::cout << nearestNeighbours.top().first << std::endl;
#endif
            nearestNeighbours.pop();
        }
    }
}
