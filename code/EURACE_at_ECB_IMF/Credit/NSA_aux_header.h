/* Network Science Agent: aux functions
 */

//Defines for NR_FIRMS, NR_BANKS
//Used in: NSA_agent_compute_adjacency_matrix()
#define ROWS 80
#define COLS 20

void initialize_network(void);

int find_node_index(int id);
int find_node_index_of_friend(int f, int h);

/* NSA Network Statistics Function */
void network_structure_consistency_check(void);
void cuttlefish_data(void);
void print_adjacency_matrix_data(int A[][COLS], double B[][COLS], int nr_rows, int nr_cols);

void clustering_coefficient(void);
void homophily_index(void);
void degree_distribution(void);
void path_length(void);
