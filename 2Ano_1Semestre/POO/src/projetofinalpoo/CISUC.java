/*
Code developed by Edgar Filipe Ferreira Duarte, student number: 2019216077
personal email: edgartip2003@gmail.com
institutional email : edgarduarte@student.dei.uc.pt
DEI 2020-2021
 */
package projetofinalpoo;

import java.io.*;
import java.util.*;

/**
 * A CISUC class represents an institution that contains research groups,
 * researchers and publications. A CISUC is defined by an researchGroupList,
 * researcherList and a publicationsList.
 * <p>
 *
 * A researchGroupList represents all the main research groups that a CISUC is
 * composed of. It is occupied solely with ResearchGroup objects.<p>
 * A researcherList represents all the researchers in a CISUC, independent of
 * their research group. It is occupied with <ul>
 * <li> Students objects
 * <li> EffectiveMember objects
 * <p>
 * </ul>
 * A publicationsList represents all the publications made. It is occupied with
 * <ul>
 * <li> Book objects
 * <li> BookChapeter objects
 * <li> ConferenceArticleBook objects
 * <li> ConferenceArticle objects
 * <li> MagazineArticle objects
 * <p>
 * </ul>
 *
 * The main objective of a CISUC object is to connect Researchers, ResearchGroup
 * and Publication objects in one object, simplifying code and saving execution
 * time. A CISUC object can do the following operations:<ul>
 * <li> return the researcherList, publicationList or researchGroupList;
 * <li> add a Researcher to the researcherList;
 * <li> add a ResearchGroup to the researchGroupList;
 * <li> add a Publication to the publicationList;
 * <li> find the responsible Researcher of a Student;
 * <li> find a specific Researcher;
 * <li> find a specific ResearchGroup;
 * <li> show general information of the CISUC object;
 * <li> show all the ResearchGroup in the researchGroupList on the console;
 * <li> show all the Researcher in the researcherList on the console;
 * <li> show the number of ResearcherGroup objects, in the researhGroupList,
 * grouped by types on the console;
 * <li> show the number of of Researcher objects, in the researcherList, grouped
 * by types on the console;
 * <li> return all the publications of a specific ResearchGroup;
 * <li> return all the publications of a specific Researcher;
 * <li> show all the publications of a specific publicationList (doesn't need to
 * the publicationList of the CISUC object), ordered by year of publication,
 * type of publication and impact factor
 * <li> return a list with only the Publication objects of the last 5 years,
 * from a specific date.
 * <li> show information on the console about all the ResearchGroup objects
 * <p>
 * </ul>
 *
 * A CISUC is serializable, meaning that it can be saved in an object file.
 *
 * @see Publication
 * @see Researcher
 * @see ResearchGroup
 *
 * @author Edgar Duarte
 */
public class CISUC implements Serializable {

    private List<ResearchGroup> researchGroupsList;
    private List<Researcher> researcherList;
    private List<Publication> publicationsList;

    /**
     * Constructors Sole constructor that initializes the researchGroupList, the
     * researcherList and the publicationList
     *
     */
    public CISUC() {
        researchGroupsList = new ArrayList<>();
        researcherList = new ArrayList<>();
        publicationsList = new ArrayList<>();

    }

    /**
     * Returns a List of ResearchGroup objects
     *
     * @return the list of ResearchGroup objects
     */
    public List<ResearchGroup> getResearchGroupsList() {
        return researchGroupsList;
    }

    /**
     * Returns a List of Publication objects
     *
     * @return the list of Publication objects
     */
    public List<Publication> getPublicationsList() {
        return publicationsList;
    }

    /**
     * Returns a List of Researcher objects
     *
     * @return the list of Researcher objects
     */
    public List<Researcher> getResearcherList() {
        return researcherList;
    }

    /**
     * Adds a ResearchGroup object to the researchGroupsList
     *
     * @param researchGroup the ResearchGroup object that will be added to the
     * investigationGroupList
     */
    public void addResearchGroup(ResearchGroup researchGroup) {
        researchGroupsList.add(researchGroup);
    }

    /**
     * Adds a Researcher object to the researcherList
     *
     * @param researcher the Researcher object that will be added to the
     * researcherList
     */
    public void addResearcher(Researcher researcher) {
        researcherList.add(researcher);
    }

    /**
     * Adds a Publication object to the publicationsList
     *
     * @param publication the Publication object that will be added to the
     * publicationsList
     */
    public void addPublication(Publication publication) {
        publicationsList.add(publication);
    }

