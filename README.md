# Signal Identification from Arrival Times

## Overview
This project implements an algorithm to identify signals in a set of arrival times and the number of buses (in this project you may think of it like buses coming to a bus stop for receiving signals) at each arrival time. The algorithm aims to find patterns in the arrival times and determine the number of bus lines based on these patterns.

## Running the Program
1. **Compilation**: Compile the program using the provided Makefile. Navigate to the project directory in the terminal and run `make`. This will compile the source files located in the `src/` directory and generate an executable named `stops`.

2. **Execution**: After compilation, execute the program by running `./stops` in the terminal.

3. **Input**: The program prompts the user to provide the expected number of bus lines and the final time to check. Providing a larger final time can improve the accuracy of the algorithm's predictions regarding the number of buses or signals.

## Functionality
- **Algorithm**: The core algorithm analyzes the arrival times of buses and their quantities to predict the number of bus lines.
  
- **Input Generation**: The program can generate random test cases with specified parameters (number of lines, final time, etc.) to evaluate the prediction algorithm.
  
- **Output**: The program provides information about the detected bus lines, including their arrival indexes in the time array, the period of the line, and the quantities before and after removal. Additionally, it prints the predicted number of lines based on the analysis.
