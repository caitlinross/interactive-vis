#include <stdlib.h>
#include <stdio.h>
#include "glib.h"

#define MAX_PARTS 6
#define MAX_LENGTH 1024

void parse_data(char *filename);

int main(int argc, char **argv)
{
    char filename[256];
    sprintf(filename, "data.txt");

    parse_data(filename);

    return EXIT_SUCCESS;
}

void parse_data(char *filename)
{
    FILE *f = fopen(filename, "r");
    char line[MAX_LENGTH];

    if (f == NULL) {
        exit(EXIT_FAILURE);
    }

    while (fgets(line, sizeof(line), f) != NULL) /* read a line */
    {
        gchar **parts = g_strsplit(line, ",", MAX_PARTS);
        for (int i = 0; i < MAX_PARTS; i++)
        {

        }
        g_strfreev(parts);
    }

}

