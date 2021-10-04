/*
Code developed by Edgar Filipe Ferreira Duarte, student number: 2019216077
personal email: edgartip2003@gmail.com
institutional email : edgarduarte@student.dei.uc.pt
DEI 2020-2021
 */
package projetofinalpoo;
import java.io.*;
/**
 *The Researcher class represents a researcher that is part of a ResearchGroup and that works in a CISUC object. A Researcher is defined by its
 * name, email, researchGroup and type. The type of a Researcher is by default an empty string "".
 * <p> 
 * Besides the gets and sets methods a Researcher also has the compareTo() method that orders Researchers by their type
 * <p>
 * A Researcher is Serializable and implements Comparable. 
 *
 * @see ResearchGroup
 * @author edgar
 */
public class Researcher implements Serializable, Comparable<Researcher>{
    protected String name;
    protected String email;
    protected ResearchGroup researchGroup;
    protected String type = "";
    
    /**
     * Sole constructor
     */
    public Researcher(){}

    /**
     * Constructor that initializes the name, email and researchGroup of a Researcher
     * @param name, name of the Researcher
     * @param email, email of the Researcher
     * @param researchGroup, researchGroup of the Researcher
     */
    public Researcher(String name, String email, ResearchGroup researchGroup) {
        this.name = name;
        this.email = email;
        this.researchGroup = researchGroup;
    }
    
    
    /**
     * Returns the current name of the Researcher
     * @return name of the Researcher
     */
    public String getName() {
        return name;
    }

    /**
     * Alters the name of the Researcher
     * @param name, new name of the Researcher
     */
    public void setName(String name) {
        this.name = name;
    }

    /**
     * Returns the current email of the Researcher
     * @return email of the Researcher
     */
    public String getEmail() {
        return email;
    }

    /**
     * Alters the email of the Researcher
     * @param email new email of the Researcher
     */
    public void setEmail(String email) {
        this.email = email;
    }

    /**
     * Returns the ResearchGroup of the Researcher
     * @return ResearchGroup of the Researcher
     */
    public ResearchGroup getResearchGroup() {
        return researchGroup;
    }

    /**
     * Alters the ResearchGroup of the Researcher
     * @param researchGroup new ResearchGroup of the Researcher
     */
    public void setResearchGroup(ResearchGroup researchGroup) {
        this.researchGroup = researchGroup;
    }

    /**
     * Returns the type of researcher of the Researcher
     * @return type of researcher of the Researcher
     */
    public String getType() {
        return type;
    }

    /**
     * Alters the type of researcher of the Researcher
     * @param type, new type of researcher of the Researcher
     */
    public void setType(String type) {
        this.type = type;
    }

    @Override
    /**
     * Method that returns a string that describes the Researcher by its name and email in the format: 
     * <p>
     * name: __, email: __
     * <p>
     * Note: __ represents a value to be set
     * @return a String that indicates the Researcher's name and email
     */
    public String toString() {
        return "name: " + name + ", email:" + email;
    }
    
    /**
     * This method orders Researcher that are in a List of Researchers, by their types, in alphabetical order.
     * This method is best used in conjunction with the Collections.sort() function, although it can be used to just
     * see which of 2 Researchers is greater (by type).
     * @param res Researcher to compare this Researcher with
     * @return 1 if this Researcher type is greater than the Researcher type by parameter, -1 if this Researcher type is less than the Researcher type by parameter or
     * 0 if the 2 Researcher types are equal.
     */
    public int compareTo(Researcher res){
        if(this.getType().compareTo(res.getType()) > 0) return 1;
        else if (this.getType().compareTo(res.getType()) < 0) return -1;
        else return 0;
    }
    
}
