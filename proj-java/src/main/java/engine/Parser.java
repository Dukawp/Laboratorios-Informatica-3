package engine;


import java.io.File;
import static java.lang.Long.parseLong;
import java.text.DateFormat;
import java.text.ParseException;
import java.time.LocalDateTime;
import java.util.Calendar;
import java.util.HashMap;
import java.util.Locale;
import java.util.Map;
import java.util.Map.Entry;
import java.util.TreeMap;
import java.util.logging.Level;
import java.util.logging.Logger;
import java.time.*;

import org.xml.sax.SAXException;
import javax.xml.parsers.SAXParser;
import javax.xml.parsers.SAXParserFactory;

import org.xml.sax.Attributes;
import org.xml.sax.helpers.DefaultHandler;


import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.*;
import java.util.Iterator;
import javax.xml.stream.XMLEventReader;
import javax.xml.stream.XMLInputFactory;
import javax.xml.stream.XMLStreamConstants;
import javax.xml.stream.XMLStreamException;
import javax.xml.stream.XMLStreamReader;
import javax.xml.stream.events.Attribute;
import javax.xml.stream.events.Characters;
import javax.xml.stream.events.EndElement;
import javax.xml.stream.events.StartElement;
import javax.xml.stream.events.XMLEvent;


/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * @author nadin
 */
public class Parser extends DefaultHandler{

    public Parser(){}

    private static HashMap<Long,User> parseUsers(XMLStreamReader reader){
        HashMap<Long,User> users = new HashMap<Long,User>();
        try{
            while (reader.hasNext()) {
                int eventType = reader.next();
                switch(eventType){
                    case XMLStreamReader.START_ELEMENT:
                        String elementName = reader.getLocalName();
                        if (elementName.equals("row")){
                            long id = parseLong(reader.getAttributeValue(null,"Id"));
                            String name = reader.getAttributeValue(null,"DisplayName");
                            String about = reader.getAttributeValue(null,"AboutMe");
                            long reputation = parseLong(reader.getAttributeValue(null,"Reputation"));
                            //System.out.println("row "+id +" "+name+" "+ reputation);

                            User user = new User(id,name,about,reputation,0);
                            users.put(id,user);
                        }

                        break;
                }
            }
        } catch (XMLStreamException e) {
             e.printStackTrace();
          }
        return users;
    }
    private static void incNPosts(long id, Struct struct){
        User h = struct.getUsers().get(id);
        int n = h.getCountPosts();
        n++;
        h.setCountPosts(n);
    }

    private static void parsePosts(XMLStreamReader reader,Struct struct){
        TreeMap<LocalDate,HashMap<Long,Post>> postsP = new TreeMap<>();
        TreeMap<LocalDate,HashMap<Long,Post>> postsR = new TreeMap<>();
        try{
            while (reader.hasNext()) {
                int eventType = reader.next();
                switch(eventType){
                    case XMLStreamReader.START_ELEMENT:
                        String elementName = reader.getLocalName();
                        if (elementName.equals("row")){
                            long id = parseLong(reader.getAttributeValue(null,"Id"));
                            long parentId=0;
                            if((reader.getAttributeValue(null,"ParentId"))!=null)
                                parentId = parseLong(reader.getAttributeValue(null,"ParentId"));
                            int postType=0;
                            if((reader.getAttributeValue(null,"PostTypeId"))!=null)
                                postType = Integer.parseInt(reader.getAttributeValue(null,"PostTypeId"));
                            String title = reader.getAttributeValue(null,"Title");
                            String creationDate = reader.getAttributeValue(null,"CreationDate");
                            String tags = reader.getAttributeValue(null,"Tags");
                            long ownerId = parseLong(reader.getAttributeValue(null,"OwnerUserId"));
                            long score = parseLong(reader.getAttributeValue(null,"Score"));
                            int comments=0;
                            if((reader.getAttributeValue(null,"CommentCount"))!=null)
                                comments = Integer.parseInt(reader.getAttributeValue(null,"CommentCount"));
                            int answers=0;
                            if((reader.getAttributeValue(null,"AnswerCount"))!=null)
                                answers = Integer.parseInt(reader.getAttributeValue(null,"AnswerCount"));
                            int nVotes = 0;
                            LocalDateTime datacompleta = LocalDateTime.parse(creationDate);
                            //LocalDate dataaa = localDateTime.toLocalTime();
                            LocalDate localDate = datacompleta.toLocalDate();
                            LocalDate data = localDate.withDayOfMonth(1);
                            //System.out.println("DATE :: " + datacompleta);
                            //System.out.println("DATE :: " + localDate);
                            Post post = new Post(id,postType,localDate,title,parentId,tags,ownerId,score,comments,answers,(int)nVotes);
                            if(postType == 1){
                                  //  System.out.println("A guardar em postsPerguntas : "+post.getCreationDate());

                                HashMap<Long,Post> posts  = postsP.get(data);
                                   
                                if(posts==null)
                                    posts = new HashMap<>();
                                        
                                    posts.put(post.getIdPost(), post);

                                    postsP.put(data, posts);
                            }

                            if(postType == 2){
                                        
                                HashMap<Long,Post> posts  = postsR.get(data);
                                   
                                if(posts==null)
                                    posts = new HashMap<>();
                                        
                                    posts.put(post.getIdPost(), post);

                                    postsR.put(data, posts);
                            }

                            incNPosts(ownerId,struct);
                        }

                        break;
                }
            }
        } catch (XMLStreamException e) {
             e.printStackTrace();
          }

          struct.setPostsPerguntas(postsP);
          struct.setPostsRespostas(postsR);
    }


