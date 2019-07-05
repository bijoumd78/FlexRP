#ifndef FLEXRP_XML_H
#define FLEXRP_XML_H

#include <string>
#include <vector>

namespace  FLEXRP
{
    struct Readers{
        std::string name;
    };

    struct Writers{
        std::string name;
    };

    struct Recon_modules_Properties{
        std::string name;
        std::string value;
    };

    struct Recon_modules{
        std::string name;
        std::vector<Recon_modules_Properties> properties;
    };


    struct Flexrp_configuration{
        std::vector<Readers> readers;
        std::vector<Writers> writers;
        std::vector<Recon_modules> recon_modules;
    };

    void deserialize(const char *config_filename, Flexrp_configuration &fcg);
    void run_processes(const Flexrp_configuration &fcg);

}

#endif // FLEXRP_XML_H
