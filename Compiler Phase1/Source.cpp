#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cctype>
#include <unordered_map>
#include <vector>
#include <stack>
using namespace std;

std::unordered_map<std::string, int> keywords_table = {
    {"Adadi", 1},
    {"Agar", 2},
    {"Ashriya", 3},
    {"False", 4},
    {"Harf", 5},
    {"Khali", 6},
    {"Mantiqi", 7},
    {"Marqazi", 8},
    {"Matn", 9},
    {"True", 10},
    {"Wagarna", 11},
    {"Wapas", 12},
    {"asm", 13},
    {"auto", 14},
    {"break", 15},
    {"case", 16},
    {"catch", 17},
    {"class", 18},
    {"const", 19},
    {"continue", 20},
    {"default", 21},
    {"delete", 22},
    {"do", 23},
    {"dost", 24},
    {"double", 25},
    {"enum", 26},
    {"explicit", 27},
    {"export", 28},
    {"extern", 29},
    {"for", 30},
    {"goto", 31},
    {"inline", 32},
    {"input->", 33},
    {"long", 34},
    {"mutable", 35},
    {"namespace", 36},
    {"new", 37},
    {"operator", 38},
    {"output<-", 39},
    {"private", 40},
    {"protected", 41},
    {"public", 42},
    {"register", 43},
    {"short", 44},
    {"signed", 45},
    {"sizeof", 46},
    {"static", 47},
    {"struct", 48},
    {"switch", 49},
    {"template", 50},
    {"this", 51},
    {"throw", 52},
    {"try", 53},
    {"typedef", 54},
    {"typeid", 55},
    {"typename", 56},
    {"union", 57},
    {"unsigned", 58},
    {"using", 59},
    {"virtual", 60},
    {"volatile", 61},
    {"while", 62},
};
std::unordered_map<std::string, std::string> rkeywords_map = {
    {"1", "Adadi"},
    {"2", "Agar"},
    {"3", "Ashriya"},
    {"4", "False"},
    {"5", "Harf"},
    {"6", "Khali"},
    {"7", "Mantiqi"},
    {"8", "Marqazi"},
    {"9", "Matn"},
    {"10", "True"},
    {"11", "Wagarna"},
    {"12", "Wapas"},
    {"13", "asm"},
    {"14", "auto"},
    {"15", "break"},
    {"16", "case"},
    {"17", "catch"},
    {"18", "class"},
    {"19", "const"},
    {"20", "continue"},
    {"21", "default"},
    {"22", "delete"},
    {"23", "do"},
    {"24", "dost"},
    {"25", "double"},
    {"26", "enum"},
    {"27", "explicit"},
    {"28", "export"},
    {"29", "extern"},
    {"30", "for"},
    {"31", "goto"},
    {"32", "inline"},
    {"33", "input->"},
    {"34", "long"},
    {"35", "mutable"},
    {"36", "namespace"},
    {"37", "new"},
    {"38", "operator"},
    {"39", "output<-"},
    {"40", "private"},
    {"41", "protected"},
    {"42", "public"},
    {"43", "register"},
    {"44", "short"},
    {"45", "signed"},
    {"46", "sizeof"},
    {"47", "static"},
    {"48", "struct"},
    {"49", "switch"},
    {"50", "template"},
    {"51", "this"},
    {"52", "throw"},
    {"53", "try"},
    {"54", "typedef"},
    {"55", "typeid"},
    {"56", "typename"},
    {"57", "union"},
    {"58", "unsigned"},
    {"59", "using"},
    {"60", "virtual"},
    {"61", "volatile"},
    {"62", "while"},
};
std::unordered_map<string, int> symbol_table{};
std::unordered_map<string, int> literal_table{};
class LexicalAnalyzer {

private:
    //Data Structures
    template<typename T, int SIZE>
    class meow_queue {
    private:
        static const int HALF_SIZE = SIZE / 2;
        int start, end, size;
        T buffer1[HALF_SIZE]{};
        T buffer2[HALF_SIZE]{};

    public:
        meow_queue() : start(0), end(0), size(SIZE) {}

        bool empty() {
            return size == SIZE;
        }

        bool full() {
            return size == 0;
        }

        void push(T element) {
            if (full()) throw std::out_of_range("Queue is full");

            if (end < HALF_SIZE) {
                buffer1[end] = element;
            }
            else {
                buffer2[end - HALF_SIZE] = element;
            }

            end = (end + 1) % SIZE;
            size--;
        }

        void pop() {
            if (empty()) throw std::out_of_range("Queue is Empty");

            start = (start + 1) % SIZE;
            size++;
        }

        T front() {
            if (empty()) throw std::out_of_range("Queue is Empty");

            return (start < HALF_SIZE) ? buffer1[start] : buffer2[start - HALF_SIZE];
        }

        T peek_next_front() {
            if (get_size() < 2) throw std::out_of_range("Queue is of size < 2");
            int next_start = (start + 1) % SIZE;
            return (next_start < HALF_SIZE) ? buffer1[next_start] : buffer2[next_start - HALF_SIZE];
        }

        T back() {
            if (empty()) throw std::out_of_range("Queue is Empty");

            int last_index = (end == 0) ? SIZE - 1 : end - 1;
            return (last_index < HALF_SIZE) ? buffer1[last_index] : buffer2[last_index - HALF_SIZE];
        }

        int get_size() {
            return SIZE - size;
        }
    };
    struct final_state_information {
        bool advance = false;
        string class_type = "Meow";
    };

