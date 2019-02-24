
package a3;

/**
 * Holds methods required to build a stock investment,
 * using methods from the super class.
 * @author austinbailie
 */
@SuppressWarnings("serial")
public class MutualFund extends Investment {
    
    /**
     * A no argument constructor,
     * using the super class constructor.
     * @throws a3.Customexceptions - Checks validity within the super constructor.
     */ 
    public MutualFund() throws Customexceptions {
    
        super(null, null, 0, 0.0, 0.0, 0.0);
    }
    
    /**
     * An argumentative constructor, used to set the values
     * for each attribute in the mutual fund object.
     * @param mfSym - The symbol value of the new mutual fund.
     * @param mfName - The name value of the new mutual fund.
     * @param mfQuan - The quantity value of the new mutual fund.
     * @param mfPrice - The price value of the new mutual fund.
     * @param mfValue - The book value of the new mutual fund.
     * @param mfGain - The gain value of the new mutual fund.
     * @throws a3.Customexceptions - Checks validity within the super constructor.
     */
    public MutualFund(String mfSym, String mfName, int mfQuan, double mfPrice, double mfValue, double mfGain) throws Customexceptions {
        
        super(mfSym, mfName, mfQuan, mfPrice, mfValue, mfGain);
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
