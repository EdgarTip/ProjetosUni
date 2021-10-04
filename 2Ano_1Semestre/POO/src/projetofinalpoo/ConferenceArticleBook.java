/*
Code developed by Edgar Filipe Ferreira Duarte, student number: 2019216077
personal email: edgartip2003@gmail.com
institutional email : edgarduarte@student.dei.uc.pt
DEI 2020-2021
 */
package projetofinalpoo;

import java.util.List;

/**
 *A ConferenceArticleBook is a child class of the Book class (which is a child class of the Publication class) so it inherits their methods and variables. A BookChapter is represented by its chapter name, 
 * its starting page, its ending page and all the variables that define a Book and a Publication. Besides gets, sets, constructors and toSting() methods it only implements the abstract method
 * impactFactorCalculator().
 * 
 * @see Publication
 * @see Book
 * @author Edgar Duarte
 */
public class ConferenceArticleBook extends Book {

    private String conferenceName;
    private int numberOfArticles;

    /**
     * Sole Constructor
     */
    public ConferenceArticleBook() {
    }

    /**
     * Constructor that initializes the ConferenceArticle, Book and Publications values (title, author, summary, keywords, publicationYear, audience, isbn, publisher, 
     * chapterName, startingPage and finalPage)
     * @param title name of the title
     * @param author List of Researcher objects that made this BookChapter
     * @param summary description of this BookChapter
     * @param keywords List of Strings that describe this BookChapter
     * @param publicationYear year that this BookChapter was published
     * @param audience quantity of people to which this BookChapter was shown to
     * @param isbn the identifying code of a book
     * @param publisher name of the entity that published the Book where this BookChapter is
     * @param conferenceName name of the conference of which this book article is based of
     * @param numberOfArticles number of articles that make up the book
     */
    public ConferenceArticleBook(String title, List<Researcher> author, String summary, List<String> keywords, int publicationYear, int audience, String publisher, String isbn, String conferenceName, int numberOfArticles) {
        super(title, author, summary, keywords, publicationYear, audience, publisher, isbn);
        type = "ConferenceArticleBook";
        this.conferenceName = conferenceName;
        this.numberOfArticles = numberOfArticles;
    }

    /**
     * Returns the current name of the conference the ConferenceArticleBook is based off
     * @return name of the conference
     */
    public String getConferenceName() {
        return conferenceName;
    }

    /**
     * Alters the name of the conference the ConferenceArticleBook is based off
     * @param conferenceName new conference name 
     */
    public void setConferenceName(String conferenceName) {
        this.conferenceName = conferenceName;
    }

    /**
     * Returns the current number of articles the ConferenceArticleBook has
     * @return number of articles of the ConferenceArticleBook
     */
    public int getNumberOfArticles() {
        return numberOfArticles;
    }

    /**
     * Alters the number of articles the ConferenceArticleBook
     * @param numberOfArticles 
     */
    public void setNumberOfArticles(int numberOfArticles) {
        this.numberOfArticles = numberOfArticles;
    }

    @Override
    /**
     *  This method calculates the impact factor a book type publication has. A MagazineArticle has a impact factor "A" when its audience is more or equal than 7500,
     * has a impact factor of "B" when its audience is less than 7500 but more or equal than 2500 and has impact factor "C" when its audience is less than 
     * 2500. 
     * 
     */
    public String impactFactorCalculator() {
        if (this.getAudience() >= 7500) {
            return "A";
        } else if (this.getAudience() < 7500 && this.getAudience() >= 2500) {
            return "B";
        } else {
            return "C";
        }
    }
    
     
    /**
     * Method that describes the characteristics of a ConferenceArticleBook. It returns a string in the following format: 
     * <p>
     * Title : __, Summary : __, Type : __
     * <p>
     *  Keywords : __
     * <p>
     *  Authors : __, Publication Year : __
     * <p>
     *  Publisher : __, ISBN : __ 
     * <p>
     *  Conference name : __, Number of articles : __
     * Note: __ is the place where the values are going to appear (they change object to object)
     * 
     * @return a String that describes a ConferenceArticleBook object
     * 
     */
     @Override
    public String toString() {
        return super.toString()
                + "\n Conference name : " + conferenceName + ", Number of articles : " + numberOfArticles;
    }
}
