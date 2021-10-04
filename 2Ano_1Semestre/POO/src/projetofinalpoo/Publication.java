/*
Code developed by Edgar Filipe Ferreira Duarte, student number: 2019216077
personal email: edgartip2003@gmail.com
institutional email : edgarduarte@student.dei.uc.pt
DEI 2020-2021
 */
package projetofinalpoo;

import java.util.*;
import java.io.*;

/**
 * A Publication is an abstract class that represents a publication made by a researcher or a group of researchers. It is defined
 * by its authors, its title, its summary, its keywords, its publication year, its audience and its type. The default type of a Publication
 * object is "unknown". 
 * <p>
 * Besides the gets, sets, constructors and toString methods, a Publication class has a abstract method and a compareTo() method. The abstract class 
 * will be implemented in the child classes of a Publication. The compareTo() method is used to compare 2 publications about their factor of impact.
 * <p>
 * A Publication is serializable so it can be saved to a binary/object file. It also implements Comparable.
 * @author Edgar Duarte
 */
public abstract class Publication implements Serializable, Comparable<Publication> {

    protected List<Researcher> authors;
    protected String title;
    protected String summary;
    protected List<String> keywords;
    protected int publicationYear;
    protected int audience;
    protected String type;

    /**
     * Sole constructor. Initializes the keywords and authors ArrayLists and initializes the default value of type to "unknown"
     */
    public Publication() {
        keywords = new ArrayList<>();
        authors = new ArrayList<>();
        type = "unknown";
    }

    /**
     * Constructor that initializes the values of the title, list of authors, summary, list of keywords, publication year and audience.
     * @param title, title of the Publication
     * @param authors, authors of the Publication
     * @param summary, summary that describes the Publication
     * @param keywords, keywords that define the Publication
     * @param publicationYear, year on which the Publication was released
     * @param audience , amount of people who saw the Publication
     */
    public Publication(String title, List<Researcher> authors, String summary, List<String> keywords, int publicationYear, int audience) {
        this.authors = authors;
        this.title = title;
        this.summary = summary;
        this.keywords = keywords;
        this.publicationYear = publicationYear;
        this.audience = audience;
    }

    /**
     * Returns a List of Researcher objects that participated in the making of this Publication
     * @return List of Researcher that made this Publication
     */
    public List<Researcher> getAuthor() {
        return authors;
    }

    /**
     * Adds a Researcher object to the authors list
     * @param author new Researcher that participated in the making of this publication
     */
    public void addAuthor(Researcher author) {
        authors.add(author);
    }

    /**
     * Returns the current title of the Publication
     * @return title of the Publication
     */
    public String getTitle() {
        return title;
    }

    /**
     * Alters the title of the Publication
     * @param title new title of the Publication
     */
    public void setTitle(String title) {
        this.title = title;
    }

    /**
     * Returns the current summary of the Publication
     * @return summary of the Publication
     */
    public String getSummary() {
        return summary;
    }

    /**
     * Returns the current value of the Publication
     * @return type of the Publication
     */
    public String getType() {
        return type;
    }

    /**
     * Alters the type of the Publication
     * @param type new type of the Publication
     */
    public void setType(String type) {
        this.type = type;
    }

    /**
     * Alters the summary of the Publication
     * @param summary new summary of the Publication
     */
    public void setSummary(String summary) {
        this.summary = summary;
    }

    /**
     * Returns the current keywords of the Publication
     * @return keywords of the Publication
     */
    public List<String> getKeywords() {
        return keywords;
    }

    /**
     * Alters the keywords of the Publication
     * @param keywords new List of keywords of the Publication
     */
    public void setKeywords(List<String> keywords) {
        this.keywords = keywords;
    }

    /**
     * Returns the current yearPublication of the Publication
     * @return yearPublication of the Publication
     */
    public int getPublicationYear() {
        return publicationYear;
    }

