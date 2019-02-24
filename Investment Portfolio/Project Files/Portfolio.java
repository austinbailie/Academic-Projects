/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package a3;

import java.awt.BorderLayout;
import java.io.IOException;
import java.util.HashMap;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.text.DecimalFormat;
import java.util.ArrayList;
import javax.swing.BorderFactory;
import javax.swing.BoxLayout;
import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import javax.swing.JTextField;
import javax.swing.JTextPane;
import javax.swing.ScrollPaneConstants;
import javax.swing.border.EtchedBorder;
import javax.swing.border.TitledBorder;
/**
 *
 * @author austinbailie
 */
@SuppressWarnings("serial")
public class Portfolio extends JFrame {

    ArrayList<Investment> invList = new ArrayList<>();
    
    public static Portfolio portfolio = new Portfolio();
    
    DecimalFormat formatter = new DecimalFormat("#0.00");
    
    int updateIndex = 0;
    
    public static String fileName;

    /**
     * @param args the command line arguments
     * @throws java.lang.ClassNotFoundException
     */
    public static void main(String[] args) throws ClassNotFoundException {
        
        portfolio.setVisible(true);
        
        if(args.length == 0) {
            
            JOptionPane.showMessageDialog(portfolio, "A file should have been specified on the command line, the default \n"
                 + "file name 'investments.txt' will now be used.\n");
            
            fileName = "investments.txt";
            
        }else {
           
           fileName = args[0];
           portfolio.load();
        }
       
        portfolio.save();
        
    }
    
    /**
     * The creation of the structure of for the entire GUI, including Buy, Sell, Update, 
     * getGain, and Search operations. 
     */
    @SuppressWarnings({"Convert2Lambda", "OverridableMethodCallInConstructor", "unchecked", "Convert2Diamond"})
    public Portfolio() {
       
        this.setSize(new Dimension(800,600));
        this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);   
        this.setTitle("Investment Portfolio");
        this.setLayout(new BoxLayout(getContentPane(), BoxLayout.X_AXIS));
        
        Font font1 = new Font("SansSerif", Font.BOLD, 15);
        
        JPanel welcPanel = new JPanel();
        JPanel buyPanel = new JPanel();
        JPanel sellPanel = new JPanel();
        JPanel updatePanel = new JPanel();
        JPanel gainPanel = new JPanel();
        JPanel searchPanel = new JPanel();
           
        /* ==== COMMANDS MENU ================================================= */
        
        JMenuItem buyItem = new JMenuItem("Buy");
        JMenuItem sellItem = new JMenuItem("Sell");
        JMenuItem updateItem = new JMenuItem("Update");
        JMenuItem gainItem = new JMenuItem("Get Gain");
        JMenuItem searchItem = new JMenuItem("Search");
        JMenuItem quitItem = new JMenuItem("Quit");
        
        JMenu menu = new JMenu("Commands");
        menu.add(buyItem);
        menu.add(sellItem);
        menu.add(updateItem);
        menu.add(gainItem); 
        menu.add(searchItem);
        menu.add(quitItem);
        
        JMenuBar menuBar = new JMenuBar();
        menuBar.add(menu);
        menuBar.setPreferredSize(new Dimension(WIDTH, 30));
        
        this.setJMenuBar(menuBar);
        
        /* ==================================================================== */
        
        /* ==== WELCOME PANEL ================================================= */
        
        welcPanel.setLayout(new BorderLayout());
        
        Font font2 = new Font("SansSerif", Font.BOLD, 20);
        
        JTextPane pane = new JTextPane();
        pane.setEditable(false);
        pane.setFont(font2);
        
        pane.setText("\n\n\n\tWelcome to Investment Portfolio.\n\n\n\n"
                + "\tChoose a command from the 'Commands' menu\n"
                + "\tto buy or sell an investment, update prices for all investments, \n"
                + "\tget gain for the portfolio, search for relevant investments, \n"
                + "\tor quit the program.");
        
        welcPanel.add(pane, BorderLayout.CENTER);
        
        this.add(welcPanel);
        
        welcPanel.setVisible(true);
        
        /* ==================================================================== */
        
        /* ==== BUY PANEL ================================================= */
        
        buyPanel.setLayout(new BorderLayout());
        buyPanel.setBorder(new TitledBorder(new EtchedBorder(), "Buy Investments"));
        
        JPanel buyDetsPanel = new JPanel();
        buyDetsPanel.setLayout(new GridLayout(5, 2));
        
        JLabel lblType = new JLabel("\tType");
        JLabel lblSymbol = new JLabel("\tSymbol");
        JLabel lblName = new JLabel("\tName");
        JLabel lblQuantity = new JLabel("\tQuantity");
        JLabel lblPrice = new JLabel("\tPrice");
             
        JComboBox<String> investType = new JComboBox<String>();
        investType.addItem("Stock");
        investType.addItem("Mutual Fund");
        
        JTextField symbolField = new JTextField();
        symbolField.setFont(font1);
        
        JTextField nameField = new JTextField();
        nameField.setFont(font1);
        
        JTextField quanField = new JTextField();
        quanField.setFont(font1);
        
        JTextField priceField = new JTextField();
        priceField.setFont(font1);
        
        buyDetsPanel.add(lblPrice, 0,0);
        buyDetsPanel.add(priceField, 0,1);
        buyDetsPanel.add(lblQuantity, 1,0);
        buyDetsPanel.add(quanField, 1,1);
        buyDetsPanel.add(lblName, 2,0);
        buyDetsPanel.add(nameField, 2,1);
        buyDetsPanel.add(lblSymbol, 3,0);
        buyDetsPanel.add(symbolField, 3,1);
        buyDetsPanel.add(lblType, 4,0); 
        buyDetsPanel.add(investType, 4,1);
          
