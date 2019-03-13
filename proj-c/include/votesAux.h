typedef struct Vote{
    long postId;
    int voteType;
} *Vote;

typedef struct VoteAVL{
    long postId;
    int classPost;
    struct VoteAVL* esq;
    struct VoteAVL* dir;
    int altura;
} *VAVL;