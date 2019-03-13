#include "../include/parser.h"


/*	parserUsers
 *
 *	Para uma dada 'row' do ficheiro 'Users.xml',
 *	guarda o contudo de cada atributo num novo 'User',
 *	recorrendo à funçao xmlGetProp de biblioteca 'libxml'
 */
User parserUsers(xmlDocPtr doc,xmlNodePtr cur){

	xmlChar *key;
	User novoUser = (User)malloc(sizeof(struct User));
	novoUser->name = mystrdup((const char*)"NULL");
	novoUser->about = mystrdup((const char*)"NULL");


		if((key = (xmlGetProp(cur, (const xmlChar *)"Id")))){

		    novoUser->idUser = atol((char*)key);
		    xmlFree(key);
		}
		if((key = xmlGetProp(cur,(const xmlChar *) "DisplayName"))){

		    novoUser->name = mystrdup((const char*)key);
		    xmlFree(key);
		}

		if((key = xmlGetProp(cur,(const xmlChar *) "AboutMe"))){

		    novoUser->about = mystrdup((const char*)key);
		    xmlFree(key);
		}

		if((key = xmlGetProp(cur,(const xmlChar *) "Reputation"))){

		    novoUser->reputation = atol((char*)key);
		    xmlFree(key);
		}

		novoUser->countPosts=0;
		
		return novoUser;
	}

/*	parserPosts
 *
 *	Para uma dada 'row' do ficheiro 'Posts.xml',
 *	guarda o contudo de cada atributo num novo 'Post',
 *	recorrendo à funçao xmlGetProp de biblioteca 'libxml'
 */
Post parserPosts(xmlDocPtr doc,xmlNodePtr cur){
		xmlChar *key;
		Post novoPost = (struct Post*)malloc(sizeof(struct Post));
		novoPost->title = mystrdup((const char*)"NULL");
		novoPost->tags = mystrdup((const char*)"NULL");

		if((key = xmlGetProp(cur, (const xmlChar *)"Id"))){

			novoPost->idPost = atol((char*)key);
		    xmlFree(key);
		}
		if((key = xmlGetProp(cur,(const xmlChar *) "PostTypeId"))){

		    novoPost->postType = atoi((char*)key);
		    xmlFree(key);
		}

		if((key = xmlGetProp(cur,(const xmlChar *) "CreationDate"))){

		    novoPost->creationDate = malloc(strlen((char*)key)+1);
		    int ano, mes, dia;
		    char* str = strtok((char*)key,"T");
		    char* out;
		    out = strtok(str,"-");
		    ano=atoi(out);
		    out=strtok(NULL,"-");
		    mes=atoi(out);
		    out=strtok(NULL,"-");
		    dia=atoi(out);
		    Date d = createDate(dia,mes,ano);
		    novoPost->creationDate=d;
		    xmlFree(key);
		}

		if((key = xmlGetProp(cur,(const xmlChar *) "Title"))){
		    novoPost->title = mystrdup((const char*)key);
		    xmlFree(key);
		}

		if((key = xmlGetProp(cur,(const xmlChar *) "ParentId"))){
		    novoPost->parentId = atol((char*)key);
		    xmlFree(key);
		}

		if((key = xmlGetProp(cur,(const xmlChar *) "Tags"))){
		   	novoPost->tags = mystrdup((const char*)key);
		    xmlFree(key);
		}

		if((key = xmlGetProp(cur, (const xmlChar *)"OwnerUserId"))){
			novoPost->owner = atol((char*)key);
		    xmlFree(key);
		}

		if((key = xmlGetProp(cur, (const xmlChar *)"Score"))){
			novoPost->score = atol((char*)key);
		    xmlFree(key);
		}

		if((key = xmlGetProp(cur, (const xmlChar *)"CommentCount"))){
			novoPost->comentCount = atoi((char*)key);
		    xmlFree(key);
		}

		if((key = xmlGetProp(cur, (const xmlChar *)"AnswerCount"))){
			novoPost->answerCount = atoi((char*)key);
		    xmlFree(key);
		}

		novoPost->nVotes = 0;
	
		return novoPost;

}

/*	parserTags
 *
 *	Para uma dada 'row' do ficheiro 'Tags.xml',
 *	guarda o contudo de cada atributo numa nova 'Tag',
 *	recorrendo à funçao xmlGetProp de biblioteca 'libxml'
 */