    /**
     *
     * This method receives as parameter a name of a Researcher and a
     * ResearchGroup. The method looks in the researcherList for a Researcher
     * with a matching name as the given one and, if found, it sees if the
     * Researcher found is in the same ResearchGroup. If there is any Researcher
     * that meets these 2 criteria, then that Researcher is returned. If there
     * is no matching Researcher the method return null to indicate nothing has
     * been found.
     *
     * @param name name of the Researcher to search for
     * @param resGroup ResearchGroup that checks, when a Researcher with the
     * same name as the one given is found, if that Researcher is in the correct
     * ResearchGroup
     * @return a Researcher if one that fits the criteria is found or null when
     * no Researcher fits the criteria
     *
     * @see Researcher
     */
    public Researcher findResponsibleResearcher(String name, ResearchGroup resGroup) {
        for (Researcher res : researcherList) {
            if (name.toLowerCase().equals(res.getName().toLowerCase()) && resGroup == res.getResearchGroup()) {
                return res;
            }
        }
        return null;
    }

    /**
     *
     * This method receives as parameter a name of a ResearchGroup. The method
     * looks in the researchGroupList for a ResearcherGroup with a matching name
     * as the given one and, if found, it returns the ResearchGroup. If there is
     * no ResearchGroup that meets this criteria the method returns null to
     * indicate nothing has been found.
     *
     * @param name name of the ResearchGroup to search for
     * @return a ResearchGroup if one that fits the criteria is found or null
     * when no ResearchGroup fits the criteria
     *
     * @see ResearchGroup
     */
    public ResearchGroup findResearchGroup(String name) {
        for (ResearchGroup resGroup : researchGroupsList) {
            if (name.toLowerCase().equals(resGroup.getName().toLowerCase())) {
                return resGroup;
            }
        }
        //Means the group or doesn't exist or has another name
        return null;
    }

    /**
     *
     * This method receives as parameter a name of a Researcher. The method
     * looks in the researcherGroupList for a Researcher with a matching name as
     * the given one and, if found, it returns the Researcher. If there is no
     * Researcher that meets this criteria the method returns null to indicate
     * nothing has been found.
     *
     * @param name name of the Researcher to search for
     * @return a Researcher if one that fits the criteria is found or null when
     * no Researcher fits the criteria
     *
     * @see Researcher
     */
    public Researcher findResearcher(String name) {
        for (Researcher res : researcherList) {
            if (name.toLowerCase().equals(res.getName().toLowerCase())) {
                return res;
            }
        }
        return null;
    }

    /**
     * This method shows the basic information of this CISUC object. It shows,
     * on the console, the number of Researchers that are in the
     * researchersList, the number of researchers by category (EffectiveMembers
     * and Students), the number of publications released in the last 5 years
     * and the number of publications by category (Book, ConferenceArticleBook,
     * BookChapter, ConferenceArticle, MagazineArticle).
     *
     */
    public void showCISUCInformation() {
        numberOfResearches();
        numberOfResearchersByCategory();
        numberOfPublicationsLastFiveYears(2020);
        numberOfPublicationsByCategory();
    }

    /**
     * This method shows on the console all the ResearchGroup objects there are
     * in this CISUC. It is shown on the console in the following format :
     * <p>
     * <p>
     *
     * ResearchGroup 1 | ResearchGroup 2 | ... | ResearchGroup n-1 |
     * ResearchGroup n , being n the amount of ResearchGroup objects in this
     * CISUC
     *
     */
    public void showAllResearchGroups() {
        System.out.println("The existing investigation groups are: ");
        boolean firstTime = true;
        for (ResearchGroup resGroup : researchGroupsList) {
            if (firstTime) {
                System.out.print(resGroup.getName());
                firstTime = false;
            } else {
                System.out.print(" | " + resGroup.getName());
            }
        }
        System.out.println();
    }

    /**
     * This method shows on the console all the Researcher objects there are in
     * this CISUC. Due to there, possibly, being a lot of Researcher objects in
     * a CISUC, every 8 Researcher objects a new line is inserted, so that there
     * isn't just 1 line that extends a long distance, making it harder to find
     * the information that we want. It is shown on the console i the following
     * format:
     * <p>
     * <p>
     *
     * Researcher 1| Researcher 2| ... | Researcher 7 | Researcher 8
     * <p>
     * Researcher 9| Researcher 10| ... | Researcher 15 | Researcher 16
     * <p>
     * ... | Researcher n
     * <p>
     * ,assuming that this CISUC has more than 16 researcher (not needed).
     */
    public void showAllResearchers() {
        System.out.println("The existing investigators are: ");
        boolean firstTime = true;
        int counter = 0;

        for (Researcher res : researcherList) {

            if (firstTime) {
                System.out.print(res.getName());
                firstTime = false;
                counter++;
            } else {
                System.out.print(" | " + res.getName());
                counter++;
            }
            if (counter == 8) {
                System.out.println();
                counter = 0;
            }
        }
        System.out.println();
    }

