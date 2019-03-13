#include "../include/votesAux.h"
#include "../include/postsAux.h"

int max(int a, int b);
int altura(struct UserAVL *N);
int max(int a, int b);
int tamanhoTree(struct PostAVL* ap);

struct UserAVL *newNode(User u);
struct UserAVL *rightRotate(struct UserAVL *y);
struct UserAVL *leftRotate(struct UserAVL *x);
struct UserAVL *insert(struct UserAVL *UserAVL, User u);
struct UserAVL *insertC(struct UserAVL *UserAVL, User u);
int getBalance(struct UserAVL *N);

int alturaL(struct LongAVL *N);
struct LongAVL *newNodeL(long u);
struct LongAVL *rightRotateL(struct LongAVL *y);
struct LongAVL *leftRotateL(struct LongAVL *x);
int getBalanceL(struct LongAVL *N);
struct LongAVL *insertL(struct LongAVL *LongAVL, long id);

int alturaP(struct PostAVL *N);
struct PostAVL* avlInit();
struct PostAVL *rightRotateP(struct PostAVL *y);
struct PostAVL *leftRotateP(struct PostAVL *x);
int getBalanceP(struct PostAVL *N);
struct PostAVL *insertP(struct PostAVL *PostAVL, Post u, long id);
struct PostAVL* insertParent(struct PostAVL* postAVL, Post u, long id, UAVL ua);
float calculaMedia(Post resposta, UAVL ua);

int alturaV(struct VoteAVL *N);
struct VoteAVL *newNodeV(Vote u);
struct VoteAVL *rightRotateV(struct VoteAVL *y);
struct VoteAVL *leftRotateV(struct VoteAVL *x);
int getBalanceV(struct VoteAVL *N);
struct VoteAVL *insertV(struct VoteAVL *VoteAVL, Vote u);

int alturaT(struct TagAVL *N);
struct TagAVL *newNodeT(Tag u, long id);
struct TagAVL *rightRotateT(struct TagAVL *y);
struct TagAVL *leftRotateT(struct TagAVL *x);
int getBalanceT(struct TagAVL *N);
struct TagAVL *insertT(struct TagAVL *TagAVL, Tag u, long id);

