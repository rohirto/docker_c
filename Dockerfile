# Use an official Linux distribution as a base image
FROM ubuntu:latest

# Install essential build tools, GCC, GDB, and other necessary packages
RUN apt-get update && apt-get install -y \
    build-essential \
    gcc \
    g++ \
    gdb \
    make \
    cmake \
    git \
    valgrind \
    cppcheck \
    && rm -rf /var/lib/apt/lists/*

# Install required dependencies
RUN apt-get update && \
    apt-get install -y python3 python3-pip && \
    pip3 install jupyter

# Lib ncurses installation
RUN apt-get update && \
    apt-get install libncurses-dev \
    && rm -rf /var/lib/apt/lists/*

# Set the working directory inside the container
WORKDIR /workspace

# Set up Jupyter configuration
RUN jupyter notebook --generate-config

# Expose the Jupyter port
EXPOSE 8888

# Start Jupyter Notebook in the background
RUN jupyter notebook --ip=0.0.0.0 --port=8888 --allow-root &

# Copy your C/C++ source code and other project files into the container
# If you have a project directory on your host, you can use a bind mount instead
# COPY . /workspace

# Entry point to keep the container running
CMD ["tail", "-f", "/dev/null"]