# FlexRP
FlexRP is a general purpose streaming data processing framework. The primary intent is to enable flexible and scalable image reconstruction using the pipeline architecture where each reconstruction step can be modularized and run on its own process. This software is a Work In Progress (WIP) and it is intended for research use only (See Licence for details).

## Take a spin
The easiest and quickest way to install and run FlexRP framework is by downloading one of the pre-built and tested Docker images:

    sudo docker run -it --name flexrp bijoumd/flexrp 
The framework consists of a server "FleXRP" and a client  "FlexRP_ismrmrd_client" applications. The Server is launched by typing:

    FleXRP
The client can be launched with the following provided test data "data.h5" and one of these template configuration files (config_file.xml or config_file_2.xml):

    FlexRP_ismrmrd_client -f FlexRP/etc/data/data.h5 -c FlexRP/etc/config/config_file.xml


## Prerequisites
 - C++11  or higher compiler
 - Boost 1.65 or higher version
 - hdf5
 - fftw3
 - pugixml
 - zmq3
 - spdlog
 - ISMRMRD
 - cmake 3.6 or higher
 - git
 
## Build and install FlexRP
Copy and paste the following commands:

    git clone https://github.com/bijoumd78/FlexRP.git
    cd FlexRP
    mkdir build
    cd build
    cmake ..
    make 
    make install
Add FlexRP installation directories to your machine search path and you are all set!
<!--stackedit_data:
eyJoaXN0b3J5IjpbMTc5NTU2Mzc5NywtMjA1NjUzMjQ0NywxNT
E2MTA4MzM3LC0xMzcyNDkzMjM3LDkwMTgwMjcyNF19
-->