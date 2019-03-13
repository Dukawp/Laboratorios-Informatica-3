typedef struct Tag{
	long idTag;
    char* name;
    long count;
} *Tag;

typedef struct TagLL{
    Tag tag;
    struct TagLL* next;
} *TagLL;

typedef struct TagAVL{
    Tag tag;
    struct TagAVL* esq;
    struct TagAVL* dir;
    int altura;
} *TAVL;
