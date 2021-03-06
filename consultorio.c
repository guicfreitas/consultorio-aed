#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define ORDEM 2

typedef struct no No;
typedef struct fila Fila;
typedef struct paciente Paciente;
typedef struct terapeuta Terapeuta;
typedef struct consultorio Consultorio;

char nomes [10][50] = {"MARIA","ANA","JOAO","ANTONIO","DEBORA","FERNANDO","ANDRESSA","DOUGLAS","FABIO","CAROLINA"};

struct no{
    int qtdeChaves;
    int chaves[(2 * ORDEM) - 1];
    Paciente *pacientes[(2 * ORDEM) - 1];
    No *nos[2 * ORDEM];
    
};

struct paciente{
    int id;
    char nome[50];
    char dtNascimento[11];
    char situacao; // A - em atendimento, E - em espera, F - abandono
    int totalSessoes; // total de sessoes restantes
    int qtdFaltas;
    int faltasConsecutivas;
    int horaConsulta; // hora agendada para o paciente entre 1 - 60, segunda-feira(1-12), terça-feira(13-24), ..., sexta-feira(49-60).
    Terapeuta* terapeuta;
    Paciente* prox;
};

struct terapeuta{
    char nome[50];
    char classe; // A - aluno, P - profissional
    int qtdeAtendidos; // numero de pacientes ja atendidos
    int qtdeAtendimento; // numero de pacientes em atendimento
    int qtdeSessoes; // quantidade de sessoes realizadas dos pacientes em atendimento
    Paciente* pacientesVinculados[5];
    Terapeuta* prox;

};

struct fila{
    Paciente* inicio;
    Paciente* fim;
};

struct consultorio{
    int id;
    int horarios[12]; // horarios[i] == 0 indica horario vazio, horarios[i] == 1 indica horario cheio

};

No *criaNo();
No* criaArvore();
No* buscaChave(No *arvore, int chave);
No* insere(No **raiz, Paciente *paciente, int chave);
No* percorreInsere(No **arvore, Paciente *paciente, int chave);
No* divide(No **pai, int indiceFilho);
No* elimina(No **arvore, int chave);
No* percorreRemove(No **arvore, int chave);
No* junta(No **pai, int indiceFilhoChave, int indiceIrmao);
No* removeChave(No **no, int chave);
No* insereChave(No **no, Paciente *paciente, int chave);
No* insereNo(No **no, No *noAInserir);
No* organizaFilhos(No **no);
int filhoAPercorrer(No *no, int chave);
void imprime(No *arvore);
void liberaArvore(No *arvore);
int chaveExisteNo(No *no, int chave);
int ehFolha(No *no);
int noCheio(No *no);
Consultorio* criaConsultorio(int id);
Fila* criarFila();
void imprimeFila(Fila *fila);
void inserirFila(Fila* fila, Paciente* p);
int filaVazia(Fila* fila);
Paciente* retirarFila(Fila* fila);
void liberarFila(Fila* fila);
Paciente* geraPaciente(int id);
void imprimePaciente(Paciente *paciente);
void imprimeTerapeuta(Terapeuta* terapeuta);
void imprimePacienteDoTerapeuta(Terapeuta* terapeuta);
int ehCrianca(Paciente* paciente);
int disponibilidadeTerapeuta(Terapeuta* terapeuta);
void gerenciaFaltasPaciente(Paciente* paciente,Fila* fila,No** arvore,int faltou);
void geraFaltaPaciente(Paciente *paciente,Fila* fila,No** arvore);
void faltaPaciente(Paciente* paciente);
No* atribuiFaltasPaciente(No **arvore,Fila* fila);
No* atribuiFaltasTerapeuta(No *arvore);
void consultaRealizada(Paciente* paciente);
int consultasRestantes(Paciente* paciente);
void geraDia(Paciente* paciente);
void geraMes(Paciente* paciente);
void geraAno(Paciente* paciente);
void geraDataNascimento(Paciente* paciente);
void setSituacao(Paciente* paciente, char situacao);
char getSituacao(Paciente* paciente);
Terapeuta* geraTerapeuta(char classe);
Terapeuta* buscaTerapeuta(Terapeuta* listaTerapeuta);
void insereTerapeuta(Terapeuta* inicio, Terapeuta* novo);
Terapeuta* iniciaListaTerapeuta();
int geraNumero(int min,int max);
void geraHorario();
int disponibilidadeHorario(Consultorio* consultorio);
int checaTerapeutaAlunoProfissional(Terapeuta* terapeuta);
int geraFaltaTerapeuta(Paciente *pa);
int consultorioDisponivel(Consultorio** consultorios);
void sessaoConsulta(Paciente* paciente,Fila* fila,No** arvore);


