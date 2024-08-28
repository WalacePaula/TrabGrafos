#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "Graph.hpp"

using namespace std;

// Função para converter string em booleano
bool to_bool(const string& str) {
    return str == "true" || str == "1";
}

// Função para salvar a saída em um arquivo
void save_output_to_file(const string& output, const string& file_name) {
    ofstream output_file(file_name, ios::app); // Abrindo em modo append para adicionar ao final do arquivo
    if (output_file.is_open()) {
        output_file << output << endl;
        output_file.close();
        cout << "Saida salva no arquivo " << file_name << endl;
    } else {
        cerr << "Erro ao abrir o arquivo de saida!" << endl;
    }
}

void print_menu() {
    cout << "Menu de Opcoes:\n";
    cout << "1. Fecho transitivo direto\n";
    cout << "2. Fecho transitivo indireto\n";
    cout << "3. Caminho minimo entre dois vertices (Dijkstra)\n";
    cout << "4. Caminho minimo entre dois vertices (Floyd)\n";
    cout << "5. Arvore Geradora Minima (Prim)\n";
    cout << "6. Arvore Geradora Minima (Kruskal)\n";
    cout << "7. Caminhamento em profundidade\n";
    cout << "8. Raio, diametro, centro e periferia do grafo\n";
    cout << "9. Conjunto de vertices de articulacao\n";
    cout << "10. Sair\n";
    cout << "Escolha uma opcao: ";

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
                    output << "Fecho transitivo direto do vertice " << vertex_id << ": ";
                    for (size_t i = 0; i < result.size(); i++)
                        output << result[i] << " ";
                    output << endl;
                    cout << output.str();
                    break;
                }
                case 2: {
                    size_t vertex_id;
                    cout << "Digite o ID do vertice: ";
                    cin >> vertex_id;
                    vector<size_t> result = graph->get_fechoTransitivoIndireto(vertex_id);
                    output << "Fecho transitivo indireto do vertice " << vertex_id << ": ";
                    for (size_t i = 0; i < result.size(); i++)
                        output << result[i] << " ";
                    output << endl;
                    cout << output.str();
                    break;
                }
                case 3: {
                    size_t start_id, end_id;
                    cout << "Digite o ID do vertice de origem: ";
                    cin >> start_id;
                    cout << "Digite o ID do vertice de destino: ";
                    cin >> end_id;
                    string result = graph->min_path_dijkstra(start_id, end_id);
                    output << "Caminho minimo entre " << start_id << " e " << end_id << " (Dijkstra): " << result << endl;
                    cout << output.str();
                    break;
                }
                case 4: {
                    size_t start_id, end_id;
                    cout << "Digite o ID do vertice de origem: ";
                    cin >> start_id;
                    cout << "Digite o ID do vertice de destino: ";
                    cin >> end_id;
                    string result = graph->min_path_floyd(start_id, end_id);
                    output << "Caminho minimo entre " << start_id << " e " << end_id << " (Floyd): " << result << endl;
                    cout << output.str();
                    break;
                }
                case 5: {
                    std::vector<size_t> vertices;
                    size_t num_vertices;

                    std::cout << "Digite o número de vertices do subconjunto X: ";
                    std::cin >> num_vertices;

                    std::cout << "Digite os IDs dos vertices do subconjunto X: ";
                    for(size_t i = 0; i < num_vertices; ++i) {
                        size_t vertex;
                        std::cin >> vertex;
                        vertices.push_back(vertex);
                    }

                    Graph* arvGeradoraMin = graph->arvoreGeradoraMinPrim(vertices);

                    std::string arvore_string = graph->print_arvoreGeradoraMinima(arvGeradoraMin);
                    output << arvore_string;

                    cout << arvore_string;

                    delete arvGeradoraMin;
                    break;
                }

                case 6: {
                    std::vector<size_t> vertices;
                    size_t num_vertices;

                    std::cout << "Digite o numero de vertices do subconjunto X: ";
                    std::cin >> num_vertices;

                    std::cout << "Digite os IDs dos vertices do subconjunto X: ";
                    for(size_t i = 0; i < num_vertices; ++i) {
                        size_t vertex;
                        std::cin >> vertex;
                        vertices.push_back(vertex);
                    }

                    Graph* arvGeradoraMin = graph->arvoreGeradoraMinKruskal(vertices);

                    std::string arvore_string = graph->print_arvoreGeradoraMinima(arvGeradoraMin);
                    output << arvore_string;

                    cout << output.str();

                    delete arvGeradoraMin;
                    break;
                }

                case 7: {
                    size_t vertice;
                    std::vector<size_t> resultado;
                    std::vector<bool> visitado(graph->getNum_vertices(), false);

                    std::cout << "Digite o ID do vertice de inicio: ";
                    std::cin >> vertice;

                    graph->buscaProfundidade(vertice, resultado, visitado);

                    output << "Ordem de caminhamento em profundidade a partir do vertice " << vertice << ":\n";
                    for(size_t v : resultado) {
                        output << v << " ";
                    }
                    output << endl;
                    cout << output.str();

                    break;
                }

                case 8: {
                    string resultado = graph->caracteristicas();
                    output << resultado;
                    cout << output.str();
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
                    output << "Saindo do programa...\n";
                    cout << output.str(); // Exibe a saída
                    break;
                default:
                    output << "Opcao invalida. Tente novamente.\n";
                    cout << output.str(); // Exibe a saída
                    break;
            }

            if (option >= 1 && option <= 9) { // Opcões válidas para salvar o resultado
                char save_choice;
                cout << "Deseja salvar a saida no arquivo (S/N)? ";
                cin >> save_choice;
                if (save_choice == 'S' || save_choice == 's') {
                    save_output_to_file(output.str(), argv[2]);
                }
            }

            output.str(""); // Limpa o conteúdo do output
            output.clear();  // Limpa as flags de erro do stringstream

        } while (option != 10);

        delete graph;

        return 0;
    } else {
        cout << "ERRO: Espera-se: ./<arquivo_entrada> <arquivo_saida> <Op_Direc> <Op_PesoAresta> <Op_PesoNos>" << endl;
        return 1;
    }
}
