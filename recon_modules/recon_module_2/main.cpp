/****************************************************************************
 * Description: New_Recon_Framework
 * Author: Mahamadou Diakite
 * Lang: C/C++
 *  Date: 3/15/2018
 * Version: 0.0.1
 *****************************************************************************/

#include "worker2.h"
#include <logger.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  // Load logging configuration file
  FlexRP::Logger log("logConfig.json");

  if (argc < 3) {
    FlexRP::Logger::error("Usage: {} input_port output_port", argv[0]);
    return EXIT_FAILURE;
  }

  FlexRP::Worker2 w(argv[1], argv[2]);
  w.process();

  return EXIT_SUCCESS;
}
