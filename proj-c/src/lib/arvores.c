#include "../include/arvores.h"
 
/*
    Referencia:
    Código fonte original: https://www.geeksforgeeks.org/avl-tree-set-1-insertion/
*/

// A utility function to get altura of the tree
int altura(struct UserAVL *N)
{
    if (N == NULL)
    return 0;
    return N->altura;
}
 
// A utility function to get maximum of two integers
int max(int a, int b)
{
    return (a > b)? a : b;
}
 

/* Helper function that allocates a new UserAVL with the given u and
    NULL esq and dir pointers. */
struct UserAVL *newNode(User u)
{
    struct UserAVL *UserAVL = (struct UserAVL*) malloc(sizeof(struct UserAVL));

    if(u!=NULL){
        UserAVL->users = (struct User*) malloc(sizeof(struct User));
        UserAVL->users   = u;
        UserAVL->esq   = NULL;
        UserAVL->dir  = NULL;
        UserAVL->altura = 1;  // new UserAVL is initially added at leaf
    }
    return(UserAVL);
}
 
// A utility function to dir rotate subtree rooted with y
// See the diagram given above.
struct UserAVL *rightRotate(struct UserAVL *y)
{
    struct UserAVL *x = y->esq;
    struct UserAVL *T2 = x->dir;
 
    // Perform rotation
    x->dir = y;
    y->esq = T2;
 
    // Update heights
    y->altura = max(altura(y->esq), altura(y->dir))+1;
    x->altura = max(altura(x->esq), altura(x->dir))+1;
 
    // Return new root
    return x;
}
 
// A utility function to esq rotate subtree rooted with x
// See the diagram given above.
struct UserAVL *leftRotate(struct UserAVL *x)
{
    struct UserAVL *y = x->dir;
    struct UserAVL *T2 = y->esq;
 
    // Perform rotation
    y->esq = x;
    x->dir = T2;
 
    //  Update heights
    x->altura = max(altura(x->esq), altura(x->dir))+1;
    y->altura = max(altura(y->esq), altura(y->dir))+1;
 
    // Return new root
    return y;
}
 
// Get Balance factor of UserAVL N
int getBalance(struct UserAVL *N)
{
    if (N == NULL)
        return 0;
    return altura(N->esq) - altura(N->dir);
}
 
// Recursive function to insert u in subtree rooted
// with UserAVL and returns new root of subtree.
struct UserAVL *insert(struct UserAVL *UserAVL, User u)
{
    long id = u->idUser;
    /* 1.  Perform the normal BST insertion */
    if (UserAVL == NULL)
        return(newNode(u));
    
    //printf("A inserir na arvore %ld\n", UserAVL->users->idUser);
    if (id < (UserAVL->users->idUser))
        UserAVL->esq  = insert(UserAVL->esq, u);
    else if (id > UserAVL->users->idUser)
        UserAVL->dir = insert(UserAVL->dir, u);
    else // Equal keys are not allowed in BST
        return UserAVL;

 
    /* 2. Update altura of this ancestor UserAVL */
    UserAVL->altura = 1 + max(altura(UserAVL->esq),
                           altura(UserAVL->dir));
    
 
    /* 3. Get the balance factor of this ancestor
          UserAVL to check whether this UserAVL became
          unbalanced */
    int balance = getBalance(UserAVL);
 
    // If this UserAVL becomes unbalanced, then
    // there are 4 cases
 
    // esq esq Case
    if (balance > 1 && id < UserAVL->esq->users->idUser)
        return rightRotate(UserAVL);
 
    // dir dir Case
    if (balance < -1 && id > UserAVL->dir->users->idUser)
        return leftRotate(UserAVL);
 
    // esq dir Case
    if (balance > 1 && id > UserAVL->esq->users->idUser)
    {
        UserAVL->esq =  leftRotate(UserAVL->esq);
        return rightRotate(UserAVL);
    }
 
    // dir esq Case
    if (balance < -1 && id < UserAVL->dir->users->idUser)
    {
        UserAVL->dir = rightRotate(UserAVL->dir);
        return leftRotate(UserAVL);
    }