int main(){
    srand(time(NULL));

    Terapeuta* terapeutas = iniciaListaTerapeuta();
    Consultorio *consultorios[6];
    Fila *filaDeEspera = criarFila();

    for(int i = 0; i < 6; i++)
        consultorios[i] = criaConsultorio(i + 1);

    No *arvore = criaArvore();
    int chave;
    int qtdeElementos;

    printf("Quantidade de pacientes a serem inseridos: ");
    scanf("%d", &qtdeElementos);

    for(int i = 0; i < qtdeElementos; i++){
        Terapeuta *terapeuta = buscaTerapeuta(terapeutas);
        Paciente *novoPaciente = geraPaciente(i + 1);

        if(terapeuta != NULL && consultorioDisponivel(consultorios)){
            novoPaciente->terapeuta = terapeuta;
            terapeuta->pacientesVinculados[terapeuta->qtdeAtendimento++] = novoPaciente;
            novoPaciente->situacao = 'A';
            insere(&arvore, novoPaciente, i + 1);
        }else{
            novoPaciente->situacao = 'E';
            inserirFila(filaDeEspera, novoPaciente);
        }
    }

//    imprime(arvore);
    imprimeFila(filaDeEspera);
    
    for(int cont = 0 ; cont < 20; cont++){
        arvore = atribuiFaltasPaciente(&arvore,filaDeEspera);
        //arvore = atribuiFaltasTerapeuta(arvore);
    }
    printf("Imprimindo pacientes em atendimento\n");
    imprime(arvore);
    printf("Imprimindo pacientes de cada terapeuta\n");
    imprimePacienteDoTerapeuta(terapeutas);

//    printf("Quantidade de pacientes a serem removidos: ");
//    scanf("%d", &qtdeElementos);
//
//    for(int i = 0; i < qtdeElementos; i++){
//        printf("Chave a ser removida: ");
//        scanf("%d", &chave);
//        arvore = elimina(&arvore, chave);
//
//    }
//
//    imprime(arvore);

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

No* insere(No **raiz, Paciente *paciente, int chave){
    No *noParaInserir = buscaChave(*raiz, chave);
    
    if(chaveExisteNo(noParaInserir, chave) == -1){
        if(noCheio(*raiz)){
            No *novaRaiz = criaNo();
            
            novaRaiz->qtdeChaves = 0;
            novaRaiz->nos[0] = *raiz;
            
            *raiz = divide(&novaRaiz, 0);
            percorreInsere(&novaRaiz, paciente, chave);
            
        }else
            percorreInsere(raiz, paciente, chave);
        
    }else
        printf("Chave ja existe, nao eh possivel inserir!\n");
    
    return *raiz;
    
}

No* percorreInsere(No **arvore, Paciente *paciente, int chave){
    if(ehFolha(*arvore)){ // encontrou a folha a inserir
        insereChave(arvore, paciente, chave);
        
    }else{
        int i = filhoAPercorrer(*arvore, chave);
        
        if(noCheio((*arvore)->nos[i])){ // verifica se o filho encontrado esta cheio
            divide(arvore, i);
            if(chave > (*arvore)->chaves[i])
                i++;
            
        }
        
        percorreInsere(&((*arvore)->nos[i]), paciente, chave);
        
    }
    
    return *arvore;
    
}

No* divide(No **pai, int indiceFilho){
    No *no = criaNo();
    
    no->qtdeChaves = ORDEM - 1;
    
    for(int i = 0; i < ORDEM - 1; i++){
        no->chaves[i] = (*pai)->nos[indiceFilho]->chaves[i + ORDEM];
        no->pacientes[i] = (*pai)->nos[indiceFilho]->pacientes[i + ORDEM];

    }
    
    if(!ehFolha((*pai)->nos[indiceFilho])){
        for(int i = 0; i < ORDEM; i++)
            no->nos[i] = (*pai)->nos[indiceFilho]->nos[i + ORDEM];
        
    }
    
    (*pai)->nos[indiceFilho]->qtdeChaves = ORDEM - 1;
    
    for(int i = (*pai)->qtdeChaves; i > indiceFilho; i--)
        (*pai)->nos[i + 1] = (*pai)->nos[i]; //shift nos filhos
    
    (*pai)->nos[indiceFilho + 1] = no;

    for(int i = (*pai)->qtdeChaves; i > indiceFilho; i--){
        (*pai)->chaves[i] = (*pai)->chaves[i - 1]; ///shift nas chaves
        (*pai)->pacientes[i] = (*pai)->pacientes[i - 1];

    }
    
    (*pai)->chaves[indiceFilho] = (*pai)->nos[indiceFilho]->chaves[ORDEM - 1];
    (*pai)->pacientes[indiceFilho] = (*pai)->nos[indiceFilho]->pacientes[ORDEM - 1];
    (*pai)->qtdeChaves++;
    
    return *pai;
    
}

void imprime(No *arvore){
    if(arvore != NULL){
        for(int i = 0; i < arvore->qtdeChaves; i++){
//            printf("Paciente %d\n", arvore->chaves[i]);
            imprimePaciente(arvore->pacientes[i]);
//            printf("\n");

        }
        
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
            removeChave(arvore, chave);

        }else{// a chave esta presente em um no interno
            // troca a chave a ser removida pelo maior dos menores
            No *maiorDosMenores = (*arvore)->nos[indiceChave];
            No *menorDosMaiores = (*arvore)->nos[indiceChave + 1];
            
            if(maiorDosMenores->qtdeChaves >= ORDEM){
                while(!ehFolha(maiorDosMenores))
                    maiorDosMenores = maiorDosMenores->nos[maiorDosMenores->qtdeChaves - 1];
                
                Paciente *auxPaciente;

                //troca as informacoes
                (*arvore)->chaves[indiceChave] = maiorDosMenores->chaves[maiorDosMenores->qtdeChaves - 1];
                maiorDosMenores->chaves[maiorDosMenores->qtdeChaves - 1] = chave;

                auxPaciente = (*arvore)->pacientes[indiceChave];
                (*arvore)->pacientes[indiceChave] = maiorDosMenores->pacientes[maiorDosMenores->qtdeChaves - 1];
                maiorDosMenores->pacientes[maiorDosMenores->qtdeChaves - 1] = auxPaciente;

                elimina(&((*arvore)->nos[indiceChave]), chave);
                
            }else if(menorDosMaiores->qtdeChaves >= ORDEM){
                while(!ehFolha(menorDosMaiores))
                    menorDosMaiores = menorDosMaiores->nos[0];
                
                Paciente *auxPaciente;

                //troca as informacoes
                (*arvore)->chaves[indiceChave] = menorDosMaiores->chaves[0];
                menorDosMaiores->chaves[0] = chave;

                auxPaciente = (*arvore)->pacientes[indiceChave];
                (*arvore)->pacientes[indiceChave] = menorDosMaiores->pacientes[0];
                menorDosMaiores->pacientes[0] = (*arvore)->pacientes[indiceChave];

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
                    insereChave(&((*arvore)->nos[indiceFilho]), (*arvore)->pacientes[indiceFilho - 1], (*arvore)->chaves[indiceFilho - 1]);
                    removeChave(arvore, (*arvore)->chaves[indiceFilho - 1]);
                    
                    //sobe a maior chave do irmao esquerdo para o pai
                    insereChave(arvore, irmaoEsquerdo->pacientes[irmaoEsquerdo->qtdeChaves - 1], irmaoEsquerdo->chaves[irmaoEsquerdo->qtdeChaves - 1]);

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
                    insereChave(&((*arvore)->nos[indiceFilho]), (*arvore)->pacientes[indiceFilho], (*arvore)->chaves[indiceFilho]);
                    removeChave(arvore, (*arvore)->chaves[indiceFilho]);
                    
                    //sobe a menor chave do irmao direito para o pai
                    insereChave(arvore, irmaoDireito->pacientes[0], irmaoDireito->chaves[0]);

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
    insereChave(&((*pai)->nos[indiceIrmao]), (*pai)->pacientes[indiceChavePai], (*pai)->chaves[indiceChavePai]);
    
    //transfere as chaves restantes de (*pai)->nos[indiceFilhoChave] para (*pai)->nos[indiceIrmao]
    qtdeChavesRestantes = (*pai)->nos[indiceFilhoChave]->qtdeChaves;
    int i = 0;
    while(qtdeChavesRestantes > 0){
        insereChave(&((*pai)->nos[indiceIrmao]), (*pai)->nos[indiceFilhoChave]->pacientes[i], (*pai)->nos[indiceFilhoChave]->chaves[i]);
        
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
    Paciente *auxPaciente;
    No *auxNo;
    
    for(int i = indiceChave; i < (*no)->qtdeChaves - 1; i++){ //shift nas chaves
        auxChave = (*no)->chaves[i + 1];
        auxPaciente = (*no)->pacientes[i + 1];

        (*no)->chaves[i] = auxChave;
        (*no)->pacientes[i] = auxPaciente;
        
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

No* insereChave(No **no, Paciente *paciente, int chave){
    int i = (*no)->qtdeChaves;
    
    while((i > 0) && (chave < (*no)->chaves[i - 1])){ // encontra a posicao onde se deve inserir a chave
        (*no)->chaves[i] = (*no)->chaves[i - 1];
        (*no)->pacientes[i] = (*no)->pacientes[i - 1];
        i--;
        
    }
    
    (*no)->chaves[i] = chave;
    (*no)->pacientes[i] = paciente;
    
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

Consultorio* criaConsultorio(int id){
    Consultorio *consultorio = malloc(sizeof *consultorio);

    consultorio->id = id;

    for(int i = 0; i < 12; i++)
        consultorio->horarios[i] = 0; //inicializa todos os horários vazios
    
    return consultorio;

}

//METODOS FILA DE ESPERA

Fila* criarFila(){
    Fila *fila = malloc(sizeof *fila);

    fila->inicio = NULL;
    fila->fim = NULL;
    
    return fila;

}

void imprimeFila(Fila *fila) {
    printf("Clientes na fila de espera\n");
    if(!filaVazia(fila)){
        for (Paciente *paciente = fila->inicio; paciente != NULL; paciente = paciente->prox){
            printf("Paciente %d\n", paciente->id);
            printf("Nome: %s\t", paciente->nome);
            printf("Data de nascimento: %s\t", paciente->dtNascimento);
            printf("Situacao: %c\n", paciente->situacao);
            printf("Sessoes restantes: %-3d", paciente->totalSessoes);
            printf("Faltas totais: %-3d", paciente->qtdFaltas);
            printf("Faltas consecutivas: %d\n", paciente->faltasConsecutivas);
            printf("\n");
        }

    }

}

void inserirFila(Fila* fila, Paciente* p){
    Paciente* novoPaciente = (Paciente*) malloc(sizeof(Paciente));
    novoPaciente->id = p->id;
    strcpy(novoPaciente->nome,p->nome);
    strcpy(novoPaciente->dtNascimento,p->dtNascimento);
    novoPaciente->situacao = p->situacao;
    novoPaciente->totalSessoes = p->totalSessoes;
    novoPaciente->qtdFaltas = p->qtdFaltas;
    novoPaciente->faltasConsecutivas = p->faltasConsecutivas;
    
    
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
        //free(pacienteTemp);
        
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

Paciente* geraPaciente(int id){
    Paciente* novoPaciente = malloc(sizeof(Paciente));

    novoPaciente->id = id;
    strcpy(novoPaciente->nome,nomes[geraNumero(0,10)]);
    geraDataNascimento(novoPaciente);
    novoPaciente->totalSessoes = 20;
    novoPaciente->qtdFaltas = novoPaciente->faltasConsecutivas = 0;
    novoPaciente->terapeuta = NULL;
    novoPaciente->prox = NULL;

    return novoPaciente;
}

void imprimePaciente(Paciente *paciente){
    if(paciente->situacao == 'A') {
        printf("Paciente %d\n", paciente->id);
        printf("Nome: %s\t", paciente->nome);
        printf("Data de nascimento: %s\t", paciente->dtNascimento);
        printf("Situacao: %c\n", paciente->situacao);
        printf("Sessoes restantes: %-3d", paciente->totalSessoes);
        printf("Faltas totais: %-3d", paciente->qtdFaltas);
        printf("Faltas consecutivas: %d\n", paciente->faltasConsecutivas);
        if (paciente->terapeuta != NULL) {
            imprimeTerapeuta(paciente->terapeuta);
        }
        printf("\n");
    }
}

void imprimeTerapeuta(Terapeuta* terapeuta){
    printf("Terapeuta:\t%-12s", terapeuta->nome);
    printf("Classe:\t%c\n", terapeuta->classe);
}

void imprimePacienteDoTerapeuta(Terapeuta* terapeutas){
    Terapeuta* terapeuta = terapeutas;
    while(terapeuta != NULL){
        printf("Terapeuta %s\n", terapeuta->nome);
        if(checaTerapeutaAlunoProfissional(terapeuta) == 0){
            for (int i = 0; i < 2; i++) {
                if(terapeuta->pacientesVinculados[i] != NULL && terapeuta->pacientesVinculados[i]->situacao != 'F') {
                    printf("Paciente %d\n", terapeuta->pacientesVinculados[i]->id);
                    printf("Nome: %s\t", terapeuta->pacientesVinculados[i]->nome);
                    printf("Data de nascimento: %s\t", terapeuta->pacientesVinculados[i]->dtNascimento);
                    printf("Situacao: %c\n", terapeuta->pacientesVinculados[i]->situacao);
                    printf("Sessoes restantes: %-3d", terapeuta->pacientesVinculados[i]->totalSessoes);
                    printf("Faltas totais: %-3d", terapeuta->pacientesVinculados[i]->qtdFaltas);
                    printf("Faltas consecutivas: %d\n", terapeuta->pacientesVinculados[i]->faltasConsecutivas);
                }
            }
        }else{
            for (int i = 0; i < 5; i++) {
                if(terapeuta->pacientesVinculados[i] != NULL && terapeuta->pacientesVinculados[i]->situacao != 'F') {
                    printf("Paciente %d\n", terapeuta->pacientesVinculados[i]->id);
                    printf("Nome: %s\t", terapeuta->pacientesVinculados[i]->nome);
                    printf("Data de nascimento: %s\t", terapeuta->pacientesVinculados[i]->dtNascimento);
                    printf("Situacao: %c\n", terapeuta->pacientesVinculados[i]->situacao);
                    printf("Sessoes restantes: %-3d", terapeuta->pacientesVinculados[i]->totalSessoes);
                    printf("Faltas totais: %-3d", terapeuta->pacientesVinculados[i]->qtdFaltas);
                    printf("Faltas consecutivas: %d\n", terapeuta->pacientesVinculados[i]->faltasConsecutivas);
                }
            }
        }
        printf("\n");
        terapeuta = terapeuta->prox;
    }
}

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

void gerenciaFaltasPaciente(Paciente* paciente,Fila* fila,No** arvore,int faltou){
    // para registrar presença: faltou = 0
    // para registrar falta: faltou = 1
    if(faltou == 1){
        paciente->qtdFaltas++;
        paciente->faltasConsecutivas++;
        paciente->totalSessoes--;
    }else if(faltou == 0){
        paciente->faltasConsecutivas = 0;
        paciente->totalSessoes--;
    }

    if(paciente->faltasConsecutivas == 3 || paciente->qtdFaltas == 5){
        paciente->situacao = 'F';
        Terapeuta* terapeutaDispo = paciente->terapeuta;
        terapeutaDispo->pacientesVinculados[--terapeutaDispo->qtdeAtendimento] = NULL;

//        *arvore = elimina(arvore,paciente->id);
        Paciente* pacienteNaFila = retirarFila(fila);
        if(pacienteNaFila != NULL){
            pacienteNaFila->situacao = 'A';
            pacienteNaFila->terapeuta = terapeutaDispo;
            terapeutaDispo->pacientesVinculados[terapeutaDispo->qtdeAtendimento++] = pacienteNaFila;
            insere(arvore,pacienteNaFila,pacienteNaFila->id);
            
        }
        
    }

}

void geraFaltaPaciente(Paciente *paciente,Fila* fila,No** arvore){
    int numero = geraNumero(0,10);

    if(numero <= 1){
        gerenciaFaltasPaciente(paciente,fila,arvore,1);
    }else{
        gerenciaFaltasPaciente(paciente,fila,arvore,0);
    }

}

void faltaPaciente(Paciente* paciente){
    int qtdFaltas = geraNumero(0,6);
    paciente->qtdFaltas = qtdFaltas;
    if(qtdFaltas == 3){
        paciente->faltasConsecutivas = qtdFaltas;
        paciente->situacao = 'F';
    }

    if(paciente->qtdFaltas == 5){
        paciente->situacao = 'F';
    }

}

No* atribuiFaltasPaciente(No **arvore,Fila* fila){
    if(*arvore != NULL){
        for(int i = 0; i < (*arvore)->qtdeChaves; i++)
            sessaoConsulta((*arvore)->pacientes[i],fila,arvore);


        for(int i = 0; i < (*arvore)->qtdeChaves + 1; i++)
            atribuiFaltasPaciente(&((*arvore)->nos[i]),fila);

    }

    return *arvore;

}

No* atribuiFaltasTerapeuta(No *arvore){
    if(arvore != NULL){
        for(int i = 0; i < arvore->qtdeChaves; i++)
            geraFaltaTerapeuta(arvore->pacientes[i]);
        
        for(int i = 0; i < arvore->qtdeChaves + 1; i++)
            atribuiFaltasTerapeuta(arvore->nos[i]);
        
    }
    
    return arvore;

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

int geraNumero(int min,int max){
    return (min + (rand()%max));
}

void geraHorario(){
    int hora, minuto, segundo;
    hora = geraNumero(hora, 24);
    minuto = geraNumero(minuto, 60);
    segundo = geraNumero(segundo, 60);
}

int consultorioDisponivel(Consultorio** consultorios){
    Consultorio *consultorio;

    for(int cont = 0; cont < 6;cont++){
        consultorio = consultorios[cont];

        for(int i = 0; i < 12; i++){
            if(consultorio->horarios[i] == 0){
                consultorio->horarios[i] = 1;
                return 1;
            }

        }

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

int disponibilidadeTerapeuta(Terapeuta* terapeuta){
    int tp;
    tp = checaTerapeutaAlunoProfissional(terapeuta);

    if(tp == 0){
        if(terapeuta->qtdeAtendimento == 2){
            return 1; // nao pode atender mais
        }

    }else{
        if(terapeuta->qtdeAtendimento == 5){
            return 1; // nao poder atender mais
        }
    }
    return 0;

}

Terapeuta* geraTerapeuta(char classe){
    Terapeuta* novoTerapeuta = (Terapeuta*) malloc (sizeof(Terapeuta));

    // char classe[3] = {'A','P',' '};
    
    strcpy(novoTerapeuta->nome,nomes[geraNumero(0,10)]);
    novoTerapeuta->qtdeAtendidos = 0 ;
    novoTerapeuta->qtdeAtendimento = 0;
    novoTerapeuta->qtdeSessoes = 0;
    novoTerapeuta->classe = classe;
    novoTerapeuta->prox = NULL;
    for (int i = 0; i < 5; i++) {
        novoTerapeuta->pacientesVinculados[i] = NULL;
    }

    return novoTerapeuta;
}

void insereTerapeuta(Terapeuta* inicio, Terapeuta* novo){
    Terapeuta* aux = inicio;
    while(aux->prox != NULL){
        aux = aux->prox;
    }
    aux->prox = novo;
}

Terapeuta* iniciaListaTerapeuta(){
    Terapeuta* terapeuta = geraTerapeuta('P');  // Criando uma lista de terapeutas
    for(int i = 2; i < 40; i++){
        if(i % 3 != 0){
            insereTerapeuta(terapeuta, geraTerapeuta('P'));
        }else{
            insereTerapeuta(terapeuta, geraTerapeuta('A'));
        }
    }
    return terapeuta;
}

Terapeuta* buscaTerapeuta(Terapeuta* listaTerapeuta){
    Terapeuta* terapeuta = listaTerapeuta;
    while(terapeuta != NULL && disponibilidadeTerapeuta(terapeuta)){
        terapeuta = terapeuta->prox;
    }
//    if(terapeuta != NULL){
//        terapeuta->qtdeAtendimento += 1;
//        terapeuta->qtdeAtendidos += 1;
//    }
    
    return terapeuta;
}

int geraFaltaTerapeuta(Paciente *pa){
    int numero = geraNumero(0,10);

    if(numero == 1){
         pa->totalSessoes = pa->totalSessoes + 1;
         return 1;
    }

    return 0;

}

void sessaoConsulta(Paciente* paciente,Fila* fila,No** arvore){
    if(geraFaltaTerapeuta(paciente) != 1){
        geraFaltaPaciente(paciente,fila,arvore);
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
