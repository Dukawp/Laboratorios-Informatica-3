package engine;

import common.MyLog;
import common.Pair;
import li3.TADCommunity;
import org.w3c.dom.Document;
import org.xml.sax.SAXException;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;
import java.io.File;
import java.io.IOException;
import java.time.LocalDate;
import java.util.*;

import javax.xml.parsers.SAXParser;
import javax.xml.parsers.SAXParserFactory;

import java.util.stream.*;

import engine.Parser;


public class TCDExample implements TADCommunity {

    private MyLog qelog;
    private Parser parser;
    private Struct struct;
    private AuxiliaPosts auxP;
    private AuxiliaUsers auxU;

    
    public void init() {
        this.qelog = new MyLog("queryengine");
        this.struct = new Struct();
        this.parser = new Parser();
        this.auxP = new AuxiliaPosts();
        this.auxU = new AuxiliaUsers();
    }

    public TCDExample(){
        init();
    }

    /**
     *
     * @param dumpPath Path dos ficheiros
     */
    public void load(String dumpPath) {
       // File inputFile = new File(dumpPath);
    //    try {


        String files[];
        files = new String[4];
        files[0] = "/Users.xml"; files[1] = "/Posts.xml";files[2] = "/Votes.xml";files[3] = "/Tags.xml";
            
        for(int f=0;f<4;f++){
            System.out.println("file :" + files[f]);
            File inputFile = new File(dumpPath+files[f]);
            parser.parser(dumpPath+files[f],this.struct);
        }
    }

    /**
     * Query 1 - procurar Titulo de um Post e o Nome do seu autor
     * @param id long que contem o identificador do Post
     * @return devolve um Pair que contem o titulo do Post e o Nome do seu autor
     **/
    public Pair<String,String> infoFromPost(long id) {
        //return new Pair<>("What are the actual risks of giving www-data sudo nopasswd access?", "WebNinja");
        Map<Long,User> users = struct.getUsers();
        Post p=null; 
        Pair pair = auxP.procuraPergunta(struct,users,p,id);
        if(p==null){
            long idPergunta = auxP.procuraResposta(struct,users,p,id);
            if(idPergunta!=0)
                return infoFromPost(idPergunta);
        }
        if(pair!=null)
            return pair;
        return new Pair<>(null,null);
    }

    /**
    * Query 2 - Utilizadores com maior numero de Posts de sempre
    * @param N Int do numero de utilizadores desejados
    * @return Devolve uma Lista com os IDs dos utilizadores
    * 
    **/
    public List<Long> topMostActive(int N) {
        TreeMap<Integer,ArrayList<User>> users = auxU.usersByCountPosts(struct);
        List list = auxU.obtemTopN(N,users); 
        return list;
    }

    /**
     * Query 3 - Total de posts num determinado tempo
     * @param begin Data inicial
     * @param end Data final
     * @return Pair que contem numero de perguntas e respostas respectivamente
     **/

    public Pair<Long,Long> totalPosts(LocalDate begin, LocalDate end) {
        //return new Pair<>(3667L,4102L);
        long np=0;
        long nr=0;
        LocalDate b1,b2;
        b1 = begin.withDayOfMonth(1); b2 = end.withDayOfMonth(1);
        NavigableMap<LocalDate,HashMap<Long,Post>> nmP = struct.getPostsPerguntas().subMap(b1,true,b2,true); 
        NavigableMap<LocalDate,HashMap<Long,Post>> nmR = struct.getPostsRespostas().subMap(b1,true,b2,true); 
        np=auxP.incrementaPosts(nmP,begin,end,np);  
        nr=auxP.incrementaPosts(nmR,begin,end,nr);      
        return new Pair<>(np,nr);
    }

    /**
     * Query 4 - procurar questoes com uma determinada Tag
     * @param tag String que contem a tag
     * @param begin Data inicial
     * @param end Data final
     * @return Lista com os IDs das perguntas ordenadas em cronologia inversa
     * 
    */
    public List<Long> questionsWithTag(String tag, LocalDate begin, LocalDate end) {
 
        TreeMap<LocalDate,ArrayList<Post>> auxpost;
        auxpost = new TreeMap<>();
        List<Long> lista;
        lista = new ArrayList<>();
        LocalDate b1,b2;
        b1 = begin.withDayOfMonth(1); b2 = end.withDayOfMonth(1);
        NavigableMap<LocalDate,HashMap<Long,Post>> nm = struct.getPostsPerguntas().subMap(b1,true,b2,true);
        Map<Integer,ArrayList<Post>> tree = new TreeMap<>(Collections.reverseOrder());
        auxpost = this.auxP.guardaPerguntasTag(nm, begin, end, tag);
        lista = this.auxP.treeToList(auxpost, lista);
        Collections.reverse(lista);
        return lista;
    }