        buyPanel.add(buyDetsPanel, BorderLayout.WEST);
        
        JPanel btnPanel = new JPanel();
        btnPanel.setLayout(new BoxLayout(btnPanel, BoxLayout.Y_AXIS));
        
        JButton btnReset = new JButton("Reset");
        btnReset.setFont(font1);
        JButton btnBuy = new JButton(" Buy ");
        btnBuy.setFont(font1);
        
        btnPanel.add(btnReset);
        btnPanel.add(btnBuy);
        btnPanel.setBorder(BorderFactory.createEmptyBorder(100, 100, 100, 100));

        buyPanel.add(btnPanel, BorderLayout.EAST);
        
        JPanel msgPanel = new JPanel();
        msgPanel.setBorder(new TitledBorder(new EtchedBorder(), "Messages"));
        
        JTextArea msgArea = new JTextArea(15, 58);
        msgArea.setEditable(false);
        JScrollPane scroll = new JScrollPane(msgArea);
        scroll.setHorizontalScrollBarPolicy(ScrollPaneConstants.HORIZONTAL_SCROLLBAR_ALWAYS);
        scroll.setVerticalScrollBarPolicy(ScrollPaneConstants.VERTICAL_SCROLLBAR_ALWAYS);
        msgPanel.add(scroll);
        
        buyPanel.add(msgPanel, BorderLayout.SOUTH);
 
        this.add(buyPanel);
        
        buyPanel.setVisible(false);
        
        /* ==================================================================== */
        
        /* ==== SELL PANEL ================================================= */
        
        sellPanel.setLayout(new BorderLayout());
        sellPanel.setBorder(new TitledBorder(new EtchedBorder(), "Sell Investments"));
        
        JPanel sellDetsPanel = new JPanel();
        sellDetsPanel.setLayout(new GridLayout(3, 2));
        
        JTextField symbolFieldSell = new JTextField();
        symbolFieldSell.setColumns(10);
        symbolFieldSell.setFont(font1);
        
        JTextField quanFieldSell = new JTextField();
        quanFieldSell.setColumns(10);
        quanFieldSell.setFont(font1);
        
        JTextField priceFieldSell = new JTextField();
        priceFieldSell.setColumns(10);
        priceFieldSell.setFont(font1);
        
        JLabel lblPrice1 = new JLabel("\tPrice");
        JLabel lblQuantity1 = new JLabel("\tQuantity");
        JLabel lblSymbol1 = new JLabel("\tSymbol");
        
        sellDetsPanel.add(lblPrice1, 0,0);
        sellDetsPanel.add(priceFieldSell, 0,1);
        sellDetsPanel.add(lblQuantity1, 1,0);
        sellDetsPanel.add(quanFieldSell, 1,1);
        sellDetsPanel.add(lblSymbol1, 2,0);
        sellDetsPanel.add(symbolFieldSell, 2,1);
        
        sellPanel.add(sellDetsPanel, BorderLayout.WEST);
        
        JPanel btnPanelSell = new JPanel();
        btnPanelSell.setLayout(new BoxLayout(btnPanelSell, BoxLayout.Y_AXIS));
        
        JButton btnResetSell = new JButton("Reset");
        btnResetSell.setFont(font1);
        JButton btnSell = new JButton(" Sell ");
        btnSell.setFont(font1);
        
        btnPanelSell.add(btnResetSell);
        btnPanelSell.add(btnSell);
        btnPanelSell.setBorder(BorderFactory.createEmptyBorder(100, 100, 100, 100));

        sellPanel.add(btnPanelSell, BorderLayout.EAST);
        
        JPanel msgPanelSell = new JPanel();
        msgPanelSell.setBorder(new TitledBorder(new EtchedBorder(), "Messages"));
        
        JTextArea msgAreaSell = new JTextArea(15, 58);
        msgAreaSell.setEditable(false);
        scroll = new JScrollPane(msgAreaSell);
        scroll.setHorizontalScrollBarPolicy(ScrollPaneConstants.HORIZONTAL_SCROLLBAR_ALWAYS);
        scroll.setVerticalScrollBarPolicy(ScrollPaneConstants.VERTICAL_SCROLLBAR_ALWAYS);
        msgPanelSell.add(scroll);
        
        sellPanel.add(msgPanelSell, BorderLayout.SOUTH);
        
        this.add(sellPanel);
        
        sellPanel.setVisible(false);
        
        /* ==================================================================== */
        
        /* ==== UPDATE PANEL ================================================= */
        
        updatePanel.setLayout(new BorderLayout());
        updatePanel.setBorder(new TitledBorder(new EtchedBorder(), "Update Investments"));
        
        JPanel upDetsPanel = new JPanel();
        upDetsPanel.setLayout(new GridLayout(3, 2));
        
        JTextField symbolFieldUp = new JTextField();
        symbolFieldUp.setColumns(10);
        symbolFieldUp.setFont(font1);
        symbolFieldUp.setEditable(false);
        
        JTextField nameFieldUp = new JTextField();
        nameFieldUp.setColumns(10);
        nameFieldUp.setFont(font1);
        nameFieldUp.setEditable(false);
        
        JTextField priceFieldUp = new JTextField();
        priceFieldUp.setColumns(10);
        priceFieldUp.setFont(font1);
        
        JLabel lblPrice2 = new JLabel("\tPrice");
        JLabel lblName1 = new JLabel("\tName");
        JLabel lblSymbol2 = new JLabel("\tSymbol");
        