    //Tables
    std::unordered_map<int, std::unordered_map<char, int>> transition_table = {
        {0, {{'e', 1}, {'E', 1}, {'a', 1}, {'b', 1}, {'c', 1}, {'d', 1}, {'f', 1}, {'g', 1}, {'h', 1}, {'i', 1}, {'j', 1}, {'k', 1}, {'l', 1}, {'m', 1}, {'n', 1}, {'o', 1}, {'p', 1}, {'q', 1}, {'r', 1}, {'s', 1}, {'t', 1}, {'u', 1}, {'v', 1}, {'w', 1}, {'x', 1}, {'y', 1}, {'z', 1}, {'A', 1}, {'B', 1}, {'C', 1}, {'D', 1}, {'F', 1}, {'G', 1}, {'H', 1}, {'I', 1}, {'J', 1}, {'K', 1}, {'L', 1}, {'M', 1}, {'N', 1}, {'O', 1}, {'P', 1}, {'Q', 1}, {'R', 1}, {'S', 1}, {'T', 1}, {'U', 1}, {'V', 1}, {'W', 1}, {'X', 1}, {'Y', 1}, {'Z', 1}, {'_', 2}, {'0', 17}, {'1', 17}, {'2', 17}, {'3', 17}, {'4', 17}, {'5', 17}, {'6', 17}, {'7', 17}, {'8', 17}, {'9', 17}, {'{', 4}, {'}', 4}, {'[', 4}, {',', 4}, {']', 4}, {'(', 4}, {')', 4}, {':', 5}, {'=', 7}, {'!', 7}, {'>', 14}, {'<', 12}, {'&', 10}, {'|', 11}, {'+', 16}, {'-', 15}, {'%', 6}, {'*', 6}, {'/', 6}, {'.', 20}, {' ', 0}, {'\t', 0}, {'\r', 0}, {'\n', 0}, {'\0', 0}}},
        {1, {{'e', 1}, {'E', 1}, {'a', 1}, {'b', 1}, {'c', 1}, {'d', 1}, {'f', 1}, {'g', 1}, {'h', 1}, {'i', 1}, {'j', 1}, {'k', 1}, {'l', 1}, {'m', 1}, {'n', 1}, {'o', 1}, {'p', 1}, {'q', 1}, {'r', 1}, {'s', 1}, {'t', 1}, {'u', 1}, {'v', 1}, {'w', 1}, {'x', 1}, {'y', 1}, {'z', 1}, {'A', 1}, {'B', 1}, {'C', 1}, {'D', 1}, {'F', 1}, {'G', 1}, {'H', 1}, {'I', 1}, {'J', 1}, {'K', 1}, {'L', 1}, {'M', 1}, {'N', 1}, {'O', 1}, {'P', 1}, {'Q', 1}, {'R', 1}, {'S', 1}, {'T', 1}, {'U', 1}, {'V', 1}, {'W', 1}, {'X', 1}, {'Y', 1}, {'Z', 1}, {'_', 2}, {'0', 1}, {'1', 1}, {'2', 1}, {'3', 1}, {'4', 1}, {'5', 1}, {'6', 1}, {'7', 1}, {'8', 1}, {'9', 1}}},
        {2, {{'e', 2}, {'E', 2}, {'a', 2}, {'b', 2}, {'c', 2}, {'d', 2}, {'f', 2}, {'g', 2}, {'h', 2}, {'i', 2}, {'j', 2}, {'k', 2}, {'l', 2}, {'m', 2}, {'n', 2}, {'o', 2}, {'p', 2}, {'q', 2}, {'r', 2}, {'s', 2}, {'t', 2}, {'u', 2}, {'v', 2}, {'w', 2}, {'x', 2}, {'y', 2}, {'z', 2}, {'A', 2}, {'B', 2}, {'C', 2}, {'D', 2}, {'F', 2}, {'G', 2}, {'H', 2}, {'I', 2}, {'J', 2}, {'K', 2}, {'L', 2}, {'M', 2}, {'N', 2}, {'O', 2}, {'P', 2}, {'Q', 2}, {'R', 2}, {'S', 2}, {'T', 2}, {'U', 2}, {'V', 2}, {'W', 2}, {'X', 2}, {'Y', 2}, {'Z', 2}, {'_', 2}, {'0', 2}, {'1', 2}, {'2', 2}, {'3', 2}, {'4', 2}, {'5', 2}, {'6', 2}, {'7', 2}, {'8', 2}, {'9', 2}, {'{', 3}, {'}', 3}, {'[', 3}, {',', 3}, {']', 3}, {'(', 3}, {')', 3}, {':', 3}, {'=', 3}, {'!', 3}, {'>', 3}, {'<', 3}, {'&', 3}, {'|', 3}, {'+', 3}, {'-', 3}, {'%', 3}, {'*', 3}, {'/', 3}, {'.', 3}, {' ', 3}, {'\t', 3}, {'\r', 3}, {'\n', 3}, {'\0', 3}}},
        {5, {{':', 4}, {'=', 6}}},
        {7, {{'=', 6}}},
        {10, {{'&', 6}}},
        {11, {{'|', 6}}},
        {12, {{'e', 13}, {'E', 13}, {'a', 13}, {'b', 13}, {'c', 13}, {'d', 13}, {'f', 13}, {'g', 13}, {'h', 13}, {'i', 13}, {'j', 13}, {'k', 13}, {'l', 13}, {'m', 13}, {'n', 13}, {'o', 13}, {'p', 13}, {'q', 13}, {'r', 13}, {'s', 13}, {'t', 13}, {'u', 13}, {'v', 13}, {'w', 13}, {'x', 13}, {'y', 13}, {'z', 13}, {'A', 13}, {'B', 13}, {'C', 13}, {'D', 13}, {'F', 13}, {'G', 13}, {'H', 13}, {'I', 13}, {'J', 13}, {'K', 13}, {'L', 13}, {'M', 13}, {'N', 13}, {'O', 13}, {'P', 13}, {'Q', 13}, {'R', 13}, {'S', 13}, {'T', 13}, {'U', 13}, {'V', 13}, {'W', 13}, {'X', 13}, {'Y', 13}, {'Z', 13}, {'_', 13}, {'0', 13}, {'1', 13}, {'2', 13}, {'3', 13}, {'4', 13}, {'5', 13}, {'6', 13}, {'7', 13}, {'8', 13}, {'9', 13}, {'{', 13}, {'}', 13}, {'[', 13}, {',', 13}, {']', 13}, {'(', 13}, {')', 13}, {':', 13}, {'=', 6}, {'!', 13}, {'>', 6}, {'<', 6}, {'&', 13}, {'|', 13}, {'+', 13}, {'-', 13}, {'%', 13}, {'*', 13}, {'/', 13}, {'.', 13}, {' ', 13}, {'\t', 13}, {'\r', 13}, {'\n', 13}, {'\0', 13}}},
        {14, {{'e', 13}, {'E', 13}, {'a', 13}, {'b', 13}, {'c', 13}, {'d', 13}, {'f', 13}, {'g', 13}, {'h', 13}, {'i', 13}, {'j', 13}, {'k', 13}, {'l', 13}, {'m', 13}, {'n', 13}, {'o', 13}, {'p', 13}, {'q', 13}, {'r', 13}, {'s', 13}, {'t', 13}, {'u', 13}, {'v', 13}, {'w', 13}, {'x', 13}, {'y', 13}, {'z', 13}, {'A', 13}, {'B', 13}, {'C', 13}, {'D', 13}, {'F', 13}, {'G', 13}, {'H', 13}, {'I', 13}, {'J', 13}, {'K', 13}, {'L', 13}, {'M', 13}, {'N', 13}, {'O', 13}, {'P', 13}, {'Q', 13}, {'R', 13}, {'S', 13}, {'T', 13}, {'U', 13}, {'V', 13}, {'W', 13}, {'X', 13}, {'Y', 13}, {'Z', 13}, {'_', 13}, {'0', 13}, {'1', 13}, {'2', 13}, {'3', 13}, {'4', 13}, {'5', 13}, {'6', 13}, {'7', 13}, {'8', 13}, {'9', 13}, {'{', 13}, {'}', 13}, {'[', 13}, {',', 13}, {']', 13}, {'(', 13}, {')', 13}, {':', 13}, {'=', 6}, {'!', 13}, {'>', 6}, {'<', 13}, {'&', 13}, {'|', 13}, {'+', 13}, {'-', 13}, {'%', 13}, {'*', 13}, {'/', 13}, {'.', 13}, {' ', 13}, {'\t', 13}, {'\r', 13}, {'\n', 13}, {'\0', 13}}},
        {15, {{'e', 13}, {'E', 13}, {'a', 13}, {'b', 13}, {'c', 13}, {'d', 13}, {'f', 13}, {'g', 13}, {'h', 13}, {'i', 13}, {'j', 13}, {'k', 13}, {'l', 13}, {'m', 13}, {'n', 13}, {'o', 13}, {'p', 13}, {'q', 13}, {'r', 13}, {'s', 13}, {'t', 13}, {'u', 13}, {'v', 13}, {'w', 13}, {'x', 13}, {'y', 13}, {'z', 13}, {'A', 13}, {'B', 13}, {'C', 13}, {'D', 13}, {'F', 13}, {'G', 13}, {'H', 13}, {'I', 13}, {'J', 13}, {'K', 13}, {'L', 13}, {'M', 13}, {'N', 13}, {'O', 13}, {'P', 13}, {'Q', 13}, {'R', 13}, {'S', 13}, {'T', 13}, {'U', 13}, {'V', 13}, {'W', 13}, {'X', 13}, {'Y', 13}, {'Z', 13}, {'_', 13}, {'0', 17}, {'1', 17}, {'2', 17}, {'3', 17}, {'4', 17}, {'5', 17}, {'6', 17}, {'7', 17}, {'8', 17}, {'9', 17}, {'{', 13}, {'}', 13}, {'[', 13}, {',', 13}, {']', 13}, {'(', 13}, {')', 13}, {':', 13}, {'=', 13}, {'!', 13}, {'>', 13}, {'<', 13}, {'&', 13}, {'|', 13}, {'+', 13}, {'-', 13}, {'%', 13}, {'*', 13}, {'/', 13}, {'.', 20}, {' ', 13}, {'\t', 13}, {'\r', 13}, {'\n', 13}, {'\0', 13}}},
        {16, {{'e', 13}, {'E', 13}, {'a', 13}, {'b', 13}, {'c', 13}, {'d', 13}, {'f', 13}, {'g', 13}, {'h', 13}, {'i', 13}, {'j', 13}, {'k', 13}, {'l', 13}, {'m', 13}, {'n', 13}, {'o', 13}, {'p', 13}, {'q', 13}, {'r', 13}, {'s', 13}, {'t', 13}, {'u', 13}, {'v', 13}, {'w', 13}, {'x', 13}, {'y', 13}, {'z', 13}, {'A', 13}, {'B', 13}, {'C', 13}, {'D', 13}, {'F', 13}, {'G', 13}, {'H', 13}, {'I', 13}, {'J', 13}, {'K', 13}, {'L', 13}, {'M', 13}, {'N', 13}, {'O', 13}, {'P', 13}, {'Q', 13}, {'R', 13}, {'S', 13}, {'T', 13}, {'U', 13}, {'V', 13}, {'W', 13}, {'X', 13}, {'Y', 13}, {'Z', 13}, {'_', 13}, {'0', 17}, {'1', 17}, {'2', 17}, {'3', 17}, {'4', 17}, {'5', 17}, {'6', 17}, {'7', 17}, {'8', 17}, {'9', 17}, {'{', 13}, {'}', 13}, {'[', 13}, {',', 13}, {']', 13}, {'(', 13}, {')', 13}, {':', 13}, {'=', 6}, {'!', 13}, {'>', 13}, {'<', 13}, {'&', 13}, {'|', 13}, {'+', 6}, {'-', 13}, {'%', 13}, {'*', 13}, {'/', 13}, {'.', 20}, {' ', 13}, {'\t', 13}, {'\r', 13}, {'\n', 13}, {'\0', 13}}},
        {17, {{'e', 21}, {'E', 21}, {'a', 19}, {'b', 19}, {'c', 19}, {'d', 19}, {'f', 19}, {'g', 19}, {'h', 19}, {'i', 19}, {'j', 19}, {'k', 19}, {'l', 19}, {'m', 19}, {'n', 19}, {'o', 19}, {'p', 19}, {'q', 19}, {'r', 19}, {'s', 19}, {'t', 19}, {'u', 19}, {'v', 19}, {'w', 19}, {'x', 19}, {'y', 19}, {'z', 19}, {'A', 19}, {'B', 19}, {'C', 19}, {'D', 19}, {'F', 19}, {'G', 19}, {'H', 19}, {'I', 19}, {'J', 19}, {'K', 19}, {'L', 19}, {'M', 19}, {'N', 19}, {'O', 19}, {'P', 19}, {'Q', 19}, {'R', 19}, {'S', 19}, {'T', 19}, {'U', 19}, {'V', 19}, {'W', 19}, {'X', 19}, {'Y', 19}, {'Z', 19}, {'_', 19}, {'0', 17}, {'1', 17}, {'2', 17}, {'3', 17}, {'4', 17}, {'5', 17}, {'6', 17}, {'7', 17}, {'8', 17}, {'9', 17}, {'{', 19}, {'}', 19}, {'[', 19}, {',', 19}, {']', 19}, {'(', 19}, {')', 19}, {':', 19}, {'=', 19}, {'!', 19}, {'>', 19}, {'<', 19}, {'&', 19}, {'|', 19}, {'+', 19}, {'-', 19}, {'%', 19}, {'*', 19}, {'/', 19}, {'.', 18}, {' ', 19}, {'\t', 19}, {'\r', 19}, {'\n', 19}, {'\0', 19}}},
        {18, {{'e', 21}, {'E', 21}, {'a', 19}, {'b', 19}, {'c', 19}, {'d', 19}, {'f', 19}, {'g', 19}, {'h', 19}, {'i', 19}, {'j', 19}, {'k', 19}, {'l', 19}, {'m', 19}, {'n', 19}, {'o', 19}, {'p', 19}, {'q', 19}, {'r', 19}, {'s', 19}, {'t', 19}, {'u', 19}, {'v', 19}, {'w', 19}, {'x', 19}, {'y', 19}, {'z', 19}, {'A', 19}, {'B', 19}, {'C', 19}, {'D', 19}, {'F', 19}, {'G', 19}, {'H', 19}, {'I', 19}, {'J', 19}, {'K', 19}, {'L', 19}, {'M', 19}, {'N', 19}, {'O', 19}, {'P', 19}, {'Q', 19}, {'R', 19}, {'S', 19}, {'T', 19}, {'U', 19}, {'V', 19}, {'W', 19}, {'X', 19}, {'Y', 19}, {'Z', 19}, {'_', 19}, {'0', 18}, {'1', 18}, {'2', 18}, {'3', 18}, {'4', 18}, {'5', 18}, {'6', 18}, {'7', 18}, {'8', 18}, {'9', 18}, {'{', 19}, {'}', 19}, {'[', 19}, {',', 19}, {']', 19}, {'(', 19}, {')', 19}, {':', 19}, {'=', 19}, {'!', 19}, {'>', 19}, {'<', 19}, {'&', 19}, {'|', 19}, {'+', 19}, {'-', 19}, {'%', 19}, {'*', 19}, {'/', 19}, {'.', 19}, {' ', 19}, {'\t', 19}, {'\r', 19}, {'\n', 19}, {'\0', 19}}},
        {20, {{'0', 18}, {'1', 18}, {'2', 18}, {'3', 18}, {'4', 18}, {'5', 18}, {'6', 18}, {'7', 18}, {'8', 18}, {'9', 18}}},
        {21, {{'0', 22}, {'1', 22}, {'2', 22}, {'3', 22}, {'4', 22}, {'5', 22}, {'6', 22}, {'7', 22}, {'8', 22}, {'9', 22}, {'+', 23}, {'-', 23}}},
        {22, {{'e', 19}, {'E', 19}, {'a', 19}, {'b', 19}, {'c', 19}, {'d', 19}, {'f', 19}, {'g', 19}, {'h', 19}, {'i', 19}, {'j', 19}, {'k', 19}, {'l', 19}, {'m', 19}, {'n', 19}, {'o', 19}, {'p', 19}, {'q', 19}, {'r', 19}, {'s', 19}, {'t', 19}, {'u', 19}, {'v', 19}, {'w', 19}, {'x', 19}, {'y', 19}, {'z', 19}, {'A', 19}, {'B', 19}, {'C', 19}, {'D', 19}, {'F', 19}, {'G', 19}, {'H', 19}, {'I', 19}, {'J', 19}, {'K', 19}, {'L', 19}, {'M', 19}, {'N', 19}, {'O', 19}, {'P', 19}, {'Q', 19}, {'R', 19}, {'S', 19}, {'T', 19}, {'U', 19}, {'V', 19}, {'W', 19}, {'X', 19}, {'Y', 19}, {'Z', 19}, {'_', 19}, {'0', 22}, {'1', 22}, {'2', 22}, {'3', 22}, {'4', 22}, {'5', 22}, {'6', 22}, {'7', 22}, {'8', 22}, {'9', 22}, {'{', 19}, {'}', 19}, {'[', 19}, {',', 19}, {']', 19}, {'(', 19}, {')', 19}, {':', 19}, {'=', 19}, {'!', 19}, {'>', 19}, {'<', 19}, {'&', 19}, {'|', 19}, {'+', 19}, {'-', 19}, {'%', 19}, {'*', 19}, {'/', 19}, {'.', 19}, {' ', 19}, {'\t', 19}, {'\r', 19}, {'\n', 19}, {'\0', 19}}},
        {23, {{'0', 22}, {'1', 22}, {'2', 22}, {'3', 22}, {'4', 22}, {'5', 22}, {'6', 22}, {'7', 22}, {'8', 22}, {'9', 22}}},
        {24, {{'e', -1}, {'E', -1}, {'a', -1}, {'b', -1}, {'c', -1}, {'d', -1}, {'f', -1}, {'g', -1}, {'h', -1}, {'i', -1}, {'j', -1}, {'k', -1}, {'l', -1}, {'m', -1}, {'n', -1}, {'o', -1}, {'p', -1}, {'q', -1}, {'r', -1}, {'s', -1}, {'t', -1}, {'u', -1}, {'v', -1}, {'w', -1}, {'x', -1}, {'y', -1}, {'z', -1}, {'A', -1}, {'B', -1}, {'C', -1}, {'D', -1}, {'F', -1}, {'G', -1}, {'H', -1}, {'I', -1}, {'J', -1}, {'K', -1}, {'L', -1}, {'M', -1}, {'N', -1}, {'O', -1}, {'P', -1}, {'Q', -1}, {'R', -1}, {'S', -1}, {'T', -1}, {'U', -1}, {'V', -1}, {'W', -1}, {'X', -1}, {'Y', -1}, {'Z', -1}, {'_', -1}, {'0', -1}, {'1', -1}, {'2', -1}, {'3', -1}, {'4', -1}, {'5', -1}, {'6', -1}, {'7', -1}, {'8', -1}, {'9', -1}, {'{', -1}, {'}', -1}, {'[', -1}, {',', -1}, {']', -1}, {'(', -1}, {')', -1}, {':', -1}, {'=', -1}, {'!', -1}, {'>', -1}, {'<', -1}, {'&', -1}, {'|', -1}, {'+', -1}, {'-', -1}, {'%', -1}, {'*', -1}, {'/', -1}, {'.', -1}, {' ', -1}, {'\t', -1}, {'\r', -1}, {'\n', -1}, {'\0', -1}}},
    };
    std::unordered_map<int, final_state_information> final_states = {
        {3,{false,"id"}},
        {4,{true,"pun"}},
        {6,{true,"op"}},
        {13,{false,"op"}},
        {19,{false,"num"}}
    };


