#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>

using namespace std;




class LexicalAnalyzer {
private:
    template<typename T,int SIZE>
    class meow_queue {
    private:
        int start;
        int end;
        int size;
        T buffer[SIZE];
    public:
        meow_queue() :start(0), end(0), size(SIZE), buffer{} {};
        bool empty()
        {
            return (size == SIZE);
        }
        bool full()
        {
            return !size;
        }
        void push(T element)
        {
            if(full()) throw std::out_of_range("Queue is full");
            else
            {
                end %= SIZE;
                buffer[end] = element;
                end++;
                size--;
            }
        }
        T pop()
        {
            if (empty()) throw std::out_of_range("Queue is Empty");
            else
            {
                start++;
                start %= SIZE;
                size++;
            }
        }
        T front()
        {
            if (empty()) throw std::out_of_range("Queue is Empty");
            start %= SIZE;
            return buffer[start];
        }
        T back()
        {
            if (empty()) throw std::out_of_range("Queue is Empty");
            end %= SIZE;
            return buffer[end];
        }
        int get_size()
        {
            return (SIZE-size);
        }
    };
    int lexeme_begin{};
    int lexeme_end{};
    ifstream input_file{};
    ofstream symbol_table{};
    static const int BUFFER_SIZE = 40;
    meow_queue<char,BUFFER_SIZE> input_buffer;
    unordered_map<int, std::unordered_map<char, int>> transition_table;

public:
    char getNextCharacter()
    {
        if (input_buffer.empty()) return '\0'; //eof
        return input_buffer.front();
    }
    void moveToNextCharacter()
    {
        if(!input_buffer.empty())
            input_buffer.pop();
    }
    void fill_input_buffer()
    {
        char ch;
        while (!input_buffer.full() && input_file.get(ch))
        {
            if (ch != '\r')
                input_buffer.push(ch);
        }
    }
    
    void close_files()
    {
        if (input_file) input_file.close();
        if (symbol_table) symbol_table.close();
    }

    LexicalAnalyzer(string input_file_name, string symbol_table_file_name = "symbol_table.txt") :lexeme_begin(0), lexeme_end(0), input_file(input_file_name), symbol_table(symbol_table_file_name),input_buffer{}
    {
        if (!input_file) {
            cerr << "Input File not found!\n Check if the file is within the project.\n";
            close_files();
            exit(1);
        }
        if (!symbol_table) {
            cerr << "Access Denied!\n Symbol Table's file is not created.\n";
            close_files();
            exit(1);
        }




    }
    ~LexicalAnalyzer()
    {
        close_files();
    }
};

int main() {
   
   /*
    // Step 1: Open input file in read mode
    ifstream inputFile("input.txt");
    if (!inputFile) {
        cerr << "Error opening input file!" << endl;
        return 1; // Exit with error code
    }

    // Step 2: Open output file in write mode
    ofstream outputFile("output.txt");
    if (!outputFile) {
        cerr << "Error creating output file!" << endl;
        return 1;
    }

    // Step 3: Read character by character
    string tempString = "";
    char ch;

    while (inputFile.get(ch)) { // Reads one character at a time
        cout << ch;
        if (ch == '\n') { // When a newline is encountered, write to file
            outputFile << tempString << endl; // Write string to output file
            tempString = ""; // Reset for next line
        }
        else {
            tempString += ch; // Append character to current string
        }
    }

    // Step 4: Write any remaining data to output file
    if (!tempString.empty()) {
        outputFile << tempString << endl;
    }

    // Step 5: Close files
    inputFile.close();
    outputFile.close();

    cout << "File processing completed successfully." << endl; */

    /*LexicalAnalyzer analyzer("input.txt");

    char ch;
    while ((ch = analyzer.test()) != '\0') {
        cout << ch;
    }
    cout << endl;*/


    std::unordered_map<int, std::unordered_map<char, int>> mainMap;

    // Initializing values
    mainMap[0] = { {'a', 10}, {'b', 20}, {'c', 30} };
    mainMap[2] = { {'x', 100}, {'y', 200}, {'z', 300} };
    mainMap[3] = { {'m', 500}, {'n', 600} };

    // Accessing elements
    std::cout << "Value at mainMap[1]['a']: " << mainMap[1]['a'] << std::endl;
    std::cout << "Value at mainMap[2]['y']: " << mainMap[2]['y'] << std::endl;

    // Iterating through the nested hash map
    for (const auto& outerPair : mainMap) {
        int outerKey = outerPair.first;
        std::cout << "Key: " << outerKey << " -> { ";

        for (const auto& innerPair : outerPair.second) {
            char innerKey = innerPair.first;
            int innerValue = innerPair.second;
            std::cout << "'" << innerKey << "': " << innerValue << ", ";
        }
        std::cout << "}" << std::endl;
    }


    return 0;
}