    /* return the (unchanged) UserAVL pointer */
    return UserAVL;
}

//----------------------------------------------------------------------
//----------------------------------------------------------------------

/* Insere valores tipo long, 
    usada também como auxiliar em algumas queries para guardar ids*/

int alturaL(struct LongAVL *N)
{
    if (N == NULL)
    return 0;
    return N->altura;
}
 

/* Helper function that allocates a new LongAVL with the given u and
    NULL esq and dir pointers. */
struct LongAVL *newNodeL(long u)
{
    struct LongAVL *LongAVL = (struct LongAVL*) malloc(sizeof(struct LongAVL));

    LongAVL->valor   = u;
    LongAVL->esq   = NULL;
    LongAVL->dir  = NULL;
    LongAVL->altura = 1;  // new LongAVL is initially added at leaf

    return(LongAVL);
}
 
// A utility function to dir rotate subtree rooted with y
// See the diagram given above.
struct LongAVL *rightRotateL(struct LongAVL *y)
{
    struct LongAVL *x = y->esq;
    struct LongAVL *T2 = x->dir;
 
    // Perform rotation
    x->dir = y;
    y->esq = T2;
 
    // Update heights
    y->altura = max(alturaL(y->esq), alturaL(y->dir))+1;
    x->altura = max(alturaL(x->esq), alturaL(x->dir))+1;
 
    // Return new root
    return x;
}
 
// A utility function to esq rotate subtree rooted with x
// See the diagram given above.
struct LongAVL *leftRotateL(struct LongAVL *x)
{
    struct LongAVL *y = x->dir;
    struct LongAVL *T2 = y->esq;
 
    // Perform rotation
    y->esq = x;
    x->dir = T2;
 
    //  Update heights
    x->altura = max(alturaL(x->esq), alturaL(x->dir))+1;
    y->altura = max(alturaL(y->esq), alturaL(y->dir))+1;
 
    // Return new root
    return y;
}
 
// Get Balance factor of LongAVL N
int getBalanceL(struct LongAVL *N)
{
    if (N == NULL)
        return 0;
    return alturaL(N->esq) - alturaL(N->dir);
}
 
// Recursive function to insert u in subtree rooted
// with LongAVL and returns new root of subtree.
struct LongAVL *insertL(struct LongAVL *LongAVL, long id)
{
    /* 1.  Perform the normal BST insertion */
    if (LongAVL == NULL)
        return(newNodeL(id));
    
    //printf("A inserir na arvore %ld\n", LongAVL->valor);
    if (id < (LongAVL->valor))
        LongAVL->esq  = insertL(LongAVL->esq, id);
    else if (id > LongAVL->valor)
        LongAVL->dir = insertL(LongAVL->dir, id);
    else // Equal keys are not allowed in BST
        return LongAVL;

 
    /* 2. Update altura of this ancestor LongAVL */
    LongAVL->altura = 1 + max(alturaL(LongAVL->esq),
                           alturaL(LongAVL->dir));
    
 
    /* 3. Get the balance factor of this ancestor
          LongAVL to check whether this LongAVL became
          unbalanced */
    int balance = getBalanceL(LongAVL);
 
    // If this LongAVL becomes unbalanced, then
    // there are 4 cases
 
    // esq esq Case
    if (balance > 1 && id < LongAVL->esq->valor)
        return rightRotateL(LongAVL);
 
    // dir dir Case
    if (balance < -1 && id > LongAVL->dir->valor)
        return leftRotateL(LongAVL);
 
    // esq dir Case
    if (balance > 1 && id > LongAVL->esq->valor)
    {
        LongAVL->esq =  leftRotateL(LongAVL->esq);
        return rightRotateL(LongAVL);
    }
 
    // dir esq Case
    if (balance < -1 && id < LongAVL->dir->valor)
    {
        LongAVL->dir = rightRotateL(LongAVL->dir);
        return leftRotateL(LongAVL);
    }

    /* return the (unchanged) LongAVL pointer */
    return LongAVL;
}
//----------------------------------------------------------------------
//----------------------------------------------------------------------

