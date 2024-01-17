# Use an official Linux distribution as a base image
FROM ubuntu:latest

ENV DEBIAN_FRONTEND noninteractive

# Install essential build tools, GCC, GDB, and other necessary packages
RUN apt-get update &&  \
    apt-get install -y wget && \
    apt-get install -y git \ 
    build-essential \
    gcc \
    g++ \
    gdb \
    make \
    cmake \
    valgrind \
    cppcheck \
    libncurses-dev \
    libevent-dev \
    doxygen \
    graphviz \
    stlink-tools \
    # build tools for openocd
    libhidapi-hidraw0 \
    libusb-0.1-4 \
    libusb-1.0-0 \
    libhidapi-dev \
    libusb-1.0-0-dev \
    libusb-dev \
    libtool \
    automake \
    pkg-config \
    tclsh \
    telnet \
    openocd \ 
    python3 \
    bzip2 -y \
    && apt-get clean && \
    rm -rf /var/lib/apt/lists/*

# Install required dependencies
#RUN apt-get update && \
#    apt-get install -y python3 python3-pip \
#    && rm -rf /var/lib/apt/lists/*

# Install arm-none-eabi compiler
RUN wget https://developer.arm.com/-/media/Files/downloads/gnu-rm/10.3-2021.10/gcc-arm-none-eabi-10.3-2021.10-x86_64-linux.tar.bz2
RUN tar xf gcc-arm-none-eabi-10.3-2021.10-x86_64-linux.tar.bz2
RUN cp -rf gcc-arm-none-eabi-10.3-2021.10/* /usr/local/
RUN rm -rf gcc-arm-none-eabi-10.3-2021.10

# Download Miniforge installer
RUN wget "https://github.com/conda-forge/miniforge/releases/latest/download/Miniforge3-$(uname -s)-$(uname -m).sh" -O miniforge.sh

# Make the installer script executable
RUN chmod +x miniforge.sh

# Run the Miniforge installer
RUN ./miniforge.sh -b -p /opt/miniforge

# Clean up
RUN rm miniforge.sh

# Add Miniforge to the PATH
ENV PATH="/opt/miniforge/bin:$PATH"

# Create a Conda environment named 'cling'
RUN conda create -n cling python=3.8

# Activate the 'cling' environment
RUN echo "conda activate cling" >> ~/.bashrc

# Install JupyterLab and xeus-cling in the 'cling' environment
RUN /bin/bash -c "source activate cling && \
    conda install -c conda-forge jupyterlab xeus-cling && \
    conda clean --all -f -y"

# Expose the Jupyter Notebook port
EXPOSE 8888

# Start Jupyter Notebook with xeus-cling
CMD ["jupyter", "notebook", "--ip=0.0.0.0", "--port=8888", "--allow-root", "--notebook-dir=/notebooks"]


# Set the working directory inside the container
WORKDIR /workspace



# Entry point to keep the container running
#CMD ["tail", "-f", "/dev/null"]