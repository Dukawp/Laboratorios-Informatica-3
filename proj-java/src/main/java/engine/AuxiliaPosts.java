package engine;

import java.time.LocalDate;
import java.util.*;
import common.Pair;

public class AuxiliaPosts{

	public AuxiliaPosts(){
	}

	 /**
     * Query 1 Auxiliar  - Procura dados de um Post
     * @param struct class das estruturas
     * @param users Hashmap com todos os Users
     * @param p Post
     * @param id long que contem o identificador do Post
     * @return TreeMap com CountPosts como key e uma ArrayList de Users como value
     **/
	public Pair procuraPergunta(Struct struct,Map<Long,User> users,Post p,Long id){
        TreeMap<LocalDate,HashMap<Long,Post>> postsP = struct.getPostsPerguntas();
		for(Map.Entry<LocalDate,HashMap<Long,Post>> entry : postsP.entrySet()){
            if((p=(entry.getValue().get(id)))!=null){
                long ownerId = p.getOwnerId();
                String nameOwner = users.get(ownerId).getName();
                String titulo = p.getTitle();
                return new Pair<>(titulo,nameOwner);
            }
        }
        return null;
	}
	/**
     * Query 1 Auxiliar  - Procura o ParentId de uma resposta
	 * @param struct class das estruturas
     * @param users Hashmap com todos os Users
     * @param p Post
     * @param id long que contem o identificador do Post     
     * @return ID da pergunta (ParentID)
     **/
	public long procuraResposta(Struct struct,Map<Long,User> users,Post p,Long id){
		TreeMap<LocalDate,HashMap<Long,Post>> postsR = struct.getPostsRespostas();
		for(Map.Entry<LocalDate,HashMap<Long,Post>> entry : postsR.entrySet()){
	        if((p=(entry.getValue().get(id)))!=null){
	            long idParent = p.getParentId();
	                return idParent;
	        }
	    }
	    return 0;
	}

	/**
     * Query 3 Auxiliar  - Conta numero de Posts 
	 * @param nm NavigableMap obtido atraves do SubMap(intervalo entre begin e end)
     * @param begin Data inicial
     * @param end Data final
     * @param np contador     
     * @return contador de posts
     **/
	public long incrementaPosts(NavigableMap<LocalDate,HashMap<Long,Post>> nm,LocalDate begin, LocalDate end,long np){
		LocalDate cd;
	        for(Map.Entry<LocalDate,HashMap<Long,Post>> map : nm.entrySet()) {
	            for(Map.Entry<Long,Post> posts : map.getValue().entrySet()){
	                cd = posts.getValue().getCreationDate();
	                if((begin.isBefore(cd))||(begin.isEqual(cd))){
	                    if(end.isAfter(cd)||end.isEqual(cd)){
	                        np++;
	                    }
	                }
	            }
	        }
	        return np;
	}

	/**
     * Query 4 Auxiliar  - Guarda Perguntas com determinada Tag 
	 * @param nm NavigableMap obtido atraves do SubMap(intervalo entre begin e end)
     * @param begin Data inicial
     * @param end Data final
     * @param tag tag procurada   
     * @return TreeMap com key LocalDate e value um ArrayList de Posts 
     **/
	public TreeMap<LocalDate,ArrayList<Post>> guardaPerguntasTag(NavigableMap<LocalDate,HashMap<Long,Post>> nm, LocalDate begin, LocalDate end, String tag){

		TreeMap<LocalDate,ArrayList<Post>> auxpost;
        auxpost = new TreeMap<>();
		
		for(HashMap<Long,Post> posts : nm.values()){
            for(Post p : posts.values()){
                if((begin.isBefore(p.getCreationDate()) || begin.isEqual(p.getCreationDate()))
                    &&(end.isAfter(p.getCreationDate())||end.isEqual(p.getCreationDate()))){

                        String tags = p.getTags();

                            if(tags.toLowerCase().contains(tag.toLowerCase())){
                                ArrayList<Post> listapp = auxpost.get(p.getCreationDate());
                                   
                                if(listapp==null)
                                    listapp = new ArrayList<>();
                                listapp.add(p);
                                auxpost.put(p.getCreationDate(), listapp);
                            }
                    }
                }
            }

            return auxpost;
	}