    /**
     * This method returns a List of Publications from a ResearchGroup given as parameter. The goes through all the publications
     * in the publicationList of the CISUC and looks for authors whose ResearchGroup is the same as the ResearchGroup given as parameter.
     * If there is a match the publication is added to the publicationList and skips to the next publication,
     * if there is no match it goes to the next publication to repeat the cycle, until there are no more publication in the publicationList.
     * @param resGroup ResearchGroup we want to find publications from
     * @return a List of Publication objects that are the publications of the given as parameter ResearchGroup
     */
    public List<Publication> getPublicationsResearchGroup(ResearchGroup resGroup) {

        List<Publication> publicationList = new ArrayList();

        //Go through all the publications in the database
        for (Publication pub : this.getPublicationsList()) {
            //Go through all the reserchers of each publication
            for (Researcher res : pub.getAuthor()) {
                //See if a there is a matching research group, if there is add the publication to the list and look for the next publication
                if (res.getResearchGroup() == resGroup) {
                    publicationList.add(pub);
                    break;
                }
            }
        }

        return publicationList;
    }

    /**
     * This method returns a list with all the publications of a researcher. The method goes through all the publications in the CISUC and the author
     * of a publication contains a specif Researcher (given as parameter) it adds that Publication to the final publicationList.
     * @param res Researcher whose publications we want
     * @return a Publication objects List, that represent the publications of the Researcher given as parameter
     */
    public List<Publication> getPublicationsResearcher(Researcher res) {

        List<Publication> publicationList = new ArrayList();

        for (Publication pub : this.getPublicationsList()) {
            for (Researcher pubRes : pub.getAuthor()) {
                if (res == pubRes) {
                    publicationList.add(pub);
                    break;
                }
            }
        }

        return publicationList;
    }

    /**
     * This method shows on the console all the publications in a given
     * Publication objects List, ordered by publication year (decreasing order), type of
     * publication and impact factor. The console message is in the following
     * format:
     * <p>
     * (Note this is just an example)
     * <p>
     * ----- Publications list -----
     * <p>
     * <p>
     * --- Year n ---
     * <p>
     * | | Type of publication
     * <p>
     * Factor A:
     * <p>
     * - Publication 1;
     * <p>
     * Factor C:
     * <p>
     * - Publication 32;
     * <p>
     * <p>
     * --- Year y ---
     * <p>
     * (...)
     *<p>
     * ,assuming that y is less then n and  that we only have 1 publication with factor A and 1 publication with factor C in year n.<p>
     * The method uses the function compareTo from the Publication object to order the publicationList in a decreasing order. After having 
     * the initial list ordered, the method sees if it is in the right folder (a folder the area covered by a specific year, publication type
     * and impact factor). If it is in the right folder we can just write another element in that folder, if it is not then the method creates
     * a new folder/s, depending on which element is different (if the year is different then it creates a new year, type of publication and factor
     * impact folder; if the type of publication is different but the year is the same the method creates a new type of publication folder and a new 
     * impact factor folder; if only the impact factor is different, then the method creates only a new impact factor folder). To write a Publication,
     * the method uses the .toString() method of the Publication object. After it has written all of the publication on the console, the method signalizes
     * the end by printing on the console 29 times "-".
     *
     * @param publicationsList list with the Publication objects that will be shown on the console
     * 
     * @see Publication
     */
    public void readPublicationList(List<Publication> publicationsList) {

        int previousYear = 0;
        String previousType = "";
        String previousFactor = "";

        System.out.println("----- Publications list -----");
        Collections.sort(publicationsList);

        for (Publication pub : publicationsList) {
            //Means we are in the right folder of year
            if (pub.getPublicationYear() == previousYear) {
                //The type of publication is the same as the previous meaning we dont have to create a new folder
                if (pub.getType().equals(previousType)) {
                    if (pub.impactFactorCalculator().equals(previousFactor)) {
                        System.out.println("-" + pub.toString());
                        System.out.println();
                    } else {
                        System.out.println("Factor " + pub.impactFactorCalculator() + ":");
                        System.out.println("-" + pub.toString());
                        System.out.println();
                        previousFactor = pub.impactFactorCalculator();
                    }
                } //The type of publication is different from the previous meaning we have to create a new folder
                else {
                    System.out.println("| |" + pub.getType() + ":");
                    System.out.println("Factor " + pub.impactFactorCalculator() + ":");
                    System.out.println("-" + pub.toString());
                    System.out.println();
                    //Update the type
                    previousType = pub.getType();

                    previousFactor = pub.impactFactorCalculator();

                }
            } //Means we are in a diferent year, meaning we have to create a new folder
            else {
                System.out.println();
                System.out.println("--- " + pub.getPublicationYear() + " ---");
                System.out.println("| |" + pub.getType() + ":");
                System.out.println("Factor " + pub.impactFactorCalculator() + ":");
                System.out.println("-" + pub.toString());
                System.out.println();
                previousType = pub.getType();
                previousYear = pub.getPublicationYear();
                previousFactor = pub.impactFactorCalculator();
            }
        }
        System.out.println("-----------------------------");
    }
    
