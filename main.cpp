//Stew Nowak
//End Tutorial (ET) Advanced Graphics
//Date last edited: 3/15/29

#include "FPEngine.h"

int main() {
    FPEngine app = FPEngine();

    try {
        app.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}