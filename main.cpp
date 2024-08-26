#include <iostream>
#include <fstream>
#include <string>
#include "Graph.hpp"

using namespace std;

bool to_bool(const string& str) {
    return str == "true" || str == "1";
}

int main(int argc, char* argv[]) {
    if (argc == 6) {
        std::ifstream instance_file(argv[1]);

        if (!instance_file.is_open()) {
            std::cerr << "Erro ao abrir o arquivo de instancia!" << std::endl;
            return 1;
        }

        bool directed = to_bool(argv[3]);
        bool weighted_edges = to_bool(argv[4]);
        bool weighted_nodes = to_bool(argv[5]);

        Graph* graph = new Graph(instance_file, directed, weighted_edges, weighted_nodes);
        instance_file.close();

        graph->print_graph();

        std::cout << "Imprimindo sequencia de nos " << std::endl;
        graph->imprime_sequencia_nos();

        graph->remove_node(3);
        std::cout << "Removendo no 3" << std::endl;
        graph->print_graph();

        std::cout << "Imprimindo sequencia de nos " << std::endl;
        graph->imprime_sequencia_nos();

        std::cout << "Removendo no 2" << std::endl;
        graph->remove_node(2);
        graph->print_graph();

        std::cout << "Imprimindo sequencia de nos " << std::endl;
        graph->imprime_sequencia_nos();

        delete graph;  // Não se esqueça de deletar o ponteiro para evitar vazamento de memória

        return 0;
    } else {
        std::cout << "ERRO: Espera-se: ./<arquivo_entrada> <arquivo_saida> <Op_Direc> <Op_PesoAresta> <Op_PesoNos>" << std::endl;
        return 1;
    }
}

    /*Graph *readFileFirstPart(ifstream &input_file, int directed, int weightedEdge, int weightedNode, bool isPERT)
{
    // Preenchimento das variáveis globais
    ::directed = directed;
    ::weightedEdge = weightedEdge;
    ::weightedNode = weightedNode;

    // Variáveis para auxiliar na criação dos nós no Grafo
    int labelNodeSource;
    int labelNodeTarget;
    int order;

    // Obtém a ordem do grafo
    input_file >> order;

    // Cria objeto grafo
    Graph *graph = new Graph(order, directed, weightedEdge, weightedNode);

    // Leitura de arquivo
    //
    // Grafo SEM peso nos nós, e SEM peso nas arestas
    if (!graph->getWeightedEdge() && !graph->getWeightedNode())
    {
        while (input_file >> labelNodeSource >> labelNodeTarget)
        {
            Node *sourceNode = nullptr;
            Node *targetNode = nullptr;
            graph->insertEdge(abs(labelNodeSource), abs(labelNodeTarget), 1, &sourceNode, &targetNode, isPERT);
        }
    }
    // Grafo SEM peso nos nós, mas COM peso nas arestas
    else if (graph->getWeightedEdge() && !graph->getWeightedNode())
    {
        float edgeWeight;

        while (input_file >> labelNodeSource >> labelNodeTarget >> edgeWeight)
        {
            Node *sourceNode = nullptr;
            Node *targetNode = nullptr;
            graph->insertEdge(abs(labelNodeSource), abs(labelNodeTarget), edgeWeight, &sourceNode, &targetNode, isPERT);
        }
    }
    // Grafo COM peso nos nós, mas SEM peso nas arestas
    else if (graph->getWeightedNode() && !graph->getWeightedEdge())
    {
        float nodeSourceWeight, nodeTargetWeight;

        while (input_file >> labelNodeSource >> nodeSourceWeight >> labelNodeTarget >> nodeTargetWeight)
        {
            Node *sourceNode = nullptr;
            Node *targetNode = nullptr;
            graph->insertEdge(abs(labelNodeSource), abs(labelNodeTarget), 1, &sourceNode, &targetNode, isPERT);
            if (sourceNode != nullptr)
                sourceNode->setWeight(nodeSourceWeight);
            if (targetNode != nullptr)
                targetNode->setWeight(nodeTargetWeight);
        }
    }
    // Grafo COM peso nos nós, e COM peso nas arestas
    else if (graph->getWeightedNode() && graph->getWeightedEdge())
    {
        float nodeSourceWeight, nodeTargetWeight, edgeWeight;

        while (input_file >> labelNodeSource >> nodeSourceWeight >> labelNodeTarget >> nodeTargetWeight >> edgeWeight)
        {
            Node *sourceNode = nullptr;
            Node *targetNode = nullptr;
            graph->insertEdge(abs(labelNodeSource), abs(labelNodeTarget), edgeWeight, &sourceNode, &targetNode, isPERT);
            if (sourceNode != nullptr)
                sourceNode->setWeight(nodeSourceWeight);
            if (targetNode != nullptr)
                targetNode->setWeight(nodeTargetWeight);
        }
    }

    int i = -1;
    while (order > graph->getNodeIdCounter())
    {
        graph->insertNode(i);
        i--;
    }

    return graph;
}



     std::ifstream instance_file("instances/instance1.txt");

    if (!instance_file.is_open()) {
        std::cerr << "Erro ao abrir o arquivo de instância!" << std::endl;
        return 1;
    }
    Graph graph(instance_file);
    instance_file.close();
    graph.print_graph();

    cout << "Imprimindo sequencia de nós " << endl;
    graph.imprime_sequencia_nos();

    graph.remove_node(3);
    cout << "Removendo nó 3" << endl;
    graph.print_graph();

    cout << "Imprimindo sequencia de nós " << endl;
    graph.imprime_sequencia_nos();

    cout << "Removendo nó 2" << endl;
    graph.remove_node(2);
    graph.print_graph();

    cout << "Imprimindo sequencia de nós " << endl;
    graph.imprime_sequencia_nos();



    return 0;
}

/****************
 * Funcao    : int main(int argc, char const *argv[])                                                                                                       *
 * Descricao : Função principal, interpreta o tipo de execução pelos argumentos passados, abre arquivo de entrada de dados e define semente de randomização *
 * Parametros: argc - quantidade de argumentos passados na linha de comando                                                                                 *
 *             argv - vetor dos argumentos passados pela linha de comando                                                                                   *
 * Retorno   : inteiro que define se houve erro, ou não, na execução do programa.                                                                           *
 ***************
int main(int argc, char const *argv[])
{
    // Verifica se todos os argumentos foram fornecidos
    if (argc == 3 || argc == 4 || argc == 6 || argc >= 8)
    {
        int endingCode = 0;
        ::seed = (unsigned)(time(NULL) & 0xFFFF) | (getpid() << 16);

        // Captura argumentos
        string program_name(argv[0]);
        ::input_file_name = argv[1];
        string output_file_name(argv[2]);

        // Abre arquivos de entrada e saída
        ifstream input_file;
        ofstream output_file;
        input_file.open(input_file_name, ios::in);

        Graph *graph;

        if (input_file.is_open())
        {
            // Executa versão adequada do programa
            if (argc == 6 && atoi(argv[3]) != 2)
            {
                graph = readFileFirstPart(input_file, atoi(argv[3]), atoi(argv[4]), atoi(argv[5]), false);
                endingCode = mainMenu(output_file_name, graph, false);
            }
            else
            {
                graph = readFileSecondPart(input_file);
                // Menu da segunda parte
                if (argc == 3)
                    endingCode = mainMenu(output_file_name, graph, true);
                else
                {
                    string returnText = "";
                    float totalCost = 0;
                    // Guloso
                    if (argc == 4)
                    {
                        // program input output 1
                        double intialTime = cpuTime();
                        list<SimpleNode> resultSet = graph->dominatingSetWeighted(&totalCost);
                        double finalTime = cpuTime();
                        double timeElapsed = finalTime - intialTime;
                        printResultVariables(&returnText, resultSet.size(), totalCost, timeElapsed, 0, false, 0, false);
                    }
                    else if (argc == 6)
                    {
                        // program input output 2 numIterations alfa
                        double intialTime = cpuTime();
                        list<SimpleNode> resultSet = graph->dominatingSetWeightedRandomized(&totalCost, ::seed, atoi(argv[4]), atof(argv[5]));
                        double finalTime = cpuTime();
                        double timeElapsed = finalTime - intialTime;
                        printResultVariables(&returnText, resultSet.size(), totalCost, timeElapsed, ::seed, true, 0, false);
                    }
                    else if (argc >= 8)
                    {
                        // program input output 3 numIterations block numAlfas alfas...
                        int numAlfas = atoi(argv[6]);
                        float *vetAlfas = new float[numAlfas];
                        for (int i = 7; i < 7 + numAlfas; i++)
                            vetAlfas[i - 7] = atof(argv[i]);

                        double intialTime = cpuTime();
                        float bestAlfa = 0;
                        list<SimpleNode> resultSet = graph->dominatingSetWeightedRandomizedReactive(&totalCost, ::seed, atoi(argv[4]), vetAlfas, atoi(argv[6]), atoi(argv[5]), &bestAlfa);
                        double finalTime = cpuTime();
                        double timeElapsed = finalTime - intialTime;
                        printResultVariables(&returnText, resultSet.size(), totalCost, timeElapsed, ::seed, true, bestAlfa, true);
                    }
                    ofstream output_file;
                    output_file.open(output_file_name, ios::out | ios::trunc);
                    output_file << returnText;
                    output_file.close();
                }
            }
        }
        else
        {
            cout << "ERRO: Não foi possível abrir o arquivo de entrada " << input_file_name << "!" << endl;
            return 1;
        }

        // Fecha arquivo de entrada
        input_file.close();
        // Fecha arquivo de saída
        output_file.close();

        return endingCode;
    }
    else
    {
        cout << "ERRO: Espera-se: ./<program_name> <input_file> [<output_file> <directed> <weighted_edge> <weighted_node>]" << endl;
        return 1;
    }

}*/


