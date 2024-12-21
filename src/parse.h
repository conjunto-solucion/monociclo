#include <string>
#include <sstream>
#include <unordered_map>


/**
 * Dado un operador de instrucción OPERATOR, retorna el número hexadecimal equivalente de 1 dígito.
 */
std::string operator_to_hexadecimal_string(std::string op);

/**
 * Sea INSTRUCTION_FRAGMENT un fragmento de instrucción correspondiente al nombre de un registro o a un valor inmediato, retorna el equivalente en hexadecimal de un dígito.
 */
std::string to_hex(std::string instruction_fragment);

/**
 * Traduce a hexadecimal las instrucciones en SOURCE y escribe el resultado en el archivo de texto OUTPUT.
 */
const void write_hexadecimal_text(std::ifstream& source, std::ofstream& output);