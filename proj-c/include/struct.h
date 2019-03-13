#include "../include/date.h"

typedef struct llista{
    Date data;
    struct PostAVL* postPerguntas;
    struct PostAVL* postRespostas;
    struct LongAVL* usersPerguntas;
    struct LongAVL* usersRespostas;
    struct TagLL* tags;
    struct llista* next;
    struct llista* prev;
} *LLista;

typedef struct TCD_community{
    struct UserAVL* users;
    struct PostAVL* respostasByParent;//caso haja mais que um voto para o msm parentId
                                    //guarda a resposta com melhor pontuacao
    struct VoteAVL* votes;
    struct TagAVL* tags;
    int usersNumber;
    struct llista* dataPosts;
} *TCD;

