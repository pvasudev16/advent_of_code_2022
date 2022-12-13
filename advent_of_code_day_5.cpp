/*
--- Day 5: Supply Stacks ---
The expedition can depart as soon as the final supplies have been unloaded from the ships.
Supplies are stored in stacks of marked crates, but because the needed supplies are buried under many other crates, the crates need to be rearranged.

The ship has a giant cargo crane capable of moving crates between stacks. To ensure none of the crates get crushed or fall over, the crane operator
will rearrange them in a series of carefully-planned steps. After the crates are rearranged, the desired crates will be at the top of each stack.

The Elves don't want to interrupt the crane operator during this delicate procedure, but they forgot to ask her which crate will end up where, and
they want to be ready to unload them as soon as possible so they can embark.

They do, however, have a drawing of the starting stacks of crates and the rearrangement procedure (your puzzle input). For example:

    [D]    
[N] [C]    
[Z] [M] [P]
 1   2   3 

move 1 from 2 to 1
move 3 from 1 to 3
move 2 from 2 to 1
move 1 from 1 to 2
In this example, there are three stacks of crates. Stack 1 contains two crates: crate Z is on the bottom, and crate N is on top.
Stack 2 contains three crates; from bottom to top, they are crates M, C, and D. Finally, stack 3 contains a single crate, P.

Then, the rearrangement procedure is given. In each step of the procedure, a quantity of crates is moved from one stack to a
different stack. In the first step of the above rearrangement procedure, one crate is moved from stack 2 to stack 1, resulting in this configuration:

[D]        
[N] [C]    
[Z] [M] [P]
 1   2   3 
In the second step, three crates are moved from stack 1 to stack 3. Crates are moved one at a time, so the first crate to be moved (D) ends up below
the second and third crates:

        [Z]
        [N]
    [C] [D]
    [M] [P]
 1   2   3
Then, both crates are moved from stack 2 to stack 1. Again, because crates are moved one at a time, crate C ends up below crate M:

        [Z]
        [N]
[M]     [D]
[C]     [P]
 1   2   3
Finally, one crate is moved from stack 1 to stack 2:

        [Z]
        [N]
        [D]
[C] [M] [P]
 1   2   3
The Elves just need to know which crate will end up on top of each stack; in this example, the top crates are C in stack 1, M in stack 2, and Z in stack 3,
so you should combine these together and give the Elves the message CMZ.

After the rearrangement procedure completes, what crate ends up on top of each stack?
*/

#include<fstream>
#include<iostream>
#include<string>
#include<vector>
#include<assert.h>
#include<stack>

class ConfigurationAndRearrangmentProcedures
{
public:
    ConfigurationAndRearrangmentProcedures(const std::string & filename)
    {
        std::ifstream file(filename);
        std::string line;

        // First element of vector will be initial configuration
        // Second element will be the rearrangment instructions
        std::vector<std::vector<std::string>> configurationAndRearrangement;

        // Vector to hold either the entire configuration or entire rearrangement instructions
        std::vector<std::string> input;

        if (file.is_open())
        {
            while (std::getline(file, line))
            {
                if(line.substr(0, 1) == " ") // The line that goes "1 ... 9" demarcates the end of the initial configuration
                {
                    configurationAndRearrangement.push_back(input);
                    input.clear();
                    continue;
                }
                else if (line == "")
                {
                    continue; // Skip the blank line
                }
                input.push_back(line);
            }
            configurationAndRearrangement.push_back(input);
            m_InitialConfiguration = configurationAndRearrangement[0];
            m_RearrangementProcedures = configurationAndRearrangement[1];
        }
        file.close();
    }
    std::vector<std::string> getInitialConfiguration(){ return m_InitialConfiguration;}
    std::vector<std::string> getRearrangementInstructions(){ return m_RearrangementProcedures;}

private:
    std::vector<std::string> m_InitialConfiguration;
    std::vector<std::string> m_RearrangementProcedures;
};

std::vector<std::stack<char>> getStacks(const std::vector<std::string> & initialConfig)
{
    // Stacks 1,  2,  3,   4,   5,   6,   7,   8,   9 are defined by characters
    //        2,  6,  10,  14,  18,  22,  26,  30,  34 in initialConfig (indexed from 1)
    //        1,  5,  9,   13,  17,  21,  25,  29,  33                  (indexed from 0)

    std::vector<std::stack<char>> vectorToReturn;

    // Iterate in reverse through initialConfig; the last line represents the bottom of the
    // stacks and have to be added first.
    for (std::vector<std::string>::const_reverse_iterator crt = initialConfig.crbegin(); crt != initialConfig.crend(); ++crt)
    {
        for (int i = 1; i < 34; i += 4)
        {
            // Get stack element character
            char stackElement = (*crt).substr(i, 1)[0];
            if (stackElement == ' ')
                continue;

            // If we are at the first line, then we need to initialize the vector with nine stacks
            if (crt == initialConfig.crbegin()) 
            {
                // Put the character onto a stack
                std::stack<char> stack;
                stack.push(stackElement);

                // Put this stack onto the vector
                vectorToReturn.push_back(stack);

                // We added only one item so pop it
                stack.pop();
            }
            else
            {
                // Get the index of the stack we're looking at
                int j = (i - 1) / 4;

                // Add the character to that stack
                vectorToReturn[j].push(stackElement);
            }
        }
    }
    return vectorToReturn;
}

