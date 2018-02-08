Problem Statement:
To design an investment portfolio that is currently limited to stocks and mutual funds. Stocks and mutual funds can be bought and sold in various quantities and at various prices. Prices of each investment can be individually updated to reflect the markets price of that investment. The gain of all investments can be calculated and displayed to the user once prices have received an update. The investment portfolio can also be searched through using search criteria such as the symbol, keywords, and a price range. The data can be loaded and saved to a file during launch and exit to retain the original state of the investment portfolio. All user interaction with the program is done through a graphical user interface that is used to gather information from the user in a more simplistic manner, compared to the command line.

Assumptions and Limitations of Solution:
More thought and detail could be introduced to the new GUI.

User Guide:
To compile on the command line, navigate to the folder in which the package folder is held (a3). In this particular case:

Compile by typing: javac a3/*.java

Run by typing: java a3.Portfolio <filename.txt>

Test Plan:
Each menu that requires user input is designed to handle all types of input within reason. Test cases can involve:
-Entering integers for strings.
-Entering strings for integers.
-Leaving input blank.
-Entering an input that does not exist (E.G: symbol).
-Entering a negative value where not possible.
-Running the program with no file argument.
-Running the program with a file argument that does not contain a .txt extension.

Possible Improvements:
- Implementing a more elegant and intuitive GUI.