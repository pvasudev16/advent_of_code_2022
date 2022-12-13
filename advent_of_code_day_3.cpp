/*
--- Day 3: Rucksack Reorganization ---
One Elf has the important job of loading all of the rucksacks with supplies for the jungle journey. Unfortunately, that
Elf didn't quite follow the packing instructions, and so a few items now need to be rearranged.

Each rucksack has two large compartments. All items of a given type are meant to go into exactly one of the two compartments.
The Elf that did the packing failed to follow this rule for exactly one item type per rucksack.

The Elves have made a list of all of the items currently in each rucksack (your puzzle input), but they need your help finding the errors.
Every item type is identified by a single lowercase or uppercase letter (that is, a and A refer to different types of items).

The list of items for each rucksack is given as characters all on a single line. A given rucksack always has the same number of items in
each of its two compartments, so the first half of the characters represent items in the first compartment, while the second half of the
characters represent items in the second compartment.

For example, suppose you have the following list of contents from six rucksacks:

vJrwpWtwJgWrhcsFMMfFFhFp
jqHRNqRjqzjGDLGLrsFMfFZSrLrFZsSL
PmmdzqPrVvPwwTWBwg
wMqvLMZHhHMvwLHjbvcjnnSBnvTQFn
ttgJtRGJQctTZtZT
CrZsJsPPZsGzwwsLwLmpwMDw
The first rucksack contains the items vJrwpWtwJgWrhcsFMMfFFhFp, which means its first compartment contains the items vJrwpWtwJgWr, while the second compartment contains the items hcsFMMfFFhFp. The only item type that appears in both compartments is lowercase p.
The second rucksack's compartments contain jqHRNqRjqzjGDLGL and rsFMfFZSrLrFZsSL. The only item type that appears in both compartments is uppercase L.
The third rucksack's compartments contain PmmdzqPrV and vPwwTWBwg; the only common item type is uppercase P.
The fourth rucksack's compartments only share item type v.
The fifth rucksack's compartments only share item type t.
The sixth rucksack's compartments only share item type s.
To help prioritize item rearrangement, every item type can be converted to a priority:

Lowercase item types a through z have priorities 1 through 26.
Uppercase item types A through Z have priorities 27 through 52.
In the above example, the priority of the item type that appears in both compartments of each rucksack is 16 (p), 38 (L), 42 (P), 22 (v), 20 (t), and 19 (s); the sum of these is 157.

Find the item type that appears in both compartments of each rucksack. What is the sum of the priorities of those item types?

--- Part Two ---
As you finish identifying the misplaced items, the Elves come to you with another issue.

For safety, the Elves are divided into groups of three.
Every Elf carries a badge that identifies their group.
For efficiency, within each group of three Elves, the badge is the only item type carried by all three Elves.
That is, if a group's badge is item type B, then all three Elves will have item type B somewhere in their rucksack, and at most two of the Elves will be carrying any other item type.

The problem is that someone forgot to put this year's updated authenticity sticker on the badges. All of the badges need to be pulled out of the rucksacks so the new authenticity stickers can be attached.

Additionally, nobody wrote down which item type corresponds to each group's badges. The only way to tell which item type is the right one is by finding the one item type that is common between all three Elves in each group.

Every set of three lines in your list corresponds to a single group, but each group can have a different badge item type. So, in the above example, the first group's rucksacks are the first three lines:

vJrwpWtwJgWrhcsFMMfFFhFp
jqHRNqRjqzjGDLGLrsFMfFZSrLrFZsSL
PmmdzqPrVvPwwTWBwg
And the second group's rucksacks are the next three lines:

wMqvLMZHhHMvwLHjbvcjnnSBnvTQFn
ttgJtRGJQctTZtZT
CrZsJsPPZsGzwwsLwLmpwMDw
In the first group, the only item type that appears in all three rucksacks is lowercase r; this must be their badges. In the second group, their badge item type must be Z.

Priorities for these items must still be found to organize the sticker attachment efforts: here, they are 18 (r) for the first group and 52 (Z) for the second group. The sum of these is 70.

Find the item type that corresponds to the badges of each three-Elf group. What is the sum of the priorities of those item types?

Answer: 
 
*/

#include<map>
#include<iostream>
#include<string>
#include<vector>
#include<fstream>
#include<assert.h>

std::map<char, int> getPriorityMap()
{
    std::string alphabet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    std::map<char, int> mapToReturn;
    int score = 1; // a scores 1
    for( auto & s : alphabet)
    {
        mapToReturn.insert(std::pair<char, int>(s, score));
        ++score;
    }
    return mapToReturn;
}

std::vector<std::string> getRucksackContents(const std::string& filename)
{
    std::ifstream file (filename);
    std::string line;
    std::vector<std::string> vectorToReturn;
    if(file.is_open())
    {
        while(std::getline(file, line))
        {
            vectorToReturn.push_back(line);
        }
    }
    file.close();
    return vectorToReturn;
}

