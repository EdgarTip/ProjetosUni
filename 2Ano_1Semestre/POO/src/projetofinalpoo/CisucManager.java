/*
Code developed by Edgar Filipe Ferreira Duarte, student number: 2019216077
personal email: edgartip2003@gmail.com
institutional email : edgarduarte@student.dei.uc.pt
2020-2021
 */
package projetofinalpoo;

import java.util.*;
import java.io.*;
import static java.lang.Integer.parseInt;

/**
 * A CisucManager class is where the program starts. In this class there are the read and write file methods, the menu method that allows the user to input whatever to operate the 
 * program and all the methods necessary for the functionalities in the menu to work.
 * 
 * @author Edgar Duarte
 * @see CISUC
 */
public class CisucManager {

    private CISUC cisuc;

    /**
     * Sole Constructor
     */
    public CisucManager() {
        cisuc = new CISUC();
    }

    /*
    This method reads a binary file that contains a CISUC object. If the binary file does not exist it looks for 3 text files that will create a CISUC object
    */
    private void readFile(String binaryFileName, String researcherTextFileName, String researchGroupTextFileName, String publicationsTextFileName) {

        FileInputStream fis = null;
        ObjectInputStream ois = null;
        try {
            fis = new FileInputStream(new File(binaryFileName));
            ois = new ObjectInputStream(fis);

            //Read the CISUC object
            this.cisuc = (CISUC) ois.readObject();

            fis.close();
            ois.close();

            //If the binary file does not exist
        } catch (FileNotFoundException e) {
            readTextFileResearchGroup(researchGroupTextFileName);
            readResearcherTextFile(researcherTextFileName);
            readPublicationsTextFile(publicationsTextFileName);

            //Write the cisuc object to a object file now
            try {
                FileOutputStream fos = new FileOutputStream(new File(binaryFileName));
                ObjectOutputStream oos = new ObjectOutputStream(fos);

                oos.writeObject(cisuc);

                fos.close();
                oos.close();

            } catch (IOException ex) {
                System.out.println("There was a problem writing the object file! Program aborted!");
                ex.printStackTrace();
                System.exit(1);
            }

        } catch (ClassNotFoundException e) {
            System.out.println("---Wrong data in the binary file---");
        } catch (IOException e) {
            System.out.println("---Something went wrong while reading the binary file---");
        } finally {
            try {
                if (fis != null) {
                    fis.close();
                }
                if (ois != null) {
                    ois.close();
                }
            } catch (IOException e) {
                System.out.println("---There was a error closing the binary file ---");
            }
        }
    }

