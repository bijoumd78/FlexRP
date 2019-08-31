#ifndef WORKER7_H
#define WORKER7_H

#include "Base_Modules.h"

namespace FleXRP {

class Worker7 final : public Module_Worker_1
{
public:
    Worker7(const char *protocol1, const char *protocol2);
    int process() override;
};

}
#endif // WORKER2_H
