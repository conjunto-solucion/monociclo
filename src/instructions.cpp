#include <string>
#include <sstream>
#include <cctype>
#include <algorithm>

#include "./instructions.h"

using namespace std;

const string OPERATOR_0 = "not";
const string OPERATOR_1 = "+";
const string OPERATOR_2 = "-";
const string OPERATOR_3 = "or";
const string OPERATOR_4 = "and";
const string OPERATOR_5 = "=";
const string OPERATOR_6 = ">";
const string OPERATOR_7 = ">=";
const string OPERATOR_8 = "+i";
const string OPERATOR_9 = "read";
const string OPERATOR_10 = "write";
const string OPERATOR_11 = "readi";
const string OPERATOR_12 = "writei";

const string PSEUDO_OPERATOR_0 = "not";
const string PSEUDO_OPERATOR_1 = "idle";
const string PSEUDO_OPERATOR_2 = "++";
const string PSEUDO_OPERATOR_3 = "--";
const string PSEUDO_OPERATOR_4 = "move";


string get_instruction_fragment(const string &source_line, short fragment_index) {
    
    stringstream stream(source_line);
    string fragment;
    
    for (short current_index = 0; stream >> fragment; current_index++)
        if (current_index == fragment_index) return fragment;

    return "";
}


string pseudoinstruction_to_instruction(string source_line) {

    const string i0 = get_instruction_fragment(source_line, 0);
    const string i1 = get_instruction_fragment(source_line, 1);
    const string i2 = get_instruction_fragment(source_line, 2);

    if (i0 == PSEUDO_OPERATOR_0 && (i2.empty() || i2.at(0) == '#'))
        return (OPERATOR_0 + " " + i1 + " " + i1 + " r0\n");

    if (i0 == PSEUDO_OPERATOR_1)
        return OPERATOR_0 + " r0 r0 r0\n";
    
    if (i0 == PSEUDO_OPERATOR_2)
        return OPERATOR_8 + " " + i1 + " " + i1 + " 1\n";

    if (i0 == PSEUDO_OPERATOR_3)
        return OPERATOR_8 + " " + i1 + " " + i1 + " -1\n";

    if (i0 == PSEUDO_OPERATOR_4)
        return OPERATOR_1 + " " + i1 + " " + i2 + " r0\n";
        

    return source_line;
}


bool is_comment_or_blank(const string& line) {
    
    auto iterator = std::find_if_not(line.begin(), line.end(), [](char c) {
        return std::isspace(c);
    });

    return iterator == line.end() || *iterator == '#';
}