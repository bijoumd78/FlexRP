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
#include <sstream>

int main(int argc, char *argv[]) {
  using namespace FlexRP;
  static Logger log;
  log.info("Starting worker5");

  if (argc < 2) {
    std::stringstream ss;
    ss << "Usage: " << argv[0] << " input_port";
    log.error(ss.str());
    return EXIT_FAILURE;
  }

  FlexRP::Worker5 s(argv[1]);
  s.process(log);

  return EXIT_SUCCESS;
}
