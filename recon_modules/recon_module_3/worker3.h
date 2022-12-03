#ifndef WORKER3_H
#define WORKER3_H

#include "Base_Modules.h"

namespace FlexRP {

class Worker3 final : public Module_Worker_1 {
 public:
  Worker3(const char *protocol1, const char *protocol2);
  int process(Logger& log) override;
};

}  // namespace FleXRP
#endif  // WORKER2_H
