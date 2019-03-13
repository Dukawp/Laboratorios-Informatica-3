#include "../include/auxiliares.h"

//Funcoes auxiliares usadas no parsing

/*	incPosts
 *
 *Na avl dos users incrementa o numero de posts
 */
void incPosts(UAVL ua, Post p){
	UAVL auxUser = ua;
	if(auxUser!=NULL){
	if(auxUser->users->idUser==p->owner){
		auxUser->users->countPosts++;
	}
	else{
		if(auxUser->users->idUser>p->owner){
			incPosts(auxUser->esq,p);
		}
		else
			incPosts(auxUser->dir,p);
	}
	}
}

/*	adicionaVotos
 *
 *	Na avl de posts incrementa ou decrementa 
 *	o numero de votos conforme o seu tipo
 */
int adicionaVotos(AVLT p, Vote newVote){
	AVLT pa = p;
	long id = newVote->postId;
	int voto = newVote->voteType;

	int r=0;
	if(pa!=NULL){
		if(pa->post->idPost==id){
			if(voto==2){
				pa->post->nVotes +=1;
			}
			else
				if(voto==3){
					pa->post->nVotes -=1;
				}
		r=1;
		}
		else
			if((pa->post->idPost>id))
				adicionaVotos(pa->esq,newVote);
			else
				adicionaVotos(pa->dir,newVote);
	}

	return r;

}

/*	searchVotes
 *	
 *	Procura cassificacao de post na avl de votos
 */
int searchVotes(long p, VAVL va){
	VAVL v = va;
	if(v!=NULL){
		if(p==v->postId){
			return v->classPost;
		}
		else
			if(p>v->postId)
				return searchVotes(p,va->dir);
			else
				return searchVotes(p,va->esq);
	}
	return 0;
}
