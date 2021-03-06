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

    //Adicionar elementos ao vector na ultima posição livre
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
        array[quantidade-1] = -1; //limpa o valor da ultima posição
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

        //Retornar posição com valor maior
        const int posicaoMaior(int posicao) const {
            //Atribui a posiçãão atual como sendo maior
            int maior = posicao;
            //Verifica se existe filho esquerdo e compara se este possui valor maior do que a posiçãão original
            int aux = obterPosicaoFilhoEsquerdo(posicao);
            if(aux < heap->obterQuantidade() && aux >= 0){
                if(heap->obterElemento(maior) < heap->obterElemento(aux)){
                    maior= aux;
                }
            }
            //Verifica se existe filho direito e compara se este possui valor maior do que a posição atualmente em maior
            aux = obterPosicaoFilhoDireito(posicao);
            if(aux < heap->obterQuantidade() && aux >= 0){
                if(heap->obterElemento(maior) < heap->obterElemento(aux)){
                    maior= aux;
                }
            }
            return maior;
        }

        //Coloca o valor da ultima posição do vetor na ordem correta
        //Ordem 0-inserir, 1 -remover
        const bool restaurarOrdemHeap(int posicao,int ordem){
            //Ordenar na ordem de folhas para raiz
            if(ordem ==0){
                if(posicao>0){
                    //Verifica se existe pai
                    int pai= obterPosicaoPai(posicao);
                    if(pai != -1){
                        //Verifica se valor de pai é menor que a do filho e troca se for menor
                        if(heap->obterElemento(pai)< heap->obterElemento(posicao)){
                            trocarValores(posicao,pai);
                            restaurarOrdemHeap(pai, ordem);
                        }
                    }
                    return true;
                }   
            } 
            //Ordenar na ordem de raiz para folhas
            else if(ordem ==1){
                if(posicao >=0){
                    //Obtem a posição maior entre o nó e os filhos
                    int maior = posicaoMaior(posicao);
                    //Verifica se a posição maior é a posição original
                    if(maior != posicao){
                        //Como posição maior mudou, então troca-se os valores e volta a executar-se a ordenação para o novo nó
                        trocarValores(posicao,maior);
                        restaurarOrdemHeap(maior, ordem);
                    }
                    return true;    
                }
            }
            return false;
        }
     
        //Retorna a altura da Heap
        const int alturaHeap(){
            int altura=1;
            int quantidade= heap->obterQuantidade();
            while((quantidade /= 2) >0){
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
            //Reordenar a arvore em ordem de folha para raiz
            restaurarOrdemHeap(heap->obterQuantidade()-1,0);
            return true;
        }

        //Retorna o elemento do topo da arvore
        const int elementoMaximo() const{
            if(heap->obterQuantidade()==0){
                throw ERRHEAPMIN();
            }
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
            } else{
                throw ERRHEAPMIN();
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
            //Se heap só tem um elemento então basta remover
            if(heap->obterQuantidade() ==1){
                heap->remover();
            } 
            //Se heap tiver mais elementos então é preciso reordenar os elemento na ordem de raiz para folha
            else{
                heap->removerPrimeiro();
                restaurarOrdemHeap(0,1);
            }
            return true;
        }

        //Restaura a ordem em toda a Heap
        const bool heapify_up(VECTOR * vetor){
            delete heap;
            heap = vetor;
            //Obtem o ultimo elemento que não é folha e percorre em ordem até à raiz
            for(int i = heap->obterQuantidade() /2; i>0; i--){
                restaurarOrdemHeap(i-1,1);
            }
            return true;
        }

        //Retorna uma string com o formato de arvore
        const string imprimirHeap(){
            if(heap->obterQuantidade()==0)
                throw ERRHEAPMIN();
            string aux;
            int altura = alturaHeap();
            int indice =0;
            //Percorre um ciclo até a altura maxima da arvore
            for(int i=0; i< altura; i++){
                aux.append("\n");
                //imprime a quantidade de elementos que devem ser mostrados no nivel atual
                for(int e=0; e< elementosDoNivel(i); e++){
                    //Verifica se o indice a imprimir pertence à arvore
                    if(indice< heap->obterQuantidade()){
                        aux.append(to_string(heap->obterElemento(indice)));
                        indice++;
                        //Se não for o ultimo elemento da linha imprime um espaço
                        if(e<elementosDoNivel(i)-1){
                            aux.append(" ");
                        }
                    }
                }
            }
            return aux;
        }
};

//Classe de No de uma fila (para heapify_up)
class NO{
    private:
        int valor;
        NO * proximo;
        NO * anterior;

    public:
        //Construtor No da Fila
        NO(int numero){
            valor = numero;
            proximo = this;
            anterior = this;
        }

        //Destrutor de No da fila
        ~NO(){
            valor=0;
            proximo = nullptr;
        }

        //Altera o valor de proximo do no
        bool alterarProximo(NO * novo){
            proximo = novo;
            return true;
        }

        //Alterar o valor de Anterior do no
        bool alterarAnterior(NO * novo){
            anterior = novo;
            return true;
        }

