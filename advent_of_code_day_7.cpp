/*
--- Day 7: No Space Left On Device ---
You can hear birds chirping and raindrops hitting leaves as the expedition proceeds. Occasionally, you can even hear much louder sounds in the 
distance; how big do the animals get out here, anyway?

The device the Elves gave you has problems with more than just its communication system. You try to run a system update:

$ system-update --please --pretty-please-with-sugar-on-top
Error: No space left on device
Perhaps you can delete some files to make space for the update?

You browse around the filesystem to assess the situation and save the resulting terminal output (your puzzle input). For example:

$ cd /
$ ls
dir a
14848514 b.txt
8504156 c.dat
dir d
$ cd a
$ ls
dir e
29116 f
2557 g
62596 h.lst
$ cd e
$ ls
584 i
$ cd ..
$ cd ..
$ cd d
$ ls
4060174 j
8033020 d.log
5626152 d.ext
7214296 k
The filesystem consists of a tree of files (plain data) and directories (which can contain other directories or files). The outermost directory is called /.
You can navigate around the filesystem, moving into or out of directories and listing the contents of the directory you're currently in.

Within the terminal output, lines that begin with $ are commands you executed, very much like some modern computers:

cd means change directory. This changes which directory is the current directory, but the specific result depends on the argument:
cd x moves in one level: it looks in the current directory for the directory named x and makes it the current directory.
cd .. moves out one level: it finds the directory that contains the current directory, then makes that directory the current directory.
cd / switches the current directory to the outermost directory, /.
ls means list. It prints out all of the files and directories immediately contained by the current directory:
123 abc means that the current directory contains a file named abc with size 123.
dir xyz means that the current directory contains a directory named xyz.
Given the commands and output in the example above, you can determine that the filesystem looks visually like this:

- / (dir)
  - a (dir)
    - e (dir)
      - i (file, size=584)
    - f (file, size=29116)
    - g (file, size=2557)
    - h.lst (file, size=62596)
  - b.txt (file, size=14848514)
  - c.dat (file, size=8504156)
  - d (dir)
    - j (file, size=4060174)
    - d.log (file, size=8033020)
    - d.ext (file, size=5626152)
    - k (file, size=7214296)
Here, there are four directories: / (the outermost directory), a and d (which are in /), and e (which is in a). These directories also contain files of various sizes.

Since the disk is full, your first step should probably be to find directories that are good candidates for deletion. To do this, you need to determine the total size of
each directory. The total size of a directory is the sum of the sizes of the files it contains, directly or indirectly. (Directories themselves do not count as having
any intrinsic size.)

The total sizes of the directories above can be found as follows:

The total size of directory e is 584 because it contains a single file i of size 584 and no other directories.
The directory a has total size 94853 because it contains files f (size 29116), g (size 2557), and h.lst (size 62596), plus file i indirectly (a contains e which contains i).
Directory d has total size 24933642.
As the outermost directory, / contains every file. Its total size is 48381165, the sum of the size of every file.
To begin, find all of the directories with a total size of at most 100000, then calculate the sum of their total sizes. In the example above, these directories are a and e; the sum
of their total sizes is 95437 (94853 + 584). (As in this example, this process can count files more than once!)

Find all of the directories with a total size of at most 100000. What is the sum of the total sizes of those directories?

--- Part Two ---
Now, you're ready to choose a directory to delete.

The total disk space available to the filesystem is 70000000. To run the update, you need unused space of at least 30000000. You need to find a directory you can delete that will free up enough space to run the update.

In the example above, the total size of the outermost directory (and thus the total amount of used space) is 48381165; this means that the size of the unused space must currently be 21618835, which isn't quite the 30000000 required by the update. Therefore, the update still requires a directory with total size of at least 8381165 to be deleted before it can run.

To achieve this, you have the following options:

Delete directory e, which would increase unused space by 584.
Delete directory a, which would increase unused space by 94853.
Delete directory d, which would increase unused space by 24933642.
Delete directory /, which would increase unused space by 48381165.
Directories e and a are both too small; deleting them would not free up enough space. However, directories d and / are both big enough! Between these, choose the smallest: d, increasing unused space by 24933642.

Find the smallest directory that, if deleted, would free up enough space on the filesystem to run the update. What is the total size of that directory?
*/

#include<string>
#include<iostream>
#include<fstream>
#include<vector>
#include<map>
#include<assert.h>
#include<queue>


