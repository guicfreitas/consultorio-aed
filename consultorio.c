#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define ORDEM 2

typedef struct no No;
typedef struct fila Fila;
typedef struct paciente Paciente;
typedef struct terapeuta Terapeuta;

char nomes [10][50] = {"MARIA","ANA","JOAO","ANTONIO","DEBORA","FERNANDO","ANDRESSA","DOUGLAS","FABIO","CAROLINA"};

struct no{
    int qtdeChaves;
    int chaves[(2 * ORDEM) - 1];
    No *nos[2 * ORDEM];
    
};

struct paciente{
    int id;
    char nome[50];
    char dtNascimento[11];
    char situacao; // A - em atendimento, E - em espera, F - abandono
    int totalSessoes; // total de sessoes realizadas
    int qtdFaltas;
    int faltasConsecutivas;
    Terapeuta* terapeuta;
    Paciente* prox;
};

struct terapeuta{
    char nome[50];
    char classe; //A - aluno, P - profissional
    int qtdeAtendidos; // numero de pacientes ja atendidos
    int qtdeAtendimento; // numero de pacientes em atendimento
    int qtdeSessoes; // quantidade de sessoes realizadas dos pacientes em atendimento
    Terapeuta* prox;

};

struct fila{
    Paciente* inicio;
    Paciente* fim;
};

No *criaNo();
No* criaArvore();
No* buscaChave(No *arvore, int chave);
No* insere(No **raiz, int chave);
No* percorreInsere(No **arvore, int chave);
No* divide(No **pai, int indiceFilho);
No* elimina(No **arvore, int chave);
No* percorreRemove(No **arvore, int chave);
No* junta(No **pai, int indiceFilhoChave, int indiceIrmao);
No* removeChave(No **no, int chave);
No* insereChave(No **no, int chave);
No* insereNo(No **no, No *noAInserir);
No* organizaFilhos(No **no);
int filhoAPercorrer(No *no, int chave);
void imprime(No *arvore);
void liberaArvore(No *arvore);
int chaveExisteNo(No *no, int chave);
int ehFolha(No *no);
int noCheio(No *no);
Fila* criarFila();
void inserirFila(Fila* fila, char nome[], char dtNasc[], char situacao, int totalSessoes, int qtdFaltas, int faltasConsecutivas);
int filaVazia(Fila* fila);
Paciente* retirarFila(Fila* fila);
void liberarFila(Fila* fila);
int ehCrianca(Paciente* paciente);
void gerenciaFaltasPaciente(Paciente* paciente, int faltou);
void consultaRealizada(Paciente* paciente);
int consultasRestantes(Paciente* paciente);
void geraDia(Paciente* paciente);
void geraMes(Paciente* paciente);
void geraAno(Paciente* paciente);
void geraDataNascimento(Paciente* paciente);
void setSituacao(Paciente* paciente, char situacao);
char getSituacao(Paciente* paciente);
Terapeuta* geraTerapeuta();
int gerarNumero(int min,int max);

int main(){
    /*Paciente *paciente = malloc(sizeof(Paciente));
    srand(time(NULL));
    geraDataNascimento(paciente);
    geraSituacao(paciente);
    printf("Data de nascimento: %s\nSituação: %c\n", paciente->dtNascimento, paciente->situacao);*/

    Terapeuta* tp = geraTerapeuta();

    No *arvore = criaArvore();
    int chave;
    int qtdeElementos;

    printf("Quantidade de elementos a serem inseridos: ");
    scanf("%d", &qtdeElementos);

    for(int i = 0; i < qtdeElementos; i++){
        printf("Chave a ser inserida: ");
        scanf("%d", &chave);
        insere(&arvore, chave);
        imprime(arvore);

    }

    printf("Quantidade de elementos a serem removidos: ");
    scanf("%d", &qtdeElementos);

    for(int i = 0; i < qtdeElementos; i++){
        printf("Chave a ser removida: ");
        scanf("%d", &chave);
        arvore = elimina(&arvore, chave);
        imprime(arvore);

    }

    //liberaArvore(arvore);
    
    return 0;
    
}

No *criaNo(){
    No *no = malloc(sizeof *no);
    
    no->qtdeChaves = 0;
    
    for(int i = 0; i < (2 * ORDEM) - 1; i++)
        no->chaves[i] = 0;
    
    for(int i = 0; i < (2 * ORDEM); i++)
        no->nos[i] = NULL;
    
    return no;
    
}