    int current_index{};
    int id_index{ 1 };
    int literal_index{ 1 };

    //Files
    ifstream input_file{};
    ofstream symbol_table_file{};
    ofstream tokens_file{};
    ofstream errors_file{};
    ofstream literal_table_file{};

    //Input Buffer
    static const int BUFFER_SIZE = 25;
    meow_queue<char, BUFFER_SIZE * 2> input_buffer;
    int lexeme_begin{};
    int lexeme_end{};

    //lexeme handler
    ostringstream lexeme{};

protected:
    char getNextCharacter()
    {
        if (input_buffer.empty()) return '\0'; //eof
        return input_buffer.front();
    }
    char getNextNextCharacter()
    {
        if (input_buffer.get_size() < 2) return '\0'; //eof
        return input_buffer.peek_next_front();
    }
    void moveToNextCharacter()
    {
        if (!input_buffer.empty())
        {
            input_buffer.pop();
            current_index++;
            if (current_index >= BUFFER_SIZE) fill_input_buffer();
        }
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
        // Close file streams if open
        if (input_file.is_open()) input_file.close();
        if (symbol_table_file.is_open()) symbol_table_file.close();
        if (tokens_file.is_open()) tokens_file.close();
        if (errors_file.is_open()) errors_file.close();
        if (literal_table_file.is_open()) literal_table_file.close();
    }
    void clearLexeme()
    {
        // Clear ostringstream for reuse
        lexeme.str("");   // Reset the string content
        lexeme.clear();   // Clear any error flags
        lexeme.seekp(0);  // Reset the write position (optional)
    }
    void commentRemover(char& ch)
    {
        if (ch == '/' && getNextNextCharacter() == '/')
        {
            while (!input_buffer.empty() && getNextCharacter() != '\n')
            {
                moveToNextCharacter();
            }
            if (getNextCharacter() == '\n')
            {
                cout << "\n";
                tokens_file << "\n";
                errors_file << "\n";
            }
            moveToNextCharacter();
            ch = getNextCharacter();
        }
        else if (ch == '/' && getNextNextCharacter() == '*')
        {
            moveToNextCharacter();
            moveToNextCharacter();
            while (!input_buffer.empty() && !(getNextCharacter() == '*' && getNextNextCharacter() == '/'))
            {
                moveToNextCharacter();
            }
            moveToNextCharacter();
            moveToNextCharacter();
            ch = getNextCharacter();
        }
    }
    void literalMaker(char& ch)
    {
        moveToNextCharacter();
        ch = getNextCharacter();
        while (!input_buffer.empty() && ch != '\"' && ch != '\n')
        {
            lexeme.put(ch);
            moveToNextCharacter();
            ch = getNextCharacter();
        }
        string complete_lexeme = lexeme.str();
        if (ch == '\"')
        {
            cout << "<literal," + complete_lexeme + ">";
            if (literal_table.find(complete_lexeme) == literal_table.end())
            {
                literal_table[complete_lexeme] = literal_index;
                complete_lexeme = to_string(literal_index++);
            }
            else
                complete_lexeme = to_string(literal_table[complete_lexeme]);

            string token = "<literal," + complete_lexeme + ">";
            tokens_file << token << '\n'; //\n added for parser
        }
        else if (ch == '\n')
        {
            cout << "<Error,\"" + complete_lexeme + ">";
            string token = "<Error,\"" + complete_lexeme + ">";
            errors_file << token;

            cout << "\n";
            tokens_file << "\n";
            errors_file << "\n";
        }
        else
            cout << "<This wasn't suppose to happen>";
        clearLexeme();
        moveToNextCharacter();
        ch = getNextCharacter();
    }
    void finalStateHandler(int& state, char& ch)
    {
        final_state_information final_state = final_states[state];
        if (final_state.advance)
        {
            moveToNextCharacter();
            if (!isspace(ch))
                lexeme.put(ch);
        }
        string complete_lexeme = lexeme.str();
        if (final_state.class_type == "id")
        {
            if (keywords_table.find(complete_lexeme) != keywords_table.end())
            {
                complete_lexeme = to_string(keywords_table[complete_lexeme]);
                final_state.class_type = "keyword";
            }
            if (symbol_table.find(complete_lexeme) == symbol_table.end())
            {
                symbol_table[complete_lexeme] = id_index;
                complete_lexeme = to_string(id_index++);
            }
            else
            {
                complete_lexeme = to_string(symbol_table[complete_lexeme]);
            }
        }
        string token = "<" + final_state.class_type + "," + complete_lexeme + ">";
        cout << token << " ";
        tokens_file << token << '\n'; //\n added for parser
        clearLexeme();
    }
    void analyzer()
    {
        bool flag = true;
        int state = 0;
        while (flag)
        {
            flag = !input_buffer.empty();
            char ch = getNextCharacter();
            //cout << "(" << state << "," << ch << ")" << "-->";

            if (ch == '\n')
            {
                cout << "\n";
                tokens_file << "\n";
                errors_file << "\n";
            }
            if (state == 0)
            {
                commentRemover(ch);
                if (ch == '\"')
                    literalMaker(ch);
            }

            //if (transition_table[24].find(ch) == transition_table[24].end()) //24 is state that have all valid language characters mapped to somewhere
            //{
            //    //lexical error
            //    string token =  ""+ ch;
            //    token = "<Error,"+token+ ">";
            //    cout << token;
            //    errors_file << token;
            //    ch = ' ';
            //}
            if (transition_table.find(state) != transition_table.end() && transition_table[state].find(ch) != transition_table[state].end())
            {
                //cout << "---" << state << "," << ch;
                state = transition_table[state][ch];
            }
            else
            {
                //cout << "\nTransition not found\n";
                if (state == 0)
                {
                    moveToNextCharacter();
                    if (!isspace(ch))
                        lexeme.put(ch);
                }



                string complete_lexeme = lexeme.str();
                //cout << "\n Complete Lexeme " << complete_lexeme << "\n";
                if (keywords_table.find(complete_lexeme) != keywords_table.end())
                {
                    complete_lexeme = to_string(keywords_table[complete_lexeme]);
                    tokens_file << ("<keyword," + complete_lexeme + ">") << '\n';//\n added for parser
                    cout << "<Keyword," + complete_lexeme + ">";
                }
                else if ((complete_lexeme == "input" && getNextCharacter() == '-' && getNextNextCharacter() == '>') || (complete_lexeme == "output" && getNextCharacter() == '<' && getNextNextCharacter() == '-'))
                {
                    complete_lexeme = complete_lexeme + getNextCharacter() + "" + getNextNextCharacter();
                    moveToNextCharacter();
                    moveToNextCharacter();
                    cout << "<Keyword," + complete_lexeme + ">";
                    complete_lexeme = to_string(keywords_table[complete_lexeme]);
                    tokens_file << ("<keyword," + complete_lexeme + ">") << '/n';//\n added for parser
                }
                else if (transition_table[state].find(' ') != transition_table[state].end() && final_states.find(transition_table[state][' ']) != final_states.end())
                {
                    finalStateHandler(transition_table[state][' '], ch);
                    moveToNextCharacter();
                    lexeme.put(ch);
                    string lexical_error_token = "<Error," + lexeme.str() + ">";
                    cout << lexical_error_token;
                    errors_file << lexical_error_token;
                    state = 0;
                }
                else
                {
                    string error_token = "<Error," + complete_lexeme + ">";
                    cout << error_token;
                    errors_file << error_token;
                }

                clearLexeme();
                state = 0; //error

                continue;
            }
            //if(! (final_states.find(state) != final_states.end() && !final_states[state].advance))
            //if(final_states.find(state) == final_states.end() || final_states[state].advance)
            if (final_states.find(state) != final_states.end())
            {
                finalStateHandler(state, ch);
                state = 0;
            }
            else
            {
                moveToNextCharacter();
                if (!isspace(ch))
                    lexeme.put(ch);
            }
        }
        //cout << state;
    }
    void writeTables()
    {
        for (auto entry : symbol_table)
            symbol_table_file << (entry.first + " " + to_string(entry.second) + "\n");

        for (auto entry : literal_table)
            literal_table_file << (entry.first + " " + to_string(entry.second) + "\n");
    }
    void cleanup() {
        //Tables
        transition_table.clear();
        final_states.clear();
        /*symbol_table.clear();
        keywords_table.clear();
        literal_table.clear()*/

        // Close file streams if open
        close_files();

        // Reset indices and streams
        lexeme_begin = lexeme_end = current_index = 0;
        clearLexeme();

        // Clear input buffer
        while (!input_buffer.empty()) input_buffer.pop();

        //std::cout << "\nMemory and resources freed successfully.\n" << std::endl;
    }

public:
    LexicalAnalyzer(string input_file_name = "input.txt", string symbol_table_file_name = "symbol_table.txt", string token_file_name = "tokens.txt", string errors_file_name = "errors.txt", string literal_table_file_name = "literal_table.txt") :lexeme_begin(0), lexeme_end(0), input_file(input_file_name), symbol_table_file(symbol_table_file_name), tokens_file(token_file_name), errors_file(errors_file_name), literal_table_file(literal_table_file_name), input_buffer{}, current_index(0), lexeme(), id_index(1), literal_index(1)
    {
        if (!input_file) {
            cerr << "Input File not found!\n Check if the file is within the project.\n";
            close_files();
            exit(1);
        }
        if (!symbol_table_file) {
            cerr << "Access Denied!\n Symbol Table's file is not created.\n";
            close_files();
            exit(1);
        }
        if (!tokens_file) {
            cerr << "Access Denied!\n Tokens file is not created.\n";
            close_files();
            exit(1);
        }
        if (!errors_file) {
            cerr << "Access Denied!\n errors file is not created.\n";
            close_files();
            exit(1);
        }
        fill_input_buffer();
        analyzer();
        writeTables();
        //cout << "\nEnd of Lexical Phase\n";        
    }
    ~LexicalAnalyzer()
    {
        cleanup();
        cout << "\nLexical Analysis Complete\n";
    }
};




