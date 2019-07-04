#include "pugixml.hpp"
#include <boost/process.hpp>
#include <cstring>
#include <string>
#include <iostream>
#include <vector>


std::vector<std::string> reconModules;
namespace bp = boost::process;

struct simple_walker: pugi::xml_tree_walker
{

    virtual bool for_each(pugi::xml_node& node)
    {
        //for (int i = 0; i < depth(); ++i)
        //  std::cout << "  "; // indentation

        if( strcmp(node.name(),  "name")== 0){
            // std::cout << node.name() << "   value= " << node.child_value() << "\n";
            reconModules.emplace_back(std::string(node.child_value()));
        }

        return true; // continue traversal
    }
};


int main(int argc, char **argv)
{
    if(argc < 2){
        std::cout << "Usage: FlexRecon configurationfilename.xml\n";
        return EXIT_FAILURE;
    }

    pugi::xml_document doc;
    const char * filename = argv[1];
    if (!doc.load_file(filename))
    {
        std::cout << "Couldn't load " << filename << std::endl;
        return EXIT_FAILURE;
    }

    // tag::traverse[]
    simple_walker walker;
    pugi::xml_node nodes = doc.child("FleXReconPipelineConfiguration");
    nodes.traverse(walker);

    for(std::string e : reconModules)
        std::cout << e <<'\n';

    //Spawn the recon module processes
    bp::group g;
    const std::string connect_port{"tcp://localhost:555"};
    const std::string bind_port{"tcp://*:555"};
    size_t count{};

    // Launch the workers
    for (size_t ii = 0; ii < reconModules.size()-1; ++ii)
    {
        auto exec = std::string{"./"} + reconModules[ii];
       // auto arg1 =  (ii+1) % 2 != 1 ? connect_port + std::to_string(5+ii)   : bind_port + std::to_string(5+ii)  ;
       // auto arg2 =  (ii+1) % 2 != 1 ? connect_port + std::to_string(5+ii+1) : bind_port + std::to_string(5+ii+1);

        auto arg1 =  (ii+1) % 2 != 1 ? connect_port + std::to_string(5+ii) : bind_port + std::to_string(5+ii);
        auto arg2 =  (ii+1) % 2 != 1 ? bind_port + std::to_string(5+ii+1)      : connect_port + std::to_string(5+ii+1);

        std::cout << arg1 << "  " << arg2 << std::endl;

       bp::spawn(exec, arg1, arg2, g);
        ++count;
    }


    // Launch the consumer
    const auto last_module_index = reconModules.size()-1;
    const auto exec1 = std::string{"./"} + reconModules[last_module_index];

    auto args1 =  count % 2 != 0 ?  bind_port + std::to_string(5+count) : connect_port + std::to_string(5+count);

    std::cout << args1 << std::endl;

    bp::spawn(exec1, args1, g);

    g.wait();
    //} catch () {

    //}

}

