#ifndef WORKER1_H
#define WORKER1_H

#include "Base_Modules.h"

namespace FlexRP {

class Worker1 final : public Module_Worker_1 {
 public:
  Worker1(const char *protocol1, const char *protocol2);
  int process() override;
};

}  // namespace FleXRP
#endif  // WORKER1_H