//-----------------------------------------------------------------------Parser
struct Token {
    string type;
    string val;
};

class TokenParser {
    bool next_token;
    std::ifstream token_file;
    Token curr_token;
    int line_number = 0;

public:
    TokenParser() : next_token(true), token_file("tokens.txt"), curr_token({ "", "" }) {
        if (!token_file.is_open()) {
            std::cerr << "Error: Cannot open tokens.txt for Syntax Analyzing Phase.\n";
            exit(1);
        }
    }
    Token parseToken(const string& str) {
        Token token;

        // Find the positions of the first '<' and last '>'
        size_t left = str.find('<');
        size_t right = str.rfind('>');

        if (left == string::npos || right == string::npos || left >= right) {
            token.type = "INVALID";
            token.val = "INVALID";
            return token;
        }

        // Extract the inside of the brackets
        string inside = str.substr(left + 1, right - left - 1);

        // Now split by the first comma
        size_t comma = inside.find(',');

        if (comma == string::npos) {
            token.type = "INVALID";
            token.val = "INVALID";
            return token;
        }

        token.type = inside.substr(0, comma);
        token.val = inside.substr(comma + 1);

        // Trim spaces from type and val
        auto trim = [](string& s) {
            size_t start = s.find_first_not_of(" \t\r\n");
            size_t end = s.find_last_not_of(" \t\r\n");
            if (start != string::npos && end != string::npos)
                s = s.substr(start, end - start + 1);
            else if (start != string::npos)
                s = s.substr(start);
            else
                s.clear();
            };

        trim(token.type);
        trim(token.val);

        return token;
    }