        upDetsPanel.add(lblPrice2, 0,0);
        upDetsPanel.add(priceFieldUp, 0,1);
        upDetsPanel.add(lblName1, 1,0);
        upDetsPanel.add(nameFieldUp, 1,1);
        upDetsPanel.add(lblSymbol2, 2,0);
        upDetsPanel.add(symbolFieldUp, 2,1);
        
        updatePanel.add(upDetsPanel, BorderLayout.WEST);
        
        JPanel btnPanelUp = new JPanel();
        btnPanelUp.setLayout(new BoxLayout(btnPanelUp, BoxLayout.Y_AXIS));
        
        JButton btnPrev = new JButton("Previous");
        btnPrev.setFont(font1);
        JButton btnNext = new JButton(" Next ");
        btnNext.setFont(font1);
        JButton btnSave = new JButton(" Save ");
        btnSave.setFont(font1);
        
        btnPanelUp.add(btnPrev);
        btnPanelUp.add(btnNext);
        btnPanelUp.add(btnSave);
        btnPanelUp.setBorder(BorderFactory.createEmptyBorder(80, 100, 100, 100));

        updatePanel.add(btnPanelUp, BorderLayout.EAST);
        
        JPanel msgPanelUp = new JPanel();
        msgPanelUp.setBorder(new TitledBorder(new EtchedBorder(), "Messages"));
        
        JTextArea msgAreaUp = new JTextArea(15, 58);
        msgAreaUp.setEditable(false);
        scroll = new JScrollPane(msgAreaUp);
        scroll.setHorizontalScrollBarPolicy(ScrollPaneConstants.HORIZONTAL_SCROLLBAR_ALWAYS);
        scroll.setVerticalScrollBarPolicy(ScrollPaneConstants.VERTICAL_SCROLLBAR_ALWAYS);
        msgPanelUp.add(scroll);
        
        updatePanel.add(msgPanelUp, BorderLayout.SOUTH);
        
        this.add(updatePanel);
        
        updatePanel.setVisible(false);
        
        /* ==================================================================== */
        
        /* ==== GAIN PANEL ================================================= */
        
        gainPanel.setLayout(new BorderLayout());
        gainPanel.setBorder(new TitledBorder(new EtchedBorder(), "Get Total Gain"));
        
        JPanel gainDetsPanel = new JPanel();
        gainDetsPanel.setLayout(new BoxLayout(gainDetsPanel, BoxLayout.X_AXIS));
        
        JLabel lblGainTotal = new JLabel("\tTotal Gain");
        
        JTextField gainField = new JTextField();
        gainField.setFont(font1);
        gainField.setEditable(false);
        
        gainDetsPanel.add(lblGainTotal);
        gainDetsPanel.add(gainField);
        gainDetsPanel.setBorder(BorderFactory.createEmptyBorder(50, 20, 20, 500));
        
        gainPanel.add(gainDetsPanel, BorderLayout.NORTH);
        
        JPanel msgPanelGain = new JPanel();
        msgPanelGain.setBorder(new TitledBorder(new EtchedBorder(), "Individual Gains"));
        
        JTextArea msgAreaGain = new JTextArea(15, 58);
        msgAreaGain.setEditable(false);
        scroll = new JScrollPane(msgAreaGain);
        scroll.setHorizontalScrollBarPolicy(ScrollPaneConstants.HORIZONTAL_SCROLLBAR_ALWAYS);
        scroll.setVerticalScrollBarPolicy(ScrollPaneConstants.VERTICAL_SCROLLBAR_ALWAYS);
        msgPanelGain.add(scroll);
        
        gainPanel.add(msgPanelGain, BorderLayout.SOUTH);
        
        this.add(gainPanel);
        
        gainPanel.setVisible(false);
        
        /* ==================================================================== */
        
        /* ==== SEARCH PANEL ================================================= */
        
        searchPanel.setLayout(new BorderLayout());
        searchPanel.setBorder(new TitledBorder(new EtchedBorder(), "Search Investments"));
        
        JPanel searchDetsPanel = new JPanel();
        searchDetsPanel.setLayout(new GridLayout(4, 2));
        
        JTextField symbolFieldSearch = new JTextField();
        symbolFieldSearch.setColumns(10);
        symbolFieldSearch.setFont(font1);
        
        JTextField keyFieldSearch = new JTextField();
        keyFieldSearch.setColumns(10);
        keyFieldSearch.setFont(font1);
        
        JTextField priceFieldLow = new JTextField();
        priceFieldLow.setColumns(10);
        priceFieldLow.setFont(font1);
        
        JTextField priceFieldHigh = new JTextField();
        priceFieldHigh.setColumns(10);
        priceFieldHigh.setFont(font1);
        
        JLabel lblLow = new JLabel("\tLow Price");
        JLabel lblHigh = new JLabel("\tHigh Price");
        JLabel lblKey = new JLabel("\tKeywords");
        
        JLabel lblSymbol3 = new JLabel("\tSymbol");
        
        searchDetsPanel.add(lblHigh, 0,0);
        searchDetsPanel.add(priceFieldHigh, 0,1);
        searchDetsPanel.add(lblLow, 1,0);
        searchDetsPanel.add(priceFieldLow, 1,1);
        searchDetsPanel.add(lblKey, 2,0);
        searchDetsPanel.add(keyFieldSearch, 2,1);
        searchDetsPanel.add(lblSymbol3, 3,0);
        searchDetsPanel.add(symbolFieldSearch, 3,1);
        
