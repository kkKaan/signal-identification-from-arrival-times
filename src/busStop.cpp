#include "../include/busStop.hpp"

// The older algorithm
// Function to divide arrival times into periods of 'x' minutes and remove single buses
void divideAndRemove(std::vector<int>& arrival_times, std::vector<int>& number_of_buses, int x) 
{
    int times_count = 1;
    int n = arrival_times.size(); // Get the initial number of elements in the vectors, and treat it as the period and start point of that line

    for (int i = 0; i < n; ++i)
    {
        if (arrival_times[i] == times_count * x)
        {
            ++times_count; // Increment the times_count as we move to the next 'x' minute period
            if (number_of_buses[i] > 1)
            {
                // If there are more than one bus at this minute, reduce the quantity
                --number_of_buses[i];
            }
            else
            {
                // If there is only one bus at this minute, remove this entry from both vectors
                arrival_times.erase(arrival_times.begin() + i);
                number_of_buses.erase(number_of_buses.begin() + i);
                --n;
                --i;
            }
        }
        else if (arrival_times[i] > times_count * x)
        {
            // If the current arrival time is beyond the end of the 'x' minute period, move to the next period
            ++times_count;
        }
    }
}

// This function works as a runner for the divideAndRemove function
int findTheNumOfLines(std::vector<int> arrival_times, std::vector<int> number_of_buses)
{
    int res = 0;
    while (arrival_times.size())
    {
        if (arrival_times[0] > 45) break;
        divideAndRemove(arrival_times, number_of_buses, arrival_times[0]);
        ++res;
    }
    return res;
}

// Creates an upper triangular matrix that contains each element's difference with the next elements in arrival_times array
void createDiffMatrix(std::vector<int> arrival_times, std::vector<std::vector<int>>& diffs)
{
    int n = arrival_times.size();
    for (int i = 0; i < n; ++i)
    {
        for (int j = i + 1; j < n; ++j)
        {
            diffs[i][j] = arrival_times[j] - arrival_times[i];
        }
    }
}

// A modified find function, also checks the quantity, returns the index if the element is found, else -1
int isInTheLine(std::vector<int> diffs, int difference, std::vector<int> quantities)
{
    int n = diffs.size();
    for (int i = 0; i < n; ++i)
    {
        if (diffs[i] == difference && quantities[i]) return i;
    }
    return -1;
}

// A function to adjust the quantity matrix, if the quantity to be decreased is more than 0 it decreases
int removeLinesIfPossible(std::vector<std::vector<int>>& diffs, std::vector<int>& quantities, std::vector<int>& stops)
{
    int n = stops.size();
    for (int i = 0; i < n; ++i)
    {
        if (quantities[stops[i]]) --quantities[stops[i]];
    }
    return 1;
}

int findBusLines(const std::vector<int>& arrival_times, std::vector<int>& quantities)
{
    // const int MAX_DIFFS = 6; // Maximum number of signals in an hour
    // const int MIN_DIFF_COUNT = 5; // Minimum identical differences to consider as a line
    const int MIN_INTERVAL = 5; // Minimum interval to consider as a line
    
    int n = arrival_times.size();

    std::vector<std::vector<int>> diffs(n, std::vector<int>(n, 0));

    createDiffMatrix(arrival_times, diffs);

    int lineCount = 0;

    for (int i = 0; i < diffs.size(); ++i)
    {
        for (int j = i + 1; j < n; ++j)
        {
            int difference = diffs[i][j];
            std::vector<int> stops; // Vector to store the indexes of stops in the same line
            
            stops.push_back(i);

            int nextLine = isInTheLine(diffs[i], difference, quantities);
            while (nextLine != -1)
            {
                stops.push_back(nextLine);
                nextLine = isInTheLine(diffs[nextLine], difference, quantities);
            }

            // If the number of stops is less than the minimum interval, continue to the next potential line
            if (stops.size() < MIN_INTERVAL) continue;

            // Display information about the detected line
            std::cout << "Quantities before removal: ";
            for (int b = 0; b < quantities.size(); ++b)
            {
                std::cout << quantities[b] << " ";
            }
            std::cout << std::endl;

            std::cout << "The line's arrival indexes in time array: ";
            for (int a = 0; a < stops.size(); ++a)
            {
                std::cout << stops[a] << " ";
            }
            std::cout << "--> Period of the line: " << difference << std::endl;

            // Remove the detected lines if possible and update the quantities
            removeLinesIfPossible(diffs, quantities, stops);

            std::cout << "Quantities after removal:  ";
            for (int b = 0; b < quantities.size(); ++b)
            {
                std::cout << quantities[b] << " ";
            }
            std::cout << std::endl << "--------------------------------------------" << std::endl;
           
            ++lineCount;
            break; // Move to the next potential line
        }
    }
    return lineCount;
}

