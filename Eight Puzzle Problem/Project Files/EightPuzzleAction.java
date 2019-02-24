/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package eightpuzzlesearchagent;

import java.util.Objects;
/**
 *
 * @author austinbailie
 */
public class EightPuzzleAction implements GenericAction {
    
    private String action ="";
    static final String [] actions = {"LEFT","RIGHT","UP","DOWN"};

    public EightPuzzleAction() {
        
    }

    /**
     * Returns an action as a string.
     * 
     * @return - a action as a string.
     */
    @Override
    public String toString() {
        
      return action;
    }

    /**
     * Prints an action.
     */
    @Override
    public void printAction() {
        
        String temp="";
        for(String s: actions){
            temp += s + ", ";
        }
        System.out.println(temp);
    }
    
    /**
     * Initializes the action variable with an action.
     * 
     * @param action - the performed action.
     */
    public EightPuzzleAction(String action) {
        this.action = action;
    }

    /**
     * Returns an action as a string.
     * 
     * @return - the action.
     */
    public String getAction() {
        return action;
    }

    /**
     * Sets the action.
     * 
     * @param action - the action to be set.
     */
    public void setAction(String action) {
        this.action = action;
    }


    /**
     * Evaluates if an action is equal to the current action.
     * 
     * @param obj - action object.
     * @return - boolean value indicating if its equal or not.
     */
    @Override
    public boolean equals(Object obj) {
        if (this == obj) {
            return true;
        }
        if (obj == null) {
            return false;
        }
        if (getClass() != obj.getClass()) {
            return false;
        }
        final EightPuzzleAction other = (EightPuzzleAction) obj;
        if (!Objects.equals(this.action, other.action)) {
            return false;
        }
        return true;
    }

    @Override
    public int hashCode() {
        int hash = 7;
        hash = 37 * hash + Objects.hashCode(this.action);
        return hash;
    }
    
    
 
}