	/**
     * Query 4 Auxiliar  - Passar da Tree para uma lista os IDs de cada Post  
	 * @param auxpost Treemap com key LocalDate e value um ArrayList de Posts 
     * @param lista Lista que vai conter os IDs dos Posts
     * @return Lista com os IDs dos Posts que se encontravam no ArrayList do auxpost 
     **/
	public List<Long> treeToList(TreeMap<LocalDate,ArrayList<Post>> auxpost, List<Long> lista){
        for(Map.Entry<LocalDate,ArrayList<Post>> listaPost : auxpost.entrySet()) {
            
            for(Post p : listaPost.getValue()){       
                lista.add(p.getIdPost());
            }

        }
        return lista;
    }

	/**
     * Query 5 Auxiliar  - Guarda Posts ordenados por data   
     * @param struct class das estruturas
	 * @param auxpost Treemap com key LocalDate e value um ArrayList de Posts 
     * @param id Identificado do User
     * @return TreeMap com key LocalDate e value um ArrayList de Posts 
     **/
    public TreeMap<LocalDate,ArrayList<Post>> guardaPostsPorData(Struct struct, TreeMap<LocalDate,ArrayList<Post>> auxpost, long id){

	    for(Map.Entry<LocalDate,HashMap<Long,Post>> respostas : struct.getPostsRespostas().descendingMap().entrySet()) {
	                    
	            for(Map.Entry<Long,Post> posts : respostas.getValue().entrySet()){
	                if(posts.getValue().getOwnerId() == id){
	                    ArrayList<Post> listapp = auxpost.get(posts.getValue().getCreationDate());
	                       
	                    if(listapp==null)
	                        listapp = new ArrayList<>();

	                    listapp.add(posts.getValue());
	                    if(auxpost.size()<10){
	                        auxpost.put(posts.getValue().getCreationDate(), listapp);
	                    }
	                }
	            }

	        }
	        return auxpost;
	}

	/**
     * Query 6 Auxiliar  - Guardar Posts ordenados pelo numero de votos   
	 * @param nm NavigableMap obtido atraves do SubMap(intervalo entre begin e end)
	 * @param begin Data inicial
     * @param end Data final
     * @return TreeMap com key numero votos e value um ArrayList de Posts 
     **/
	public Map<Integer,ArrayList<Post>> postsByNVotes(NavigableMap<LocalDate,HashMap<Long,Post>> nm,LocalDate begin, LocalDate end){
		Map<Integer,ArrayList<Post>> tree = new TreeMap<>(Collections.reverseOrder());
	        for(HashMap<Long,Post> hs : nm.values()){
	            for(Post p : hs.values()){
	                if((begin.isBefore(p.getCreationDate()) || begin.isEqual(p.getCreationDate()))
	                    &&(end.isAfter(p.getCreationDate())||end.isEqual(p.getCreationDate()))){
	                    ArrayList<Post> ps = tree.get(p.getnVotes());
	                    if(ps==null)
	                        ps = new ArrayList<Post>();
	                    ps.add(p);
	                    tree.put(p.getnVotes(),ps);
	                }
	            }
	        }
	        return tree;
	}

	/**
     * Query 6 Auxiliar  - Guarda os N primeiros IDs num ArrayList 
     * @param N numero desejado de IDs
     * @param tree Treemap com key numero votos e value um ArrayList de Posts 
     * @return Lista com os N IDs 
     **/
	public List<Long> obtemTopNOfTree(int N, Map<Integer,ArrayList<Post>> tree){
		List<Long> list = new ArrayList<>();
	        int i=0;
	        for(ArrayList<Post> tr : tree.values()){
	            for(Post pt : tr){
	                if(list.contains(pt.getIdPost())==false){
	                    list.add(pt.getIdPost());
	                    i++;
	                }
	                if(i==N) break;
	            }
	            if(i==N) break;
	        }

	        return list;
	}


