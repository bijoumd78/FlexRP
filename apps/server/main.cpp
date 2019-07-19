#include "Flexrp_xml.h"
#include <exception>
#include <spdlog/spdlog.h>


int main()
{
    spdlog::info("Flex Recon Pipeline started...");
    using namespace FLEXRP;
    while(true){
        Flexrp_configuration fcg;
        try {
            deserialize(fcg);
            run_processes(fcg);

        } catch (std::exception& e) {
            spdlog::error("Exception: {}", e.what() );
        }

    }
}