No* criaArvore(){
    return criaNo();
    
}

No* buscaChave(No *arvore, int chave){
    if(arvore != NULL){
        int i = 0;
        
        while((i < arvore->qtdeChaves) && (chave > arvore->chaves[i]))
            i++;
        
        if((i < arvore->qtdeChaves) && (chave == arvore->chaves[i]))
            return arvore; // encontrou a chave
        
        else if(arvore->nos[i] != NULL)
            return buscaChave(arvore->nos[i], chave); //percorre o filho em que a chave possivelmente se encontra
        
        else
            return arvore; // chegou na folha e nao encontrou a chave, entao retorna o no atual que e onde a chave deveria estar
        
    }
    
    return NULL; // a arvore e NULL, nao ha busca
    
}

No* insere(No **raiz, int chave){
    No *noParaInserir = buscaChave(*raiz, chave);
    
    if(chaveExisteNo(noParaInserir, chave) == -1){
        if(noCheio(*raiz)){
            No *novaRaiz = criaNo();
            
            novaRaiz->qtdeChaves = 0;
            novaRaiz->nos[0] = *raiz;
            
            *raiz = divide(&novaRaiz, 0);
            percorreInsere(&novaRaiz, chave);
            
        }else
            percorreInsere(raiz, chave);
        
    }else
        printf("Chave ja existe, nao eh possivel inserir!\n");
    
    return *raiz;
    
}

No* percorreInsere(No **arvore, int chave){
    if(ehFolha(*arvore)){ // encontrou a folha a inserir
        insereChave(arvore, chave);
        
    }else{
        int i = filhoAPercorrer(*arvore, chave);
        
        if(noCheio((*arvore)->nos[i])){ // verifica se o filho encontrado esta cheio
            divide(arvore, i);
            if(chave > (*arvore)->chaves[i])
                i++;
            
        }
        
        percorreInsere(&((*arvore)->nos[i]), chave);
        
    }
    
    return *arvore;
    
}

No* divide(No **pai, int indiceFilho){
    No *no = criaNo();
    
    no->qtdeChaves = ORDEM - 1;
    
    for(int i = 0; i < ORDEM - 1; i++)
        no->chaves[i] = (*pai)->nos[indiceFilho]->chaves[i + ORDEM];
    
    if(!ehFolha((*pai)->nos[indiceFilho])){
        for(int i = 0; i < ORDEM; i++)
            no->nos[i] = (*pai)->nos[indiceFilho]->nos[i + ORDEM];
        
    }
    
    (*pai)->nos[indiceFilho]->qtdeChaves = ORDEM - 1;
    
    for(int i = (*pai)->qtdeChaves; i > indiceFilho; i--)
        (*pai)->nos[i + 1] = (*pai)->nos[i]; //shift nos filhos
    
    (*pai)->nos[indiceFilho + 1] = no;
    
    for(int i = (*pai)->qtdeChaves; i > indiceFilho; i--)
        (*pai)->chaves[i] = (*pai)->chaves[i - 1]; ///shift nas chaves
    
    (*pai)->chaves[indiceFilho] = (*pai)->nos[indiceFilho]->chaves[ORDEM - 1];
    (*pai)->qtdeChaves++;
    
    return *pai;
    
}

void imprime(No *arvore){
    if(arvore != NULL){
        for(int i = 0; i < arvore->qtdeChaves; i++)
            printf("| %d ", arvore->chaves[i]);
        
        printf("|\n");
        
        for(int i = 0; i < arvore->qtdeChaves + 1; i++)
            imprime(arvore->nos[i]);
        
    }
    
}

void liberaArvore(No *arvore){
    if(arvore != NULL){
        for(int i = 0; i < arvore->qtdeChaves; i++)
            liberaArvore(arvore);
        
        free(arvore);
        
    }
    
}

