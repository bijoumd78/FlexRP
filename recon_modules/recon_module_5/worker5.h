#ifndef WORKER4_H
#define WORKER4_H

#include "Base_Modules.h"

namespace FlexRP {

class Worker5 final : public Module_Sink_2 {
 public:
  explicit Worker5(const char *protocol);
  int process(Logger& log) override;
};

}  // namespace FleXRP

#endif  // WORKER2_H
