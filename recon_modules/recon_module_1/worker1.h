#ifndef WORKER1_H
#define WORKER1_H

#include "Base_Modules.h"

namespace FleXRP{

class Worker1 : public Module_Worker_1
{
public:
    Worker1(const char *protocol1, const char *protocol2);
    int process() override;
};

}
#endif // WORKER1_H
