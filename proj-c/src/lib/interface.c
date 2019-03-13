#include "../include/parser.h"



TAD_community init(){

	TAD_community mystruct = (TAD_community)malloc(sizeof(struct TCD_community));
	
	mystruct->users = (UAVL)malloc(sizeof(struct UserAVL));
	mystruct->users=NULL;

	mystruct->usersNumber=0;

	mystruct->respostasByParent = (AVLT)malloc(sizeof(struct PostAVL));
	mystruct->respostasByParent = NULL;

	mystruct->dataPosts = (LLista)malloc(sizeof(struct llista));
	mystruct->dataPosts->data = NULL;

	mystruct->dataPosts->postPerguntas = (AVLT)malloc(sizeof(struct PostAVL));
	mystruct->dataPosts->postPerguntas = NULL;

	mystruct->dataPosts->postRespostas = (AVLT)malloc(sizeof(struct PostAVL));
	mystruct->dataPosts->postRespostas= NULL;

	mystruct->votes = (VAVL)malloc(sizeof(struct VoteAVL));
	mystruct->votes = NULL;

	mystruct->tags = (TAVL)malloc(sizeof(struct TagAVL));
	mystruct->tags = NULL;

	mystruct->dataPosts->tags = (TagLL)malloc(sizeof(struct TagLL));
	mystruct->dataPosts->tags = NULL;

	mystruct->dataPosts->next= NULL;
	mystruct->dataPosts->prev = NULL;
	return mystruct;	

}


TAD_community load(TAD_community com, char* dump_path){

	char filename[40] = "Users.xml,Votes.xml,Posts.xml,Tags.xml";
	char* file;
	char* nameFiles[3];
	file = strtok(filename,",");
	char* path=mystrdup(dump_path);
	int i=0;

	while((file!=NULL)){
		printf("FILE %s\n", file);
		nameFiles[i]=mystrdup(file);
		strcpy(nameFiles[i],file);
		file=strtok(NULL,",");
		i++;
	}

	i=0;

	while(i<4){
		strcpy(path,dump_path);
		strcat(path,nameFiles[i]);
		com=parsing(com,path);
		i++;
	}

	return com;

}

// query 1
//1 p e 2 r
STR_pair info_from_post(TAD_community com, long id){

	LLista aux = com->dataPosts;

	while(com->dataPosts->prev!=NULL)
		com->dataPosts=com->dataPosts->prev;


	Post post=NULL;

	while((aux!=NULL) && (post==NULL)){
		post = searchAVLPosts(aux->postPerguntas,id);
		if(post==NULL)
			post = searchAVLPosts(aux->postRespostas,id);

		if((post!=NULL) && (post->postType==2)){
			return info_from_post(com,post->parentId);
		}
		aux=aux->next;
	}
	char * name = NULL;

	if((post!=NULL)&&(post->postType==1)){
		name = searchAVLUsersByOwner(com->users,post->owner);

		if((strcmp("NULL",post->title)==0)||(strcmp("NULL",name)==0)){
			return NULL;
		}

		STR_pair pair = create_str_pair(post->title,name);

		return pair;
	}
	return NULL;

}

// query 2

LONG_list top_most_active(TAD_community com, int N){

	LONG_list top = create_list(N);

	UAVL ua = com->users;
	
	User arrayU[com->usersNumber];

	User arrayUFinal[N];
	
	int i=0;
	while(i<com->usersNumber)
		arrayU[i++]=NULL;
	i=0;
	while(i<N)
		arrayUFinal[i++]=NULL;
	i=0;
	toArray(ua,arrayU,com->usersNumber,i);

	i=0;

	if(N<10000){
		maxUser(arrayU,arrayUFinal,com->usersNumber,N);
		int it=0;
		while(i<N){
			if(arrayUFinal[i]->idUser!=-1){
				set_list(top,it++,arrayUFinal[i]->idUser);
			}
				i++;
		}
	}
	else{
		sortByCountPosts(arrayU,com->usersNumber);
		int it=0;
		while(i<N){
			if(arrayU[i]->idUser!=-1){
				set_list(top,it++,arrayU[i]->idUser);
			}
				i++;
		}
	}

	return top;
	
}

// query 3

