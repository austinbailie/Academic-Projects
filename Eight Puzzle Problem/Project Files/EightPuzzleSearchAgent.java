/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package eightpuzzlesearchagent;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Comparator;
import java.util.LinkedList;
import java.util.List;
import java.util.Queue;
import java.util.PriorityQueue;

/**
 *
 * @author austinbailie
 */
public class EightPuzzleSearchAgent {

    EightPuzzleProblem problem;
    private final Queue<Node<EightPuzzleBoard, EightPuzzleAction>> frontier;
    private final PriorityQueue<Node<EightPuzzleBoard, EightPuzzleAction>> queueAS1;
    private final PriorityQueue<Node<EightPuzzleBoard, EightPuzzleAction>> queueAS2;
    
    private final ArrayList<Integer> BFSdepth = new ArrayList<Integer>();
    private final ArrayList<Integer> AS1depth = new ArrayList<Integer>();
    private final ArrayList<Integer> AS2depth = new ArrayList<Integer>();

    private ArrayList<Node<EightPuzzleBoard, EightPuzzleAction>> BFSpath = new ArrayList<Node<EightPuzzleBoard, EightPuzzleAction>>();
    private ArrayList<Node<EightPuzzleBoard, EightPuzzleAction>> AS1path = new ArrayList<Node<EightPuzzleBoard, EightPuzzleAction>>();
    private ArrayList<Node<EightPuzzleBoard, EightPuzzleAction>> AS2path = new ArrayList<Node<EightPuzzleBoard, EightPuzzleAction>>();
    

    
    public EightPuzzleSearchAgent(EightPuzzleProblem aProblem) {
        problem = aProblem;
        frontier = new LinkedList<Node<EightPuzzleBoard, EightPuzzleAction>>();
        queueAS1 = new PriorityQueue<Node<EightPuzzleBoard, EightPuzzleAction>>(new costComparatorAS1());
        queueAS2 = new PriorityQueue<Node<EightPuzzleBoard, EightPuzzleAction>>(new costComparatorAS2());
        
    }
    
    
    /**
     * Reads the file for the initial and goal states and assigns 
     * the values to appropriate array. 
     * 
     * @param args the command line arguments
     * @throws java.io.IOException
     */
    public static void main(String[] args) throws IOException {
        
        int[] initialArray = new int[9]; 
        int[] goalArray = new int[9];
        int count = 0;
        int count2 = 0;
        
        BufferedReader br = new BufferedReader(new FileReader(args[0]));
        
        try {
             
            String line = br.readLine();
            
            while(line != null) {
                
                int[] temp = new int[3];
                
                if(!line.equals("Initial State") && !line.equals("Goal State") ) {
                    
                    String[] nums = line.split(" ");
                    temp[0] = Integer.parseInt(nums[0]);
                    temp[1] = Integer.parseInt(nums[1]);
                    temp[2] = Integer.parseInt(nums[2]);
                    
                    if(count < 3) {
                                            
                        int index = count * 3;
                        
                        for(int i = 0; i < 3; i++) {
                            
                            initialArray[index + i] = temp[i];
                        }                        
                        ++count;
                    
                    } else {

                        int index = count2 * 3;
                        
                        for(int i = 0; i < 3; i++) {
                            
                            goalArray[index + i] = temp[i];
                        }
                        ++count2;
                    }
                    
                }
                
                System.out.println(line);
                line = br.readLine();
            
            }
            
        }finally {
            
            if(br != null) {
                br.close();
            }
        }

        
        EightPuzzleBoard initialState = new EightPuzzleBoard(initialArray);
        EightPuzzleBoard goalState = new EightPuzzleBoard(goalArray);
        
        EightPuzzleProblem problem = new EightPuzzleProblem(initialState,goalState);        
        EightPuzzleSearchAgent sa = new EightPuzzleSearchAgent(problem);
       
        Node<EightPuzzleBoard, EightPuzzleAction> root = new Node<EightPuzzleBoard, EightPuzzleAction>(initialState,null,null,0.0);
        
        sa.showSolution(root);

    }
    
    /**
     * Initiates each search and adds the initial state(rootNode)
     * to the queues.
     * 
     * @param rootNode - The node containing the initial state.
    **/
    public void showSolution(Node<EightPuzzleBoard, EightPuzzleAction> rootNode) {
        //do the search and print out 
        
        BFSdepth.add(1);
        frontier.add(rootNode);
        breadthFirstSearch();
        frontier.clear();
        
        AS1depth.add(1);
        queueAS1.add(rootNode);
        AstarSearchH1();
        queueAS1.clear();
        
        AS2depth.add(1);
        queueAS2.add(rootNode);
        AstarSearchH2();
        queueAS2.clear();
        
        printDepthTable();
 
    }
    
