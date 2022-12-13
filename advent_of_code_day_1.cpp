/*
--- Day 1: Calorie Counting ---
Santa's reindeer typically eat regular reindeer food, but they need a lot of magical energy to deliver presents on Christmas. 
For that, their favorite snack is a special type of star fruit that only grows deep in the jungle. The Elves have brought you on their annual expedition to the grove where the fruit grows.

To supply enough magical energy, the expedition needs to retrieve a minimum of fifty stars by December 25th. Although the Elves assure you that the grove has plenty of fruit,
you decide to grab any fruit you see along the way, just in case.

Collect stars by solving puzzles. Two puzzles will be made available on each day in the Advent calendar; the second puzzle is unlocked when you complete the first.
Each puzzle grants one star. Good luck!

The jungle must be too overgrown and difficult to navigate in vehicles or access from the air; the Elves' expedition traditionally goes on foot. As your boats approach
land, the Elves begin taking inventory of their supplies. One important consideration is food - in particular, the number of Calories each Elf is carrying (your puzzle input).

The Elves take turns writing down the number of Calories contained by the various meals, snacks, rations, etc. that they've brought with them, one item per line.
Each Elf separates their own inventory from the previous Elf's inventory (if any) by a blank line.

For example, suppose the Elves finish writing their items' Calories and end up with the following list:

1000
2000
3000

4000

5000
6000

7000
8000
9000

10000
This list represents the Calories of the food carried by five Elves:

The first Elf is carrying food with 1000, 2000, and 3000 Calories, a total of 6000 Calories.
The second Elf is carrying one food item with 4000 Calories.
The third Elf is carrying food with 5000 and 6000 Calories, a total of 11000 Calories.
The fourth Elf is carrying food with 7000, 8000, and 9000 Calories, a total of 24000 Calories.
The fifth Elf is carrying one food item with 10000 Calories.
In case the Elves get hungry and need extra snacks, they need to know which Elf to ask: they'd like to know how many Calories are being carried by the Elf carrying the most Calories. In the example above, this is 24000 (carried by the fourth Elf).

Find the Elf carrying the most Calories. How many total Calories is that Elf carrying?

To begin, get your puzzle input. (See data/advent_of_code_day_1_input.txt)
*/

#include<string>
#include<iostream>
#include<fstream>
#include<vector>
#include<numeric>
#include<algorithm>

// Write a function which will read the input file into a vector of vectors of integers.
// Using the example above, we'd end up with
// [ [1000, 2000, 3000], [4000], [5000, 6000], [7000, 8000, 9000], [10000]]
std::vector<std::vector<int>> getElvesCalorieCount(std::string inputFileName)
{
    // Open an ifstream object to get the file
    std::ifstream file (inputFileName);

    // Vector to return
    std::vector<std::vector<int>> vectorToReturn;

    // Declare an empty string which will hold each line as we read in the data
    std::string line;
    std::vector<int> elfCalorieCount;
    if (file.is_open())
    {
        while(getline(file, line))
        {
            if (line == "")
            {
                vectorToReturn.push_back(elfCalorieCount);
                elfCalorieCount.clear();
            }
            else
            {
                int calorieCount = std::atoi(line.c_str());
                elfCalorieCount.push_back(calorieCount);
            }
        }
    }
    file.close();
    return vectorToReturn;
}

std::vector<int> getSortedVectorOfElvesCalories(const std::vector<std::vector<int>>& elvesCalories)
{
    std::vector<int> totalCalorieCount;
    totalCalorieCount.reserve(elvesCalories.size());

    for (int i=0; i<elvesCalories.size(); ++i)
    {
        int calorieCount = std::accumulate(elvesCalories[i].begin(), elvesCalories[i].end(), 0);
        totalCalorieCount.push_back(calorieCount);
    }
    std::sort(totalCalorieCount.rbegin(), totalCalorieCount.rend());
    return totalCalorieCount;
}

int main()
{
    std::string filename = "./data/advent_of_code_day_1_input.txt";
    std::vector<std::vector<int>> elvesCalorieList = getElvesCalorieCount(filename);
    std::vector<int> elvesCalorieCount = getSortedVectorOfElvesCalories(elvesCalorieList);
    // Part 1
    std::cout << "The elf with the max calorie count has " << elvesCalorieCount[0] << " calories." << std::endl;

    // Part 2
    int topThreeCalorieCount = std::accumulate(elvesCalorieCount.begin(), elvesCalorieCount.begin() + 3, 0);
    std::cout << "The three elves with the most calories have a total of " << topThreeCalorieCount << std::endl;
}