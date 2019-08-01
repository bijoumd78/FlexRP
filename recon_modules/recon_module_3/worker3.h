#ifndef WORKER3_H
#define WORKER3_H

#include "Base_Modules.h"

namespace FleXRP {

class Worker3 final : public Module_Worker_1
{
public:
    Worker3(const char *protocol1, const char *protocol2);
    int process() override;
};

}
#endif // WORKER2_H
