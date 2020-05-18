#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <exception>

using namespace std;

//Estrutura de Erros para Heap vazio
struct ERRHEAPMIN: public exception{
    const char* what() const throw(){
        return "Heap vazio!";
    }

};

//Estrutura de Erros para Heap Cheio
struct ERRHEAPMAX: public exception{
    virtual const char* what() const throw(){
        return "Heap cheio!";
    }
};


//Classe que implementa um vector "semi-dinámico"
//Vector não aumenta automaticamente quando atinge a capacidade maxima
//Vector elimina todos os valores armazenados quando é redimencionado
class VECTOR{
private:
    int *array;     //Ponteiro para a primeira posição do array
    int capacidade; //Capacidade que o vetor permite armazenar
    int quantidade; //Elementos atualmente no array
public:
    //Construtor do Vector
    VECTOR(){
        array = new int[15];
        capacidade = 15;
        quantidade = 0;
        //Inicializa o vetor com valores inválidos
        for (int i = 0; i < capacidade; i++){
            array[i] = -1;
        }
    }

    //Destrutor do Vetor
    ~VECTOR(){
        //Limpa as posições de memoria utilizadas
        for (int i = 0; i < capacidade; i++){
            array[i] = -1;
        }
        delete[] array;
        capacidade = 0;
        quantidade = 0;
    }

    //Adicionar elementos ao vector
    bool adicionar(int valor){
        if (quantidade < capacidade){
            quantidade++;
            array[quantidade] = valor;
            return true;
        }
        return false;
    }

    //Adiciona um elemento na posição
    bool adicionar(int valor, int posicao){
        if (posicao >= 0 && posicao < quantidade){
            if (quantidade < capacidade){
                for (int i = quantidade; i >= posicao; i--){
                    array[i + 1] = array[i];
                }
                array[posicao] = valor;
                quantidade++;
                return true;
            }
        }
        return false;
    }

    //Remover ultimo elemento do vetor
    bool remover(){
        array[quantidade] = -1;
        quantidade--;
        return true;
    }

    //Remover elemento do vetor na posição
    bool remover(int posicao){
        if (posicao >= 0 && posicao < quantidade){
            for (int i = posicao; i < quantidade - 1; i++){
                array[i] = array[i + 1];
            }
            array[quantidade] = -1;
            quantidade--;
        }
        return false;
    }

    //Obter capacidade do vetor
    const int obterCapacidade() const{
        return capacidade;
    }

    //Obter Quantidade de elemetos no vetor
    const int obterQunatidade() const{
        return quantidade;
    }

    //Obter elemento na posição do vector
    //Retorna -1 se não encontrar
    const int obterElemento(int posicao) const{
        if (posicao >= 0 && posicao < quantidade){
            return array[posicao];
        }
        return -1;
    }

    //Altera o valor do elemento na posiçao do vetor
    bool alterarElemento(int valor, int posicao){
        if (posicao >= 0 && posicao < quantidade){
            array[posicao] = valor;
            return true;
        }
        return false;
    }

    //Permite redimensionar o Vetor
    bool redimensionar(int tamanho){
        if (tamanho > 0){
            delete[] array;
            array = new int[tamanho];
            capacidade = tamanho;
            quantidade = 0;
            //Inicializa o vetor com valores inválidos
            for (int i = 0; i < capacidade; i++){
                array[i] = -1;
            }
            return true;
        }
        return false;
    }
};

//Classe Arvore Binária com Max Heap
class BTREEMAXHEAP{
    private:
        VECTOR *heap; //Vector do Heap da arvore

        //Comparar se primeiro elemento é maior do que o segundo
        //Serve para compararse o filho é maior do que o pai
        bool

        //Trocar elementos de posição



    public:
        //Construtor da Arvore Max-Heap
        BTREEMAXHEAP(){
            heap = new VECTOR();
        }

        //Destrutorda Árvore Max-Heap
        ~BTREEMAXHEAP(){
            delete (heap);
        }

        //Inserir elementos na arvore
        bool inserirElemento(int valor){

        }
};

//Classe que gere a execução dos comandos que estão no ficheiro
class INTERFACE{
    private:
        string comando;
        string argumentos;

    public:
        //Constroi a interface
        INTERFACE(){
            this->comando = "";
            this->argumentos = "";
        }

        //Destroi a interface
        ~INTERFACE(){
            this->comando = "";
            this->argumentos = "";
        }

        //Atualiza o valor de comando
        void inserirComando(string comando){
            this->comando = comando;
        }

        //Atualiza o valor de argumentos
        void inserirArgumentos(string argumentos){
            this->argumentos = argumentos;
        }

        //Executa o comando respectivo
        void executaComando(){
            if (comando == "insert"){
                //insert();
            } else {
                cout << "Comando não suportado!" << endl; // Caso o comando não seja válido
            }
        }
};

int main(){
    string linha;
    INTERFACE *consola = new INTERFACE();

    /*/Código HR

    while(getline(cin,linha)){//Le a entrada CIN
        if(linha.substr(0,1)== "#" || linha ==""){
            continue;//Verifica se é uma linha vazia ou comentada e ignora
        }
        int pos = linha.find_first_of(" ", 0);//Procura o primeiro espaço para verificar onde termina o nome do comando
        consola->inserirComando(linha.substr(0,pos)); //Insere o nome do comando na interface
        pos = linha.find_first_not_of(" ",pos);//Procura o ultimo espaço após a posição anterior para verificar onde começa os argumentos
        if(pos!=-1){
            consola->inserirArgumentos(linha.substr(pos,linha.size())); // Se a posição for válida copia os argumentos para a interface
        }
        consola->executaComando(); //Executa o comando
    }

    //Fim Codigo HR */

    //Código IDE

    fstream file;
    file.open("cmd.txt", ios::in);
    while (getline(file, linha)){ //Lê o ficheiro até ao final
        if (linha.substr(0, 1) == "#" || linha == ""){
            continue; //Verifica se é uma linha vazia ou comentada e ignora
        }
        int pos = linha.find_first_of(" ", 0);         //Procura o primeiro espaço para verificar onde termina o nome do comando
        consola->inserirComando(linha.substr(0, pos)); //Insere o nome do comando na interface
        pos = linha.find_first_not_of(" ", pos);       //Procura o ultimo espaço após a posição anterior para verificar onde começa os argumentos
        if (pos != -1){
            consola->inserirArgumentos(linha.substr(pos, linha.size())); // Se a posição for válida copia os argumentos para a interface
        }
        consola->executaComando(); //Executa o comando
    }
    file.close();

    //Fim Código IDE*/
    return 0;
}