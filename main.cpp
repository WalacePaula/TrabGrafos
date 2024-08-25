#include <iostream>
#include "Graph.hpp"
#include "Graph.cpp"


using namespace std;

int main(int argc, char* argv[]) {

     std::ifstream instance_file("C:/Users/walac/OneDrive/Documentos/codeblocks/Grafos/instances/instance1.txt");

    // Verifique se o arquivo foi aberto corretamente
    if (!instance_file.is_open()) {
        std::cerr << "Erro ao abrir o arquivo de instância!" << std::endl;
        return 1;
    }
    Graph graph(instance_file);
    // Crie o grafo a partir do arquivo de instância
    graph.print_graph();

    // Feche o arquivo de instância
    instance_file.close();

    // Agora você pode trabalhar com o grafo 'g'

    // Aqui você pode realizar operações com o grafo
    //graph.print_graph();

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


