#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<ctype.h>

#define levmax 3
#define amax 2047
#define stacksize 30
#define Tam 30

int  Monta_arvore   (void);
int  base           (int , int);
int  resto          (int);
void gravar         (void);
void Post_order     (int);
void execute        (void);
void operateOnStack (void);
void exibirPilha    (void);

enum instr{INT, STO, LIT, LOD, OPR, CAL, FCH, JMP, JPC };

typedef struct {
    int f;
    int l;
    int a; // valor
} Tinstruction;

int s[stacksize];   //piljha de execução
char arvore[Tam];   // vetor que representa a arvore
char polonesa[Tam];      // vetor que repreenta a notação polonesa reversa
int j=0;                // variavel auxiliar
int tam;                // variavel que diz o tamanho exato da notação polonesa
int k=0;                // variavel auxiliar


// Registradores
int bas =1;
int top =0;
int pos =0;
Tinstruction ins;

// Vetor de instruções
Tinstruction code[stacksize];

int main(){

    Monta_arvore();
    Post_order(0);
    gravar();

    char url[] = "polish.txt";
    FILE *arq;

    arq = fopen(url,"r");
    if ( arq == NULL)
            printf("Erro, não foi possivel abrir o arquivo\n");
    else
            fgets(polonesa,30,arq);

    fclose(arq);
    int i;
    printf("Notacao polonesa reversa: %s\n",polonesa);
    tam = strlen(polonesa);
    for(i=0;i<tam;i++){

        if (isdigit(polonesa[i])){
            code[i].f = LIT;    code[i].l = 0;       code[i].a = polonesa[i]-48;
            }
        else if (polonesa[i] == '+'){
            code[i].f = OPR;    code[i].l = 0;       code[i].a = 2;
             }
        else if (polonesa[i] == '~'){
            code[i].f = OPR;    code[i].l = 0;       code[i].a = 1;
             }
        else if (polonesa[i] == '-'){
            code[i].f = OPR;    code[i].l = 0;       code[i].a = 3;
             }
        else if (polonesa[i] == '*'){
            code[i].f = OPR;    code[i].l = 0;       code[i].a = 4;
             }
        else if (polonesa[i] == '/'){
            code[i].f = OPR;    code[i].l = 0;       code[i].a = 5;
             }
        else
            continue;
    }

    //Inicialização
    bas = 0;
    top = -1; //topo é -1 porque a pilha inicia vazia
    pos = 0;

    s[1]=0;
    s[2]=0;
    s[3]=0;

    execute();
    //exibirPilha();

    return 0;
}
int Monta_arvore(){  // abre o arquivo para montar o array/arvore

    char url[] = "arvore.txt";
    FILE *arq;
    int i;

    arq = fopen(url,"r");
    if ( arq == NULL)
        printf("Erro, não foi possivel abrir o arquivo\n");
    else
        fgets(arvore,Tam,arq);

    fclose(arq);
}

void Post_order(int i) {  // percorre o vetor em pós ordem
    if (arvore[i] == NULL)
        return;
    Post_order(2*i+1);
    Post_order(2*i+2);
    if(arvore[i]!='X'){
        polonesa[j]= arvore[i];
        j++;
    }
}
void gravar(){  // coloca a notação polonesa gerada da arvore no arquivo
    char url[] = "polish.txt";
    FILE *arq;
    int i;

    arq = fopen(url,"w");
    if ( arq == NULL)
        printf("Erro, não foi possivel criar o arquivo\n");
    else
        fputs(polonesa,arq);

}
// Retorna o endereço base da pilha
int base(int l, int base) {
    int b1;
    b1 = base;
    while(l > 0){
      b1 = s[b1];
      l--;
    }
    return b1;
}

void exibirPilha(){
            int i;
            printf("f[%d]   ",code[pos-1].f);
            printf("l[%d]   ",code[pos-1].l);
            printf("a[%d]   ",code[pos-1].a);
            printf("t[%d]   ",top);
            printf("p[%d]   ",pos);
            printf("b[%d]   ",bas);
            printf("\nStack ");
            for(i = 0; i <= tam/2; i++){
                printf("[%d]\t",s[i]);
            }
            printf("\n");
}
// Executa as instruções
void execute(){

    while(pos < stacksize){
    ins = code[pos];
    pos++;

    switch(ins.f){
        case LIT :      //Coloca um numero na pilha
            top++;
            s[top] = ins.a;
            if(k < tam){
            printf("\n--> LIT\n");
            exibirPilha();
            k++;
            }
            break;

        case FCH :      //Pula uma interação
            if(k < tam){
            printf("\n--> FCH\n");
            exibirPilha();
            k++;
            }
            break;

        case OPR :      //executa uma operação aritimetica
            operateOnStack();

            if(k < tam){
            printf("\n--> OPR\n");
            exibirPilha();
            k++;
            }
            break;

        case LOD :      // carrega o topo da pilha com um elemento dela
            top++;
            s[top] = s[base(ins.l,bas) + ins.a];
            if(k < tam){
            printf("\n--> LOD\n");
            exibirPilha();
            k++;
            }
            break;

        case STO :      // retira um elemento do topo e poe na posição de a
            s[base(ins.l,bas) + ins.a] = s[top];
            top--;
            if(k < tam){
            printf("\n--> STO\n");
            exibirPilha();
            k++;
            }
            break;

        case INT :       // reserva espaços na pilha
            top = top + ins.a;
            if(k < tam){
            printf("\n--> INT\n");
            exibirPilha();
            k++;
            }
            break;

        case CAL:
            s[top + 1] = base(ins.l, bas);   //static link
            s[top + 2] = bas;                //dynamic link
            s[top + 3] = pos;                //return address
            bas = top+ 1;
            pos = ins.a;
            if(k < tam){
            printf("\n--> CAL\n");
            exibirPilha();
            k++;
            }
            break;

        case JMP :      // pula para uma determinada instrução
            pos = ins.a;
            if(k < tam){
            printf("\n--> JMP\n");
            exibirPilha();
            k++;
            }
            break;

        case JPC :      // pula instruções
            if(s[top] == 0){
                pos = ins.a;
            }
            top--;
            if(k < tam){
            printf("\n--> JPC\n");
            exibirPilha();
            k++;
            }
            break;
        }
    }
}
//Faz a operação de resto do comando OPR
int resto( int num){
    if ((num%2) == 0)
        return 0;
    else
        return 1;
}
// Listas de operações que podem ser feitas
void operateOnStack() {
   switch (ins.a) {
      case 1:
         s[top] *= -1;
         break;
      case 2:
         top--;
         s[top] = s[top] + s[top + 1];
         break;
      case 3:
         top--;
         s[top] = s[top] - s[top + 1];
         break;
      case 4:
         top--;
         s[top] = s[top] * s[top + 1];
         break;
      case 5:
         top--;
         s[top] = s[top] / s[top + 1];
         break;
      case 6:
         s[top] = resto(s[top]);
         break;
      case 7:
         top--;
         s[top] = s[top] == s[top + 1];
         break;
      case 8:
         top--;
         s[top] = (s[top] != s[top + 1]);
         break;
      case 9:
         top--;
         s[top] = (s[top] < s[top + 1]);
         break;
      case 10:
         top--;
         s[top] = (s[top] >= s[top + 1]);
         break;
      case 11:
         top--;
         s[top] = (s[top] > s[top + 1]);
         break;
      case 12:
         top--;
         s[top] = (s[top] <= s[top + 1]);
         break;
   }
}
