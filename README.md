# FlexRP
FlexRP is a general purpose streaming data processing framework. The primary intent is to enable flexible and scalable image reconstruction using the pipeline architecture where each reconstruction step can be modularized and run on its own process. This software is a Work In Progress (WIP) and it is intented for research use only (See Licence for details).

## Take a spin
The easiest and quickest way to install and run FlexRP framework is by downloading one of the pre-built and tested Docker images:

    sudo docker run -it --name flexrp bijoumd/flexrp 
The framework consists of a server "FleXRP" and a client  "FlexRP_ismrmrd_client" applications. The Server is launched by typing:

    FleXRP
The client can be launched with the following provided data "data.h5" and one of these templates configuration file:
FlexRP_ismrmrd_client -f FlexRP/etc/data/data.h5 -c FlexRP/etc/config/config_file

<!--stackedit_data:
eyJoaXN0b3J5IjpbLTcyNDE0OTU3OSwtMTM3MjQ5MzIzNyw5MD
E4MDI3MjRdfQ==
-->