    /*
    Shows the number of publications ordered by year, type and factor of impact. 
    */
    private void readNumberOfPublications(List<Publication> publicationsList){
        
        int counter = 0; 
        int previousYear = 0; 
        String previousType = "";
        String previousFactor = "";

        System.out.println("----- Number of Publications by Year -----");
        Collections.sort(publicationsList);

        for (Publication pub : publicationsList) {
            //Means we are in the right folder of year
            if (pub.getPublicationYear() == previousYear) {
                //The type of publication is the same as the previous meaning we dont have to create a new folder
                if (pub.getType().equals(previousType)) {
                    //It is the same impact factor meaning we are in the same factor has before, so we increment the counter
                    if (pub.impactFactorCalculator().equals(previousFactor)) {
                        counter++;
                    } else {
                        System.out.println("-" + counter);
                        System.out.println("Factor " + pub.impactFactorCalculator() + ":");
                        counter = 1;
                        previousFactor = pub.impactFactorCalculator();
                    }
                } //The type of publication is different from the previous meaning we have to create a new folder
                else {
                    System.out.println("-" + counter);
                    System.out.println("||" + pub.getType() + ":");
                    System.out.println("Factor " + pub.impactFactorCalculator() + ":");
                    counter = 1;
                    //Update the type
                    previousType = pub.getType();
                    previousFactor = pub.impactFactorCalculator();

                }
            } //Means we are in a diferent year, meaning we have to create a new folder
            else {
                if(counter != 0) System.out.println("-" + counter);
                System.out.println();
                System.out.println("--- " + pub.getPublicationYear() + " ---");
                System.out.println("||" + pub.getType() + ":");
                System.out.println("Factor " + pub.impactFactorCalculator() + ":");
                counter = 1;
                previousType = pub.getType();
                previousYear = pub.getPublicationYear();
                previousFactor = pub.impactFactorCalculator();
            }
        }
        if(counter != 0) System.out.println("-" + counter);
        System.out.println("-----------------------------");
    }
    