        searchPanel.add(searchDetsPanel, BorderLayout.WEST);
        
        JPanel btnPanelSearch = new JPanel();
        btnPanelSearch.setLayout(new BoxLayout(btnPanelSearch, BoxLayout.Y_AXIS));
        
        JButton btnResetSearch = new JButton("Reset");
        btnResetSearch.setFont(font1);
        JButton btnSearch = new JButton(" Search ");
        btnSearch.setFont(font1);
        
        btnPanelSearch.add(btnResetSearch);
        btnPanelSearch.add(btnSearch);
        btnPanelSearch.setBorder(BorderFactory.createEmptyBorder(100, 100, 100, 100));

        searchPanel.add(btnPanelSearch, BorderLayout.EAST);
        
        JPanel msgPanelSearch = new JPanel();
        msgPanelSearch.setBorder(new TitledBorder(new EtchedBorder(), "Search Results"));
        
        JTextArea msgAreaSearch = new JTextArea(15, 58);
        msgAreaSearch.setEditable(false);
        scroll = new JScrollPane(msgAreaSearch);
        scroll.setHorizontalScrollBarPolicy(ScrollPaneConstants.HORIZONTAL_SCROLLBAR_ALWAYS);
        scroll.setVerticalScrollBarPolicy(ScrollPaneConstants.VERTICAL_SCROLLBAR_ALWAYS);
        msgPanelSearch.add(scroll);
        
        searchPanel.add(msgPanelSearch, BorderLayout.SOUTH);
        
        this.add(searchPanel);
        
        searchPanel.setVisible(false);
        
        /* ==================================================================== */
        
        /* ==== COMMAND ACTIONS  ================================================= */
        
        buyItem.addActionListener(new ActionListener(){
            
            @Override
            public void actionPerformed(ActionEvent ae) {
                
                welcPanel.setVisible(false);
                buyPanel.setVisible(true);
                sellPanel.setVisible(false);
                updatePanel.setVisible(false);
                gainPanel.setVisible(false);
                searchPanel.setVisible(false);
                
                symbolField.setText("");
                nameField.setText("");
                quanField.setText("");
                priceField.setText("");
                msgArea.setText("");
            } 
        });
        
        sellItem.addActionListener(new ActionListener(){
            
            @Override
            public void actionPerformed(ActionEvent ae) {
                
                welcPanel.setVisible(false);
                buyPanel.setVisible(false);
                sellPanel.setVisible(true);
                updatePanel.setVisible(false);
                gainPanel.setVisible(false);
                searchPanel.setVisible(false);
                
                String out = portfolio.printInvest();
                msgAreaSell.setText(out);
            }  
        });
        
        updateItem.addActionListener(new ActionListener(){
            
            @Override
            public void actionPerformed(ActionEvent ae) {
                
                welcPanel.setVisible(false);
                buyPanel.setVisible(false);
                sellPanel.setVisible(false);
                updatePanel.setVisible(true);
                gainPanel.setVisible(false);
                searchPanel.setVisible(false);
                
                updateIndex = 0;
                btnPrev.setEnabled(false);
                
                if(invList.size() <= 1)
                    btnNext.setEnabled(false);
                
                if(!invList.isEmpty()) {
                
                    symbolFieldUp.setText(invList.get(updateIndex).getSymbol());
                    nameFieldUp.setText(invList.get(updateIndex).getName());
                
                }else {
                    
                    msgAreaUp.setText("Your investment portfolio is empty, please aquire investments to update.");
                }
            }
        });
        
        gainItem.addActionListener(new ActionListener(){
            
            @Override
            public void actionPerformed(ActionEvent ae) {
                
                welcPanel.setVisible(false);
                buyPanel.setVisible(false);
                sellPanel.setVisible(false);
                updatePanel.setVisible(false);
                gainPanel.setVisible(true);
                searchPanel.setVisible(false);
                
                if(invList != null) {

                    double totalGain = 0.0;
                    String gainOut;
                    
                    for(int i = 0; i < invList.size(); i++)
                        totalGain += invList.get(i).getGainVal();
                    
                    gainOut = String.valueOf(formatter.format(totalGain));
                    gainField.setText(gainOut);
                }
                
                String out = portfolio.getGain();
                msgAreaGain.setText(out);

            }   
        });
        
        searchItem.addActionListener(new ActionListener(){
            
            @Override
            public void actionPerformed(ActionEvent ae) {
                
                welcPanel.setVisible(false);
                buyPanel.setVisible(false);
                sellPanel.setVisible(false);
                updatePanel.setVisible(false);
                gainPanel.setVisible(false);
                searchPanel.setVisible(true);
                
                if(invList.isEmpty()) {
                    
                    msgAreaSearch.setText("Your portfolio contains no investments.");
                    btnSearch.setEnabled(false);
                
                }else {
                    
                    btnSearch.setEnabled(true);
                    msgAreaSearch.setText("");
                }
            }   
        });
        
        quitItem.addActionListener(new ActionListener(){
            
            @Override
            public void actionPerformed(ActionEvent ae) {
                
                portfolio.save();
                System.exit(0);
            }
            
        });
        
        /* ==================================================================== */
        
        /* ==== BUY ACTIONS  ================================================= */
        
        btnBuy.addActionListener(new ActionListener(){
            
            @Override
            public void actionPerformed(ActionEvent ae) {
                
                String out = portfolio.Buy(investType.getSelectedIndex(), symbolField.getText(), nameField.getText(), quanField.getText(), priceField.getText());
                out += portfolio.printInvest();
                msgArea.setText(out);
            }  
        });
        