    Token getToken() {
        if (!next_token) return curr_token;

        next_token = false;
        curr_token = { "", "" };
        std::string line;

        while (std::getline(token_file, line)) {
            line_number++;

            // Remove leading and trailing whitespace
            size_t start = line.find_first_not_of(" \t\r\n");
            size_t end = line.find_last_not_of(" \t\r\n");

            if (start == std::string::npos) {
                continue; // Skip empty lines
            }

            line = line.substr(start, end - start + 1);
            curr_token = parseToken(line);

            if (curr_token.type == "INVALID" && curr_token.val == "INVALID") {
                continue; // Skip invalid token lines
            }

            break; // Successfully parsed a valid token
        }

        return curr_token;
    }

    void advance() {
        next_token = true;
    }

    bool areTokenLeft() {
        return token_file.peek() != EOF;
    }

    ~TokenParser() {
        token_file.close();
    }
};



class Parser {
private:
    TokenParser tokens;

    // Node structure for the parse tree
    struct ParseNode {
        string name;
        vector<ParseNode*> children;
        string tokenValue; // For leaf nodes (tokens)
        bool isToken;

        ParseNode(const string& nodeName, bool token = false, const string& value = "")
            : name(nodeName), isToken(token), tokenValue(value) {}

        ~ParseNode() {
            for (auto child : children) {
                delete child;
            }
        }
    };

