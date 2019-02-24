/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package eightpuzzlesearchagent;


import java.util.Arrays;
import java.util.Objects;

/**
 *
 * @author austinbailie
 */
public class EightPuzzleBoard implements GenericState<EightPuzzleBoard , EightPuzzleAction> {
    
    private int[] boardState;
    private EightPuzzleAction left;
    private EightPuzzleAction right;
    private EightPuzzleAction up;
    private EightPuzzleAction down;
    
    private int position;
    
    
    
    public EightPuzzleBoard(){
        boardState = new int[]{0,1,2,3,4,5,6,7,8};
        left = new EightPuzzleAction(EightPuzzleAction.actions[0]);
        right = new EightPuzzleAction(EightPuzzleAction.actions[1]);
        up = new EightPuzzleAction(EightPuzzleAction.actions[2]);
        down = new EightPuzzleAction(EightPuzzleAction.actions[3]);
                 
    }
    
    /**
     * Initializes a puzzle board and sets the board state.
     * Also finds the position of the blank tile.
     * 
     * @param inputState 
     */
    public EightPuzzleBoard(int[] inputState){
        
        this.boardState = inputState;
        
        for (int i = 0; i < 9; i++) {
            if (inputState[i] == 0) {
                this.position = i;
                break;
            }
        }
        
    }
    
    /**
     * Returns the state of the board.
     * @return - the state of the board.
     */
    @Override
    public EightPuzzleBoard getState() {
        return this;
    }

    /**
     * Prints the state of the board.
     */
    @Override
    public void printState() {
        System.out.println(this);   

    }

    /**
     * Performs the specified move on the board.
     * @param action - The specified action to perform.
     */
    @Override
    public void move(EightPuzzleAction action) {
          //do the move here.
          
        int newState[] = this.boardState.clone();

        int swap = -1;
        if(action.getAction().equals("LEFT")) {

            swap = position -1;

        }else if(action.getAction().equals("RIGHT")) {

            swap = position + 1;

        }else if(action.getAction().equals("UP")) {

            swap = position - 3;

        }else if(action.getAction().equals("DOWN")) {

            swap = position + 3;

        }
          
        int temp = newState[position];
        newState[position] = newState[swap];
        newState[swap] = temp;
          
        setBoardState(newState);
        this.position = swap;
       
        
    }
    
    /**
     * Checks if the action to be performed is a 
     * valid action based on its current position.
     * 
     * @param action - move to be performed
     * @return - boolean value indicating a valid move or not.
     */
    boolean isMoveValid(EightPuzzleAction action) {

        switch(action.getAction()) {
            case "LEFT":
                if (position == 0 || position == 3 || position == 6) {
                    return false;
                } else {
                    return true;
                }
            case "RIGHT":
                if (position == 2 || position == 5 || position == 8) {
                    return false;
                } else {
                    return true;
                }
            case "UP":
                if (position < 3) {
                    return false;
                } else {
                    return true;
                }
            case "DOWN":
                if (position >= 6) {
                    return false;
                } else {
                    return true;
                }
        }

        return false;
    }

    /**
     * Returns the current board state array.
     * 
     * @return - the board state array.
     */
    public int[] getBoardState() {
        return this.boardState;
    }

    /**
     * Sets the board state to the passed in array.
     * 
     * @param boardState - the board state to be set.
     */
    public void setBoardState(int[] boardState) {
        
        this.boardState = boardState;
    }

    /**
     * Prints the board state as a 3 by 3 grid.
     * 
     * @return - the 3 by 3 grid as a string.
     */
    @Override
    public String toString() {
        return boardState[0] + " " + boardState[1] + " " + boardState[2] + "\n"
	     + boardState[3] + " " + boardState[4] + " " + boardState[5] + "\n"
	     + boardState[6] + " " + boardState[7] + " " + boardState[8];
    }

    /**
     * Evaluates if a board state is equal to the current board state.
     * 
     * @param obj - board state object.
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
        final EightPuzzleBoard other = (EightPuzzleBoard) obj;
        if (!Objects.equals(this.boardState, other.boardState)) {
            return false;
        }
        return true;
        
    }

    @Override
    public int hashCode() {
        int hash = 5;
        hash = 67 * hash + Arrays.hashCode(this.boardState);
        return hash;
    }

    /**
     * Calculates the misplaced tile heuristic value.
     * 
     * @param initialState - the state to start evaluating the heuristic.
     * @param goalState - the state to stop evaluating the heuristic.
     * @return - the heuristic value.
     */
    public int calcH1val(EightPuzzleBoard initialState, EightPuzzleBoard goalState) {
        
        int[] initialArray = initialState.getBoardState();
        int[] goalArray = goalState.getBoardState();
        
        int hCost = 0;
        
        for(int i = 0; i < 9; i++) {
            
            if(initialArray[i] != goalArray[i] && initialArray[i] != 0) {
                
                ++hCost;
            }
        }
        
        
        return hCost;
    }
    
    /**
     * Calculates the manhattan distance heuristic value.
     * 
     * @param initialState - the state to start evaluating the heuristic.
     * @param goalState - the state to stop evaluating the heuristic.
     * @return - the heuristic value.
     */
    public int calcH2val(EightPuzzleBoard initialState, EightPuzzleBoard goalState) {
        
        int[] initialArray = initialState.getBoardState();
        int[] goalArray = goalState.getBoardState();
        
        int hCost = 0;
        
        for (int i = 0; i < initialArray.length; i++) {
            
            for (int j = 0; j < goalArray.length; j++) {
                
                if (initialArray[i] == goalArray[j] && initialArray[i] != 0 && goalArray[j] != 0) {
                    
                    hCost = hCost + ((Math.abs((i % 3) - (j % 3))) + (Math.abs((i / 3) - (j / 3))));
                   
                }
                
            }
            
        }
        
        return hCost;
    }
    
    
}

