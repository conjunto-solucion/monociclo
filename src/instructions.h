#include <string>

extern const std::string OPERATOR_0;
extern const std::string OPERATOR_1;
extern const std::string OPERATOR_2;
extern const std::string OPERATOR_3;
extern const std::string OPERATOR_4;
extern const std::string OPERATOR_5;
extern const std::string OPERATOR_6;
extern const std::string OPERATOR_7;
extern const std::string OPERATOR_8;
extern const std::string OPERATOR_9;
extern const std::string OPERATOR_10;
extern const std::string OPERATOR_11;
extern const std::string OPERATOR_12;

extern const std::string PSEUDO_OPERATOR_0;
extern const std::string PSEUDO_OPERATOR_1;
extern const std::string PSEUDO_OPERATOR_2;
extern const std::string PSEUDO_OPERATOR_3;
extern const std::string PSEUDO_OPERATOR_4;

/**
 * Separa a SOURCE_LINE en subcadenas delimitadas por espacios y
 * retorna la subcadena en la posición FRAGMENT_INDEX, comenzando a contar desde 0.
 */
extern std::string get_instruction_fragment(const std::string &source_line, short fragment_index);

/**
 * Dada la línea de pseudoinstrucción SOURCE_LINE, retorna la línea de instrucción equivalente.
 * Si SOURCE_LINE no es una pseudoinstrucción, devuelve SOURCE_LINE.
 */
extern std::string pseudoinstruction_to_instruction(std::string source_line);


/**
 *  Determina si LINE es una línea de comentario o una línea vacía.
 */
extern bool is_comment_or_blank(const std::string& line);