#!/bin/sh

# Get the original contents save the last four lines
TEXT=$(head -n-4 config.h)

# Delete the old config file
rm -f config.h

# Create a new config.h file
echo "$TEXT" > config.h

# Echo in the parameters
awk '{
    if(NR == 1) {
        print "#define BITS", $0;
    } else if (NR == 2) {
        print "#define DIMENSIONS", $0;
    } else if (NR == 3) {
        print "#define PAGESIZE", $0;
    } else if (NR == 4) {
        print "#define MEMORY (", $0, "* 1000 * 1000 * 1000 )";
    }
}' < "vafile.config" >> "config.h"