     private static void parseVotes(XMLStreamReader reader,Struct struct){
        try{
            while (reader.hasNext()) {
                int eventType = reader.next();
                switch(eventType){
                    case XMLStreamReader.START_ELEMENT:
                        String elementName = reader.getLocalName();
                        if (elementName.equals("row")){
                            long idPost = parseLong(reader.getAttributeValue(null,"PostId"));
                            int voteType = Integer.parseInt(reader.getAttributeValue(null,"VoteTypeId"));  

                            Post p = null;
                    
                            for(Map.Entry<LocalDate,HashMap<Long,Post>> entry : struct.getPostsPerguntas().entrySet()){
                                if((p=(entry.getValue().get(idPost)))!=null){
                                    
                                    int nv =  p.getnVotes();                                    if(voteType==1){
                                        nv = nv-1;
                                        entry.getValue().get(idPost).setnVotes(nv);  
                                    }
                                    else
                                        if(voteType==2){
                                            nv = nv+1;
                                            entry.getValue().get(idPost).setnVotes(nv); 
                                        }
                                    break;                   
                                }
                            }
                            if(p==null){
                                for(Map.Entry<LocalDate,HashMap<Long,Post>> entry : struct.getPostsRespostas().entrySet()){
                                    if((p=(entry.getValue().get(idPost)))!=null){
                                        int nv =  p.getnVotes();
                                        if(voteType==1){
                                            nv = nv-1;
                                            entry.getValue().get(idPost).setnVotes(nv); 
                                        }
                                        else
                                            if(voteType==2){
                                                nv = nv+1;
                                                entry.getValue().get(idPost).setnVotes(nv);    
                                            }
                                        break;                
                                    }
                                }

                            }
                        }

                        break;
                }
            }
        } catch (XMLStreamException e) {
             e.printStackTrace();
          }
    }

    private static HashMap<String,Long> parseTags(XMLStreamReader reader){
        HashMap<String,Long> tags = new HashMap<>();
        try{
            while (reader.hasNext()) {
                int eventType = reader.next();
                switch(eventType){
                    case XMLStreamReader.START_ELEMENT:
                        String elementName = reader.getLocalName();
                        if (elementName.equals("row")){
                            long idTag = parseLong(reader.getAttributeValue(null,"Id"));
                            String name = reader.getAttributeValue(null,"TagName");
                            tags.put(name,idTag);
                        }
                        break;
                }
            }
        } catch (XMLStreamException e) {
             e.printStackTrace();
          }
        return tags;
    }

    public static void parser(String fileName, Struct struct) {
        System.out.println("PARSING" + fileName);
    try{
         XMLInputFactory factory = XMLInputFactory.newInstance();
         XMLStreamReader reader = factory.createXMLStreamReader(new FileInputStream(fileName));

         while (reader.hasNext()) {
            int eventType = reader.next();
            switch(eventType){
                case XMLStreamReader.START_ELEMENT:
                    String elementName = reader.getLocalName();
                    if (elementName.equals("users")){
                        System.out.println("users");
                        
                        struct.setUsers(parseUsers(reader));
                    }
                    if (elementName.equals("posts")){
                        System.out.println("posts");
                        
                        parsePosts(reader,struct);
                    }
                    if (elementName.equals("votes")){
                        System.out.println("votes");
                        
                        parseVotes(reader,struct);
                    }
                    if (elementName.equals("tags")){
                        System.out.println("tags");
                        
                        struct.setTags(parseTags(reader));
                    }
                    break;
            }
        }

    }catch (FileNotFoundException e) {
         e.printStackTrace();
    } catch (XMLStreamException e) {
         e.printStackTrace();
      }

    }
}        