No* elimina(No **arvore, int chave){
    int indiceChave = chaveExisteNo(*arvore, chave);
    
    if(indiceChave != -1){// a chave esta presente no no
        if(ehFolha(*arvore)){
            *arvore = removeChave(arvore, chave);

        }else{// a chave esta presente em um no interno
            // troca a chave a ser removida pelo maior dos menores
            No *maiorDosMenores = (*arvore)->nos[indiceChave];
            No *menorDosMaiores = (*arvore)->nos[indiceChave + 1];
            
            if(maiorDosMenores->qtdeChaves >= ORDEM){
                while(!ehFolha(maiorDosMenores))
                    maiorDosMenores = maiorDosMenores->nos[maiorDosMenores->qtdeChaves - 1];
                
                (*arvore)->chaves[indiceChave] = maiorDosMenores->chaves[maiorDosMenores->qtdeChaves - 1]; //troca as informacoes
                maiorDosMenores->chaves[maiorDosMenores->qtdeChaves - 1] = chave;

                elimina(&((*arvore)->nos[indiceChave]), chave);
                
            }else if(menorDosMaiores->qtdeChaves >= ORDEM){
                while(!ehFolha(menorDosMaiores))
                    menorDosMaiores = menorDosMaiores->nos[0];
                
                (*arvore)->chaves[indiceChave] = menorDosMaiores->chaves[0]; //troca as informacoes
                menorDosMaiores->chaves[0] = chave;

                elimina(&((*arvore)->nos[indiceChave + 1]), chave);
                
            }else{
                junta(arvore, indiceChave, indiceChave + 1);

                elimina(arvore, chave);
                
            }
            
        }
        
    }else{// encontrar qual o filho a ser percorrido
        int indiceFilho = filhoAPercorrer(*arvore, chave);
        
        if((*arvore)->nos[indiceFilho]->qtdeChaves == ORDEM - 1){ // precisa organizar a arvore antes de descer
            No *irmaoEsquerdo = indiceFilho != 0 ? (*arvore)->nos[indiceFilho - 1] : NULL;
            No *irmaoDireito = indiceFilho != (*arvore)->qtdeChaves ? (*arvore)->nos[indiceFilho + 1] : NULL;
            
            if(irmaoEsquerdo != NULL){
                if((irmaoEsquerdo->qtdeChaves >= ORDEM)){
                    //desce a chave do pai para o filho a percorrer
                    insereChave(&((*arvore)->nos[indiceFilho]), (*arvore)->chaves[indiceFilho - 1]);
                    removeChave(arvore, (*arvore)->chaves[indiceFilho - 1]);
                    
                    //sobe a maior chave do irmao esquerdo para o pai
                    insereChave(arvore, irmaoEsquerdo->chaves[irmaoEsquerdo->qtdeChaves - 1]);

                    //passa o ponteiro de filho adequado do irmao para o filho a percorrer
                    int i = (*arvore)->nos[indiceFilho]->qtdeChaves;
                    while((i > 0)){
                        (*arvore)->nos[indiceFilho]->nos[i] = (*arvore)->nos[indiceFilho]->nos[i - 1];

                        i--;

                    }

                    (*arvore)->nos[indiceFilho]->nos[0] = irmaoEsquerdo->nos[irmaoEsquerdo->qtdeChaves];
                    
                    removeChave(&irmaoEsquerdo, irmaoEsquerdo->chaves[irmaoEsquerdo->qtdeChaves - 1]);

                    organizaFilhos(&((*arvore)->nos[indiceFilho]));
                    
                    
                }else{
                    *arvore = junta(arvore, indiceFilho, indiceFilho - 1);
                    
                }
                
            }else if(irmaoDireito != NULL){
                if(irmaoDireito->qtdeChaves >= ORDEM){
                    //desce a chave do pai para o filho a percorrer
                    insereChave(&((*arvore)->nos[indiceFilho]), (*arvore)->chaves[indiceFilho]);
                    removeChave(arvore, (*arvore)->chaves[indiceFilho]);
                    
                    //sobe a menor chave do irmao direito para o pai
                    insereChave(arvore, irmaoDireito->chaves[0]);

                    //passa o ponteiro de filho adequado do irmao para o filho a percorrer
                    (*arvore)->nos[indiceFilho]->nos[(*arvore)->nos[indiceFilho]->qtdeChaves] = irmaoDireito->nos[0];
                    for(int i = 0; i < irmaoDireito->qtdeChaves; i++){
                        irmaoDireito->nos[i] =  irmaoDireito->nos[i + 1];

                    }
                    
                    removeChave(&irmaoDireito, irmaoDireito->chaves[0]);

                    organizaFilhos(&((*arvore)->nos[indiceFilho]));
                    
                }else
                    *arvore = junta(arvore, indiceFilho, indiceFilho + 1);
                
            }

            elimina(arvore, chave);
            
        }else
            elimina(&((*arvore)->nos[indiceFilho]), chave);
        
    }
    
    return *arvore;
    
}