//Insere Posts ordenado por id de posts

int alturaP(struct PostAVL *N)
{
    if (N == NULL)
    return 0;
    return N->altura;
}
 /* Helper function that allocates a new PostAVL with the given u and
    NULL esq and dir pointers. */
 
// A utility function to dir rotate subtree rooted with y
// See the diagram given above.
struct PostAVL *rightRotateP(struct PostAVL *y)
{
    struct PostAVL *x = y->esq;
    struct PostAVL *T2 = x->dir;
 
    // Perform rotation
    x->dir = y;
    y->esq = T2;
 
    // Update heights
    y->altura = max(alturaP(y->esq), alturaP(y->dir))+1;
    x->altura = max(alturaP(x->esq), alturaP(x->dir))+1;
 
    // Return new root
    return x;
}
 
// A utility function to esq rotate subtree rooted with x
// See the diagram given above.
struct PostAVL *leftRotateP(struct PostAVL *x)
{
    struct PostAVL *y = x->dir;
    struct PostAVL *T2 = y->esq;
 
    // Perform rotation
    y->esq = x;
    x->dir = T2;
 
    //  Update heights
    x->altura = max(alturaP(x->esq), alturaP(x->dir))+1;
    y->altura = max(alturaP(y->esq), alturaP(y->dir))+1;
 
    // Return new root
    return y;
}
 
// Get Balance factor of PostAVL N
int getBalanceP(struct PostAVL *N)
{
    if (N == NULL)
        return 0;
    return alturaP(N->esq) - alturaP(N->dir);
}

struct PostAVL* avlInit(Post u){

    struct PostAVL* postAVL = (struct PostAVL*)
                        malloc(sizeof(struct PostAVL));
    postAVL->post   = u;
    postAVL->esq   = NULL;
    postAVL->dir  = NULL;
    postAVL->altura = 1;  // new PostAVL is initially added at leaf
    return(postAVL);
}
// Recursive function to insert u in subtree rooted
// with PostAVL and returns new root of subtree.
struct PostAVL* insertP(struct PostAVL* postAVL, Post u, long id)
{
    /* 1.  Perform the normal BST insertion */
        //postAVL = avlInit();

    if (postAVL == NULL){
        //printf("A arvore esta null\n");
        postAVL = avlInit(u);
       
        //printf("A inserir na arvore vazia %ld\n",id);
        return postAVL;
    }
    //printf("A inserir na arvore %ld\n", id);
    if (id < (postAVL->post->idPost))
        postAVL->esq  = insertP(postAVL->esq, u,id);
    else if (id > postAVL->post->idPost)
        postAVL->dir = insertP(postAVL->dir, u,id);
    else // Equal keys are not allowed in BST
        return postAVL;

 
    /* 2. Update altura of this ancestor PostAVL */
    postAVL->altura = 1 + max(alturaP(postAVL->esq),
                           alturaP(postAVL->dir));
    
 
    /* 3. Get the balance factor of this ancestor
          PostAVL to check whether this PostAVL became
          unbalanced */
    int balance = getBalanceP(postAVL);
 
    // If this PostAVL becomes unbalanced, then
    // there are 4 cases
 
    // esq esq Case
    if (balance > 1 && id < postAVL->esq->post->idPost)
        return rightRotateP(postAVL);
 
    // dir dir Case
    if (balance < -1 && id > postAVL->dir->post->idPost)
        return leftRotateP(postAVL);
 
    // esq dir Case
    if (balance > 1 && id > postAVL->esq->post->idPost)
    {
        postAVL->esq =  leftRotateP(postAVL->esq);
        return rightRotateP(postAVL);
    }
 
    // dir esq Case
    if (balance < -1 && id < postAVL->dir->post->idPost)
    {
        postAVL->dir = rightRotateP(postAVL->dir);
        return leftRotateP(postAVL);
    }

    /* return the (unchanged) PostAVL pointer */
    
    return postAVL;
}