    /**
    * Performs the breadth first search.
    * 
    **/
    public void breadthFirstSearch() {
    
        System.out.println("\nBreadth First Search:");
        
        while(!frontier.isEmpty()) {
            
            Node<EightPuzzleBoard, EightPuzzleAction> currentNode = frontier.remove();
            
            EightPuzzleBoard currentState = currentNode.getState();
                 
            List<EightPuzzleAction> actions = problem.getActions(currentState);
            
            System.out.println("\n" + currentState);
            System.out.println("Action = " + actions);
            double g = currentNode.getPathCost();
            System.out.println("g = " + g + "\n--------------------------------");
            
            
            if(problem.goalTest(currentState) == true) {
                
                printBFSsolution(currentNode);
                break;
            }
            
            
            for(int i = 0; i < actions.size(); i++) {
                
                EightPuzzleAction action = actions.get(i);
                
                EightPuzzleBoard clone = new EightPuzzleBoard(currentState.getBoardState().clone());
                EightPuzzleBoard childState = problem.getResult(clone, action);
                
                double gCost = currentNode.getPathCost() + 1.0;

                Node<EightPuzzleBoard, EightPuzzleAction> childNode = new Node<EightPuzzleBoard, EightPuzzleAction>(childState,currentNode,action,gCost);
                
                if(childNode.getPathCost() >= BFSdepth.size()) {
                
                    BFSdepth.add(1);

                }else {

                    int index = (int)childNode.getPathCost();

                    int nodeCount = BFSdepth.get(index);

                    nodeCount = nodeCount + 1;

                    BFSdepth.set(index, nodeCount);

                }
                
                
                frontier.add(childNode);
               
            }
        }
    }
    
    /**
     * Prints the path solution from the BFS search.
     * 
     * @param node - The node containing the goal state.
     */
    public void printBFSsolution(Node<EightPuzzleBoard, EightPuzzleAction> node) {
        
        BFSpath = getPathSolution(node);
        
        System.out.println("\nSolution:\n\nInitial State");
        
        for(int i = (BFSpath.size() - 1); i >= 0; i--) {
                
            Node<EightPuzzleBoard, EightPuzzleAction> pathNode = BFSpath.get(i);
            Node<EightPuzzleBoard, EightPuzzleAction> nextNode = null;

            if(i != 0) 
                nextNode = BFSpath.get(i - 1);

            System.out.println(pathNode.getState());

            if(i != 0) 
                System.out.println("Action = " + nextNode.getAction());

            double g = 0;
            
            g = pathNode.getPathCost();

            System.out.println("g = " + g + "\n--------------------------------\n");
        }
        
        
        int nodeCount = 0;
        
        for(int nodes : BFSdepth) {
            
            nodeCount += nodes;
        }
        
        System.out.println("Number of nodes on the tree = " + nodeCount + "\n");
        
    }
    
    /**
     * Performs A-Star search with the misplaced tiles heuristic.
     * 
     */
    public void AstarSearchH1() {
        
        System.out.println("\nA* Search with H1:");

        // EACH NODE
        while(!queueAS1.isEmpty()) {
            
            Node<EightPuzzleBoard, EightPuzzleAction> currentNode = queueAS1.remove();
            
            EightPuzzleBoard currentState = currentNode.getState();
            
            List<EightPuzzleAction> actions = problem.getActions(currentState);
            
            
            System.out.println("\n" + currentState);
            System.out.println("Action = " + actions);
            double g = currentNode.getPathCost();
            double h = currentState.calcH1val(currentState, problem.getGoalState());
            double f = g + h;
            System.out.println("g = " + g + ", " + "h = " + h + ", " + "f = " + f + "\n--------------------------------");
            
            if(problem.goalTest(currentState) == true) {
                
                printAstarSolution(currentNode, true);
                break;
            }
            
            
            // EACH ACTION
            for(int i = 0; i < actions.size(); i++) {
                
                EightPuzzleAction action = actions.get(i);
                
                EightPuzzleBoard clone = new EightPuzzleBoard(currentState.getBoardState().clone());
                EightPuzzleBoard childState = problem.getResult(clone, action);
                
                double gCost = currentNode.getPathCost() + 1.0;

                Node<EightPuzzleBoard, EightPuzzleAction> childNode = new Node<EightPuzzleBoard, EightPuzzleAction>(childState,currentNode,action,gCost);              

                if(childNode.getPathCost() >= AS1depth.size()) {

                    AS1depth.add(1);

                }else {

                    int index = (int)childNode.getPathCost();

                    int nodeCount = AS1depth.get(index);

                    nodeCount = nodeCount + 1;

                    AS1depth.set(index, nodeCount);

                }

                queueAS1.add(childNode);
                  
            }
        }
    }
    
