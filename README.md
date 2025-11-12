The Stock Analyzer is a C-based program designed to process and analyze stock market data efficiently from text files. It reads structured stock data, computes key insights such as price trends, averages, highs, and lows, and displays them in a formatted table for quick evaluation.

This project demonstrates:

File handling and data parsing in C

Data structure usage for record management

Modular programming (multiple .c files for analysis and display)

Command-line based report generation for real-world stock datasets

Files Overview:

stock_analyzer.c → Main program logic for stock analysis

stock_2.c → Alternate or extended analysis functions

table.c → Handles formatted data display

stock_data.txt → Sample dataset for analysis

analyze_stock.c → Supporting or experimental analysis module


Commant Used:

gcc stock_analyzer.c -o stock_analyzer
./stock_analyzer
