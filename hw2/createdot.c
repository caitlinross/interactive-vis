/*
 * Author: Caitlin Ross
 * CSCI 6963 Interactive Visualization
 * Assignment 2
 * Due:  2/4/16
 */
#include <stdlib.h>
#include <stdio.h>
#include <gvc.h>
#include "argp.h"
#include "glib.h"
#include <time.h>

#define MAX_PARTS 5
#define NUM_ENTRIES 28
#define MAX_LENGTH 1024

static void parse_data(char *filename, int **data);
static void class_data_stuff(char *filename);
static int parse_opt(int key, char *arg, struct argp_state *state);
static void graphing(int **data, char *name, Agdesc_t type, int tree_levels, int num_nodes);
static Agraph_t *class_graph(char *name, Agdesc_t type, int **data);
static Agraph_t *awesim_inf_graph(char *name, Agdesc_t type, int num_nodes);
static Agraph_t *bipartite_graph(char *name, Agdesc_t type);
static Agraph_t *create_tree(char *name, Agdesc_t type, int levels);
static Agraph_t *tree_recurse(Agraph_t *G, Agnode_t *root, int level, int *index);
static Agraph_t *create_square(char *name, Agdesc_t type, int num_nodes);

// some set up for argp
char doc[] = "This program generates DOT files for graphvis";
char args_doc[] = "";

// create flags and other vars for storing cmd line args
int num_nodes = 5;
int awesim_f = 0;
int class_f = 0;
int bipar_f = 0;
int square_f = 0;
int tree_levels = 0;
char *filename;
Agdesc_t type;

// cmd line options
static struct argp_option options[] = {
    {"nodes", 'n', "n", 0, "number of nodes in graph"},
    {"awesim", 'a', 0, 0, "create graph of awesim infrastructure"},
    {"class-data", 'c', "str", 0, "input filename to create graph from class data"},
    {"bipartite", 'b', 0, 0, "create a bipartite graph (use --nodes to set size)"},
    {"undirected", 'd', 0, 0, "set to make graph undirected, default is directed"},
    {"tree-levels", 't', "n", 0, "create a tree with the specified number of levels"},
    {"square", 'r', 0, 0, "create a square; use nodes to specify size"},
    {0}
};
static struct argp argp = {options, parse_opt, args_doc, doc};

