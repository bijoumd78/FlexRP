#include <iostream>
#include "Flexrp_xml.h"

int main(int argc, char **argv)
{
    if(argc < 2){
        std::cout << "Usage: FlexRecon configurationfilename.xml\n";
        return EXIT_FAILURE;
    }

    using namespace FLEXRP;
    Flexrp_configuration fcg;
    deserialize(argv[1], fcg);
    run_processes(fcg);

    return 0;
}

