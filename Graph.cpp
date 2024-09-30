#include "Graph.hpp"
#include "defines.hpp"

using namespace std;

Graph::Graph(ifstream& instance, bool directed, bool weighted_edges, bool weighted_nodes)
    : _number_of_nodes(0), _number_of_edges(0), _directed(directed),
      _weighted_edges(weighted_edges), _weighted_nodes(weighted_nodes), _first(nullptr), _last(nullptr), _p(0)
{
    string line;

    // Lê o parâmetro p
    while (getline(instance, line)) {
        // Ignora linhas que são vazias ou começam com '#'
        if (line.empty() || line[0] == '#') {
            continue;
        }

        if (line.find("param p :=") != string::npos) {
            // Encontra o índice onde o valor de p começa
            size_t pos = line.find("param p :=") + string("param p :=").length();

            // Extrai o valor e remove espaços em branco manualmente
            string value;
            for (size_t i = pos; i < line.length(); ++i) {
                if (!isspace(line[i])) {
                    value += line[i]; // Adiciona apenas caracteres não espaços
                }
            }
            istringstream iss(value);
            iss >> _p; // Lê o parâmetro p
            break; // Sai do loop ao encontrar p
        }
    }

    // Lê o número de nós
    while (getline(instance, line)) {
        // Ignora linhas que são vazias ou não têm informações relevantes
        if (line.empty() || line[0] == '#') {
            // Verifica se a linha contém "vertici" e extrai o número
            if (line.find("vertici") != string::npos) {
                istringstream iss(line);
                string temp;
                iss >> temp;
                iss >> _number_of_nodes; // Pega o número após o prefixo
                break; // Sai do loop ao encontrar o número de nós
            }
            continue;
        }
    }

    // Lê os nós e os pesos
    bool reading_weights = false; // Flag para indicar que estamos lendo pesos
    while (getline(instance, line)) {
        // Verifica se encontramos o cabeçalho dos pesos
        if (line.find("param w :=") != string::npos) {
            reading_weights = true; // Inicia a leitura dos pesos
            continue; // Pula a linha que contém o cabeçalho
        }

        // Se estamos lendo pesos e encontramos uma linha vazia, paramos a leitura
        if (reading_weights) {
            if (line.empty()) break; // Para a leitura se a linha estiver vazia

            istringstream iss(line);
            size_t node_id;
            float weight;

            // Lê o par (id do nó, peso) e chama add_node
            while (iss >> node_id >> weight) {
                add_node(node_id, weight); // Adiciona o nó com o respectivo peso
            }
        }
    }
    // Lê as arestas
    bool reading_edges = false; // Flag para indicar que estamos lendo arestas
    while (getline(instance, line)) {
        // Verifica se encontramos o cabeçalho das arestas
        if (line.find("set E :=") != string::npos) {
            reading_edges = true; // Inicia a leitura das arestas
            continue; // Pula a linha que contém o cabeçalho
        }

        // Se estamos lendo arestas e encontramos uma linha com ";", paramos a leitura
        if (reading_edges) {
            if (line.find(";") != string::npos) break; // Finaliza ao encontrar ";"

            istringstream iss(line);
            string edge;

            // Lê cada par (nó inicial, nó final) no formato (x,y)
            while (iss >> edge) {
                // Remove os parênteses e separa pelo delimitador ','
                edge.erase(remove(edge.begin(), edge.end(), '('), edge.end());
                edge.erase(remove(edge.begin(), edge.end(), ')'), edge.end());

                // Separa os nós (nó inicial, nó final)
                istringstream edge_stream(edge);
                size_t node1, node2;
                char comma;

                edge_stream >> node1 >> comma >> node2;

                // Chama a função add_edge com os dois nós
                add_edge(node1, node2);
            }
        }
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
    // Verifica se o grafo est� vazio
    if (_first == nullptr) return;

    Node* current = _first;
    Node* previous = nullptr;

    // Localiza o n� a ser removido
    while (current != nullptr && current->_id != node_id) {
        previous = current;
        current = current->_next_node;
    }

    if (current == nullptr) return; // N� n�o encontrado

    // Percorre todos os n�s para remover as arestas que t�m como alvo o n� a ser removido
    Node* node_to_check = _first;
    while (node_to_check != nullptr) {
        remove_edge(node_to_check->_id, node_id);
        node_to_check = node_to_check->_next_node;
    }

    // Remove todas as arestas conectadas ao n� a ser removido
    while (current->_first_edge != nullptr) {
        Edge* temp = current->_first_edge;
        current->_first_edge = current->_first_edge->_next_edge;
        delete temp;
    }

    // Ajusta os ponteiros para remover o n� da lista
    if (previous != nullptr) {
        previous->_next_node = current->_next_node;
    } else {
        _first = current->_next_node; // Caso o n� removido seja o primeiro
    }

    if (current->_next_node != nullptr) {
        current->_next_node->_previous_node = previous;
    } else {
        _last = previous; // Caso o n� removido seja o �ltimo
    }

    // Diminui o n�mero de n�s
    _number_of_nodes--;

    // Libera a mem�ria do n� removido
    delete current;
}

void Graph::remove_edge(size_t node_id_1, size_t node_id_2)
{
    Node* node_1 = _first;

    // Encontra o n� de origem
    while (node_1 != nullptr && node_1->_id != node_id_1) {
        node_1 = node_1->_next_node;
    }

    if (node_1 == nullptr) return; // N� n�o encontrado

    Edge* current_edge = node_1->_first_edge;
    Edge* previous_edge = nullptr;

    // Encontra a aresta a ser removida
    while (current_edge != nullptr && current_edge->_target_id != node_id_2) {
        previous_edge = current_edge;
        current_edge = current_edge->_next_edge;
    }

    if (current_edge == nullptr) return; // Aresta n�o encontrada

    // Remove a aresta
    if (previous_edge != nullptr) {
        previous_edge->_next_edge = current_edge->_next_edge;
    } else {
        node_1->_first_edge = current_edge->_next_edge; // Caso seja a primeira aresta
    }

    // Diminui o n�mero de arestas do n� e do grafo
    node_1->_number_of_edges--;
    _number_of_edges--;

    // Libera a mem�ria da aresta removida
    delete current_edge;

    // Se o grafo n�o for direcionado, remove a aresta oposta
    if (!_directed) {
        Node* node_2 = _first;

        // Encontra o n� de destino
        while (node_2 != nullptr && node_2->_id != node_id_2) {
            node_2 = node_2->_next_node;
        }

        if (node_2 == nullptr) return; // N� de destino n�o encontrado

        Edge* current_edge_opposite = node_2->_first_edge;
        Edge* previous_edge_opposite = nullptr;

        // Encontra a aresta oposta a ser removida
        while (current_edge_opposite != nullptr && current_edge_opposite->_target_id != node_id_1) {
            previous_edge_opposite = current_edge_opposite;
            current_edge_opposite = current_edge_opposite->_next_edge;
        }

        if (current_edge_opposite == nullptr) return; // Aresta oposta n�o encontrada

        // Remove a aresta oposta
        if (previous_edge_opposite != nullptr) {
            previous_edge_opposite->_next_edge = current_edge_opposite->_next_edge;
        } else {
            node_2->_first_edge = current_edge_opposite->_next_edge; // Caso seja a primeira aresta
        }

        // Diminui o n�mero de arestas do n� de destino
        node_2->_number_of_edges--;

        // Libera a mem�ria da aresta oposta removida
        delete current_edge_opposite;
    }
}

Node* Graph::find_node(size_t node_id)
{
    Node* current = _first;
    while (current != nullptr) {
        if (current->_id == node_id) {
            return current; // N� encontrado
        }
        current = current->_next_node;
    }
    return nullptr; // N� n�o encontrado
}

void Graph::add_node(size_t node_id, float weight)
{
    // Verifica se o n� j� existe
    if (find_node(node_id) != nullptr) {
        return; // N� j� existe, n�o faz nada
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
    new_node->_previous_node = _last; // O �ltimo n� existente se torna o anterior

    if (_first == nullptr) {
        // Se o grafo estiver vazio, este n� � o primeiro
        _first = new_node;
    } else {
        // Caso contr�rio, o �ltimo n� existente aponta para o novo n�
        _last->_next_node = new_node;
    }

    // Atualiza o �ltimo n�
    _last = new_node;

    // Incrementa o n�mero de n�s
    _number_of_nodes++;
}

void Graph::add_edge(size_t node_id_1, size_t node_id_2, float weight)
{
    // Encontrar o n� de origem
    Node* node_1 = _first;
    while (node_1 != nullptr && node_1->_id != node_id_1) {
        node_1 = node_1->_next_node;
    }

    if (node_1 == nullptr) return; // N� de origem n�o encontrado

    // Criar uma nova aresta para o n� de origem
    Edge* new_edge_1 = new Edge;
    new_edge_1->_weight = weight;
    new_edge_1->_target_id = node_id_2;
    new_edge_1->_next_edge = node_1->_first_edge;
    node_1->_first_edge = new_edge_1;

    // Incrementar o n�mero de arestas do n� de origem e do grafo
    node_1->_number_of_edges++;
    _number_of_edges++;

    if (!_directed) {
        // Se o grafo n�o � direcionado, adicionar a aresta inversa
        Node* node_2 = _first;
        while (node_2 != nullptr && node_2->_id != node_id_2) {
            node_2 = node_2->_next_node;
        }

        if (node_2 == nullptr) return; // N� de destino n�o encontrado

        // Criar uma nova aresta para o n� de destino
        Edge* new_edge_2 = new Edge;
        new_edge_2->_weight = weight;
        new_edge_2->_target_id = node_id_1;
        new_edge_2->_next_edge = node_2->_first_edge;
        node_2->_first_edge = new_edge_2;

        // Incrementar o n�mero de arestas do n� de destino
        node_2->_number_of_edges++;
        _number_of_edges++;
    }
}

void print_graph(ofstream &output_file)
{}

void Graph::imprime_sequencia_nos()
{   Node* no_atual = _first;
    while(no_atual != nullptr){
        cout << no_atual->_id << " --> " << ends;
        no_atual = no_atual->_next_node;
    }
    cout << "null" << endl;
}

void Graph::print_graph(){
    for (Node* current_node = _first; current_node != nullptr; current_node = current_node->_next_node) {

        cout << "Node " << current_node->_id << " (" << current_node->_weight << "): ";

        for (Edge* current_edge = current_node->_first_edge; current_edge != nullptr; current_edge = current_edge->_next_edge) {
            cout << " -> " << current_edge->_target_id << " (" << current_edge->_weight << ")";
        }

        cout << endl;
    }
}

int Graph::conected(size_t node_id_1, size_t node_id_2){
    Node* node_1 = _first;

    while (node_1 != nullptr && node_1->_id != node_id_1) {
        node_1 = node_1->_next_node;
    }

    if (node_1 == nullptr) return 0; // N� n�o encontrado

    Edge* current_edge = node_1->_first_edge;
    while (current_edge != nullptr) {
        if (current_edge->_target_id == node_id_2) {
            return 1; // Conectado
        }
        current_edge = current_edge->_next_edge;
    }

    return 0; // N�o conectado
}

bool Graph::ehConexo(vector<size_t> &vertices){
    if (vertices.empty()) return false;

    vector<bool> visitado(vertices.size(), false);
    vector<size_t> fila;
    vector<size_t> mapa_indices(vertices.size());

    // Mapeia os vértices para índices contínuos
    for (size_t i = 0; i < vertices.size(); ++i) {
        mapa_indices[i] = vertices[i];
    }

    // Adiciona o primeiro vértice à fila
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
                    fila.push_back(alvo);  // Adiciona o novo vértice a fila
                }
            }
        }
    }

    // Verifica se todos os vértices foram visitados
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
    vector<size_t> parent(_number_of_nodes); // Ajuste o tamanho conforme necessário
    vector<size_t> rank(_number_of_nodes); // Ajuste o tamanho conforme necessário

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

