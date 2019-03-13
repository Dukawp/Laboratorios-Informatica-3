#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../include/tagsAux.h"
#include "../include/interface.h"


typedef struct User{
    long idUser;
    long reputation;
    char* about;
    char* name;
    int countPosts;
} *User;

typedef struct UserAVL{
    User users;
    struct UserAVL* esq;
    struct UserAVL* dir;
    int altura; 
} *UAVL;

typedef struct LongAVL{
    long valor;
    struct LongAVL* esq;
    struct LongAVL* dir;
    int altura;
} *LAVL;

char* searchAVLUsersByOwner(UAVL u, int owner);

void sortByCountPosts(User* arr, int size);
int toArray(UAVL u, User* arrayP, int n, int i);
void maxUser(User* arrayU, User* arrayUFinal, int uN, int N);

User get_User(UAVL ua, long id);

int searchLAVLAux(LAVL la, long id);
int searchLAVL(LAVL per, LAVL res, long id);

int searchUsers(LAVL la, UAVL ua,User* arrayU,int i);
int contaL(LAVL l);
int contaU(UAVL l);
int contaT(TAVL l);
UAVL bestRep(TAD_community com, int N, Date begin, Date end);
void sortByCountRep(User* arr, int size);
void sortByCountTag(Tag* arr, int size);
int avlToArrayU(UAVL u, User* arrayU, int i);
int avlToArrayT(TAVL u, Tag* arrayU, int i);

void imprimeUAVL(struct UserAVL* uv);
void imprimeLAVL(struct LongAVL* uv);