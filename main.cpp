#include <iostream>
#include <windows.h>
#include <time.h>
#define TAM 6

#define RESET "\033[0m"
#define RED "\033[31m"     
#define GREEN "\033[32m"       
#define MAGENTA "\033[35m"        
#define GREEN_BACKGROUND "\033[42m"
#define MAGENTA_BACKGROUND "\033[45m"

using namespace std;

typedef int stack_element;

#include "stack.h"
//#include "stack_ld.h"

typedef struct{
    int numero_elementos;
    Stack pilha;
} Tubo;

void cabecalho(){
    cout << MAGENTA << "\n";
    cout << "------------------------------------------" <<  endl;
    cout << MAGENTA_BACKGROUND << "              JOGO DAS PILHAS             " << RESET << endl;
    cout << MAGENTA << "------------------------------------------" << RESET << endl;
    cout << "  Empilhe todas as bolinhas de mesma cor  " << endl;
    cout << "           Digite -1 para sair            \n" << endl;
}

void vitoria(){
    cout << GREEN << "\n";
    cout << "------------------------------------------" <<  endl;
    cout << GREEN_BACKGROUND << "              V I T O R I A               " << RESET << endl;
    cout << GREEN << "------------------------------------------" << RESET << endl;
}

void iniciar_vazias(Tubo T[]){
    for(int i=0;i<TAM;i++){
        initStack(T[i].pilha);
        T[i].numero_elementos = 0;
    }
}

void mudacor(int cor){
    HANDLE Saida;
    Saida = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(Saida,cor);
}

void printa_cor(int I){
    int cor[5]={9,13,2,15,6};
    mudacor(cor[I-1]);
    cout << 'O';
    mudacor(15);
}

void distribuir(Tubo T[]){
    int freq[TAM-1] = {0};
    int num;

    srand(time(0));

    for(int j=0;j<(TAM-1);j++){
        for(int i=0;i<(TAM-1);i++){
            do{
                num = rand() % (TAM-1);
            }while(freq[num] >= (TAM-1));

            freq[num]++;
            
            push(T[i].pilha,num+1);

            T[i].numero_elementos++;
        }
    } 
}

void mostrar(Tubo T[]) {
    Tubo Aux[TAM];
    stack_element v;

    iniciar_vazias(Aux);

    cout << endl;

    for(int linha = (TAM-1); linha > 0; linha --){
        for(int i=0; i<TAM; i++){
            cout << " | ";
            if(T[i].numero_elementos +1 <= linha){
                cout << " ";
            }
            else if (!isEmpty(T[i].pilha)){
                v = pop(T[i].pilha);
                push(Aux[i].pilha,v);
                printa_cor(v);
            }
            cout << " | ";
        }
        cout << endl;
    }

    cout << " ";
    for (int i = 0; i < TAM; i++) {
        cout << "-----  ";
    }
    cout << endl;

    for (int i = 0; i < TAM; i++) {
        cout << "   " << i+1 << "   ";
    }
    cout << endl;

    for (int i = 0; i < TAM; i++) {
        while (!isEmpty(Aux[i].pilha)) {
            v = pop(Aux[i].pilha);
            push(T[i].pilha, v);
        }
    }
}

int validar(Tubo T[], int o, int d){
    if(T[o].numero_elementos == 0) return 1;
    if(T[d].numero_elementos >= (TAM-1)) return 2;
    return 0;
}

int validar_fim(Tubo T[]){
    Tubo Aux[TAM];
    stack_element topo,v;
    int flag=0;

    iniciar_vazias(Aux);
    
    for(int i=0; i<TAM; i++){
        if(!isEmpty(T[i].pilha) && T[i].numero_elementos < (TAM-1)){
            flag = 1;
        }
        if(!isEmpty(T[i].pilha) && T[i].numero_elementos == (TAM-1)){
            topo = pop(T[i].pilha);
            push(Aux[i].pilha,topo);
            while(!isEmpty(T[i].pilha)){
                v = pop(T[i].pilha);
                push(Aux[i].pilha,v);
                if(v != topo){
                    flag = 1;
                    break;
                }
            }
        }
        if(flag == 1) break;
    }

    for(int i=0; i<TAM; i++){
        while(!isEmpty(Aux[i].pilha)){
            v = pop(Aux[i].pilha);
            push(T[i].pilha,v);
        }
    }

    return flag;
}

int jogada(Tubo T[]){
    int origem, destino,retValidacao,retValFim;
    stack_element v;

    do{
        do{
            cout << MAGENTA << "\nORIGEM: " << RESET;
            cin >> origem;
            if(origem == -1){
                cout << "Encerrando...";
                exit(0);
            }
        }while(origem < 1 || origem > (TAM));

        do{
            cout << MAGENTA << "DESTINO: " << RESET;
            cin >> destino;
            if(destino == -1){
                cout << "Encerrando...";
                exit(0);
            }
        }while(destino < 1 || destino > (TAM));

        retValidacao = validar(T,(origem-1),(destino-1));
        if(retValidacao == 1) cout << RED << "\nOrigem Vazia." << RESET << endl;
        if(retValidacao == 2) cout << RED << "\nDestino cheio." << RESET << endl;
    }while(retValidacao != 0);

    v = pop(T[origem-1].pilha);
    push(T[destino-1].pilha,v);
    T[origem-1].numero_elementos --;
    T[destino-1].numero_elementos ++;
    
    retValFim = validar_fim(T);

    return retValFim;
}

int main(){
    Tubo T[TAM];
    int repetir = 1, retorno;

    do{
        iniciar_vazias(T);
        distribuir(T);
        do{
            system("cls");
            cabecalho();
            mostrar(T);
            retorno = jogada(T);
            if(retorno == 0) break;
        }while(retorno != 0);
        system("cls");
        vitoria();
        mostrar(T);
        do{
            cout << "\n\nDeseja jogar novamente?" << endl;
            cout << "1. Sim" << endl;
            cout << "2. Nao" << endl;
            cout << "-> ";
            cin >> repetir;
        }while(repetir < 1 || repetir > 2);
    }while(repetir != 2);

    cout << "\nFim de jogo!" << endl;

    return 0;
}