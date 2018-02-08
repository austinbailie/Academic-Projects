
package a3;

import java.io.Serializable;
import java.text.DecimalFormat;
/**
 *
 * @author austinbailie
 * 
 * Holds methods required to create
 * the investments Stock, and Mutual Fund.
 */
@SuppressWarnings({"serial", "RedundantStringConstructorCall", "UnneccesaryBoxing", "UnnecessaryBoxing"})
public abstract class Investment implements Serializable {
    
    private final String symbol;
    private final String name;
    private final int quantity;
    private final double price;
    private final double bookValue;
    private final double gain;
    
    DecimalFormat formatter = new DecimalFormat("#0.00");
    
    /**
     * A no argument constructor for the investment object. 
    */
    public Investment() {
        
        symbol = null;
        name = null;
        quantity = 0;
        price = 0.0;
        bookValue = 0.0;
        gain = 0.0;  
    }
    
    /**
     * An argumentative constructor, used to set the values
     * for each attribute in the object.
     * @param invSym - The symbol value of the new investment.
     * @param invName - The name value of the new investment.
     * @param invQuan - The quantity value of the new investment.
     * @param invPrice - The price value of the new investment.
     * @param invValue - The bookValue value of the new investment.
     * @param invGain - The gain value of the new investment.
     * @throws a3.Customexceptions - Checks validity within the constructor.
     */
    public Investment(String invSym, String invName, int invQuan, double invPrice, double invValue, double invGain) throws Customexceptions {
        
        /* Not all attributes can be tested for invalid input in the constructors due to segments of code
        that use a none tested constructor to its advantage(E.g: checks for unique investments). However,
        all attributes are tested upon user input.*/
        if(invQuan < 0) throw new Customexceptions("ERROR: Invalid quantity entry, quantity cannot be negative."); 
        if(invPrice < 0) throw new Customexceptions("ERROR: Invalid price entry, price cannot be negative."); 
        
        this.symbol = invSym;
        this.name = invName;
        this.quantity = invQuan;
        this.price = invPrice;
        this.bookValue = invValue;     
        this.gain = invGain;
    }
    
     /**
     * Used to retrieve a symbol of an investment(non-leaking getter).
     * @return the symbol string of the specified investment.
     */
    public String getSymbol() {
        
        return new String(symbol);
    }
    
    /**
     * Retrieve a name of an investment(non-leaking getter).
     * @return the name string of specified investment.
     */
    public String getName() {
        
        return new String(name);
    }
    
     /**
     * Retrieve the quantity of an investment(non-leaking getter).
     * @return the quantity integer of specified investment.
     */
    public int getQuantity() {
        
        return new Integer(quantity);
    }
    
    /**
     * Retrieve the price of an investment(non-leaking getter).
     * @return the price double of specified investment.
     */
    public double getPrice() {
        
        return new Double(price);
    }
    
    /**
     * Retrieve the book value of an investment(non-leaking getter).
     * @return the bookValue double of specified investment.
     */
    public double getBookVal() {
        
        return new Double(bookValue);
    }
    
    /**
    * Retrieve the gain of an investment(non-leaking getter).
    * @return the gain double of specified investment.
    */
    public double getGainVal() {
        
        return new Double(gain);
    }
    /**
     * Used to check if the given object is equal to 
     * a specific investment, using the symbol variable to validate.
     * @param obj - The object to be checked.
     * @return true if stocks are equal.
     */
    @Override
    public boolean equals(Object obj) {
        
        boolean check = false;
        
        if(obj instanceof Investment) {
               
            Investment invest = (Investment) obj;
            
            if(invest != null && this.symbol.equalsIgnoreCase(invest.symbol)) {
            
                check = true;
            }
        }
            
        return check;
    }
    /**
     * Called by the .get() method to print the investment.
     * @return a formated output of all attributes.
     */
    @Override
    public String toString() {
        
        return (this.symbol + ", " + this.name + ", " + this.quantity + ", $" + formatter.format(this.price) + ", $" + formatter.format(this.bookValue));
    } 
}


