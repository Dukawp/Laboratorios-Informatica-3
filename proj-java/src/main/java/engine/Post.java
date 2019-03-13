/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package engine;

import java.time.LocalDate;
import java.util.Date;

/**
 *
 * @author nadin
 */
class Post {
    
    private long idPost;
    private int postType;
    private LocalDate creationDate;
    private String title;
    private long parentId;
    private String tags;
    private long ownerId;
    private long score;
    private int commentCount;
    private int answerCount;
    private int nVotes;

    public Post(long idPost, int postType, LocalDate creationDate, String title, long parentId, String tags, long ownerId, long score, int commentCount, int answerCount, int nVotes) {
        this.idPost = idPost;
        this.postType = postType;
        this.creationDate = creationDate;
        this.title = title;
        this.parentId = parentId;
        this.tags = tags;
        this.ownerId = ownerId;
        this.score = score;
        this.commentCount = commentCount;
        this.answerCount = answerCount;
        this.nVotes = nVotes;
    }

    public int getPostType() {
        return postType;
    }

    public LocalDate getCreationDate() {
        return creationDate;
    }

    public String getTitle() {
        return title;
    }

    public long getParentId() {
        return parentId;
    }

    public String getTags() {
        return tags;
    }

    public long getOwnerId() {
        return ownerId;
    }

    public long getScore() {
        return score;
    }

    public int getCommentCount() {
        return commentCount;
    }

    public int getAnswerCount() {
        return answerCount;
    }

    public int getnVotes() {
        return nVotes;
    }

    public long getIdPost() {
        return idPost;
    }

    public void setIdPost(long idPost) {
        this.idPost = idPost;
    }

    public void setPostType(int postType) {
        this.postType = postType;
    }

    public void setCreationDate(LocalDate creationDate) {
        this.creationDate = creationDate;
    }

    public void setTitle(String title) {
        this.title = title;
    }

    public void setParentId(long parentId) {
        this.parentId = parentId;
    }

    public void setTags(String tags) {
        this.tags = tags;
    }

    public void setOwnerId(long ownerId) {
        this.ownerId = ownerId;
    }

    public void setScore(long score) {
        this.score = score;
    }

    public void setCommentCount(int commentCount) {
        this.commentCount = commentCount;
    }

    public void setAnswerCount(int answerCount) {
        this.answerCount = answerCount;
    }

    public void setnVotes(int nVotes) {
        this.nVotes = nVotes;
    }

    
    
}
