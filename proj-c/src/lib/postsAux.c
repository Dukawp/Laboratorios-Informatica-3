#include "../include/arvores.h"

//querie1

/*	searchAVLPosts
 *
 *	Procura post por id na avl de posts
 */
Post searchAVLPosts(AVLT p, int id){
	AVLT pa = p;
	if(pa==NULL) return NULL;

	if(pa->post->idPost==id)return pa->post;

	else{
		if(id<pa->post->idPost)
			return(searchAVLPosts(pa->esq,id));
		else
			return(searchAVLPosts(pa->dir,id));
	}

}

/*	contaPosts
 *
 *	Conta posts entre as dadas datas
 * 	devolvendo o numero total de posts do periodo
 */
long contaPosts(AVLT p, Date begin, Date end, long posts){

	AVLT pa = p;

	if(pa!=NULL){

		if((comparaDatasTotal(pa->post->creationDate,begin)>=0)&&(comparaDatasTotal(pa->post->creationDate,end)<=0)){
				posts=1;
		}
		return posts += contaPosts(pa->esq,begin,end,posts) + contaPosts(pa->dir,begin,end,posts);
	}
	else 
		return 0;

}

//querie4

/* 	toArrayP
 *
 *	Converte AVLT, avl de posts, num array de posts
 */
int toArrayP(AVLT u, Post* arrayP, int n, int i){
	AVLT ua = u;

	if(ua!=NULL){
		i = toArrayP(ua->esq,arrayP,n,i);
		arrayP[i] = ua->post;
		i++;
		i = toArrayP(ua->dir,arrayP,n,i);
	}

	return i;
}

/*	searchTag
 *
 *	Procura tag na lista ligada de tags
 *	Devolve 1 caso exista e 0 caso contrário
 */
int searchTag(TagLL tags, char* tag){

	TagLL t = tags;

	while(t!=NULL){
		if((strcmp(t->tag->name,tag))==0)
			return 1;
		t=t->next;
	}
	return 0;
}

/*	containsTag
 *
 *	Verifica se um post contem o nomde de uma dada tag
 */
int containsTag(struct Post* post, char* tag){

    char* auxt ;
    auxt = strtok(post->tags,"<>");
    while(auxt != NULL){
    	    printf("%s\n", auxt);

        if(strcmp(tag,auxt)==0){
            return 1;
        }
        auxt = strtok(NULL,"<>");
    }
    return 0;
}

/*	postAVLSize
 *
 *	Calcula numero de posts presentes na avl de posts
 */
int postAVLSize(AVLT p){
	AVLT post = p;
	if(post!=NULL){
		return 1 + postAVLSize(post->esq) + postAVLSize(post->dir);
	}
	else return 0;
}

/*	guardaPerguntasWithTag
 *
 *	Guarda perguntas contendo uma dada tag entre um dado perído
 *	numa árvore de posts ordenada por id de post
 */
AVLT guardaPerguntasWithTag(AVLT p,char* tag, Date begin, Date end, AVLT perguntas){

	AVLT pa = p;

	if(pa!=NULL){

		if((comparaDatasTotal(pa->post->creationDate,begin)>=0)&&(comparaDatasTotal(pa->post->creationDate,end)<=0)){

			if(strstr(pa->post->tags,tag)!=NULL){
				perguntas = insertP(perguntas,pa->post,pa->post->idPost);
			}
		
		}

		perguntas = guardaPerguntasWithTag(pa->esq,tag,begin,end,perguntas); 
		perguntas = guardaPerguntasWithTag(pa->dir,tag,begin,end,perguntas);
	}
	return perguntas;

}

//Query 5 
/* guardaArrayPosts
* compara datas dos posts para inserir apenas os mais recentes
*/
Post* guardaArrayPosts(Post p, Post* arrayP){

	int i=0;

	if(p!=NULL){

		while(i<10){

			if(arrayP[i]==NULL){
				arrayP[i]=p;
				return arrayP;
			}

			i++;
		}

		i=0;

		while(i<10){

			if(comparaDatasTotal(arrayP[i]->creationDate, p->creationDate) == -1){
				arrayP[i] = p;
				i=10;
			}

			i++;
		}

	}
	
	return arrayP;

}
/*
*procura posts contendo um ID
*para guardar num Array
*/

void guardaPostID(AVLT p,Post* arrayP, long id){

	AVLT auxP = p;

	if(auxP != NULL){
		if(auxP->post->owner == id){
			arrayP = guardaArrayPosts(auxP->post,arrayP);
		}

		guardaPostID(auxP->esq,arrayP,id);
		guardaPostID(auxP->dir,arrayP,id);
	}
}

