#include "../include/linkedlist.h"

/* llInit
 *
 *Inicializa nodo de lista ligada LLista
 */
struct llista* llInit(){

        struct llista* newlink = (LLista)malloc(sizeof(struct llista));

        newlink->data = NULL;

        newlink->postPerguntas = (AVLT)malloc(sizeof(struct PostAVL));
        newlink->postPerguntas = NULL;

        newlink->postRespostas = (AVLT)malloc(sizeof(struct PostAVL));
        newlink->postRespostas = NULL;

        newlink->usersPerguntas = (LAVL)malloc(sizeof(struct LongAVL));
        newlink->usersPerguntas = NULL;

        newlink->usersRespostas = (LAVL)malloc(sizeof(struct LongAVL));
        newlink->usersRespostas = NULL;

        newlink->tags = (TagLL)malloc(sizeof(struct TagLL));
        newlink->tags = NULL;

        newlink->next = (LLista)malloc(sizeof(struct llista));
        newlink->next= NULL;
        newlink->prev = NULL;

        return newlink;
}


/* insertAtDate
 *
 *Insere estruturas na lista ligada ordenada por data (crescente):
 *   
 *   ->Data, em que o periodo escolhido foi 1 mês (factor de ordenação)
 *   ->AVLT postPerguntas (arvore com perguntas daquela data(mês e ano))
 *   ->AVLT postRespostas (arvore com respostas daquela data(mês e ano))
 *   ->LAVL usersPerguntas (arvore com ids dos users com perguntas na dada data)
 *   ->LAVL usersRespostas (arvore com ids dos users com respostas na dada data)
 *   ->TagLL tags (lista ligada com as tags usadas pelas perguntas na dada data, partidas por '<>')
 */
struct llista* insertAtDate(struct llista* ll, Date data, struct Post* post, struct TagLL* tags){
    //start from the first link

    while(ll->prev != NULL){
        ll = ll->prev;
    }

    struct llista* current = ll;

    char* auxt;

    auxt=malloc(strlen((char*)post->tags)+1);
    strcpy(auxt,"NULL");
    strcpy(auxt, post->tags);

    if(current == NULL || current->data == NULL){

        //create link
        current = llInit();
        current->data = data;
        if(post->postType==1){  
            current->postPerguntas = insertP(current->postPerguntas,post,post->idPost);
            current->usersPerguntas = insertL(current->usersPerguntas,post->owner);
            auxt = strtok(auxt,"<>");
            while(auxt != NULL){
                if(strcmp(auxt,"NULL")!=0){
                    current->tags = insertTag(current->tags,auxt);
                }
            
                auxt = strtok(NULL,"<>");
            }
        }
        else
            if(post->postType==2){
                current->postRespostas = insertP(current->postRespostas,post,post->idPost);
                current->usersRespostas = insertL(current->usersRespostas,post->owner);
                auxt = strtok(auxt,"<>");
                while(auxt != NULL){
                    if(strcmp(auxt,"NULL")!=0){
                        current->tags = insertTag(current->tags,auxt);
                    }
            
                    auxt = strtok(NULL,"<>");
                }
            }


        current->tags = tags;
    
        current->next = NULL;
        free(auxt);
        return current;

    }



    while(current != NULL){

        if(get_month(current->data) == get_month(data) && get_year(current->data) == get_year(data)){

            if(post->postType==1){  
                current->postPerguntas = insertP(current->postPerguntas,post,post->idPost);
                current->usersPerguntas = insertL(current->usersPerguntas,post->owner);
                auxt = strtok(auxt,"<>");
                while(auxt != NULL){
                    if(strcmp(auxt,"NULL")!=0){
                        current->tags = insertTag(current->tags,auxt);
                    }
            
                    auxt = strtok(NULL,"<>");
                }
            }
            else
                if(post->postType==2){
                    current->postRespostas = insertP(current->postRespostas,post,post->idPost);
                    current->usersRespostas = insertL(current->usersRespostas,post->owner);
                    auxt = strtok(auxt,"<>");
                    while(auxt != NULL){
                        if(strcmp(auxt,"NULL")!=0){
                        current->tags = insertTag(current->tags,auxt);
                        }
            
                        auxt = strtok(NULL,"<>");
                    }
                }
            free(auxt);

            return current;
        }
            current = current->next;
        
    }   

    current = ll;
    int para = 0;

