#include <spdlog/spdlog.h>
#include "Flexrp_xml.h"

int main(int argc, char **argv)
{
    if(argc < 2){
        spdlog::error("Usage: FlexRecon configurationfilename.xml");
        return EXIT_FAILURE;
    }

    spdlog::info("Flex Recon Pipeline started...");
    spdlog::info("Waiting for the incoming data...");
    using namespace FLEXRP;
    Flexrp_configuration fcg;
    deserialize(argv[1], fcg);
    run_processes(fcg);

    return 0;
}

