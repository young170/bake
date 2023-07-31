#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * Removes trailing \n and \r symbols in a line prior to the \0 symbol
 */
void trimLine(char *line) {
    while(*line != '\0') {
        if(*line == '\n' || *line == '\r') {
            *line = '\0';
            break;
        }

        line++;
    }
}

/**
 * Reads each line from a given file and checks if the last character contains
 * the extension line char, '\'. Extends the line to the next line accordingly.
 * Comment lines, starting with '#' are excluded at this point
 */
char* read_file(FILE *bakefile) {
	char line[BUFSIZ];
    int len = 0;

    char *full_line = NULL;
    int full_len = 0;

	while(fgets(line, sizeof line, bakefile) != NULL) { // read line-by-line
        // Exclude comment lines
        if(*line == '#') {
            continue;
        }

		trimLine(line);

        len = strlen(line);
        full_len += len;

        if(full_line == NULL) {
            full_line = strdup(line);
        } else {
            full_line = realloc(full_line, full_len+1);
            strcat(full_line, line);
        }

        // Check for more continuation symbols
        if(full_line[full_len-1] != '\\') {
            break;
        }

        full_line[--full_len] = '\0';
	}
    
    return full_line;
}
