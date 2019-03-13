#include <stdio.h>
#include "../include/interface.h" 

int main(int argc, char **argv){

	char* docname;


	if(argc<=1){
		printf("Usage: %s docname\n",argv[0]);
	}

	TAD_community t = init();

	docname = argv[1];
	t=load(t,docname);

	LONG_list top;
	Date d1, d2;
	
	int d=0;

	
	//QUERY 1
	printf("\n**QUERIE1**\n");
	STR_pair pair = info_from_post(t, 801049);
	STR_pair pair2 = info_from_post(t,796430);
	if(pair==NULL) printf("PORRA NULA\n");

	printf("Titulo: %s\n", get_fst_str(pair));

	printf("Nome: %s\n", get_snd_str(pair));

	printf("Titulo2: %s\n", get_fst_str(pair2));

	printf("Nome2: %s\n", get_snd_str(pair2));
	

	//QUERIE 2
	printf("\n**QUERIE2**\n");

	top = top_most_active(t,10);

	while(d<10){
		printf("IDUSER: %ld\n", get_list(top,d));
		d++;
	}

	printf("\nRESEULTADO 2:\n");

	top = top_most_active(t,100);

	d=0;
	while(d<100){
		printf("IDUSER: %ld\n", get_list(top,d));
		d++;
	}


	//QUERY 3

	printf("\n**QUERIE3**\n");
	d1=createDate(01,07,2016);
	d2=createDate(31,07,2016);

	LONG_pair pair3 =  total_posts(t, d1, d2);

	d1=createDate(01,01,2014);
	d2=createDate(31,12,2014);

	LONG_pair pair4 =  total_posts(t, d1, d2);

	printf("Perguntas: %ld\n", get_fst_long(pair3));

	printf("Respostas: %ld\n", get_snd_long(pair3));

	printf("Perguntas2: %ld\n", get_fst_long(pair4));

	printf("Respostas2: %ld\n", get_snd_long(pair4));	


	//QUERIE 4
	printf("\n**QUERIE4**\n");
	LONG_list top2;
	d1=createDate(01,03,2013);
	d2=createDate(31,03,2013);
	top2 = questions_with_tag(t, "package-management", d1, d2);
	d=0;
	while(d<54){
		printf("idPost: %ld\n", get_list(top2,d));
		d++;
	}

	printf("\nRESEULTADO2:\n");
	d1=createDate(01,01,2014);
	d2=createDate(31,01,2014);
	top2 = questions_with_tag(t, "nautilus", d1, d2);
	d=0;
	while(d<23){
		printf("idPost: %ld\n", get_list(top2,d));
		d++;
	}
	
	//QUERIE 5
	printf("\n**QUERIE5**\n");
	USER u;
	u = get_user_info(t,15811);
	printf("ABOUT: %s\n", get_bio(u));
	long* l = get_10_latest_posts(u);
	d=0;
	while(d<10){
		printf("IDPOST: %ld\n", l[d]);
		d++;
	}

	d=0;
	printf("\nNOVO\n");
	u = get_user_info(t,449);
	printf("ABOUT: %s\n", get_bio(u));
	l = get_10_latest_posts(u);
	while(d<10){
		printf("IDPOST: %ld\n", l[d]);
		d++;
	}

	//QUERIE 6
	printf("\n**QUERIE6**\n");
	d1=createDate(01,11,2015);
	d2=createDate(30,11,2015);
	int N=5;

	top2 = most_voted_answers(t,N,d1 ,d2);

	d=0;
	while(d<N){
		printf("idPost: %ld\n", get_list(top2,d));
		d++;
	}

	printf("\nResposta2:\n");

	d1=createDate(01,05,2013);
	d2=createDate(06,05,2013);
	N=50;

	top2 = most_voted_answers(t,N,d1 ,d2);

	d=0;
	while(d<N){
		printf("idPost: %ld\n", get_list(top2,d));
		d++;
	}
	

	//QUERIE 7
	printf("\n**QUERIE7**\n");
	d1=createDate(01,8,2014);
	d2=createDate(11,8,2014);
	N=10;

	top2 = most_answered_questions(t,N,d1 ,d2);
	
	d=0;
	while(d<N){
		printf("idPost: %ld\n", get_list(top2,d));
		d++;
	}

	printf("\nResposta2:\n");

	d1=createDate(01,01,2012);
	d2=createDate(31,12,2012);
	N=100;

	top2 = most_answered_questions(t,N,d1 ,d2);

	d=0;
	while(d<N){
		printf("idPost: %ld\n", get_list(top2,d));
		d++;
	}

	//QUERIE8
	
	printf("\n**QUERIE8**\n");
	top2 = contains_word(t,"kde",10);

	d=0;
	while(d<10){
		printf("idPost: %ld\n", get_list(top2,d));
		d++;
	}

	printf("\nResposta2\n");

	top2 = contains_word(t,"glib",10);

	d=0;
	while(d<10){
		printf("idPost: %ld\n", get_list(top2,d));
		d++;
	}


	//QUERIE 9

	printf("\n**QUERIE9**\n");

	top = both_participated(t,87,5691,10);
	top2 = both_participated(t,253,455,5);

	d=0;
	while(d<10){
		printf("idPost: %ld\n", get_list(top,d));
		d++;
	}

	d=0;
	while(d<5){
		printf("idPost: %ld\n", get_list(top2,d));
		d++;
	}
	
	//QUERIE 10
	printf("\n**QUERIE410**\n");
	long l2, l1;

	l2 = better_answer(t, 30334);
	l1 = better_answer(t, 5942);
	printf("RespostaID: %ld\n", l2);
	printf("RespostaID1: %ld\n", l1);
	

	//QUERIE 11
	
	d1=createDate(01,11,2013);
	d2=createDate(30,11,2013);
	N=5;

	top2 = most_used_best_rep(t,N,d1 ,d2);
	
	d=0;
	while(d<N){
		printf("idTag: %ld\n", get_list(top2,d));
		d++;
	}

	d1=createDate(01,01,2014);
	d2=createDate(31,12,2014);
	N=10;

	top2 = most_used_best_rep(t,N,d1 ,d2);
	
	d=0;
	while(d<N){
		printf("idPost: %ld\n", get_list(top2,d));
		d++;
	}


	t = clean(t);


	return 0;
}