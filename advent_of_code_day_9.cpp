/*
--- Day 9: Rope Bridge ---
This rope bridge creaks as you walk along it. You aren't sure how old it is, or whether it can even support your weight.

It seems to support the Elves just fine, though. The bridge spans a gorge which was carved out by the massive river far below you.

You step carefully; as you do, the ropes stretch and twist. You decide to distract yourself by modeling rope physics; maybe you can even figure out where not to step.

Consider a rope with a knot at each end; these knots mark the head and the tail of the rope. If the head moves far enough away from the tail, the tail is pulled toward the head.

Due to nebulous reasoning involving Planck lengths, you should be able to model the positions of the knots on a two-dimensional grid. Then, by following a hypothetical series of motions (your puzzle input) for the head, you can determine how the tail will move.

Due to the aforementioned Planck lengths, the rope must be quite short; in fact, the head (H) and tail (T) must always be touching (diagonally adjacent and even overlapping both count as touching):

....
.TH.
....

....
.H..
..T.
....

...
.H. (H covers T)
...
If the head is ever two steps directly up, down, left, or right from the tail, the tail must also move one step in that direction so it remains close enough:

.....    .....    .....
.TH.. -> .T.H. -> ..TH.
.....    .....    .....

...    ...    ...
.T.    .T.    ...
.H. -> ... -> .T.
...    .H.    .H.
...    ...    ...
Otherwise, if the head and tail aren't touching and aren't in the same row or column, the tail always moves one step diagonally to keep up:

.....    .....    .....
.....    ..H..    ..H..
..H.. -> ..... -> ..T..
.T...    .T...    .....
.....    .....    .....

.....    .....    .....
.....    .....    .....
..H.. -> ...H. -> ..TH.
.T...    .T...    .....
.....    .....    .....
You just need to work out where the tail goes as the head follows a series of motions. Assume the head and the tail both start at the same position, overlapping.

For example:

R 4
U 4
L 3
D 1
R 4
D 1
L 5
R 2
This series of motions moves the head right four steps, then up four steps, then left three steps, then down one step, and so on. After each step, you'll need to update the position of the tail if the step means the head is no longer adjacent to the tail. Visually, these motions occur as follows (s marks the starting position as a reference point):

== Initial State ==

......
......
......
......
H.....  (H covers T, s)

== R 4 ==

......
......
......
......
TH....  (T covers s)

......
......
......
......
sTH...

......
......
......
......
s.TH..

......
......
......
......
s..TH.

== U 4 ==

......
......
......
....H.
s..T..

......
......
....H.
....T.
s.....

......
....H.
....T.
......
s.....

....H.
....T.
......
......
s.....

== L 3 ==

...H..
....T.
......
......
s.....

..HT..
......
......
......
s.....

.HT...
......
......
......
s.....

== D 1 ==

..T...
.H....
......
......
s.....

== R 4 ==

..T...
..H...
......
......
s.....

..T...
...H..
......
......
s.....

......
...TH.
......
......
s.....

......
....TH
......
......
s.....

== D 1 ==

......
....T.
.....H
......
s.....

== L 5 ==

......
....T.
....H.
......
s.....

......
....T.
...H..
......
s.....

......
......
..HT..
......
s.....

......
......
.HT...
......
s.....

......
......
HT....
......
s.....

== R 2 ==

......
......
.H....  (H covers T)
......
s.....

......
......
.TH...
......
s.....
After simulating the rope, you can count up all of the positions the tail visited at least once. In this diagram, s again marks the starting position (which the tail also visited) and # marks other positions the tail visited:

..##..
...##.
.####.
....#.
s###..
So, there are 13 positions the tail visited at least once.

Simulate your complete hypothetical series of motions. How many positions does the tail of the rope visit at least once?
*/

#include<iostream>
#include<string>
#include<vector>
#include<fstream>
#include<map>
#include<assert.h>

