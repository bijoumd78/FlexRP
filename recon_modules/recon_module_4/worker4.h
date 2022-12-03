#ifndef WORKER4_H
#define WORKER4_H

#include "Base_Modules.h"

namespace FlexRP {

class Worker4 final : public Module_Sink_1 {
 public:
  explicit Worker4(Logger& log, const char *protocol);
  int process(Logger& log) override;
};

}  // namespace FleXRP

#endif  // WORKER2_H
