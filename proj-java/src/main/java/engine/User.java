/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package engine;

/**
 *
 * @author nadin
 */
public class User {
    
    private long idUser;
    private String name;
    private String about;
    private long reputation;
    private int countPosts;
    
    public User(long id, String name, String about, long reputation,int countPosts) {
        this.idUser = id;
        this.name = name;
        this.about = about;
        this.reputation = reputation;
        this.countPosts = countPosts;
    }

    public long getId() {
        return idUser;
    }

    public String getName() {
        return name;
    }

    public String getAbout() {
        return about;
    }

    public long getReputation() {
        return reputation;
    }

    public int getCountPosts() {
        return countPosts;
    }

    public void setId(long idUser) {
        this.idUser = idUser;
    }

    public void setName(String name) {
        this.name = name;
    }

    public void setAbout(String about) {
        this.about = about;
    }

    public void setReputation(long reputation) {
        this.reputation = reputation;
    }

    public void setCountPosts(int countPosts) {
        this.countPosts = countPosts;
    }

  
}