        btnReset.addActionListener(new ActionListener(){
            
            @Override
            public void actionPerformed(ActionEvent ae) {
                
                symbolField.setText("");
                nameField.setText("");
                quanField.setText("");
                priceField.setText("");
            }
        });
        
        /* ==================================================================== */
        
        /* ==== SELL ACTIONS  ================================================= */
        
        btnSell.addActionListener(new ActionListener(){
            
            @Override
            public void actionPerformed(ActionEvent ae) {
                
                String out = portfolio.Sell(symbolFieldSell.getText(), quanFieldSell.getText(), priceFieldSell.getText());
                out += portfolio.printInvest();
                msgAreaSell.setText(out);
            }  
        });
        
        btnResetSell.addActionListener(new ActionListener(){
            
            @Override
            public void actionPerformed(ActionEvent ae) {
                
                symbolFieldSell.setText("");
                quanFieldSell.setText("");
                priceFieldSell.setText("");
            }
        });
        
        /* ==================================================================== */
        
        /* ==== UPDATE ACTIONS  ================================================= */
        
        btnPrev.addActionListener(new ActionListener(){
            
            @Override
            public void actionPerformed(ActionEvent ae) {
                
                --updateIndex;
                symbolFieldUp.setText(invList.get(updateIndex).getSymbol());
                nameFieldUp.setText(invList.get(updateIndex).getName());
                
                 if(updateIndex <= 0)
                    btnPrev.setEnabled(false);
                 else
                    btnPrev.setEnabled(true);
                  
                 if(updateIndex == invList.size() - 1)
                    btnNext.setEnabled(false);
                  else
                    btnNext.setEnabled(true);
                
            }  
        });
        
        btnNext.addActionListener(new ActionListener(){
            
            @Override
            public void actionPerformed(ActionEvent ae) {
                
                ++updateIndex;
                symbolFieldUp.setText(invList.get(updateIndex).getSymbol());
                nameFieldUp.setText(invList.get(updateIndex).getName());
                
                 if(updateIndex <= 0)
                    btnPrev.setEnabled(false);
                 else
                    btnPrev.setEnabled(true);
                  
                 if(updateIndex == invList.size() - 1)
                    btnNext.setEnabled(false);
                  else
                    btnNext.setEnabled(true);
            }  
        });
        
        btnSave.addActionListener(new ActionListener(){
            
            @Override
            public void actionPerformed(ActionEvent ae) {
                
                String out = portfolio.update(updateIndex, priceFieldUp.getText());
                msgAreaUp.setText(out);
                priceFieldUp.setText("");
            }  
        });
        
        /* ==================================================================== */
        
        /* ==== SEARCH ACTIONS  ================================================= */
        
        btnSearch.addActionListener(new ActionListener(){
            
            @Override
            public void actionPerformed(ActionEvent ae) {
                
                String out = portfolio.search(symbolFieldSearch.getText(), keyFieldSearch.getText(), priceFieldLow.getText(), priceFieldHigh.getText());
                msgAreaSearch.setText(out);
            }  
        });
        
        btnResetSearch.addActionListener(new ActionListener(){
            
            @Override
            public void actionPerformed(ActionEvent ae) {
                
                symbolFieldSearch.setText("");
                keyFieldSearch.setText("");
                priceFieldLow.setText("");
                priceFieldHigh.setText("");
                msgAreaSearch.setText("");
            }
        });
        
