# FlexRP 

<!-- Doxygen Table of Contents -->
[TOC]

FlexRP is a general-purpose data streaming framework. The primary intent is to enable flexible and scalable image reconstruction using the pipeline architecture where each reconstruction step can be modularized and run on its own process. This software is a Work In Progress (WIP) and it is intended for research use only (See License for details).


## Quick Start with Docker 

The easiest way to install and to run FlexRP framework is to download one of the pre-built and tested Docker images:

    sudo docker run -it --name flexrp bijoumd/flexrp 
The framework consists of a server "FleXRP" and a client  "FlexRP_ismrmrd_client" applications. The Server is launched by typing:

    FleXRP
The client can be launched with the following provided test data "data.h5" and one of these template configuration files (config_file.xml or config_file_2.xml):

    FlexRP_ismrmrd_client -f FlexRP/etc/data/data.h5 -c FlexRP/etc/config/config_file.xml -o output_file.h5


## Prerequisites

 - C++17 or higher 
 - Boost-program-options
 - Boost-filesystem
 - hdf5
 - fftw3
 - pugixml
 - cppzmq
 - spdlog
 - ISMRMRD
 - cmake
 - git


## Build and Install FlexRP 

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
