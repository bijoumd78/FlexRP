/****************************************************************************
 * Description: New_Recon_Framework
 * Author: Mahamadou Diakite
 * Lang: C/C++
 *  Date: 3/15/2018
 * Version: 0.0.1
 *****************************************************************************/

#include <spdlog/spdlog.h>
#include "worker4.h"

int main(int argc, char *argv[]) {
  if (argc < 2) {
    spdlog::error("Usage: {} input_port", argv[0]);
    return EXIT_FAILURE;
  }

  FleXRP::Worker4 s(argv[1]);
  s.process();

  return EXIT_SUCCESS;
}