    /**
     * Alters the value of the publicationYear of the Publication
     * @param publicationYear new publicationYear of the Publication
     */
    public void setPublicationYear(int publicationYear) {
        this.publicationYear = publicationYear;
    }

    /**
     * Returns the current amount of people that saw the Publication
     * @return audience of the Publication
     */
    public int getAudience() {
        return audience;
    }

    /**
     * Alters the amount of people that saw the Publication
     * @param audience new amount of people that saw the Publication
     */
    public void setAudience(int audience) {
        this.audience = audience;
    }

    /**
     * Method that will calculate a Publication's child factor of impact
     * @return a String that indicates the factor of impact (either "A" , "B" or "C")
     */
    protected abstract String impactFactorCalculator();


    @Override
    /**
     * Method that compares the current Publication with another given as parameter and compares their factor of impact and decides which one is greater.
     * A factor of type "A" is greater than a factor of type "B" and a factor of type "B" is greater than a factor of type "C". 
     * 
     * @param pub, Publication that will compare with this Publication
     * 
     * @return -1 if this Publication is less than the Publication given as parameter, 1 if this Publication is greater than the Publication given as parameter and 
     * 0 if their value is equal.
     */
    public int compareTo(Publication pub) {
        //Sort the list in descending order
        //Compare the publication years
        if (this.getPublicationYear() < pub.getPublicationYear()) {
            return 1;
        } else if (this.getPublicationYear() > pub.getPublicationYear()) {
            return -1;
        } //means that the publication years are equal
        else {
            //See if the type of publication are different and order them alfabeticly
            if (this.getType().compareTo(pub.getType()) > 0) {
                return 1;
            } else if (this.getType().compareTo(pub.getType()) < 0) {
                return -1;
            } else {
                if (this.impactFactorCalculator().compareTo(pub.impactFactorCalculator()) < 0) {
                    return -1;
                } else if (this.impactFactorCalculator().compareTo(pub.impactFactorCalculator()) > 0) {
                    return 1;
                } else {
                    return 0;
                }

            }
        }
        //means there is nothing to sort
    }
    

    /**
     * Method that describes the characteristics of a Publication. It returns a string in the following format: 
     * <p>
     * Title : __, Summary : __, Type : __
     * <p>
     *  Keywords : __
     * <p>
     *  Authors : __, Publication Year : __
     * <p>
     * Note: __ is the place where the values are going to appear (they change object to object)
     * 
     * @return a String that describes a Publication object
     * 
     */
     @Override
    public String toString() {
        return "Title : " + title + ", Summary : " + summary + ", Type : " + type +
                "\n Keywords : " + returnKeywordsNames() +
               "\n Authors : " + returnAuthorsNames() + ", Publication Year : " + publicationYear;
              
    }
    
    //This method returns the authors names in a way so the resulting String can be used in the toString() method   
    private String returnAuthorsNames(){
        String authorsNames = "";
        boolean isFirst = true;
        
        for(Researcher res : authors){
            String arrayOfNames[] = res.getName().split(" ");
            //Only runs after the first iteration
            if(!isFirst){
                authorsNames += " | ";
            }
            //A effective member has to have the title of Professor
            if( res.getType().equals("effective member")){
                authorsNames += "Professor " + arrayOfNames[0] + " " + arrayOfNames[arrayOfNames.length -1];
            }
            //A student only gets his first name initial and last name shown in a pulbication detail
            else if( res.getType().equals("student")){
                authorsNames += arrayOfNames[0].substring(0, 1) + ". " + arrayOfNames[arrayOfNames.length -1];
            }
            
            isFirst = false;
            
        }
        
        return authorsNames;
    }
    
    //This method returns the keywords names in a way so the resulting String can be used in the toString() method
    private String returnKeywordsNames(){
        String keywordsNames = "";
        boolean isFirst = true;
        
        for(String s : keywords){
            
            if(!isFirst){
                keywordsNames += " | ";
            }
            
            keywordsNames += s;
            isFirst = false;
        }
        
        return keywordsNames;
    }

    
    
}
