/*
--- Day 4: Camp Cleanup ---
Space needs to be cleared before the last supplies can be unloaded from the ships, and so several Elves have been assigned the job of cleaning up sections of the camp.
Every section has a unique ID number, and each Elf is assigned a range of section IDs.

However, as some of the Elves compare their section assignments with each other, they've noticed that many of the assignments overlap. To try to quickly find overlaps and
reduce duplicated effort, the Elves pair up and make a big list of the section assignments for each pair (your puzzle input).

For example, consider the following list of section assignment pairs:

2-4,6-8
2-3,4-5
5-7,7-9
2-8,3-7
6-6,4-6
2-6,4-8
For the first few pairs, this list means:

Within the first pair of Elves, the first Elf was assigned sections 2-4 (sections 2, 3, and 4), while the second Elf was assigned sections 6-8 (sections 6, 7, 8).
The Elves in the second pair were each assigned two sections.
The Elves in the third pair were each assigned three sections: one got sections 5, 6, and 7, while the other also got 7, plus 8 and 9.
This example list uses single-digit section IDs to make it easier to draw; your actual list might contain larger numbers. Visually, these pairs of section assignments look like this:

.234.....  2-4
.....678.  6-8

.23......  2-3
...45....  4-5

....567..  5-7
......789  7-9

.2345678.  2-8
..34567..  3-7

.....6...  6-6
...456...  4-6

.23456...  2-6
...45678.  4-8
Some of the pairs have noticed that one of their assignments fully contains the other. For example, 2-8 fully contains 3-7, and 6-6 is fully contained by 4-6. In pairs where one
assignment fully contains the other, one Elf in the pair would be exclusively cleaning sections their partner will already be cleaning, so these seem like the most in need of
reconsideration. In this example, there are 2 such pairs.

In how many assignment pairs does one range fully contain the other?

--- Part Two ---
It seems like there is still quite a bit of duplicate work planned. Instead, the Elves would like to know the number of pairs that overlap at all.

In the above example, the first two pairs (2-4,6-8 and 2-3,4-5) don't overlap, while the remaining four pairs (5-7,7-9, 2-8,3-7, 6-6,4-6, and 2-6,4-8) do overlap:

5-7,7-9 overlaps in a single section, 7.
2-8,3-7 overlaps all of the sections 3 through 7.
6-6,4-6 overlaps in a single section, 6.
2-6,4-8 overlaps in sections 4, 5, and 6.
So, in this example, the number of overlapping assignment pairs is 4.

In how many assignment pairs do the ranges overlap?
*/

#include<fstream>
#include<iostream>
#include<vector>
#include<string>
#include<assert.h>

const std::vector<std::string>& readSectionAssigments(const std::string& filename)
{
    static std::vector<std::string> vectorToReturn;
    std::ifstream file(filename);
    std::string line;
    if (file.is_open())
    {
        while(std::getline(file, line))
        {
            vectorToReturn.push_back(line);
        }
    }
    file.close();
    return vectorToReturn;
}

const std::vector<std::vector<std::string>>& getSectionAssignmentForPairsOfElves(const std::vector<std::string>& sectionAssignments)
{
    static std::vector<std::vector<std::string>> vectorToReturn;
    for (std::vector<std::string>::const_iterator cit = sectionAssignments.cbegin(); cit != sectionAssignments.cend(); ++cit)
    {
        // *cit is a string like "26-92,13-91"
        size_t commaPosition = (*cit).find(",");

        // Make sure we didn't get ot the end of the string
        assert(commaPosition != std::string::npos); 

        vectorToReturn.push_back(std::vector<std::string>{(*cit).substr(0                , commaPosition),
                                                          (*cit).substr(commaPosition + 1, (*cit).size())});
        // vectorToReturn.push_back(std::pair<std::string, std::string>((*cit).substr(0                , commaPosition),
        //                                                              (*cit).substr(commaPosition + 1, (*cit).size())));
    }
    return vectorToReturn;
}