int main(int argc, char **argv)
{
    srand48(time(0));
    type = Agstrictdirected;
    filename = malloc(sizeof(char)*256);
    argp_parse(&argp, argc, argv, 0, 0, 0);

    // call appropriate graphing functions based on cmd line args
    if (awesim_f)
        graphing(NULL, "awesim", type, 0, num_nodes);
    if(class_f)
        class_data_stuff(filename);
    if (bipar_f)
        graphing(NULL, "bipartite", type, 0, 0);
    if (tree_levels)
        graphing(NULL, "tree", type, tree_levels, 0);
    if (square_f)
        graphing(NULL, "square", type, 0, num_nodes);

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

// some set up for getting data and calling graphing cunction
void class_data_stuff(char *filename)
{
    int **data = malloc(NUM_ENTRIES*sizeof(int*));
    for (int i = 0; i < NUM_ENTRIES; i++){
        data[i] = malloc(MAX_PARTS*sizeof(int));
    }
    parse_data(filename, data);

    graphing(data, "class", Agdirected, 0, 0);
}

/* used by argp to parse command line args */
int parse_opt(int key, char *arg, struct argp_state *state)
{
    switch(key)
    {
        case 'n':
            num_nodes = atoi(arg);
            break;
        case 'a':
            awesim_f = 1;
            break;
        case 'c':
            class_f = 1;
            strcpy(filename, arg);
            break;
        case 'd':
            type = Agstrictundirected;        
            break;
        case 'b':
            bipar_f = 1;
            break;
        case 't':
            tree_levels = atoi(arg);
            break;
        case 'r':
            square_f = 1;
            break;
    }
    return 0;
}

// create graph using graphviz library 
void graphing(int **data, char *name, Agdesc_t type, int tree_levels, int num_nodes)
{
    Agraph_t *G;
    GVC_t *gvc;
    gvc = gvContext();
    if (strcmp(name, "class") == 0)
    {
        G = class_graph(name, type, data);
        gvLayout(gvc, G, "circo");
    }
    else if (strcmp(name, "awesim") == 0)
    {
        G = awesim_inf_graph(name, type, num_nodes);
        gvLayout(gvc, G, "dot");
    }
    else if (strcmp(name, "bipartite") == 0)
    {
        G = bipartite_graph(name, type);
        gvLayout(gvc, G, "dot");
    }
    else if (strcmp(name, "tree") == 0)
    {
        G = create_tree(name, type, tree_levels);
        gvLayout(gvc, G, "dot");
    }
    else 
    {
        G = create_square(name, type, num_nodes);
        gvLayout(gvc, G, "dot");
    }
    char filename[256];
    sprintf(filename, "%s.gv", name);
    gvRenderFilename(gvc, G, "dot", filename);
    sprintf(filename, "%s.png", name);
    gvRenderFilename(gvc, G, "png", filename);
    gvFreeLayout(gvc, G);
    agclose(G);
    gvFreeContext(gvc);
}

// create graph based on class data
Agraph_t *class_graph(char *name, Agdesc_t type, int **data)
{
    Agraph_t *G = agopen(name, type, 0);
    Agnode_t *root = agnode(G, "Interactive Vis Students", 1);
    agsafeset(root, "fillcolor", "plum", "white");
    agsafeset(root, "style", "filled", "filled");
    Agraph_t *before_RPI = agsubg(G, "before RPI", 1);
    Agraph_t *dorm = agsubg(G, "Roommates", 1);
    Agraph_t *ds = agsubg(G, "Data Structures", 1);
    Agraph_t *from_RPI = agsubg(G, "From RPI", 1);
    Agraph_t *today = agsubg(G, "this class", 1);
    Agnode_t *bR = agnode(before_RPI, "Knew before RPI", 1);
    agsafeset(bR, "fillcolor", "aquamarine", "white");
    agsafeset(bR, "style", "filled", "filled");
    Agnode_t *dor = agnode(dorm, "Met as roommates", 1);
    agsafeset(dor, "fillcolor", "pink", "white");
    agsafeset(dor, "style", "filled", "filled");
    Agnode_t *d = agnode(ds, "Met in Data Structures", 1);
    agsafeset(d, "fillcolor", "violet", "white");
    agsafeset(d, "style", "filled", "filled");
    Agnode_t *fR = agnode(from_RPI, "Met at RPI", 1);
    agsafeset(fR, "fillcolor", "skyblue", "white");
    agsafeset(fR, "style", "filled", "filled");
    Agnode_t *t = agnode(today, "Met in this class", 1);
    agsafeset(t, "fillcolor", "seagreen1", "white");
    agsafeset(t, "style", "filled", "filled");
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
            agsafeset(t_node, "label", tmp2, ""); 
            agedge(before_RPI, bR, t_node, tmp, 1);
        }

        if (data[i][1] != 0)
        {
            sprintf(tmp, "%d_%d_1", data[i][1], i);
            sprintf(tmp2, "%d", data[i][1]);
            t_node = agnode(dorm, tmp, 1);
            agsafeset(t_node, "label", tmp2, ""); 
            agedge(dorm, dor, t_node, tmp, 1);
        }

        if (data[i][2] != 0)
        {
            sprintf(tmp, "%d_%d_2", data[i][2], i);
            sprintf(tmp2, "%d", data[i][2]);
            t_node = agnode(ds, tmp, 1);
            agsafeset(t_node, "label", tmp2, ""); 
            agedge(ds, d, t_node, tmp, 1);
        }

        if (data[i][3] != 0)
        {
            sprintf(tmp, "%d_%d_3", data[i][3], i);
            sprintf(tmp2, "%d", data[i][3]);
            t_node = agnode(from_RPI, tmp, 1);
            agsafeset(t_node, "label", tmp2, ""); 
            agedge(from_RPI, fR, t_node, tmp, 1);
        }

        if (data[i][4] != 0)
        {
            sprintf(tmp, "%d_%d_4", data[i][4], i);
            sprintf(tmp2, "%d", data[i][4]);
            t_node = agnode(today, tmp, 1);
            agsafeset(t_node, "label", tmp2, ""); 
            agedge(today, t, t_node, tmp, 1);
        }

    }
    return G;
}

// create graph based on the infrastructure simulated in my research
Agraph_t *awesim_inf_graph(char *name, Agdesc_t type, int num_nodes)
{
    Agraph_t *G = agopen(name, type, 0);
    Agnode_t *awe_serv = agnode(G, "AWE Server", 1);
    Agraph_t *site1 = agsubg(G, "cluster_wan1", 1);
    agsafeset(site1, "label", "WAN Site 1", "");
    agsafeset(G, "concentrate", "true", "true");
    agsafeset(awe_serv, "root", "true", "false");
    Agnode_t *shock = agnode(site1, "Shock data server", 1);
    Agnode_t *router = agnode(site1, "router", 1);
    agedge(G, shock, router, NULL, 1);
    agedge(G, router, shock, NULL, 1);
    Agraph_t *site2 = agsubg(G, "cluster_wan2", 1);
    agsafeset(site2, "label", "WAN Site 2", "");

    Agnode_t *proxy1 = agnode(site1, "Proxy1", 1);
    agset(proxy1, "label", "Proxy");
    Agnode_t *proxy2 = agnode(site2, "Proxy2", 1);
    agset(proxy2, "label", "Proxy");

    agedge(G, router, proxy1, NULL, 1);
    agedge(G, proxy1, router, NULL, 1);
    agedge(G, router, proxy2, NULL, 1);
    agedge(G, proxy2, router, NULL, 1);

    for (int i = 0; i < num_nodes; i++)
    {
        char tmp[16];
        sprintf(tmp, "local client %d", i);
        Agnode_t *c1 = agnode(site1, tmp, 1);
        agset(c1, "label", "Client");
        agedge(G, proxy1, c1, NULL, 1);
        agedge(G, c1, proxy1, NULL, 1);
        agedge(G, awe_serv, c1, NULL, 1);
        agedge(G, c1, awe_serv, NULL, 1);

        sprintf(tmp, "remote client %d", i);
        Agnode_t *c2 = agnode(site2, tmp, 1);
        agset(c2, "label", "Client");
        agedge(G, proxy2, c2, NULL, 1);
        agedge(G, c2, proxy2, NULL, 1);
        agedge(G, awe_serv, c2, NULL, 1);
        agedge(G, c2, awe_serv, NULL, 1);
    }
    
    return G;
}

