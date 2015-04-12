# VAFile

## Results
- The computed time is in microseconds.

**VAFile**

QUERY  | MAX    | MIN    | AVG       | STD
------ | ------ | -----  | --------- | --------
1      | 115882 | 98666  | 103254    | 248.6
2      | 134572 | 100186 | 105565    | 448.3
3      | 304390 | 168004 | 280348    | 328.9


**Linear Array**

QUERY  | MAX    | MIN   | AVG       | STD
------ | ------ | ----- | --------- | --------
1      | 12411  | 9089  | 9728.65   | 3.565
2      | 11248  | 9159  | 9722.29   | 16.629
3      | 11708  | 9293  | 10168.5   | 9.25

## Observations

**Linear Array vs VA-file**

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