bool isTotalOverlap(std::vector<int> sectionAssignments)
{
    if
    (
        // elf 1's assignment is completely contained within elf 2's.
        // This means elf_1 start >= elf_2 start AND elf_1 end <= elf_2 end
        ((sectionAssignments[0] >= sectionAssignments[2]) && (sectionAssignments[1] <= sectionAssignments[3]))

        // elf 2's assignment is completely contained within elf 2's
        // This means elf_2 start >= elf_1 start AND elf_2 end <= elf_1 end
        || ((sectionAssignments[2] >= sectionAssignments[0]) && (sectionAssignments[3] <= sectionAssignments[1]))
    )
        return true;
    else
        return false;
}

bool isPartialOverlap(std::vector<int> sectionAssignments)
{
    /*
    Consider the pairs 2-6, 4-8
    1---2---3---4---5---6---7---8---9
    ----[-------(-------]-------)----

    Let square brackets be pair 1, curved brackets be pair 2
    Partial overlap means: 
           (opening 1 <= closing 2 and closing 1 >= closing 2)
        OR (opening 2 <= closing 1 and closing 2 >= closing 1)

    Our vector is [ opening 1, closing 1, opening 2, closing 2]
                    [0],       [1],       [2],       [3]
    */
    if
    (
        ((sectionAssignments[0] <= sectionAssignments[3]) && (sectionAssignments[1] >= sectionAssignments[3]))
        || ((sectionAssignments[2] <= sectionAssignments[1]) && (sectionAssignments[3] >= sectionAssignments[1]))
    )
        return true;
    else
        return false;
}

int getNumberOfPairsWhereThereExistsOverlap(std::vector<std::vector<std::string>> sectionAssignmentsForElfPairs, const std::string & overlapType)
{
    // Set overlapType to "total" or "partial"
    int numberOfOverlaps = 0;
    std::vector<std::vector<std::string>>::const_iterator cit = sectionAssignmentsForElfPairs.cbegin();
    for ( ; cit != sectionAssignmentsForElfPairs.cend(); ++cit)
    {
        // (*cit) is an std::vector<std::string> with two elements: one for the first elf, and one for the second

        // Make a vector that holds [elf1_start_assignment, elf_1_end_assignment, elf_2_start_assignment, elf_2_end_assignment]
        std::vector<int> sectionAssignments;
        for (auto & s : (*cit))
        {
            size_t dashPosition = s.find("-");
            assert(dashPosition != std::string::npos);
            int startingAssignmentNumber = std::stoi(s.substr(0, dashPosition));
            int endingAssignmentNumber = std::stoi(s.substr(dashPosition + 1, s.size()));
            sectionAssignments.push_back(startingAssignmentNumber);
            sectionAssignments.push_back(endingAssignmentNumber);
        }

        if (overlapType == "total")
        {
            if (isTotalOverlap(sectionAssignments))
                numberOfOverlaps += 1;
        }
        if(overlapType == "partial")
        {
            if(isPartialOverlap(sectionAssignments))
                numberOfOverlaps += 1;
        }
    }
    return numberOfOverlaps;
}

int main()
{
    std::vector<std::string> sectionAssignments = readSectionAssigments("./data/advent_of_code_day_4_input.txt");
    std::vector<std::vector<std::string>> sectionAssignmentsForElfPairs = getSectionAssignmentForPairsOfElves(sectionAssignments);
    int numberOfTotalOverlaps = getNumberOfPairsWhereThereExistsOverlap(sectionAssignmentsForElfPairs, "total");
    std::cout << "The number of pairs of elves where one's assignment are completely contained by the other is " << numberOfTotalOverlaps << std::endl;

    // Part 2
    int numberOfPartialOverlaps = getNumberOfPairsWhereThereExistsOverlap(sectionAssignmentsForElfPairs, "partial");
    std::cout << "The number of pairs of elves where one's assignment are partially contained by the other is " << numberOfPartialOverlaps << std::endl;
    return 0;
}