/*
Code developed by Edgar Filipe Ferreira Duarte, student number: 2019216077
personal email: edgartip2003@gmail.com
institutional email : edgarduarte@student.dei.uc.pt
DEI 2020-2021
 */
package projetofinalpoo;

import java.util.List;

/**
 * A ConferenceArticle is a child class of a Publication class so it inherits
 * its methods and variables. A ConferenceArticle is represented by its name,
 * its date of launch and the localization where the conference was held and all
 * the variables that define a Publication.
 *
 * @see Publication
 * @author Edgar Duarte
 */
public class ConferenceArticle extends Publication {

    private String name;
    private Date date;
    private String localization;

    /**
     * Sole constructor
     */
    public ConferenceArticle() {
    }

    /**
     * Constructor that initializes the ConferenceArticle and Publications
     * values (title, author, summary, keywords, publicationYear, audience,
     * name, date, localization)
     *
     * @param title name of the title
     * @param author List of Researcher objects that made this ConferenceArticle
     * @param summary description of this ConferenceArticle
     * @param keywords List of Strings that describe this ConferenceArticle
     * @param publicationYear year that this ConferenceArticle was published
     * @param audience quantity of people to which this ConferenceArticle was
     * shown to
     * @param name name of the conference
     * @param date date on which the conference was held
     * @param localization place where the conference was held
     */
    public ConferenceArticle(String title, List<Researcher> author, String summary, List<String> keywords, int publicationYear, int audience, String name, Date date, String localization) {
        super(title, author, summary, keywords, publicationYear, audience);
        this.type = "ConferenceArticle";
        this.name = name;
        this.date = date;
        this.localization = localization;
    }

    /**
     * Returns the current name of this ConferenceArticle object
     *
     * @return name of the conference
     */
    public String getName() {
        return name;
    }

    /**
     * Alters the name of the conference on which this ConferenceArticle is
     * based on
     *
     * @param name new conference name
     */
    public void setName(String name) {
        this.name = name;
    }

    /**
     * Returns the date on which this ConferenceArticle was published
     *
     * @return date on which the conference was held
     */
    public Date getDate() {
        return date;
    }

    /**
     * Alters the date on which this conference was held
     *
     * @param date new date on which the conference was held
     */
    public void setDate(Date date) {
        this.date = date;
    }

    /**
     * Returns the localization where the conference on which this
     * ConferenceArticle was held
     *
     * @return name of the place where the conference was held
     */
    public String getLocalization() {
        return localization;
    }

    /**
     * Alters the name of the localization where this conference was held
     *
     * @param localization name of the place where the conference was held
     */
    public void setLocalization(String localization) {
        this.localization = localization;
    }

    /**
     * This method calculates the impact factor a book type publication has. A
     * ConferenceArticle has a impact factor "A" when its audience is more or
     * equal than 500, has a impact factor of "B" when its audience is less than
     * 500 but more or equal than 200 and has impact factor "C" when its
     * audience is less than 200.
     *
     * @return the impact factor of this ConferenceArticle
     */
    @Override
    public String impactFactorCalculator() {
        if (this.getAudience() >= 500) {
            return "A";
        } else if (this.getAudience() < 500 && this.getAudience() >= 200) {
            return "B";
        } else {
            return "C";
        }
    }

    /**
     * Method that describes the characteristics of a ConferenceArticle. It returns a
     * string in the following format:
     * <p>
     * Title : __, Summary : __, Type : __
     * <p>
     *  Keywords : __
     * <p>
     *  Authors : __, Publication Year : __
     * <p>
     *  Conference name : __, Conference date : __, localization : __
     * <p>
     * Note: __ is the place where the values are going to appear (they change
     * object to object)
     *
     * @return a String that describes a ConferenceArticle object
     *
     */
    @Override
    public String toString() {
        return super.toString()
                + "\n Conference name : " + name + ", Conference date : " + date + ", localization : " + localization;
    }

}
