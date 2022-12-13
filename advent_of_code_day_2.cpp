/*
--- Day 2: Rock Paper Scissors ---
The Elves begin to set up camp on the beach. To decide whose tent gets to be closest to the snack
storage, a giant Rock Paper Scissors tournament is already in progress.

Rock Paper Scissors is a game between two players. Each game contains many rounds; in each round,
the players each simultaneously choose one of Rock, Paper, or Scissors using a hand shape. Then, a winner for that
round is selected: Rock defeats Scissors, Scissors defeats Paper, and Paper defeats Rock. If both players choose the same shape,
the round instead ends in a draw.

Appreciative of your help yesterday, one Elf gives you an encrypted strategy guide (your puzzle input) that they say will be sure
to help you win. "The first column is what your opponent is going to play: A for Rock, B for Paper, and C for Scissors. The second
column--" Suddenly, the Elf is called away to help with someone's tent.

The second column, you reason, must be what you should play in response: X for Rock, Y for Paper, and Z for Scissors.
Winning every time would be suspicious, so the responses must have been carefully chosen.

The winner of the whole tournament is the player with the highest score.
Your total score is the sum of your scores for each round. The score for a single round is the score for the shape you
selected (1 for Rock, 2 for Paper, and 3 for Scissors) plus the score for the outcome of the round (0 if you lost, 3 if the round was a draw, and 6 if you won).

Since you can't be sure if the Elf is trying to help you or trick you,
you should calculate the score you would get if you were to follow the strategy guide.

For example, suppose you were given the following strategy guide:

A Y
B X
C Z
This strategy guide predicts and recommends the following:

In the first round, your opponent will choose Rock (A), and you should choose Paper (Y). This ends in a win for you with a score of 8 (2 because you chose Paper + 6 because you won).
In the second round, your opponent will choose Paper (B), and you should choose Rock (X). This ends in a loss for you with a score of 1 (1 + 0).
The third round is a draw with both players choosing Scissors, giving you a score of 3 + 3 = 6.
In this example, if you were to follow the strategy guide, you would get a total score of 15 (8 + 1 + 6).

What would your total score be if everything goes exactly according to your strategy guide?
*/

#include<fstream>
#include<iostream>
#include<string>
#include<vector>
#include<map>
#include<sstream>

std::vector<std::string> readStrategyGuide(const std::string& filename)
{
    std::ifstream file (filename);
    std::string line;
    std::vector<std::string> vectorToReturn;
    if(file.is_open())
    {
        while(std::getline(file, line))
            vectorToReturn.push_back(line);
    }
    file.close();
    return vectorToReturn;
}

int getMyScore(const std::vector<std::string>& strategyGuide)
{
    int myScore=0;
    // First column is what your opposition plays. A = Rock, B = Paper, C = Scissors
    // Second column is what you play. X = Rock, Y = Paper, Z = Scissors
    for (int i=0; i<strategyGuide.size(); ++i)
    {
        std::string strategyLine = strategyGuide[i];
        std::string oppositionPlay = strategyLine.substr(0, 1);
        std::string myPlay = strategyLine.substr(2, 1);
        if (myPlay == "X") // I played Rock
        {
            myScore += 1; // Score 1 just for playing Rock
            if (oppositionPlay == "A") // They played Rock; It's a tie
                myScore += 3;
            else if (oppositionPlay == "B") // They played Paper; I lost
                myScore += 0;
            else if (oppositionPlay == "C") // They played Scissors I won
                myScore += 6;

        }
        else if (myPlay == "Y") // I played Paper
        {
            myScore += 2; // Score 2 just for playing Paper
            if (oppositionPlay == "A") // They played Rock; I won
                myScore += 6;
            else if(oppositionPlay == "B") // They played Paper; draw
                myScore += 3;
            else if (oppositionPlay == "C") // They played Scissors; I lost
                myScore += 0;
        }
        else if (myPlay == "Z") // I played Scissors
        {
            myScore += 3; // Score 3 just for playing Scissors
            if (oppositionPlay == "A") // They played Rock; I lost
                myScore += 0;
            else if(oppositionPlay == "B") // They played Paper; I win
                myScore += 6;
            else if (oppositionPlay == "C") // They played Scissors; it's a draw
                myScore += 3;
        }
    }
    return myScore;
}

int getSecretScore(const std::vector<std::string> strategyGuide)
{
    // Now X means lose, Y means draw, and Z means win
    int myScore = 0;
    for (int i=0; i<strategyGuide.size(); ++i)
    {
        std::string strategyLine = strategyGuide[i];
        std::string oppositionPlay = strategyLine.substr(0, 1);
        std::string myPlay = strategyLine.substr(2, 1);
        if (myPlay == "X") // I have to lose.
        {
            if (oppositionPlay == "A") // They chose Rock, so I need chose scissors and score 3 points
                myScore += 3;
            else if (oppositionPlay == "B") // THey chose Paper, so I need to chose rock and socre 1 point
                myScore += 1;
            else if (oppositionPlay == "C") // They chose Scissors, so I need to choose paper and score 2 points
                myScore += 2;
        }
        else if (myPlay == "Y") // I have to draw
        {
            myScore += 3;
            if (oppositionPlay == "A") // They chose Rock, so I need to choose Rock and score 1 point
                myScore += 1;
            else if (oppositionPlay == "B") // THey chose Paper, so I need to chose paper and score 2 points
                myScore += 2;
            else if (oppositionPlay == "C") // They chose Scissors, so I need to choose scissors and score 3 points
                myScore += 3;
        }
        else if (myPlay == "Z") // I have to win
        {
            myScore += 6;
            if (oppositionPlay == "A") // they chose rock, so i need to choose paper and score 2 points
                myScore += 2;
            else if (oppositionPlay == "B") // they chose paper so i need to choose scissors and score 3 points
                myScore += 3;
            else if (oppositionPlay == "C") // they chose scissors so i need to choose rock and score 1 point
                myScore += 1;
        }
    }
    return myScore;
}

int main()
{
    std::string filename = "./data/advent_of_code_day_2_input.txt";
    std::vector<std::string> strategyGuide = readStrategyGuide(filename);

    // Part 1
    std::cout << "My score is " << getMyScore(strategyGuide) << std::endl;

    // Part 2
    std::cout << "My score with the secret strategy guide is " << getSecretScore(strategyGuide) << std::endl;


}