	/**
     * Query 7 Auxiliar  - Guardar Posts ordenados pelo AnswerCount   
	 * @param nm NavigableMap obtido atraves do SubMap(intervalo entre begin e end)
	 * @param begin Data inicial
     * @param end Data final
     * @return TreeMap com key answerCount e value um ArrayList de Posts 
     **/
	public Map<Integer,ArrayList<Post>> postsByAnswer(NavigableMap<LocalDate,HashMap<Long,Post>> nm,LocalDate begin, LocalDate end){
		Map<Integer,ArrayList<Post>> tree = new TreeMap<>(Collections.reverseOrder());
        for(HashMap<Long,Post> hs : nm.values()){
            for(Post p : hs.values()){
                if((begin.isBefore(p.getCreationDate()) || begin.isEqual(p.getCreationDate()))
                    &&(end.isAfter(p.getCreationDate())||end.isEqual(p.getCreationDate()))){
                    ArrayList<Post> ps = tree.get(p.getAnswerCount());
                    if(ps==null)
                        ps = new ArrayList<Post>();
                    ps.add(p);
                    tree.put(p.getAnswerCount(),ps);
                }
            }
        }
        return tree;
	}

	/**
     * Query 8 Auxiliar  - Guarda perguntas que contenham a palavra no title  
     * @param N numero desejado de IDs
     * @param word Palavra a procurar
	 * @param struct class das estruturas
     * @return Lista de Posts
     **/
	public List<Post> perguntasWithWord(int N, String word, Struct struct){
		Map<LocalDate,ArrayList<Post>> treeAux = new TreeMap<>();
		List <Post> listP = new ArrayList<>();
	        int it=0;
	        for(Map.Entry<LocalDate,HashMap<Long,Post>> perguntas : struct.getPostsPerguntas().descendingMap().entrySet()){
	              for(Post pm : perguntas.getValue().values()){
	                if(pm.getTitle()!=null)
	                    if(pm.getTitle().contains(word)){
	                        ArrayList<Post> p;
	                        if(treeAux.get(pm.getCreationDate())==null)
	                            p = new ArrayList<Post>();
	                        else
	                            p = treeAux.get(pm.getCreationDate());

	                        p.add(pm);
	                        treeAux.put(pm.getCreationDate(),p);
	                        it++;
	                    }
	            }
	            if(it==N){
	             break;
	            }
	        }
	        for(Map.Entry<LocalDate,ArrayList<Post>> tr : treeAux.entrySet()){
	            for(Post p : tr.getValue()){
	                listP.add(p);
	            }
	        }
	        return listP;
	}

	/**
     * Query 8 Auxiliar  - Guarda perguntas que contenham a palavra no title  
     * @param N numero para limitir tamanho da lista
     * @param listP  lista com os Posts
     * @param word Palavra a procurar
     * @return Lista de IDs 
     **/
	public List<Long> obtemTopNWords(int N,List<Post> listP, String word){
		List <Long> list = new ArrayList<>();
		int i=0;
        Collections.reverse(listP);
        for(Post p : listP){
            if(p.getTitle().contains(word)){
                list.add(p.getIdPost());
            }
            i++;
            if(i==N)break;
        }
        return list;
	}



	/**
     * Query 9 Auxiliar  - Guarda perguntas se pertencerem ao id1 ou ao id2
	 * @param struct class das estruturas
     * @param auxpostP Treemap com key Id e value um ArrayList de Posts 
     * @param id1 Identificador do primeiro User
     * @param id2 Identificador do segundor User
     * @return Treemap com key Id e value um ArrayList de Posts
     **/
	public TreeMap<Long,ArrayList<Post>> guardaPerguntasOwnerID(Struct struct, TreeMap<Long,ArrayList<Post>> auxpostP, long id1, long id2){


		for(Map.Entry<LocalDate,HashMap<Long,Post>> perguntas : struct.getPostsPerguntas().descendingMap().entrySet()) {
                    
            for(Map.Entry<Long,Post> posts : perguntas.getValue().entrySet()){
                if(posts.getValue().getOwnerId() == id1){

                    ArrayList<Post> listapp = auxpostP.get(id1);
                       
                    if(listapp==null)
                        listapp = new ArrayList<>();

                    listapp.add(posts.getValue());

                    auxpostP.put(id1,listapp);
                    
                }
                if(posts.getValue().getOwnerId() == id2){
                    ArrayList<Post> listapp = auxpostP.get(id2);
                       
                    if(listapp==null)
                        listapp = new ArrayList<>();

                    listapp.add(posts.getValue());

                    auxpostP.put(id2,listapp);
                }
            }

        }
        return auxpostP;
	}