// Function to generate random numbers between some min and max values (assuming max > min)
int generateRandomInBetween(int minr, int maxr)
{
    return minr + rand() % (maxr - minr + 1);
}

// Function to generate random tests
void generateRandomTests(int numberOfLines, int final_time, std::vector<int>& arrival_times, std::vector<int>& quantity_of_buses)
{
    arrival_times.clear();
    quantity_of_buses.clear();

    srand(static_cast<unsigned>(time(0))); // Seed for random number generator

    // Adjust these values to your desired testing coverage
    const int MIN_PERIOD = 20;
    const int MAX_PERIOD = 50;
    const int MIN_BUSES = 5;
    const int MAX_BUSES = 10;

    // Generate random starting times for each line
    std::vector<int> startingTimes;
    for (int i = 0; i < numberOfLines; ++i)
    {
        startingTimes.push_back(generateRandomInBetween(0, final_time / 2)); // "final_time / 2" is a design choice, you may change that
    }

    // Generate random quantities (number of buses) for each line
    std::vector<int> quantities;
    for (int i = 0; i < numberOfLines; ++i)
    {
        quantities.push_back(generateRandomInBetween(MIN_BUSES, MAX_BUSES));
    }

    // Generate random periods (time between bus arrivals) for each line
    std::vector<int> periods;
    for (int i = 0; i < numberOfLines; ++i)
    {
        periods.push_back(generateRandomInBetween(MIN_PERIOD, MAX_PERIOD));
    }

    // Display the generated information for each line
    for (int i = 0; i < numberOfLines; ++i)
    {
        std::cout << "Line-" << i << "---> " << "Starting time: " << startingTimes[i] << " | " << "Period: " << periods[i] << " | " << "Max quantity: " << quantities[i] << std::endl;
    }

    std::vector<std::vector<int>> arrivalsArray;

    // Generate arrival times for each line and store them in the 'arrivalsArray'
    for (int i = 0; i < numberOfLines; ++i)
    {
        std::vector<int> currentTimes;
        for (int j = 0, currentTime = startingTimes[i]; j < quantities[i] && currentTime < final_time; ++j, currentTime += periods[i])
        {
            currentTimes.push_back(currentTime);
        }
        arrivalsArray.push_back(currentTimes);
    }

    // Merge arrivalsArray in increasing order, also add to the quantity_of_buses, if there are intersections increase the amount in the quantity array 
    while (!arrivalsArray.empty())
    {
        int curMin = INT_MAX;
        int curQuant = 0;

        // Find the minimum arrival time and its quantity across all lines
        for (int i = 0; i < arrivalsArray.size(); ++i)
        {
            if (arrivalsArray[i][0] < curMin)
            {
                curMin = arrivalsArray[i][0];
                curQuant = 1; // Reset quantity to 1 for the new minimum
            }
            else if (arrivalsArray[i][0] == curMin)
            {
                ++curQuant; // Increment quantity for buses arriving at the same time
            }
        }

        // Update 'arrivalsArray' and remove processed entries
        for (int i = 0; i < arrivalsArray.size(); ++i)
        {
            if (find(arrivalsArray[i].begin(), arrivalsArray[i].end(), curMin) != arrivalsArray[i].end())
            {
                arrivalsArray[i].erase(arrivalsArray[i].begin()); // Remove the processed arrival time
            }
            if (arrivalsArray[i].empty())
            {
                arrivalsArray.erase(arrivalsArray.begin() + i); // Remove lines that have no more arrivals
            }
        }

        // Add the minimum arrival time to 'arrival_times' and the quantity to 'quantity_of_buses'
        arrival_times.push_back(curMin);
        quantity_of_buses.push_back(curQuant);
    }
}

