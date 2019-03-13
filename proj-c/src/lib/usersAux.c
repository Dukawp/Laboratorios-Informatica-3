#include "../include/auxiliares.h"


//querie1

/*
 *
 *Procura nome do user na avl
 */
char* searchAVLUsersByOwner(UAVL u, int owner){
	UAVL ua = u;
	if(ua==NULL)return NULL;

	if(ua->users->idUser==owner) return ua->users->name;

	else{
		if(owner<ua->users->idUser)
			return(searchAVLUsersByOwner(ua->esq,owner));
		else
			return(searchAVLUsersByOwner(ua->dir,owner));
	}	
}

//querie3

/*	sortByCountPosts
 *
 *Ordena array de users por numero de posts
 */
void sortByCountPosts(User* arr, int size){
	for (int i = 1; i < size; i++)
    {
        User value = arr[i];
        int j;

        for (j = i; j > 0 && value->countPosts > arr[j - 1]->countPosts; j--)
        {
            arr[j] = arr[j - 1];
        }
        arr[j] = value;
    }
}

/* toArray
 *
 *Converte UAVL, avl de users, num array de User
 */
int toArray(UAVL u, User* arrayP, int n, int i){
	UAVL ua = u;

	if(ua!=NULL){
		i = toArray(ua->esq,arrayP,n,i);
		arrayP[i] = ua->users;
		i++;
		i = toArray(ua->dir,arrayP,n,i);
	}

	return i;

}

/* maxUser
 *
 *	Guarda no array os users ordenado por numero de posts (decrescente)
 */
void maxUser(User* arrayU, User* arrayUFinal, int uN, int N){
	int max = arrayU[0]->countPosts;
	User maxU = NULL;
	int indice = 0;
	int i=0;
	int it=0;
	while(it<N){
		while(i<uN){
			if((arrayU[i]!=NULL)&&(arrayU[i]->countPosts>max)){
				max=arrayU[i]->countPosts;
				maxU = arrayU[i];
				indice = i;
			}
			i++;
		}
		if(maxU->idUser!=-1)
			arrayUFinal[it++] = maxU;
		arrayU[indice]=NULL;
		max=-1;
		i=0;
	}

}

//Querie 5
/*
* procura id numa UAVL (Users)
*/
User get_User(UAVL ua, long id){

	UAVL auxU = ua;
	if(auxU==NULL)return NULL;

	if(auxU->users->idUser == id) return auxU->users;

	else{
		if(id < auxU->users->idUser)
			return(get_User(auxU->esq,id));
		else
			return(get_User(auxU->dir,id));
	}	
}

//Querie9

/* searchLAVLAux
 *
 *Procura id numa LAVL
 */
int searchLAVLAux(LAVL la, long id){

	LAVL l = la;

	if(l!=NULL){
		if(l->valor==id)
			return 1;
		else
			if(l->valor>id)
				return searchLAVLAux(l->esq,id);
			else
				return searchLAVLAux(l->dir,id);
	}

	return 0;

}

/* serachLAVL
*
*Procura id nas LAVL de perguntas e respostas
*
*	1->encontrou pergunta
*	2->encontrou resposta
*	0-> nao encontrou
*/
int searchLAVL(LAVL per, LAVL res, long id){

	int r;

	r = searchLAVLAux(per,id);

	if(r==0){

		r = searchLAVLAux(res,id);
		if(r==1)	
			r = 2;
	}
	return r;

}

//querie 11

/* searchUsersAux
*
*	Procura um dado user na avl de users
*/
User searchUsersAux(long id, UAVL ua){
	UAVL u = ua;

	if(u!=NULL){

		if(u->users->idUser==id){
			return u->users;
		}

		else{
			if(id<u->users->idUser)
				return searchUsersAux(id,u->esq);
			else
				return searchUsersAux(id,u->dir);
		}
	}
	return NULL;
}

/* searchUsers
*
*	Para cada id de user presente na LAVL 
*	procura o seu User correspondente na avl de users
*	e adiciona-o ao array
*/
int searchUsers(LAVL la, UAVL ua,User* arrayU,int i){

	LAVL l = la;

	User u =NULL;

	if(l!=NULL){

		u = searchUsersAux(l->valor,ua);

		arrayU[i++] = u;

		i=searchUsers(l->esq,ua,arrayU,i);

		i=searchUsers(l->dir,ua,arrayU,i);
	}
	return i;
}
/* contaL
*	Conta numero de ids presentes numa avl de tidss
*/
int contaL(LAVL l){
	LAVL la = l;
	if(la!=NULL){
		return 1 + contaL(la->esq) + contaL(la->dir);
	}
	else
		return 0;
}
/* contaU
*	Conta numero de users presentes numa avl de users
*/
int contaU(UAVL l){
	UAVL la = l;
	if(la!=NULL){
		return 1 + contaU(la->esq) + contaU(la->dir);
	}
	else
		return 0;
}
/* contaT
*	Conta numero de tags presentes numa avl de tags
*/
int contaT(TAVL l){
	TAVL la = l;
	if(la!=NULL){
		return 1 + contaT(la->esq) + contaT(la->dir);
	}
	else
		return 0;
}

