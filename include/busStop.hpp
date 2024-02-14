#ifndef BUSSTOP_HPP
#define BUSSTOP_HPP

#include <iostream>
#include <vector>
#include <unordered_map>
#include <ctime>
#include <cstdlib>
#include <algorithm>

// Function to divide arrival times into periods of 'x' minutes and remove single buses
void divideAndRemove(std::vector<int>& arrival_times, std::vector<int>& number_of_buses, int x);

// This function works as a runner for the divideAndRemove function
int findTheNumOfLines(std::vector<int> arrival_times, std::vector<int> number_of_buses);

// Creates a matrix in upper triangular form that contains each element's difference with the next elements in arrival_times array
void createDiffMatrix(std::vector<int> arrival_times, std::vector<std::vector<int>>& diffs);

// A modified find function, also checks the quantity, returns the index if the element is found, else -1
int isInTheLine(std::vector<int> diffs, int difference, std::vector<int> quantities);

// A function to adjust the quantity matrix, if the quantity to be decreased is more than 0 it decreases
int removeLinesIfPossible(std::vector<std::vector<int>>& diffs, std::vector<int>& quantities, std::vector<int>& stops);

// Function to find bus lines based on arrival times and quantities of buses
int findBusLines(const std::vector<int>& arrival_times, std::vector<int>& quantities);

// Function to generate random tests
void generateRandomTests(int number_of_lines, int final_time, std::vector<int>& arrival_times, std::vector<int>& quantity_of_buses);

// Function to generate random numbers between some min and max values (assuming max > min)
int generateRandomInBetween(int minr, int maxr);

#endif // BUSSTOP_HPP