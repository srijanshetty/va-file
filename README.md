# VAFile

## Results
- The computed time is in microseconds.

### VAFile

**Uniform Distribution**

QUERY  | MAX    | MIN    | AVG       | STD
------ | ------ | -----  | --------- | --------
1      | 115882 | 98666  | 103254    | 248.6
2      | 134572 | 100186 | 105565    | 448.3
3      | 304390 | 168004 | 280348    | 328.9

**Exp Distribution**

QUERY  | MAX    | MIN    | AVG       | STD
------ | ------ | -----  | --------- | --------
1      | 155766 | 95727  | 108512    | 3272.5
2      | 155044 | 97024  | 106746    | 467.8
3      | 395127 | 233080 | 297905    | 5135.1

### Linear Array

** Uniform Distribution**

QUERY  | MAX    | MIN   | AVG       | STD
------ | ------ | ----- | --------- | --------
1      | 12411  | 9089  | 9728.65   | 3.565
2      | 11248  | 9159  | 9722.29   | 16.629
3      | 11708  | 9293  | 10168.5   | 9.25

**Exp Distribution**

QUERY  | MAX    | MIN   | AVG       | STD
------ | ------ | ----- | --------- | --------
1      | 11245  | 9050  | 9710.3    | 20.43
2      | 10658  | 9109  | 9714.59   | 17.041
3      | 15306  | 9419  | 10373     | 23.2


## Observations

**Linear Array vs VA-file**
- The experiments match our intuition that linear array outperforms a VA File as there is no unnecessary overhead of disk access in the case of linear array. (This observation is without the pre-loading of the file to memory)

**Optimizations**
- Pre-loading the file to memory saves quite a lot of time and the performance is comparable to linear array in such a case.

**Effect of Distribution**
- The average case performance remains the same while the standard deviations increase by an order of magnitude.

## INSTALL

- Tree parameters are defined in *[config.h]*(config.h).

- To build a new tree one can run:

        $ ./configure.sh
        $ make

- The configuration for computing time/output is:

        // #define OUTPUT
        #define TIME

- To run either LinearArray or VAFile, the configuration is:

        // #define LINEAR
        #define VA