User searchAVLUsersById(UAVL u, int id){
    UAVL ua = u;
    if(ua==NULL)return NULL;

    if(ua->users->idUser==id) return ua->users;

    else{
        if(id<ua->users->idUser)
            return(searchAVLUsersById(ua->esq,id));
        else
            return(searchAVLUsersById(ua->dir,id));
    }   
}

//----------------------------------------------------------------------
//----------------------------------------------------------------------

/*Insere Posts ordenado por parentId, caso o parentId já se encontre inserido na avl
    guarda sempre aquele que tiver melhor média (querie 10)*/

float calculaMedia(Post resposta, UAVL ua){
    float pontuacao = -1;
    if(resposta!=NULL){
        User u = searchAVLUsersById(ua,resposta->owner);
        if(u==NULL) printf("NULL: %ld\n",resposta->idPost);
        pontuacao = (resposta->score *  0.45)
                        +(u->reputation * 0.25)
                        +(resposta->nVotes * 0.2)
                        +(resposta->comentCount * 0.1);
    }
    return pontuacao;
}

struct PostAVL* insertParent(struct PostAVL* postAVL, Post u, long id, UAVL uavl)
{
    /* 1.  Perform the normal BST insertion */
        //postAVL = avlInit();
    UAVL ua = uavl;

    if (postAVL == NULL){
        //printf("A arvore esta null\n");
        postAVL = avlInit(u);
       
        //printf("A inserir na arvore vazia %ld\n",id);
        return postAVL;
    }
    //printf("A inserir na arvore %ld\n", id);
    if (id < (postAVL->post->parentId))
        postAVL->esq  = insertParent(postAVL->esq, u,id,ua);
    else if (id > postAVL->post->parentId)
        postAVL->dir = insertParent(postAVL->dir, u,id,ua);
    
    else{ // Equal keys are not allowed in BST
        //swap
        if(calculaMedia(postAVL->post,ua)<calculaMedia(u,ua))
            postAVL->post = u;
        return postAVL;
    }

 
    /* 2. Update altura of this ancestor PostAVL */
    postAVL->altura = 1 + max(alturaP(postAVL->esq),
                           alturaP(postAVL->dir));
    
 
    /* 3. Get the balance factor of this ancestor
          PostAVL to check whether this PostAVL became
          unbalanced */
    int balance = getBalanceP(postAVL);
 
    // If this PostAVL becomes unbalanced, then
    // there are 4 cases
 
    // esq esq Case
    if (balance > 1 && id < postAVL->esq->post->parentId)
        return rightRotateP(postAVL);
 
    // dir dir Case
    if (balance < -1 && id > postAVL->dir->post->parentId)
        return leftRotateP(postAVL);
 
    // esq dir Case
    if (balance > 1 && id > postAVL->esq->post->parentId)
    {
        postAVL->esq =  leftRotateP(postAVL->esq);
        return rightRotateP(postAVL);
    }
 
    // dir esq Case
    if (balance < -1 && id < postAVL->dir->post->parentId)
    {
        postAVL->dir = rightRotateP(postAVL->dir);
        return leftRotateP(postAVL);
    }

    /* return the (unchanged) PostAVL pointer */
    
    return postAVL;
}



//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

/*Insere Votos por id de post, somando os votos à classificaçao do post */

int alturaV(struct VoteAVL *N)
{
    if (N == NULL)
    return 0;
    return N->altura;
}

void somaVotos(VAVL va, Vote newVote){
    //printf("SOMAVOTOS\n");
    VAVL v = va;
    long id = newVote->postId;
    int voto = newVote->voteType;

    if(v!=NULL){
        if(v->postId==id){
            if(voto==2){
                v->classPost +=1;
            }
            else
                if(voto==1){
                    v->classPost -=1;
                }
        }
        else
            if(v->postId>id)
                somaVotos(v->esq,newVote);
            else
                somaVotos(v->dir,newVote);
    }

}


struct VoteAVL *newNodeV(Vote u)
{
    struct VoteAVL *VoteAVL = (struct VoteAVL*) malloc(sizeof(struct VoteAVL));