class FolderNode
{
public:
    FolderNode(const std::string& folderName) : m_FolderName(folderName), m_FilesAndSizes{}, m_Children{}, m_Parent{}, m_IsDirectorySizeCalculated{false}, m_DirectorySize{0} {};
    void setChildren(std::vector<FolderNode *> children){m_Children = children;}
    std::vector<FolderNode *> getChildren(){ return m_Children; }
    // void setFilesAndSizes(std::map<std::string, int> filesAndSizes){ m_FilesAndSizes = filesAndSizes; }
    // std::map<std::string, int> getFilesAndSizes(){ return m_FilesAndSizes; }
    void setParent(FolderNode * parentNode){ m_Parent = parentNode; }
    FolderNode* getParent(){ return m_Parent; }
    std::string getFolderName(){ return m_FolderName; }

    void addChild(FolderNode * child){ m_Children.push_back(child); }

    void addFileNameAndFileSize(std::string filename, int filesize)
    {
        // Check if we already have an entry in the map for the file name
        std::map<std::string, int>::iterator it = m_FilesAndSizes.find(filename);

        if (it != m_FilesAndSizes.end())
        {
            // So if we already have an entry for this file, we want to check that the 
            // file size is the same and do nothing
            assert(it->second == filesize);
        }
        else
            m_FilesAndSizes.emplace(filename, filesize);
    }

    int calculateDirectorySize()
    {
        int totalSum = 0;
        // In the current directory, sum up the size of the files
        for (std::map<std::string, int>::iterator it = m_FilesAndSizes.begin(); it != m_FilesAndSizes.end(); ++it)
            totalSum += it->second;

        // Now we have to get the size of all the children directories
        for (std::vector<FolderNode* >::iterator jt = m_Children.begin(); jt != m_Children.end(); ++jt)
            totalSum += (*jt)->calculateDirectorySize();
        
        return totalSum;
    }

    int getDirectorySize()
    {
        if (!m_IsDirectorySizeCalculated)
        {
            m_DirectorySize = calculateDirectorySize();
            m_IsDirectorySizeCalculated = true;
        }
        return m_DirectorySize;
    }

private:
    std::string m_FolderName;
    std::map<std::string, int> m_FilesAndSizes;
    std::vector<FolderNode*> m_Children;
    FolderNode* m_Parent;
    bool m_IsDirectorySizeCalculated;
    int m_DirectorySize;
};

std::vector<std::string> readInTerminalOutput(std::string fileName)
{
    std::ifstream file (fileName);
    std::string line;
    std::vector<std::string> vectorToReturn;
    if (file.is_open())
    {
        while(std::getline(file, line))
            vectorToReturn.push_back(line);
        file.close();
    }
    return vectorToReturn;
}

bool existsDirectory(FolderNode * cursor, std::string folderName)
{
    // The purpose of this function is to determine if a folder with name folderName
    // exists within the children of cursor.
    std::vector<FolderNode *> children = cursor->getChildren();
    std::vector<FolderNode *>::iterator it = std::find_if(children.begin(),
                                                          children.end(),
                                                          [folderName](FolderNode * folderNode)
                                                          {
                                                              if (folderNode->getFolderName() == folderName)
                                                                  return true;
                                                              else
                                                                  return false;
                                                          });
    return it == children.end() ? false : true;
}

FolderNode * findFolderWithinChildren(FolderNode * cursor, std::string folderName)
{
    std::vector<FolderNode *> children = cursor->getChildren();
    std::vector<FolderNode *>::iterator it = std::find_if(children.begin(),
                                                          children.end(),
                                                          [folderName](FolderNode * node)
                                                          {
                                                               if (node->getFolderName() == folderName)
                                                                   return true;
                                                               else
                                                                   return false;
                                                          });
    assert(it != children.end());
    return (*it);

                    
}