        /* ==================================================================== */
        
    }
    
    
   /**
    * Checks the validity of user given attributes, then creates the investment and 
    * inserts it into the ArrayList.
    * @param type - Represents if the type of investment to be made (stock or mutual fund).
    * @param symbol - The symbol representing the new investment.
    * @param name - The name representing the new investment.
    * @param quantity - The amount of investments being bought.
    * @param price - The price at which the investment is being bought.
    * @return output - A string containing success or error messages.
    */
    public String Buy(int type, String symbol, String name, String quantity, String price) {
        
        String output = "";
          
        int oldQuan;
        double oldValue;
        double newValue;
        String curName;            
        double bookValue;
        double gain = 0.0;
        
        int quanInt;
        
        try {
            
            quanInt = Integer.parseInt(quantity);
            
        }catch(NumberFormatException e) {
            
            return "ERROR: Unable to parse integer for quantity.";
        }
            
        double priceDub;
        
        try {
            
            priceDub = Double.parseDouble(price);
            
        }catch(NumberFormatException e) {
            
            return "ERROR: Unable to parse double for price.";
        }
         
        try {
            
            portfolio.symbolCheck(symbol);
            portfolio.nameCheck(name);
            portfolio.quantityCheck(quanInt);
            portfolio.priceCheck(priceDub);
        
        }catch(Customexceptions m) {
            
            return m.getMessage();
        }
         
        Investment symInvest;
        try {
            
            symInvest = new Investment(symbol, null, 0, 0.0, 0.0, 0.0) {};
                
        } catch(Customexceptions m) { return m.getMessage(); }

        if(invList.contains(symInvest)) {

            output = ("ALERT: This investment already exists, any new investment will be added to your current investment.\n");
        }

        if(type == 0) {
        
            bookValue = (quanInt * priceDub) + 9.99;
            Stock tempStock;
                
                try {
                
                    tempStock = new Stock(symbol, name, quanInt, priceDub, bookValue, gain);
                
                } catch(Customexceptions m) { return m.getMessage(); }
        
            if(invList.contains(tempStock)) {

                int index = invList.indexOf(tempStock);
                
                if(invList.get(index).getClass().getCanonicalName().equals("a3.Stock")) {
                
                    oldValue = invList.get(index).getBookVal();
                    oldQuan = invList.get(index).getQuantity();
                    curName = invList.get(index).getName();
                    gain = invList.get(index).getGainVal();

                    newValue = oldValue + bookValue;
                    quanInt = quanInt + oldQuan;
                    Stock newStock;
                
                    try {

                       newStock = new Stock(symbol, curName, quanInt, priceDub, newValue, gain);

                    } catch(Customexceptions m) { return m.getMessage(); }

                    invList.set(index, newStock);
                    output += "Additional Stocks bought succesfully!";
                
                }else if(invList.get(index).getClass().getCanonicalName().equals("a3.MutualFund")) {
                    
                    System.out.println("\nERROR: An investment with this symbol already exists in Mutual Funds, please enter a different stock.");
                }
                
            }else {

                invList.add(tempStock);
                output += "Stock bought succesfully!";
            }    
        }
        
        if(type == 1) {
            
            bookValue = (quanInt * priceDub);
            MutualFund tempMF;
                
            try {

               tempMF = new MutualFund(symbol, name, quanInt, priceDub, bookValue, gain);

            } catch(Customexceptions m) { return m.getMessage(); }
          
            if(invList.contains(tempMF)) {

                int index = invList.indexOf(tempMF);
                
                if(invList.get(index).getClass().getCanonicalName().equals("a3.MutualFund")) {
                
                    oldValue = invList.get(index).getBookVal();
                    oldQuan = invList.get(index).getQuantity();
                    curName = invList.get(index).getName();
                    gain = invList.get(index).getGainVal();

                    newValue = oldValue + bookValue;
                    quanInt = quanInt + oldQuan;
                    
                    MutualFund newMF;
                
                    try {

                        newMF = new MutualFund(symbol, curName, quanInt, priceDub, newValue, gain);

                    } catch(Customexceptions m) { return m.getMessage(); }

                    invList.set(index, newMF);
                    output += "Additional Mutual Funds bought succesfully!";
                
                }else if(invList.get(index).getClass().getCanonicalName().equals("a3.Stock")) {
                    
                    System.out.println("\nERROR: An investment with this symbol already exists in Stocks, please enter a different mutual fund.");
                }
                
            }else {

                invList.add(tempMF);
                output += "Mutual Fund bought succesfully!";
            }
        }  
        
        if(output == null)
            output = "ERROR: Unsuccessful in buying investments, please try again.";
        
        
        return output;
    }
    
    
   /**
    * Gathers required attributes from user, then resets quantity, price, and bookValue in
    * the ArrayList accordingly.
    * @param symbol - The symbol of the investment specified by the user.
    * @param quantity - The name of the investment specified by the user.
    * @param price - The price at which the user would like to sell the investment.
    * @return output - A formatted string containing payment received or error message.
    */
    public String Sell(String symbol, String quantity, String price) {
        
        String output;
        int index;
        
        Investment symInvest;
        int oldQuan;
        
        int quanInt;
        
        try {
            
            quanInt = Integer.parseInt(quantity);
            
        }catch(NumberFormatException e) {
            
            return "ERROR: Unable to parse integer for quantity.";
        }
        
        double priceDub;
        
        try {
            
            priceDub = Double.parseDouble(price);
            
        }catch(NumberFormatException e) {
            
            return "ERROR: Unable to parse double for price.";
        }
       
        try {
            
            portfolio.symbolCheck(symbol);
            portfolio.quantityCheck(quanInt);
            portfolio.priceCheck(priceDub);
            
            symInvest = new Investment(symbol, null, 0, 0.0, 0.0, 0.0) {};
        
        }catch(Customexceptions m) {
            
            return m.getMessage();
        }
               
        if(!invList.contains(symInvest)) 
            return ("ERROR: No investments with this symbol exist, please try again.");
            
       index = invList.indexOf(symInvest);
       oldQuan = invList.get(index).getQuantity();

       if(oldQuan < quanInt)
           return("ERROR: Quantity entered is greater than quantity owned, please try again.");


        double bookValue;
        double payment = 0.0;
        double oldVal;
        String curName;
        int newQuan;
        double quanRatio;
        double gain;
           
        if(oldQuan - quanInt != 0) {

            newQuan = oldQuan - quanInt;
            quanRatio = (double)newQuan / (double)oldQuan;
            oldVal = invList.get(index).getBookVal();
            bookValue = oldVal * quanRatio;
            curName = invList.get(index).getName();
            gain = invList.get(index).getGainVal();
            
            if(invList.get(index).getClass().getCanonicalName().equals("a3.MutualFund")) {

                payment = (quanInt * priceDub) - 45;
                MutualFund newMF;
                
                try {
                
                    newMF = new MutualFund(symbol, curName, newQuan, priceDub, bookValue, gain);
                
                } catch(Customexceptions m) { return m.getMessage(); }
                           
                invList.set(index, newMF);
            
            }else if(invList.get(index).getClass().getCanonicalName().equals("a3.Stock")) {
                
                payment = (quanInt * priceDub) - 9.99;
                Stock newStock;
                
                try {
                
                   newStock = new Stock(symbol, curName, newQuan, priceDub, bookValue, gain);
                
                } catch(Customexceptions m) { return m.getMessage(); }
             
                invList.set(index, newStock);
            }

        }else {

            if(invList.get(index).getClass().getCanonicalName().equals("a3.MutualFund")) {
                
                payment = (quanInt * priceDub) - 45;
            
            }else if(invList.get(index).getClass().getCanonicalName().equals("a3.Stock")) {
                
                payment = (quanInt * priceDub) - 9.99;
            }
            
            invList.remove(index);
        }
        
        output = ("Received Payment: $" + formatter.format(payment));
        
        if(output == null)
            output = "ERROR: Unsuccessful in selling invesments, please try again.";
   
        return output;
    }
    
    /**
    * Adjusts price of specific investment given by user,
    * then calculates gain and resets price into the array list.
    * @param index - Location of the currently displayed investment within the ArrayList,
    * calculated by the Next and Previous buttons.
    * @param price - The updated price specified by the user.
    * @return output - A string containing success or error messages.
    */
    public String update(int index, String price) {
        
        String symbol;
        String name;
        int quantity;
        double bookValue;
        double gain;
        String output = null;
        
        if(invList != null) {
                   
            double priceDub;

            try {

                priceDub = Double.parseDouble(price);

            }catch(NumberFormatException e) {

                return "ERROR: Unable to parse double for price.";
            }

            try {
                
                portfolio.priceCheck(priceDub);
                
            } catch(Customexceptions m) { return m.getMessage(); }
             

            symbol = invList.get(index).getSymbol();
            name = invList.get(index).getName();
            quantity = invList.get(index).getQuantity();
            bookValue = invList.get(index).getBookVal();

            if(invList.get(index).getClass().getCanonicalName().equals("a3.Stock")) {

                gain = ((quantity * priceDub) - 9.99) - bookValue;
                Stock tempStock;
                
                try {
                
                   tempStock = new Stock(symbol, name, quantity, priceDub, bookValue, gain);
                
                } catch(Customexceptions m) { return m.getMessage(); }
             
                invList.set(index, tempStock); 
                output = "Stock price succesfully updated!\n\n" + invList.get(index);

            }else if(invList.get(index).getClass().getCanonicalName().equals("a3.MutualFund")) {

                gain = ((quantity * priceDub) - 45) - bookValue;
                MutualFund tempMF;
                
                try {
                
                    tempMF = new MutualFund(symbol, name, quantity, priceDub, bookValue, gain);
                
                } catch(Customexceptions m) { return m.getMessage(); }
                
                invList.set(index, tempMF);    
                output = "Mutual Fund price succesfully updated!\n\n" + invList.get(index);
            }
        }  
           
        return output;
    }
    
    /**
    * Calculates the total gain of all investments 
    * by adding the calculated gain on each investment.
    * @return - A formatted string containing all investments and their individual gains.
    */
    public String getGain() {
        
        String gainOut = "";
        
        if(invList != null) {
            
            for(int i = 0; i < invList.size(); i++) {
                 
                gainOut += invList.get(i) + "  Gain: " + formatter.format(invList.get(i).getGainVal()) + "\n";
            }
        }
        
        return gainOut;
    }
    
    /**
     * Searches investments list for investments matching specific input from user.
     * @param symbol - A specific symbol input specified by the user.
     * @param keywords - A specific keyword input specified by the user.
     * @param lowPrice - A specific low-end input specified by the user.
     * @param highPrice - A specific high-end input specified by the user.
     * @return - A formatted string of all investments matching the search criteria.
    */
    public String search(String symbol, String keywords, String lowPrice, String highPrice) {
        
        double lowerBound = -1;
        double upperBound = -1;
        double invPrice;
        
        boolean symCheck = true;
        boolean keyCheck = true;
        boolean rangeCheck = true;
        
        String output = "";
              
        HashMap<String, ArrayList<Integer>> indexHM = new HashMap<>();
        
        String delimiters = "[, ]+";
        
        for (int j = 0; j < invList.size(); j++) {
            
            String nameTokens[] = invList.get(j).getName().split(delimiters);
                    
            for(String tokens : nameTokens) {
                
                indexHM.putIfAbsent(tokens.toLowerCase(), new ArrayList<>());
                
                indexHM.get(tokens.toLowerCase()).add(j);                
            }   
        }
        
        if(symbol.isEmpty())
            symCheck = false;
        
        String[] keys = keywords.split(delimiters);
        
        if(keys[0].isEmpty())
            keyCheck = false;
        
               
        try {
            
            if(!lowPrice.isEmpty() && !highPrice.isEmpty() ) {
            
                lowerBound = Double.parseDouble(lowPrice);
                
                if(lowerBound < 0)
                    return("ERROR: Price must be positive, this data will not be used for the search.");
        
                upperBound = Double.parseDouble(highPrice);
                
                if(upperBound < 0)
                    return("ERROR: Price must be positive, this data will not be used for the search.");
                
            }else if(!lowPrice.isEmpty() && highPrice.isEmpty()) {
                
                lowerBound = upperBound = Double.parseDouble(lowPrice);   
                
                if(lowerBound < 0)
                    return("ERROR: Price must be positive, this data will not be used for the search.");
                          
            }else {
                
                rangeCheck = false;
            }
            
        }catch (NumberFormatException e) {
            
            return("ERROR: Invalid input, this data will not be used for the search.");
        }
        
        if(lowerBound > upperBound)
            return "ERROR: Low-end price cannot be greater than high-end price.";
        
        output += "Investments matching your search criteria: \n";
        
        if(keyCheck) {
            
            ArrayList<Integer> resultIndex = new ArrayList<>();
            
            
            if(keys.length <= 1) {
                
                if(indexHM.containsKey(keys[0].toLowerCase())) 
                    resultIndex.addAll(indexHM.get(keys[0].toLowerCase()));
                
            }else {
            
                for(int k = 0; k < keys.length - 1; k++) {

                    if(indexHM.containsKey(keys[k].toLowerCase())) 
                        resultIndex.addAll(indexHM.get(keys[k].toLowerCase()));    
                
                }
                
                if(resultIndex.size() > 0)
                    resultIndex.retainAll(indexHM.get(keys[keys.length - 1].toLowerCase()));
            }
           
            for(Integer i : resultIndex) {
                
                if(!symCheck)              
                    symbol = invList.get(i).getSymbol();
                
                if(!rangeCheck)             
                    upperBound = lowerBound = invList.get(i).getPrice();
                    
                if(symbol.equalsIgnoreCase(invList.get(i).getSymbol())) {
                    
                    invPrice = invList.get(i).getPrice();
                    
                    if(invPrice >= lowerBound && invPrice <= upperBound) {
                        
                        if(invList.get(i).getClass().getCanonicalName().equals("a3.MutualFund")) {
                        
                            output += "MF: " + invList.get(i) + "\n";
                        
                        }else if(invList.get(i).getClass().getCanonicalName().equals("a3.Stock")) {
                            
                            output += "S: " + invList.get(i) + "\n";
                        }
                    }              
                }       
            }
            
        }else {
            
            for(int i = 0; i < invList.size(); i++) {

                if(!symCheck) {

                    symbol = invList.get(i).getSymbol(); 
                }

                if(!rangeCheck) {

                    lowerBound = invList.get(i).getPrice();
                    upperBound = invList.get(i).getPrice();
                }

                if(symbol.equalsIgnoreCase(invList.get(i).getSymbol())) {
                  
                    invPrice = invList.get(i).getPrice();

                    if(invPrice >= lowerBound && invPrice <= upperBound) {

                        if(invList.get(i).getClass().getCanonicalName().equals("a3.MutualFund")) {

                            output += "MF: " + invList.get(i) + "\n";

                        }else if(invList.get(i).getClass().getCanonicalName().equals("a3.Stock")) {

                            output += "S: " + invList.get(i) + "\n";
                        }
                    }                      
                }            
            }
        }
        
        return output;
    }  
    
    /**
     * Checks symbol for correct input, throws a custom exception if input is invalid.
     * @param symbol - The variable to be checked.
     * @throws Customexceptions - A custom exception with a error message specific to
     * the kind of check taking place.
     */
    public void symbolCheck(String symbol) throws Customexceptions {
        
        if(symbol == null || symbol.isEmpty())
            throw new Customexceptions("ERROR: Invalid symbol entry, please try again."); 
    }
    
    /**
     * Checks name for correct input, throws a custom exception if input is invalid.
     * @param name - The variable to be checked.
     * @throws Customexceptions - A custom exception with a error message specific to
     * the kind of check taking place.
     */
    public void nameCheck(String name) throws Customexceptions {
        
        if(name == null || name.isEmpty())
            throw new Customexceptions("ERROR: Invalid name entry, please try again."); 
    }
   
    /**
     * Checks quantity for correct input, throws a custom exception if input is invalid.
     * @param quantity - The variable to be checked.
     * @throws Customexceptions - A custom exception with a error message specific to
     * the kind of check taking place.
     */
    public void quantityCheck(int quantity) throws Customexceptions {
        
        if(quantity < 1)
            throw new Customexceptions("ERROR: Invalid quantity entry, quantity must be greater than 1."); 
    }
    
    /**
     * Checks price for correct input, throws a custom exception if input is invalid.
     * @param price - The variable to be checked.
     * @throws Customexceptions - A custom exception with a error message specific to
     * the kind of check taking place.
     */
    public void priceCheck(double price) throws Customexceptions {
        
        if(price <= 0)
            throw new Customexceptions("ERROR: Invalid price entry, price must be greater than 0."); 
    }
    
    /**
     * Used to create a formatted string of all investments
     * that are displayed in buy and sell message areas.
     * @return output - A formatted string containing all investments.
     */
    public String printInvest() {
        
        String output;
        
        output = "\n\nCurrent Portfolio Investments: \n";
        
        for(int i = 0; i < invList.size(); i++) {
            
            if(invList.get(i).getClass().getCanonicalName().equals("a3.Stock")) {
            
                output += "S: " + invList.get(i) + "\n";  
            }
            
            if(invList.get(i).getClass().getCanonicalName().equals("a3.MutualFund")) {
            
                output += "MF: " + invList.get(i) + "\n";  
            } 
        }
        
        if(invList.isEmpty())
            output += "Your investment portfolio is empty.\n";
        
        return output;
    }
    
     /**
     * Saves the ArrayList object to a file
     * when the user exits the program.
     */
    @SuppressWarnings("ConvertToTryWithResources")
    public void save() {
       
        try {
        
            FileOutputStream fileOut = new FileOutputStream(fileName);
            ObjectOutputStream objOut = new ObjectOutputStream(fileOut);
            objOut.writeObject(invList);
            objOut.close();
            
        }catch(IOException e) {
            
            System.out.println("CONSOLE: Failed to write file.");
        } 
    }
    
    /**
     * Loads data of an ArrayList object from a file
     * on program start up.
     * @throws ClassNotFoundException
     */
    @SuppressWarnings({"unchecked", "ConvertToTryWithResources"})
    public void load() throws ClassNotFoundException {
        
        try {
        
            FileInputStream fileIn = new FileInputStream(fileName);
            ObjectInputStream objIn = new ObjectInputStream(fileIn);
            invList = (ArrayList<Investment>)objIn.readObject();
            objIn.close();         
                      
        }catch(IOException e) {
            
           
        }
    }
    
}