//querie 6

/*	sortByNvotes
 *
 *	Ordena array de posts por numero de votos
 */
void sortByNvotes(Post arr[],int size)
{
    for (int i = 1; i < size; i++)
    {
        Post value = arr[i];
        int j;
        /* Shift elements over by one slot to make way for the insertion */
        for (j = i; j > 0 && value->nVotes > arr[j - 1]->nVotes; j--)
        {
            arr[j] = arr[j - 1];
        }
        arr[j] = value;
    }
}

/*	guardaRespostasAux
 *
 *	Guarda respostas com maior numero de votos
 *	num array de posts, valor de retorno
 */
Post* guardaRespostasAux(Post p, Post* arrayP, int N){

	int i=0;
	int dif = 0;
	int indice = -1;

	if(p!=NULL){

		while(i<N){

			if(arrayP[i]==NULL){
				arrayP[i]=p;
				return arrayP;
			}
			i++;
		}

		i=0;

		if(arrayP[i]!=NULL){
			dif = arrayP[i]->nVotes - p->nVotes;
			indice = i;
		}

		while(i<N){

			if(arrayP[i]->nVotes - p->nVotes<dif){
				dif = arrayP[i]->nVotes - p->nVotes;
				indice = i;
			}

			i++;
		}

		if((indice!=-1)&&(arrayP[indice]->nVotes < p->nVotes)){
			arrayP[indice] = p;
		}
		
	}

	return arrayP;
}

/*	guardaRespostas
 *
 *	Dada uma avl de respostas, guarda respostas entre as dadas datas
 *	num array de posts.
 */
void guardaRespostas(AVLT p, Date begin, Date end, Post* arrayP, int N){


	AVLT pa = p;

	if(pa!=NULL){

		if((comparaDatasTotal(pa->post->creationDate,begin)>=0)&&(comparaDatasTotal(pa->post->creationDate,end)<=0)){

			arrayP=guardaRespostasAux(pa->post,arrayP,N);
		
		}

		guardaRespostas(pa->esq,begin,end,arrayP,N);
		guardaRespostas(pa->dir,begin,end,arrayP,N);
	}


}

//querie 7

/*	sortByNAnswers
 *
 *	Ordena array de posts por numero de respostas
 */
void sortByNAnswers(Post arr[], int size)
{
    for (int i = 1; i < size; i++)
    {
        Post value = arr[i];
        int j;
        /* Shift elements over by one slot to make way for the insertion */
        for (j = i; j > 0 && value->answerCount > arr[j - 1]->answerCount; j--)
        {
            arr[j] = arr[j - 1];
        }
        arr[j] = value;
    }
}

/*	guardaPerguntasAux
 *
 *	Guarda as N perguntas com maior numero de respostas
 *	num array de posts, devolvendo-o
 */
Post* guardaPerguntasAux(Post p, Post* arrayP, int N){

	int i=0;
	int dif = 0;
	int indice = -1;

	if(p!=NULL){

		while(i<N){

			if(arrayP[i]==NULL){
				arrayP[i]=p;
				return arrayP;
			}

			i++;
		}

		i=0;

		if(arrayP[i]!=NULL){
			dif = arrayP[i]->answerCount - p->answerCount;
			indice = i;
		}

		while(i<N){

			if(arrayP[i]->answerCount - p->answerCount<dif){
				dif = arrayP[i]->answerCount - p->answerCount;
				indice = i;
			}

			i++;
		}

		if((indice!=-1)&&(arrayP[indice]->answerCount < p->answerCount)){
			arrayP[indice] = p;
		}
		
	}
	return arrayP;

}


/*	guardaPerguntas
 *
 *	Guarda perguntas, presentes na avl de perguntas,
 *	entre as dadas datas, no array de posts
 */
void guardaPerguntas(AVLT p, Date begin, Date end, Post* arrayP, int N){


	AVLT pa = p;

	if(pa!=NULL){

		if((comparaDatasTotal(pa->post->creationDate,begin)>=0)&&(comparaDatasTotal(pa->post->creationDate,end)<=0)){

			arrayP=guardaPerguntasAux(pa->post,arrayP,N);
		
		}


		guardaPerguntas(pa->esq,begin,end,arrayP,N);
		guardaPerguntas(pa->dir,begin,end,arrayP,N);
	}

}

//querie8

/*	sortByDate
 *
 *	Ordena array de posts por data de criaçao,
 *	da mais recente para a mais antiga
 */
