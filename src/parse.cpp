#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <stdexcept>

#include "./instructions.h"
#include "./parse.h"


#define MINIMUM_REGISTER_NUMBER 0
#define MAXIMUM_REGISTER_NUMBER 15
#define MAXIMUM_IMMEDIATE_VALUE 7
#define REGISTER_PREFIX 'r'
#define NEGATIVE_PREFIX '-'


using namespace std;



static const string hexdigit[16] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "a", "b", "c", "d", "e", "f"};

static const unordered_map<string, string> hex_opcodes = {
    {OPERATOR_0, "0"},
    {OPERATOR_1, "1"},
    {OPERATOR_2, "2"},
    {OPERATOR_3, "3"},
    {OPERATOR_4, "4"},
    {OPERATOR_5, "5"},
    {OPERATOR_6, "6"},
    {OPERATOR_7, "7"},
    {OPERATOR_8, "8"},
    {OPERATOR_9, "9"},
    {OPERATOR_10, "a"},
    {OPERATOR_11, "b"},
    {OPERATOR_12, "c"},
};

static string register_name_to_hex(string register_name) {
    const int register_number = stoi(register_name.substr(1));

    if (register_number > MAXIMUM_REGISTER_NUMBER || register_number < MINIMUM_REGISTER_NUMBER)
    throw runtime_error("El registro referenciado no existe.");
    return hexdigit[register_number];
}


static string negative_immediate_to_hex(string immediate) {
    const int immediate_value = stoi(immediate.substr(1));

    if (immediate_value > MAXIMUM_IMMEDIATE_VALUE)
    throw runtime_error("Los valores inmediatos deben estar entre -7 y 7.");

    if (immediate_value == '0')
    return "0";

    return hexdigit[16-immediate_value];
}







string operator_to_hexadecimal_string(string op) {
    return hex_opcodes.find(op)->second;
}


string to_hex(string instruction_fragment) {
    
    if (instruction_fragment.at(0) == REGISTER_PREFIX)
    return register_name_to_hex(instruction_fragment);


    if (instruction_fragment.at(0) == NEGATIVE_PREFIX)
    return negative_immediate_to_hex(instruction_fragment);


    const int immediate_value = stoi(instruction_fragment);
    if (immediate_value > MAXIMUM_IMMEDIATE_VALUE)
    throw runtime_error("Los valores inmediatos deben estar entre -7 y 7.");

    return hexdigit[stoi(instruction_fragment)];
}



const void write_hexadecimal_text(ifstream& source, ofstream& output) {

    string line, i0, i1, i2, i3;
    unsigned int instruction_count = 0;
    unsigned int line_count = 0;
    

    output << "v2.0 raw\n";


    while(getline(source, line)) {

        line_count++;

        if (is_comment_or_blank(line))
        continue;

        line = pseudoinstruction_to_instruction(line);
        
        i0 = i1 = i2 = i3 = "";
        istringstream iss(line);
        iss >> i0 >> i1 >> i2 >> i3;

        
        if (i0.empty() || i0 == "#" || i1.empty() || i1 == "#" || i2.empty() || i2 == "#" || i3.empty() || i3 == "#") {
            cerr << "Error en la línea " << line_count << ". Formato incorrecto de instrucción." << endl;
            return;
        }

        try {
            output << operator_to_hexadecimal_string(i0) + to_hex(i1) + to_hex(i2) + to_hex(i3) + " ";
        }

        catch (const runtime_error& e) {
            cerr << "Error en la línea " << line_count << ". " << e.what() << endl;
            return;
        }
        
        catch (const invalid_argument& e) {
            cerr << "Error en la línea " << line_count << ". Expresión inválida." << endl;
            return;
        }
        
        catch (const out_of_range& e) {
            std::cerr << "Error en la línea " << line_count << ". Número fuera de rango." << endl;
            return;
        }

        instruction_count++;

        if (instruction_count%8 == 0)
        output << "\n";
            
    }
}