    /*
    This method reads a text file that contains information about the reserch Groups
    */
    private void readTextFileResearchGroup(String fileName) {
        try {
            File f2 = new File(fileName);
            FileReader fs = new FileReader(f2);
            BufferedReader br = new BufferedReader(fs);


            
            String s = br.readLine();
            int i = 0;
            //
            while (s != null) {

                try {
                    String dataArray[] = s.split("#");
                    ResearchGroup resGroup = new ResearchGroup(dataArray[0].trim(), dataArray[1].trim());
                    cisuc.addResearchGroup(resGroup);
                    i++;
                    s = br.readLine();
                } catch (IndexOutOfBoundsException ex) {
                    System.out.println("--- CAUTION ---");
                    System.out.println("--- There is missing data on line, " + i + " , of the reasearcherGroup file! ---");
                }

            }
            fs.close();
            br.close();

        } catch (FileNotFoundException ex) {
            //File does not exist
            System.out.println("--- Researcher Groups text file doesn't exist ---");
            System.exit(1);
        } catch (IOException ex) {
            //Error while reading / closing the file
            System.out.println("--- Something went wrong ---");
            System.exit(1);
        }

    }
    /* 
    This method reads the Researcher Text File, creating the reaseracherList of the CISUC object
    */
    private void readResearcherTextFile(String fileName) {
        try {
            File f1 = new File(fileName);
            FileReader fs = new FileReader(f1);
            BufferedReader br = new BufferedReader(fs);

            //The file exists
            String s = br.readLine();
            int i = 1;
            //Reads all of the lines
            while (s != null) {
                String[] dataArray = s.split("#");

                try {
                    ResearchGroup resGroup = cisuc.findResearchGroup(dataArray[3].trim());
                    if (resGroup != null) {
                        //Looks for a match of a type of researcher
                        switch (dataArray[0].toUpperCase().trim()) {
                            //Creates a effective Member that is a Leader
                            case "LEADER":
                                EffectiveMember newLeaderResearcher = new EffectiveMember(dataArray[1].trim(), dataArray[2].trim(), resGroup, parseInt(dataArray[4].trim()),
                                        parseInt(dataArray[5].trim()), true);
                                cisuc.addResearcher(newLeaderResearcher);
                                resGroup.addResearcher(newLeaderResearcher);
                                if (resGroup.getLeadResearcher() == null) {
                                    resGroup.setLeadResearcher(newLeaderResearcher);
                                } else {
                                    System.out.println("Carefull! There is more than one researcher assigned as the lead role in the same research group! Only the first one "
                                            + "will appear as this group's lead researcher. The other ones will become regular EffectiveMembers");
                                    newLeaderResearcher.setIsLeader(false);
                                }

                                break;
                              //Creates a effective Member
                            case "EFFECTIVE":
                                Researcher newEffectiveMemberResearcher = new EffectiveMember(dataArray[1].trim(), dataArray[2].trim(), resGroup, parseInt(dataArray[4].trim()),
                                        parseInt(dataArray[5].trim()), false);
                                resGroup.addResearcher(newEffectiveMemberResearcher);
                                cisuc.addResearcher(newEffectiveMemberResearcher);
                                break;

                               //Creates a Student
                            case "STUDENT":

                                String dateArray[] = dataArray[5].split("/");

                                Date date = new Date(parseInt(dateArray[0].trim()), parseInt(dateArray[1].trim()), parseInt(dateArray[2].trim()));

                                Researcher responsibleResearcher = cisuc.findResponsibleResearcher(dataArray[6].trim(), resGroup);

                                if (responsibleResearcher != null) {
                                    Researcher newStudentResearcher = new Student(dataArray[1].trim(), dataArray[2].trim(), resGroup, dataArray[4].trim(), date, responsibleResearcher);
                                    resGroup.addResearcher(newStudentResearcher);
                                    cisuc.addResearcher(newStudentResearcher);
                                } else {
                                    System.out.println("The researcher that guides student " + dataArray[1] + ", line " + i + ", of given file does not exist. Please check if "
                                            + "the order in the file is correct, if the data given is correct or add the given researcher to the file!");
                                }
                                break;

                                //Invalid type of researcher given
                            default:
                                System.out.println("The type of researcher is not valid on line " + i);

                        }
                    } else {
                        //Research group doesn't exist
                        System.out.println("--CAUTION---");
                        System.out.println("In line " + i + ", of the chosen file, the researcher group  does not exist!");
                    }
                } catch (NumberFormatException ex) {
                    System.out.println("--CAUTION--");
                    System.out.println("In line" + i + ", of the chosen file, there are invalid values (should be integer)!");
                } catch (IndexOutOfBoundsException ex) {
                    System.out.println("--CAUTION--");
                    System.out.println("In line " + i + ", of the researcher file, there are missing elements");
                }

                i++;
                s = br.readLine();
            }
            fs.close();
            br.close();
        } catch (FileNotFoundException ex) {
            System.out.println("Reasercher text file does not exist!");
            System.exit(1);
        } catch (IOException ex) {
            System.out.println("Something went wrong");
            System.exit(1);
        }
    }
    /*
    Reads the publication text file creatign the publication List of the CISUC object
    */
    private void readPublicationsTextFile(String fileName) {
        try {
            //Open files
            File f1 = new File(fileName);
            FileReader fs = new FileReader(f1);
            BufferedReader br = new BufferedReader(fs);

            String s = br.readLine();
            int i = 1;
            //Read the whole file
            while (s != null) {
                //get all the data
                String dataArray[] = s.split("#");

                try {
                    String authorArray[] = dataArray[2].split(",");
                    List<Researcher> authorList = new ArrayList();

                    //Get all the authors
                    for (int j = 0; j < authorArray.length; j++) {

                        Researcher author = cisuc.findResearcher(authorArray[j].trim());

                        //The author does not exist
                        if (author == null) {
                            System.out.println("In line " + i + " of the publications text file there is a author that does not exist in the data base");
                        } else {
                            authorList.add(author);
                        }
                    }

                    //Get the keywords
                    String arrayKeywords[] = dataArray[4].split(",");
                    List<String> keywords = new ArrayList();
                    //Trim all the words
                    for (int j = 0; j < arrayKeywords.length; j++) {
                        keywords.add(arrayKeywords[j].trim());
                    }

                    if (authorList.size() > 0) {
                        //Choose the type of book acording to the file and create the corresponding object and add it to the cisuc object
                        switch (dataArray[0].toUpperCase().trim()) {
                            case "BOOK":
                                Book book = new Book(dataArray[1].trim(), authorList, dataArray[3].trim(), keywords, parseInt(dataArray[5].trim()), parseInt(dataArray[6].trim()),
                                        dataArray[7].trim(), dataArray[8].trim());

                                cisuc.addPublication(book);
                                break;

                            case "BOOKCHAPTER":
                                BookChapter bookChapter = new BookChapter(dataArray[1].trim(), authorList, dataArray[3].trim(), keywords, parseInt(dataArray[5].trim()), parseInt(dataArray[6].trim()),
                                        dataArray[7].trim(), dataArray[8].trim(), dataArray[9].trim(), parseInt(dataArray[10].trim()), parseInt(dataArray[11].trim()));
                                cisuc.addPublication(bookChapter);
                                break;

                            case "CONFERENCEARTICLEBOOK":
                                ConferenceArticleBook conferenceChapter = new ConferenceArticleBook(dataArray[1].trim(), authorList, dataArray[3].trim(), keywords, parseInt(dataArray[5].trim()), parseInt(dataArray[6].trim()),
                                        dataArray[7].trim(), dataArray[8].trim(), dataArray[9].trim(), parseInt(dataArray[10].trim()));
                                cisuc.addPublication(conferenceChapter);
                                break;

                            case "MAGAZINEARTICLE":
                                //Create a date object
                                String dateData[] = dataArray[8].split("/");
                                Date date = new Date(parseInt(dateData[0].trim()), parseInt(dateData[1]), parseInt(dateData[2].trim()));

                                MagazineArticle magazineArticle = new MagazineArticle(dataArray[1].trim(), authorList, dataArray[3].trim(), keywords, parseInt(dataArray[5].trim()), parseInt(dataArray[6].trim()),
                                        dataArray[7].trim(), date, parseInt(dataArray[9].trim()));

                                cisuc.addPublication(magazineArticle);
                                break;

                            case "CONFERENCEARTICLE":
                                //Create a date object
                                dateData = dataArray[8].split("/");
                                date = new Date(parseInt(dateData[0].trim()), parseInt(dateData[1].trim()), parseInt(dateData[2].trim()));

                                ConferenceArticle conferenceArticle = new ConferenceArticle(dataArray[1].trim(), authorList, dataArray[3].trim(), keywords, parseInt(dataArray[5].trim()), parseInt(dataArray[6].trim()),
                                        dataArray[7].trim(), date, dataArray[9].trim());

                                cisuc.addPublication(conferenceArticle);
                                break;

                            default:
                                //Type of publication is wrong
                                System.out.println("--CAUTION--");
                                System.out.println("The type of publication given in line " + i + " does not exist!");
                        }
                    } else {
                        //When there are no author or the authors given do not exist
                        System.out.println("CAUTION");
                        System.out.println("In line " + i + " of the publication text file or there are no authors or none of the authors given do not exist in the database");
                        System.out.println("For this reason, this publication is not be added to the program");
                    }
                    //When it should be an integer and it isn't
                } catch (NumberFormatException ex) {
                    System.out.println("--CAUTION--");
                    System.out.println("In line" + i + ", of the chosen file, there are invalid values (should be integer)!");
                    System.out.println("For this reason, this publication is not be added to the program");
                } catch (IndexOutOfBoundsException ex) {
                    //When there are elements missing in a array
                    System.out.println("--CAUTION--");
                    System.out.println("In line " + i + ", of the researcher file, there are missing elements");
                    System.out.println("For this reason, this publication is not be added to the program");
                }
                i++;
                s = br.readLine();
            }
            fs.close();
            br.close();
            //File doesn't exist
        } catch (FileNotFoundException ex) {
            System.out.println("Publication text file does not exist");
            System.exit(1);
        } catch (IOException ex) {
            System.out.println("There was a proble reading the file");
            System.exit(1);
        }
    }