    /**
     * This method receives a Publication objects List and return a Publication objects List that has the Publications of the 
     * list as parameter which year of publication is only, at maximum, 5 years apart from a given date (given as parameter).
     * This goes through all the Publication objects and looks if the subtraction of the given year and the objects publication year
     * is at maximum 5. If it is, add the object to the final Publications List, if not, ignores the object and goes to the next object. 
     * 
     * @param publicationList list of Publication objects to look for objects that fit the criteria above described
     * @param currentYear year from which the publications can differ 5 years from 
     * @return a List of Publication objects that differ, at maximum, 5 years from the input year
     */
    public List<Publication> publicationsOfTheLast5Years(List<Publication> publicationList, int currentYear) {
        List<Publication> last5YearList = new ArrayList();
        for (Publication pub : publicationList) {
            if (currentYear - pub.getPublicationYear() <= 5) {
                last5YearList.add(pub);
            }
        }
        return last5YearList;

    }
    /**
     * This method shows information about all of the ResearchGroups on at a time. It shows on the console:<ul>
     * <li> the number of researchers in the ResearchGroup;
     * <li> the number of researcher by type in the ResearchGroup;
     * <li> the number of publication in the last 5 years;
     * <li> the number of publications in the last 5 years, ordered by year, type of publication and factor of impact. 
     * </ul>
     * The last feature will show on the console in the following format (example): 
     * <p>
     * ----- Number of Publications by Year -----
     * <p>
     * --- 2020 ---
     * <p>
     * || Book
     * <p>
     * Factor A:
     * <p>
     * -2
     * <p>
     * Factor C:
     * <p>
     * -1
     * <p>
     * --- 2019 ---
     * <p>
     * (...)
     * <p>
     * ,assuming that 2020 has 3 Book objects of which 2 are of factor A and 1 is of factor C.
     * <p>
     * This method will show all of the features above for all of the ResearchGroups in the researchGroupList in the CISUC object.
     * 
     */
    public void showResearchGroupsInformation() {
        for (ResearchGroup resGroup : researchGroupsList) {
            System.out.println("---" + resGroup.getName() + "---");
            System.out.println("There are " + resGroup.getResearcherList().size() + " members.");
            numberOfResearchersInResearchGroup(resGroup);

            List<Publication> publicationList = getPublicationsResearchGroup(resGroup);
            List<Publication> last5YearsPublicationList = publicationsOfTheLast5Years(publicationList, 2020);

            System.out.println("In the last 5 years there have been " + last5YearsPublicationList.size() + " publications.");
            
            readNumberOfPublications(last5YearsPublicationList);
            System.out.println();

        }
    }
    /*
    Shows the number of researcher by class (effective member and student) on the console.
    */
    private void numberOfResearchersInResearchGroup(ResearchGroup resGroup) {
        int counterStudents = 0, counterEffectiveMembers = 0;
        for (Researcher res : resGroup.getResearcherList()) {
            if (res.getType().equals("effective member")) {
                counterEffectiveMembers++;
            } else if (res.getType().equals("student")) {
                counterStudents++;
            }
        }
        System.out.println("There are " + counterStudents + " students and " + counterEffectiveMembers + " effective members.");
    }

    /*
    Shows the number of researchers on the console.
    */
    private void numberOfResearches() {
        System.out.println("There are " + researcherList.size() + " researchers.");
    }

    /*Shows the amount of publications made in the last 5 years in this CISUC object to the console
    */
    private void numberOfPublicationsLastFiveYears(int currentYear) {
        int counter = 0;

        for (Publication pub : publicationsList) {
            if (currentYear - pub.getPublicationYear() <= 5) {
                counter++;
            }
        }

        System.out.println("There where " + counter + " publications in the last 5 years");
    }

    /*
     This method counts the amount of researchers of each type that are in the researcherList. It creates a counter for EffectiveMembers and Students (both
     are child objects of Researcher). The method goes through the entire researcherList and sees what type they are, incrementing a respective counter when a 
     type is found. After counting, it shows the resutls on the console.
     */
    private void numberOfResearchersByCategory() {
        int counterStudents = 0, counterEffectiveMembers = 0;
        for (Researcher res : researcherList) {
            if (res.getType().equals("effective member")) {
                counterEffectiveMembers++;
            } else if (res.getType().equals("student")) {
                counterStudents++;
            }
        }
        System.out.println("There are " + counterStudents + " students and " + counterEffectiveMembers + " effective members.");
    }


    /*This method counts the amount of publications of each type that are in the researchGroupList. It creates a counter for each of the Publication child.
    The method goes through the entire publicationsList and sees what type they are, incrementing a respective counter when a 
     type is found. After counting, it shows the resutls on the console.*/
    private void numberOfPublicationsByCategory() {
        int counterBooks = 0, counterConferenceArticle = 0, counterMagazineArticle = 0, counterBookChapters = 0, counterConferenceArticleBook = 0;

        for (Publication pub : publicationsList) {
            switch (pub.getType()) {
                case "Book":
                    counterBooks++;
                    break;

                case "ConferenceArticle":
                    counterConferenceArticle++;
                    break;

                case "MagazineArticle":
                    counterMagazineArticle++;
                    break;

                case "BookChapter":
                    counterBookChapters++;
                    break;

                case "ConferenceArticleBook":
                    counterConferenceArticleBook++;
                    break;

                default:
                    System.out.println("There is an error in the code! Please notify the creator!");
            }
        }
        System.out.println("There are " + counterBooks + " books.");
        System.out.println("There are " + counterBookChapters + " book chapters.");
        System.out.println("There are " + counterConferenceArticleBook + " conference article books.");
        System.out.println("There are " + counterConferenceArticle + " conference articles.");
        System.out.println("There are " + counterMagazineArticle + " magazine articles.");
    }

}
