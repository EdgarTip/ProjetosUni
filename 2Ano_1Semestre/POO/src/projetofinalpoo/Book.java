/*
Code developed by Edgar Filipe Ferreira Duarte, student number: 2019216077
personal email: edgartip2003@gmail.com
institutional email : edgarduarte@student.dei.uc.pt
DEI 2020-2021
 */
package projetofinalpoo;

import java.util.List;

/**
 * A Book is a child object of a Publication so it inherits its methods and variables. A Book is represented by its publisher and its ISBN and all the values inherited from the Publication 
 * class. A ISBN is a code that identifies a book. A publisher is the entity that publishes the book.
 * 
 * @see Publication
 * 
 * @author Edgar Duarte
 */
public class Book extends Publication{
    protected String publisher;
    protected String isbn;
    
    /**
     * Sole Constructor
     */
    public Book(){}
    
    /**
     * Constructor that initializes the Book and Publications value (title, author, summary, keywords, publicationYear, audience, publisher and isbn)
     * @param title name of the title
     * @param author List of Researcher objects that made this Book
     * @param summary description of this Book
     * @param keywords List of Strings that describe this Book
     * @param publicationYear year that this Book was published
     * @param audience quantity of people to which this Book was shown to
     * @param publisher name of the publisher of the book, the entity that published the book
     * @param isbn the identifying code of a book
     * 
     * @see Publication
     */
    public Book( String title, List<Researcher> author, String summary, List<String> keywords, int publicationYear, int audience, String publisher, String isbn) {
        super(title, author, summary, keywords, publicationYear, audience);
        this.type = "Book";
        this.publisher = publisher;
        this.isbn = isbn;
    }
    
    /**
     * This method returns the current publisher of this Book object
     * @return name of the publisher who published this Book
     */
    public String getPublisher() {
        return publisher;
    }

    /**
     * This method alters the name of this Book object's publisher. A publisher is the entity that published the book to the public.
     * @param publisher 
     */
    public void setPublisher(String publisher) {
        this.publisher = publisher;
    }

    /**
     * This method returns the current value of the ISBN of this book.
     * @return a String that represents the Book's ISBN, a code that identifies the Book object.
     */
    public String getIsbn() {
        return isbn;
    }

    
    /**
     * This method alters the code of the ISBN of this Book object. A ISBN is a String that identifies a book.
     * @param isbn String that will be the book's new ISBN
     */
    public void setIsbn(String isbn) {
        this.isbn = isbn;
    }
    
    @Override
    /**
     * This method calculates the impact factor a book type publication has. A Book has a impact factor "A" when its audience is more or equal than 10000,
     * has a impact factor of "B" when its audience is less than 10000 but more or equal than 5000 and has impact factor "C" when its audience is less than 
     * 5000. 
     * 
     * @return a String indicating the impact factor of this book object.
     */
    public String impactFactorCalculator(){
        if(this.getAudience() >= 10000) return "A";
        else if( this.getAudience() < 10000 && this.getAudience() >= 5000) return "B";
        else return "C";
    }

     
    /**
     * Method that describes the characteristics of a Book. It returns a string in the following format: 
     * <p>
     * Title : __, Summary : __, Type : __
     * <p>
     *  Keywords : __
     * <p>
     *  Authors : __, Publication Year : __
     * <p>
     *  Publisher : __, ISBN : __ 
     * <p>
     * Note: __ is the place where the values are going to appear (they change object to object)
     * 
     * @return a String that describes a Book object
     * 
     */
     @Override
    public String toString() {
        return super.toString() + "\n Publisher : " +  publisher + ", ISBN : " + isbn;
    }
    
    
    
}