Tag parserTags(xmlDocPtr doc,xmlNodePtr cur){
		xmlChar *key;
		Tag novaTag = (struct Tag*)malloc(sizeof(struct Tag));

		if((key = xmlGetProp(cur, (const xmlChar *)"Id"))){
			novaTag->idTag = atol((char*)key);
		   	xmlFree(key);
		}

		if((key = xmlGetProp(cur, (const xmlChar *)"TagName"))){
			novaTag->name = mystrdup((const char*)key);
		    xmlFree(key);
		}
		
		novaTag->count = 0;
		

		return novaTag;
}

/*	parsing
 *
 *	Funçao que recebe como argumento a estrutura principal e o caminho do ficheiro xml a processar
 *	Guarda nas devidas estruturas o conteúdo necessário, invocando as funçoes d eparsing auxiliares
 *	descritas em cima.
 *	Para além disso:
 *		-> É contabilizado o numero de votos para cada user,
 *		-> Adicionado o numeore de votos para cada post
 */
TAD_community parsing(TAD_community com, char* dump_path){

	xmlDocPtr doc;
	xmlNodePtr cur;

	int encontrou = 0;

	doc = xmlParseFile(dump_path);

	User newUser;
	Post newPost;
	Tag newTag;

	if (doc == NULL ) {
		fprintf(stderr,"Document not parsed successfully. \n");
		return 0;
	}
	
	cur = xmlDocGetRootElement(doc);
	
	if (cur == NULL) {
		fprintf(stderr,"empty document\n");
		xmlFreeDoc(doc);
		return 0;
	}

	if((!encontrou)&&(!xmlStrcmp(cur->name,(const xmlChar *) "users"))) {
		cur = cur-> xmlChildrenNode;
		while(cur!=NULL){
			if((!xmlStrcmp(cur->name,(const xmlChar *) "row"))){
	            encontrou = 1;
				newUser=parserUsers(doc,cur);
		
				com->users = insert(com->users,newUser);

				com->usersNumber++;

			}
			cur = cur ->next;
		}

	}

	if((!encontrou)&&(!xmlStrcmp(cur->name,(const xmlChar *) "votes"))) {
		cur = cur-> xmlChildrenNode;

		while(cur!=NULL){
			if((!xmlStrcmp(cur->name,(const xmlChar *) "row"))){
				encontrou=1;
				xmlChar *key;

				Vote newVote = (Vote)malloc(sizeof(struct Vote));

				long postId;
				int nVoto;
				if((key = xmlGetProp(cur, (const xmlChar *)"PostId"))){
					postId= atol((char*)key);
		   			xmlFree(key);
				}
				if((key = xmlGetProp(cur, (const xmlChar *)"VoteTypeId"))){
					nVoto= atoi((char*)key);
		   			xmlFree(key);
				}

				newVote->postId = postId;
				newVote->voteType = nVoto;

				com->votes = insertV(com->votes,newVote);
	            
			}
			cur = cur ->next; 
		}
	}

	if((!encontrou)&&(!xmlStrcmp(cur->name,(const xmlChar *) "posts"))) {
		cur = cur-> xmlChildrenNode;

		while(cur!=NULL){
			if((!xmlStrcmp(cur->name,(const xmlChar *) "row"))){
	            encontrou = 1;
				newPost=parserPosts(doc,cur);

				if(newPost->postType!=2)
					newPost->parentId = 0;
				if(newPost->postType!=1)
					newPost->answerCount = 0;

				newPost->nVotes = searchVotes(newPost->idPost,com->votes);

				//incrementa posts do user
				incPosts(com->users,newPost);

				//insere posts na avl by data
				com->dataPosts = insertAtDate(com->dataPosts,newPost->creationDate,newPost,com->dataPosts->tags);

				if(newPost->postType==2)
					com->respostasByParent = insertParent(com->respostasByParent,newPost,newPost->parentId,com->users);
				}

			cur = cur ->next; 
		}
	}


	if((!encontrou)&&(!xmlStrcmp(cur->name,(const xmlChar *) "tags"))) {
		cur = cur-> xmlChildrenNode;

		while(cur!=NULL){
			if((!xmlStrcmp(cur->name,(const xmlChar *) "row"))){
	            encontrou = 1;

				newTag = parserTags(doc,cur);

				com->tags = insertT(com->tags,newTag,newTag->idTag);
			}
			cur = cur ->next;
		}
	}

    if(!encontrou){

		fprintf(stderr,"document of the wrong type, root node != xml\n");
        fprintf(stderr,"Nao foi possivel fazer parser do documento -> %s\n",dump_path);
		xmlFreeDoc(doc);
		return 0;
	}
	else {
		printf("\nParser efetuado com sucesso -> %s\n",dump_path);
	}
	
	xmlFreeDoc(doc);


	while(com->dataPosts->prev!=NULL){
		com->dataPosts=com->dataPosts->prev;
	}

	return com;

}