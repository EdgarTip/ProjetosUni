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
 * The ResearchGroup class represents a group of Researchers that work on a same topic in the CISUC institution. A ResearchGroup
 * is defined by its name, acronym, leadResearcher (Researcher leader) and a list of Researchers (researcherList) that are part of this ResearchGroup.
 * <p>
 * Besides the get and set functions a ResearchGroup class can also show, in the console, all of the Researcher object's names that are part of 
 * the researcherList. 
 * <p>
 * A ResearchGroup is a Serializable object, meaning that it can be stored in a object/binary file.
 * 
 * @see Publication
 * @see Researcher
 * 
 * @author Edgar Duarte
 */
public class ResearchGroup implements Serializable{
    private String name; 
    private String acronym;
    private EffectiveMember leadResearcher;
    private List<Researcher> researcherList;
    
    /**
     * Sole Constructor that initializes the researchList 
     **/
    public ResearchGroup(){
        researcherList = new ArrayList<>();
    }
    
    /**
     * Constructor that initializes the researchList, the name and acronym of the ResearchGroup
     * @param name, name of the ResearchGroup
     * @param acronym, acronym of the ResearchGroup
     */
    public ResearchGroup(String name, String acronym) {
        this();
        this.name = name;
        this.acronym = acronym;

    }
    
    
    /**
    * Constructor that initializes the name, acronym, lead researcher the ResearchGroup and the researcherList  
    * @param name, name of the ResearchGroup
    * @param acronym, acronym of the ResearchGroup
    * @param leadResearcher, lead Researcher of the ResearchGroup
    * @param researcherList, list of Researchers that are part of the ResearchGroup
    */
    public ResearchGroup(String name, String acronym, EffectiveMember leadResearcher, List<Researcher> researcherList) {
        this.name = name;
        this.acronym = acronym;
        this.leadResearcher = leadResearcher;
        this.researcherList = researcherList;
    }
    
    //Gets and sets
    /**
     * Returns the current name of the ResearchGroup
     * @return the name of the ResearchGroup
     */
    public String getName() {
        return name;
    }

    /**
     * Alters the name of the ResearchGroup
     * @param name new name of the ResearchGroup
     */
    public void setName(String name) {
        this.name = name;
    }

    /**
     * Returns the current acronym of the ResearchGroup
     * @return acronym of the ResearchGroup
     */
    public String getAcronym() {
        return acronym;
    }

    /**
     * Alters the acronym of the ResearchGroup
     * @param acronym new acronym of the ResearchGroup
     */
    public void setAcronym(String acronym) {
        this.acronym = acronym;
    }

    /**
     * Returns the current leadResearcher of the ReaserchGroup
     * @return leadResearcher of the ResearchGroup
     */
    public EffectiveMember getLeadResearcher() {
        return leadResearcher;
    }

    /**
     * Alters the Researcher that is the leadReasercher of the ResearchGroup
     * @param leadResearcher new leadResearcher of the ResearchGroup
     */
    public void setLeadResearcher(EffectiveMember leadResearcher) {
        this.leadResearcher = leadResearcher;
    }
    
    /**
     * Returns the current list of Researchers that make up the ResearchGroup
     * @return researcherList of the ResearchGroup
     */
    public List<Researcher> getResearcherList() {
        return researcherList;
    }

    /**
     * Alters the researchList of the ResearchGroup
     * @param researcherList new researchList of the ResearchGroup
     */
    public void setResearcherList(List<Researcher> researcherList) {
        this.researcherList = researcherList;
    }
    
    /**
     * This method adds a Researcher to the researcherList
     * @param researcher new Researcher to add to the researcherList
     */
    public void addResearcher(Researcher researcher){
        researcherList.add(researcher);
    }

    @Override
    /**
     * Method that returns a string that describes this ResearchGroup
     */
    public String toString() {
        return "ResearchGroup{" + "name=" + name + ", acronym=" + acronym + ", leadResearcher=" + leadResearcher + '}';
    }
    
    /**
     * This method shows all of the researchers organized by their categories. This method uses the compareTo() function of the Researcher class
     * to easily sort the researcherList. The end result will be something like the following:
     * 
     * EFFECTIVE MEMBER:
     * -member1
     * -member2
     * -member76
     * 
     * STUDENT:
     * -member4
     * -member12
     * 
     */
    public void showResearchers(){
        Collections.sort(researcherList);
        String previousType = "";
        for(Researcher res : researcherList){
            if(res.getType().equals(previousType)){
                System.out.println("-" + res.toString());
            }
            else{
                System.out.println();
                System.out.println(res.getType().toUpperCase() +":");
                System.out.println("-" + res.toString());
                previousType = res.getType();
            }
        }
        
    }
    

}
