FROM ubuntu:disco

# Install tools required for the project
RUN apt-get update --quiet\
    && apt-get install --no-install-recommends --no-install-suggests --yes  \
    && apt-get install gcc -y \
    && apt-get install g++ -y \
    && apt-get install cmake -y \
    && apt-get install wget -y  \
    && apt-get install libhdf5-serial-dev \
    && apt-get install libfftw3-dev \
    && apt-get install libpugixml-dev \
    && apt-get install ninja-build

# Install Boost
RUN mkdir /opt/code
RUN cd /opt/code \
    && wget http://downloads.sourceforge.net/project/boost/boost/1.67.0/boost_1_67_0.tar.gz \
    && tar xfz boost_1_67_0.tar.gz \
    && rm boost_1_67_0.tar.gz \
    && cd boost_1_67_0 \
    && ./bootstrap.sh --with-libraries=system,thread,program_options,filesystem,timer \
    && ./b2 install

# Install ISMRMRD
RUN cd /opt/code && \
    git clone https://github.com/ismrmrd/ismrmrd.git && \
    cd ismrmrd && \
    mkdir build && \
    cd build && \
    cmake -G Ninja ../ && \
    ninja && \
    ninja install

# Intall spdlog
RUN cd /opt/code && \
    git clone https://github.com/gabime/spdlog.git && \
    cd spdlog && \
    mkdir build && \
    cd build && \
    cmake -G Ninja../ && \
    ninja && \
    ninja install

#Set more environment variables in preparation for FlexRP installation
ENV FLEXRP_HOME=/usr/local \
    ISMRMRD_HOME=/usr/local

ENV PATH=$PATH:$FLEXRP_HOME/bin:$ISMRMRD_HOME/bin \
    LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$ISMRMRD_HOME/lib:$FLEXRP_HOME/lib

# Copy the entire project and build it
COPY ../../ /FlexRP/ 
WORKDIR /FlexRP/

RUN    cd /FlexRP \
    && mkdir build \
    && cd build    \
    && cmake -G Ninja ../ \
    && ninja  \
    && ninja install 


