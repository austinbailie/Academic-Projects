
package a3;

/**
 * Holds methods required to build a stock investment,
 * using methods from the super class.
 * @author austinbailie
 */
@SuppressWarnings("serial")
public class Stock extends Investment {
    
    /**
     * An no argument constructor,
     * using the super class constructor.
     * @throws a3.Customexceptions - Checks validity within the super constructor.
     */ 
    public Stock() throws Customexceptions {
        
        super(null, null, 0, 0.0, 0.0, 0.0);
    }
    
    /**
     * An argumentative constructor, used to set the values
     * for each attribute in the stock object.
     * @param stockSym - The symbol value of the new stock.
     * @param stockName - The name value of the new stock.
     * @param stockQuan - The quantity value of the new stock.
     * @param stockPrice - The price value of the new stock.
     * @param stockValue - The book value of the new stock.
     * @param stockGain - The gain value of the new stock.
     * @throws a3.Customexceptions - Checks validity within the super constructor.
     */
    public Stock(String stockSym, String stockName, int stockQuan, double stockPrice, double stockValue, double stockGain) throws Customexceptions {
        
        super(stockSym, stockName, stockQuan, stockPrice, stockValue, stockGain);
    }

    
    /**
     * Called by the .get() method to print the investment.
     * @return a formated output of all attributes.
     */
    @Override
    public String toString( ) {
        
        return super.toString();
    }
}
