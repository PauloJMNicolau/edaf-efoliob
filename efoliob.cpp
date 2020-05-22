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

struct ERRPOSINVALIDA: public exception{
    const char* what() const throw(){
        return "Posição Invalida!";
    }
};


//Classe que implementa um vector "semi-dinámico" para uma Heap
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
            array[quantidade] = valor;
            quantidade++;
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
        array[quantidade-1] = -1;
        quantidade--;
        return true;
    }

    //Remover Primeiro elemento do vetor
    bool removerPrimeiro(){
        array[0]=array[quantidade-1]; //Copia ultimo elemento para o inicio
        return remover(); //Remove Ultimo elemento
    }

    //Obter capacidade do vetor
    const int obterCapacidade() const{
        return capacidade;
    }

    //Obter Quantidade de elemetos no vetor
    const int obterQuantidade() const{
        return quantidade;
    }

    //Alterar o Valor da Quantidade de elementos no vetor
    const bool alterarQuantidade(int valor){
        if(valor <= capacidade){
            quantidade= valor;
            return true;
        }
        return false;
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
        bool compararValores(int filho, int pai){
            if(filho > pai){
                return true;
            }
            return false;
        }

        //Trocar elementos de posição
        bool trocarValores(int posicaoInicial, int posicaoFinal){
            if(posicaoInicial < heap->obterQuantidade() && posicaoFinal< heap->obterQuantidade()){
                int aux = heap->obterElemento(posicaoInicial);
                heap->alterarElemento(heap->obterElemento(posicaoFinal), posicaoInicial);
                heap->alterarElemento(aux, posicaoFinal);
                return true;
            }
            return false;
        }

        //Determinar indice pai do indice fornecido
        const int obterPosicaoPai(int indice)const{
            if(indice > 0){
                return (int((indice+1) /2)-1); //Retorna o menor numero inteiro que resulta da divisão do indice por 2 
            }
            return -1; //Como o indice é a raiz da arvore não tem pai ou é inválido
        }

        //Determina indice do filho do lado esquerdo da arvore do indice fornecido
        const int obterPosicaoFilhoEsquerdo(int indice)const{
            if(indice >= 0 && indice < heap->obterQuantidade()){
                return 2*indice+1;
            }
            return -1;
        }

        //Determina indice do filho do lado direito da arvore do indice fornecido
        const int obterPosicaoFilhoDireito(int indice)const{
            if(indice >= 0 && indice < heap->obterQuantidade()){
                return 2*indice+2;
            }
            return -1;
        }

        //Inicializa posições do vetor
        const bool inicializarHeap(){
            for(int i = 0; i < heap->obterCapacidade(); i++){
                heap->alterarElemento(-1,i);
            }
            return true; 
        }

        //Coloca o valor da ultima posição do vetor na ordem correta
        const bool restaurarOrdemHeap(int posicao){
            int filhoMaior = posicaoMaior(obterPosicaoFilhoEsquerdo(posicao), obterPosicaoFilhoDireito(posicao));
            int maior = posicao;
            maior = posicaoMaior(maior, filhoMaior);
            if(maior != posicao){
                trocarValores(posicao, maior);
                restaurarOrdemHeap(maior);
            }
            return true;
        }

        //Retornar posição com valor maior
        const int posicaoMaior(int posicaoEsquerda, int posicaoDireita) const {
            //Verificar se posições são validas
            if((posicaoEsquerda >= heap->obterQuantidade() || posicaoEsquerda <= 0) 
            && (posicaoDireita >= heap->obterQuantidade() || posicaoDireita <= 0)){
                return -1;
            } 
            int maior=-1;
            if(posicaoEsquerda < heap->obterQuantidade() || posicaoEsquerda >= 0){
                maior= posicaoEsquerda;
            }
            if(posicaoDireita < heap->obterQuantidade() || posicaoDireita >= 0){
                if(maior < posicaoDireita){
                    maior = posicaoDireita;
                }
            }
            return maior;
        }

        //Retorna a altura da Heap
        const int alturaHeap(){
            int altura=0;
            int quantidade= heap->obterQuantidade();
            while((quantidade /= 2) >1){
                altura++;
            }
            return altura;
        }

        //Calcula elementos do Nivel da Heap
        const int elementosDoNivel(int nivel){
            int elementos=1;
            for(int i=0; i< nivel; i++){
                elementos*=2;
            }
            return elementos;
        }

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
            //Como não está cheio então insere no vetor
            if(!heap->adicionar(valor)){
                throw ERRHEAPMAX(); //Erro por Vetor Cheio
            }
            restaurarOrdemHeap(heap->obterQuantidade()-1);
            return true;
        }

        //Retorna o elemento do topo da arvore
        const int elementoMaximo() const{
            return heap->obterElemento(0);
        }

        //Retorna a dimensão atual da arvore
        const int obterDimensao() const{
            return heap->obterQuantidade();
        }

        //Obter a capacidade atual da arvore
        const int obterCapacidadeHeap() const{
            return heap->obterCapacidade();
        }

        //Reinicia os valores de toddas as posições do Heap
        const bool reinicializarHeap(){
            if(heap->obterQuantidade() > 0){
                if(heap->alterarQuantidade(0))
                    return inicializarHeap();
            }
            return false;
        }

        //Redimensionar Heap da árvore
        const bool redimensionarHeap(int valor){
            if(valor >0){
                return heap->redimensionar(valor);
            }
            return false;
        }

        //Apagar Elemento da Heap
        const bool remover(){
            if(heap->obterQuantidade() ==0){
                throw ERRHEAPMIN();
            }
            if(heap->obterQuantidade() ==1){
                heap->remover();
            } else{
                heap->removerPrimeiro();
                restaurarOrdemHeap(0);
            }
        }

        //Restaura a ordem em toda a Heap
        const bool heapify_up(){
            for(int i = heap->obterQuantidade() /2; i>0; i--){
                restaurarOrdemHeap(i-1);
            }
            return true;
        }

        //Retorna uma string com o formato de arvore
        const string imprimirHeap(){
            string aux;
            int altura = alturaHeap();
            int indice =0;
            for(int i=0; i< altura; i++){
                aux.append("\n");
                for(int e=0; e< elementosDoNivel(i); e++){
                    if(indice< heap->obterQuantidade()){
                        aux.append(to_string(heap->obterElemento(indice)));
                        indice++;
                        if(e<elementosDoNivel(i)-1){
                            aux.append(" ");
                        }
                    }
                }
            }
            return aux;
        }
};

//Classe que gere a execução dos comandos que estão no ficheiro
class INTERFACE{
    private:
        BTREEMAXHEAP * heap;
        string comando;
        string argumentos;


        //Executar Comando Inserir
        void insert(){
            try{
                int aux;
                bool res;
                if(argumentos != " "){
                    istringstream arg(argumentos);
                    while(arg >> aux){
                        res = heap->inserirElemento(aux);
                    } 
                } else{
                    cout << "Comando " << comando<< ": Não existem valores a introduzir!"<<endl;
                }
            }catch (ERRHEAPMAX e){
                cout << "Comando " <<comando <<": " <<e.what() <<endl;
            }

        }

        void print(){
    int aux;
                bool res;
                if(argumentos != " "){
                    istringstream arg(argumentos);
                    while(arg >> aux){
                        res = heap->inserirElemento(aux);
                    } 
                } else{
                    cout << "Comando " << comando<< ": Não existem valores a introduzir!"<<endl;
                }
        }

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