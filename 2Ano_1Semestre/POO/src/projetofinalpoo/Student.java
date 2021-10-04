/*
Code developed by Edgar Filipe Ferreira Duarte, student number: 2019216077
personal email: edgartip2003@gmail.com
institutional email : edgarduarte@student.dei.uc.pt
DEI 2020-2021
 */
package projetofinalpoo;

/**
 *A Student class is a child from the Researcher class. Besides all the variables that define a Researcher (and so it also defines a Student), a 
 * Student is defined by its thesis title, the thesis conclusion Date and the guiding Researcher. By default its type is set to "student".
 * <p>
 * A Students has constructors, sets, gets and toString() methods of its own, but it inherits all methods from the Researcher class.
 * 
 * @see Researcher
 * @author Edgar Duarte
 */
public class Student extends Researcher{
    private String thesisTitle;
    private Date conclusionDate;
    private Researcher guidingResearcher;
    
    /**
     * Sole constructor, initializes the type as "student"
     */
    public Student(){
        type = "student";
    }
    
    /**
     * Constructor that initializes the Student and Researcher variables (name, email, research group, thesis title, thesis conclusion date and guiding researcher). 
     * The type of student is set to a default value of "student".
     * @param name, name of the Student
     * @param email, email of the Student
     * @param researchGroup, researchGroup that the Student is in
     * @param thesisTitle, title of the thesis the Student wrote
     * @param conclusionDate, conclusion date of the thesis
     * @param guidingResearcher, Researcher that helps the Student
     * 
     */
    public Student(String name, String email, ResearchGroup researchGroup, String thesisTitle, Date conclusionDate, Researcher guidingResearcher ) {
        super(name, email, researchGroup);
        this.thesisTitle = thesisTitle;
        this.conclusionDate = conclusionDate;
        this.guidingResearcher = guidingResearcher;
        type = "student";
    }
        
    /**
     * Returns the current title of the thesis the Student wrote
     * @return name of the thesis written by the Student
     */
    public String getThesisTitle() {
        return thesisTitle;
    }
    
    /**
     * Alters the value of the title of the thesis the Student wrote
     * @param thesisTitle new name of the thesis written by the Student
     */
    public void setThesisTitle(String thesisTitle) {
        this.thesisTitle = thesisTitle;
    }

    /**
     * Returns the Date of conclusion of the Student's thesis
     * @return Date of conclusion of the Students thesis
     * @see Date
     */
    public Date getConclusionDate() {
        return conclusionDate;
    }

    /**
     * Alters the Date of conclusion of the Student's thesis
     * @param conclusionDate new Date of conclusion of the thesis
     * @see Date
     */
    public void setConclusionDate(Date conclusionDate) {
        this.conclusionDate = conclusionDate;
    }

    /**
     * Returns the Researcher that guides the Student
     * @return current Researcher that guides the Student
     */
    public Researcher getGuidingResearcher() {
        return guidingResearcher;
    }

    /**
     * Alters the Researcher whose job is to guide the Student
     * @param guidingResearcher new Researcher who will guide the Student
     */
    public void setGuidingResearcher(Researcher guidingResearcher) {
        this.guidingResearcher = guidingResearcher;
    }
    
    @Override
    /**
     * Method that returns a string that describes the Student by its name, email, thesis title, thesis conclusion date and
     * guiding Researcher name in the format:
     * <p>
     * name: __, email: __, thesisTitle: __, conclusionDate: __, guiding Researcher: __
     * <p>
     * Note: __ indicates a value to be set
     * <p>
     * Note: the to.String() method from its parent class (Researcher) is also used
     * 
     * @return a String that indicates the Student's 
     * @see Researcher
     */
    public String toString(){
        return super.toString() + ", thesis title: " + thesisTitle + ", conclusionDate: " + conclusionDate.toString() + ", guiding Researcher: " + guidingResearcher.getName();
    }
    
    
}