LONG_pair total_posts(TAD_community com, Date begin, Date end){

	LLista ll = com->dataPosts;

	long perguntas, respostas;
	perguntas = 0;
	respostas = 0;

	if((begin!=NULL)&&(end!=NULL)){
		//Data begin é mais antiga que a de end:
		if(comparaDatasTotal(begin,end)<0){
			if(comparaDatas(ll->data,begin)>0){//ll->data é mais recente que begin
				/*Caso o período passado como argumento seja fora dos limites da lista*/
					begin=ll->data;
					int mes = get_month(begin);
					int ano = get_year(begin);
					begin = createDate(1,mes,ano);
					return total_posts(com,begin,end);
			}	

			while((ll!=NULL)&&(comparaDatas(ll->data,begin)<0)){
				ll = ll->next;
			}


			while((ll!=NULL)&&(comparaDatas(ll->data,end)<=0)){
				perguntas += contaPosts(ll->postPerguntas, begin, end,perguntas);
				respostas += contaPosts(ll->postRespostas, begin, end,respostas);
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
					return total_posts(com,begin,end);
				}

				while((ll!=NULL)&&(comparaDatas(ll->data,end)<0))
					ll = ll->next;

				while((ll!=NULL)&&(comparaDatas(ll->data,begin)<=0)){
					 perguntas += contaPosts(ll->postPerguntas, end, begin,perguntas);
					 respostas += contaPosts(ll->postRespostas, end, begin,respostas);
					 ll = ll->next;
				}
			}

		}
	}

	LONG_pair pair = create_long_pair(perguntas,respostas);

	return pair;
}

// QUERY 4-------------------------------------------------

LONG_list questions_with_tag(TAD_community com, char* tag, Date begin, Date end){
	printf("oooo\n");
	LLista ll = com->dataPosts;

	LONG_list list = NULL;


	int N=0;

	AVLT perguntas = (AVLT)malloc(sizeof(struct PostAVL));
	perguntas=NULL;

	if((begin!=NULL)&&(end!=NULL)){
		//Data begin é mais antiga que a de end:
		if(comparaDatasTotal(begin,end)<0){
			if(comparaDatas(ll->data,begin)>0){//ll->data é mais recente que begin
					begin=ll->data;
					int mes = get_month(begin);
					int ano = get_year(begin);
					begin = createDate(1,mes,ano);
					return questions_with_tag(com,tag,begin,end);
			}	

			while((ll!=NULL)&&(comparaDatas(ll->data,begin)<0)){
				ll = ll->next;
			}


			while((ll!=NULL)&&(comparaDatas(ll->data,end)<=0)){
				if(searchTag(ll->tags,tag)==1){
					perguntas=guardaPerguntasWithTag(ll->postPerguntas,tag,begin,end,perguntas);
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
					return questions_with_tag(com,tag,begin,end);
				}

				while((ll!=NULL)&&(comparaDatas(ll->data,end)<0))
					ll = ll->next;

				while((ll!=NULL)&&(comparaDatas(ll->data,begin)<=0)){
					if(searchTag(ll->tags,tag)==1)
						perguntas=guardaPerguntasWithTag(ll->postPerguntas,tag,begin,end,perguntas);
					ll = ll->next;
				}
			}

		}

		N = postAVLSize(perguntas);

		Post arrayP[N];

		int it=0;

		toArrayP(perguntas,arrayP,N,it);

		sortByDate(arrayP,N);

		it=0;
		
		list = create_list(N);

		while(it<N){
			if(arrayP[it]!=NULL)
				set_list(list,it,arrayP[it]->idPost);
			it++;
		}
	}
	return list;

}


// QUERY 5-------------------------------------------------

USER get_user_info(TAD_community com, long id){

	User UserAux;
	int i = 0;
	Post arrayP[10];

	while(i<10){
		arrayP[i] = NULL;
		i++;
	}


	LLista ll = com->dataPosts;

	while(ll->next != NULL){
		ll = ll->next;
	}

	long posts[10];
	i = 0;
	while(i<10){
		posts[i] = 0;
		i++;
	}

	i=0;
	while(arrayP[9] == NULL && ll != NULL){

		while(ll!= NULL){
			guardaPostID(ll->postPerguntas,arrayP,id);
			guardaPostID(ll->postRespostas,arrayP,id);

			ll = ll->prev;
		}
	}
	UserAux = get_User(com->users,id);

	sortByDate(arrayP,10);

	i=0;
	while(i<10 && arrayP[i] != NULL){

		posts[i]=arrayP[i]->idPost;

		i++;
	}

	USER user = create_user(UserAux->about,posts);

	return user;
}