/* bestRep
*	Guarda numa avl de users, os N users com melhor
* 	reputaçao.
*/
UAVL bestRep(TAD_community com, int N, Date begin, Date end){

	UAVL ua = (UAVL)malloc(sizeof(struct UserAVL));
	ua=NULL;

	int it=0;

	LLista ll = com->dataPosts;

	if((begin!=NULL)&&(end!=NULL)){
		//Data begin é mais antiga que a de end:
		if(comparaDatasTotal(begin,end)<0){
			if(comparaDatas(ll->data,begin)>0){//ll->data é mais recente que begin
					begin=ll->data;
					int mes = get_month(begin);
					int ano = get_year(begin);
					begin = createDate(1,mes,ano);
					return bestRep(com,N,begin,end);
			}	

			while((ll!=NULL)&&(comparaDatas(ll->data,begin)<0)){
				ll = ll->next;
			}

			while((ll!=NULL)&&(comparaDatas(ll->data,end)<=0)){
				int tam = contaL(ll->usersPerguntas);
				User arrayU[tam];
				int i=0;
				while(i<tam){
					arrayU[i]=NULL;
					i++;
				}
				i=0;
				searchUsers(ll->usersPerguntas,com->users,arrayU,it);
				while(i<tam){
					ua = insert(ua,arrayU[i]);
					i++;
				}
				tam = contaL(ll->usersRespostas);
				searchUsers(ll->usersRespostas,com->users,arrayU,it);
				i=0;
				while(i<tam){
					ua = insert(ua,arrayU[i]);
					i++;
				}
				ll = ll->next;
			}
		}

		else{
		//Data end é mais antiga que a begin:
			if(comparaDatasTotal(begin,end)>0){
				if(comparaDatas(ll->data,end)>0){
					end = ll->data;
					int mes = get_month(end);
					int ano = get_year(end);
					end = createDate(1,mes,ano);
					return bestRep(com,N,begin,end);
				}

				while((ll!=NULL)&&(comparaDatas(ll->data,end)<0))
					ll = ll->next;

				while((ll!=NULL)&&(comparaDatas(ll->data,begin)<=0)){
					int tam = contaL(ll->usersPerguntas);
					User arrayU[tam];
					int i=0;
					while(i<tam){
						arrayU[i]=NULL;
						i++;
					}
					i=0;
					searchUsers(ll->usersPerguntas,com->users,arrayU,it);
					while(i<tam){
						ua = insert(ua,arrayU[i]);
						i++;
					}
					tam = contaL(ll->usersRespostas);
					searchUsers(ll->usersRespostas,com->users,arrayU,it);
					i=0;
					while(i<tam){
						ua = insert(ua,arrayU[i]);
						i++;
					}
					ll = ll->next;
				}
			}

		}
	}
	return ua;
}


/*	sortByCountRep
*	Ordena um array de users por reputaçao de user
* 	em ordem decrescente da mesma.
*/
void sortByCountRep(User* arr, int size){
	for (int i = 1; i < size; i++)
    {
        User value = arr[i];
        int j;
        /* Shift elements over by one slot to make way for the insertion */
        if((value!=NULL)&&(arr[j-1]!=NULL)){
	        for (j = i; j > 0 && value->reputation > arr[j - 1]->reputation; j--)
	        {
	            arr[j] = arr[j - 1];
	        }
	        arr[j] = value;
	    }
    }
}

/*	sortByCountTag
*	Ordena um array de tags por número total de tags
* 	em ordem decrescente do numero de tags
*/
void sortByCountTag(Tag* arr, int size){
	for (int i = 1; i < size; i++)
    {
        Tag value = arr[i];
        int j;
        /* Shift elements over by one slot to make way for the insertion */
        if((value!=NULL)&&(arr[j-1]!=NULL)){
	        for (j = i; j > 0 && value->count > arr[j - 1]->count; j--)
	        {
	            arr[j] = arr[j - 1];
	        }
	        arr[j] = value;
	    }
    }
}

/*	avlToArrayU
*	Converte uma avl de users num array de users
*/
int avlToArrayU(UAVL u, User* arrayU, int i){
	UAVL ua = u;
	if(ua!=NULL){
		i = avlToArrayU(u->esq,arrayU,i);
		arrayU[i++] = ua->users;
		i = avlToArrayU(u->dir,arrayU,i);
	}
	return i;
}


/*	avlToArrayT
*	Converte uma avl de tags num array de tags
*/
int avlToArrayT(TAVL u, Tag* arrayU, int i){
	TAVL ua = u;
	if(ua!=NULL){
		i = avlToArrayT(u->esq,arrayU,i);
		arrayU[i++] = ua->tag;
		i = avlToArrayT(u->dir,arrayU,i);
	}
	return i;
}

/* searchTagId
*	Procura numa avl de tags uma tag com um dado nome
*	Devolve a Tag encontrada
*/
Tag searchTagId(TAVL tags, char* name){
	TAVL t=tags;

	if(t!=NULL){
		if(strcmp(t->tag->name,name)==0){
			return t->tag;
		}

		else{
			if(strcmp(t->tag->name,name)>0)
				return searchTagId(tags->esq,name);
			else
				return searchTagId(tags->dir,name);
			}
	}
	return NULL;
}


//Funçoes de impressao
void imprimeUAVL(struct UserAVL* uv){
	if(uv==NULL) printf("NULL\n");

	if(uv!=NULL){
		User newUser = uv->users;
		printf("UserID -> %ld\n", newUser->idUser);
		printf("::ESTRUTURA::\n");
		printf("Name -> %s\n", newUser->name);				
		printf("Rep -> %ld\n", newUser->reputation);
		printf("AboutME -> %s\n\n", newUser->about);
		printf("CountPosts -> %d\n", newUser->countPosts);
		printf("\nESQUERDA\n");
		imprimeUAVL(uv->esq);
		printf("\nDIREITA\n");
		imprimeUAVL(uv->dir);
	}
}

void imprimeLAVL(struct LongAVL* uv){
	if(uv==NULL) printf("NULL\n");

	if(uv!=NULL){
		printf("OwnerID -> %ld\n", uv->valor);
		printf("\nESQUERDA\n");
		imprimeLAVL(uv->esq);
		printf("\nDIREITA\n");
		imprimeLAVL(uv->dir);
	}
}