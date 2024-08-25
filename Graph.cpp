#include "Graph.hpp"
#include <unordered_set>


Graph::Graph(std::ifstream& instance)
    : _number_of_nodes(0), _number_of_edges(0), _directed(false),
      _weighted_edges(false), _weighted_nodes(false), _first(nullptr), _last(nullptr)
{
    // Lê o número de nós
    size_t num_nodes;
    instance >> num_nodes;

    // Lê as arestas entre os nós
    size_t node_id_1, node_id_2;
    float weight;
    while (instance >> node_id_1 >> node_id_2 >> weight) {
        add_node(node_id_1); // Adiciona o nó de origem, se não existir
        add_node(node_id_2); // Adiciona o nó de destino, se não existir
        add_edge(node_id_1, node_id_2, weight); // Adiciona a aresta
    }
}

Graph::Graph()
{
}

Graph::~Graph()
{
}

void Graph::remove_node(size_t node_id)
{
    Node* current = _first;
    Node* previous = nullptr;

    // Localiza o nó a ser removido
    while (current != nullptr && current->_id != node_id) {
        previous = current;
        current = current->_next_node;
    }

    if (current == nullptr) return; // Nó não encontrado

    // Remove todas as arestas conectadas ao nó
    while (current->_first_edge != nullptr) {
        Edge* temp = current->_first_edge;
        current->_first_edge = current->_first_edge->_next_edge;
        delete temp;
    }

    // Ajusta os ponteiros do nó anterior e do próximo nó
    if (previous != nullptr) {
        previous->_next_node = current->_next_node;
    } else {
        _first = current->_next_node; // Caso o nó removido seja o primeiro
    }

    if (current->_next_node != nullptr) {
        current->_next_node->_previous_node = previous;
    } else {
        _last = previous; // Caso o nó removido seja o último
    }

    // Diminui o número de nós
    _number_of_nodes--;

    // Libera a memória do nó removido
    delete current;
}

void Graph::remove_edge(size_t node_id_1, size_t node_id_2)
{
    Node* node_1 = _first;

    // Encontra o nó de origem
    while (node_1 != nullptr && node_1->_id != node_id_1) {
        node_1 = node_1->_next_node;
    }

    if (node_1 == nullptr) return; // Nó não encontrado

    Edge* current_edge = node_1->_first_edge;
    Edge* previous_edge = nullptr;

    // Encontra a aresta a ser removida
    while (current_edge != nullptr && current_edge->_target_id != node_id_2) {
        previous_edge = current_edge;
        current_edge = current_edge->_next_edge;
    }

    if (current_edge == nullptr) return; // Aresta não encontrada

    // Remove a aresta
    if (previous_edge != nullptr) {
        previous_edge->_next_edge = current_edge->_next_edge;
    } else {
        node_1->_first_edge = current_edge->_next_edge; // Caso seja a primeira aresta
    }

    // Diminui o número de arestas do nó e do grafo
    node_1->_number_of_edges--;
    _number_of_edges--;

    // Libera a memória da aresta removida
    delete current_edge;
}

void Graph::add_node(size_t node_id, float weight)
{
    Node* new_node = new Node;
    new_node->_id = node_id;
    new_node->_weight = weight;
    new_node->_number_of_edges = 0;
    new_node->_first_edge = nullptr;
    new_node->_next_node = nullptr;
    new_node->_previous_node = _last; // O último nó existente se torna o anterior

    if (_first == nullptr) {
        // Se o grafo estiver vazio, este nó é o primeiro
        _first = new_node;
    } else {
        // Caso contrário, o último nó existente aponta para o novo nó
        _last->_next_node = new_node;
    }

    // Atualiza o último nó
    _last = new_node;

    // Incrementa o número de nós
    _number_of_nodes++;
}

void Graph::add_edge(size_t node_id_1, size_t node_id_2, float weight)
{
    // Encontrar o nó de origem
    Node* node_1 = _first;
    while (node_1 != nullptr && node_1->_id != node_id_1) {
        node_1 = node_1->_next_node;
    }

    if (node_1 == nullptr) return; // Nó de origem não encontrado

    // Criar uma nova aresta para o nó de origem
    Edge* new_edge_1 = new Edge;
    new_edge_1->_weight = weight;
    new_edge_1->_target_id = node_id_2;
    new_edge_1->_next_edge = node_1->_first_edge;
    node_1->_first_edge = new_edge_1;

    // Incrementar o número de arestas do nó de origem e do grafo
    node_1->_number_of_edges++;
    _number_of_edges++;

    if (!_directed) {
        // Se o grafo não é direcionado, adicionar a aresta inversa
        Node* node_2 = _first;
        while (node_2 != nullptr && node_2->_id != node_id_2) {
            node_2 = node_2->_next_node;
        }

        if (node_2 == nullptr) return; // Nó de destino não encontrado

        // Criar uma nova aresta para o nó de destino
        Edge* new_edge_2 = new Edge;
        new_edge_2->_weight = weight;
        new_edge_2->_target_id = node_id_1;
        new_edge_2->_next_edge = node_2->_first_edge;
        node_2->_first_edge = new_edge_2;

        // Incrementar o número de arestas do nó de destino
        node_2->_number_of_edges++;
        _number_of_edges++;
    }
}


void Graph::print_graph()
{
    std::unordered_set<size_t> printed_nodes;

    for (Node* current_node = _first; current_node != nullptr; current_node = current_node->_next_node) {
        if (printed_nodes.find(current_node->_id) != printed_nodes.end()) continue;

        printed_nodes.insert(current_node->_id);

        std::cout << "Node " << current_node->_id << " (" << current_node->_weight << "): ";

        std::unordered_set<size_t> printed_edges;
        for (Edge* current_edge = current_node->_first_edge; current_edge != nullptr; current_edge = current_edge->_next_edge) {
            if (printed_edges.find(current_edge->_target_id) != printed_edges.end()) continue;

            std::cout << " -> " << current_edge->_target_id << " (" << current_edge->_weight << ")";
            printed_edges.insert(current_edge->_target_id);
        }

        std::cout << std::endl;
    }
}


int Graph::conected(size_t node_id_1, size_t node_id_2)
{
    Node* node_1 = _first;

    while (node_1 != nullptr && node_1->_id != node_id_1) {
        node_1 = node_1->_next_node;
    }

    if (node_1 == nullptr) return 0; // Nó não encontrado

    Edge* current_edge = node_1->_first_edge;
    while (current_edge != nullptr) {
        if (current_edge->_target_id == node_id_2) {
            return 1; // Conectado
        }
        current_edge = current_edge->_next_edge;
    }

    return 0; // Não conectado
}