// query 6

LONG_list most_voted_answers(TAD_community com, int N, Date begin, Date end){

	LLista ll = com->dataPosts;

	int i = 0;

	Post arrayP[N];
	while(i<N){
		arrayP[i]=NULL;
		i++;
	}

	LONG_list list = create_list(N);


	if((begin!=NULL)&&(end!=NULL)){
		//Data begin é mais antiga que a de end:
		if(comparaDatasTotal(begin,end)<0){
			if(comparaDatas(ll->data,begin)>0){//ll->data é mais recente que begin
					begin=ll->data;
					int mes = get_month(begin);
					int ano = get_year(begin);
					begin = createDate(1,mes,ano);
					return most_voted_answers(com,N,begin,end);
			}	

			while((ll!=NULL)&&(comparaDatas(ll->data,begin)<0)){
				ll = ll->next;
			}


			while((ll!=NULL)&&(comparaDatas(ll->data,end)<=0)){
				guardaRespostas(ll->postRespostas,begin,end,arrayP,N);
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
					return most_voted_answers(com,N,begin,end);
				}

				while((ll!=NULL)&&(comparaDatas(ll->data,end)<0))
					ll = ll->next;

				while((ll!=NULL)&&(comparaDatas(ll->data,begin)<=0)){
					guardaRespostas(ll->postRespostas,end,begin,arrayP,N);
					ll = ll->next;
				}
			}

		}
		i=0;

		sortByNvotes(arrayP,N);

		while(i<N){
			if(arrayP[i]!=NULL){
				set_list(list,i,arrayP[i]->idPost);
			}
			else
				set_list(list,i,0);
			i++;
		}
	}
	return list;
}

// query 7
LONG_list most_answered_questions(TAD_community com, int N, Date begin, Date end){

	LLista ll = com->dataPosts;

	int i = 0;

	Post arrayP[N];
	while(i<N){
		arrayP[i]=NULL;
		i++;
	}

	LONG_list list = create_list(N);


	if((begin!=NULL)&&(end!=NULL)){
		//Data begin é mais antiga que a de end:
		if(comparaDatasTotal(begin,end)<0){
			if(comparaDatas(ll->data,begin)>0){//ll->data é mais recente que begin
					begin=ll->data;
					int mes = get_month(begin);
					int ano = get_year(begin);
					begin = createDate(1,mes,ano);
					return most_answered_questions(com,N,begin,end);
			}	

			while((ll!=NULL)&&(comparaDatas(ll->data,begin)<0)){
				ll = ll->next;
			}


			while((ll!=NULL)&&(comparaDatas(ll->data,end)<=0)){
				guardaPerguntas(ll->postPerguntas,begin,end,arrayP,N);
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
					return most_answered_questions(com,N,begin,end);
				}

				while((ll!=NULL)&&(comparaDatas(ll->data,end)<0))
					ll = ll->next;

				while((ll!=NULL)&&(comparaDatas(ll->data,begin)<=0)){
					guardaPerguntas(ll->postPerguntas,end,begin,arrayP,N);
					ll = ll->next;
				}
			}

		}
		i=0;

		sortByNAnswers(arrayP,N);

		while(i<N){
			if(arrayP[i]!=NULL){
				printf("IDPOST: %ld NANSWERS: %d\n",arrayP[i]->idPost,arrayP[i]->answerCount);
				set_list(list,i,arrayP[i]->idPost);
			}
			else
				set_list(list,i,0);
			i++;
		}
	}
	return list;

}

// QUERY 8

