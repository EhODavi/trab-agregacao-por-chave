#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <iomanip>

using namespace std;

// Struct que guarda o campoC e campoA de cada linha. ✓
struct registro {
    char campoC[255];
    char campoA[255];
};

// Função que transforma uma string em um registro. ✓
registro stringParaRegistro(const char *vetorLinha) {
    // Declarando um registro.
    registro registro;

    // Declarando variáveis auxiliares.
    int j = 0, k = 0;

    // Guardando o conteúdo da linha sem as vírgulas.
    for (int i = 0; vetorLinha[i] != '\0'; i++) {
        // Se o j for 0, quer dizer que estamos lendo a primeira coluna.
        if (j == 0) {
            if (vetorLinha[i] == ',') {
                registro.campoC[k] = '\0';
                j++;
                k = 0;
            } else {
                registro.campoC[k] = vetorLinha[i];
                k++;
            } 
        } else { // Se for 1, estamos lendo a segunda coluna.
            registro.campoA[k] = vetorLinha[i];
            k++;
        }
    }

    // Definindo o '\0' no campoA.
    registro.campoA[k] = '\0';

    // Retornando o registro.
    return registro;
}

// Função que intercala os qtdArquivos arquivos ordenados. ✓
void mergeExterno(int qtdArquivos) {
    // Abrindo o arquivo.
    ofstream fout("dispositivo0.txt");
    
    // Verificando se o arquivo foi aberto. Se não foi aberto,
    // o programa emite uma mensagem de erro e finaliza.
    if (!fout.is_open()) {
        cerr << "ERRO AO ABRIR O ARQUIVO DE SAIDA!" << endl;
        exit(1);
    }

    // Criando um vetor de arquivos de leitura.
    ifstream *fin = new ifstream[qtdArquivos - 1];

    // Abrindo os arquivos de leitura.
    for (int i = 0; i < qtdArquivos - 1; i++) {
        // Criando um nome para o arquivo.
        char nomeArquivo[255];
        sprintf(nomeArquivo, "dispositivo%d.txt", i + 1);

        // Abrindo o arquivo.
        fin[i].open(nomeArquivo);

        // Verificando se o arquivo foi aberto. Se não foi aberto,
        // o programa emite uma mensagem de erro e finaliza.
        if (!fin[i].is_open()) {
            cerr << "ERRO AO ABRIR O ARQUIVO DE ENTRADA!" << endl;
            exit(1);
        }
    }
    
    // Criando um vetor de registros
    registro *registros = new registro[qtdArquivos - 1];

    // Criando um vetor que armazena o estado dos arquivos.
    bool *terminou = new bool[qtdArquivos - 1];

    // Criando uma string que guardará o conteúdo de uma linha.
    string linha;
    
    // Guardando o primeiro registro de cada arquivo.
    for (int i = 0; i < qtdArquivos - 1; i++) {
        // Pegando uma linha.
        getline(fin[i], linha);
        
        // Transformando a string para um vetor de char.
        const char *vetorLinha = linha.c_str();

        // Transformando a linha obtida para um registro.
        registros[i] = stringParaRegistro(vetorLinha);

        // Inicializando o vetor terminou (Inicialmente, nenhum arquivo terminou).
        terminou[i] = false;
    }

    // Variável que guardará o valor de quantos arquivos terminaram de ser lidos.
    int qtdArquivosTerminaram = 0;

    // Irá intercalar enquanto todos os arquivos não acabarem.
    while (qtdArquivosTerminaram != (qtdArquivos - 1)) {
        // Variável que guardará o menor registro de todos.
        registro menorRegistro;

        // Variável que guardará a posição do menor registro.
        int posicaoRegistro;

        // Inicializando a variável menorRegistro e posicaoRegistro.
        for (int i = 0; i < qtdArquivos - 1; i++) {
            if (!terminou[i]) {
                menorRegistro = registros[i];
                posicaoRegistro = i;
                break;
            }
        }

        // Guardando o menorRegistro e a posicaoRegistro.
        for (int i = 0; i < qtdArquivos - 1; i++) {
            if (!terminou[i]) {
                if (strcmp(registros[i].campoC, menorRegistro.campoC) < 0) {
                    menorRegistro = registros[i];
                    posicaoRegistro = i;
                }
            }
        }

        // Verificando se o arquivo acabou.
        if (fin[posicaoRegistro].eof()) { 
            qtdArquivosTerminaram++;
            terminou[posicaoRegistro] = true;
            fin[posicaoRegistro].close();
        } else {
            // Pegando uma linha.
            getline(fin[posicaoRegistro], linha);

            // Transformando a string para um vetor de char.
            const char *vetorLinha = linha.c_str();

            // Transformando a linha obtida para um registro.
            registros[posicaoRegistro] = stringParaRegistro(vetorLinha);
        }

        // Verificando se a posicaoLinha e a qtdLinhas são iguais. Se forem
        // não é dado um endl no final. Se não, ele é dado.
        if (qtdArquivosTerminaram == (qtdArquivos - 1)) {
            fout << menorRegistro.campoC << "," << menorRegistro.campoA;
        } else {
            fout << menorRegistro.campoC << "," << menorRegistro.campoA << endl;
        }
    }
    
    // Fechando o arquivo.
    fout.close();

    // Desalocando os vetores.
    delete[] fin;
    delete[] registros;
    delete[] terminou;
}