    /**
     * Performs A-Star search with the manhattan distance heuristic.
     * 
     */
    public void AstarSearchH2() {
        
        System.out.println("\nA* Search with H2:");
        
        // EACH NODE
        while(!queueAS2.isEmpty()) {
            
            Node<EightPuzzleBoard, EightPuzzleAction> currentNode = queueAS2.remove();
            
            EightPuzzleBoard currentState = currentNode.getState();
            
            List<EightPuzzleAction> actions = problem.getActions(currentState);
            
           
            System.out.println("\n" + currentState);
            System.out.println("Action = " + actions);
            double g = currentNode.getPathCost();
            double h = currentState.calcH2val(currentState, problem.getGoalState());
            double f = g + h;
            System.out.println("g = " + g + ", " + "h = " + h + ", " + "f = " + f + "\n--------------------------------");
            
            if(problem.goalTest(currentState) == true) {
                
                printAstarSolution(currentNode, false);
                break;
            }
            
            // EACH ACTION - expanding nodes.
            for(int i = 0; i < actions.size(); i++) {
                
                EightPuzzleAction action = actions.get(i);
                
                EightPuzzleBoard clone = new EightPuzzleBoard(currentState.getBoardState().clone());
                EightPuzzleBoard childState = problem.getResult(clone, action);
                
                 
                double gCost = currentNode.getPathCost() + 1.0;

                Node<EightPuzzleBoard, EightPuzzleAction> childNode = new Node<EightPuzzleBoard, EightPuzzleAction>(childState,currentNode,action,gCost);

                if(childNode.getPathCost() >= AS2depth.size()) {

                    AS2depth.add(1);

                }else {

                    int index = (int)childNode.getPathCost();

                    int nodeCount = AS2depth.get(index);

                    nodeCount = nodeCount + 1;

                    AS2depth.set(index, nodeCount);

                }

                queueAS2.add(childNode);
                    
            }
        }
    }
    
    /**
     * Prints the path solution from either of A-Star searches.
     * 
     * @param node - the node containing the goal state.
     * @param H1search - boolean indicating what A-Star search the node came from.
     */
    public void printAstarSolution(Node<EightPuzzleBoard, EightPuzzleAction> node, boolean H1search) {
        
        System.out.println("\nSolution:\n\nInitial State");
        
        if(H1search == true) {
            
            AS1path = getPathSolution(node);
            
            for(int i = (AS1path.size() - 1); i >= 0; i--) {
                
                Node<EightPuzzleBoard, EightPuzzleAction> pathNode = AS1path.get(i);
                Node<EightPuzzleBoard, EightPuzzleAction> nextNode = null;
                
                if(i != 0) 
                    nextNode = AS1path.get(i - 1);
                
                System.out.println(pathNode.getState());
                
                if(i != 0) 
                    System.out.println("Action = " + nextNode.getAction());
                
                double g = 0;
                double h = 0;
                
                g = pathNode.getPathCost();
                
                EightPuzzleBoard boardPath = pathNode.getState();
                h = problem.getInitialState().calcH1val(boardPath.getState(), problem.getGoalState());
                
                double f = g + h;
                System.out.println("g = " + g + ", " + "h = " + h + ", " + "f = " + f + "\n--------------------------------\n");
            }
            
        }else {
            
            AS2path = getPathSolution(node);
            
            for(int i = (AS2path.size() - 1); i >= 0; i--) {
                
                Node<EightPuzzleBoard, EightPuzzleAction> pathNode = AS2path.get(i);
                Node<EightPuzzleBoard, EightPuzzleAction> nextNode = null;
                
                if(i != 0) 
                    nextNode = AS2path.get(i - 1);
                
                System.out.println(pathNode.getState());
                
                if(i != 0) 
                    System.out.println("Action = " + nextNode.getAction());
                
                double g = 0;
                double h = 0;
                
                g = pathNode.getPathCost();
                
                EightPuzzleBoard boardPath = pathNode.getState();
                h = problem.getInitialState().calcH2val(boardPath.getState(), problem.getGoalState());
                
                double f = g + h;
                System.out.println("g = " + g + ", " + "h = " + h + ", " + "f = " + f + "\n--------------------------------\n");
            }
            
        }
        
        int nodeCount = 0;
        
        if(H1search == true) {
            
            for(int nodes : AS1depth) {

                nodeCount += nodes;
            }

        }else {
            
            for(int nodes : AS2depth) {

                nodeCount += nodes;
            }
        }
        
        System.out.println("Number of nodes on the tree = " + nodeCount + "\n");
        
    }
    