int main()
{
    // assuming at time t = 0, each bus came and go
    // periods 7 15 20 --> { 7, 14, 15, 20, 21, 28, 30, 35, 40, 42, 45, 49, 56, 60, 63, 70, 75, 77, 80, 84, 90, 91, 98, 100 } --- { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }
    // periods 7 9 20 --> { 7, 9, 14, 18, 20, 21, 27, 28, 35, 36, 40, 42, 45, 49, 54, 56 } --- { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }
    // periods 8 12 24 --> { 8, 12, 16, 24, 32, 36, 40, 48, 56, 60, 64, 72, 80, 84, 88, 96, 104, 108, 112, 120 } --- { 1, 1, 1, 3, 1, 1, 1, 3, 1, 1, 1, 3, 1, 1, 1, 3, 1, 1, 1, 3 }
    // periods 8 12 --> { 8, 12, 16, 24, 32, 36, 40, 48, 56 } --- { 1, 1, 1, 2, 1, 1, 1, 2, 1 }

    // busses may not come at t = 0, so their starting points may not be the period
    // periods 8 45 --> {8, 10, 16, 24, 32, 40, 48, 55, 56} --- { 1, 1, 1, 1, 1, 1, 1, 1, 1 }

    // max 6 in an hour, should be appear in 5 different hours
    // 60 * 24 until 1440 --> periods 20 30 40 --> { 20, 30, 40, 60*, 80, 90, 100, 120*, 140, 150, 160, 180*, 200, 210, 220, 240*, 260, 270, 280, 300*, 320, 330, 340, 360*, 380, 400 }

    // may start between some times
    // periods 25 40 50 (50 starts at time t=20) --> { 20, 25, 40, 50, 70, 75, 80, 100, 120, 125, 150, 160, 170, 175, 200, 220, 225, 240, 250, 270, 280 }
    //                                           --- { 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1 }
    // periods 21 (t=12), 36 (t=19), 43 (t=8)    --> { 8, 12, 19, 33, 36, 51, 54, 55, 75, 91, 94, 96, 117, 127, 137, 138, 159, 163, 180, 199, 201, 222, 223, 235, 243, 264, 266, 271 }
    //                                           --- { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1 }
    
    // -----------------------------------------------------------------------------------------

    // std::vector<int> arrival_times{ 8, 12, 19, 33, 36, 51, 54, 55, 75, 91, 94, 96, 117, 127, 137, 138, 159, 163, 180, 199, 201, 222, 223, 235, 243, 264, 266, 271 };
    // std::vector<int> number_of_buses{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1 };

    // int n = findBusLines(arrival_times, number_of_buses);
    // std::cout << std::endl << "The predicted number of lines: " << n << std::endl;

    // -----------------------------------------------------------------------------------------

    std::vector<int> arrival_times;
    std::vector<int> number_of_buses;

    int num_lines;
    int time_until;

    std::cout << "Expected number of lines: ";
    std::cin >> num_lines;

    std::cout << "The final time to check: ";
    std::cin >> time_until;

    generateRandomTests(num_lines, time_until, arrival_times, number_of_buses);

    std::cout << "Arrival times: ";
    for (int i = 0; i < arrival_times.size(); ++i)
    {
        std::cout << arrival_times[i] << " ";
    }
    std::cout << std::endl;

    std::cout << "Number of busses: ";
    for (int i = 0; i < number_of_buses.size(); ++i)
    {
        std::cout << number_of_buses[i] << " ";
    }
    std::cout << std::endl;

    std::cout << "----------------------------------------------------------------------------------------" << std::endl;

    int n = findBusLines(arrival_times, number_of_buses);
    std::cout << std::endl << "The predicted number of lines: " << n << std::endl;

    return 0;
}