	/**
     * Query 9 Auxiliar  - Guarda respostas se pertencerem ao id1 ou ao id2
	 * @param struct class das estruturas
     * @param auxpostR Treemap com key Id e value um ArrayList de Posts 
     * @param id1 Identificador do primeiro User
     * @param id2 Identificador do segundor User
     * @return Treemap com key Id e value um ArrayList de Posts
     **/
	public TreeMap<Long,ArrayList<Post>> guardaRespostasOwnerID(Struct struct, TreeMap<Long,ArrayList<Post>> auxpostR, long id1, long id2){

		 for(Map.Entry<LocalDate,HashMap<Long,Post>> respostas : struct.getPostsRespostas().descendingMap().entrySet()) {

            for(Map.Entry<Long,Post> posts : respostas.getValue().entrySet()){              
                if(posts.getValue().getOwnerId() == id1 ){
                    ArrayList<Post> listapp = auxpostR.get(id1);
                       
                    if(listapp==null)
                        listapp = new ArrayList<>();

                    listapp.add(posts.getValue());

                    auxpostR.put(id1,listapp);
                }

                    if(posts.getValue().getOwnerId() == id2  ){
                        ArrayList<Post> listapp = auxpostR.get(id2);
                           
                        if(listapp==null)
                            listapp = new ArrayList<>();

                        listapp.add(posts.getValue());

                        auxpostR.put(id2,listapp);
                }
            }

        }
		return auxpostR;
	}

	/**
     * Query 9 Auxiliar  - Guarda ID das perguntas que tenham resposta
	 * @param post1 ArrayList de Posts
	 * @param post2 ArrayList de Posts
     * @param both Treemap com key creationDate e value os IDs dos Posts
     * @return Treemap com key creationDate e value os IDs dos Posts (both)
     **/
	public TreeMap<LocalDate,Long> guardaCompareP1P2(ArrayList<Post> post1, ArrayList<Post> post2, TreeMap<LocalDate,Long> both){

        for(Post p : post1){
            for(Post p1 : post2){
                if(p.getIdPost() == p1.getParentId()){
                    both.put(p.getCreationDate(),p.getIdPost());
                }
            }
        }
        return both;
	}

	/**
     * Query 9 Auxiliar  - Guarda post se for pergunta e tiver resposta
	 * @param post1 ArrayList de Posts
	 * @param post2 ArrayList de Posts
     * @param both Treemap com key creationDate e value os IDs dos Posts
     * @return Treemap com key creationDate e value os IDs dos Posts (both)
     **/
	public TreeMap<LocalDate,Long> guardaByPostType(ArrayList<Post> post1, ArrayList<Post> post2, TreeMap<LocalDate,Long> both){

        for(Post p : post1){
            for(Post p1 : post2){
                if(p.getIdPost() == p1.getParentId() && p.getPostType() == 1){
                    both.put(p.getCreationDate(),p.getIdPost());
                }
            }
        }
        return both;
	}


	/**
     * Query 11 Auxiliar  - Guarda tags
	 * @param struct class das estruturas
	 * @param users Map que contem um long e um arraylist com longs
     * @param nm NavigableMap obtido atraves do SubMap(intervalo entre begin e end)
     * @param tags hashmap com as tags
     * @param N int 
     * @return Treemap com key creationDate e value os IDs dos Posts (both)
     **/
	public HashMap<String,Integer> guardaTags(Struct struct, Map<Long,ArrayList<Long>> users, NavigableMap<LocalDate,HashMap<Long,Post>> nm, HashMap<String,Integer> tags, int N){
		
		TreeMap<Long, ArrayList<Long>> usersN = users.entrySet().stream()
        .limit(N)
        .collect(TreeMap::new, (m, e) -> m.put(e.getKey(), e.getValue()), Map::putAll);

		for(HashMap<Long,Post> hs : nm.values()){
            for(Post p : hs.values()){
                Long idU = struct.getUsers().get(p.getOwnerId()).getId();//id do user do post
                Long rep = struct.getUsers().get(idU).getReputation();//reputaçao do user do post
                ArrayList us = usersN.get(rep);
                if((us!=null)&&(us.contains(idU))){
                    String t = p.getTags();
                    String[] parts = t.split("[\\<\\>]");
                    int size = parts.length;
                    for(int i=0;i<size;i++){
                        if(!parts[i].equals("")){
                            if(tags.get(parts[i])==null){
                                tags.put(parts[i],1);
                            }
                            else{
                                Integer n = tags.get(parts[i])+1;
                                tags.put(parts[i],n);
                            }
                        }
                    }
                }
            }
        }
        return tags;
	}
}