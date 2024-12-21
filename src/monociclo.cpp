#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

#include "./parse.h"

using namespace std;



int main(int argc, char *argv[]) {

    if (argc < 2 || argc > 3) {
        perror("\nUso: monociclo <fuente> [salida]\n");
        return EXIT_FAILURE;
    }

    const string    source_file_path = argv[1];
    const string    output_file_path = (argc == 3) ? argv[2] : "./out";

    ifstream        source(source_file_path);
    ofstream        output(output_file_path);

    write_hexadecimal_text(source, output);


    source.close();
    output.close();

    return EXIT_SUCCESS;
}
