/*
--- Day 8: Treetop Tree House ---
The expedition comes across a peculiar patch of tall trees all planted carefully in a grid. The Elves explain that a previous expedition planted these trees as
a reforestation effort. Now, they're curious if this would be a good location for a tree house.

First, determine whether there is enough tree cover here to keep a tree house hidden. To do this, you need to count the number of trees that are visible
from outside the grid when looking directly along a row or column.

The Elves have already launched a quadcopter to generate a map with the height of each tree (your puzzle input). For example:

30373
25512
65332
33549
35390
Each tree is represented as a single digit whose value is its height, where 0 is the shortest and 9 is the tallest.

A tree is visible if all of the other trees between it and an edge of the grid are shorter than it. Only consider trees in the same row or column;
that is, only look up, down, left, or right from any given tree.

All of the trees around the edge of the grid are visible - since they are already on the edge, there are no trees to block the view.
In this example, that only leaves the interior nine trees to consider:

The top-left 5 is visible from the left and top. (It isn't visible from the right or bottom since other trees of height 5 are in the way.)
The top-middle 5 is visible from the top and right.
The top-right 1 is not visible from any direction; for it to be visible, there would need to only be trees of height 0 between it and an edge.
The left-middle 5 is visible, but only from the right.
The center 3 is not visible from any direction; for it to be visible, there would need to be only trees of at most height 2 between it and an edge.
The right-middle 3 is visible from the right.
In the bottom row, the middle 5 is visible, but the 3 and 4 are not.
With 16 trees visible on the edge and another 5 visible in the interior, a total of 21 trees are visible in this arrangement.

Consider your map; how many trees are visible from outside the grid?

--- Part Two ---
Content with the amount of tree cover available, the Elves just need to know the best spot to build their tree house: they would like to be able to see a lot of trees.

To measure the viewing distance from a given tree, look up, down, left, and right from that tree; stop if you reach an edge or at the first tree that is the same height or taller than the tree under consideration. (If a tree is right on the edge, at least one of its viewing distances will be zero.)

The Elves don't care about distant trees taller than those found by the rules above; the proposed tree house has large eaves to keep it dry, so they wouldn't be able to see higher than the tree house anyway.

In the example above, consider the middle 5 in the second row:

30373
25512
65332
33549
35390
Looking up, its view is not blocked; it can see 1 tree (of height 3).
Looking left, its view is blocked immediately; it can see only 1 tree (of height 5, right next to it).
Looking right, its view is not blocked; it can see 2 trees.
Looking down, its view is blocked eventually; it can see 2 trees (one of height 3, then the tree of height 5 that blocks its view).
A tree's scenic score is found by multiplying together its viewing distance in each of the four directions. For this tree, this is 4 (found by multiplying 1 * 1 * 2 * 2).

However, you can do even better: consider the tree of height 5 in the middle of the fourth row:

30373
25512
65332
33549
35390
Looking up, its view is blocked at 2 trees (by another tree with a height of 5).
Looking left, its view is not blocked; it can see 2 trees.
Looking down, its view is also not blocked; it can see 1 tree.
Looking right, its view is blocked at 2 trees (by a massive tree of height 9).
This tree's scenic score is 8 (2 * 2 * 1 * 2); this is the ideal spot for the tree house.

Consider each tree on your map. What is the highest scenic score possible for any tree?
*/

#include<fstream>
#include<vector>
#include<string>
#include<assert.h>
#include<iostream>
#include<algorithm>
#include<numeric>

std::vector<std::vector<int>> readInTreeHeightMatrix(const std::string & filename)
{
    std::ifstream file(filename);
    std::string line;
    std::vector<std::vector<int>> treeSizeMatrix;
    std::vector<int> treeSizeRow;
    int treeHeight;
    if (file.is_open())
    {
        while(std::getline(file, line))
        {
            // Convert each char in line into an int and push it to the matrix of tree sizes
            for (int i = 0; i < line.size(); ++i)
            {
                treeHeight = std::atoi(line.substr(i, 1).c_str());
                treeSizeRow.push_back(treeHeight);
            }
            treeSizeMatrix.push_back(treeSizeRow);
            treeSizeRow.clear();
        }
    }
    file.close();
    return treeSizeMatrix;
}

bool isVisible(int x, int y, const std::vector<std::vector<int>> & treeSizeMatrix)
{
    std::vector<int> treeHeightsNorthOfX;
    for (int i = 0; i < x; ++i)
        treeHeightsNorthOfX.push_back(treeSizeMatrix[i][y]);
    
    std::vector<int> treeHeightsSouthOfX;
    for (int i = x + 1; i < treeSizeMatrix.size(); ++i)
        treeHeightsSouthOfX.push_back(treeSizeMatrix[i][y]);
    
    // Trees are visible if it's visible from W or N or S or E
    if (treeSizeMatrix[x][y] > *(std::max_element(treeHeightsNorthOfX.begin(), treeHeightsNorthOfX.end())) // taller than trees North of it
        || treeSizeMatrix[x][y] > *(std::max_element(treeSizeMatrix[x].cbegin(), treeSizeMatrix[x].cbegin() + y)) // taller than trees West of it
        || treeSizeMatrix[x][y] > *(std::max_element(treeHeightsSouthOfX.begin(), treeHeightsSouthOfX.end())) // taller than trees South of it
        || treeSizeMatrix[x][y] > *(std::max_element(treeSizeMatrix[x].cbegin() + y + 1, treeSizeMatrix[x].cend()))) // taller than trees East of it
    {

        return true;
    }
    
    return false;
}

