package eightpuzzlesearchagent;

/**
 *
 * @author Le
 */
public class Node<S, A> {

	private S state;     //a state
	private Node<S, A> parent; //parent node.
	private A action; //action that applied to parent to generate the node.
	private double pathCost; //path cost g(n)

	/**
	 * Constructs a node with the specified state.
	 * 
	 * @param state
	 * the state in the state space to which the node corresponds.
	 */
	public Node(S state) {
		this.state = state;
		pathCost = 0.0;
	}

	/**
	 * Constructs a node with the specified state, parent, action, and path
	 * cost.
	 * 
	 * @param state
	 *            the state in the state space to which the node corresponds.
	 * @param parent
	 *            the node in the search tree that generated the node.
	 * @param action
	 *            the action that was applied to the parent to generate the
	 *            node.
	 * @param pathCost
	 *            full pathCost from the root node to here, typically
	 *            the root's path costs plus the step costs for executing
	 *            the the specified action.
	 */
	public Node(S state, Node<S, A> parent, A action, double pathCost) {
		this(state);
		this.parent = parent;
		this.action = action;
		this.pathCost = pathCost;
	}

	/**
	 * Returns the state in the state space to which the node corresponds.
	 * 
	 * @return the state in the state space to which the node corresponds.
	 */
	public S getState() {
		return state;
	}

	/**
	 * Returns this node's parent node, from which this node was generated.
	 * 
	 * @return the node's parenet node, from which this node was generated.
	 */
	public Node<S, A> getParent() {
		return parent;
	}

	/**
	 * Returns the action that was applied to the parent to generate the node.
	 * 
	 * @return the action that was applied to the parent to generate the node.
	 */
	public A getAction() {
		return action;
	}

	/**
	 * Returns the cost of the path from the initial state to this node as
	 * indicated by the parent pointers.
	 * 
	 * @return the cost of the path from the initial state to this node as
	 *         indicated by the parent pointers.
	 */
	public double getPathCost() {
		return pathCost;
	}

	/**
	 * Returns <code>true</code> if the node has no parent.
	 * 
	 * @return <code>true</code> if the node has no parent.
	 */
	public boolean isRootNode() {
		return parent == null;
	}

	@Override
	public String toString() {
		return "[parent=" + parent + ", action=" + action + ", state=" + getState() + ", pathCost=" + pathCost + "]\n";
	}
}