/****************************************************************************
 * Description: New_Recon_Framework
 * Author: Mahamadou Diakite
 * Lang: C/C++
 *  Date: 3/15/2018
 * Version: 0.0.1
 *****************************************************************************/

#include "worker5.h"
#include <logger.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  // Load logging configuration file
  FlexRP::Logger log("logConfig.json");

  if (argc < 2) {
    FlexRP::Logger::error("Usage: {} input_port", argv[0]);
    return EXIT_FAILURE;
  }

  FlexRP::Worker5 s(argv[1]);
  s.process();

  return EXIT_SUCCESS;
}