void sortByDate(Post arr[], int N)
{
    for (int i = 1; i < N; i++)
    {
        Post value = arr[i];
        int j;
        /* Shift elements over by one slot to make way for the insertion */
        for (j = i; j > 0 && (comparaDatasTotal(value->creationDate, arr[j - 1]->creationDate) ==1) ; j--)
        {
            arr[j] = arr[j - 1];
        }
        arr[j] = value;
    }
}

/* 
* compara datas dos posts para inserir apenas os mais recentes
*/

Post* noArray(Post* arrayP, Post p, int N){
	int i = 0;

	while(i<N){
		if(arrayP[i]==NULL){
			arrayP[i]=p;
			return arrayP;
		}
		i++;
	}

	i=0;
	while(i<N){
		if(comparaDatasTotal(arrayP[i]->creationDate,p->creationDate) == 1){
			arrayP[i] = p;
			i=N;
		}
		i++;
	}

	return arrayP;
}

/*
*percorre o title para verificar se tem alguma string igual ao word
*/
int comparaTitle(char* title, char* word){

	char* auxt;

    //alocar espaço para o token e copiar a tag do post para a variavel auxiliar
    auxt=malloc(strlen((char*)title)+1);
    strcpy(auxt,title);
    auxt = strtok(auxt," ,.-");

    while(auxt != NULL){

        if(strcmp(word,auxt)==0){
            return 1;
        }
        auxt = strtok(NULL," ,.-");
    }
    return 0;
}

/*
* procura Posts com a word no title
*/
Post* postContains(struct PostAVL* aux, char* word, Post* arrayP, int N){

	if(aux == NULL)
		return arrayP;

	if(comparaTitle(aux->post->title, word)){
		arrayP = noArray(arrayP, aux->post, N);
	}

	postContains(aux->esq,word,arrayP,N);
	postContains(aux->dir,word,arrayP,N);
	
	return arrayP;

}


//querie9

/*	searchUserInAnswer
 *
 *	Procura post de um dado user e um dado parentId na AVLT de respostas
 */
Post searchUserInAnswer(AVLT res, long idP, long idU, Post p){

	AVLT respostas = res;

	if(respostas!=NULL){
		if((respostas->post->parentId==idP)){
		
			if(respostas->post->owner==idU){
					return  respostas->post;
			}
			}

		p = searchUserInAnswer(respostas->esq,idP,idU,p); 
		if(p==NULL)
			p=searchUserInAnswer(respostas->dir,idP,idU,p);

	}
	return p;
}

/*	searchUserInQuestion
 *
 *	Procura post de um dado user e um dado idPost na AVLT de perguntas
 */
Post searchUserInQuestion(AVLT res, long idP, long idU, Post p){

	AVLT respostas = res;

	if(respostas!=NULL){
		if((p!=NULL))

		if((respostas->post->idPost==idP)){

			if(respostas->post->owner==idU){
				return respostas->post;
			}
		}

		p = searchUserInQuestion(respostas->esq,idP,idU,p);
		p = searchUserInQuestion(respostas->dir,idP,idU,p);

	}
	return p;
}

/*	searchAVLPostsByOwner
 *
 *	Procura post de um dado user na AVLT,
 *  devolvendo o post caso ainda nao exita na LAVL 
 *	que contem os ids dos posts já encontrados.
 */
Post searchAVLPostsByOwner(AVLT p, long idOwner, LAVL questions){
	AVLT pa = p;

	Post ps = NULL;

	if(pa==NULL) return NULL;

	if(pa->post->owner == idOwner){

		if(pa->post->postType==1){
			if((searchLAVLAux(questions,pa->post->idPost))==0)
				return pa->post;
		}

		else
			if(pa->post->postType==2){
				if(searchLAVLAux(questions,pa->post->parentId)==0)
					return pa->post;
			} 
	}

	ps = searchAVLPostsByOwner(pa->esq,idOwner,questions);
	if(ps==NULL)
	ps = searchAVLPostsByOwner(pa->dir,idOwner,questions);

	return ps;

}

/*	ordenaByQuestion
 *
 *	Ordena array de perguntas por ordem decrescente de data de criação,
 *	caso seja resposta procura a pergunta respetiva e guarda-a no array
*/
void ordenaByQuestion(TAD_community com, Post arr[], int size)
{
	LLista ll = com->dataPosts;
	LLista cabeca = ll;

	Post p=NULL;

		for (int i = 0; i < size; i++)
    	{
	        Post value = arr[i];

	        if(value!=NULL){
	        	if(value->postType==2){
	        		while(ll!=NULL){
	        			if((p = searchAVLPosts(ll->postPerguntas,value->parentId))!=NULL){
	        				value = p;
	        				break;
	        			}
	        			ll = ll->next;
	        		}
	        		ll=cabeca;
	        	}
	        }

	        int j;
	        /* Shift elements over by one slot to make way for the insertion */
	        if(value!=NULL){
		        for (j = i; j > 0 && comparaDatasTotal((value->creationDate), (arr[j - 1])->creationDate)>0; j--)
		        {
		            arr[j] = arr[j - 1];
		        }
		        arr[j] = value;
    		}
   	 	}
}