LONG_list contains_word(TAD_community com, char* word, int N){

	LONG_list list = create_list(N);

	struct PostAVL* aux;
	LLista ll = com->dataPosts;

	int i = 0;

	Post arrayP[N];
	while(i<N){
		arrayP[i]=NULL;
		i++;
	}

	while(ll->next != NULL){
		ll = ll->next;
	}

	while(arrayP[N-1] == NULL && ll != NULL){

		while(ll!= NULL){

			aux = ll->postPerguntas;
			if(aux != NULL){
				postContains(aux,word,arrayP,N);

			}
			ll = ll->prev;
		}
	}


	sortByDate(arrayP,N);
	i=0;
	while(i<N){
		if(arrayP[i]!=NULL){
			printf("%ld\n", arrayP[i]->idPost);
			set_list(list,i,arrayP[i]->idPost);
		}
		else
			set_list(list,i,0);
		i++;
	}

	return list;
}




// query 9
LONG_list both_participated(TAD_community com, long id1, long id2, int N){


	LLista ll = com->dataPosts;

	LONG_list list = create_list(N);

	LAVL questions = (LAVL)malloc(sizeof(struct LongAVL));
	questions=NULL;

	Post arrayP[N];

	Post pfst = NULL;

	Post psnd = NULL;


	while(ll->next!=NULL){
		ll=ll->next;
	}

	LLista llanswer = ll;

	LLista cabeca = ll;

	int r1=0;

	int r2=0;

	int ri=0;

	int i = 0;

	while(i<N){
		set_list(list,i,0);
		arrayP[i] = NULL;
		i++;
	}
	i=0;

	while((ll!=NULL)&&(i<N)){

		r1 = searchLAVL(ll->usersPerguntas,ll->usersRespostas,id1);


		if(r1==1){

			//r1 é pergunta vamos procurar o post correspondente
			while((pfst = searchAVLPostsByOwner(ll->postPerguntas,id1,questions))!=NULL){
				questions = insertL(questions,pfst->idPost);
			//encontrou pergunta, logo vai procurar respostas de id2

			while((llanswer!=NULL)&&(psnd==NULL)){
				//printf("Procurando respostas %d\n", llanswer->usersRespostas->valor);
				if((searchLAVLAux(llanswer->usersRespostas,id2))==1){
					psnd = searchUserInAnswer(llanswer->postRespostas,pfst->idPost,id2,psnd);
				}


				llanswer = llanswer->prev;

			}

			llanswer = cabeca;
			if((pfst!=NULL)&&(psnd!=NULL)){
					arrayP[i++]=pfst;
					pfst=NULL;
					psnd=NULL;
				}
			}
		}

		else
			if(r1==2){
				//r2 é resposta
				while((pfst = searchAVLPostsByOwner(ll->postRespostas,id1,questions))!=NULL){

					questions = insertL(questions,pfst->parentId);


				while((llanswer!=NULL)&&(psnd==NULL)){
					//vamos ver se id2 existe nas arvores
					ri = searchLAVL(llanswer->usersPerguntas,llanswer->usersRespostas,id2);


						if(ri!=0){
							psnd = searchUserInQuestion(llanswer->postPerguntas,pfst->parentId,id2,psnd);
							if(psnd==NULL)
								psnd = searchUserInAnswer(llanswer->postRespostas,pfst->parentId,id2,psnd);
						}
					ri=0;
					llanswer = llanswer->prev;
				}

				llanswer = cabeca;
				if((pfst!=NULL)&&(psnd!=NULL)){
					arrayP[i++]=pfst;
					pfst=NULL;
					psnd=NULL;
				}
			
				}
			}


		r2 = searchLAVL(ll->usersPerguntas,ll->usersRespostas,id2);
		if(r2==1){

			while((pfst = searchAVLPostsByOwner(ll->postPerguntas,id2,questions))!=NULL){

				questions = insertL(questions,pfst->idPost);

				while((llanswer!=NULL)&&(psnd==NULL)){

					if(searchLAVLAux(llanswer->usersRespostas,id1)==1)

						psnd = searchUserInAnswer(llanswer->postRespostas,pfst->idPost,id1,psnd);

					llanswer = llanswer->prev;
				}

				llanswer = cabeca;
				if((pfst!=NULL)&&(psnd!=NULL)){
					arrayP[i++]=pfst;
					pfst=NULL;
					psnd=NULL;
				}
			
				}
		}

		else
			if(r2==2){

				while((pfst = searchAVLPostsByOwner(ll->postRespostas,id2,questions))!=NULL){

					questions = insertL(questions,pfst->parentId);

					while((llanswer!=NULL)&&(psnd==NULL)){

						ri = searchLAVL(llanswer->usersPerguntas,llanswer->usersRespostas,id1);

						if(ri!=0){
							psnd = searchUserInQuestion(llanswer->postPerguntas,pfst->parentId,id1,psnd);
							if(psnd==NULL){
								psnd = searchUserInAnswer(llanswer->postRespostas,pfst->parentId,id1,psnd);
							}
						}

						ri=0;

						llanswer = llanswer->prev;
					}

					llanswer = cabeca;

					if((pfst!=NULL)&&(psnd!=NULL)){
						arrayP[i++]=pfst;
						pfst=NULL;
						psnd=NULL;
					}

				}

			}
		r1=0;
		r2=0;
			
		ll = ll->prev;
		
	}

	ordenaByQuestion(com,arrayP,N);

	i=0;

	while(i<N){
		if(arrayP[i]!=NULL)
			set_list(list,i,arrayP[i]->idPost);
		i++;
	}

	return list;
}
// query 10