std::vector<std::vector<bool>> isVisibleMatrix(std::vector<std::vector<int>> treeSizeMatrix)
{
    // Get the size of the treeSizeMatrix. Let the number in the North-South direction be nX and in the East-West be nY
    // In this case, we know that each row has the same number of trees in it, so we can just get the size of the first row
    size_t nX = treeSizeMatrix.size();
    size_t nY = treeSizeMatrix[0].size(); 

    // Make the visibility matrix the same size as the treeSizeMatrix and populate it, by default, with false
    std::vector<std::vector<bool>> treeVisibilityMatrix;
    treeVisibilityMatrix.reserve(nX);
    std::vector<bool> treeVisibilityRow;
    for (int i = 0; i < nX; ++i)
    {
        treeVisibilityRow = std::vector<bool>(nY, false);
        treeVisibilityMatrix.push_back(treeVisibilityRow);
        treeVisibilityRow.clear();
    }
    
    // Set the North and South edges to true
    treeVisibilityMatrix[0] = std::vector<bool>(nY, true);
    treeVisibilityMatrix[nX - 1] = std::vector<bool>(nY, true);

    // Now the West and East edges
    //        (1-indexed)   (0-indexed)
    // Cycle over the [1]   [0]  rows
    //                 2     1      <--
    //                 3     3        |
    //                 ...   ...      |-- This range
    //                 97    96       |
    //                 98    97     <--
    //                [99]   [98] == nX
    // In each of these rows, set the 0-th and last element to zero
    for (int i = 1; i < nX - 1; ++i)
    {
        treeVisibilityMatrix[i][0] = true;
        treeVisibilityMatrix[i][nY - 1] = true;
    }

    // Now we have to do the interior trees.
    for (int i = 1; i < nX - 1; ++ i) // Cycle over all interior rows
    {
        for (int j = 1; j < nY - 1; ++j) // Cycle over all interior columns
        {
            treeVisibilityMatrix[i][j] = isVisible(i, j, treeSizeMatrix);
        }
    }
    return treeVisibilityMatrix;
}

 // Part 2

 int getScenicScore(int x, int y, int nX, int nY, const std::vector<std::vector<int>> & treeSizeMatrix)
{
    int treeSize = treeSizeMatrix[x][y];
    // We know we're not at an edge, so let's start at x and y and cycle outwards.
    // Get North viewing distance
    int northViewingDistance = 0;
    for(int i = x; i > 0; --i)
    {
        ++northViewingDistance;
        if (treeSizeMatrix[i - 1][y] >= treeSize)
            break;
    }

    int southViewingDistance = 0;
    for (int i = x; i < nX - 1; ++i)
    {
        ++southViewingDistance;
        if (treeSizeMatrix[i + 1][y] >= treeSize)
            break;
    }

    int westViewingDistance = 0;
    for (int i = y; i > 0; --i)
    {
        ++westViewingDistance;
        if (treeSizeMatrix[x][i - 1] >= treeSize)
            break;
    }

    int eastViewingDistance = 0;
    for (int i = y; i < nY - 1; ++i)
    {
        ++eastViewingDistance;
        if (treeSizeMatrix[x][i + 1] >= treeSize)
            break;
    }
    return northViewingDistance * eastViewingDistance * westViewingDistance * southViewingDistance;
}

 std::vector<std::vector<int>> scenicScoreMatrix(std::vector<std::vector<int>> treeSizeMatrix)
{
    // Repeat some code from above
    size_t nX = treeSizeMatrix.size();
    size_t nY = treeSizeMatrix[0].size(); 

    std::vector<std::vector<int>> scenicMatrix;
    scenicMatrix.reserve(nX);
    std::vector<int> scenicRow;
    for (int i = 0; i < nX; ++i)
    {
        scenicRow = std::vector<int>(nY, 0);
        scenicMatrix.push_back(scenicRow);
        scenicRow.clear();
    }
    
    // Set the North and South edges to 0
    scenicMatrix[0] = std::vector<int>(nY, 0);
    scenicMatrix[nX - 1] = std::vector<int>(nY, 0);
    // East and West edges
    for (int i = 1; i < nX - 1; ++i)
    {
        scenicMatrix[i][0] = 0;
        scenicMatrix[i][nY - 1] = 0;
    }

    // Now we have to do the interior trees.
    for (int i = 1; i < nX - 1; ++ i) // Cycle over all interior rows
    {
        for (int j = 1; j < nY - 1; ++j) // Cycle over all interior columns
        {
            scenicMatrix[i][j] = getScenicScore(i, j, nX, nY, treeSizeMatrix);
        }
    }
    return scenicMatrix;
}

int main()
{
    std::vector<std::vector<int>> treeSizeMatrix = readInTreeHeightMatrix("./data/advent_of_code_day_8_input.txt");
    assert(treeSizeMatrix.size() == 99);
    assert(treeSizeMatrix[0].size() == 99);
    std::vector<std::vector<bool>> treeVisibilityMatrix = isVisibleMatrix(treeSizeMatrix);

    int numberOfTreesVisible = 0;
    for (int i = 0; i < treeVisibilityMatrix.size(); ++i)
    {
        for (int j = 0; j < treeVisibilityMatrix[i].size(); ++j)
        {
            numberOfTreesVisible += int(treeVisibilityMatrix[i][j]);
        }
    }

    std::cout << "The number of trees visible from outside the grid is " << numberOfTreesVisible << std::endl;

    // Part 2
    std::vector<std::vector<int>> scenicMatrix = scenicScoreMatrix(treeSizeMatrix);
    int maxScenicScore = 0;
    for (int i = 0; i < scenicMatrix.size(); ++i)
    {
        for (int j = 0; j < scenicMatrix[i].size(); ++j)
        {
            if (scenicMatrix[i][j] > maxScenicScore)
                maxScenicScore = scenicMatrix[i][j];
        }
    }
    std::cout << "The spot with the maximum scenic score has " << maxScenicScore << "." << std::endl;
    return 0;
}