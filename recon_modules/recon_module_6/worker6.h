#ifndef WORKER6_H
#define WORKER6_H

#include "Base_Modules.h"

namespace FleXRP{

class Worker6 final : public Module_Worker_2
{
public:
    Worker6(const char *protocol1, const char *protocol2);
    int process() override;
};

}

#endif // WORKER2_H