/****************************************************************************
 * Description: New_Recon_Framework
 * Author: Mahamadou Diakite
 * Lang: C/C++
 *  Date: 12/3/2022
 * Version: 0.0.1
 *****************************************************************************/

#include "worker2.h"
#include <logger.h>
#include <stdlib.h>
#include <sstream>

int main(int argc, char *argv[]) {
  static FlexRP::Logger log;
  if (argc < 3) {
    std::stringstream ss;
    ss << "Usage: " << argv[0] << " input_port output_port";
    log.error(ss.str());
    return EXIT_FAILURE;
  }

  FlexRP::Worker2 w(argv[1], argv[2]);
  w.process(log);

  return EXIT_SUCCESS;
}
