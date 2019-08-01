#ifndef WORKER4_H
#define WORKER4_H

#include "Base_Modules.h"

namespace FleXRP {

class Worker4 final : public Module_Sink
{
public:
    explicit Worker4(const char *protocol);
    int process() override;
};

}

#endif // WORKER2_H
