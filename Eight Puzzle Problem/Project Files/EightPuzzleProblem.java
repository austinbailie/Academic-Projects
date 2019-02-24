/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package eightpuzzlesearchagent;


import java.util.Arrays;
import java.util.List;
import java.util.LinkedList;

/**
 *
 * @author austinbailie
 */
public class EightPuzzleProblem implements GenericProblem<EightPuzzleBoard, EightPuzzleAction> {
    
    private EightPuzzleBoard initialState;
    private EightPuzzleBoard goalState;

    /**
     * Gets the initial state of the problem.
     * 
     * @return - the initial state.
     */
    @Override
    public EightPuzzleBoard getInitialState() {
        return initialState;
    }

    /**
     * Gets the goal state of the problem.
     * 
     * @return - the goal state.
     */
    public EightPuzzleBoard getGoalState() {
        return goalState;
    }
    
    /**
     * Generates a list of possible actions from the current board state.
     * 
     * @param boardState - the board state to determine a list of actions from.
     * @return - a list of possible actions.
     */
    @Override
    public List<EightPuzzleAction> getActions(EightPuzzleBoard boardState) {
        
        List<EightPuzzleAction> actions = new LinkedList<EightPuzzleAction>();
        
        EightPuzzleAction left;
        EightPuzzleAction right;
        EightPuzzleAction up;
        EightPuzzleAction down;
        
        left = new EightPuzzleAction(EightPuzzleAction.actions[0]);
        right = new EightPuzzleAction(EightPuzzleAction.actions[1]);
        up = new EightPuzzleAction(EightPuzzleAction.actions[2]);
        down = new EightPuzzleAction(EightPuzzleAction.actions[3]);
        
        if(boardState.isMoveValid(left)) {
            
            actions.add(left);
            
        }
        
        if(boardState.isMoveValid(right)) {
            
            actions.add(right);
            
        }
        
        if(boardState.isMoveValid(up)) {
            
            actions.add(up);
            
        }
        
        if(boardState.isMoveValid(down)) {
            
            actions.add(down);
        }
        
        
        return actions;
    }
    
    /**
     * Performs a specified move on a given state.
     * 
     * @param state - the state to perform the move on.
     * @param action - the action to perform.
     * @return - the state after the action was performed.
     */
    @Override
    public EightPuzzleBoard getResult(EightPuzzleBoard state, EightPuzzleAction action) {
                
        state.move(action);
        
        return state;
    }
    
    /**
     * Checks if a state is the goal state.
     * 
     * @param boardState - the state to check.
     * @return - boolean value indicating if it is a goal state.
     */
    @Override
    public boolean goalTest(EightPuzzleBoard boardState) {
        
        return Arrays.equals(boardState.getBoardState(), this.goalState.getBoardState());
    }

    /**
     * Initializes the initial and goal state.
     * 
     * @param initialState - the initial state of the problem.
     * @param goalState - the goal state of the problem.
     */
    public EightPuzzleProblem(EightPuzzleBoard initialState, EightPuzzleBoard goalState) {
        this.initialState = initialState;
        this.goalState = goalState;
    }


    /**
     * Returns a step cost.
     * 
     * @param state1 - a state.
     * @param action - a action.
     * @param state2 - a state.
     * @return 
     */
    @Override
    public double getStepCosts(EightPuzzleBoard state1, EightPuzzleAction action, EightPuzzleBoard state2) {
        return 1.0;
    }
    
}
