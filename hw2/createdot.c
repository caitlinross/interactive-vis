#include <stdlib.h>
#include <stdio.h>
#include "argp.h"
#include "glib.h"

#define MAX_PARTS 5
#define NUM_ENTRIES 28
#define MAX_LENGTH 1024

static void parse_data(char *filename, int **data);
static int parse_opt(int key, char *arg, struct argp_state *state);

char doc[] = "This program generates DOT files for graphvis";
char args_doc[] = "";

int num_nodes = 0;

static struct argp_option options[] = {
    {"nodes", 'n', "NUM", 0, "number of nodes in graph"},
    {"sparse", 's', 0, 0, "sparse graph"},
    {"type", 't', 0, 0, "type of graph"},
    {0}
};
static struct argp argp = {options, parse_opt, args_doc, doc};

int main(int argc, char **argv)
{
    argp_parse(&argp, argc, argv, 0, 0, 0);
    char filename[256];
    sprintf(filename, "data.txt");

    // parse data from file
    int **data = malloc(NUM_ENTRIES*sizeof(int*));
    for (int i = 0; i < NUM_ENTRIES; i++){
        data[i] = malloc(MAX_PARTS*sizeof(int));
    }
    parse_data(filename, data);

    return EXIT_SUCCESS;
}

/* parses the data from the file and stores in 2d array */
void parse_data(char *filename, int **data)
{
    FILE *f = fopen(filename, "r");
    char line[MAX_LENGTH];

    if (f == NULL) {
        exit(EXIT_FAILURE);
    }

    int i = 0;
    while (fgets(line, sizeof(line), f) != NULL) /* read a line */
    {
        if (line[0] == '#')
        {
            gchar **parts = g_strsplit(line, ",", MAX_PARTS);
            for (int j = 0; j < MAX_PARTS; j++)
            {
                    data[i][j] = atoi(parts[j]);
            }
            i++;
            g_strfreev(parts);
        }
    }

}

int parse_opt(int key, char *arg, struct argp_state *state)
{
    switch(key)
    {
        case 'n':
            num_nodes = atoi(arg);
            break;
    }
    return 0;
}
