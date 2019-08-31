/****************************************************************************
 * Description: New_Recon_Framework
 * Author: Mahamadou Diakite
 * Lang: C/C++
 *  Date: 3/15/2018
 * Version: 0.0.1
 *****************************************************************************/

#include "worker7.h"
#include <spdlog/spdlog.h>

int main (int argc, char *argv[])
{
    if (argc < 3)
    {
        spdlog::error("Usage: {} input_port output_port", argv[0]);
        return EXIT_FAILURE;
    }

    FleXRP::Worker7 w(argv[1], argv[2]);
    w.process();

    return EXIT_SUCCESS;
}
