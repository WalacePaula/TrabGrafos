Grupo: 
Dayane Guimarães Gomes Godoy
Emerson Caneschi Coelho de Souza
Habirou Mama
Walace Alan de Paula

Trabalho 1:
Observações: Função que obtém os Vértices de Articulação não foi implementada

Instruções para execução: Segue os padrões especificados. Compilamos fazendo g++ *.c* -o execGrafos
Executamos fazendo ./execGrafos <arquivo_entrada> <arquivo_saida> <Op_Direc> <Op_PesoAresta> <Op_PesoNos>
Onde:
<arquivo_entrada> é o nome do arquivo que contém as informações do grafo,
<arquivo_saida> é o arquivo onde será gravado o grafo armazenado na memória ao término da execução do programa;
<Op_Direc> <Op_PesoAresta> <Op_PesoNos> são parâmetros booleanos referentes às características do grafo (direcionado, peso nas arestas, peso nos nós)

Arquivos de instâncias: utilizamos os disponibilizados no template do trabalho, mas convertidos para .txt



Trabalho 2:
Observações: O arquivo de saída não foi configurado para os algoritmos gulosos

Para executar segue os padrões especificados da etapa anterior:
- Compilamos fazendo g++ *.c* -o execGrafos
Executamos fazendo ./execGrafos <arquivo_entrada> <arquivo_saida> <Op_Direc> <Op_PesoAresta> <Op_PesoNos>
Onde:
<arquivo_entrada> é o nome do arquivo que contém as informações do grafo,
<arquivo_saida> é o arquivo onde será gravado o grafo armazenado na memória ao término da execução do programa;
<Op_Direc> <Op_PesoAresta> <Op_PesoNos> são parâmetros booleanos referentes às características do grafo (direcionado, peso nas arestas, peso nos nós)

Arquivos de instâncias: utilizamos os disponibilizados no template do trabalho, mas convertidos para .txt
As instâncias indicadas na planilha disponibilizada estão na pasta MGGPP_Selecionadas

Na interface é possível escolher as opções, inclusive executar o algoritmo Guloso pressionando 10, o guloso randomizado pressionando 11 ou guloso randomizado reativo pressionando 12

Parâmetros: 
Guloso -> Não há parâmetros
GulosoRandomizado -> alfa será usado 0,5, e semente 42
GulosoRandomizadoReativo -> Número de iterações máximo = 500, vetor de alfas = {0.05, 0.1, 0.15, 0.3, 0.5, 0.7, 0.9}, semente = 123
