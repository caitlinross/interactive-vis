#include <stdlib.h>
#include <stdio.h>
#include <gvc.h>
#include "argp.h"
#include "glib.h"

#define MAX_PARTS 5
#define NUM_ENTRIES 28
#define MAX_LENGTH 1024

static void parse_data(char *filename, int **data);
static int parse_opt(int key, char *arg, struct argp_state *state);
void graphing(int **data, char *name, Agdesc_t type);
static Agraph_t *create_graph(char *name, Agdesc_t type, int **data);

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

    graphing(data, "class", Agundirected);

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
        if (line[0] != '#')
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

/* used by argp to parse command line args */
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

/* create graph using graphviz library */
void graphing(int **data, char *name, Agdesc_t type)
{
    Agraph_t *G;
    GVC_t *gvc;
    gvc = gvContext();
    G = create_graph(name, type, data);
    gvLayout(gvc, G, "circo");
    char filename[256];
    sprintf(filename, "%s.gv", name);
    gvRenderFilename(gvc, G, "dot", filename);
    sprintf(filename, "%s.png", name);
    gvRenderFilename(gvc, G, "png", filename);
    gvFreeLayout(gvc, G);
    agclose(G);
    gvFreeContext(gvc);
}

Agraph_t *create_graph(char *name, Agdesc_t type, int **data)
{
    Agraph_t *G = agopen(name, type, 0);
    Agnode_t *root = agnode(G, "Interactive Vis Students", 1);
    Agraph_t *before_RPI = agsubg(G, "before RPI", 1);
    Agraph_t *dorm = agsubg(G, "Roommates", 1);
    Agraph_t *ds = agsubg(G, "Data Structures", 1);
    Agraph_t *from_RPI = agsubg(G, "From RPI", 1);
    Agraph_t *today = agsubg(G, "this class", 1);
    Agnode_t *bR = agnode(before_RPI, "Knew before RPI", 1);
    Agnode_t *dor = agnode(dorm, "Met as roommates", 1);
    Agnode_t *d = agnode(ds, "Met in Data Structures", 1);
    Agnode_t *fR = agnode(from_RPI, "Met at RPI", 1);
    Agnode_t *t = agnode(today, "Met in this class", 1);
    agedge(G, root, bR, "", 1);
    agedge(G, root, dor, "", 1);
    agedge(G, root, d, "", 1);
    agedge(G, root, fR, "", 1);
    agedge(G, root, t, "", 1);
    
    for (int i = 0; i < NUM_ENTRIES; i++)
    {
        char tmp[16];
        char tmp2[16];
        Agnode_t *t_node;

        if (data[i][0] != 0)
        {
            sprintf(tmp, "%d_%d_0", data[i][0], i);
            sprintf(tmp2, "%d", data[i][0]);
            t_node = agnode(before_RPI, tmp, 1);
            agset(t_node, "label", tmp2); 
            agedge(before_RPI, bR, t_node, tmp, 1);
        }

        if (data[i][1] != 0)
        {
            sprintf(tmp, "%d_%d_1", data[i][1], i);
            sprintf(tmp2, "%d", data[i][1]);
            t_node = agnode(dorm, tmp, 1);
            agset(t_node, "label", tmp2); 
            agedge(dorm, dor, t_node, tmp, 1);
        }

        if (data[i][2] != 0)
        {
            sprintf(tmp, "%d_%d_2", data[i][2], i);
            sprintf(tmp2, "%d", data[i][2]);
            t_node = agnode(ds, tmp, 1);
            agset(t_node, "label", tmp2); 
            agedge(ds, d, t_node, tmp, 1);
        }

        if (data[i][3] != 0)
        {
            sprintf(tmp, "%d_%d_3", data[i][3], i);
            sprintf(tmp2, "%d", data[i][3]);
            t_node = agnode(from_RPI, tmp, 1);
            agset(t_node, "label", tmp2); 
            agedge(from_RPI, fR, t_node, tmp, 1);
        }

        if (data[i][4] != 0)
        {
            sprintf(tmp, "%d_%d_4", data[i][4], i);
            sprintf(tmp2, "%d", data[i][4]);
            t_node = agnode(today, tmp, 1);
            agset(t_node, "label", tmp2); 
            agedge(today, t, t_node, tmp, 1);
        }

    }
    return G;
}

