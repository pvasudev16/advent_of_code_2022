/*
--- Day 6: Tuning Trouble ---
The preparations are finally complete; you and the Elves leave camp on foot and begin to make your way toward the star fruit grove.

As you move through the dense undergrowth, one of the Elves gives you a handheld device. He says that it has many fancy features, but
the most important one to set up right now is the communication system.

However, because he's heard you have significant experience dealing with signal-based systems, he convinced the other Elves that it
would be okay to give you their one malfunctioning device - surely you'll have no problem fixing it.

As if inspired by comedic timing, the device emits a few colorful sparks.

To be able to communicate with the Elves, the device needs to lock on to their signal. The signal is a series of seemingly-random
characters that the device receives one at a time.

To fix the communication system, you need to add a subroutine to the device that detects a start-of-packet marker in the datastream.
In the protocol being used by the Elves, the start of a packet is indicated by a sequence of four characters that are all different.

The device will send your subroutine a datastream buffer (your puzzle input); your subroutine needs to identify the first position where
the four most recently received characters were all different. Specifically, it needs to report the number of characters from the beginning
of the buffer to the end of the first such four-character marker.

For example, suppose you receive the following datastream buffer:

mjqjpqmgbljsphdztnvjfqwrcgsmlb
After the first three characters (mjq) have been received, there haven't been enough characters received yet to find the marker. The first time
a marker could occur is after the fourth character is received, making the most recent four characters mjqj. Because j is repeated, this isn't a marker.

The first time a marker appears is after the seventh character arrives. Once it does, the last four characters received are jpqm, which are all different.
In this case, your subroutine should report the value 7, because the first start-of-packet marker is complete after 7 characters have been processed.

Here are a few more examples:

bvwbjplbgvbhsrlpgdmjqwftvncz: first marker after character 5
nppdvjthqldpwncqszvftbrmjlhg: first marker after character 6
nznrnfrfntjfmvfwmzdfjlvtqnbhcprsg: first marker after character 10
zcfzfwzzqfrljwzlrfnpqdbhtmscgvjw: first marker after character 11
How many characters need to be processed before the first start-of-packet marker is detected?

--- Part Two ---
Your device's communication system is correctly detecting packets, but still isn't working. It looks like it also needs to look for messages.

A start-of-message marker is just like a start-of-packet marker, except it consists of 14 distinct characters rather than 4.

Here are the first positions of start-of-message markers for all of the above examples:

mjqjpqmgbljsphdztnvjfqwrcgsmlb: first marker after character 19
bvwbjplbgvbhsrlpgdmjqwftvncz: first marker after character 23
nppdvjthqldpwncqszvftbrmjlhg: first marker after character 23
nznrnfrfntjfmvfwmzdfjlvtqnbhcprsg: first marker after character 29
zcfzfwzzqfrljwzlrfnpqdbhtmscgvjw: first marker after character 26
How many characters need to be processed before the first start-of-message marker is detected?
*/

#include<fstream>
#include<iostream>
#include<vector>
#include<map>
#include<assert.h>

class DataStream
{
public:
    DataStream(const std::string & filename)
    {
        m_File.open(filename);
    }
    
    std::ifstream& getDataStream() {return m_File;}

    void resetDataStream()
    {
        m_File.clear();
        m_File.seekg(0);
    }
private:
    std::ifstream m_File;    
};

bool isStartingPacket(std::map<char, int> & charCountMap)
{
    for (std::map<char, int>::iterator it = charCountMap.begin(); it != charCountMap.end(); ++it)
    {
        if (it->second > 1)
            return false;
    }
    return true;
}

void advancePacket(char * packet, char c, std::map<char, int>& charCountMap, int packetSize)
{

    // Decrement the count of the first character that will go bye-bye
    std::map<char, int>::iterator it = charCountMap.find(packet[0]);
    assert (it != charCountMap.end());
    it->second -= 1;

    // Now increment the count of the newest character
    it = charCountMap.find(c);
    if (it == charCountMap.end())
        charCountMap.emplace(c, 1);
    else
        it->second +=1;

    
    // Shift characters 2, 3, ... packetSize left
    for (int i = 0; i < packetSize - 1; ++i)
        packet[i] = packet[i + 1];

    // Re-assign rightmost character
    packet[packetSize - 1] = c;
}

int positionOfStartOfPacket(std::ifstream & dataStream, int packetSize)
{
    int position = 0;
    bool foundStartingPacket = false;
    char packet[packetSize + 1];
    char c;

    std::map<char, int> charCountMap;
    std::map<char, int>::iterator it;

    while (dataStream.get(c))
    {
        // Populate the first four characters
        if (position < packetSize)
        {
            // Update the packet
            packet[position] = c;

            // Update the char count map
            it = charCountMap.find(c);
            if (it == charCountMap.end())
                charCountMap.emplace(c, 1);
            else
                it->second += 1;

            ++position;

            // Nothing else to do
            continue;
        }

        foundStartingPacket = isStartingPacket(charCountMap);

        if (foundStartingPacket)
            return position;
        advancePacket(packet, c, charCountMap, packetSize);
        ++position;
    }
    return position;
}

int main()
{
    DataStream dataStreamObject = DataStream("./data/advent_of_code_day_6_input.txt");
    std::ifstream & dataStream = dataStreamObject.getDataStream();
    int packetSize = 4;
    int pos = positionOfStartOfPacket(dataStream, packetSize);
    std::cout << "The position of the start-of-packet is " << pos << std::endl;


    // Part 2
    dataStreamObject.resetDataStream();
    packetSize = 14;
    pos = positionOfStartOfPacket(dataStream, packetSize);
    std::cout << "The position of the start-of-message is " << pos << std::endl;
    return 0;
}