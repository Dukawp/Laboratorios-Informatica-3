package engine;

import java.time.LocalDate;
import java.util.*;
import common.Pair;

public class AuxiliaUsers{

	public AuxiliaUsers(){
	}

	 /**
     * Query 2 Auxiliar  - Users ordenados pelo CountPosts
     * @param struct class das estruturas
     * @return TreeMap com CountPosts como key e uma ArrayList de Users como value
     **/
	public TreeMap<Integer,ArrayList<User>> usersByCountPosts(Struct struct){
		TreeMap<Integer,ArrayList<User>> tree = new TreeMap<>(Collections.reverseOrder());
        for(Map.Entry<Long,User> users : struct.getUsers().entrySet()){
            ArrayList cp = tree.get(users.getValue().getCountPosts());
            if(cp==null)
                cp = new ArrayList<>();
            cp.add(users.getValue());
            tree.put(users.getValue().getCountPosts(),cp);
        }
        return tree;
	}

	/**
     * Query 2 Auxiliar  - Guarda os N primeiros IDs num ArrayList 
     * @param N numero desejado de IDs
     * @param users TreeMap com CountPosts como key e um ArrayList de users como value
     * @return Lista com os N IDs 
     **/
	public List<Long> obtemTopN(int N,TreeMap<Integer,ArrayList<User>> users){
		List<Long> list = new ArrayList<>();
        int i=0;
		for(Map.Entry<Integer,ArrayList<User>> tr : users.entrySet()){
	            for(User u : tr.getValue()){
	                if(u.getId()!=-1){
	                    list.add(u.getId());
	                    i++;
	                }
	                if(i==N) break;
	            }
	            if(i==N) break;
	        }
	        return list;
	}

	/**
     * Query 11 Auxiliar  - Guarda os N primeiros IDs num ArrayList 
	 * @param struct class das estruturas
     * @param nm NavigableMap obtido atraves do SubMap(intervalo entre begin e end)
 	 * @param users Map que contem um long e um arraylist com longs
     * @return Lista com os N IDs 
     **/
	public Map<Long,ArrayList<Long>> guardaUserByRep(Struct struct, NavigableMap<LocalDate,HashMap<Long,Post>> nm, Map<Long,ArrayList<Long>> users){
		for(HashMap<Long,Post> hs : nm.values()){
            for(Post p : hs.values()){
                long idU = p.getOwnerId();
                User u = struct.getUsers().get(idU);
                ArrayList ul = users.get(u.getReputation());
                if(ul==null)
                    ul = new ArrayList<Long>();
                ul.add(u.getId());
                users.put(u.getReputation(),ul);
            }
        }
        return users;
	}
}