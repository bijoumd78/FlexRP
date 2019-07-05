#include "Flexrp_xml.h"
#include "pugixml.hpp"
#include <boost/process.hpp>
#include <iostream>
#include <stdexcept>
#include <algorithm>

namespace  FLEXRP
{
    void deserialize(const char *config_filename, Flexrp_configuration &fcg)
    {
        pugi::xml_document doc;
        if (!doc.load_file(config_filename))
        {
           std::runtime_error("FlexRP configuration file not");
        }

        pugi::xml_node nodes = doc.child("FleXReconPipelineConfiguration");

        // Start filling the config struct
        pugi::xml_node reader = nodes.child("reader");
        while (reader) {
          Readers r;
          r.name = reader.child_value("name");
          fcg.readers.emplace_back(r);
          reader = reader.next_sibling("reader");
        }

        pugi::xml_node writer = nodes.child("writer");
        while (writer) {
          Writers w;
          w.name = writer.child_value("name");
          fcg.writers.push_back(w);
          writer = writer.next_sibling("writer");
        }

        pugi::xml_node recon_module = nodes.child("ReconModule");
        while (recon_module) {
          Recon_modules rm;
          rm.name = recon_module.child_value("name");

          pugi::xml_node property = recon_module.child("property");
          while (property) {
            Recon_modules_Properties p;
            p.name = property.child_value("name");
            p.value = property.child_value("value");
            rm.properties.push_back(p);
            property = property.next_sibling("property");
          }

          fcg.recon_modules.push_back(rm);
          recon_module = recon_module.next_sibling("ReconModule");
        }
    }

    void run_processes(const Flexrp_configuration &fcg)
    {
        namespace bp = boost::process;

        try {

            bp::group g;
            const std::string connect_port{"tcp://localhost:555"};
            const std::string bind_port{"tcp://*:555"};
            size_t count{};

            // Launch readers
            std::for_each(fcg.readers.cbegin(),
                          fcg.readers.cend(),
                          [&](const Readers &r){
               auto exec = std::string{"./"} + r.name;
               auto arg1 =  (count+1) % 2 == 1 ? connect_port + std::to_string(5+count)   : bind_port + std::to_string(5+count)  ;
               auto arg2 =  (count+1) % 2 == 1 ? connect_port + std::to_string(5+count+1) : bind_port + std::to_string(5+count+1);

                std::cout << arg1 << "  " << arg2 << std::endl;

               bp::spawn(exec, arg1, arg2, g);
                ++count;
            });

            // Launch the workers
            std::for_each(fcg.recon_modules.cbegin(),
                          fcg.recon_modules.cend(),
                          [&](const Recon_modules &rc){
               auto exec = std::string{"./"} + rc.name;
               auto arg1 =  (count+1) % 2 == 1 ? connect_port + std::to_string(5+count)   : bind_port + std::to_string(5+count)  ;
               auto arg2 =  (count+1) % 2 == 1 ? connect_port + std::to_string(5+count+1) : bind_port + std::to_string(5+count+1);

                std::cout << arg1 << "  " << arg2 << std::endl;

               bp::spawn(exec, arg1, arg2, g);
                ++count;
            });

           // Launch the writers
            std::for_each(fcg.writers.cbegin(),
                          fcg.writers.cend(),
                          [&](const Writers &w){
               auto exec = std::string{"./"} + w.name;
               auto arg1 =  (count+1) % 2 == 1 ? connect_port + std::to_string(5+count)   : bind_port + std::to_string(5+count)  ;
               auto arg2 =  (count+1) % 2 == 1 ? connect_port + std::to_string(5+count+1) : bind_port + std::to_string(5+count+1);

                std::cout << arg1 << std::endl;

               bp::spawn(exec, arg1, g);
                ++count;
            });

            g.wait();

        } catch (std::exception &e) {
            std::cout << e.what() << std::endl;
        }

    }

}