    /**
     * Query 5 - Procurar informaçao do perfil de um utilizador e os IDs dos seus ultimos 10 Posts
     * @param id long que contem o ID do utilizador 
     * @return Pair com String(short bio) como primeiro parametro e uma lista com os IDs dos Posts
     */
    public Pair<String, List<Long>> getUserInfo(long id) {
        TreeMap<LocalDate,ArrayList<Post>> auxpost;
        auxpost = new TreeMap<>();
        ArrayList<Long> lista;
        lista = new ArrayList<>();
        String shortBio = struct.getUsers().get(id).getAbout();
        auxpost = this.auxP.guardaPostsPorData(struct, auxpost, id);
        auxpost = this.auxP.guardaPostsPorData(struct, auxpost, id);
        for(Map.Entry<LocalDate,ArrayList<Post>> listaPost : auxpost.descendingMap().entrySet()) {
            ArrayList<Post> auxList = listaPost.getValue();
            Collections.reverse(auxList);
            for(Post p : auxList){
                if(lista.size()<10)       
                    lista.add(p.getIdPost());
            }
        }
        return new Pair<>(shortBio,lista);
    }

    /**
     * Query 6 - Dado um intervalo de tempo arbitrario, devolve os IDs das N respostas com mais votos
     * @param N Numero de repostas esperadas
     * @param begin Data inicial
     * @param end Data final
     * @return Lista com os IDs dos posts(respostas) ordenada por ordem decrescente do numero de votos
     */
    public List<Long> mostVotedAnswers(int N, LocalDate begin, LocalDate end) {
        LocalDate b1,b2;
        b1 = begin.withDayOfMonth(1); b2 = end.withDayOfMonth(1);
        NavigableMap<LocalDate,HashMap<Long,Post>> nm = struct.getPostsRespostas().subMap(b1,true,b2,true);
        Map tree = auxP.postsByNVotes(nm,begin,end);
        List list = auxP.obtemTopNOfTree(N,tree);
        return list;

    }

    /**
     * Query 7 - Dado um intervalo de tempo arbitrario, devolve as IDs das N perguntas com mais respostas
     * @param N Numero de perguntas esperadas
     * @param begin Data inicial
     * @param end Data final 
     * @return Lista com os IDs do post(perguntas) ordenada por ordem decrescente do numero de votos
     */
    public List<Long> mostAnsweredQuestions(int N, LocalDate begin, LocalDate end) {
        LocalDate b1,b2;
        b1 = begin.withDayOfMonth(1); b2 = end.withDayOfMonth(1);
        NavigableMap<LocalDate,HashMap<Long,Post>> nm = struct.getPostsPerguntas().subMap(b1,true,b2,true);
        Map tree = auxP.postsByAnswer(nm,begin,end);
        List list = auxP.obtemTopNOfTree(N,tree);
        return list;
    }

    /**
     * Query 8 - Dado uma palavra, devolve uma lista com os IDs de N perguntas cujos titulos a contenham, ordenados por cronologia inversa
     * @param N Numero de perguntas esperadas
     * @param word Palavra que deve estar presente no titulo
     * @return Lista com os IDs das perguntas cuja palavra fazia parte do titulo
     */
    public List<Long> containsWord(int N, String word) {
        List <Post> listP = auxP.perguntasWithWord(N,word,struct);  
        List <Long> list = auxP.obtemTopNWords(N, listP,word);
        return list;
    }

    /**
     * Query 9 - Dados os IDs de dois utilizadores, devolve as ultimas N perguntas (cronologia inversa) em que participaram dois utilizadores
     * @param N Numero de perguntas esperadas 
     * @param id1 ID de um utilizador
     * @param id2 ID de outro utilizador
     * @return Lista com os IDs das N perguntas
     */
    public List<Long> bothParticipated(int N, long id1, long id2) {
     
        TreeMap<Long,ArrayList<Post>> auxpostP;
        auxpostP = new TreeMap<>();

        TreeMap<Long,ArrayList<Post>> auxpostR;
        auxpostR = new TreeMap<>();

        TreeMap<LocalDate,Long> both;
        both = new TreeMap<>();

        ArrayList listafinal;
        listafinal = new ArrayList<>();

        auxpostP = this.auxP.guardaPerguntasOwnerID(struct, auxpostP, id1, id2);
        auxpostR = this.auxP.guardaRespostasOwnerID(struct, auxpostR, id1, id2);
        ArrayList<Post> post1 = auxpostP.get(id1);
        ArrayList<Post> post2 = auxpostR.get(id2);
        both = this.auxP.guardaCompareP1P2(post1, post2,both);

        post1 = auxpostP.get(id2);
        post2 = auxpostR.get(id1);
        both = this.auxP.guardaCompareP1P2(post1, post2, both);

        post1 = auxpostR.get(id1);
        post2 = auxpostR.get(id2);
        both = this.auxP.guardaByPostType(post1, post2, both);

        for(Map.Entry<LocalDate,Long> post : both.descendingMap().entrySet()){
            if(listafinal.size()< N){
                listafinal.add(post.getValue());
            }
        }

        return listafinal;

    }