// Função que intercala registros[p...q-1] e registros[q...r-1]. ✓
void merge(registro *registros, int p, int q, int r) {
    int tam = r - p;
    registro *aux = new registro[tam];
    int i = p; // cursor 1.
    int j = q; // cursor 2.
    int k = 0; // cursor para aux.

    while (i < q && j < r) {
        if (strcmp(registros[i].campoC, registros[j].campoC) <= 0)
            aux[k++] = registros[i++];
        else
            aux[k++] = registros[j++];
    }

    while (i < q)
        aux[k++] = registros[i++];
    
    while (j < r)
        aux[k++] = registros[j++];
    
    for(k = 0; k < tam; k++)
        registros[p + k] = aux[k];
    
    delete []aux;
}

// Função que ordena o vetor registros entre as posições p e r-1. ✓
void mergeSort(registro *registros, int p, int r) {  
    if (p < r-1) {
        int meio = (p + r) / 2;

        mergeSort(registros, p, meio);
        mergeSort(registros, meio, r);
        merge(registros, p, meio, r); 
    }
}

// Função que ordena o vetor registros. ✓
void mergeSort(registro *registros, int n) {
    mergeSort(registros, 0, n);
}

// Função que deleta os arquivos utilizados na ordenação externa. ✓
void deletaArquivos(int qtdArquivos) {
    for (int i = 0; i < qtdArquivos; i++) {
        // Criando um nome para o arquivo.
        char nomeArquivo[255];
        sprintf(nomeArquivo, "dispositivo%d.txt", i);

        // Deletando o arquivo.
        remove(nomeArquivo);
    }
}

// Função que calcula a quantidade de linhas que o arquivo possui,
// com o intuito de saber quantos arquivos deverão ser criados. ✓
int quantidadeLinhas(char *nomeArquivo) {
    // Inicializando a variável que guarda a quantidade de linhas do arquivo.
    int qtdLinhas = 0;
    
    // Abrindo o arquivo.
    ifstream fin(nomeArquivo);

    // Verificando se o arquivo foi aberto. Se não foi aberto,
    // o programa emite uma mensagem de erro e finaliza.
    if (!fin.is_open()) {
        cerr << "ERRO AO ABRIR O ARQUIVO DE ENTRADA!" << endl;
        exit(1);
    }

    // Declarando a variável que guardará o conteúdo da linha.
    string linha;

    // Contando quantas linhas o arquivo possui.
    while (!fin.eof()) {
        getline(fin, linha);
        qtdLinhas++;       
    }

    // Contamos quantos registros o arquivo tem, por isso descartamos a linha
    // que possui o nome das colunas.
    qtdLinhas--;

    // Fechando o arquivo
    fin.close();

    // Retornando a quantidade de linhas.
    return qtdLinhas;
}

// Função que calcula a quantidade de colunas que o arquivo possui. ✓
int quantidadeColunas(char *nomeArquivo) {
    // Inicializando a variável que guarda a quantidade de colunas do arquivo.
    int qtdColunas = 1;
    
    // Abrindo o arquivo.
    ifstream fin(nomeArquivo);

    // Verificando se o arquivo foi aberto. Se não foi aberto,
    // o programa emite uma mensagem de erro e finaliza.
    if (!fin.is_open()) {
        cerr << "ERRO AO ABRIR O ARQUIVO DE ENTRADA!" << endl;
        exit(1);
    }

    // Declarando a variável que guardará o conteúdo da linha.
    string linha;

    // Guardando a primeira linha na variável linha.
    getline(fin, linha);
    
    // Transformando a string para um vetor de char.
    const char *primeiraLinha = linha.c_str();

    // Contando quantas colunas a primeira linha possui.
    for (int i = 0; primeiraLinha[i] != '\0'; i++) {
        if (primeiraLinha[i] == ',') qtdColunas++;
    }

    // Fechando o arquivo
    fin.close();

    // Retornando a quantidade de colunas.
    return qtdColunas;
}

