#include "Graph.hpp"
#include "defines.hpp"

using namespace std;

Graph::Graph(ifstream& instance, bool directed, bool weighted_edges, bool weighted_nodes)
    : _number_of_nodes(0), _number_of_edges(0), _directed(directed),
      _weighted_edges(weighted_edges), _weighted_nodes(weighted_nodes), _first(nullptr), _last(nullptr)
{
    // Lï¿½ o nï¿½mero de nï¿½s
    size_t num_nodes;
    instance >> num_nodes;

    // Lï¿½ as arestas entre os nï¿½s
    size_t node_id_1, node_id_2;
    float weight;

    for (size_t i = 1; i <= num_nodes; i++) {
        add_node(i, weight); // Adiciona o nï¿½
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
    // Verifica se o grafo estï¿½ vazio
    if (_first == nullptr) return;

    Node* current = _first;
    Node* previous = nullptr;

    // Localiza o nï¿½ a ser removido
    while (current != nullptr && current->_id != node_id) {
        previous = current;
        current = current->_next_node;
    }

    if (current == nullptr) return; // Nï¿½ nï¿½o encontrado

    // Percorre todos os nï¿½s para remover as arestas que tï¿½m como alvo o nï¿½ a ser removido
    Node* node_to_check = _first;
    while (node_to_check != nullptr) {
        remove_edge(node_to_check->_id, node_id);
        node_to_check = node_to_check->_next_node;
    }

    // Remove todas as arestas conectadas ao nï¿½ a ser removido
    while (current->_first_edge != nullptr) {
        Edge* temp = current->_first_edge;
        current->_first_edge = current->_first_edge->_next_edge;
        delete temp;
    }

    // Ajusta os ponteiros para remover o nï¿½ da lista
    if (previous != nullptr) {
        previous->_next_node = current->_next_node;
    } else {
        _first = current->_next_node; // Caso o nï¿½ removido seja o primeiro
    }

    if (current->_next_node != nullptr) {
        current->_next_node->_previous_node = previous;
    } else {
        _last = previous; // Caso o nï¿½ removido seja o ï¿½ltimo
    }

    // Diminui o nï¿½mero de nï¿½s
    _number_of_nodes--;

    // Libera a memï¿½ria do nï¿½ removido
    delete current;
}

void Graph::remove_edge(size_t node_id_1, size_t node_id_2)
{
    Node* node_1 = _first;

    // Encontra o nï¿½ de origem
    while (node_1 != nullptr && node_1->_id != node_id_1) {
        node_1 = node_1->_next_node;
    }

    if (node_1 == nullptr) return; // Nï¿½ nï¿½o encontrado

    Edge* current_edge = node_1->_first_edge;
    Edge* previous_edge = nullptr;

    // Encontra a aresta a ser removida
    while (current_edge != nullptr && current_edge->_target_id != node_id_2) {
        previous_edge = current_edge;
        current_edge = current_edge->_next_edge;
    }

    if (current_edge == nullptr) return; // Aresta nï¿½o encontrada

    // Remove a aresta
    if (previous_edge != nullptr) {
        previous_edge->_next_edge = current_edge->_next_edge;
    } else {
        node_1->_first_edge = current_edge->_next_edge; // Caso seja a primeira aresta
    }

    // Diminui o nï¿½mero de arestas do nï¿½ e do grafo
    node_1->_number_of_edges--;
    _number_of_edges--;

    // Libera a memï¿½ria da aresta removida
    delete current_edge;

    // Se o grafo nï¿½o for direcionado, remove a aresta oposta
    if (!_directed) {
        Node* node_2 = _first;

        // Encontra o nï¿½ de destino
        while (node_2 != nullptr && node_2->_id != node_id_2) {
            node_2 = node_2->_next_node;
        }

        if (node_2 == nullptr) return; // Nï¿½ de destino nï¿½o encontrado

        Edge* current_edge_opposite = node_2->_first_edge;
        Edge* previous_edge_opposite = nullptr;

        // Encontra a aresta oposta a ser removida
        while (current_edge_opposite != nullptr && current_edge_opposite->_target_id != node_id_1) {
            previous_edge_opposite = current_edge_opposite;
            current_edge_opposite = current_edge_opposite->_next_edge;
        }

        if (current_edge_opposite == nullptr) return; // Aresta oposta nï¿½o encontrada

        // Remove a aresta oposta
        if (previous_edge_opposite != nullptr) {
            previous_edge_opposite->_next_edge = current_edge_opposite->_next_edge;
        } else {
            node_2->_first_edge = current_edge_opposite->_next_edge; // Caso seja a primeira aresta
        }

        // Diminui o nï¿½mero de arestas do nï¿½ de destino
        node_2->_number_of_edges--;

        // Libera a memï¿½ria da aresta oposta removida
        delete current_edge_opposite;
    }
}

Node* Graph::find_node(size_t node_id)
{
    Node* current = _first;
    while (current != nullptr) {
        if (current->_id == node_id) {
            return current; // Nï¿½ encontrado
        }
        current = current->_next_node;
    }
    return nullptr; // Nï¿½ nï¿½o encontrado
}

void Graph::add_node(size_t node_id, float weight)
{
    // Verifica se o nï¿½ jï¿½ existe
    if (find_node(node_id) != nullptr) {
        return; // Nï¿½ jï¿½ existe, nï¿½o faz nada
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
    new_node->_previous_node = _last; // O ï¿½ltimo nï¿½ existente se torna o anterior

    if (_first == nullptr) {
        // Se o grafo estiver vazio, este nï¿½ ï¿½ o primeiro
        _first = new_node;
    } else {
        // Caso contrï¿½rio, o ï¿½ltimo nï¿½ existente aponta para o novo nï¿½
        _last->_next_node = new_node;
    }

    // Atualiza o ï¿½ltimo nï¿½
    _last = new_node;

    // Incrementa o nï¿½mero de nï¿½s
    _number_of_nodes++;
}

void Graph::add_edge(size_t node_id_1, size_t node_id_2, float weight)
{
    // Encontrar o nï¿½ de origem
    Node* node_1 = _first;
    while (node_1 != nullptr && node_1->_id != node_id_1) {
        node_1 = node_1->_next_node;
    }

    if (node_1 == nullptr) return; // Nï¿½ de origem nï¿½o encontrado

    // Criar uma nova aresta para o nï¿½ de origem
    Edge* new_edge_1 = new Edge;
    new_edge_1->_weight = weight;
    new_edge_1->_target_id = node_id_2;
    new_edge_1->_next_edge = node_1->_first_edge;
    node_1->_first_edge = new_edge_1;

    // Incrementar o nï¿½mero de arestas do nï¿½ de origem e do grafo
    node_1->_number_of_edges++;
    _number_of_edges++;

    if (!_directed) {
        // Se o grafo nï¿½o ï¿½ direcionado, adicionar a aresta inversa
        Node* node_2 = _first;
        while (node_2 != nullptr && node_2->_id != node_id_2) {
            node_2 = node_2->_next_node;
        }

        if (node_2 == nullptr) return; // Nï¿½ de destino nï¿½o encontrado

        // Criar uma nova aresta para o nï¿½ de destino
        Edge* new_edge_2 = new Edge;
        new_edge_2->_weight = weight;
        new_edge_2->_target_id = node_id_1;
        new_edge_2->_next_edge = node_2->_first_edge;
        node_2->_first_edge = new_edge_2;

        // Incrementar o nï¿½mero de arestas do nï¿½ de destino
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

void Graph::print_graph(){
    for (Node* current_node = _first; current_node != nullptr; current_node = current_node->_next_node) {

        std::cout << "Node " << current_node->_id << " (" << current_node->_weight << "): ";

        for (Edge* current_edge = current_node->_first_edge; current_edge != nullptr; current_edge = current_edge->_next_edge) {
            std::cout << " -> " << current_edge->_target_id << " (" << current_edge->_weight << ")";
        }

        std::cout << std::endl;
    }
}

int Graph::conected(size_t node_id_1, size_t node_id_2){
    Node* node_1 = _first;

    while (node_1 != nullptr && node_1->_id != node_id_1) {
        node_1 = node_1->_next_node;
    }

    if (node_1 == nullptr) return 0; // Nï¿½ nï¿½o encontrado

    Edge* current_edge = node_1->_first_edge;
    while (current_edge != nullptr) {
        if (current_edge->_target_id == node_id_2) {
            return 1; // Conectado
        }
        current_edge = current_edge->_next_edge;
    }

    return 0; // Nï¿½o conectado
}

bool Graph::ehConexo(vector<size_t> &vertices){
    if (vertices.empty()) return false;

    vector<bool> visitado(vertices.size(), false);
    vector<size_t> fila;
    vector<size_t> mapa_indices(vertices.size());

    // Mapeia os vÃ©rtices para Ã­ndices contÃ­nuos
    for (size_t i = 0; i < vertices.size(); ++i) {
        mapa_indices[i] = vertices[i];
    }

    // Adiciona o primeiro vÃ©rtice Ã  fila
    fila.push_back(vertices[0]);
    size_t indice_inicio = 0;
    visitado[indice_inicio] = true;

    // Processa a fila
    while (!fila.empty()) {
        size_t atual = fila.front();  // Pega o primeiro elemento da fila
        fila.erase(fila.begin());     // Remove o primeiro elemento da fila

        Node* noAtual = find_node(atual);
        if (noAtual == nullptr)
            continue;

        // Visita os vizinhos
        for (Edge* aresta = noAtual->_first_edge; aresta != nullptr; aresta = aresta->_next_edge) {
            size_t alvo = aresta->_target_id;
            auto it = find(mapa_indices.begin(), mapa_indices.end(), alvo);
            if (it != mapa_indices.end()) {
                size_t indice_destino = distance(mapa_indices.begin(), it);
                if (!visitado[indice_destino]) {
                    visitado[indice_destino] = true;
                    fila.push_back(alvo);  // Adiciona o novo vÃ©rtice a fila
                }
            }
        }
    }

    // Verifica se todos os vÃ©rtices foram visitados
    for (size_t i = 0; i < visitado.size(); ++i) {
    if (!visitado[i])
        return false;
    }
    return true;
}

Graph* Graph::arvoreGeradoraMinKruskal(vector<size_t>& vertices) {
    if (_directed) {
        cout << endl << "So funciona para grafos nao direcionados" << endl;
        return nullptr;
    }

    if (!ehConexo(vertices)) {
        cout << endl << "Subgrafo induzido pelos vertices {";
        for (size_t i = 0; i < vertices.size(); ++i) {
            cout << vertices[i];
            if (i < vertices.size() - 1) cout << ", ";
        }
        cout << "} nao eh conexo" << endl;
        return nullptr;
    }

    Graph* arvGeradoraMinKruskal = new Graph();
    vector<size_t> parent(_number_of_nodes); // Ajuste o tamanho conforme necessÃ¡rio
    vector<size_t> rank(_number_of_nodes); // Ajuste o tamanho conforme necessÃ¡rio

    for (size_t i = 0; i < vertices.size(); ++i) {
        size_t vertex = vertices[i];
        parent[vertex] = vertex;
        rank[vertex] = 0;
        arvGeradoraMinKruskal->add_node(vertex, 0.0);
    }

    vector<tuple<float, size_t, size_t>> edges;

    for (Node* node = _first; node != nullptr; node = node->_next_node) {
        if (find(vertices.begin(), vertices.end(), node->_id) == vertices.end()) {
            continue;
        }
        for (Edge* edge = node->_first_edge; edge != nullptr; edge = edge->_next_edge) {
            if (find(vertices.begin(), vertices.end(), edge->_target_id) == vertices.end()) {
                continue;
            }
            edges.push_back(make_tuple(edge->_weight, node->_id, edge->_target_id));
        }
    }

    // Ordena as arestas pelo peso
    sort(edges.begin(), edges.end(), [](const tuple<float, size_t, size_t>& a, const tuple<float, size_t, size_t>& b) {
        return get<0>(a) < get<0>(b);
    });

    for (size_t i = 0; i < edges.size(); ++i) {
        float weight = get<0>(edges[i]);
        size_t u = get<1>(edges[i]);
        size_t v = get<2>(edges[i]);

        if (encontrar_pai(u, parent) != encontrar_pai(v, parent)) {
            arvGeradoraMinKruskal->add_edge(u, v, weight);
            unir_conjuntos(u, v, parent, rank);
        }
    }

    return arvGeradoraMinKruskal;
}

void Graph::print_arvoreGeradoraMinima(Graph *arvGeradoraMin) {
    if (!arvGeradoraMin) {
        cout << "Arvore Geradora Minima nao existe." << endl;
        return;
    }
    cout << endl;
    // Itera sobre todos os nÃ³s e suas arestas para imprimir
    for (Node* node = arvGeradoraMin->_first; node != nullptr; node = node->_next_node) {
        Edge* edge = node->_first_edge;
        while (edge != nullptr) {
            // Garante que a aresta nÃ£o seja impressa duas vezes
            if (edge->_target_id > node->_id) {
                cout << "Aresta: " << node->_id << " - " << edge->_target_id
                     << " | Peso: " << edge->_weight << endl;
            }
            edge = edge->_next_edge;
        }
    }
}

void Graph::buscaProfundidade(size_t vertice, vector<size_t>& resultado, vector<bool>& visitado) {
     if (visitado[vertice]) return;

    // Marca o vÃ©rtice como visitado
    visitado[vertice] = true;
    // Adiciona o vÃ©rtice ao resultado
    resultado.push_back(vertice);

    // Encontra o nÃ³ atual no grafo
    Node* node = find_node(vertice);
    if (!node) return;

    // Percorre todas as arestas conectadas ao nÃ³ atual
    for (Edge* edge = node->_first_edge; edge != nullptr; edge = edge->_next_edge) {
        size_t vizinho = edge->_target_id;

        // Se o vizinho ainda nÃ£o foi visitado, realiza a busca em profundidade nele
        if (!visitado[vizinho]) {
            buscaProfundidade(vizinho, resultado, visitado);
        }
    }
}

vector<size_t> Graph::get_fechoTransitivoIndireto(size_t vertice_inicio) {
    if (!_directed) {
        cout << "Nao eh possivel obter o fecho transitivo indireto de um grafo nao direcionado." << endl;
        return {};
    }

    // Criar um vetor para armazenar o fecho transitivo indireto do vÃ©rtice inicial
    vector<size_t> fecho_transitivo_indireto;
    // Criar um vetor para acompanhar os vÃ©rtices jÃ¡ visitados
    vector<bool> visitado(_number_of_nodes + 1, false);

    // Realiza a busca em profundidade considerando a inversÃ£o das arestas
    buscaProfundidadeInvertida(vertice_inicio, fecho_transitivo_indireto, visitado);

    return fecho_transitivo_indireto;
}

void Graph::buscaProfundidadeInvertida(size_t vertice, vector<size_t>& resultado, vector<bool>& visitado) {
    if (visitado[vertice]) return;

    // Marca o vÃ©rtice como visitado
    visitado[vertice] = true;
    // Adiciona o vÃ©rtice ao resultado
    resultado.push_back(vertice);

    // Encontra o nÃ³ atual no grafo
    Node* node = find_node(vertice);
    if (!node) return;

    // Percorre todos os vÃ©rtices no grafo e verifica se existe uma aresta que seria invertida
    for (Node* potencial_vizinho = _first; potencial_vizinho != nullptr; potencial_vizinho = potencial_vizinho->_next_node) {
        for (Edge* edge = potencial_vizinho->_first_edge; edge != nullptr; edge = edge->_next_edge) {
            if (edge->_target_id == vertice) { // Verifica se a aresta seria invertida
                size_t vizinho = potencial_vizinho->_id;

                // Se o vizinho ainda nÃ£o foi visitado, realiza a busca em profundidade nele
                if (!visitado[vizinho]) {
                    buscaProfundidadeInvertida(vizinho, resultado, visitado);
                }
            }
        }
    }
}

void Graph::min_path_dijkstra(size_t node_id_1, size_t node_id_2){
    float INFINITO = 1e10;
    Node* no_atual = find_node(node_id_1);
    if (!no_atual) {
        cout << "No de origem nao encontrado." << endl;
        return; // Se o nó de origem não existir, saímos
    }

    Node* no_final = find_node(node_id_2);
    if (!no_final) {
        cout << "No de destino nao encontrado." << endl;
        return; // Se o nó de destino não existir, saímos
    }

    // Inicialização
    vector<float> estimativa(_number_of_nodes, INFINITO);
    vector<int> precedente(_number_of_nodes, -1);
    vector<bool> visitado(_number_of_nodes, false);

    estimativa[node_id_1] = 0;

    while (true){
        // Encontrar o nó com a menor estimativa que não foi visitado
        float menor_distancia = INFINITO;
        size_t u = (size_t) -1;
        for (size_t i = 0; i < _number_of_nodes; ++i){
            if (!visitado[i] && estimativa[i] < menor_distancia){
                menor_distancia = estimativa[i];
                u = i;
            }
        }

        if (u == (size_t)-1 || u == node_id_2) break; // Se todos os nós foram visitados ou encontramos o destino

        no_atual = find_node(u);
        if (!no_atual) {
            cout << "Erro ao encontrar o no " << u << "." << endl;
            return; // Verificação de erro
        }

        visitado[u] = true;

        // Processa as arestas do nó atual
        for (Edge* aresta = no_atual->_first_edge; aresta != nullptr; aresta = aresta->_next_edge){
            size_t v = aresta->_target_id;
            if (v >= _number_of_nodes) {
                cout << "ID de destino " << v << " esta fora do limite." << endl;
                continue; // Verificação adicional
            }

            float nova_estimativa = estimativa[u] + aresta->_weight;

            if (nova_estimativa < estimativa[v]){
                estimativa[v] = nova_estimativa;
                precedente[v] = u;
            }
        }
    }

    // Imprimir o caminho mínimo
    vector<size_t> caminho;
    for (size_t i = node_id_2; i != (size_t)-1; i = precedente[i])
    {
        caminho.push_back(i);
        if (i == node_id_1) break;
    }
    reverse(caminho.begin(), caminho.end()); // Inverte o caminho para exibir corretamente

    // Exibe o caminho e a distância total
    cout << "Caminho minimo: ";
    for (size_t i : caminho) {
        cout << i << " ";
    }
    cout << "\nDistancia total: " << estimativa[node_id_2] << endl;
}

void Graph::min_path_floyd(size_t node_id_1, size_t node_id_2) {
     // Contar o número de vértices
    size_t numVertices = this->_number_of_nodes;
    float infinity = 1e10;
    // Inicializar a matriz de distâncias
    vector<vector<float>> L(numVertices, vector<float>(numVertices, infinity));

    // Preencher a matriz com os pesos das arestas
    for (Node* node = _first; node != nullptr; node = node->_next_node) {
        size_t u = node->_id;
        for (Edge* edge = node->_first_edge; edge != nullptr; edge = edge->_next_edge) {
            size_t v = edge->_target_id;
            L[u][v] = edge->_weight;
        }
    }

    // Algoritmo de Floyd-Warshall
    for (size_t k = 0; k < numVertices; ++k) {
        for (size_t i = 0; i < numVertices; ++i) {
            for (size_t j = 0; j < numVertices; ++j) {
                if (L[i][k] < infinity && L[k][j] < infinity) {
                    if (L[i][j] > L[i][k] + L[k][j]) {
                        L[i][j] = L[i][k] + L[k][j];
                    }
                }
                if (k==i) {
                    L[k][i] = 0; // Distância de um nó para ele mesmo é 0
                }
            }
        }
    }

    // Imprimir o caminho mínimo entre os dois vértices
    cout << "Caminho minimo entre " << node_id_1 << " e " << node_id_2 << " é: ";
    if (L[node_id_1][node_id_2] == infinity) {
        cout << "Não existe caminho." << endl;
    } else {
        cout << L[node_id_1][node_id_2] << endl;
    }

    for (size_t k = 1; k < numVertices; ++k) {
        for (size_t i = 1; i < numVertices; ++i) {
                cout << "[" << L[k][i] << "]";
        }
        cout << endl;
    }
}

size_t Graph::encontrar_pai(size_t v, vector<size_t>& pai) {
    // Se o vértice v não é seu próprio pai (não é o representante do conjunto)
    if (pai[v] != v) {
        // Faz uma chamada recursiva para encontrar o pai do pai de v
        pai[v] = encontrar_pai(pai[v], pai);
    }
    // Retorna o pai do conjunto ao qual v pertence
    return pai[v];
}

void Graph::unir_conjuntos(size_t a, size_t b, vector<size_t>& pai, vector<size_t>& rank) {
    // Encontra o pai dos conjuntos aos quais 'a' e 'b' pertencem
    a = encontrar_pai(a, pai);
    b = encontrar_pai(b, pai);

    // Se 'a' e 'b' pertencem a conjuntos diferentes
    if (a != b) {
        // Une os conjuntos com base na profundidade (rank)
        if (rank[a] < rank[b]) {
            swap(a, b); // Troca a com b
        }
        // Define 'a' como o pai de 'b'
        pai[b] = a;

        // Se os ranks dos conjuntos eram iguais, aumenta o rank do novo pai
        if (rank[a] == rank[b]) {
            rank[a]++;
        }
    }
}

Graph* Graph::arvoreGeradoraMinPrim(vector<size_t>& vertices) {
    if (_directed) {
        cout << endl << "So funciona para grafos nao direcionados" << endl;
        return nullptr;
    }

    if (!ehConexo(vertices)) {
        cout << endl << "Subgrafo induzido pelos vertices {";
        for (size_t i = 0; i < vertices.size(); ++i) {
            cout << vertices[i];
            if (i < vertices.size() - 1) cout << ", ";
        }
        cout << "} nao eh conexo" << endl;
        return nullptr;
    }

    // Cria um novo grafo para a árvore geradora mínima
    Graph* arvGeradoraMinPrim = new Graph();
    arvGeradoraMinPrim->_directed = false;

    // Mapeia os vértices para índices contínuos
    vector<size_t> vertex_index(vertices.size());
    vector<size_t> index_to_vertex(vertices.size());
    size_t index = 0;

    for (size_t i = 0; i < vertices.size(); ++i) {
        vertex_index[index] = vertices[i];
        index_to_vertex[index] = vertices[i];
        arvGeradoraMinPrim->add_node(vertices[i]);  // Adiciona os nós ao grafo arvGeradoraMinPrim
        ++index;
    }

    // Vetores para armazenar o nó adjacente mais próximo, os pesos das arestas e se o nó está no arvGeradoraMinPrim
    const float INF = INF; // Valor grande para representar infinito
    vector<size_t> prox(vertices.size(), -1);
    vector<float> c(vertices.size(), INF);
    vector<bool> in_arvGeradoraMinPrim(vertices.size(), false);

    // Inicializa o vetor de proximidade com um nó arbitrário
    size_t start_node = vertices[0];
    size_t start_index = 0;
    c[start_index] = 0;  // Define o peso inicial do nó de partida como 0
    prox[start_index] = start_index;
    in_arvGeradoraMinPrim[start_index] = true;

    // Encontra o menor peso de aresta conectando ao nó inicial
    Node* start_node_ptr = find_node(start_node);
    for (Edge* e = start_node_ptr->_first_edge; e != nullptr; e = e->_next_edge) {
        for (size_t i = 0; i < vertex_index.size(); ++i) {
            if (vertex_index[i] == e->_target_id) {
                size_t target_index = i;
                c[target_index] = e->_weight;
                prox[target_index] = start_index;
                break;
            }
        }
    }

    size_t num_nodes = vertices.size();
    size_t cont = 0;

    while (cont < num_nodes - 1) {
        size_t u = -1;
        size_t v = -1;
        float min_weight = INF;

        // Encontra a aresta de menor peso conectando o arvGeradoraMinPrim ao novo vértice
        for (size_t i = 0; i < vertices.size(); ++i) {
            if (!in_arvGeradoraMinPrim[i] && c[i] < min_weight) {
                min_weight = c[i];
                v = i;
                u = prox[i];
            }
        }

        if (min_weight == INF) {
            cout << "Nao ha arestas disponiveis para adicionar a arvore geradora minima" << endl;
            delete arvGeradoraMinPrim;
            return nullptr;
        }

        // Adiciona a aresta ao arvGeradoraMinPrim
        size_t vertex_u = index_to_vertex[u];
        size_t vertex_v = index_to_vertex[v];
        arvGeradoraMinPrim->add_edge(vertex_u, vertex_v, min_weight);
        in_arvGeradoraMinPrim[v] = true;

        // Atualiza o vetor 'prox' e 'c' para os nós não incluídos no arvGeradoraMinPrim
        Node* vertex_v_ptr = find_node(vertex_v);
        for (Edge* e = vertex_v_ptr->_first_edge; e != nullptr; e = e->_next_edge) {
            for (size_t i = 0; i < vertex_index.size(); ++i) {
                if (vertex_index[i] == e->_target_id) {
                    size_t target_index = i;
                    if (!in_arvGeradoraMinPrim[target_index] && e->_weight < c[target_index]) {
                        c[target_index] = e->_weight;
                        prox[target_index] = v;
                    }
                    break;
                }
            }
        }

        ++cont;
    }

    return arvGeradoraMinPrim;
}

vector<size_t> Graph::get_fechoTransitivoDireto(size_t vertice_inicio)
{
   if (_directed == false) {
        cout << endl << "Nao eh possivel obter o fecho transitivo direto de um grafo nao direcionado" << endl;
        return {};
    }

    // Cria um vetor para armazenar o fecho transitivo direto do vértice inicial
    vector<size_t> fechoTransitivoDireto;

    // Cria uma fila para a busca em largura
    queue<size_t> to_visit;

    // Encontra o nó inicial no grafo com base no ID do vértice
    Node* no_inicio = find_node(vertice_inicio);
    if (!no_inicio) {
        // Se o nó inicial não for encontrado, exibe uma mensagem de erro
        cout << "Vertice inicial nao encontrado" << endl;
        // Retorna o vetor vazio, pois o vértice inicial não existe
        return fechoTransitivoDireto;
    }

    // Adiciona o vértice inicial à fila para começar a busca
    to_visit.push(vertice_inicio);
    // Adiciona o vértice inicial ao vetor do fecho transitivo direto
    fechoTransitivoDireto.push_back(vertice_inicio);

    // Enquanto houver vértices na fila para processar
    while (!to_visit.empty()) {
        // Obtém o próximo vértice da fila
        size_t atual = to_visit.front();
        // Remove o vértice da fila após obter
        to_visit.pop();

        // Encontra o nó correspondente ao vértice atual
        Node* noAtual = find_node(atual);
        if (!noAtual) {
            // Se o nó atual não for encontrado, continua para o próximo vértice
            continue;
        }

        // Percorre todas as arestas conectadas ao nó atual
        for (Edge* arestaAtual = noAtual->_first_edge; arestaAtual != nullptr; arestaAtual = arestaAtual->_next_edge) {
            // Obtém o ID do vértice vizinho a partir da aresta
            size_t vizinho = arestaAtual->_target_id;

            // Verifica se o vizinho já está no vetor do fecho transitivo direto
            bool flag = false;
            for (size_t node_id : fechoTransitivoDireto) {
                if (node_id == vizinho) {
                    // Se o vizinho já está no vetor, define a flag como verdadeira
                    flag = true;
                    break;
                }
            }

            // Se o vizinho não estiver no vetor do fecho transitivo direto
            if (!flag) {
                // Adiciona o vizinho à fila para processamento futuro
                to_visit.push(vizinho);
                // Adiciona o vizinho ao vetor do fecho transitivo direto
                fechoTransitivoDireto.push_back(vizinho);
            }
        }
    }
    // Retorna o vetor contendo o fecho transitivo direto do vértice inicial
    return fechoTransitivoDireto;
}
