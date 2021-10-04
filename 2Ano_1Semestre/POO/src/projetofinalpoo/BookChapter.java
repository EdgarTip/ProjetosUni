/*
Code developed by Edgar Filipe Ferreira Duarte, student number: 2019216077
personal email: edgartip2003@gmail.com
institutional email : edgarduarte@student.dei.uc.pt
DEI 2020-2021
 */
package projetofinalpoo;

import java.util.List;

/**
 * A BookChapter is a child class of the Book class (which is a child class of the Publication class) so it inherits their methods and variables. A BookChapter is represented by its chapter name, 
 * its starting page, its ending page and all the variables that define a Book and a Publication.
 * @see Publication
 * @see Book
 * @author edgar
 * 
 */
public class BookChapter extends Book {

    private String chapterName;
    private int startingPage;
    private int finalPage;

    /**
     * Sole constructor
     */
    public BookChapter() {
    }


     /**
     * Constructor that initializes the BookChapter, Book and Publications values (title, author, summary, keywords, publicationYear, audience, isbn, publisher, 
     * chapterName, startingPage and finalPage)
     * @param title name of the title
     * @param author List of Researcher objects that made this BookChapter
     * @param summary description of this BookChapter
     * @param keywords List of Strings that describe this BookChapter
     * @param publicationYear year that this BookChapter was published
     * @param audience quantity of people to which this BookChapter was shown to
     * @param isbn the identifying code of a book
     * @param publisher name of the entity that published the Book where this BookChapter is
     * @param chapterName name of this BookChapter
     * @param startingPage number of the page where this chapter starts
     * @param finalPage number of the page where this chapter ends
     */
    public BookChapter(String title, List<Researcher> author, String summary, List<String> keywords, int publicationYear, int audience, String publisher, String isbn, String chapterName, int startingPage, int finalPage) {
        super(title, author, summary, keywords, publicationYear, audience, publisher, isbn);
        this.type = "BookChapter";
        this.chapterName = chapterName;
        this.startingPage = startingPage;
        this.finalPage = finalPage;
    }

     /**
     * This method returns the current chapter name
     * @return name of this chapter
     */
    public String getChapterName() {
        return chapterName;
    }

    /**
     * This method alters the name of this chapter
     * @param chapterName new name of the chapter
     */
    public void setChapterName(String chapterName) {
        this.chapterName = chapterName;
    }

    /**
     * This method returns the starting page of this BookChapter
     * @return an integer that represents the starting page of this BookChapter
     */
    public int getStartingPage() {
        return startingPage;
    }

    /**
     * This method alters the value of the starting page of a BookChapter
     * @param startingPage an integer that represents the first page of the chapter 
     */
    public void setStartingPage(int startingPage) {
        this.startingPage = startingPage;
    }

    /**
     * This method returns the final page of this BookChapter
     * @return an integer that represents the final page of this BookChapter
     */
    public int getFinalPage() {
        return finalPage;
    }

    /**
     * This method alters the value of the final page of this BookChapter
     * @param finalPage an integer that represents the final page of this chapter
     */
    public void setFinalPage(int finalPage) {
        this.finalPage = finalPage;
    }
    
        /**
     * Method that describes the characteristics of a BookChapter. It returns a string in the following format: 
     * <p>
     * Title : __, Summary : __, Type : __
     * <p>
     *  Keywords : __
     * <p>
     *  Authors : __, Publication Year : __
     * <p>
     *  Publisher : __, ISBN : __ 
     * <p>
     *  Chapter name : __, Starting page : __, Final page : __
     * Note: __ is the place where the values are going to appear (they change object to object)
     * 
     * @return a String that describes a BookChapter object
     * 
     */
     @Override
    public String toString() {
        return super.toString()
                + "\n Chapter name : " + chapterName + ", Starting page : " + startingPage + ", Final page : " + finalPage;
    }
    //Note this object publication does not need a impactFactorCalculator() because it would be equal to its Book parent function

}