    /*
    Menu with the user's options
    */
    private void menu() {

        Scanner sc = new Scanner(System.in);
        Scanner sr = new Scanner(System.in);

        int userChoice = 0;

        while (userChoice != 7) {
            System.out.println("Action to realize:");
            System.out.println("1- General information about CISUC (number of members, number of members of each category, number of publications in the last 5 years, "
                    + "number of publications of each type");
            System.out.println("2- See the publications made in the last 5 years by a specific research group, organized by year, type of publication and impact");
            System.out.println("3- See the members of a specific research group organized by their categories");
            System.out.println("4- See the publications made by a specific researcher organized  by year, type of publication and impact");
            System.out.println("5- Information about all the research groups (number of members, number of members of each category, number of publications in the last 5 years"
                    + " and number of publications grouped by year, type of publication and impact");
            System.out.println("6- Credits");
            System.out.println("7- Exit program");
            //Ask for a number from the user until he gives a valid one
            while (true) {
                try {
                    System.out.print("Action to take:");
                    userChoice = Integer.parseInt(sc.nextLine());
                    if (userChoice <= 7 && userChoice > 0) {
                        break;
                    }
                    System.out.println("Please choose an option between 1 and 7 (including)");
                } catch (NumberFormatException e) {
                    System.out.println("The value given is not an integer!");
                }
            }
            //Activates the chosen functionality
            switch (userChoice) {
                case 1:
                    showCisucInformation();
                    break;

                case 2:
                    previous5YearsPublications();

                    break;

                case 3:
                    membersOfResearchGroup();
                    break;

                case 4:
                    seePublicationsOfResearcher();
                    break;

                case 5:
                    showResearchGroupsInformation();
                    break;

                case 6:
                    credits();
                    break;
                    
                case 7:
                    System.out.println("Goodbye!");
                    break;

            }
        }
    }
    /*
    Shows information about the CISUC ( the number of Researchers that are in the
     researchersList, the number of researchers by category, the number of publications released in the last 5 years
     and the number of publications by category.
    */
    private void showCisucInformation() {
        cisuc.showCISUCInformation();
    }