void findCommonElement(const std::string & s1, const std::string & s2, char & commonCharacter)
{
    std::map<char, int> s1CharacterMap;
    for (auto & c : s1)
    {
        std::map<char, int>::const_iterator cit = s1CharacterMap.find(c);
        if (cit == s1CharacterMap.cend()) // It's not in the map
            s1CharacterMap.insert(std::pair<char, int>(c, 1));
        else
            s1CharacterMap[c] += 1;
    }

    // Cycle through s2 and find the common character
    for (auto & c : s2)
    {
        std::map<char, int>::const_iterator cit = s1CharacterMap.find(c);
        if (cit != s1CharacterMap.cend())
            commonCharacter = c;
    }
}

std::vector<char> getCommonItems(std::vector<std::string> rucksackContents)
{
    std::vector<char> vectorToReturn;
    char commonCharacter;
    for (int i = 0; i < rucksackContents.size(); ++i)
    {
        int s = rucksackContents[i].size();
        std::string leftCompartment = rucksackContents[i].substr(0, s/2);
        std::string rightCompartment = rucksackContents[i].substr(s/2, s/2);
        findCommonElement(leftCompartment, rightCompartment, commonCharacter);
        vectorToReturn.push_back(commonCharacter);
    }
    
    return vectorToReturn;
}

int scoreCommonElements(std::vector<char> commonElements)
{
    std::map<char, int> priorityMap = getPriorityMap();
    int score = 0;
    for (int i = 0; i < commonElements.size(); ++i)
    {
        score += priorityMap[commonElements[i]];
    }
    return score;
}

// Part Two
std::vector<std::vector<std::string>> getGroupsOfThree(std::vector<std::string> rucksackContents)
{
    std::vector<std::vector<std::string>> vectorToReturn;
    
    // Vector with three lines of rucksacks
    std::vector<std::string> threeRucksacks;
    for (int i = 0; i < rucksackContents.size(); i += 3)
    {
        threeRucksacks.clear();
        // Graph the rucksack contents, three lines at a time
        for (int j = 0; j < 3; ++j)
            threeRucksacks.push_back(rucksackContents[i + j]);
        vectorToReturn.push_back(threeRucksacks);
    }
    return vectorToReturn;
}

void findCommonElement(const std::string & s1, const std::string & s2, const std::string & s3, char & commonElement)
{
    std::map<char, int> s1CharacterMap;
    // Populate s1 character map. I'm counting characters in s1 here. Why not?
    for( auto & c : s1)
    {
        std::map<char, int>::iterator it = s1CharacterMap.find(c);
        if (it == s1CharacterMap.end())
            s1CharacterMap.insert(std::pair<char, int>(c, 1));
        else
            it->second += 1;
    }

    std::map<char, int> commonToS1AndS2;
    for (auto & c : s2)
    {
        std::map<char, int>::iterator it = s1CharacterMap.find(c);
        if (it != s1CharacterMap.end()) // character is common to s1 and s2
            commonToS1AndS2.insert(std::pair<char, int>(c, 0));
    }

    std::map<char, int> commonToS1AndS2AndS3;
    for (auto & c : s3)
    {
        std::map<char, int>::iterator it = commonToS1AndS2.find(c);
        if (it != commonToS1AndS2.end())
            commonToS1AndS2AndS3.insert(std::pair<char, int>(c, 0));
    }

    assert(commonToS1AndS2AndS3.size() == 1);
    commonElement = commonToS1AndS2AndS3.begin()->first;
}

int getSumOfTripletPriorities(std::vector<std::vector<std::string>> groupsOfThree)
{
    int score = 0;
    char c;
    std::map<char, int> priorityMap = getPriorityMap();
    for (int i = 0; i< groupsOfThree.size(); ++i)
    {
        assert(groupsOfThree[i].size() == 3);
        findCommonElement(groupsOfThree[i][0], groupsOfThree[i][1], groupsOfThree[i][2], c);
        score += priorityMap[c];
    }
    return score;
}


int main()
{
    std::map<char, int> priorityMap = getPriorityMap();
    std::vector<std::string> rucksackContents = getRucksackContents("./data/advent_of_code_day_3_input.txt");
    std::vector<char> commonItems = getCommonItems(rucksackContents);
    int score = scoreCommonElements(commonItems);
    std::cout << "The sum of the common priorities is " << score << std::endl;

    // Part 2
    std::vector<std::vector<std::string>> groupsOfThree = getGroupsOfThree(rucksackContents);
    int scorePartTwo = getSumOfTripletPriorities(groupsOfThree);
    std::cout << "The sum of the priorities for the badges is " << scorePartTwo << std::endl;
    return 0;
}