    if(u!=NULL){
        VoteAVL->postId = u->postId;
        if(u->voteType==2)
            VoteAVL->classPost = 1;
        else
            if(u->voteType==1)
                VoteAVL->classPost = -1;
        VoteAVL->esq   = NULL;
        VoteAVL->dir  = NULL;
        VoteAVL->altura = 1;  // new VoteAVL is initially added at leaf
        free(u);
    }
    return(VoteAVL);
}
 
// A utility function to dir rotate subtree rooted with y
// See the diagram given above.
struct VoteAVL *rightRotateV(struct VoteAVL *y)
{
    struct VoteAVL *x = y->esq;
    struct VoteAVL *T2 = x->dir;
 
    // Perform rotation
    x->dir = y;
    y->esq = T2;
 
    // Update heights
    y->altura = max(alturaV(y->esq), alturaV(y->dir))+1;
    x->altura = max(alturaV(x->esq), alturaV(x->dir))+1;
 
    // Return new root
    return x;
}
 
// A utility function to esq rotate subtree rooted with x
// See the diagram given above.
struct VoteAVL *leftRotateV(struct VoteAVL *x)
{
    struct VoteAVL *y = x->dir;
    struct VoteAVL *T2 = y->esq;
 
    // Perform rotation
    y->esq = x;
    x->dir = T2;
 
    //  Update heights
    x->altura = max(alturaV(x->esq), alturaV(x->dir))+1;
    y->altura = max(alturaV(y->esq), alturaV(y->dir))+1;
 
    // Return new root
    return y;
}
 
// Get Balance factor of VoteAVL N
int getBalanceV(struct VoteAVL *N)
{
    if (N == NULL)
        return 0;
    return alturaV(N->esq) - alturaV(N->dir);
}
 
// Recursive function to insert u in subtree rooted
// with VoteAVL and returns new root of subtree.
struct VoteAVL *insertV(struct VoteAVL *VoteAVL, Vote u)
{
    long id = u->postId;
    /* 1.  Perform the normal BST insertion */
    if (VoteAVL == NULL)
        return(newNodeV(u));
    
    //printf("A inserir na arvore %ld\n", VoteAVL->vote->postId);
    if (id < (VoteAVL->postId))
        VoteAVL->esq  = insertV(VoteAVL->esq, u);
    else if (id > VoteAVL->postId)
        VoteAVL->dir = insertV(VoteAVL->dir, u);
    else{ // Equal keys are not allowed in BST

        somaVotos(VoteAVL,u);

        return VoteAVL; //vou ter que somar o nvote ao novo
    }

 
    /* 2. Update altura of this ancestor VoteAVL */
    VoteAVL->altura = 1 + max(alturaV(VoteAVL->esq),
                           alturaV(VoteAVL->dir));
    
 
    /* 3. Get the balance factor of this ancestor
          VoteAVL to check whether this VoteAVL became
          unbalanced */
    int balance = getBalanceV(VoteAVL);
 
    // If this VoteAVL becomes unbalanced, then
    // there are 4 cases
 
    // esq esq Case
    if (balance > 1 && id < VoteAVL->esq->postId)
        return rightRotateV(VoteAVL);
 
    // dir dir Case
    if (balance < -1 && id > VoteAVL->dir->postId)
        return leftRotateV(VoteAVL);
 
    // esq dir Case
    if (balance > 1 && id > VoteAVL->esq->postId)
    {
        VoteAVL->esq =  leftRotateV(VoteAVL->esq);
        return rightRotateV(VoteAVL);
    }
 
    // dir esq Case
    if (balance < -1 && id < VoteAVL->dir->postId)
    {
        VoteAVL->dir = rightRotateV(VoteAVL->dir);
        return leftRotateV(VoteAVL);
    }

    /* return the (unchanged) VoteAVL pointer */
    return VoteAVL;
}

//-----------------------------------------------------------
//-----------------------------------------------------------

//Insere tags por ordem alfabetica

int alturaT(struct TagAVL *N)
{
    if (N == NULL)
    return 0;
    return N->altura;
}


/* Helper function that allocates a new TagAVL with the given u and
    NULL esq and dir pointers. */
