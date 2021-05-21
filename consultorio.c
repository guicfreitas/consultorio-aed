#include <stdio.h>
#include <stdlib.h>
#define ORDEM 2

typedef struct no No;

struct no{
    int qtdeChaves;
    int chaves[(2 * ORDEM) - 1];
    No *nos[2 * ORDEM];

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
int filhoAPercorrer(No *no, int chave);
void imprime(No *arvore);
void liberaArvore(No *arvore);
int chaveExisteNo(No *no, int chave);
int ehFolha(No *no);
int noCheio(No *no);

int main(){
    No *arvore = criaArvore();
    int chave;
    int qtdeElementos;

    printf("Quantidade de elementos a serem inseridos: ");
    scanf("%d", &qtdeElementos);

    for(int i = 0; i < qtdeElementos; i++){
        scanf("%d", &chave);
        insere(&arvore, chave);
        imprime(arvore);

    }

    printf("Quantidade de elementos a serem removidos: ");
    scanf("%d", &qtdeElementos);

    for(int i = 0; i < qtdeElementos; i++){
        scanf("%d", &chave);
        elimina(&arvore, chave);
        imprime(arvore);

    }

    liberaArvore(arvore);
    
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
            printf("no cheio!\n");
            No *novaRaiz = criaNo();

            novaRaiz->qtdeChaves = 0;
            novaRaiz->nos[0] = *raiz;

            *raiz = divide(&novaRaiz, 0);
            printf("divisao feita com sucesso\n");
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
        printf("filho a percorrer = %d\n", i);

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
    printf("indiceChave = %d\n", indiceChave);

    if(indiceChave != -1){// a chave esta presente no no
        if(ehFolha(*arvore)){
            *arvore = removeChave(arvore, chave);
            //verifica se desbalanceou

        }else{// a chave esta presente em um no interno
            // troca a chave a ser removida pelo maior dos menores
            No *maiorDosMenores = (*arvore)->nos[indiceChave];
            No *menorDosMaiores = (*arvore)->nos[indiceChave + 1];

            if(maiorDosMenores->qtdeChaves >= ORDEM){
                while(!ehFolha(maiorDosMenores))
                    maiorDosMenores = maiorDosMenores->nos[maiorDosMenores->qtdeChaves];

                (*arvore)->chaves[indiceChave] = maiorDosMenores->chaves[maiorDosMenores->qtdeChaves]; //troca as informacoes
                maiorDosMenores->chaves[maiorDosMenores->qtdeChaves] = chave;

            }else if(menorDosMaiores->qtdeChaves >= ORDEM){
                while(!ehFolha(menorDosMaiores))
                    menorDosMaiores = menorDosMaiores->nos[0];

                (*arvore)->chaves[indiceChave] = menorDosMaiores->chaves[0]; //troca as informacoes
                menorDosMaiores->chaves[0] = chave;

            }else{
                junta(arvore, indiceChave, indiceChave + 1);

            }

            elimina(arvore, chave);
        
        }

    }else{// encontrar qual o filho a ser percorrido
        int indiceFilho = filhoAPercorrer(*arvore, chave);
        printf("filho a percorrer = %d\n", indiceFilho);

        if((*arvore)->nos[indiceFilho]->qtdeChaves == ORDEM - 1){
            printf("precisa organizar a arvore!!\n");
            No *irmaoEsquerdo = indiceFilho != 0 ? (*arvore)->nos[indiceFilho - 1] : NULL;
            No *irmaoDireito = indiceFilho != (*arvore)->qtdeChaves ? (*arvore)->nos[indiceFilho + 1] : NULL;

            if(irmaoEsquerdo != NULL){
                if((irmaoEsquerdo->qtdeChaves >= ORDEM)){
                    printf("organiza o irmao esquerdo!!\n");
                    //desce a menor chave do pai para o filho a percorrer
                    insereChave(&((*arvore)->nos[indiceFilho]), (*arvore)->chaves[0]);
                    removeChave(arvore, (*arvore)->chaves[0]);

                    //sobe a maior chave do irmao esquerdo para o pai
                    insereChave(arvore, irmaoEsquerdo->chaves[irmaoEsquerdo->qtdeChaves - 1]);
                    removeChave(&irmaoEsquerdo, irmaoEsquerdo->chaves[irmaoEsquerdo->qtdeChaves - 1]);
                    //passa o ponteiro de filho adequado do irmao para o filho a percorrer

                }else{
                    printf("junta com o irmao esquerdo!!\n");
                    junta(arvore, indiceFilho, indiceFilho - 1);

                }

            }else if(irmaoDireito != NULL){
                if(irmaoDireito->qtdeChaves >= ORDEM){
                    printf("organiza o irmao direito!!\n");
                    //desce a maior chave do pai para o filho a percorrer
                    insereChave(&((*arvore)->nos[indiceFilho]), (*arvore)->chaves[(*arvore)->qtdeChaves - 1]);

                    //sobe a menor chave do irmao direito para o pai
                    insereChave(arvore, irmaoDireito->chaves[0]);

                    removeChave(arvore, (*arvore)->chaves[0]);
                    removeChave(&irmaoDireito, irmaoDireito->chaves[0]);
                    //passa o ponteiro de filho adequado do irmao para o filho a percorrer

                }else{
                    printf("junta com o irmao direito!!\n");
                    junta(arvore, indiceFilho, indiceFilho + 1);

                }

            }

        }

        imprime(*arvore);
        indiceFilho = filhoAPercorrer(*arvore, chave);
        printf("filho a percorrer = %d\n", indiceFilho);

        elimina(&((*arvore)->nos[indiceFilho]), chave);

    }

    return *arvore;

}

No* junta(No **pai, int indiceFilhoChave, int indiceIrmao){ // 2 1
    //transfere chave do pai que separa os irmaos para (*pai)->nos[indiceIrmao]
    int indiceChavePai = indiceFilhoChave < indiceIrmao ? indiceFilhoChave : indiceIrmao;
    int qtdeChavesRestantes;

    insereChave(&((*pai)->nos[indiceIrmao]), (*pai)->chaves[indiceChavePai]);
    //imprime(*pai);

    //transfere as chaves restantes de (*pai)->nos[indiceFilhoChave] para (*pai)->nos[indiceIrmao]
    qtdeChavesRestantes = (*pai)->nos[indiceFilhoChave]->qtdeChaves;
    while(qtdeChavesRestantes > 0){
        (*pai)->nos[indiceIrmao] = insereChave(&((*pai)->nos[indiceIrmao]), (*pai)->nos[indiceFilhoChave]->chaves[0]);

        //imprime(*pai);

        (*pai)->nos[indiceFilhoChave] = removeChave(&((*pai)->nos[indiceFilhoChave]), (*pai)->nos[indiceFilhoChave]->chaves[0]);
        //imprime(*pai);
        printf("transferiu a chave pro irmao!!!\n");

        qtdeChavesRestantes--;

    }

    //imprime(*pai);

    *pai = removeChave(pai, (*pai)->chaves[indiceChavePai]);

    //libera o filho que continha a chave a ser removida
    free((*pai)->nos[indiceFilhoChave]);

    //imprime(*pai);

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

    if((*no)->qtdeChaves == 0){
        free(*no);
        return NULL;

    }else
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


int main1() {
    // Gerou 40 terapeutas
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

    
    
}