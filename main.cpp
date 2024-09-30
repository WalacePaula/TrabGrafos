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
    cout << "10. Algoritmo Guloso\n";
    cout << "11. Algoritmo Guloso Randomizado\n";
    cout << "12. Algoritmo Guloso Randomizado Reativo\n";
    cout << "0. Sair\n";
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

        ///////////////////////////////////////////////////////////////////////////////////
        /*
        vector<vector<int>> subgraphs = graph->algoritmoGulosoMGGPP();

        float total_gap = graph->calculaTotalGap(subgraphs);
        graph->print_graph();
        for (size_t i = 0; i < subgraphs.size(); ++i) {
            cout << "Subgrafo " << i + 1 << ": ";
            graph->printSubgraph(subgraphs[i]); 
        }
        */

        ///////////////////////////////////////////////////////////////////////////////////

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
                    vector<size_t> vertices;
                    size_t num_vertices;

                    cout << "Digite o número de vertices do subconjunto X: ";
                    cin >> num_vertices;

                    cout << "Digite os IDs dos vertices do subconjunto X: ";
                    for(size_t i = 0; i < num_vertices; ++i) {
                        size_t vertex;
                        cin >> vertex;
                        vertices.push_back(vertex);
                    }

                    Graph* arvGeradoraMin = graph->arvoreGeradoraMinPrim(vertices);

                    string arvore_string = graph->print_arvoreGeradoraMinima(arvGeradoraMin);
                    output << arvore_string;

                    cout << arvore_string;

                    delete arvGeradoraMin;
                    break;
                }

                case 6: {
                    vector<size_t> vertices;
                    size_t num_vertices;

                    cout << "Digite o numero de vertices do subconjunto X: ";
                    cin >> num_vertices;

                    cout << "Digite os IDs dos vertices do subconjunto X: ";
                    for(size_t i = 0; i < num_vertices; ++i) {
                        size_t vertex;
                        cin >> vertex;
                        vertices.push_back(vertex);
                    }

                    Graph* arvGeradoraMin = graph->arvoreGeradoraMinKruskal(vertices);

                    string arvore_string = graph->print_arvoreGeradoraMinima(arvGeradoraMin);
                    output << arvore_string;

                    cout << output.str();

                    delete arvGeradoraMin;
                    break;
                }

                case 7: {
                    size_t vertice;
                    vector<size_t> resultado;
                    vector<bool> visitado(graph->getNum_vertices(), false);

                    cout << "Digite o ID do vertice de inicio: ";
                    cin >> vertice;

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
                    /*vector<size_t> vertices_articulacao = graph->get_verticesArticulacao();

                    cout << "Conjunto de vértices de articulação:\n";
                    for(size_t v : vertices_articulacao) {
                        cout << v << " ";
                    }
                    cout << endl;*/
                    break;
                }

                case 10: {
                    graph->print_graph();
                    graph->Guloso();
                    float gapTotal = graph->calcularGapTotal();
                    cout << "Gap Total: " << gapTotal << endl;
                    break;
                }

                case 11: {
                    graph->GulosoRandomizado(0.5,42);
                    float gapTotal = graph->calcularGapTotal();
                    cout << "Gap Total: " << gapTotal << endl;
                    break;
                    /*
                    cout << "\n\nALFA: 0.1" << endl;
                    for (int i = 0; i < 10; i++){
                        graph->GulosoRandomizado(0.1, i);
                    }
                    cout << "\n\nALFA: 0.2" << endl;
                    for (int i = 0; i < 10; i++){
                        graph->GulosoRandomizado(0.2, i);
                    }
                    cout << "\n\nALFA: 0.3" << endl;
                    for (int i = 0; i < 10; i++){
                        graph->GulosoRandomizado(0.3, i);
                    }
                    cout << "\n\nALFA: 0.5" << endl;
                    for (int i = 0; i < 10; i++){
                        graph->GulosoRandomizado(0.5, i);
                    }
                    cout << "\n\nALFA: 0.7" << endl;
                    for (int i = 0; i < 10; i++){
                        graph->GulosoRandomizado(0.7, i);
                    }
                    */
                    break;
                }

                case 12: {
                    // Parâmetros
                    int maxIterations = 500;  // Número de iterações para a abordagem reativa
                    vector<float> alphas = {0.05, 0.1, 0.15, 0.3, 0.5, 0.7, 0.9};  // Lista de valores de alpha
                    int seed = 123;  // Defina uma semente para garantir a reprodutibilidade

                    graph->GulosoRandomizadoReativo(maxIterations,alphas, seed);
                    float gapTotal = graph->calcularGapTotal();
                    cout << "Gap Total: " << gapTotal << endl;
                    break;
                }

                case 0:
                    output << "Saindo do programa...\n";
                    cout << output.str(); // Exibe a saída
                    break;
                default:
                    output << "Opcao invalida. Tente novamente.\n";
                    cout << output.str(); // Exibe a saída
                    break;
            }

            if (option >= 1 && option <= 12) { // Opcões válidas para salvar o resultado
                char save_choice;
                cout << "Deseja salvar a saida no arquivo (S/N)? ";
                cin >> save_choice;
                if (save_choice == 'S' || save_choice == 's') {
                    save_output_to_file(output.str(), argv[2]);
                }
            }

            output.str(""); // Limpa o conteúdo do output
            output.clear();  // Limpa as flags de erro do stringstream

        } while (option != 0);

        delete graph;

        return 0;
    } else {
        cout << "ERRO: Espera-se: ./<arquivo_entrada> <arquivo_saida> <Op_Direc> <Op_PesoAresta> <Op_PesoNos>" << endl;
        return 1;
    }
}
