/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package a3;

/**
 *
 * @author austinbailie
 */
@SuppressWarnings("serial")
public class Customexceptions extends Exception {
    
    public Customexceptions() {
        
    }
    
    /**
     * Used to create a custom exception with a specific error message.
     * @param message - A string containing a message detailing the cause
     * of the thrown exception.
     */
    public Customexceptions(String message) {
        
        super(message);
    }
}