FolderNode * parseTerminalOutput(const std::vector<std::string>& terminalOutput)
{
    // Define a pointer to the root node
    FolderNode * rootNode = nullptr;

    // Define a pointer to a cursor
    FolderNode * cursor;

    for (auto & line : terminalOutput)
    {
        // We need to identify our inputs, which begin with "$"
        // $ cd /
        // $ cd fml
        // $ cd ..
        // $ ls
        if (line.substr(0, 1) == "$")
        {
            if (line.substr(2, 2) == "cd")
            {
                // Two possibilities: choose a folder or go up a level
                if (line.substr(5) == "..")
                {
                    cursor = cursor->getParent();
                    continue;
                }
                // Here we are choosing a folder
                else
                {
                    std::string directoryName = line.substr(5);

                    // If this is the first line, then create a new folder node
                    if (!rootNode)
                    {
                        rootNode = new FolderNode(directoryName);
                        cursor = &(*rootNode);
                        continue;
                    }

                    // Find directory name within the children
                    bool doesDirectoryExist = existsDirectory(cursor, directoryName);
                    if (!doesDirectoryExist)
                    {
                        FolderNode * newFolder = new FolderNode(directoryName);
                        
                        // Set the parent node of the new folder to the cursor.
                        newFolder->setParent(cursor);

                        // Then add the new folder as a child of the cursor.
                        cursor->addChild(newFolder);

                        // Then go into the directory
                        cursor = findFolderWithinChildren(cursor, directoryName);
                    }
                    else
                    {
                        // If the folder was found, then let's move the cursor to it
                        cursor = findFolderWithinChildren(cursor, directoryName); 
                    }
                    continue;
                }

            }
            else if (line.substr(2, 2) == "ls")
            {
                // ls technically doesn't do anything. It's just a place holder to tell us that we're gonna list
                // off a bunch of stuff
                continue;
            }
        }
        // The other lines are just the result from ls
        else
        {
            // Output of ls will look like
            // 240038 dwhl.nrn
            // 124868 dwhl.vvb
            // dir fml
            // dir jbgpgvj
            //
            // This means that it is written as ["dir", folder_name] or [number, file_name]
            // Check if the first three characters are
            if (line.substr(0, 3) == "dir")
            {
                std::string directoryName = line.substr(4);
                // If this directory exists within the children nothing to do
                if (existsDirectory(cursor, directoryName))
                    continue;
                else
                {
                    FolderNode * childNode = new FolderNode(directoryName);
                    childNode->setParent(cursor);
                    cursor->addChild(childNode);
                    continue;
                }
            }
            else
            {
                // Now we are dealing with files.
                // Find the space that splits the file size from the file name

                size_t spacePosition = line.find(" ");
                assert(spacePosition != std::string::npos);
                int fileSize = std::atoi(line.substr(0, spacePosition).c_str());
                std::string fileName = line.substr(spacePosition + 1);
                cursor->addFileNameAndFileSize(fileName, fileSize);
            }
            continue;
        }
    }
    return rootNode;
}

void calculateSumOfDirectoriesWithSizeLessThanThreshold(FolderNode * root, int threshold, int & total)
{
    int dirSize = root->getDirectorySize();
    if (dirSize < threshold)
        total += dirSize;
    std::vector<FolderNode *> children = root->getChildren();
    for (int i = 0; i < children.size(); ++i)
        calculateSumOfDirectoriesWithSizeLessThanThreshold(children[i], threshold, total);
}

// Part 2
int calculateSizeOfSmallestDirectoryToDelete(int totalFileSystemSize, int spaceRequiredForUpdate, FolderNode * root)
{
    // Get current amount of used space
    int usedSpace = root->getDirectorySize();

    // Get free space
    int freeSpace = totalFileSystemSize - usedSpace;

    // Get the additional amount of space needed
    int additionalRequiredSpace = spaceRequiredForUpdate - freeSpace;

    // Make a vector to hold the sizes of all directories whose space exceeds additionalRequiredSpace.
    std::vector<int> candidates;

    std::queue<FolderNode *> queue;
    queue.push(root);
    FolderNode * cur;
    int dirSize;

    while(!queue.empty())
    {
        cur = queue.front();
        queue.pop();
        dirSize = cur->getDirectorySize();
        if (dirSize > additionalRequiredSpace)
            candidates.push_back(dirSize);
        
        std::vector<FolderNode *> children = cur->getChildren();
        for (int i = 0; i < children.size(); ++i)
        {
            queue.push(children[i]);
        }
    }

    return *std::min_element(candidates.begin(), candidates.end());


}


int main()
{
    std::vector<std::string> terminalOutput = readInTerminalOutput("./data/advent_of_code_day_7_input.txt");
    FolderNode * root = parseTerminalOutput(terminalOutput);
    int total = 0;
    int threshold = 100000;
    // Now traverse the file tree and count the folders whose folder sizes are < 10k
    calculateSumOfDirectoriesWithSizeLessThanThreshold(root, threshold, total);
    std::cout << "The total size of all directories that have a size less than " << threshold << " is " << total << std::endl;

    // Part 2: We know that the system has a size of 70000000 (70M). We need 30000000 (30M) for the update.
    int sizeOfSmallesDirectoryToDelete = calculateSizeOfSmallestDirectoryToDelete(70000000, 30000000, root);
    std::cout << "The size of the smallest directory to delete is " << sizeOfSmallesDirectoryToDelete << std::endl;


    return 0;
}