std::vector<std::string> readInstructions(const std::string& filename)
{
  std::ifstream file(filename);
  std::string line;
  std::vector<std::string> vectorToReturn;
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

std::vector<char> parseInstructions(std::vector<std::string> instructions)
{
  std::vector<char> vectorToReturn;
  for (auto & line : instructions)
  {
    char direction = line.at(0);
    size_t spacePosition = line.find(" ");
    int repetitions = std::atoi(line.substr(spacePosition + 1).c_str());
    for (int i = 0; i < repetitions; ++i)
      vectorToReturn.push_back(direction);
  }
  return vectorToReturn;
}

class Knot
{
  public:
    Knot() : m_X(0), m_Y(0) {};
    void applyMovement(char direction)
    {
      if (direction == 'R')
        ++m_X;
      else if (direction == 'L')
        --m_X;
      else if (direction == 'U')
        ++m_Y;
      else
        --m_Y;
    }
  int getXPosition() { return m_X;};
  int getYPosition() { return m_Y;};
  std::pair<int, int> getXYPosition(){ return std::pair<int, int>(m_X, m_Y);}
  private:
    int m_X;
    int m_Y;
};

std::pair<int, int> getRelativePosition(Knot head, Knot tail)
{
  return std::pair<int, int>(tail.getXPosition() - head.getXPosition(), tail.getYPosition() - head.getYPosition());
}

std::vector<char> getTailMovementPattern(std::pair<int, int> relativePosition)
{
  std::vector<char> vectorToReturn;
  std::map<std::pair<int, int>, std::vector<char>> patternLibrary;
  patternLibrary.insert({std::pair<int, int>(0, 0), std::vector<char>()});
  patternLibrary.insert({std::pair<int, int>(1, 0), std::vector<char>()});
  patternLibrary.insert({std::pair<int, int>(-1, 0), std::vector<char>()});
  patternLibrary.insert({std::pair<int, int>(0, 1), std::vector<char>()});
  patternLibrary.insert({std::pair<int, int>(0, -1), std::vector<char>()});
  patternLibrary.insert({std::pair<int, int>(1, 1), std::vector<char>()});
  patternLibrary.insert({std::pair<int, int>(1, -1), std::vector<char>()});
  patternLibrary.insert({std::pair<int, int>(-1, 1), std::vector<char>()});
  patternLibrary.insert({std::pair<int, int>(-1, -1), std::vector<char>()});
  patternLibrary.insert({std::pair<int, int>(2, 0), std::vector<char>{'L'}});
  patternLibrary.insert({std::pair<int, int>(-2, 0), std::vector<char>{'R'}});
  patternLibrary.insert({std::pair<int, int>(0, 2), std::vector<char>{'D'}});
  patternLibrary.insert({std::pair<int, int>(0, -2), std::vector<char>{'U'}});
  patternLibrary.insert({std::pair<int, int>(2, 1), std::vector<char>{'L', 'D'}});
  patternLibrary.insert({std::pair<int, int>(2, -1), std::vector<char>{'L', 'U'}});
  patternLibrary.insert({std::pair<int, int>(-2, 1), std::vector<char>{'R', 'D'}});
  patternLibrary.insert({std::pair<int, int>(-2, -1), std::vector<char>{'R', 'U'}});
  patternLibrary.insert({std::pair<int, int>(1, 2), std::vector<char>{'L', 'D'}});
  patternLibrary.insert({std::pair<int, int>(1, -2), std::vector<char>{'L', 'U'}});
  patternLibrary.insert({std::pair<int, int>(-1, 2), std::vector<char>{'R', 'D'}});
  patternLibrary.insert({std::pair<int, int>(-1, -2), std::vector<char>{'R', 'U'}});
  // Part 2: Need to entertain what happens if they are two apart
  patternLibrary.insert({std::pair<int, int>(-2, -2), std::vector<char>{'R', 'U'}});
  patternLibrary.insert({std::pair<int, int>(2, 2), std::vector<char>{'L', 'D'}});
  patternLibrary.insert({std::pair<int, int>(-2, 2), std::vector<char>{'R', 'D'}});
  patternLibrary.insert({std::pair<int, int>(2, -2), std::vector<char>{'L', 'U'}});

  std::map<std::pair<int, int>, std::vector<char>>::iterator it = patternLibrary.find(relativePosition);
  assert (it != patternLibrary.end());
  vectorToReturn = it->second;
  return vectorToReturn;
}

int main()
{
  std::vector<std::string> instructions = readInstructions("./data/advent_of_code_day_9_input.txt");
  std::vector<char> parsedInstructions = parseInstructions(instructions);
  std::map<std::pair<int, int>, int> positionsVisitedByTail;
  Knot head = Knot();
  Knot tail = Knot();
  positionsVisitedByTail.insert({tail.getXYPosition(), 1});

  for (auto & dir : parsedInstructions)
  {
    head.applyMovement(dir);
    std::pair<int, int> relPosition = getRelativePosition(head, tail);
    std::vector<char> tailMovementPattern = getTailMovementPattern(relPosition);
    for (auto & c : tailMovementPattern)
    {
      tail.applyMovement(c);
    }
    std::map<std::pair<int, int>, int>::iterator it = positionsVisitedByTail.find(tail.getXYPosition());
    if (it == positionsVisitedByTail.end())
      positionsVisitedByTail.insert({tail.getXYPosition(), 1});
  }
  std::cout << "PART 1" << std::endl;
  std::cout << "After instructions, head's final x-position is " << head.getXPosition() << std::endl;
  std::cout << "After instructions, head's final y-position is " << head.getYPosition() << std::endl;
  std::cout << "After instructions, tail's final x-position is " << tail.getXPosition() << std::endl;
  std::cout << "After instructions, tail's final y-position is " << tail.getYPosition() << std::endl;
  std::cout << "The number of positions visited by the tail is " << positionsVisitedByTail.size() << std::endl << std::endl;;

  // Part 2
  // It's useful to use the sample input for part 2 and compare it with the worked example
  // instructions = readInstructions("./data/advent_of_code_day_9_part_2_sample_input.txt");

  // parsedInstructions = parseInstructions(instructions);
  std::vector<Knot> rope(10);
  positionsVisitedByTail.clear();
  positionsVisitedByTail.insert({rope[9].getXYPosition(), 1});
  for (auto  & dir : parsedInstructions)
  {
    // Apply the movement to the head
    rope[0].applyMovement(dir);

    // Then for each subsequent knot, get the relative position to the previous knot
    for (int i = 1; i < rope.size(); ++i)
    {
      std::pair<int, int> relPosition = getRelativePosition(rope[i-1], rope[i]);
      std::vector<char> nextKnotMovementPattern = getTailMovementPattern(relPosition);
      for (auto & c : nextKnotMovementPattern)
      {
        rope[i].applyMovement(c);
      }
      std::map<std::pair<int, int>, int>::iterator it = positionsVisitedByTail.find(rope[9].getXYPosition());
      if (it == positionsVisitedByTail.end())
        positionsVisitedByTail.insert({rope[9].getXYPosition(), 1});
    }
  }
  std::cout << "PART 2" << std::endl;
  std::cout << "After instructions, head's final x-position is " << rope[0].getXPosition() << std::endl;
  std::cout << "After instructions, head's final y-position is " << rope[0].getYPosition() << std::endl;
  std::cout << "After instructions, tail's final x-position is " << rope[9].getXPosition() << std::endl;
  std::cout << "After instructions, tail's final y-position is " << rope[9].getYPosition() << std::endl;
  std::cout << "The number of positions visited by the tail is " << positionsVisitedByTail.size() << std::endl;
  return 0;

}