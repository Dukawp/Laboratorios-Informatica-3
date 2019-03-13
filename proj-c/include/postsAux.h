#include "../include/usersAux.h"
#include "../include/struct.h"

typedef struct Post{
    long idPost;
    int postType;
    Date creationDate;
    char* title;
    long parentId;
    char* tags;
    long score;
    long favCount;
    long owner;
    long nVotes;
    int comentCount;
    int answerCount;
} *Post;

typedef struct PostAVL{
    Post post;
    struct PostAVL* esq;
    struct PostAVL* dir;
    int altura;
} *AVLT;


int comparaDatasTotal(Date d1, Date d2);
int comparaDatas(Date d1, Date d2);
LONG_pair somaPair(LONG_pair pair1, LONG_pair pair2);

Post searchAVLPosts(AVLT p, int id);
long contaPosts(AVLT p, Date begin, Date end, long posts);

int toArrayP(AVLT u, Post* arrayP, int n, int i);
int searchTag(TagLL tags, char* tag);
int containsTag(struct Post* post, char* tag);
int postAVLSize(AVLT p);
AVLT guardaPerguntasWithTag(AVLT p,char* tag, Date begin, Date end, AVLT perguntas);

Post* guardaArrayPosts(Post p, Post* arrayP);
void guardaPostID(AVLT p,Post* arrayP, long id);

void sortByNvotes(Post arr[], int size);
void guardaRespostas(AVLT p, Date begin, Date end, Post* arrayP, int N);

void sortByNAnswers(Post arr[], int size);
void guardaPerguntas(AVLT p, Date begin, Date end, Post* arrayP, int N);


Post searchUserInAnswer(AVLT res, long idP, long idU, Post p);
Post searchUserInQuestion(AVLT res, long idP, long idU, Post p);
Post searchAVLPostsByOwner(AVLT p, long idOwner, LAVL questions);
void ordenaByQuestion(TAD_community com, Post arr[], int size);

void sortByDate(Post arr[], int N);
Post* noArray(Post* arrayP, Post p, int N);
int comparaTitle(char* title, char* word);
Post* postContains(struct PostAVL* aux, char* word, Post* arrayP, int N);

Post answerLookUp(AVLT res, long id);

Post searchAVLPostsSR(AVLT p, long idOwner, LAVL questions);

void imprimePostAVL(struct PostAVL* u);