    /**
     * Prints table showing the search cost at each depth for all searches.
     */
    public void printDepthTable() {
        

        System.out.println("\nDepth\t  | Search Cost\n------------------------------------");
        System.out.println("\t  | A*(h1) | A*(h2) | BFS ");
        
        int largestArray = 0;
        
        if(BFSdepth.size() > AS1depth.size() && BFSdepth.size() > AS2depth.size()) {
            
            largestArray = BFSdepth.size();
            
        }else if(AS1depth.size() > BFSdepth.size() && AS1depth.size() > AS2depth.size()) {
            
            largestArray = AS1depth.size();
            
        }else if(AS2depth.size() > BFSdepth.size() && AS2depth.size() > AS1depth.size()) {
            
            largestArray = AS2depth.size();
            
        }else if(BFSdepth.size() == AS1depth.size() && BFSdepth.size() == AS2depth.size()) {
            
            largestArray = BFSdepth.size();
        }
        
        for(int i = 0; i < largestArray; i++) {
            
            String AS1 = " ";
            String AS2 = " ";
            String BFS = " ";
                    
            if(AS1depth.size() > i)
                AS1 = AS1depth.get(i).toString();
            
            if(AS2depth.size() > i)
                AS2 = AS2depth.get(i).toString();
            
            if(BFSdepth.size() > i)
                BFS = BFSdepth.get(i).toString();
            
            System.out.print(i + "\t  | " + AS1 + "\t   | " + AS2 + "\t    | " + BFS + "\n");
            
            
        }
        
    }
    
    /**
     * Calculates the path solution for each search.
     * Uses the parent node of a node starting at the goal node.
     * 
     * @param goalNode - the node containing the goal state.
     * @return - returns array of nodes containing the path solution.
     */
    public ArrayList<Node<EightPuzzleBoard, EightPuzzleAction>> getPathSolution(Node<EightPuzzleBoard, EightPuzzleAction> goalNode) {
        
        Node<EightPuzzleBoard, EightPuzzleAction> currentNode = goalNode;
        
        ArrayList<Node<EightPuzzleBoard, EightPuzzleAction>> goalPath = new ArrayList<Node<EightPuzzleBoard, EightPuzzleAction>>();
        goalPath.add(goalNode);
        
        while(!currentNode.isRootNode()) {
            
            Node<EightPuzzleBoard, EightPuzzleAction> parentNode = currentNode.getParent();
            goalPath.add(parentNode);
            currentNode = parentNode;
        }
        
        return goalPath;
    }
    
    /**
     * A class containing a comparator for the priority queue
     * of the A-Star Search with the misplaced tiles heuristic.
     */
    class costComparatorAS1 implements Comparator<Node<EightPuzzleBoard, EightPuzzleAction>> {
    
        @Override
        public int compare(Node<EightPuzzleBoard, EightPuzzleAction> node1, Node<EightPuzzleBoard, EightPuzzleAction> node2) {

            EightPuzzleBoard b1 = node1.getState();
            EightPuzzleBoard b2 = node2.getState();

            int b1Val = b1.calcH1val(b1, problem.getGoalState());
            int b2Val = b2.calcH1val(b2, problem.getGoalState());
            
            double nodeOneCost = b1Val + node1.getPathCost();
            double nodeTwoCost = b2Val + node2.getPathCost();

            return (int)(nodeOneCost - nodeTwoCost);
        }
    }
    
    /**
     * A class containing a comparator for the priority queue
     * of the A-Star Search with the manhattan distance heuristic.
     */
    class costComparatorAS2 implements Comparator<Node<EightPuzzleBoard, EightPuzzleAction>> {
    
        @Override
        public int compare(Node<EightPuzzleBoard, EightPuzzleAction> node1, Node<EightPuzzleBoard, EightPuzzleAction> node2) {

            EightPuzzleBoard b1 = node1.getState();
            EightPuzzleBoard b2 = node2.getState();

            int b1Val = b1.calcH2val(b1, problem.getGoalState());
            int b2Val = b2.calcH2val(b2, problem.getGoalState());
            
            double nodeOneCost = b1Val + node1.getPathCost();
            double nodeTwoCost = b2Val + node2.getPathCost();

            return (int)(nodeOneCost - nodeTwoCost);
        }
    }
    
    
}