    ParseNode* root = nullptr;
    stack<ParseNode*> nodeStack;

    ParseNode* createNode(const string& nodeName) {
        ParseNode* node = new ParseNode(nodeName);

        // If we have a parent node on the stack, add this as a child
        if (!nodeStack.empty()) {
            nodeStack.top()->children.push_back(node);
        }
        else {
            // This is the root node
            root = node;
        }

        return node;
    }

    void addTokenNode(const string& tokenType, const string& tokenValue) {
        if (!nodeStack.empty()) {
            ParseNode* tokenNode = new ParseNode(tokenType, true, tokenValue);
            nodeStack.top()->children.push_back(tokenNode);
        }
    }

    bool match(string expectedType, string given) {
        if (expectedType == given) {
            // Add token to parse tree
            addTokenNode(expectedType, tokens.getToken().val);
            tokens.advance();
            return true;
        }
        return false;
    }

    // Print the parse tree with nice formatting
    void printTree(ParseNode* node, string prefix = "", bool isLast = true) {
        if (node == nullptr) return;

        // Print current node
        cout << prefix;
        std::cout << (isLast ? "`-- " : "|-- ");


        if (node->isToken) {
            cout << node->name << " : " << node->tokenValue << endl;
        }
        else {
            cout << node->name << endl;
        }

        // Print children with appropriate prefixes
        prefix += isLast ? "    " : "|   ";

        for (size_t i = 0; i < node->children.size(); ++i) {
            printTree(node->children[i], prefix, i == node->children.size() - 1);
        }
    }

public:
    Parser() {}