struct TagAVL *newNodeT(Tag u, long id)
{
    struct TagAVL *TagAVL = (struct TagAVL*) malloc(sizeof(struct TagAVL));

    if(u!=NULL){
        TagAVL->tag = (struct Tag*) malloc(sizeof(struct Tag));
        TagAVL->tag  = u;
        TagAVL->tag->idTag   = id;
        TagAVL->tag->count = 1;
        TagAVL->esq   = NULL;
        TagAVL->dir  = NULL;
        TagAVL->altura = 1;  // new TagAVL is initially added at leaf
    }
    return(TagAVL);
}
 
// A utility function to dir rotate subtree rooted with y
// See the diagram given above.
struct TagAVL *rightRotateT(struct TagAVL *y)
{
    struct TagAVL *x = y->esq;
    struct TagAVL *T2 = x->dir;
 
    // Perform rotation
    x->dir = y;
    y->esq = T2;
 
    // Update heights
    y->altura = max(alturaT(y->esq), alturaT(y->dir))+1;
    x->altura = max(alturaT(x->esq), alturaT(x->dir))+1;
 
    // Return new root
    return x;
}
 
// A utility function to esq rotate subtree rooted with x
// See the diagram given above.
struct TagAVL *leftRotateT(struct TagAVL *x)
{
    struct TagAVL *y = x->dir;
    struct TagAVL *T2 = y->esq;
 
    // Perform rotation
    y->esq = x;
    x->dir = T2;
 
    //  Update heights
    x->altura = max(alturaT(x->esq), alturaT(x->dir))+1;
    y->altura = max(alturaT(y->esq), alturaT(y->dir))+1;
 
    // Return new root
    return y;
}
 
// Get Balance factor of TagAVL N
int getBalanceT(struct TagAVL *N)
{
    if (N == NULL)
        return 0;
    return alturaT(N->esq) - alturaT(N->dir);
}
 
// Recursive function to insert u in subtree rooted
// with TagAVL and returns new root of subtree.
struct TagAVL *insertT(struct TagAVL *TagAVL, Tag u, long id)
{
    /* 1.  Perform the normal BST insertion */
    if (TagAVL == NULL)
        return(newNodeT(u,id));
    
    //printf("A inserir na arvore %ld\n", TagAVL->Tags->idTag);
    if (strcmp(u->name,TagAVL->tag->name)<0)
        TagAVL->esq  = insertT(TagAVL->esq, u,id);
    else if (strcmp(u->name,TagAVL->tag->name)>0)
        TagAVL->dir = insertT(TagAVL->dir, u,id);
    else{ // Equal keys are not allowed in BST

        TagAVL->tag->count++;

        return TagAVL;
    }

 
    /* 2. Update altura of this ancestor TagAVL */
    TagAVL->altura = 1 + max(alturaT(TagAVL->esq),
                           alturaT(TagAVL->dir));
    
 
    /* 3. Get the balance factor of this ancestor
          TagAVL to check whether this TagAVL became
          unbalanced */
    int balance = getBalanceT(TagAVL);
 
    // If this TagAVL becomes unbalanced, then
    // there are 4 cases
 
    // esq esq Case
    if (balance > 1 && strcmp(u->name, TagAVL->esq->tag->name)<0)
        return rightRotateT(TagAVL);
 
    // dir dir Case
    if (balance < -1 && strcmp(u->name, TagAVL->dir->tag->name)>0)
        return leftRotateT(TagAVL);
 
    // esq dir Case
    if (balance > 1 && strcmp(u->name, TagAVL->esq->tag->name)>0)
    {
        TagAVL->esq =  leftRotateT(TagAVL->esq);
        return rightRotateT(TagAVL);
    }
 
    // dir esq Case
    if (balance < -1 && strcmp(u->name, TagAVL->dir->tag->name)<0)
    {
        TagAVL->dir = rightRotateT(TagAVL->dir);
        return leftRotateT(TagAVL);
    }

    /* return the (unchanged) TagAVL pointer */
    return TagAVL;
}
