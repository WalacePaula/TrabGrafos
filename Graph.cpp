#include "Graph.hpp"

Graph::Graph(std::ifstream& instance, bool directed, bool weighted_edges, bool weighted_nodes)
    : _number_of_nodes(0), _number_of_edges(0), _directed(directed),
      _weighted_edges(weighted_edges), _weighted_nodes(weighted_nodes), _first(nullptr), _last(nullptr)
{
    // Lê o número de nós
    size_t num_nodes;
    instance >> num_nodes;

    // Lê as arestas entre os nós
    size_t node_id_1, node_id_2;
    float weight;

    for (size_t i = 1; i <= num_nodes; i++) {
        add_node(i, weight); // Adiciona o nó
    }
    while (instance >> node_id_1 >> node_id_2 >> weight) {
        add_edge(node_id_1, node_id_2, weight); // Adiciona a aresta
    }
}

Graph::Graph()
    : _number_of_nodes(0), _number_of_edges(0), _directed(false),
    _weighted_edges(false), _weighted_nodes(false), _first(nullptr), _last(nullptr){
}

Graph::~Graph()
{
}

void Graph::remove_node(size_t node_id)
{
    // Verifica se o grafo está vazio
    if (_first == nullptr) return;

    Node* current = _first;
    Node* previous = nullptr;

    // Localiza o nó a ser removido
    while (current != nullptr && current->_id != node_id) {
        previous = current;
        current = current->_next_node;
    }

    if (current == nullptr) return; // Nó não encontrado

    // Percorre todos os nós para remover as arestas que têm como alvo o nó a ser removido
    Node* node_to_check = _first;
    while (node_to_check != nullptr) {
        remove_edge(node_to_check->_id, node_id);
        node_to_check = node_to_check->_next_node;
    }

    // Remove todas as arestas conectadas ao nó a ser removido
    while (current->_first_edge != nullptr) {
        Edge* temp = current->_first_edge;
        current->_first_edge = current->_first_edge->_next_edge;
        delete temp;
    }

    // Ajusta os ponteiros para remover o nó da lista
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

    // Se o grafo não for direcionado, remove a aresta oposta
    if (!_directed) {
        Node* node_2 = _first;

        // Encontra o nó de destino
        while (node_2 != nullptr && node_2->_id != node_id_2) {
            node_2 = node_2->_next_node;
        }

        if (node_2 == nullptr) return; // Nó de destino não encontrado

        Edge* current_edge_opposite = node_2->_first_edge;
        Edge* previous_edge_opposite = nullptr;

        // Encontra a aresta oposta a ser removida
        while (current_edge_opposite != nullptr && current_edge_opposite->_target_id != node_id_1) {
            previous_edge_opposite = current_edge_opposite;
            current_edge_opposite = current_edge_opposite->_next_edge;
        }

        if (current_edge_opposite == nullptr) return; // Aresta oposta não encontrada

        // Remove a aresta oposta
        if (previous_edge_opposite != nullptr) {
            previous_edge_opposite->_next_edge = current_edge_opposite->_next_edge;
        } else {
            node_2->_first_edge = current_edge_opposite->_next_edge; // Caso seja a primeira aresta
        }

        // Diminui o número de arestas do nó de destino
        node_2->_number_of_edges--;

        // Libera a memória da aresta oposta removida
        delete current_edge_opposite;
    }
}

Node* Graph::find_node(size_t node_id)
{
    Node* current = _first;
    while (current != nullptr) {
        if (current->_id == node_id) {
            return current; // Nó encontrado
        }
        current = current->_next_node;
    }
    return nullptr; // Nó não encontrado
}

void Graph::add_node(size_t node_id, float weight)
{
    // Verifica se o nó já existe
    if (find_node(node_id) != nullptr) {
        return; // Nó já existe, não faz nada
    }

    if(!_weighted_nodes){
        weight = 0;
    }

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

void print_graph(std::ofstream &output_file)
{}

void Graph::imprime_sequencia_nos()
{   Node* no_atual = _first;
    while(no_atual != nullptr){
        std::cout << no_atual->_id << " --> " << std::ends;
        no_atual = no_atual->_next_node;
    }
    std::cout << "null" << std::endl;
}


void Graph::print_graph()
{
    for (Node* current_node = _first; current_node != nullptr; current_node = current_node->_next_node) {

        std::cout << "Node " << current_node->_id << " (" << current_node->_weight << "): ";

        for (Edge* current_edge = current_node->_first_edge; current_edge != nullptr; current_edge = current_edge->_next_edge) {
            std::cout << " -> " << current_edge->_target_id << " (" << current_edge->_weight << ")";
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