    /*
    Shows the publications made in the last 5 years by a specific research group, organized by year, type of publication and impact
    The user can type "Help" or "Exit" to see all the possible options or leave the program, respectively,  whenever he wants.
    */
    private void previous5YearsPublications() {
        Scanner sr = new Scanner(System.in);
        boolean exit = false;
        while (!exit) {
            System.out.print("Choose an research group (type \"Help\" if you want to see all the possible options or type \"Exit\" if you want to return to the menu) : ");
            String input = sr.nextLine();
            
            //Selects the choosen user's input functionality
            switch (input.toUpperCase().trim()) {
                //In case the user wants to see all the possible options at his disposal 
                case "HELP":
                    cisuc.showAllResearchGroups();
                    break;

                //In case the user wants to exit this functionality
                case "EXIT":
                    exit = true;
                    break;
                
                /*See if the input of the user is equal to any research group. If it is it gets the publications of the last 5 years, shows them on the console and exits 
                   the functionality. If there is no match a new research group name will be asked to the user.*/
                default:
                    ResearchGroup resGroup = cisuc.findResearchGroup(input.trim());
                    if (resGroup != null) {
                        List<Publication> publicationList = cisuc.getPublicationsResearchGroup(resGroup);
                        System.out.println("Publication for" + resGroup.getName() + " research group:");
                        publicationList = cisuc.publicationsOfTheLast5Years(publicationList, 2020);
                        cisuc.readPublicationList(publicationList);
                        exit = true;

                    } else {
                        System.out.println("This research group does not exist!");
                    }

            }
        }

    }
    /*
    Shows the members of a specific research group organized by their categories
    The user can type "Help" or "Exit" to see all the possible options or leave the program, respectively.
    */
    private void membersOfResearchGroup() {
        Scanner sr = new Scanner(System.in);
        boolean exit = false;
        while (!exit) {
            System.out.print("Choose a research group (type \"Help\" if you want to see all the possible options or type \"Exit\" if you want to return to the menu) : ");
            String input = sr.nextLine();
            //Selects the chosen user's input
            switch (input.toUpperCase().trim()) {
                case "HELP":
                    cisuc.showAllResearchGroups();
                    break;

                case "EXIT":
                    exit = true;
                    break;

                default:
                    //Looks for a research group with the name provided by the user
                    ResearchGroup resGroup = cisuc.findResearchGroup(input.trim());
                    //Finds a name that matches
                    if (resGroup != null) {
                        resGroup.showResearchers();
                        exit = true;
                    } else {
                        System.out.println("This research group does not exist");
                    }

            }
        }

    }