// query 10

/*	answerLookUp
 *	
 *	Procura id de resposta na AVLT de respostas por parentId
 */
Post answerLookUp(AVLT res, long id){

	AVLT respostas = res;

	if(res!=NULL){
		if(respostas->post->parentId==id){
			return respostas->post;
		}
		else
			if(respostas->post->parentId>id){
				return answerLookUp(respostas->esq,id);
			}
			else
				return answerLookUp(respostas->dir,id);

	}
	return NULL;
}

// querie 11

/*
* 	searchAVLPostsSR
*
*	Dado uma avl de posts, um id de owner e uma avl de ids de perguntas
*	devolve uma pergunta do dado owner, uqe não se encontre na 
*	avl de ids
*/
Post searchAVLPostsSR(AVLT p, long idOwner, LAVL questions){
	AVLT pa = p;

	Post ps = NULL;

	if(pa==NULL) return NULL;

	if(pa->post->owner == idOwner){
		if((searchLAVLAux(questions,pa->post->idPost))==0){
			return pa->post;
		}
	}

	ps = searchAVLPostsSR(pa->esq,idOwner,questions);
	if(ps==NULL)
	ps = searchAVLPostsSR(pa->dir,idOwner,questions);

	return ps;

}

//Funcoes Datas Auxiliares

/*	comparaDatasTotal
 *
 *	Comapara datas na totalidade
 */
int comparaDatasTotal(Date d1, Date d2){

	/*
	a<0 d1<d2
	a>0 d1>d2
	a==0 d1=d2

	2018 2017 -> retorna -1 logo 2018 é mais recente
	*/
	
	int dia,mes,ano;
	int dia2,mes2,ano2;
	dia = get_day(d1);
	mes = get_month(d1);
	ano = get_year(d1);

	dia2 = get_day(d2);
	mes2 = get_month(d2);
	ano2 = get_year(d2);

	if(ano==ano2){
		if(mes==mes2){
			if(dia==dia2)
				return 0;
			else
				if(dia<dia2)
					return -1;
				else
					return 1;
					
		}
		else
			if(mes<mes2)
				return-1;
			else
				return 1; 
	}
	else
		if(ano<ano2)
			return -1;
		else
			return 1;
}

/*	comparaDatas
 *
 *	Compara meses e anos
 */
int comparaDatas(Date d1, Date d2){
//printf("comparadtaas\n");
	/*
	a<0 d1<d2
	a>0 d1>d2
	a==0 d1=d2

	2018 2017 -> retorna -1 logo 2018 é mais recente*/
	
	int mes,ano;
	int mes2,ano2;

	mes = get_month(d1);
	ano = get_year(d1);

	mes2 = get_month(d2);
	ano2 = get_year(d2);

	if(ano==ano2){
		if(mes==mes2){
			return 0;
		}
		else
			if(mes<mes2)
				return-1;
			else
				return 1; 
	}
	else
		if(ano<ano2)
			return -1;
		else
			return 1;

}

//Extras

void imprimePostAVL(struct PostAVL* u){
	AVLT uv = u;
	if(uv==NULL) printf("NULL\n");

	if(uv!=NULL){
		Post newPost = uv->post;
		printf("\n::ESTRUTURA::\n");
				printf("PostID -> %ld\n", newPost->idPost);
				printf("Tags -> %s\n", newPost->tags);
				printf("ParentId -> %ld\n", newPost->parentId);
				printf("AnswersCount -> %d\n", newPost->answerCount);
				printf("Title -> %s\n", newPost->title);
				printf("Owner -> %ld\n", newPost->owner);
				printf("postType -> %d\n", newPost->postType);				
				printf("CreationDate -> %d-%d-%d\n", get_day(newPost->creationDate),
											   get_month(newPost->creationDate),
											   get_year(newPost->creationDate));
				printf("::FIM::\n");
		printf("\nESQUERDA\n");
		imprimePostAVL(uv->esq);
		printf("\nDIREITA\n");
		imprimePostAVL(uv->dir);
	}
}