// Função que calcula a posição do campo informado. ✓
int posicaoCampo(char *nomeArquivo, char *campo) {
    // Abrindo o arquivo
    ifstream fin(nomeArquivo);

    // Verificando se o arquivo foi aberto. Se não foi aberto,
    // o programa emite uma mensagem de erro e finaliza.
    if (!fin.is_open()) {
        cerr << "ERRO AO ABRIR O ARQUIVO DE ENTRADA!" << endl;
        exit(1);
    }

    // Declarando a variável que guardará o conteúdo da linha.
    string linha;
    
    // Guardando a primeira linha na variável linha.
    getline(fin, linha);
    
    // Transformando a string para um vetor de char.
    const char *primeiraLinha = linha.c_str();

    // Inicializando a variável colunas.
    int colunas = quantidadeColunas(nomeArquivo);

    // Criando a variável linhaVetor. Esta variável basicamente terá o conteúdo da linha
    // sem as vírgulas.
    char linhaVetor[colunas][255];

    // Declarando variáveis auxiliares.
    int j = 0, k = 0;

    // Guardando o conteúdo da linha sem as vírgulas.
    for (int i = 0; primeiraLinha[i] != '\0'; i++) {
        if (primeiraLinha[i] == ',') {
            linhaVetor[j][k] = '\0';
            j++;
            k = 0;
        } else {
            linhaVetor[j][k] = primeiraLinha[i];
            k++;
        }
    }

    linhaVetor[j][k] = '\0';

    // Criando a variável posição que guardará a posição do campo.
    int posicao;

    // Identificando em qual coluna o campo está.
    for (int i = 0; i < colunas; i++) {
        if (strcmp(linhaVetor[i], campo) == 0) {
            posicao = i;
            break;
        }
    }

    // Fechando o arquivo.
    fin.close();
    
    // Retornando a posição.
    return posicao;
}

// Função de copia APENAS as informações importantes do arquivoOrigem para o dispositivo0.txt. ✓
void copiaArquivo(char *arquivoOrigem, int qtdLinhas, int posicaoC, int posicaoA) {
     // Abrindo dois arquivos.
    ifstream fin(arquivoOrigem);
    ofstream fout("dispositivo0.txt");

    // Verificando se o arquivo foi aberto. Se não foi aberto,
    // o programa emite uma mensagem de erro e finaliza.
    if (!fin.is_open()) {
        cerr << "ERRO AO ABRIR O ARQUIVO DE ENTRADA!" << endl;
        exit(1);
    }

    // Verificando se o arquivo foi aberto. Se não foi aberto,
    // o programa emite uma mensagem de erro e finaliza.
    if (!fout.is_open()) {
        cerr << "ERRO AO ABRIR O ARQUIVO DE SAIDA!" << endl;
        exit(1);
    }

    // Declarando a variável que guardará o conteúdo da linha.
    string linha;

    // Guardando a primeira linha na variável linha. Porém, vamos ignorá-lo.
    getline(fin, linha);

    // Declarando a variável que guardará qual linha está sendo lida.
    int posicaoLinha = 0;

    // Copiando o conteúdo do nomeArquivo para o dispositivo0.txt
    while (!fin.eof()) {
        getline(fin, linha);

        // Transformando a string para um vetor de char.
        const char *vetorLinha = linha.c_str();

        // Inicializando a variável colunas.
        int colunas = quantidadeColunas(arquivoOrigem);

        // Criando a variável linhaVetor. Esta variável basicamente terá o conteúdo da linha
        // sem as vírgulas.
        char linhaVetor[colunas][255];

        // Declarando variáveis auxiliares.
        int j = 0, k = 0;

        // Guardando o conteúdo da linha sem as vírgulas.
        for (int i = 0; vetorLinha[i] != '\0'; i++) {
            if (vetorLinha[i] == ',') {
                linhaVetor[j][k] = '\0';
                j++;
                k = 0;
            } else {
                linhaVetor[j][k] = vetorLinha[i];
                k++;
            } 
        }

        // Definindo o '\0' da última coluna.
        linhaVetor[j][k] = '\0';

        // Incrementando a variável posicaoLinha.
        posicaoLinha++;

        // Verificando se a posicaoLinha e a qtdLinhas são iguais. Se forem
        // não é dado um endl no final. Se não, ele é dado.
        if (posicaoLinha == qtdLinhas) {
            fout << linhaVetor[posicaoC] << "," << linhaVetor[posicaoA];
        } else {
            fout << linhaVetor[posicaoC] << "," << linhaVetor[posicaoA] << endl;
        }

    }

    // Fechando os dois arquivos.
    fin.close();
    fout.close();
}