string Graph::print_arvoreGeradoraMinima(Graph *arvGeradoraMin) {
    if (!arvGeradoraMin) {
        return "Arvore Geradora Minima nao existe.";
    }

    stringstream ss;
    // Itera sobre todos os n�s e suas arestas para construir a string
    for (Node* node = arvGeradoraMin->_first; node != nullptr; node = node->_next_node) {
        Edge* edge = node->_first_edge;
        while (edge != nullptr) {
            // Garante que a aresta n�o seja impressa duas vezes
            if (edge->_target_id > node->_id) {
                ss << "Aresta: " << node->_id << " - " << edge->_target_id
                   << " | Peso: " << edge->_weight << "\n";
            }
            edge = edge->_next_edge;
        }
    }

    return ss.str();
}

void Graph::buscaProfundidade(size_t vertice, vector<size_t>& resultado, vector<bool>& visitado) {
     if (visitado[vertice]) return;

    // Marca o vertice como visitado
    visitado[vertice] = true;
    // Adiciona o vertice ao resultado
    resultado.push_back(vertice);

    // Encontra o no atual no grafo
    Node* node = find_node(vertice);
    if (!node) return;

    // Percorre todas as arestas conectadas ao no atual
    for (Edge* edge = node->_first_edge; edge != nullptr; edge = edge->_next_edge) {
        size_t vizinho = edge->_target_id;

        // Se o vizinho ainda nao foi visitado, realiza a busca em profundidade nele
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

    // Criar um vetor para armazenar o fecho transitivo indireto do v�rtice inicial
    vector<size_t> fecho_transitivo_indireto;
    // Criar um vetor para acompanhar os v�rtices ja� visitados
    vector<bool> visitado(_number_of_nodes + 1, false);

    // Realiza a busca em profundidade considerando a invers�o das arestas
    buscaProfundidadeInvertida(vertice_inicio, fecho_transitivo_indireto, visitado);

    return fecho_transitivo_indireto;
}

void Graph::buscaProfundidadeInvertida(size_t vertice, vector<size_t>& resultado, vector<bool>& visitado) {
    if (visitado[vertice]) return;

    // Marca o vértice como visitado
    visitado[vertice] = true;
    // Adiciona o vértice ao resultado
    resultado.push_back(vertice);

    // Encontra o nó atual no grafo
    Node* node = find_node(vertice);
    if (!node) return;

    // Percorre todos os vértices no grafo e verifica se existe uma aresta que seria invertida
    for (Node* potencial_vizinho = _first; potencial_vizinho != nullptr; potencial_vizinho = potencial_vizinho->_next_node) {
        for (Edge* edge = potencial_vizinho->_first_edge; edge != nullptr; edge = edge->_next_edge) {
            if (edge->_target_id == vertice) { // Verifica se a aresta seria invertida
                size_t vizinho = potencial_vizinho->_id;

                // Se o vizinho ainda não foi visitado, realiza a busca em profundidade nele
                if (!visitado[vizinho]) {
                    buscaProfundidadeInvertida(vizinho, resultado, visitado);
                }
            }
        }
    }
}

string Graph::min_path_dijkstra(size_t node_id_1, size_t node_id_2) {
    float INFINITO = 1e10;
    Node* no_atual = find_node(node_id_1);
    if (!no_atual) {
        return "N� de origem n�o encontrado.";
    }

    Node* no_final = find_node(node_id_2);
    if (!no_final) {
        return "N� de destino n�o encontrado.";
    }

    // Inicializa��o
    vector<float> estimativa(_number_of_nodes, INFINITO);
    vector<int> precedente(_number_of_nodes, -1);
    vector<bool> visitado(_number_of_nodes, false);

    estimativa[node_id_1] = 0;

    while (true) {
        // Encontrar o n� com a menor estimativa que n�o foi visitado
        float menor_distancia = INFINITO;
        size_t u = (size_t) -1;
        for (size_t i = 0; i < _number_of_nodes; ++i) {
            if (!visitado[i] && estimativa[i] < menor_distancia) {
                menor_distancia = estimativa[i];
                u = i;
            }
        }

        if (u == (size_t)-1 || u == node_id_2) break; // Se todos os n�s foram visitados ou encontramos o destino

        no_atual = find_node(u);
        if (!no_atual) {
            return "Erro ao encontrar o n� " + to_string(u) + ".";
        }

        visitado[u] = true;

        // Processa as arestas do n� atual
        for (Edge* aresta = no_atual->_first_edge; aresta != nullptr; aresta = aresta->_next_edge) {
            size_t v = aresta->_target_id;
            if (v >= _number_of_nodes) {
                continue; // Verifica��o adicional
            }

            float nova_estimativa = estimativa[u] + aresta->_weight;

            if (nova_estimativa < estimativa[v]) {
                estimativa[v] = nova_estimativa;
                precedente[v] = u;
            }
        }
    }

    // Construir o caminho m�nimo
    vector<size_t> caminho;
    for (size_t i = node_id_2; i != (size_t)-1; i = precedente[i]) {
        caminho.push_back(i);
        if (i == node_id_1) break;
    }
    reverse(caminho.begin(), caminho.end());

    // Construir a string de resultado
    stringstream resultado;
    for (size_t i : caminho) {
        resultado << i << " ";
    }

    return resultado.str();
}


string Graph::min_path_floyd(size_t node_id_1, size_t node_id_2) {
     // Contar o n�mero de v�rtices
    size_t numVertices = this->_number_of_nodes +1;
    float infinity = 1e10;
    // Inicializar a matriz de dist�ncias
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
    for (size_t k = 1; k < numVertices; ++k) {
        for (size_t i = 1; i < numVertices; ++i) {
            for (size_t j = 1; j < numVertices; ++j) {
                if (L[i][k] < infinity && L[k][j] < infinity) {
                    if (L[i][j] > L[i][k] + L[k][j]) {
                        L[i][j] = L[i][k] + L[k][j];
                    }
                }
                if (k==i) {
                    L[k][i] = 0; // Dist�ncia de um n� para ele mesmo � 0
                }
            }
        }
    }

    // Criar a string do caminho m�nimo
    string caminho_minimo = to_string(node_id_1);  // Inicia o caminho com o n� de origem
    size_t atual = node_id_1;

    // Percorre o caminho m�nimo a partir da matriz de dist�ncias
    while (atual != node_id_2) {
        float menor_distancia = infinity;
        size_t proximo = (size_t)-1;

        // Encontra o pr�ximo n� no caminho m�nimo
        for (size_t i = 0; i < numVertices; ++i) {
            if (L[atual][i] < menor_distancia && i != atual) {
                menor_distancia = L[atual][i];
                proximo = i;
            }
        }

        // Adiciona o pr�ximo n� ao caminho
        if (proximo != (size_t)-1) {
            caminho_minimo += " -> " + to_string(proximo);
            atual = proximo;
        } else {
            break; // Se n�o encontrar pr�ximo n�, para o la�o
        }
    }

    return caminho_minimo;
}

size_t Graph::encontrar_pai(size_t v, vector<size_t>& pai) {
    // Se o v�rtice v n�o � seu pr�prio pai (n�o � o representante do conjunto)
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

    // Cria um novo grafo para a �rvore geradora m�nima
    Graph* arvGeradoraMinPrim = new Graph();
    arvGeradoraMinPrim->_directed = false;

    // Mapeia os v�rtices para �ndices cont�nuos
    vector<size_t> vertex_index(vertices.size());
    vector<size_t> index_to_vertex(vertices.size());
    size_t index = 0;

    for (size_t i = 0; i < vertices.size(); ++i) {
        vertex_index[index] = vertices[i];
        index_to_vertex[index] = vertices[i];
        arvGeradoraMinPrim->add_node(vertices[i]);  // Adiciona os n�s ao grafo arvGeradoraMinPrim
        ++index;
    }

    // Vetores para armazenar o n� adjacente mais pr�ximo, os pesos das arestas e se o n� est� no arvGeradoraMinPrim
    const float INF = INF; // Valor grande para representar infinito
    vector<size_t> prox(vertices.size(), -1);
    vector<float> c(vertices.size(), INF);
    vector<bool> in_arvGeradoraMinPrim(vertices.size(), false);

    // Inicializa o vetor de proximidade com um n� arbitr�rio
    size_t start_node = vertices[0];
    size_t start_index = 0;
    c[start_index] = 0;  // Define o peso inicial do n� de partida como 0
    prox[start_index] = start_index;
    in_arvGeradoraMinPrim[start_index] = true;

    // Encontra o menor peso de aresta conectando ao n� inicial
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

        // Encontra a aresta de menor peso conectando o arvGeradoraMinPrim ao novo v�rtice
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

        // Atualiza o vetor 'prox' e 'c' para os n�s n�o inclu�dos no arvGeradoraMinPrim
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

    // Cria um vetor para armazenar o fecho transitivo direto do v�rtice inicial
    vector<size_t> fechoTransitivoDireto;

    // Cria uma fila para a busca em largura
    queue<size_t> to_visit;

    // Encontra o n� inicial no grafo com base no ID do v�rtice
    Node* no_inicio = find_node(vertice_inicio);
    if (!no_inicio) {
        // Se o n� inicial n�o for encontrado, exibe uma mensagem de erro
        cout << "Vertice inicial nao encontrado" << endl;
        // Retorna o vetor vazio, pois o v�rtice inicial n�o existe
        return fechoTransitivoDireto;
    }

    // Adiciona o v�rtice inicial � fila para come�ar a busca
    to_visit.push(vertice_inicio);
    // Adiciona o v�rtice inicial ao vetor do fecho transitivo direto
    fechoTransitivoDireto.push_back(vertice_inicio);

    // Enquanto houver v�rtices na fila para processar
    while (!to_visit.empty()) {
        // Obt�m o pr�ximo v�rtice da fila
        size_t atual = to_visit.front();
        // Remove o v�rtice da fila ap�s obter
        to_visit.pop();

        // Encontra o n� correspondente ao v�rtice atual
        Node* noAtual = find_node(atual);
        if (!noAtual) {
            // Se o n� atual n�o for encontrado, continua para o pr�ximo v�rtice
            continue;
        }

        // Percorre todas as arestas conectadas ao n� atual
        for (Edge* arestaAtual = noAtual->_first_edge; arestaAtual != nullptr; arestaAtual = arestaAtual->_next_edge) {
            // Obt�m o ID do v�rtice vizinho a partir da aresta
            size_t vizinho = arestaAtual->_target_id;

            // Verifica se o vizinho j� est� no vetor do fecho transitivo direto
            bool flag = false;
            for (size_t node_id : fechoTransitivoDireto) {
                if (node_id == vizinho) {
                    // Se o vizinho j� est� no vetor, define a flag como verdadeira
                    flag = true;
                    break;
                }
            }

            // Se o vizinho n�o estiver no vetor do fecho transitivo direto
            if (!flag) {
                // Adiciona o vizinho � fila para processamento futuro
                to_visit.push(vizinho);
                // Adiciona o vizinho ao vetor do fecho transitivo direto
                fechoTransitivoDireto.push_back(vizinho);
            }
        }
    }
    // Retorna o vetor contendo o fecho transitivo direto do v�rtice inicial
    return fechoTransitivoDireto;
}

string Graph::caracteristicas() {
    size_t numVertices = this->_number_of_nodes;
    float infinity = 1e10;

    // Inicializar a matriz de dist�ncias
    float L[numVertices][numVertices];
    for (size_t i = 0; i < numVertices; ++i) {
        for (size_t j = 0; j < numVertices; ++j) {
            L[i][j] = (i == j) ? 0 : infinity; // Inicializa com 0 na diagonal e infinito nos demais
        }
    }

    // Preencher a matriz com os pesos das arestas
    for (Node* node = _first; node != nullptr; node = node->_next_node) {
        size_t u = node->_id;
        for (Edge* edge = node->_first_edge; edge != nullptr; edge = edge->_next_edge) {
            size_t v = edge->_target_id;
            L[u][v] = edge->_weight;
            if (!this->_directed) {
                L[v][u] = edge->_weight;  // Para grafos n�o direcionados
            }
        }
    }

    // Algoritmo de Floyd-Warshall para calcular o menor caminho entre todos os pares de v�rtices
    for (size_t k = 0; k < numVertices; ++k) {
        for (size_t i = 0; i < numVertices; ++i) {
            for (size_t j = 0; j < numVertices; ++j) {
                if (L[i][k] < infinity && L[k][j] < infinity) {
                    if (L[i][j] > L[i][k] + L[k][j]) {
                        L[i][j] = L[i][k] + L[k][j];
                    }
                }
            }
        }
    }

    // Vari�veis para armazenar o raio, di�metro, centro e periferia
    float raio = infinity;
    float diametro = 0;
    size_t centro[numVertices], periferia[numVertices]; // Vetores para armazenar o centro e periferia
    int countCentro = 0, countPeriferia = 0;

    for (size_t i = 0; i < numVertices; ++i) {
        float max_dist = 0;
        for (size_t j = 0; j < numVertices; ++j) {
            if (L[i][j] < infinity && L[i][j] > max_dist) {
                max_dist = L[i][j];
            }
        }

        if (max_dist < raio) {
            raio = max_dist;  // O menor dos maiores valores de dist�ncia
        }
        if (max_dist > diametro) {
            diametro = max_dist;  // O maior dos maiores valores de dist�ncia
        }
    }

    // Encontrar os v�rtices que comp�em o centro e a periferia
    for (size_t i = 0; i < numVertices; ++i) {
        float max_dist = 0;
        for (size_t j = 0; j < numVertices; ++j) {
            if (L[i][j] < infinity && L[i][j] > max_dist) {
                max_dist = L[i][j];
            }
        }

        if (max_dist == raio) {
            centro[countCentro++] = i; // Adiciona ao centro
        }
        if (max_dist == diametro) {
            periferia[countPeriferia++] = i; // Adiciona � periferia
        }
    }

    // Usar ostringstream para construir a string de sa�da
    ostringstream output;
    output << "Raio do grafo: " << raio << "\n";
    output << "Diametro do grafo: " << diametro << "\n";

    output << "Centro do grafo: ";
    for (int i = 0; i < countCentro; ++i) {
        output << centro[i] << " ";
    }
    output << "\n";

    output << "Periferia do grafo: ";
    for (int i = 0; i < countPeriferia; ++i) {
        output << periferia[i] << " ";
    }
    output << "\n";

    // Retornar a string gerada
    return output.str();
}


size_t Graph::getNum_vertices() {
    return _number_of_nodes;
}


// Função auxiliar para verificar se o nó atual está conectado a algum nó do subgrafo
bool Graph::IsConnected(Node* node, const vector<size_t>& subgrafo) {
    if (!node) return false; // Se o nó é nulo, não pode estar conectado

    for (size_t id : subgrafo) {
        Node* otherNode = find_node(id);
        if (!otherNode) continue; // Ignora se não encontrar o nó
        if (HasEdge(node, otherNode)) {
            return true;
        }
    }
    return false;
}

bool Graph::HasEdge(Node* node1, Node* node2) {
    Edge* edge = node1->_first_edge; // Começa do primeiro nó
    while (edge != nullptr) {
        if (edge->_target_id == node2->_id) { // Se o ID do nó de destino é igual ao ID do segundo nó
            return true; // Existe uma aresta entre node1 e node2
        }
        edge = edge->_next_edge; // Move para a próxima aresta
    }
    return false; // Não há aresta entre os dois nós
}

// Adicione isso à classe Graph
vector<Node*> Graph::GetNodesSortedByDegree() {
    vector<Node*> nodes;
    Node* currentNode = _first;

    // Coleta todos os nós
    while (currentNode != nullptr) {
        nodes.push_back(currentNode);
        currentNode = currentNode->_next_node;
    }

    // Ordena os nós pelo número de arestas (grau)
    std::sort(nodes.begin(), nodes.end(), [](Node* a, Node* b) {
        return a->_number_of_edges > b->_number_of_edges; // Ordenação decrescente
    });

    return nodes;
}



// Adicione isso à classe Graph
vector<Node*> Graph::GetNeighbors(const vector<size_t>& subgrafo) {
    set<Node*> neighbors; // Usamos um set para evitar duplicatas
    for (size_t node_id : subgrafo) {
        Node* node = find_node(node_id);
        if (node) {
            Edge* currentEdge = node->_first_edge;
            while (currentEdge != nullptr) {
                Node* neighborNode = find_node(currentEdge->_target_id);
                if (neighborNode) {
                    neighbors.insert(neighborNode); // Adiciona vizinhos
                }
                currentEdge = currentEdge->_next_edge;
            }
        }
    }

    // Converte o set em um vetor
    return vector<Node*>(neighbors.begin(), neighbors.end());
}



int Graph::Guloso() {
    vector<Node*> nodes = GetNodesSortedByDegree(); // Ordena os nós pelo grau (número de arestas)
    subgrafos = vector<vector<size_t>>(_p); // Vetor de subgrafos
    vector<float> minPesos(_p, numeric_limits<float>::max()); // Pesos mínimos
    vector<float> maxPesos(_p, numeric_limits<float>::min()); // Pesos máximos

    // Passo 1: Distribuir os primeiros nós (nós de maior grau) em cada subgrafo
    for (int i = 0; i < _p; ++i) {
        Node* node = nodes[i];
        subgrafos[i].push_back(node->_id); // Aloca o nó ao subgrafo i
        minPesos[i] = node->_weight; // Inicializa o peso mínimo
        maxPesos[i] = node->_weight; // Inicializa o peso máximo
        nodes.erase(nodes.begin() + i); // Remove o nó da lista de nós não alocados
    }

    // Passo 2: Expandir os subgrafos com base nos vizinhos
    while (!nodes.empty()) {
        for (int i = 0; i < _p; ++i) {
            Node* bestNode = nullptr;
            float bestGap = numeric_limits<float>::max();

            // Verificar os vizinhos dos nós já alocados ao subgrafo i
            for (Node* currentNode : GetNeighbors(subgrafos[i])) {
                if (std::find(nodes.begin(), nodes.end(), currentNode) == nodes.end()) {
                    continue; // Pular se o nó já foi alocado
                }

                // Calcular o novo gap se adicionarmos esse nó
                float newMinPeso = min(minPesos[i], currentNode->_weight);
                float newMaxPeso = max(maxPesos[i], currentNode->_weight);
                float newGap = newMaxPeso - newMinPeso;

                // Se o novo gap for menor, escolha este nó
                if (newGap < bestGap) {
                    bestNode = currentNode;
                    bestGap = newGap;
                }
            }

            // Se encontrou um nó adequado, adicioná-lo ao subgrafo
            if (bestNode != nullptr) {
                subgrafos[i].push_back(bestNode->_id);
                minPesos[i] = min(minPesos[i], bestNode->_weight);
                maxPesos[i] = max(maxPesos[i], bestNode->_weight);
                nodes.erase(std::remove(nodes.begin(), nodes.end(), bestNode), nodes.end()); // Remove o nó da lista
            }
        }
    }


    // Exibir a solução final
    cout << "Subgrafos resultantes: " << endl; // Mensagem inicial
    for (int i = 0; i < _p; ++i) {
        cout << "Subgrafo " << i << ": ["; // Inicia a impressão de cada subgrafo
        for (size_t m = 0; m < subgrafos[i].size(); ++m) {
            cout << subgrafos[i][m]; // Imprime o ID do nó no subgrafo
            if (m != subgrafos[i].size() - 1) { // Se não for o último nó
                cout << ','; // Adiciona uma vírgula
            }
        }
        cout << ']' << endl; // Fecha a lista do subgrafo
    }

    return 0; // Retorna 0 indicando que a função foi executada com sucesso
}

// VERIFICAR NOS REPETIDOS
int Graph::GulosoMelhorado(){
    vector<Node*> nodes = GetNodesSortedByDegree(); // Ordena os nós pelo grau
    subgrafos = vector<vector<size_t>>(_p);  // Inicializa os subgrafos
    vector<float> minPesos(_p, numeric_limits<float>::max());
    vector<float> maxPesos(_p, numeric_limits<float>::min());

    // Conjunto para armazenar nós alocados
    unordered_set<size_t> alocados;

    // Passo 1: Alocar os primeiros nós em cada subgrafo
    for (int i = 0; i < _p && i < nodes.size(); ++i) {
        Node* node = nodes[i];
        subgrafos[i].push_back(node->_id);
        alocados.insert(node->_id); // Adiciona o nó ao conjunto de alocados
        minPesos[i] = node->_weight;
        maxPesos[i] = node->_weight;
    }

    int contemplados = _p;  // Contabiliza os nós alocados
    cout << "Nós iniciais alocados: " << contemplados << endl;

    // Passo 2: Alocação dos próximos nós
    while (contemplados < _number_of_nodes) {
        bool noNovoAlocado = false; // Para verificar se alocamos um novo nó na iteração atual

        for (int i = 0; i < _p; ++i) {
            if (subgrafos[i].size() < (_number_of_nodes / _p) + 1) { // Verifica se ainda pode adicionar nós
                // Seleciona um nó que minimize o gap
                Node* melhorNo = nullptr;
                float menorGap = numeric_limits<float>::max();

                // Considera vizinhos dos nós já alocados
                for (size_t id : subgrafos[i]) {
                    Node* currentNode = find_node(id);
                    for (Edge* edge = currentNode->_first_edge; edge != nullptr; edge = edge->_next_edge) {
                        Node* neighborNode = find_node(edge->_target_id);
                        if (neighborNode != nullptr &&
                            alocados.find(neighborNode->_id) == alocados.end()) { // Verifica se não está alocado

                            // Calcula novo gap
                            float novoMinPeso = min(minPesos[i], neighborNode->_weight);
                            float novoMaxPeso = max(maxPesos[i], neighborNode->_weight);
                            float novoGap = novoMaxPeso - novoMinPeso;

                            // Atualiza se for o melhor nó
                            if (novoGap < menorGap) {
                                menorGap = novoGap;
                                melhorNo = neighborNode;
                            }
                        }
                    }
                }

                // Adiciona o nó escolhido ao subgrafo
                if (melhorNo != nullptr) {
                    subgrafos[i].push_back(melhorNo->_id);
                    alocados.insert(melhorNo->_id); // Adiciona ao conjunto de alocados
                    minPesos[i] = min(minPesos[i], melhorNo->_weight);
                    maxPesos[i] = max(maxPesos[i], melhorNo->_weight);

                    // Verifica conectividade
                    if (ehConexo(subgrafos[i])) {
                        contemplados++;
                        noNovoAlocado = true; // Marcamos que um novo nó foi alocado
                    } else {
                        subgrafos[i].pop_back(); // Remove se não é conexo
                        alocados.erase(melhorNo->_id); // Remove do conjunto de alocados
                    }
                }
            }
        }

        // Se não foi alocado nenhum novo nó na iteração atual, encerramos o loop
        if (!noNovoAlocado) {
            cout << "Nenhum novo nó alocado na iteração, saindo do loop." << endl;
            break;
        }
    }

    // Exibir a solução final
    cout << "Subgrafos resultantes: " << endl;
    for (int i = 0; i < _p; ++i) {
        cout << "Subgrafo " << i << ": [";
        for (size_t m = 0; m < subgrafos[i].size(); ++m) {
            cout << subgrafos[i][m];
            if (m != subgrafos[i].size() - 1) {
                cout << ',';
            }
        }
        cout << ']' << endl;
    }

    return 0;
}

// Implementação do método para obter o peso de um vértice
float Graph::obterPesoVertice(size_t id) {
    Node* node = _first;
    while (node != nullptr) {
        if (node->_id == id) {
            return node->_weight;
        }
        node = node->_next_node;
    }
    return 0; // Retorna 0 se o nó não for encontrado
}

// Função que calcula o gap de um subgrafo
float Graph::calcularGap(const std::vector<size_t>& subgrafo) {
    if (subgrafo.size() < 2) return 0; // Não calcula gap para subgrafos pequenos
    float maxPeso = std::numeric_limits<float>::min();
    float minPeso = std::numeric_limits<float>::max();

    for (size_t id : subgrafo) {
        float peso = obterPesoVertice(id);
        if (peso > maxPeso) maxPeso = peso;
        if (peso < minPeso) minPeso = peso;
    }
    return maxPeso - minPeso; // Retorna a diferença entre o maior e menor peso
}
int Graph::GulosoRandomizado(float alpha, int seed) {
    srand(time(0) + seed);

    vector<Node*> nodes;
    Node* node = _first;

    // Coletar todos os nós na lista nodes
    while (node != nullptr) {
        nodes.push_back(node);
        node = node->_next_node;
    }

    // Ordenar nós com base no número de arestas, em ordem crescente
    sort(nodes.begin(), nodes.end(), [](Node* a, Node* b) {
        return a->_number_of_edges < b->_number_of_edges;
    });

    subgrafos = vector<vector<size_t>>(_p);
    vector<float> minPesos(_p, numeric_limits<float>::max());
    vector<float> maxPesos(_p, numeric_limits<float>::min());

    // Iniciar cada subgrafo com um nó aleatório conectado
    for (int i = 0; i < _p; ++i) {
        if (!nodes.empty()) {
            // Seleciona o primeiro nó disponível
            Node* firstNode = nodes[0];
            subgrafos[i].push_back(firstNode->_id);
            minPesos[i] = firstNode->_weight;
            maxPesos[i] = firstNode->_weight;
            nodes.erase(nodes.begin());
        }
    }

    int contemplados = _p;  // Contabiliza os nós já alocados aos subgrafos

    // Itera sobre os nós restantes
    while (contemplados < _number_of_nodes && !nodes.empty()) {
        // Seleciona aleatoriamente um nó a partir da lista de nós restantes
        int randomIndex = rand() % nodes.size();
        Node* currentNode = nodes[randomIndex];
        nodes.erase(nodes.begin() + randomIndex);

        int Vc = currentNode->_id;
        int melhorSubgrafo = -1;
        float menorGap = numeric_limits<float>::max();

        // Verificar se o nó pode ser adicionado a algum subgrafo existente
        for (int i = 0; i < _p; ++i) {
            // Verificar se o nó está conectado a algum nó do subgrafo
            if (IsConnected(currentNode, subgrafos[i])) {
                // Calcular o novo gap se ele for adicionado
                float novoMinPeso = min(minPesos[i], currentNode->_weight);
                float novoMaxPeso = max(maxPesos[i], currentNode->_weight);
                float novoGap = novoMaxPeso - novoMinPeso;

                // Escolher o subgrafo que minimize o gap de peso
                if (novoGap < menorGap) {
                    menorGap = novoGap;
                    melhorSubgrafo = i;
                }
            }
        }

        // Adicionar o nó ao subgrafo escolhido
        if (melhorSubgrafo != -1) {
            // Verifica se o subgrafo já tem pelo menos 1 nó
            if (subgrafos[melhorSubgrafo].size() >= 1) {
                subgrafos[melhorSubgrafo].push_back(Vc);
                minPesos[melhorSubgrafo] = min(minPesos[melhorSubgrafo], currentNode->_weight);
                maxPesos[melhorSubgrafo] = max(maxPesos[melhorSubgrafo], currentNode->_weight);
                contemplados++;
            } else {
                // Se não houver subgrafo válido, não aloque o nó
                cout << "Subgrafo com apenas 1 nó, não alocando o nó: " << currentNode->_id << endl;
            }
        } else {
            // Se não houver subgrafo válido, não aloque o nó
            cout << "Nó não conectado: " << currentNode->_id << endl;
        }
    }

    // Garantir que todos os subgrafos tenham pelo menos 2 nós
    for (int i = 0; i < _p; ++i) {
        if (subgrafos[i].size() < 2) {
            cout << "Subgrafo " << i << " tem menos de 2 nós, realocando..." << endl;

            // Tente realocar nós de outros subgrafos para este subgrafo até ter pelo menos 2 nós
            for (int j = 0; j < _p; ++j) {
                if (i != j && subgrafos[j].size() > 0) {
                    // Mova um nó do subgrafo j para o subgrafo i

                    int nodeToMove = subgrafos[j].back(); // pega o último nó
                    subgrafos[i].push_back(nodeToMove);
                    subgrafos[j].pop_back(); // remove o nó de j

                    if (subgrafos[i].size() >= 2) {
                        break; // se já tem 2 nós, para de realocar
                    }
                }
            }
        }
    }

    // Exibir a solução final
    cout << "Subgrafos resultantes: " << endl;
    for (int i = 0; i < _p; ++i) {
        cout << "Subgrafo " << i << ": [";
        for (size_t m = 0; m < subgrafos[i].size(); ++m) {
            cout << subgrafos[i][m];
            if (m != subgrafos[i].size() - 1) {
                cout << ',';
            }
        }
        cout << ']' << endl;
    }

    // Verificar se todos os nós foram alocados
    if (nodes.size() > 0) {
        cout << "Nó(s) não alocado(s): ";
        for (size_t i = 0; i < nodes.size(); ++i) {
            cout << nodes[i]->_id << " ";
        }
        cout << endl;
    }

    return 0;
}

int Graph::GulosoRandomizadoReativo(int maxIterations, vector<float> alphas, int seed) {
    srand(time(0) + seed);

    // Inicializar os subgrafos
    subgrafos = vector<vector<size_t>>(_p);
    vector<float> minPesos(_p, numeric_limits<float>::max());
    vector<float> maxPesos(_p, numeric_limits<float>::min());
    unordered_set<size_t> alocados;  // Conjunto para armazenar nós alocados

    // Selecionar nós disponíveis
    vector<Node*> nodes;
    Node* node = _first;
    while (node != nullptr) {
        nodes.push_back(node);
        node = node->_next_node;
    }

    // Inicializar os subgrafos com nós aleatórios
    for (int i = 0; i < _p && !nodes.empty(); ++i) {
        int randomIndex = rand() % nodes.size();
        Node* firstNode = nodes[randomIndex];
        subgrafos[i].push_back(firstNode->_id);
        alocados.insert(firstNode->_id);
        minPesos[i] = firstNode->_weight;
        maxPesos[i] = firstNode->_weight;
        nodes.erase(nodes.begin() + randomIndex);
    }

    int contemplados = alocados.size();

    // Iterar sobre nós restantes
    while (contemplados < _number_of_nodes && !nodes.empty()) {
        int randomIndex = rand() % nodes.size();
        Node* currentNode = nodes[randomIndex];
        nodes.erase(nodes.begin() + randomIndex);

        int melhorSubgrafo = -1;
        float menorGap = numeric_limits<float>::max();

        // Verificar se o nó pode ser adicionado a algum subgrafo
        for (int i = 0; i < _p; ++i) {
            if (IsConnected(currentNode, subgrafos[i])) {
                // Calcular o novo gap se ele for adicionado
                float novoMinPeso = min(minPesos[i], currentNode->_weight);
                float novoMaxPeso = max(maxPesos[i], currentNode->_weight);
                float novoGap = novoMaxPeso - novoMinPeso;

                // Escolher o subgrafo que minimize o gap de peso
                if (novoGap < menorGap) {
                    menorGap = novoGap;
                    melhorSubgrafo = i;
                }
            }
        }

        // Adicionar o nó ao subgrafo escolhido
        if (melhorSubgrafo != -1) {
            subgrafos[melhorSubgrafo].push_back(currentNode->_id);
            alocados.insert(currentNode->_id);
            minPesos[melhorSubgrafo] = min(minPesos[melhorSubgrafo], currentNode->_weight);
            maxPesos[melhorSubgrafo] = max(maxPesos[melhorSubgrafo], currentNode->_weight);
            contemplados++;
        }
    }

    // Garantir que todos os subgrafos tenham pelo menos 2 nós
    for (int i = 0; i < _p; ++i) {
        while (subgrafos[i].size() < 2 && !nodes.empty()) {
            int randomIndex = rand() % nodes.size();
            Node* currentNode = nodes[randomIndex];
            nodes.erase(nodes.begin() + randomIndex);

            subgrafos[i].push_back(currentNode->_id);
            alocados.insert(currentNode->_id);
            minPesos[i] = min(minPesos[i], currentNode->_weight);
            maxPesos[i] = max(maxPesos[i], currentNode->_weight);
        }
    }

    // Exibir a solução final
    cout << "Subgrafos resultantes: " << endl;
    for (int i = 0; i < _p; ++i) {
        cout << "Subgrafo " << i << ": [";
        for (size_t m = 0; m < subgrafos[i].size(); ++m) {
            cout << subgrafos[i][m];
            if (m != subgrafos[i].size() - 1) {
                cout << ',';
            }
        }
        cout << ']' << endl;
    }

    return 0;
}

float Graph::calcularGapTotal() {
    float gapTotal = 0.0;

    // Itera sobre cada subgrafo
    for (const auto& subgrafo : subgrafos) {
        if (subgrafo.empty()) continue; // Ignorar subgrafos vazios

        float minPeso = numeric_limits<float>::max();
        float maxPeso = numeric_limits<float>::min();

        // Percorre todos os nós do subgrafo para calcular o peso mínimo e máximo
        for (size_t nodeId : subgrafo) {
            Node* node = find_node(nodeId);  // Supondo que você tenha essa função para encontrar o nó pelo ID
            if (node != nullptr) {
                minPeso = min(minPeso, node->_weight);
                maxPeso = max(maxPeso, node->_weight);
            }
        }

        // Calcula o gap para este subgrafo e adiciona ao gap total
        float gap = maxPeso - minPeso;
        gapTotal += gap;

        // Imprimir os detalhes do subgrafo (opcional)
        cout << "Subgrafo: [";
        for (size_t nodeId : subgrafo) {
            cout << nodeId << " ";
        }
        cout << "] | MinPeso: " << minPeso << " | MaxPeso: " << maxPeso << " | Gap: " << gap << endl;
    }

    return gapTotal;
}

bool Graph::IsSubgrafoConexo(const vector<size_t>& subgrafo) {
    if (subgrafo.empty()) return true;  // Subgrafo vazio é considerado conexo

    // Marca de visitação dos vértices
    vector<bool> visitado(_number_of_nodes, false);

    // Realiza a busca em profundidade a partir do primeiro vértice do subgrafo
    vector<size_t> resultado;
    buscaProfundidade(subgrafo[0], resultado, visitado);

    // Verifica se todos os vértices do subgrafo foram visitados
    for (size_t vertice : subgrafo) {
        if (!visitado[vertice]) {
            return false;  // Se algum vértice do subgrafo não foi visitado, não é conexo
        }
    }

    return true;  // Se todos os vértices foram visitados, o subgrafo é conexo
}
