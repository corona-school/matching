#include <stdexcept>
#include "algo.h"

int main(int argc, char *argv[]){
    if(argc != 6){
        throw std::runtime_error("The algorithm needs 5 input files. A file with the informaiont about the pupils, two files for the students, one file about the dissolved matches and the location for the output file."
                                 "For further information see the readme file.");
    }
    char const* filename_pupils = argv[1];
    char const* filename_students = argv[2];
    char const* filename_students_lb = argv[3];
    char const* filename_dissolved_matches = argv[4];
    char const* filename_output = argv[5];

    performAlgorithmWithFilenames(filename_pupils, filename_students, filename_students_lb, filename_dissolved_matches, filename_output);
}
