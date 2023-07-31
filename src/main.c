#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdbool.h>
#include <string.h>
#include "main.h"
#include "process.h"
#include "variables.h"
#include "targets.h"
#include "append.h"
#include "readfile.h"
#include "bake.h"
#include "expandvar.h"

#define OPTLIST "C:f:inps"

// Argument flags
bool i_option_flag = false;
bool n_option_flag = false;
bool s_option_flag = false;
bool b_option_flag = false;

/**
 * Prints all variables, targets and actions to the user. The function iterates
 * through each LinkedList to print the relevant info. Uses bolded and tab
 * indentation in the output for clear output.
 */
void printInfo() {
	// Variables
	variable *varItem = variableList;
	printf("\n\033[1m\033[37mVARIABLES\033[0m\n");
	while(varItem != NULL) {
		printf("\t%s = %s\n", varItem->variable, varItem->value);
		varItem = varItem->next;
	}
	// Targets
	printf("\033[1m\033[37mTARGETS\033[0m\n");
	target *targetItem = targetList;
	while(targetItem != NULL) {
		printf("\t%s : ", targetItem->target);
		for(int i = 0; i < targetItem->numDep; i++) {
			printf("%s ", targetItem->dependencies[i]);
		}
		targetItem = targetItem->next;
		printf("\n");
	}
	// Actions
	printf("\033[1m\033[37mACTIONS\033[0m");
	target *actionItem = targetList;
	while(actionItem != NULL) {
		printf("\n\t%s\n", actionItem->target);
		for(int i = 0; i < actionItem->numAct; i++) {
			printf("\t%i. %s\n", i+1, actionItem->actions[i]);
		}
		actionItem = actionItem->next;
	}
	printf("\n");

}

/**
 * Main function to be called when ./bake is used. Handles command line
 * arguments using getopt() and prints appropiate error messages where needed
 */
int main(int argc, char *argv[]) {
	int opt; // getopt()

	char *filename = NULL;
	char *dirpath = NULL;

    bool p_option_flag = false;
    bool c_option_flag = false;
    bool f_option_flag = false;

	opterr = 0; // use custom error printing

	// Argument handler
	while((opt = getopt(argc, argv, OPTLIST)) != -1) {
		switch(opt) {
			case 'i':
				i_option_flag = !i_option_flag;
				break;
			case 'n':
				n_option_flag = !n_option_flag;
				break;
			case 'p':
				p_option_flag = !p_option_flag;
				break;
            case 'B':
                b_option_flag = !b_option_flag;
                break;
			case 's':
				s_option_flag = !s_option_flag;
				break;
			case 'C':
				dirpath = strdup(optarg);
                c_option_flag = !c_option_flag;
				break;
			case 'f':
				filename = strdup(optarg);
                f_option_flag = !f_option_flag;
				break;
			case '?':
				if(optopt == 'C') {
					fprintf(stderr, "Usage: %s [-C] [dirname]\n", argv[0]);
					exit(EXIT_FAILURE);
				} else if(optopt == 'f') {
					fprintf(stderr, "Usage: %s [-f] [filename]\n", argv[0]);
					exit(EXIT_FAILURE);
				} else {
					fprintf(stderr, "Illegal argument -%c\nUsage: %s [-Cfinps] [file..]\n", optopt, argv[0]);
					exit(EXIT_FAILURE);
				}
		}
	}

	// check for any targetname options
	char *targetToBuild = NULL;
	argc -= optind;
	argv += optind;

	if(argc > 0) {
		targetToBuild = *argv;
	}

	// check argument values
	if(dirpath != NULL) { // -C, change working directory
		int result = chdir(dirpath);

		if(result != 0) {
			fprintf(stderr, "%s: No such directory\n", dirpath);
			exit(EXIT_FAILURE);
		}
	}

	if(filename == NULL) { // -f, change default bakefile
		filename = "bakefile";
	}

	FILE *fp = fopen(filename, "r");
	if(fp == NULL) { // fopen() error checking
		if(dirpath == NULL) {
			perror(filename);
		} else {
			fprintf(stderr, "%s%s: No such file\n", dirpath, filename);
		}
		exit(EXIT_FAILURE);
	}

    // free allocated memory by strdup
    if (c_option_flag) {
        free(dirpath);
    }

    if (f_option_flag) {
        free(filename);
    }

    char *extend = NULL;
	// iterate through the file
	while(!feof(fp)) {
		// reads extended '\' lines
		extend = read_file(fp);

		// process the line if valid
		if(extend && extend[0] != '\0') {
			char *line = expandVariables(extend);

			if(currentTarget != NULL && *line == '\t') {
				processActionDef(line);
	        } else {
	            currentTarget = NULL;
	         	readLine(line);
	        }
		}
	}
    free(extend);

    (p_option_flag) ? printInfo() : bake(targetToBuild);

	fclose(fp);
	exit(EXIT_SUCCESS);
}
