import csv
import re
from collections import defaultdict

def parse_char_set(char_set):
    """Expands a character set like '{eE}' into a list of characters ['e', 'E']."""  
    match = re.match(r"\{(.*)\}", char_set)
    if match:
        return list(match.group(1)) 
    elif char_set == "blank_space":
        return [" ", "\\t", "\\r", "\\n", "\\0"]  # Includes all types of blank spaces     #return [" "] 
    else:
        return [char_set]  

def read_transition_table(filename):
    """Reads the CSV file and maps it to transition_table[i][j], ignoring blank cells."""
    transition_table = defaultdict(dict)

    with open(filename, newline='', encoding='utf-8') as csvfile:
        reader = csv.reader(csvfile)
        columns = next(reader)[1:]  # Extract column headers, ignoring the first empty cell

        # Expand column headers into individual characters
        column_keys = [parse_char_set(col) for col in columns]

        for row in reader:
            state = row[0]  # The first column is the state (i)
            transitions = row[1:]  # Remaining are transition values

            for key_set, transition in zip(column_keys, transitions):
                if transition.strip():  # Ignore blank cells
                    for key in key_set:  
                        transition_table[state][key] = int(transition)

    return transition_table

def print_cpp_map(transition_table):
    print("std::unordered_map<int, std::unordered_map<char, int>> transition_table = {")
    
    for state, transitions in transition_table.items():
        transitions_str = ", ".join(f"{{'{char}', {val}}}" for char, val in transitions.items())
        print(f"    {{{state}, {{{transitions_str}}}}},")
    
    print("};")

# Usage
filename = "MeowMeow_Transitions.csv"  # Replace with your actual CSV file path
transition_table = read_transition_table(filename)

# # Example output:
# for state, transitions in transition_table.items():
#     print(f"State {state}: {transitions}")

print_cpp_map(transition_table)




def sort_and_write_to_file(arr, filename):
    """Sorts an array and writes each element line by line to a file."""
    sorted_arr = sorted(arr)  # Sort the array

    with open(filename, 'w', encoding='utf-8') as file:
        for item in sorted_arr:
            file.write(f"{item}\n")  # Write each item on a new line

keywords = [
    "asm", "Wagarna", "new", "this", "auto", "enum", "operator", "throw", "Mantiqi", "explicit", "private",
    "True", "break", "export", "protected", "try", "case", "extern", "public", "typedef", "catch", "False", "register", "typeid",
    "Harf", "Ashriya", "typename", "Adadi", "class", "for", "Wapas", "union", "const", "dost", "short", "unsigned", "goto",
    "signed", "using", "continue", "Agar", "sizeof", "virtual", "default", "inline", "static", "Khali", "delete", "volatile", "do",
    "long", "struct", "double", "mutable", "switch", "while", "namespace", "template", "Marqazi", "Matn", "input->",
    "output<-"
]
sort_and_write_to_file(keywords, "keywords.txt")


def read_file_to_hashmap(filename):
    """Reads lines from a file, strips them, and stores them in a hashmap with line numbers."""
    hashmap = {}
    
    with open(filename, 'r', encoding='utf-8') as file:
        for line_number, line in enumerate(file, start=1):
            stripped_line = line.strip()  # Remove leading/trailing spaces
            hashmap[line_number] = stripped_line  # Store in hashmap
    
    return hashmap

def print_cpp_kmap(hashmap):
    """Prints the hashmap in C++ unordered_map<int, std::string> format."""
    print("std::unordereds_table<std::string,int> keyword_map = {")
    
    for key, value in hashmap.items():
        print(f'    {{"{value}", {key}}},')
    
    print("};")

# Usage
filename = "keywords.txt"  # Replace with your actual filename
hashmap = read_file_to_hashmap(filename)

# Print in C++ unordered_map format
print_cpp_kmap(hashmap)
