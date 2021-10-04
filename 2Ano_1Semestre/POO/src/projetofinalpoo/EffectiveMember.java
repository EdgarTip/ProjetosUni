/*
Code developed by Edgar Filipe Ferreira Duarte, student number: 2019216077
personal email: edgartip2003@gmail.com
institutional email : edgarduarte@student.dei.uc.pt
DEI 2020-2021
 */
package projetofinalpoo;

/**
 *A EffectiveMember class is a child from the Researcher class. Besides all the variables that define a Researcher (and so it also defines a EffectiveMember), a 
 *EffecTiveMember is defined by its cabinetNumber, telephoneNumber and isLeader boolean (if he is a Leader or not). The type of researcher of and EffectiveMember 
 * is by default "effective member".
 * <p>
 * A EffectiveMmber has gets, sets, constructors and toString() methods of its own but it inherits the methods from its parent class (Researcher).
 * 
 * @see Researcher
 * @author Edgar Duarte
 */
public class EffectiveMember extends Researcher{
    private int cabinetNumber;
    private int telephoneNumber;
    private boolean isLeader;
    
    /**
     * Sole constructor that initializes the type with the default value of "effective member".
     */
    public EffectiveMember(){
        type = "effective member";
    }

    /**
     * Constructor that initializes the EffectiveMember and Researcher variables (name, email, research group, cabinet number, telephone number, isLeader boolean). It sets the
     * type of researcher with the default value of "effective member".
     * @param name, name of the EffectiveMember
     * @param email, email of the EffectiveMember
     * @param researchGroup, researchGroup that the EffectiveMember is in
     * @param cabinetNumber, number of the cabinet of the EffectiveMember
     * @param telephoneNumber, telephone number of the EffectiveMember
     * @param isLeader , boolean that indicates if the EffectiveMember is or is not a leader
     */
    public EffectiveMember(String name, String email, ResearchGroup researchGroup, int cabinetNumber, int telephoneNumber, boolean isLeader) {
        super(name, email, researchGroup);
        this.cabinetNumber = cabinetNumber;
        this.telephoneNumber = telephoneNumber;
        this.isLeader = isLeader;
        this.type = "effective member";
    }

    
    /**
     * Returns the current value of the cabinet number of the EffectiveMember
     * @return cabinet number of the EffectiveMember
     */
    public int getCabinetNumber() {
        return cabinetNumber;
    }

    /**
     * Alters the value of the cabinet number of the Effective Member
     * @param cabinetNumber new cabinet number of the Effective Member
     */
    public void setCabinetNumber(int cabinetNumber) {
        this.cabinetNumber = cabinetNumber;
    }

    /**
     * Alters the value of the boolean that indicates if the EffectiveMember is or is not a leader
     * @param isLeader boolean that indicates if the EffectiveMember is a leader
     */
    public void setIsLeader(boolean isLeader){
        this.isLeader = isLeader;
    }
    
    /**
     * Returns the current value of the boolean that indicates if the EffectiveMember is or is not a leader
     * @return a boolean that indicates if the EffectiveMember is a leader (true if he is, false if he is not)
     */
    public boolean getIsLeader(){
        return isLeader;
    }
    
    /**
     * Returns the current value of the telephone number of the EffectiveMember
     * @return telephone number of the EffectiveMember
     */
    public int getTelephoneNumber() {
        return telephoneNumber;
    }

    /**
     * Alters the value of the telephone number of the EffectiveMember
     * @param telephoneNumber new telephone number of the EffectiveMember
     */
    public void setTelephoneNumber(int telephoneNumber) {
        this.telephoneNumber = telephoneNumber;
    }
   
    @Override
    /**
     *Method that returns a string that describes the EffectiveMember by its name, email, cabinet number, telephone number and
     * is leader in the format:
     * <p>
     * name: __, email: __, cabinet number: __, telephone number: __, is leader: __
     * <p>
     * Note: __ indicates a value to be set
     * <p>
     * Note: the to.String() method from its parent class (Researcher) is also used
     * 
     * @return a String that indicates the EffectiveMember's
     * @see Researcher
     */
     
    public String toString(){
        return super.toString() +", cabinet number: " + cabinetNumber +", telephone number: " + telephoneNumber + ", is leader: " + isLeader;
    }
    
}