No* junta(No **pai, int indiceFilhoChave, int indiceIrmao){
    int indiceChavePai = indiceFilhoChave < indiceIrmao ? indiceFilhoChave : indiceIrmao;
    int qtdeChavesRestantes, indiceFilhosTransferidos;

    //transfere chave do pai que separa os irmaos para (*pai)->nos[indiceIrmao]
    insereChave(&((*pai)->nos[indiceIrmao]), (*pai)->chaves[indiceChavePai]);
    
    //transfere as chaves restantes de (*pai)->nos[indiceFilhoChave] para (*pai)->nos[indiceIrmao]
    qtdeChavesRestantes = (*pai)->nos[indiceFilhoChave]->qtdeChaves;
    int i = 0;
    while(qtdeChavesRestantes > 0){
        insereChave(&((*pai)->nos[indiceIrmao]), (*pai)->nos[indiceFilhoChave]->chaves[i]);
        
        i++;
        qtdeChavesRestantes--;
        
    }

    for(int i = 0; i <= (*pai)->nos[indiceFilhoChave]->qtdeChaves; i++)
        insereNo(&((*pai)->nos[indiceIrmao]), (*pai)->nos[indiceFilhoChave]->nos[i]);

    i = 0;
    qtdeChavesRestantes = (*pai)->nos[indiceFilhoChave]->qtdeChaves;
    while(qtdeChavesRestantes > 0){
        removeChave(&((*pai)->nos[indiceFilhoChave]), (*pai)->nos[indiceFilhoChave]->chaves[i]);
        
        i++;
        qtdeChavesRestantes--;
        
    }
    
    if((*pai)->qtdeChaves == 1){
        No *auxPai = (*pai)->nos[indiceIrmao];
        removeChave(pai, (*pai)->chaves[indiceChavePai]);
        *pai = auxPai;

    }else
        removeChave(pai, (*pai)->chaves[indiceChavePai]);

    organizaFilhos(pai);
    
    return *pai;
    
}

No* removeChave(No **no, int chave){
    int indiceChave = chaveExisteNo(*no, chave);
    int auxChave;
    No *auxNo;
    
    for(int i = indiceChave; i < (*no)->qtdeChaves - 1; i++){ //shift nas chaves
        auxChave = (*no)->chaves[i + 1];
        (*no)->chaves[i] = auxChave;
        
    }
    
    for(int i = indiceChave; i < (*no)->qtdeChaves; i++){ //shift nos filhos
        auxNo = (*no)->nos[i];
        (*no)->nos[i] = auxNo;
        
    }
    
    (*no)->qtdeChaves--;
    
    if((*no)->qtdeChaves == 0)
        return NULL;
        
    else
        return *no;
    
}

No* insereChave(No **no, int chave){
    int i = (*no)->qtdeChaves;
    
    while((i > 0) && (chave < (*no)->chaves[i - 1])){ // encontra a posicao onde se deve inserir a chave
        (*no)->chaves[i] = (*no)->chaves[i - 1];
        i--;
        
    }
    
    (*no)->chaves[i] = chave;
    
    (*no)->qtdeChaves++;
    
    return *no;
    
}

No* insereNo(No **no, No *noAInserir){
    if(noAInserir != NULL){
        int i = (*no)->qtdeChaves;

        while((i > 0) && (noAInserir->chaves[0] < (*no)->chaves[i - 1])){ // encontra a posicao onde se deve inserir a chave
            (*no)->nos[i] = (*no)->nos[i - 1];
            i--;
            
        }
        
        (*no)->nos[i] = noAInserir;

    }

    return *no;

}

No* organizaFilhos(No **no){
    int encontrouVazio = 0;
    int i = 0;

    while(!encontrouVazio && i <= (*no)->qtdeChaves){
        if(((*no)->nos[i] == NULL) || ((*no)->nos[i]->qtdeChaves == 0)){
            encontrouVazio = 1;

            while(i <= (*no)->qtdeChaves){
                (*no)->nos[i] = (*no)->nos[i + 1];

                i++;

            }

        }
        
        i++;
    }
    
    return *no;

}

