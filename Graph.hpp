#ifndef GRAPH_HPP
#define GRAPH_HPP

#include "Node.hpp"
#include "defines.hpp"

using namespace std;

class Graph
{
public:
    /*Assinatura dos metodos basicos para o funcionamento da classe*/

    Graph(std::ifstream& instance, bool directed, bool weighted_edges, bool weighted_nodes);
    Graph();
    ~Graph();

    void remove_node(size_t node_id);
    void remove_edge(size_t node_id_1, size_t node_id_2);
    void add_node(size_t node_id, float weight = 0);
    void add_edge(size_t node_id_1, size_t node_id_2, float weight = 0);
    void print_graph(std::ofstream &output_file);
    void print_graph();
    Graph *readFileFirstPart(std::ifstream &input_file, int directed, int weightedEdge, int weightedNode, bool isPERT);
    int conected(size_t node_id_1, size_t node_id_2);
    void imprime_sequencia_nos();
    Node* find_node(size_t node_id);
    bool ehConexo(vector<size_t>& vertices);
    Graph* arvoreGeradoraMinKruskal(vector<size_t>& vertices);
    Graph* arvoreGeradoraMinPrim(vector<size_t>& vertices);
    vector<size_t> get_fechoTransitivoDireto(size_t vertice_inicio);
    vector<size_t> get_fechoTransitivoIndireto(size_t vertice_inicio);
    void buscaProfundidade(size_t vertice, vector<size_t>& resultado, vector<bool>& visitado);
    void buscaProfundidadeInvertida(size_t vertice, vector<size_t>& resultado, vector<bool>& visitado);
    void min_path_dijkstra(size_t node_id_1, size_t node_id_2);
    void min_path_floyd(size_t node_id_1, size_t node_id_2);
    size_t encontrar_pai(size_t v, vector<size_t>& pai);
    void print_arvoreGeradoraMinima(Graph *arvGeradoraMin);
    void unir_conjuntos(size_t a, size_t b, vector<size_t>& pai, vector<size_t>& rank);


private:
    size_t _number_of_nodes;
    size_t _number_of_edges;
    bool   _directed;
    bool   _weighted_edges;
    bool   _weighted_nodes;
    Node  *_first;
    Node  *_last;
};

#endif  //GRAPH_HPP
