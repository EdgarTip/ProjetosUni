/* 
Programa desenvolvido por Edgar Filipe Ferreira Duarte, número de aluno : 2019216077
Email  pessoal: edgartip2003@gmail.com
Email universidade: uc2019216077@student.uc.pt
*/
package projetofinalpoo;
import java.io.*;
/**
 * The Date class represents a real life date. It is defined by 3 integers and a boolean that indicates if the date is valid.
 * The 3 integer represent the day, month and year of the date. The boolean's value is set when the date is created (in the constructor) or 
 * it can be forced to be valid /invalid via the setValidDate() method. If the boolean has the value true it means it is a valid date, if it has a 
 * value of false it means the date is not valid.
 * <p>
 * A Date has sets, gets, constructors and toString() public methods. 
 * <p>
 * A Date is Serializable, meaning that it can be stored in a binary/object file.
 * 
 * @author Edgar Duarte
 */
public class Date implements Serializable{
    private int day;
    private int month;
    private int year;
    public boolean isValidDate;
    
    public Date(){}
    public Date(int day, int month, int year){
        if(checkDate(day, month)){
            this.day = day;
            this.month = month;
            this.year = year;
            isValidDate = true;
        }
        isValidDate = false;
    }
    
    /**
     * Returns the current day of the Date
     * @return day of the Date
     */
    public int getDay(){
        return day;
    }
    
    /**
     * Returns the current month of the Date
     * @return month of the Date
     */
    public int getMonth(){
        return month;
    }
    
    /**
     * Returns the current year of the Date
     * @return year of the Date
     */
    public int getYear(){
        return year;
    }
    
    /**
     * Returns the boolean that indicates if the Date is valid or not
     * @return boolean that indicates the Date's validity
     */
    public boolean getIsValidDate(){
        return isValidDate;
    }

    /**
     * Alters the value of the day. If the day is valid the value is altered, if it is not the value is not altered
     * @param newDay new value of the day of the Date
     * @return true if the date was successfully set, false if the date was not successfully set
     */
    public boolean setDay(int newDay){
        //If the given day is impossible we don't update the value of 'day'
        if(newDay <= 0  || newDay > 31){
            System.out.println("Invalid day");
            return false;
        }
        
        this.day = newDay;
        return true;
    }
    
     /**
     * Alters the value of the month. If the month is valid the value is altered, if it is not the value is not altered
     * @param newMonth new value of the month of the Date
     * @return true if the date was successfully set, false if the date was not successfully set
     */
    public boolean setMonth(int newMonth){
        //If the given month is impossible we don't update the value of 'month'
        if( newMonth <=0 || newMonth > 12){
            System.out.println("Invalid month");
            return false;
        }
        
        this.month = newMonth;
        return true;
    }
    
    /**
     * Alters the value of the boolean that indicates the validity of the Date
     * @param isValidDate boolean that indicates the validity of the Date
     */
    public void setValidDate(boolean isValidDate){
        this.isValidDate = isValidDate;
    }
    
    /**
     * Alters the value of the year of the Date
     * @param year new year of the Date
     */
    public void setYear(int year){     
        this.year = year;
    }
    
    /*
    Superficial check of the date if it is valid or not. There are cases where this verification fails, and in those cases 
    nothing crashes you only have an invalid date.
    */
    private boolean checkDate(int day, int month){
        //if the day is invalid
        if(day > 0 && month > 0 ){
            if(day <= 31 && ( month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12)){
                return true;
        }
            else if( day <= 30 && ( month == 4 || month == 6 || month == 9 || month == 11)){
                return true;
            }
            
            else if (day <= 29 && ( month == 2))
                return true;
        }
        return false;
    }

    @Override
    /**
     * This method returns a String that describes a Date by its day, month and year in the format: 
     * <p>
     * day/month/year
     * <p>
     * @return a String that describes the Date
     */
    public String toString() {
        return day + "/" + month + "/" + year;
    }
    
    
}