int filhoAPercorrer(No *no, int chave){
    int i = no->qtdeChaves;
    
    while((i > 0) && (chave < no->chaves[i - 1])) // encontra qual o filho deve ser percorrido
        i--;
    
    return i;
    
}

int chaveExisteNo(No *no, int chave){
    int i = 0;
    
    while(i < no->qtdeChaves){
        if(no->chaves[i] == chave)
            return i;
        
        i++;
        
    }
    
    return -1;
    
}

int ehFolha(No *no){
    int i = 0;
    
    while(i < no->qtdeChaves){
        if(no->nos[i] != NULL)
            return 0;
        
        i++;
        
    }
    
    return 1;
    
}

int noCheio(No *no){
    if(no->qtdeChaves == (2 * ORDEM) - 1)
        return 1;
    
    return 0;
    
}

//METODOS FILA DE ESPERA

Fila* criarFila(){
    Fila *fila = malloc(sizeof *fila);

    fila->inicio = NULL;
    fila->fim = NULL;
    
    return fila;

}

void inserirFila(Fila* fila, char nome[], char dtNasc[], char situacao, int totalSessoes, int qtdFaltas, int faltasConsecutivas){
    Paciente* novoPaciente = (Paciente*) malloc(sizeof(Paciente));
    strcpy(novoPaciente->nome,nome);
    strcpy(novoPaciente->dtNascimento,dtNasc);
    novoPaciente->situacao = situacao;
    novoPaciente->totalSessoes = totalSessoes;
    novoPaciente->qtdFaltas = qtdFaltas;
    novoPaciente->faltasConsecutivas = faltasConsecutivas;
    
    
    if(fila->inicio == NULL){
        
        fila->inicio = novoPaciente;
        fila->fim = novoPaciente;
        novoPaciente->prox = NULL;
        
    }else{
        
        fila->fim->prox = novoPaciente;
        novoPaciente->prox = NULL;
        fila->fim = novoPaciente;
        
        
    }
    
}
int filaVazia(Fila* fila){
    
    if(fila->inicio==NULL){
        return 1;
    }else{
        return  0;
    }
    
}
Paciente* retirarFila(Fila* fila){
    
    Paciente* pacienteTemp;
    
    if(filaVazia(fila)!=1){
        
        pacienteTemp = fila->inicio;
        Paciente* pacienteRetirado = pacienteTemp;
        fila->inicio = fila->inicio->prox;
        free(pacienteTemp);
        
        return pacienteRetirado;
        
    }else{
        
        return NULL;
        
    }
}

void liberarFila(Fila* fila){
    Paciente* atual = fila->inicio;
    Paciente* atualTemp;
    
    while(atual != NULL){
        
        atualTemp=atual;
        atual = atual->prox;
        free(atualTemp);
        
    }
    
    free(fila);
}

// METODOS PACIENTE

int ehCrianca(Paciente* paciente){
    // retorna 0 caso o paciente tenha 12 anos ou mais
    // retorna 1 caso o paciente seja considerado infantil
    int dia = ((paciente->dtNascimento[0] - 48) * 10) + paciente->dtNascimento[1] - 48;
    int mes = ((paciente->dtNascimento[3] - 48) * 10) + paciente->dtNascimento[4] - 48;
    int ano = ((paciente->dtNascimento[6] - 48) * 1000) + ((paciente->dtNascimento[7] - 48) * 100)
            + ((paciente->dtNascimento[8] - 48) * 10) + paciente->dtNascimento[9] - 48;
    time_t mytime; 
    mytime = time(NULL);
    struct tm tm = *localtime(&mytime);
    if(tm.tm_year + 1900 - ano <= 12){
        if(tm.tm_mon + 1 == mes){
            if(tm.tm_mday < dia){
                return 1;
            }
        }else if(tm.tm_mon + 1 < mes){
            return 1;
        }
    }
    return 0;
}


int disponibilidadeTerapeuta(Terapeuta* terapeuta){

    if(terapeuta->classe == 'A'){
        if(terapeuta->qtdeAtendimento == 3){
            //TODO: nao tem disponibilidade e tem que passar pra outro
            return 1;
        }
    }else{
         if(terapeuta->qtdeAtendimento == 5){
            //TODO: nao tem disponibilidade e tem que passar pra outro
            return 1;
        }
    }
    return 0;
    
}



