package eightpuzzlesearchagent;



import java.util.List;

/**
 * 
 * @author Le
 */
public interface GenericProblem <S, A>{
    	/**
	 * Returns the initial state of the agent.
	 */
	S getInitialState();

	/**
	 * Returns the description of the possible actions available to the agent.
	 */
	List<A> getActions(S state);
        
	/**
	 * Returns the State that is a result of an action.
         * This is a transition model. e.g. Successor.
	 */        
        public S getResult(S state, A action);

	/**
	 * Determines whether a given state is a goal state.
	 */
	boolean goalTest(S state);

	/**
	 * Returns the <b>step cost</b> of taking action <code>action</code> in state <code>state</code> to reach state
	 * <code>stateDelta</code> denoted by c(s, a, s').
	 */
	double getStepCosts(S state1, A action, S state2);
}