// create a complete bipartite graph
Agraph_t *bipartite_graph(char *name, Agdesc_t type)
{
    Agraph_t *G = agopen(name, type, 0);
    agsafeset(G, "nodesep", "1", "5");
    agsafeset(G, "ranksep", "3", "5");
    Agraph_t *g1 = agsubg(G, "cluster_1", 1);
    agsafeset(g1, "rank", "same", "same");
    Agraph_t *g2 = agsubg(G, "cluster_2", 1);
    agsafeset(g2, "rank", "same", "same");

    Agnode_t *n1[num_nodes];
    Agnode_t *n2[num_nodes];

    int i1 = 0, i2 = 0;
    for (int i = 0; i < num_nodes; i++)
    {
        double r = drand48();
        char tmp[4];
        sprintf(tmp, "%d", i); 
        if (r < .5)
        {
            n1[i1] = agnode(g1, tmp, 1);
            agsafeset(n1[i1], "fillcolor", "crimson", "white");
            agsafeset(n1[i1], "style", "filled", "filled");
            i1++;
        }
        else
        {
            n2[i2] = agnode(g2, tmp, 1);
            agsafeset(n2[i2], "fillcolor", "darkorchid", "white");
            agsafeset(n2[i2], "style", "filled", "filled");
            i2++;
        }
    }

    for (int i = 0; i < i1; i++)
    {
        for (int j = 0; j < i2; j++)
        {
            agedge(G, n1[i], n2[j], NULL, 1);
            agedge(G, n2[j], n1[i], NULL, 1);
        }
    }
    return G;
}

// create a tree with # of levels specificed by levels
// calls a recursive function to create
Agraph_t *create_tree(char *name, Agdesc_t type, int levels)
{
    Agraph_t *G = agopen(name, type, 0);
    char tmp[4];
    int index = 0;
    sprintf(tmp, "%d", index);
    index++;
    Agnode_t *root = agnode(G, tmp, 1);

    tree_recurse(G, root, levels-1, &index);

    return G;
}

// recursive function to create the rest of the tree
Agraph_t *tree_recurse(Agraph_t *G, Agnode_t *root, int level, int *index)
{
    if (level == 0)
        return G;
    char tmp[8];
    sprintf(tmp, "%d", *index);
    (*index)++;
    Agnode_t *c1 = agnode(G, tmp, 1);
    agedge(G, root, c1, NULL, 1);
    G = tree_recurse(G, c1, level-1, index);

    sprintf(tmp, "%d", *index);
    (*index)++;
    Agnode_t *c2 = agnode(G, tmp, 1);
    agedge(G, root, c2, NULL, 1);
    G = tree_recurse(G, c2, level-1, index);
    return G;
}

// create a square grid
Agraph_t *create_square(char *name, Agdesc_t type, int num_nodes)
{
    Agraph_t *G = agopen(name, type, 0);
    Agnode_t *nodes[num_nodes][num_nodes];
    // create nodes
    for (int i = 0; i < num_nodes; i++)
    {
        char tmp[16];
        sprintf(tmp, "level_%d", i);
        Agraph_t *sub = agsubg(G, tmp, 1);
        agsafeset(sub, "rank", "same", "same");
        for (int j = 0; j < num_nodes; j++)
        {
            sprintf(tmp, "%d_%d", i, j);
            nodes[i][j] = agnode(sub, tmp, 1);
        }
    }

    // create edges
    for (int i = 0; i < num_nodes; i++)
    {
        for (int j = 0; j < num_nodes; j++)
        {
            if (j < num_nodes-1)
                agedge(G, nodes[i][j], nodes[i][j+1], NULL, 1);
            if (i < num_nodes-1)
                agedge(G, nodes[i][j], nodes[i+1][j], NULL, 1);
        }
    }
    return G;
}