void gerenciaFaltasPaciente(Paciente* paciente, int faltou){
    // para registrar presença: faltou = 0
    // para registrar falta: faltou = 1
    if(faltou == 1){
        paciente->qtdFaltas++;
        paciente->faltasConsecutivas++;
        if(paciente->faltasConsecutivas >= 3 || paciente->qtdFaltas >= 5){  // desistente
            // TODO: chamar função que remove ou retornar algum valor
        }
    }else if(faltou == 0){
        paciente->faltasConsecutivas = 0;
    }
}

void consultaRealizada(Paciente* paciente){
    paciente->totalSessoes--;
}

int consultasRestantes(Paciente* paciente){
    return paciente->totalSessoes;
}

void geraDia(Paciente* paciente){
    int aleatorio = (rand() % 31) + 1;
    paciente->dtNascimento[0] = aleatorio / 10 + 48;
    paciente->dtNascimento[1] = aleatorio % 10 + 48;
    paciente->dtNascimento[2] = '/';
}

void geraMes(Paciente* paciente){
    int aleatorio = (rand() % 12) + 1;
    paciente->dtNascimento[3] = aleatorio / 10 + 48;
    paciente->dtNascimento[4] = aleatorio % 10 + 48;
    paciente->dtNascimento[5] = '/';
}

void geraAno(Paciente* paciente){
    int aleatorio = (rand() % 60) + 1960;
    paciente->dtNascimento[6] = aleatorio / 1000 + 48;
    paciente->dtNascimento[7] = aleatorio % 1000 / 100 + 48;
    paciente->dtNascimento[8] = aleatorio % 1000 % 100 / 10 + 48;
    paciente->dtNascimento[9] = aleatorio % 1000 % 100 % 10 + 48;
    paciente->dtNascimento[10] = '\0';
}

void geraDataNascimento(Paciente* paciente){
    geraDia(paciente);
    geraMes(paciente);
    geraAno(paciente);
}

void setSituacao(Paciente* paciente, char situacao){
    // A - em atendimento, E - em espera, F - abandono
    paciente->situacao = situacao;
}

char getSituacao(Paciente* paciente){
    // A - em atendimento, E - em espera, F - abandono
    return paciente->situacao;
}

int gerarNumero(int min,int max){
    int random = 0;
    
    
    random = (min + (rand()%max));
    
    return random;
}

void geraHorario(){
    int hora, minuto, segundo;
    hora = gerarNumero(hora, 24);
    minuto = gerarNumero(minuto, 60);
    segundo = gerarNumero(segundo, 60);
}

int disponibilidadeHorario(int hora){
    if(hora <= 9 || hora > 20){
        printf("Consultório fechado");
    }
    return 0;
}

int checaTerapeutaAlunoProfissional(Terapeuta* terapeuta){
    //1 é profissional e 0 é aluno
    if(terapeuta->classe == 'A'){
        return 0;
    }else{
        return 1;
    }

}

Terapeuta* geraTerapeuta(){
    Terapeuta* novoTerapeuta = (Terapeuta*) malloc (sizeof(Terapeuta));

    char classe[3] = {'A','P','o'};
    
    strcpy(novoTerapeuta->nome,nomes[gerarNumero(0,10)]);
    novoTerapeuta->qtdeAtendidos = 0 ;
    novoTerapeuta->qtdeAtendimento = 0;
    novoTerapeuta->qtdeSessoes = 0;
    novoTerapeuta->classe = classe[gerarNumero(0,2)];
    novoTerapeuta->prox = NULL;

    return novoTerapeuta;
}

void gerenciaAtendimentoTerapeuta(Terapeuta* tp, int situ){
    // 0 - terapeuta faltou, 1 - cliente faltou
    if(situ == 0){

    }else{

    }


}

// // Gerou 40 terapeutas
    // Gerou 600 pacientes

    //adicionou na arvore
    //gerou consultas

    //cirar fila de espera 

    //verificar idade
    //verificar classe

    //verificar disponibilidade terapeuta 
    //verificar disponibilidade horaio 
    //verificar disponibilidade sala 

    //inserir na fila de espera 

    //guaradr a falta terapeuta
    //guaradr a falta paciente
    //verificar se falta = 3 ou 5 total

    //se falta = 3 => remover nó paciente

    //decrementar consultas que faltaram

    //adicionar sessao final (quando transfere a sessao, vai pro final da fila)

    //verificar lista de espera

    //tirar da lista de espera 
