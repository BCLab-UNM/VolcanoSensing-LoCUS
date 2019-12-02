# Install Base OS
FROM ubuntu:18.04

# Install dependencies and set permissions
RUN apt-get update --fix-missing
RUN apt-get -y dist-upgrade
RUN apt-get install tzdata -y && apt-get install sudo git g++ cmake libgsl-dev \
    libfreeimage-dev libeigen3-dev libfreeimageplus-dev qt5-default freeglut3-dev libxi-dev \
    libxmu-dev liblua5.2-dev lua5.2 python vim wget -y
RUN useradd -m docker && echo "docker:docker" | chpasswd && adduser docker sudo

# Install ARGoS
ADD docker/*.deb /
RUN dpkg -i argos3_simulator-*.deb

# Install neatfa
ADD ./argos /faulttolerance/argos
WORKDIR /faulttolerance/argos
RUN mkdir build
RUN chmod +x /faulttolerance/argos/build.sh
RUN ls
RUN /faulttolerance/argos/build.sh

# Copy Scripts
RUN mkdir /root/scripts
COPY docker/scripts/*.py /root/scripts/

# Set up the entry point to invoke the simulator.
ENTRYPOINT ["python", "/root/scripts/entrypoint.py"]
