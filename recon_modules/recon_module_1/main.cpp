/****************************************************************************
 * Description: New_Recon_Framework
 * Author: Mahamadou Diakite
 * Lang: C/C++
 *  Date: 3/15/2018
 * Version: 0.0.1
 *****************************************************************************/

#include <logger.h>
#include <stdlib.h>
#include "worker1.h"
#include <sstream>


int main(int argc, char *argv[]) {

  static FlexRP::Logger log;
  log.info("Starting worker1");

  if (argc < 3) {
    std::stringstream ss;
    ss << "Usage: " << argv[0] << " input_port output_port";
    log.error(ss.str());
    return EXIT_FAILURE;
  }

  FlexRP::Worker1 w(argv[1], argv[2]);
  w.process(log);

  return EXIT_SUCCESS;
}