    /*
    See the publications made by a specific researcher organized  by year, type of publication and impact.
    The user can type "Help" or "Exit" to see all the possible options or leave the program, respectively.
    */
    private void seePublicationsOfResearcher() {
        Scanner sr = new Scanner(System.in);
        boolean exit = false;
        
        while (!exit) {
            System.out.print("Choose a researcher (type \"Help\" if you want to see all the possible options or type \"Exit\" if you want to return to the menu) : ");
            String input = sr.nextLine();
            //Selects the user's input
            switch (input.toUpperCase().trim()) {
                case "HELP":
                    cisuc.showAllResearchers();
                    break;

                case "EXIT":
                    exit = true;
                    break;

                default:
                    //Looks for a researcher with the name provided by the user
                    Researcher res = cisuc.findResearcher(input.trim());
                    //Finds a match
                    if (res != null) {
                        List<Publication> publicationList = cisuc.getPublicationsResearcher(res);
                        cisuc.readPublicationList(publicationList);
                        exit = true;

                    } else {
                        System.out.println("This research group does not exist");
                    }

            }
        }
    }
    
    /*
    Shows information about all the research groups (number of members, number of members of each category, number of publications in the last 5 years
    and number of publications grouped by tipe of publication and impact).
    */
    private void showResearchGroupsInformation() {
        cisuc.showResearchGroupsInformation();
    }
    
    //Credits the creator of the software
    private void credits() {
        System.out.println("----- Credits -----");
        System.out.println("This program was created by Edgar Filipe Ferreira Duarte as the final project for the subject \" Programação Orientada a Objetos \"");
        System.out.println("Universidade de Coimbra, 2020/2021 DEI");
        System.out.println("Please enjoy the program. If you find/have any bugs/feedback feel free to share them to the email: edgarduarte@student.dei.uc.pt");
        System.out.println("Note: If you are using the files that were given as examples, please note that the information is mostly made up!"
                + " Please refrain from contacting the phone numbers (they are made up) or emailing the email adresses provided, unless you have a good reason to do it!");
        System.out.println("-------------------");
    }

    //Main function where everything starts
    public static void main(String[] args) {
        //Creates a CisucManager
        CisucManager cm = new CisucManager();
        cm.execute();
    }
    //function that starts the execution of the program. First it reads the files and then it goes to the menu
    private void execute() {
        //Read the files
        readFile("CISUCdata.bin", "Researchers.txt", "ResearchGroups.txt", "Publications.txt");
        //Go to the menu
        menu();
    }
}
