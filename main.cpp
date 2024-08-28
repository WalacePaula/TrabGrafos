#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "Graph.hpp"

using namespace std;

bool to_bool(const string& str) {
    return str == "true" || str == "1";
}

void save_output_to_file(const string& output, const string& file_name) {
    ofstream output_file(file_name, ios::app); // Abrindo em modo append para adicionar ao final do arquivo
    if (output_file.is_open()) {
        output_file << output << endl;
        output_file.close();
        cout << "Saída salva no arquivo " << file_name << endl;
    } else {
        cerr << "Erro ao abrir o arquivo de saída!" << endl;
    }
}

void print_menu() {
    cout << "Menu de Opções:\n";
    cout << "1. Fecho transitivo direto\n";
    cout << "2. Fecho transitivo indireto\n";
    cout << "3. Caminho mínimo entre dois vértices (Dijkstra)\n";
    cout << "4. Caminho mínimo entre dois vértices (Floyd)\n";
    cout << "5. Árvore Geradora Mínima (Prim)\n";
    cout << "6. Árvore Geradora Mínima (Kruskal)\n";
    cout << "7. Caminhamento em profundidade\n";
    cout << "8. Raio, diâmetro, centro e periferia do grafo\n";
    cout << "9. Conjunto de vértices de articulação\n";
    cout << "10. Sair\n";
    cout << "Escolha uma opção: ";
}

int main(int argc, char* argv[]) {
    if (argc == 6) {
        ifstream instance_file(argv[1]);

        if (!instance_file.is_open()) {
            cerr << "Erro ao abrir o arquivo de instancia!" << endl;
            return 1;
        }

        bool directed = to_bool(argv[3]);
        bool weighted_edges = to_bool(argv[4]);
        bool weighted_nodes = to_bool(argv[5]);

        Graph* graph = new Graph(instance_file, directed, weighted_edges, weighted_nodes);
        instance_file.close();

        int option;
        stringstream output;

        do {
            print_menu();
            cin >> option;

            switch (option) {
                case 1: {
                    size_t vertex_id;
                    cout << "Digite o ID do vertice: ";
                    cin >> vertex_id;
                    vector<size_t> result = graph->get_fechoTransitivoDireto(vertex_id);
                    for (int i = 0; i < result.size(); i++)
                        cout << result[i] << " ";
                    cout << endl;
                    break;
                }
                case 2: {
                    int vertex_id;
                    cout << "Digite o ID do vertice: ";
                    cin >> vertex_id;
                    vector<size_t> result = graph->get_fechoTransitivoIndireto(vertex_id);
                    for (int i = 0; i < result.size(); i++)
                        cout << result[i] << " ";
                    cout << endl;
                    break;
                }
                case 3: {
                    int start_id, end_id;
                    cout << "Digite o ID do vertice de origem: ";
                    cin >> start_id;
                    cout << "Digite o ID do vertice de destino: ";
                    cin >> end_id;
                    graph->min_path_dijkstra(start_id, end_id);
                    break;
                }
                case 4: {
                    int start_id, end_id;
                    cout << "Digite o ID do vertice de origem: ";
                    cin >> start_id;
                    cout << "Digite o ID do vertice de destino: ";
                    cin >> end_id;
                    string result = graph->min_path_floyd(start_id, end_id);
                    cout << result << endl;
                    break;
                }
                case 5: {
                    std::vector<size_t> vertices;
                    size_t num_vertices;

                    std::cout << "Digite o número de vértices do subconjunto X: ";
                    std::cin >> num_vertices;

                    std::cout << "Digite os IDs dos vértices do subconjunto X: ";
                    for(size_t i = 0; i < num_vertices; ++i) {
                        size_t vertex;
                        std::cin >> vertex;
                        vertices.push_back(vertex);
                    }

                    Graph* arvGeradoraMin = graph->arvoreGeradoraMinPrim(vertices);
                    std::cout << "Árvore Geradora Mínima (Prim) gerada com sucesso!\n";
                    graph->print_arvoreGeradoraMinima(arvGeradoraMin);

                    delete arvGeradoraMin;
                    break;
                }
                case 6: {
                    std::vector<size_t> vertices;
                    size_t num_vertices;

                    std::cout << "Digite o número de vértices do subconjunto X: ";
                    std::cin >> num_vertices;

                    std::cout << "Digite os IDs dos vértices do subconjunto X: ";
                    for(size_t i = 0; i < num_vertices; ++i) {
                        size_t vertex;
                        std::cin >> vertex;
                        vertices.push_back(vertex);
                    }

                    Graph* arvGeradoraMin = graph->arvoreGeradoraMinKruskal(vertices);
                    std::cout << "Árvore Geradora Mínima (Kruskal) gerada com sucesso!\n";
                    graph->print_arvoreGeradoraMinima(arvGeradoraMin);

                    delete arvGeradoraMin;
                    break;
                }

                case 7: {
                    size_t vertice;
                    std::vector<size_t> resultado;
                    std::vector<bool> visitado(graph->getNum_vertices(), false);

                    std::cout << "Digite o ID do vértice de início: ";
                    std::cin >> vertice;

                    graph->buscaProfundidade(vertice, resultado, visitado);

                    std::cout << "Ordem de caminhamento em profundidade a partir do vértice " << vertice << ":\n";
                    for(size_t v : resultado) {
                        std::cout << v << " ";
                    }
                    std::cout << std::endl;

                    break;
                }

                case 8: {
                    // Função para calcular raio, diâmetro, centro e periferia do grafo
                    break;
                }
                case 9: {
                    /*std::vector<size_t> vertices_articulacao = graph->get_verticesArticulacao();

                    std::cout << "Conjunto de vértices de articulação:\n";
                    for(size_t v : vertices_articulacao) {
                        std::cout << v << " ";
                    }
                    std::cout << std::endl;*/

                    break;
                }

                case 10:
                    cout << "Saindo do programa...\n";
                    break;
                default:
                    cout << "Opção inválida. Tente novamente.\n";
            }

            // Exibindo a saída no console
            cout << output.str() << endl;

            // Perguntando se o usuário deseja salvar a saída
            char save_option;
            cout << "Deseja salvar a saída em um arquivo? (s/n): ";
            cin >> save_option;

            if (save_option == 's' || save_option == 'S') {
                save_output_to_file(output.str(), argv[2]);
            }

            // Limpa a saída após cada iteração
            output.str("");
            output.clear();

        } while (option != 10);

        delete graph;  // Deletando o ponteiro para evitar vazamento de memória
        return 0;
    } else {
        cout << "ERRO: Espera-se: ./<arquivo_entrada> <arquivo_saida> <Op_Direc> <Op_PesoAresta> <Op_PesoNos>" << endl;
        return 1;
    }
}