    while(current != NULL && !para){
        if(get_year(current->data) > get_year(data) || (get_year(current->data) == get_year(data) && get_month(current->data) > get_month(data))){

            struct llista* newlink;
            newlink = llInit();
            newlink->data = data;

            if(post->postType==1){
                newlink->postPerguntas = insertP(newlink->postPerguntas,post, post->idPost);
                newlink->usersPerguntas = insertL(newlink->usersPerguntas, post->owner);
                auxt = strtok(auxt,"<>");
                while(auxt != NULL){
                    if(strcmp(auxt,"NULL")!=0){
                        newlink->tags = insertTag(newlink->tags,auxt);
                    }
            
                    auxt = strtok(NULL,"<>");
                }
            }
            else
                if(post->postType==2){
                    newlink->postRespostas = insertP(newlink->postRespostas,post, post->idPost);
                    newlink->usersRespostas = insertL(newlink->usersRespostas, post->owner);
                    auxt = strtok(auxt,"<>");
                    while(auxt != NULL){
                        if(strcmp(auxt,"NULL")!=0){
                            newlink->tags = insertTag(newlink->tags,auxt);
                        }
                
                        auxt = strtok(NULL,"<>");
                    }   
                }

            newlink->tags = tags;
            if(current->prev == NULL){
                newlink->prev = NULL;
            }
            else{
                newlink->prev = current->prev;
                current->prev->next = newlink;

            }
            newlink->next = current;
            current->prev = newlink;

            return newlink;
        }
        if(current->next == NULL){
            para = 1;
        }
        else{
            current = current->next;
        }

    }

    if(current->next == NULL){
        struct llista* newlink;

        newlink = llInit();
        newlink->data = data;

        if(post->postType==1){
                newlink->postPerguntas = insertP(newlink->postPerguntas,post, post->idPost);
                newlink->usersPerguntas = insertL(newlink->usersPerguntas, post->owner);
                auxt = strtok(auxt,"<>");
                while(auxt != NULL){
                    if(strcmp(auxt,"NULL")!=0){
                        newlink->tags = insertTag(newlink->tags,auxt);
                    }
            
                    auxt = strtok(NULL,"<>");
                }
            }
            else
                if(post->postType==2){
                    newlink->postRespostas = insertP(newlink->postRespostas,post, post->idPost);
                    newlink->usersRespostas = insertL(newlink->usersRespostas, post->owner);
                    auxt = strtok(auxt,"<>");
                    while(auxt != NULL){
                        if(strcmp(auxt,"NULL")!=0){
                            newlink->tags = insertTag(newlink->tags,auxt);
                        }
                    
                        auxt = strtok(NULL,"<>");
                    }
                }


        newlink->tags = tags;

        newlink->next = NULL;
        current->next = newlink;
        newlink->prev = current;
        free(auxt);
        return newlink;
    } 

    return current;
}

// TAG LINKED LIST

/*  imprimeTags
 *
 *Insere tags numa lista ligada
 */

void imprimeTags(struct TagLL* tags){


    struct TagLL* currentT = tags;
    if(currentT == NULL){
    }
    while(currentT != NULL){  

        currentT = currentT->next;
    }
}

/*  tagInit
 *
 * Inicializa nodo de uma lista ligada de tags
 */
struct TagLL* tagInit(char* tag){

        struct TagLL* newTag = (struct TagLL*)malloc(sizeof(struct TagLL));

        newTag->tag = (struct Tag*)malloc(sizeof(struct Tag));
        newTag->tag->name = malloc(strlen((char*)tag)+1);
        newTag->tag->name = tag;
        newTag->tag->count = 1;
        newTag->next = NULL;

        return newTag;
}

/*  insertTag
 *
 * Insere um nome de uma dada tag numa lista ligada de tags
 */
struct TagLL* insertTag(struct TagLL* tags, char* tag){

    struct TagLL* currentT = tags;
    if(currentT == NULL){
        currentT = tagInit(tag);
        currentT->next = NULL;
        return currentT;
    }

    else{

        while(currentT->next != NULL){
            if(strcmp(currentT->tag->name,tag) == 0){
                currentT->tag->count +=1;
                return tags;
            }
           currentT = currentT->next;
        }
        struct TagLL* newNode;
        newNode = tagInit(tag);


        currentT->next = newNode;
        newNode->next = NULL;

        return tags;
    }
}
