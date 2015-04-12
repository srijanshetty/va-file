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
#include <string>

namespace LinearArray {
    /**
     * Build a Linear Array from a normal file
     */
    void buildLinearArray();

    /**
     * Perform pointQuery on the Linear Array
     * @param point A vector representation of the query point
     */
    void pointQuery(std::vector<double> point);

    /**
     * Perform rangeQuery on the Linear Array
     * @param point A vector representation of the query point
     * @param radius Query radius
     */
    void rangeQuery(std::vector<double> point, double radius);

    /**
     * Perform kNNQuery on the Linear Array
     * @param point A vector representation of the query point
     * @param k no of nearest neighbours
     */
    void kNNQuery(std::vector<double> point, long long k);
}