long better_answer(TAD_community com, long id){

	long solucao = -1;

	Post resposta = answerLookUp(com->respostasByParent,id);

	if(resposta!=NULL)
		solucao = resposta->idPost;

	return solucao;

	
}
// query 11

/*

Resultado: 6,29,72,163,193
Resultado2: 140,5629,72,103,201,277,431,9,80,133

*/

LONG_list most_used_best_rep(TAD_community com, int N, Date begin, Date end){
	LLista ll = com->dataPosts;

	TAVL tags = (TAVL)malloc(sizeof(struct TagAVL));
	tags=NULL;

	LAVL idsFound = (LAVL)malloc(sizeof(struct LongAVL));
	idsFound = NULL;

	Post p=NULL;

	Tag tx;

	LLista cabeca = ll;

	int i = 0;

	UAVL ua = bestRep(com,N,begin,end);
	int tam1 = contaU(ua);

	User arrayU[tam1];
	avlToArrayU(ua,arrayU,i);
	sortByCountRep(arrayU,tam1);

	i=0;
	Tag arrayT[N];
	while(i<N){
		arrayT[i]=NULL;
		i++;
	}
	int c =0;
	char* tagCopia;

	LONG_list list = create_list(N);


	if((begin!=NULL)&&(end!=NULL)){
		//Data begin é mais antiga que a de end:
		if(comparaDatasTotal(begin,end)<0){
			if(comparaDatas(ll->data,begin)>0){//ll->data é mais recente que begin
					begin=ll->data;
					int mes = get_month(begin);
					int ano = get_year(begin);
					begin = createDate(1,mes,ano);
					return most_used_best_rep(com,N,begin,end);
			}	

			while((ll!=NULL)&&(comparaDatas(ll->data,begin)<0)){
				ll = ll->next;
			}


			while((ll!=NULL)&&(comparaDatas(ll->data,end)<=0)){
				i=0;
				c++;
				while(i<N){
					if((searchLAVLAux(ll->usersPerguntas,arrayU[i]->idUser))==1){
						//printf("Encontrou user1 %ld REp: %ld\n", arrayU[i]->idUser, arrayU[i]->reputation);
						//existe na avlt
						while((p = searchAVLPostsSR(ll->postPerguntas,arrayU[i]->idUser,idsFound))!=NULL){
							if((comparaDatasTotal(p->creationDate,begin)>=0)&&(comparaDatasTotal(p->creationDate,end)<=0)){
								idsFound = insertL(idsFound,p->idPost);
								tagCopia = (char*)malloc(strlen(p->tags)+1);
								strcpy(tagCopia,p->tags);

								tagCopia = strtok(tagCopia,"<>");

							    while(tagCopia != NULL){
							    	    tx = searchTagId(com->tags,tagCopia);
							    	    tags = insertT(tags,tx,tx->idTag);
							        	
							        	tagCopia = strtok(NULL,"<>");
							    }
							    free(tagCopia);
							}

						}

					}
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
					return most_used_best_rep(com,N,begin,end);
				}

				while((ll!=NULL)&&(comparaDatas(ll->data,end)<0))
					ll = ll->next;

				while((ll!=NULL)&&(comparaDatas(ll->data,begin)<=0)){
					while(i<N){
					if((searchLAVLAux(ll->usersPerguntas,arrayU[i]->idUser))==1){
						//printf("Encontrou user1 %ld REp: %ld\n", arrayU[i]->idUser, arrayU[i]->reputation);
						//existe na avlt
						while((p = searchAVLPostsSR(ll->postPerguntas,arrayU[i]->idUser,idsFound))!=NULL){
							idsFound = insertL(idsFound,p->idPost);
							tagCopia = (char*)malloc(strlen(p->tags)+1);
							strcpy(tagCopia,p->tags);

							tagCopia = strtok(tagCopia,"<>");

						    while(tagCopia != NULL){
						    	    tx = searchTagId(com->tags,tagCopia);

						    	    tags = insertT(tags,tx,tx->idTag);
						        	
						        	tagCopia = strtok(NULL,"<>");
						    }
						    free(tagCopia);

						}

					}
					i++;
					ll = cabeca;
				}
				i=0;
				ll = ll->next;
				cabeca=ll;
				}
			}

		}

	}
	i=0;
	avlToArrayT(tags,arrayT,i);

	int tam=contaT(tags);

	sortByCountTag(arrayT,tam);

	i=0;
	printf("%d\n", c);
	while(i<N){
		if(arrayT[i]!=NULL)
			set_list(list,i,arrayT[i]->idTag);
		i++;
	}

	return list;

}