std::vector<std::vector<int>> parseInstructions(const std::vector<std::string> & instructions)
{
    // Parse the instructions. Each line of the instructions should be [number_of_crates, start, end]
    // whih means move number_of_crates from stack start to stack end
    // Each line is        "move 3 from 1 to 2"
    // Notice the spaces:       ^ ^    ^ ^  ^
    //                          4 6   11 13 16

    std::vector<std::vector<int>> vectorToReturn;

    // Make a vector of ints to store each line of instructions
    std::vector<int> instructionsLine;
    
    std::vector<int> spacePositions;

    for( const std::string & line : instructions)
    {
        size_t previousSpacePosition = -1;
        // Get the position of the five spaces
        for (int i = 0; i < 5; ++i)
        {
            size_t position = line.find(" ", previousSpacePosition + 1);
            spacePositions.push_back(position);
            previousSpacePosition = position;
        }
        // Given the space positions, now get the numbers. Subtract one since inputs are 1-indexed
        int numberOfCrates = std::atoi(line.substr(spacePositions[0] + 1, spacePositions[1] - spacePositions[0] - 1).c_str());
        int startingStack = std::atoi(line.substr(spacePositions[2] + 1, spacePositions[3] - spacePositions[2] - 1).c_str()) - 1;
        int endingStack = std::atoi(line.substr(spacePositions[4] + 1, line.size() - spacePositions[4] - 1).c_str()) - 1;

        // Push these as one line of instructions
        instructionsLine.push_back(numberOfCrates);
        instructionsLine.push_back(startingStack);
        instructionsLine.push_back(endingStack);

        // Add the line to the vector of instructions
        vectorToReturn.push_back(instructionsLine);

        // Reset instruction line and space positions
        instructionsLine.clear();
        spacePositions.clear();
    }
    return vectorToReturn;
}

void modifyStacks(const std::vector<std::vector<int>> & instructions, std::vector<std::stack<char>> & initialConfig)
{
    // Instructions are a series of lines [number_of_crates, start, end]
    int numberOfCrates;
    int start;
    int end;
    char crate;
    for(auto & line : instructions)
    {
        numberOfCrates = line[0];
        start = line[1];
        end = line[2];

        int numberOfCratesInStartingStack = initialConfig[start].size();
        int loopBound =  numberOfCratesInStartingStack < numberOfCrates ? numberOfCratesInStartingStack : numberOfCrates;
        for(int i = 0; i < loopBound; ++i)
        {
            crate = initialConfig[start].top();
            initialConfig[start].pop();
            initialConfig[end].push(crate);
        }
    }
}

// Part 2
void modifyStacksWithNewCrane(const std::vector<std::vector<int>> & instructions, std::vector<std::stack<char>> & initialConfig)
{
    // Instructions are a series of lines [number_of_crates, start, end]
    int numberOfCrates;
    int start;
    int end;
    char crate;
    char crateToAdd; // From crane stack
    for(auto & line : instructions)
    {
        numberOfCrates = line[0];
        start = line[1];
        end = line[2];

        int numberOfCratesInStartingStack = initialConfig[start].size();
        int loopBound =  numberOfCratesInStartingStack < numberOfCrates ? numberOfCratesInStartingStack : numberOfCrates;

        // Now push the crates to be moved all onto one stack and then unload it
        std::stack<char> craneStack;
        for(int i = 0; i < loopBound; ++i)
        {
            crate = initialConfig[start].top();
            craneStack.push(crate);
            initialConfig[start].pop();
        }

        // Unload the crane stack
        for(int i = 0; i < loopBound; ++i)
        {
            crateToAdd = craneStack.top();
            craneStack.pop();
            initialConfig[end].push(crateToAdd);
        }
    }
}

int main()
{
    std::string inputFile = "./data/advent_of_code_day_5_input.txt";
    ConfigurationAndRearrangmentProcedures initialConfigAndInstructions = ConfigurationAndRearrangmentProcedures(inputFile);
    std::vector<std::string> initialConfig = initialConfigAndInstructions.getInitialConfiguration();
    std::vector<std::string> instructions = initialConfigAndInstructions.getRearrangementInstructions();
    std::vector<std::stack<char>> stacks = getStacks(initialConfig);
    std::vector<std::vector<int>> parsedInstructions = parseInstructions(instructions);
    // for (int i = 0; i < parsedInstructions.size(); ++i)
    // {
    //     for (int j = 0; j < parsedInstructions[i].size(); ++j)
    //         std::cout << parsedInstructions[i][j] << " ";
    //     std::cout << std::endl;
    // }
    std::cout << "Initial configuration..." << std::endl;
    assert(stacks.size() == 9);
    for (int i = 0 ; i < stacks.size(); ++i)
    {
        std::cout << "Stack " << i << " ";
        while (!stacks[i].empty())
        {
            char character = stacks[i].top();
            std::cout << character;
            stacks[i].pop();
        }
        std::cout << std::endl;
    }

    stacks = getStacks(initialConfig);
    modifyStacksWithNewCrane(parsedInstructions, stacks);
    std::cout << std::endl << "Final configuration..." << std::endl;
    for (int i = 0 ; i < stacks.size(); ++i)
    {
        while (!stacks[i].empty())
        {
            char character = stacks[i].top();
            std::cout << character;
            stacks[i].pop();
        }
        std::cout <<  std::endl;
    }

    return 0;
}