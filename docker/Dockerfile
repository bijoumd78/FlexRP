FROM ubuntu:bionic AS build

# Install tools and libraries required for the project
RUN apt-get update --quiet &&\
    apt-get install --no-install-recommends --no-install-suggests --yes  \
    software-properties-common apt-utils wget build-essential cmake git-core vim tmux libboost-all-dev libhdf5-serial-dev libfftw3-dev libpugixml-dev libzmq3-dev tree

# Install ISMRMRD
RUN mkdir /opt/code &&\
    cd /opt/code && \
    git clone https://github.com/ismrmrd/ismrmrd.git && \
    cd ismrmrd && \
    mkdir build && \
    cd build && \
    cmake ../ && \
    make -j $(nproc) && \
    make install

# Intall spdlog
RUN cd /opt/code && \
    git clone https://github.com/gabime/spdlog.git && \
    cd spdlog && \
    mkdir build && \
    cd build && \
    cmake ../ && \
    make -j $(nproc) && \
    make install


#Set more environment variables in preparation for FlexRP installation
ENV FLEXRP_HOME=/usr/local \
    ISMRMRD_HOME=/usr/local

ENV PATH=$PATH:$FLEXRP_HOME/bin:$ISMRMRD_HOME/bin \
    LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$ISMRMRD_HOME/lib:$FLEXRP_HOME/lib

# Copy the entire project and build it
COPY . /FlexRP/ 

RUN cd /FlexRP &&\
    mkdir build &&\
    cd build  &&\
    cmake ../ &&\
    make -j $(nproc) &&\
    make install