    ~Parser() {
        delete root; // Clean up the entire tree
    }

    bool parse() {
        bool result = tokens.areTokenLeft() && Function();

        // // After parsing, print the parse tree
        // if (result) {
        //     cout << "\n===== Parse Tree =====\n";
        //     printTree(root);
        // }

        printTree(root);

        return result;
    }

private:
    bool Function() {
        ParseNode* node = createNode("Function");
        nodeStack.push(node);

        bool result = Type() && match("id", tokens.getToken().type) &&
            match("(", tokens.getToken().val) &&
            ArgList() &&
            match(")", tokens.getToken().val) &&
            CompStmt();

        nodeStack.pop();
        return result;
    }

    bool ArgList() {
        ParseNode* node = createNode("ArgList");
        nodeStack.push(node);

        bool result = Arg() && ArgListPrime();

        nodeStack.pop();
        return result;
    }

    bool ArgListPrime() {
        ParseNode* node = createNode("ArgListPrime");
        nodeStack.push(node);

        bool result = true;
        if (match(",", tokens.getToken().val)) {
            result = Arg() && ArgListPrime();
        }

        nodeStack.pop();
        return result;
    }

    bool Arg() {
        ParseNode* node = createNode("Arg");
        nodeStack.push(node);

        bool result = Type() && match("id", tokens.getToken().type);

        nodeStack.pop();
        return result;
    }

    bool Type() {


        string t = rkeywords_map[tokens.getToken().val];
        bool result = (t == "Adadi" || t == "Ashriya" || t == "Harf" || t == "Matn" || t == "Mantiqi");

        if (result) {
            ParseNode* node = createNode("Type");
            nodeStack.push(node);
            addTokenNode("keyword", t);
            tokens.advance();
        }

        nodeStack.pop();
        return result;
    }

    bool Declaration() {
        ParseNode* node = createNode("Declaration");
        nodeStack.push(node);

        bool result = Type() && IdentList() && match("::", tokens.getToken().val);

        nodeStack.pop();
        return result;
    }

    bool IdentList() {
        ParseNode* node = createNode("IdentList");
        nodeStack.push(node);

        bool result = match("id", tokens.getToken().type) && IdentListPrime();

        nodeStack.pop();
        return result;
    }

    bool IdentListPrime() {
        ParseNode* node = createNode("IdentListPrime");
        nodeStack.push(node);

        bool result = true;
        if (match(",", tokens.getToken().val)) {
            result = IdentList();
        }

        nodeStack.pop();
        return result;
    }

    bool stmt() {
        ParseNode* node = createNode("stmt");
        nodeStack.push(node);

        bool result = false;
        if (match("for", rkeywords_map[tokens.getToken().val]))
            result = ForStmt();
        else if (match("While", rkeywords_map[tokens.getToken().val]))
            result = WhileStmt();
        else if (match("id", tokens.getToken().type))
            result = HaHa();
        else if (match("(", tokens.getToken().val))
            result = Expr() && match(")", tokens.getToken().val) && HeHe();
        else if (match("num", tokens.getToken().type))
            result = HeHe();
        else if (match("Agar", rkeywords_map[tokens.getToken().val]))
            result = IfStmt();
        else if (match("{", tokens.getToken().val))
            result = CompStmt() && stmtList();
        else
            result = (match("::", tokens.getToken().val) || Declaration());



        nodeStack.pop();
        return result;
    }
    bool CompStmtPrime() {
        ParseNode* node = createNode("CompStmt");
        nodeStack.push(node);

        bool result = stmtList() &&
            match("}", tokens.getToken().val);

        nodeStack.pop();
        return result;
    }
    bool ForStmt() {
        ParseNode* node = createNode("ForStmt");
        nodeStack.push(node);

        bool result = match("(", tokens.getToken().val) &&
            Expr() &&
            match("::", tokens.getToken().val) &&
            OptExpr() &&
            match("::", tokens.getToken().val) &&
            OptExpr() &&
            match(")", tokens.getToken().val) &&
            stmt();

        nodeStack.pop();
        return result;
    }

    bool OptExpr() {
        ParseNode* node = createNode("OptExpr");
        nodeStack.push(node);

        bool result = true;
        if (match("id", tokens.getToken().type))
            result = HaHa();
        else if (match("(", tokens.getToken().val))
            result = Expr() && match(")", tokens.getToken().val) && HeHe();
        else if (match("num", tokens.getToken().type))
            result = HeHe();

        nodeStack.pop();
        return result;
    }

    bool WhileStmt() {
        ParseNode* node = createNode("WhileStmt");
        nodeStack.push(node);

        bool result = match("(", tokens.getToken().val) &&
            Expr() &&
            match(")", tokens.getToken().val) &&
            stmt();

        nodeStack.pop();
        return result;
    }

    bool IfStmt() {
        ParseNode* node = createNode("IfStmt");
        nodeStack.push(node);

        bool result = match("(", tokens.getToken().val) &&
            Expr() &&
            match(")", tokens.getToken().val) &&
            stmt() &&
            ElsePart();

        nodeStack.pop();
        return result;
    }

    bool ElsePart() {
        ParseNode* node = createNode("ElsePart");
        nodeStack.push(node);

        bool result = true;
        if (match("Wagarna", rkeywords_map[tokens.getToken().val])) {
            result = stmt();
        }

        nodeStack.pop();
        return result;
    }