// Função que quebra o dispositivo0.txt em vários outros dispositivos ordenados. ✓
void quebraArquivo(int capacidadeMemoria) {
    // Abrindo o primeiro arquivo.
    ifstream fin("dispositivo0.txt");

    // Verificando se o arquivo foi aberto. Se não foi aberto,
    // o programa emite uma mensagem de erro e finaliza.
    if (!fin.is_open()) {
        cerr << "ERRO AO ABRIR O ARQUIVO DE ENTRADA!" << endl;
        exit(1);
    }

    // Declarando a variável posicaoDispositivo para saber em qual
    // dispositivo está aberto.
    int posicaoDispositivo = 1;

    // Abrindo o arquivo.
    ofstream fout("dispositivo1.txt");

    // Verificando se o arquivo foi aberto. Se não foi aberto,
    // o programa emite uma mensagem de erro e finaliza.
    if (!fout.is_open()) {
        cerr << "ERRO AO ABRIR O ARQUIVO DE SAIDA!" << endl;
        exit(1);
    }

    // Declarando um vetor de registos que será escrito no
    // arquivo de saída caso ele "estoure" ou caso o arquivo de leitura termine.
    registro *registros = new registro[capacidadeMemoria];
    int qtdRegistros = 0;

    // Declarando a variável que guardará o conteúdo da linha.
    string linha;

    // Quebrando o conteúdo do dispositivo0.txt para os outros dispositivos.
    while (!fin.eof()) {
        // Pegando uma linha.
        getline(fin, linha);

        // Transformando a string para um vetor de char.
        const char *vetorLinha = linha.c_str();

        // Transformando a linha obtida para um registro.
        registro registro = stringParaRegistro(vetorLinha);

        // Colocando o registro no vetor registros.
        registros[qtdRegistros] = registro;
        qtdRegistros++;

        // Se a quantidade de registros for igual a capacidade de memória
        // o vetor registros é escrito no arquivo de saída
        if (qtdRegistros == capacidadeMemoria) {
            // Ordena o vetor registros.
            mergeSort(registros, qtdRegistros);

            // Escreve o vetor registros no arquivo de saída.
            for (int i = 0; i < qtdRegistros; i++) {
                // Verificando se i e a qtdRegistros - 1 são iguais. Se forem
                // não é dado um endl no final. Se não, ele é dado.
                if (i == qtdRegistros - 1) {
                    fout << registros[i].campoC << "," << registros[i].campoA;
                } else {
                    fout << registros[i].campoC << "," << registros[i].campoA << endl;
                }
            }

            // Fechando o arquivo.
            fout.close();

            // Se o arquivo de leitura não acabou, vamos preencher o vetor registros
            // com os novos registros e escrever este vetor em um NOVO arquivo de saída.
            if (!fin.eof()) {
                 // Inicializando a quantidade de registros.
                qtdRegistros = 0;

                // Incrementando a variável posicaoDispotivo.
                posicaoDispositivo++;

                // Criando um nome para o arquivo.
                char nomeArquivo[255];
                sprintf(nomeArquivo, "dispositivo%d.txt", posicaoDispositivo);

                // Abrindo o arquivo.
                fout.open(nomeArquivo);

                // Verificando se o arquivo foi aberto. Se não foi aberto,
                // o programa emite uma mensagem de erro e finaliza.
                if (!fout.is_open()) {
                    cerr << "ERRO AO ABRIR O ARQUIVO DE SAIDA!" << endl;
                    exit(1);
                }
            }

        } else if (fin.eof()) { // Se o arquivo de entrada acabar, o vetor registros é escrito do arquivo de saída.
            // Ordena o vetor registros.
            mergeSort(registros, qtdRegistros);

            // Escreve o vetor registros no arquivo de saída.
            for (int i = 0; i < qtdRegistros; i++) {
                // Verificando se i e a qtdRegistros - 1 são iguais. Se forem
                // não é dado um endl no final. Se não, ele é dado.
                if (i == qtdRegistros - 1) {
                    fout << registros[i].campoC << "," << registros[i].campoA;
                } else {
                    fout << registros[i].campoC << "," << registros[i].campoA << endl;
                }
            }
            
            // Fechando o arquivo de saída.
            fout.close();
        }
    }

    // Desalocando o vetor de registros.
    delete[] registros;

    // Fechando o arquivo de entrada.
    fin.close();
}

