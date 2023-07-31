#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * Gets each line from a particular Bakefile ensuring that they are extended
 * where required
 * @param  bakefile The Bakefile to grab a line from
 * @return          The pointer to the start of a line
 */
char* read_file(FILE *bakefile);