//CLEAN

void freeUser(User u){
	free(u->about);
	free(u->name);
	free(u);
}

void freeUsers(UAVL users){
	//printf("FREEUSERS\n");
	if(users!=NULL){
		freeUsers(users->esq);
		freeUsers(users->dir);
		freeUser(users->users);
		free(users);
	}
}

void freeLAVL(LAVL users){
	//printf("FREEUSERS\n");
	if(users!=NULL){
		freeLAVL(users->esq);
		freeLAVL(users->dir);
		free(users);
	}
}

void freeTagAVL(TAVL tags){
	if(tags!=NULL){
		freeTagAVL(tags->esq);
		freeTagAVL(tags->dir);
		free(tags);
	}
}

void freePost(Post p){
	free_date(p->creationDate);
	free(p->title);
	free(p->tags);
	free(p);
}

void freePosts(AVLT posts){
	//printf("FREEPosts\n");
	if(posts!=NULL){
		freePosts(posts->esq);
		freePosts(posts->dir);
		freePost(posts->post);
		free(posts);
	}
}

void freeAVLT(AVLT posts){
	//printf("FREEPostsAVLT\n");
	if(posts!=NULL){
		freeAVLT(posts->esq);
		freeAVLT(posts->dir);
		free(posts);
	}
}

void freeVotes(VAVL votes){
	//printf("FREEVotes\n");
	if(votes!=NULL){
		freeVotes(votes->esq);
		freeVotes(votes->dir);
		free(votes);
	}
}

void freeTag(Tag t){
	//printf("FREETag\n");
	if((t!=NULL)&&(t->name!=NULL))
		free(t->name);
	//free(t);
}

void freeTAGLL(TagLL tags){
	//printf("FREETagLL\n");
	TagLL tmp;
	while(tags!=NULL){
		tmp=tags;
		tags=tags->next;
		//freeTag(tmp->tag);
		free(tmp);
	}
}

void freeLLista(LLista ll){
	LLista tmp;
	while(ll!=NULL){
		tmp=ll;
		ll=ll->next;
		free_date(tmp->data);
		freeAVLT(tmp->postPerguntas);
		freeAVLT(tmp->postRespostas);
		freeLAVL(tmp->usersPerguntas);
		freeLAVL(tmp->usersRespostas);
		freeTAGLL(tmp->tags);
		free(tmp);
	}
}

TAD_community clean(TAD_community com){
	freeUsers(com->users);
	freePosts(com->respostasByParent);
	freeVotes(com->votes);
	freeLLista(com->dataPosts);
	freeTagAVL(com->tags);
	return com;
}