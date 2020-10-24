// C++ program for Huffman Coding
#include <iostream>
#include <queue>
#include <map>
#include <string>
#include <fstream>
#include <ctype.h>
#include <stdio.h>
using namespace std;

//Global variable
map<char, string> huff;

// A Huffman tree node
class MinHeapNode
{
public:
    // One of the input characters
    char data;

    // Frequency of the character
    unsigned freq;

    // Left and right child
    MinHeapNode *left, *right;

    MinHeapNode(char data, unsigned freq)

    {

        left = right = NULL;
        this->data = data;
        this->freq = freq;
    }
};

// For comparison of
// two heap nodes (needed in min heap)
struct compare
{

    bool operator()(MinHeapNode *l, MinHeapNode *r)

    {
        return (l->freq > r->freq);
    }
};

// Prints huffman codes from
// the root of Huffman Tree.
void printCodes(MinHeapNode *root, string str)
{
    if (!root)
    {
        return;
    }

    if (root->data != '$')
    {
        cout << root->data << ": " << str << "\n";
        huff.insert(pair<char, string>(root->data, str));
    }

    printCodes(root->left, str + "0");
    printCodes(root->right, str + "1");
}

// The main function that builds a Huffman Tree and
// print codes by traversing the built Huffman Tree
void HuffmanCodes(char data[], int freq[], int size)
{
    MinHeapNode *left, *right, *top;

    // Create a min heap & inserts all characters of data[]
    priority_queue<MinHeapNode *, vector<MinHeapNode *>, compare> minHeap;

    for (int i = 0; i < size; ++i)
        minHeap.push(new MinHeapNode(data[i], freq[i]));

    // Iterate while size of heap doesn't become 1
    while (minHeap.size() != 1)
    {

        // Extract the two minimum
        // freq items from min heap
        left = minHeap.top();
        minHeap.pop();

        right = minHeap.top();
        minHeap.pop();

        // Create a new internal node with
        // frequency equal to the sum of the
        // two nodes frequencies. Make the
        // two extracted node as left and right children
        // of this new node. Add this node
        // to the min heap '$' is a special value
        // for internal nodes, not used
        top = new MinHeapNode('$', left->freq + right->freq);

        top->left = left;
        top->right = right;

        minHeap.push(top);
    }

    // Print Huffman codes using
    // the Huffman tree built above
    printCodes(minHeap.top(), "");
}

std::string getfile(std::fstream &is)
{
    std::string contents;
    // Here is one way to read the whole file
    for (char ch; is.get(ch); contents.push_back(ch))
    {
    }
    return contents;
}

string getString(char x)
{
    // string class has a constructor
    // that allows us to specify size of
    // string as first parameter and character
    // to be filled in given size as second
    // parameter.
    string s(1, x);

    return s;
}

void find_and_replace(std::string &file_contents,
                      const std::string &fin, const std::string &repl)
{
    // This searches the file for the first occurence of the fin string.
    auto pos = file_contents.find(fin);
    while (pos != std::string::npos)
    {
        file_contents.replace(pos, fin.length(), repl);
        // Continue searching from here.
        pos = file_contents.find(fin, pos);
    }
    // std::cout << file_contents << std::endl;
}

//Makes all the content in the file to lower case
void makelowercase(std::string file)
{
    std::fstream theFile;
    theFile.open(file);
    if (!theFile)
    {
        cout << "Sorry the file: " << file << " is not found!\n";
        return;
    }
    std::string theLine, str;
    while (!theFile.eof())
    {
        theFile >> theLine;
        for (size_t j = 0; j < theLine.length(); ++j)
        {
            theLine[j] = tolower(theLine[j]);
        }
        str += theLine + " ";
    }
    theFile.close();
    std::ofstream change(file, std::ios::out);
    change << str;
    change.close();
    return;
}

// Driver program to test above functions
int main()
{
    fstream text;
    ofstream textWrite;
    char letter;
    string join;
    int counter = 0;

    string file, rep;
    cout << "Enter the filename to be compressed: ";
    cin >> file;

    char arr[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};

    //This is the size of the array of letters
    int size = sizeof(arr) / sizeof(arr[0]);
    int freq[size];

    makelowercase(file);

    for (int i = 0; i < size; i++)
    {
        text.open(file);
        if (!text)
        {
            cout << "Sorry the file: " << file << " is not found!\n";
            return 1;
        }
        while (!text.eof())
        {
            //Not necessary anymore
            letter = text.get();
            //Making lowercase
            letter = (char)(tolower(letter));

            if (letter == arr[i])
            {
                counter++;
            }
        }
        freq[i] = counter;
        counter = 0;
        text.close();
    }

    ifstream in_file(file, ios::binary);
    if (!in_file)
    {
        cout << "Sorry the file: " << file << " is not found!\n";
        return 1;
    }
    in_file.seekg(0, ios::end);
    float file_size1 = in_file.tellg();
    cout << "Size of the file is"
         << " " << file_size1 << " "
         << "bytes\n";
    cout << "That is"
         << " " << file_size1 * 8 << " "
         << "bits\n";
    //in_file.close();
    /*ofstream xo("encoding.txt");
    check:
    for (int i = 0; i < size; i++)
    {
        xo << arr[i] << "==>" << freq[i] << endl;
    }
    xo.close();
    */

    HuffmanCodes(arr, freq, size);

    std::fstream cont(file);
    std::string contents = getfile(cont); //Read the entire file

    for (auto itr = huff.begin(); itr != huff.end(); ++itr)
    {
        find_and_replace(contents, getString(itr->first), itr->second);
    }
    cout << "\n";

    textWrite.open(file);
    textWrite << contents;
    textWrite.close();

    in_file.seekg(0, ios::end);
    float file_size2 = in_file.tellg();
    cout << "After compression size of the file is"
         << " " << file_size2 / 8 << " "
         << "byte\n";
    cout << "That is"
         << " " << file_size2 << " "
         << "bits\n";
    float percent = (100 / (file_size1 / (file_size2 / 8)));
    cout << "Percentage reduced: " << percent << "%\n";
    in_file.close();

    return 0;
}
