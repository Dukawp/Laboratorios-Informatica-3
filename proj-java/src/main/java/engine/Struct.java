package engine;
import java.util.HashMap;
import java.util.Locale;
import java.util.Map;
import java.util.Map.Entry;
import java.util.TreeMap;
import java.time.LocalDate;


public class Struct{

    private HashMap<Long,User> users;
    
    private TreeMap<LocalDate,HashMap<Long,Post>> postsPerguntas;
    
    private TreeMap<LocalDate,HashMap<Long,Post>> postsRespostas;
        
    private HashMap<String,Long> tags;

    public Struct() {
        this.users = new HashMap<Long,User>();
        this.postsPerguntas = new TreeMap<LocalDate,HashMap<Long,Post>>();
        this.postsRespostas = new TreeMap<LocalDate,HashMap<Long,Post>>();
        this.tags = new HashMap<String,Long>();
    }
    public Struct(HashMap<Long,User> users,TreeMap<LocalDate,HashMap<Long,Post>> postsPerguntas,
                  TreeMap<LocalDate,HashMap<Long,Post>> postsRespostas,HashMap<String,Long> tags) {
        this.users = users;
        this.postsPerguntas = postsPerguntas;
        this.postsRespostas = postsRespostas;
        this.tags = tags;
    }

    public HashMap<Long,User> getUsers(){
        return this.users;
    }

    public void setUsers(HashMap<Long,User> users){
        this.users = users;
    }

    public void setPostsPerguntas(TreeMap<LocalDate,HashMap<Long,Post>> postsPerguntas){
        this.postsPerguntas = postsPerguntas;
    }

    public void setPostsRespostas(TreeMap<LocalDate,HashMap<Long,Post>> postsRespostas){
        this.postsRespostas = postsRespostas;
    }

    public TreeMap<LocalDate,HashMap<Long,Post>> getPostsPerguntas(){
        return this.postsPerguntas;
    }

    public TreeMap<LocalDate,HashMap<Long,Post>> getPostsRespostas(){
        return this.postsRespostas;
    }

    public HashMap<String,Long> getTags(){
        return this.tags;
    }
    public void setTags(HashMap<String,Long> tags){
        this.tags = tags;
    }
}