        //Obter o proximo nó ao no
        NO * obterProximo(){
            return proximo;
        }

        //Obter o nó anterior ao nó
        NO * obterAnterior(){
            return anterior;
        }

        //Obter o valor do nó
        const int obterValor(){
            return valor;
        }
};

//Classe para gestão de uma fila (para heapify_up)
class FILA{
    private:
        int elementos;
        NO * inicio;

    public:
        //Construtor da Fila
        FILA(){
            elementos = 0;
            inicio = nullptr;
        }

        //Destrutor a Fila
        ~FILA(){
            while(elementos >0){
                remover();
            }
        }

        //Inserir elementos na fila
        bool inserir(int valor){
            NO * novo = new NO(valor);
            if(elementos == 0){
                inicio = novo;
            } else{
                novo->alterarAnterior(inicio->obterAnterior());
                novo->alterarProximo(inicio);
                inicio->alterarAnterior(novo);
                novo->obterAnterior()->alterarProximo(novo);
            }
            elementos++;
            return true;
        }
        
        //Remover elementos na fila
        int remover(){
            if(elementos >0){
                int valor = inicio->obterValor();
                inicio->obterAnterior()->alterarProximo(inicio->obterProximo());
                NO * temp = inicio;
                inicio = inicio->obterProximo();
                delete temp;
                elementos--;
                return valor;
            }
            return -1;
        }
};

//Classe que gere a execução dos comandos que estão no ficheiro
//Reaproveitamento de código do Efolio A - Apenas com alterações para os comandos novos
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

        //Executa o comando de Imprimir
        void imprimir(){
            
            try{
                string texto = heap->imprimirHeap();
                cout << "Heap=" << texto <<endl;
            } catch (ERRHEAPMIN e){
                cout << "Comando " <<comando <<": " <<e.what() <<endl;
            }
        }

        //Executa comando de Imprimir valor Maximo
        void imprimirMax(){
            try{
                int max = heap->elementoMaximo();
                cout << "Max= " << max <<endl;
            } catch (ERRHEAPMIN e){
                cout << "Comando " <<comando <<": " <<e.what() <<endl;
            }
        }

        //Imprimir quantidade de elementos na arvore
        void imprimirDim(){
            cout << "Heap tem " << heap->obterDimensao() << " itens" <<endl;
        }

        //Imprimir quantidade de elementos na arvore
        void imprimirCapacidade(){
            cout << "Heap tem capacidade " << heap->obterCapacidadeHeap() << " itens" <<endl;
        }

        //Apaga todo o vetor
        void apagar(){
            try{
                heap->reinicializarHeap();
            } catch (ERRHEAPMIN e){
                cout << "Comando " <<comando <<": " <<e.what() <<endl;
            }
        }

        //Redimensiona o tamanho do vetor
        void redimensionar(){
            int aux;
            bool res;
            if(argumentos != " "){
                istringstream arg(argumentos);
                while(arg >> aux){
                    res = heap->redimensionarHeap(aux);
                } 
            } else{
                cout << "Comando " << comando<< ": Não existem valores a introduzir!"<<endl;
            }
        }

        //Executa Comando Remover
        void remover(){
            try{
                heap->remover();
            } catch(ERRHEAPMIN e){
                cout << "Comando " <<comando <<": " <<e.what() <<endl;
            }
        }

        //Executa comando Heapify_up
        void reconstruirVetor(){
            try{
                int aux;
                bool res;
                if(argumentos != " "){
                    VECTOR  * temp = new VECTOR();
                    int tamanho = 0;
                    FILA * auxFila = new FILA();
                    istringstream arg(argumentos);
                    while(arg >> aux){
                        tamanho++;
                        auxFila->inserir(aux);
                    } 
                    if(tamanho > temp->obterCapacidade()){
                        temp->redimensionar(tamanho);
                    }
                    while(tamanho!=0){
                        temp->adicionar(auxFila->remover());
                        tamanho--;
                    }
                    delete auxFila;
                    heap->heapify_up(temp);
                } else{
                    cout << "Comando " << comando<< ": Não existem valores a introduzir!"<<endl;
                }
            }catch (ERRHEAPMAX e){
                cout << "Comando " <<comando <<": " <<e.what() <<endl;
            }
        }

    public:
        //Constroi a interface
        INTERFACE(){
            this->comando = "";
            this->argumentos = "";
            this->heap=new BTREEMAXHEAP();
        }

        //Destroi a interface
        ~INTERFACE(){
            delete heap;
            heap = nullptr;
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
                insert();
            } else if(comando == "print"){
                imprimir();
            } else if(comando == "print_max"){
                imprimirMax();
            } else if(comando == "dim"){
                imprimirDim();
            } else if(comando == "dim_max"){
                imprimirCapacidade();
            } else if(comando == "clear"){
                apagar();
            } else if(comando == "delete"){
                remover();
            } else if(comando == "heapify_up"){
                reconstruirVetor();
            } else if(comando == "redim_max"){
                redimensionar();
            } else
                cout << "Comando não suportado!" << endl; // Caso o comando não seja válido
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