    /**
     * Query 10 - Dado um id de uma pergunta,obtemos a melhor resposta
     * @param id ID da pergunta 
     * @return ID do post com maior pontuaçao
     */
    public long betterAnswer(long id) {
        //return 175891;
        long scr, rep;
        int comt, vot;
        Post atual=null;
        double totalAtual=0;
        double total=0;
        for(Map.Entry<LocalDate,HashMap<Long,Post>> respostas : struct.getPostsRespostas().entrySet()) {
           for(Map.Entry<Long,Post> rm : respostas.getValue().entrySet()){
                Post r = rm.getValue();
                if(r.getParentId()==id){
                    scr = r.getScore();
                    rep = struct.getUsers().get(r.getOwnerId()).getReputation();
                    vot = r.getnVotes();
                    comt = r.getCommentCount();
                    total = scr*0.45 + rep*0.25 + vot*0.2 + comt*0.1;
                    if(atual==null){
                        atual=r;
                        totalAtual = total;
                    }
                    else{
                        if(total>totalAtual){
                            atual = r;
                            totalAtual = total;
                        }
                    }
                }
            }
        }
        return atual.getIdPost();
    }

    /**
     * Query 11 - Dado um intervalo arbitrario de tempo, devolve as N tags mais usadas pelos N utilizadores com melhor reputação.
     * @param N Numero de tags esperadas 
     * @param begin Data inicial
     * @param end Data final
     * @return Lista com os IDs 
     */
    public List<Long> mostUsedBestRep(int N, LocalDate begin, LocalDate end) {
        LocalDate b1,b2;
        b1 = begin.withDayOfMonth(1); b2 = end.withDayOfMonth(1);
        NavigableMap<LocalDate,HashMap<Long,Post>> nm = struct.getPostsPerguntas().subMap(b1,true,b2,true);

        Map<String,Integer> tags = new HashMap<>();
        Map<Long,ArrayList<Long>> users = new TreeMap<>(Collections.reverseOrder());

        for(Map.Entry<LocalDate,HashMap<Long,Post>> hs : struct.getPostsPerguntas().entrySet()){
            for(Post p : hs.getValue().values()){
                    long idU = p.getOwnerId();
                    User u = struct.getUsers().get(idU);
                    ArrayList ul = users.get(u.getReputation());
                    if(ul==null)
                        ul = new ArrayList<Long>();
                    if(!ul.contains(u.getId()))
                        ul.add(u.getId());
                    users.put(u.getReputation(),ul);
                }
        }
        TreeMap<Long, ArrayList<Long>> usersN = users.entrySet().stream()
        .limit(N)
        .collect(TreeMap::new, (m, e) -> m.put(e.getKey(), e.getValue()), Map::putAll);

 
        for(HashMap<Long,Post> hs : nm.values()){
            for(Post p : hs.values()){
                Long rep = struct.getUsers().get(p.getOwnerId()).getReputation();//reputaçao do user do post
                ArrayList us = usersN.get(rep);
                if((us!=null)&&(us.contains(p.getOwnerId()))){
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




        Map<String, Integer> sortedMap = tags.entrySet().stream()
        .sorted(Map.Entry.comparingByValue())
        .collect(Collectors.toMap(Map.Entry::getKey, Map.Entry::getValue,
                              (e1, e2) -> e1, LinkedHashMap::new));
       
        List<Long> list = new ArrayList<>();
        int it=0;
        for(Map.Entry<String,Integer> m : sortedMap.entrySet()){
            list.add(struct.getTags().get(m.getKey()));
            it++;
            if(it==N) break;
        }
        return list;
        //return Arrays.asList(6L,29L,72L,163L,587L);
       

    }
    public void clear(){
        System.out.println("Cya");
        this.struct.getPostsPerguntas().clear();
        this.struct.getPostsRespostas().clear();
        this.struct.getUsers().clear();
        this.struct.getTags().clear();
    }
}
