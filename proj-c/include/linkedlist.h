#include <stdbool.h>
#include "../include/arvores.h"


bool isEmpty();
int length();
void imprimeLL(struct llista* ll);
struct llista* insertAtDate(struct llista* ll,Date data, struct Post* post, struct TagLL* tags);


//

struct TagLL* tagInit(char* tag);
struct TagLL* insertTag(struct TagLL* tags, char* tag);
void imprimeTag(struct TagLL** tags);

