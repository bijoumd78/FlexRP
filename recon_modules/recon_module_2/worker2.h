#ifndef WORKER2_H
#define WORKER2_H

#include "Base_Modules.h"

namespace FleXRP {

class Worker2 final : public Module_Worker_2 {
 public:
  Worker2(const char *protocol1, const char *protocol2);
  int process() override;
};

}  // namespace FleXRP

#endif  // WORKER2_H