// Função que realiza a ordenação externa do arquivo. ✓
int ordenaArquivo(char *nomeArquivo, int capacidadeMemoria, char *campoC, char *campoA) {
    // Declarando as variáveis que auxiliarão na ordenação externa.
    int qtdLinhas = quantidadeLinhas(nomeArquivo);
    int qtdArquivos = (qtdLinhas / capacidadeMemoria) + 1;
    int posicaoC = posicaoCampo(nomeArquivo, campoC);
    int posicaoA = posicaoCampo(nomeArquivo, campoA);

    // Verificando se a quantidade de arquivos criados é suficiente.
    if (qtdLinhas % capacidadeMemoria != 0) {
        qtdArquivos++;
    }

    // Copiando o conteúdo IMPORTANTE do nomeArquivo para o dispositivo0.txt
    copiaArquivo(nomeArquivo, qtdLinhas, posicaoC, posicaoA);

    // Quebra o dispositivo0.txt em vários outros dispositivos ordenados.
    quebraArquivo(capacidadeMemoria);

    // Intercala os qtdArquivos arquivos ordenados.
    mergeExterno(qtdArquivos);

    // Retornando a quantidade de arquivos criados para, posteriormente, deletá-los.
    return qtdArquivos;
}

// Função que imprime a média. ✓
void imprimeMedia() {
    // Criando as variáveis que auxiliarão no cálculo da média.
    char campoC[255];
    double somaMedia = 0;
    int qtdMedia = 0;
    double media;

    // Abrindo o arquivo.
    ifstream fin("dispositivo0.txt");

    // Verificando se o arquivo foi aberto. Se não foi aberto,
    // o programa emite uma mensagem de erro e finaliza.
    if (!fin.is_open()) {
        cerr << "ERRO AO ABRIR O ARQUIVO DE ENTRADA!" << endl;
        exit(1);
    }

    // Declarando a variável que guardará o conteúdo da linha.
    string linha;

    // Pegando a primeira linha.
    getline(fin, linha);

    // Transformando a string para um vetor de char.
    const char *vetorLinha = linha.c_str();

    // Inicializando um registro.
    registro registro = stringParaRegistro(vetorLinha);

    // Copiando o campoC do registro para o campoC. Acumulando a soma da média
    // e a quantidade.
    strcpy(campoC, registro.campoC);
    somaMedia += atof(registro.campoA);
    qtdMedia++;

    while (!fin.eof()) {
        // Pegando uma linha.
        getline(fin, linha);

        // Transformando a string para um vetor de char.
        vetorLinha = linha.c_str();
        
        // Transformando a linha obtida para um registro.
        registro = stringParaRegistro(vetorLinha);

        // Se o campoC do registro for igual a variável campoC,
        // apenas acumulamos a soma da média e a quantidade.
        if (strcmp(registro.campoC, campoC) == 0) {
            somaMedia += atof(registro.campoA);
            qtdMedia++;
        } else { // Se eles forem diferentes, calculamos a média e imprimimos. E começamos a calcular a próxima média.
            media = somaMedia / qtdMedia;
            cout << campoC << ", " << fixed << setprecision(15) << media << endl;
            strcpy(campoC, registro.campoC);
            somaMedia = atof(registro.campoA);
            qtdMedia = 1;
        }
    }

    // Imprimindo a média do último campoC.
    media = somaMedia / qtdMedia;
    cout << campoC << ", " << fixed << setprecision(15) << media << endl;

    // Fechando o arquivo.
    fin.close();
}

// Função main do programa. ✓
int main(int argc, char **argv) {
    
    // Verificando se todos os argumentos foram passados. Se estiver faltando algum,
    // o programa emite uma mensagem de erro e finaliza.
    if (argc != 5) {
        cerr << "ERRO, USE: ./agg nomeArquivo capacidadeMemoria campoC campoA" << endl;
        exit(1);
    }

    // Declarando as variáveis recebidas por linha de comando.
    char *nomeArquivo = argv[1];
    int capacidadeMemoria = atoi(argv[2]);
    char *campoC = argv[3];
    char *campoA = argv[4];

    // Realizando a ordenação externa.
    int qtdArquivos = ordenaArquivo(nomeArquivo, capacidadeMemoria, campoC, campoA);

    // Imprimindo a média.
    cout << campoC << ", " << campoA << endl;
    imprimeMedia();
    
    // Deletando os arquivos utilizados na ordenação externa.
    deletaArquivos(qtdArquivos);

    return 0;
}