    bool CompStmt() {
        ParseNode* node = createNode("CompStmt");
        nodeStack.push(node);

        bool result = match("{", tokens.getToken().val) &&
            stmtList() &&
            match("}", tokens.getToken().val);

        nodeStack.pop();
        return result;
    }

    bool stmtList() {
        ParseNode* node = createNode("stmtList");
        nodeStack.push(node);

        bool result = true;
        if (match("for", rkeywords_map[tokens.getToken().val]))
            result = ForStmt() && stmtList();
        else if (match("While", rkeywords_map[tokens.getToken().val]))
            result = WhileStmt() && stmtList();
        else if (match("id", tokens.getToken().type))
            result = HaHa() && stmtList();
        else if (match("(", tokens.getToken().val))
            result = Expr() && match("::", tokens.getToken().val) && match(")", tokens.getToken().val) && HeHe() && stmtList();
        else if (match("num", tokens.getToken().type))
            result = HeHe() && stmtList();
        else if (match("Agar", rkeywords_map[tokens.getToken().val]))
            result = IfStmt() && stmtList();
        else if (match("{", tokens.getToken().val))
            result = CompStmt() && stmtList();
        else if (match("::", tokens.getToken().val))
        {
            result = stmtList();
        }
        else if (Type())
        {
            result = IdentList() && match("::", tokens.getToken().val) && stmtList();
        }


        nodeStack.pop();
        return result;
    }

    bool Expr() {
        ParseNode* node = createNode("Expr");
        nodeStack.push(node);

        bool result = false;
        if (match("id", tokens.getToken().type))
            result = HaHa();
        else if (match("(", tokens.getToken().val))
            result = Expr() && match(")", tokens.getToken().val) && HeHe();
        else if (match("num", tokens.getToken().type))
            result = HeHe();

        nodeStack.pop();
        return result;
    }

    bool HaHa() {
        ParseNode* node = createNode("HaHa");
        nodeStack.push(node);

        bool result = false;
        if (match(":=", tokens.getToken().val))
            result = Expr();
        else
            result = HeHe();

        nodeStack.pop();
        return result;
    }

    bool HeHe() {
        ParseNode* node = createNode("HeHe");
        nodeStack.push(node);

        bool result = TermPrimePrime() && MagPrimePrime() && RvaluePrime();

        nodeStack.pop();
        return result;
    }

    bool RvaluePrime() {
        ParseNode* node = createNode("RvaluePrime");
        nodeStack.push(node);

        bool result = true;
        if (Compare()) result = Mag() && RvaluePrime();

        nodeStack.pop();
        return result;
    }

    bool Compare() {


        string t = tokens.getToken().val;
        bool result = (t == "==" || t == "<" || t == ">" || t == "<=" || t == ">=" || t == "!=" || t == "<>");

        if (result) {
            ParseNode* node = createNode("Compare");
            nodeStack.push(node);
            addTokenNode("operator", t);
            tokens.advance();
        }

        nodeStack.pop();
        return result;
    }

    bool Mag() {
        ParseNode* node = createNode("Mag");
        nodeStack.push(node);
        bool result = false;
        if (match("id", tokens.getToken().type))
            result = TermPrimePrime() && MagPrimePrime();
        else if (match("(", tokens.getToken().val))
            result = Expr() && match(")", tokens.getToken().val) && TermPrimePrime() && MagPrimePrime();
        else if (match("num", tokens.getToken().type))
            result = TermPrimePrime() && MagPrimePrime();
        nodeStack.pop();
        return result;
    }

    bool MagPrime() {
        ParseNode* node = createNode("MagPrime");
        nodeStack.push(node);

        bool result = false;
        if (match("+", tokens.getToken().val))
            result = Term();
        else if (match("-", tokens.getToken().val))
            result = Term();

        nodeStack.pop();
        return result;
    }

    bool MagPrimePrime() {
        ParseNode* node = createNode("MagPrimePrime");
        nodeStack.push(node);

        bool result = true;
        if (match("+", tokens.getToken().val))
            result = Term() && MagPrimePrime();
        else if (match("-", tokens.getToken().val))
            result = Term() && MagPrimePrime();

        nodeStack.pop();
        return result;
    }

    bool Term() {
        ParseNode* node = createNode("Term");
        nodeStack.push(node);

        bool result = false;
        if (match("id", tokens.getToken().type))
            result = TermPrimePrime();
        else if (match("(", tokens.getToken().val))
            result = Expr() && match(")", tokens.getToken().val) && TermPrimePrime();
        else if (match("num", tokens.getToken().type))
            result = TermPrimePrime();

        nodeStack.pop();
        return result;
    }

    bool TermPrime() {
        ParseNode* node = createNode("TermPrime");
        nodeStack.push(node);

        bool result = false;
        if (match("*", tokens.getToken().val))
            result = Factor();
        else if (match("/", tokens.getToken().val))
            result = Factor();

        nodeStack.pop();
        return result;
    }

    bool TermPrimePrime() {
        ParseNode* node = createNode("TermPrimePrime");
        nodeStack.push(node);

        bool result = true;
        if (match("*", tokens.getToken().val))
        {
            result = Factor() && TermPrimePrime();
        }
        else if (match("/", tokens.getToken().val))  // else if(result = match("/", tokens.getToken().val)) traitorrrrrrrrrrrrrrrr
        {
            result = Factor() && TermPrimePrime();
        }

        nodeStack.pop();
        return result;
    }

    bool Factor() {
        ParseNode* node = createNode("Factor");
        nodeStack.push(node);

        bool result = match("id", tokens.getToken().type) ||
            match("num", tokens.getToken().type) ||
            (match("(", tokens.getToken().val) && Expr() && match(")", tokens.getToken().val));
        nodeStack.pop();
        return result;
    }
};

// Main function
int main() {
    Parser parser;
    if (parser.parse()) {
        cout << "Accepted\n";
    }
    else {
        cout << "Rejected\n";
    }

    return 0;
}


//int main() {
//    
//    cout << "Analyzing Source Code...\n";
//    LexicalAnalyzer* sad = new LexicalAnalyzer("input.txt");
//    delete sad;
//
//    TokenParser parser;
//
//    while (parser.areTokenLeft()) {
//        Token t = parser.getToken();
//        cout << "Type: " << t.type << ", Value: " << t.val << endl;
//        parser.advance();
//    }
//
//    
//    return 0;
//}