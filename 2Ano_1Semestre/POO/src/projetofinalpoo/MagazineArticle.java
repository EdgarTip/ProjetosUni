    /*
Code developed by Edgar Filipe Ferreira Duarte, student number: 2019216077
personal email: edgartip2003@gmail.com
institutional email : edgarduarte@student.dei.uc.pt
DEI 2020-2021
 */
package projetofinalpoo;

import java.util.List;

/**
 * A MagazineArticle is a child class of a Publication class so it inherits its methods and variables. A ConferenceArticle is represented by its name, its date of launch, its
 * magazine number and all the variables that define a Publication.
 * 
 * @see Publication
 * @author Edgar Duarte
 */
public class MagazineArticle extends Publication {

    private String name;
    private Date date;
    private int magazineNumber;

    /**
     * Sole constructor
     */
    public MagazineArticle(){}

     /**
     * Constructor that initializes the MagazineArticle and Publications values (title, author, summary, keywords, publicationYear, audience, name, date, magazineNumber)
     * @param title name of the title
     * @param author List of Researcher objects that made this MagazineArticle
     * @param summary description of this MagazineArticle
     * @param keywords List of Strings that describe this MagazineArticle
     * @param publicationYear year that this MagazineArticle was published
     * @param audience quantity of people to which this MagazineArticle was shown to
     * @param name name of the magazine where this MagazineArticle was written
     * @param date date of launch of this MagazineArticle
     * @param magazineNumber number of the magazine where this MagazineArticle was written
     * 
     * @see Publication
     */
    public MagazineArticle(String title, List<Researcher> author, String summary, List<String> keywords, int publicationYear, int audience, String name, Date date, int magazineNumber) {
        super(title, author, summary, keywords, publicationYear, audience);
        this.type = "MagazineArticle";
        this.name = name;
        this.date = date;
        this.magazineNumber = magazineNumber;
    }

     /**
     * Returns the current name of the magazine where this MagazineArticle was published
     * @return name of the magazine
     */
    public String getName() {
        return name;
    }

    /**
     * Alters the name of the magazine on which this MagazineArticle was published
     * @param name new magazine name
     */
    public void setName(String name) {
        this.name = name;
    }

    /**
    * Returns the date on which this MagazineArticle was published
    * @return date on which the magazine was held
    */
    public Date getDate() {
        return date;
    }

     /**
     * Alters the  date on which this magazine was released
     * @param date new date on which the magazine was released
     */
    public void setDate(Date date) {
        this.date = date;
    }

    /**
    * Returns the number of the magazine on which this MagazineArticle was published
    * @return number of the magazine
    */
    public int getMagazineNumber() {
        return magazineNumber;
    }

     /**
     * Alters the number of the magazine on which this MagazineArticle was published
     * @param magazineNumber new magazine number
     */
    public void setMagazineNumber(int magazineNumber) {
        this.magazineNumber = magazineNumber;
    }

    /**
     * This method calculates the impact factor a book type publication has. A MagazineArticle has a impact factor "A" when its audience is more or equal than 1000,
     * has a impact factor of "B" when its audience is less than 1000 but more or equal than 500 and has impact factor "C" when its audience is less than 
     * 500. 
     * 
     * @return the impact factor of this MagazineArticle
     */
    public String impactFactorCalculator() {
        if (this.getAudience() >= 1000) {
            return "A";
        } else if (this.getAudience() < 1000 && this.getAudience() >= 500) {
            return "B";
        } else {
            return "C";
        }
    }
      
     /**
     * Method that describes the characteristics of a MagazineArticle. It returns a
     * string in the following format:
     * <p>
     * Title : __, Summary : __, Type : __
     * <p>
     *  Keywords : __
     * <p>
     *  Authors : __, Publication Year : __
     * <p>
     *  Magazine name : __, Magazine date : __, Magazine number : __
     * <p>
     * Note: __ is the place where the values are going to appear (they change
     * object to object)
     *
     * @return a String that describes a MagazineArticle object
     *
     */
    @Override
    public String toString() {
        return super.toString()
                + "\n Magazine name : " + name + ", Magazine date : " + date + ", magazine number : " + magazineNumber;
    }

}
