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
    libncurses-dev \
    libevent-dev\
    && rm -rf /var/lib/apt/lists/*

# Install required dependencies
RUN apt-get update && \
    apt-get install -y python3 python3-pip \
    && rm -rf /var/lib/apt/lists/*

# Lib ncurses installation
# RUN apt-get update && \
#     apt-get install libncurses-dev \
#     && rm -rf /var/lib/apt/lists/*

# # Install wget, bash, and other required utilities
# RUN apt-get update && apt-get install -y wget bash && rm -rf /var/lib/apt/lists/*

# # Download and install Miniforge
# RUN wget "https://github.com/conda-forge/miniforge/releases/latest/download/Miniforge3-$(uname -s)-$(uname -m).sh" && \
#     bash Miniforge3-$(uname -s)-$(uname -m).sh -b && \
#     rm Miniforge3-$(uname -s)-$(uname -m).sh

# # Set the environment variables for Miniforge
# ENV PATH="/root/miniforge3/bin:$PATH"
# ENV CONDA_DEFAULT_ENV=cling
# ENV CONDA_PREFIX=/root/miniforge3/envs/$CONDA_DEFAULT_ENV

# # Create and activate a Conda environment named 'cling'
# RUN mamba create -n cling python=3.8 && \
#     echo "activate cling" >> ~/.bashrc && \
#     /bin/bash -c "source ~/.bashrc"

# # Install JupyterLab and xeus-cling in the 'cling' environment
# RUN mamba install -n cling -c conda-forge jupyterlab xeus-cling

# # Install and configure Jupyter kernels for C++11, C++14, and C++17

# # Expose the JupyterLab port
# EXPOSE 8888

# # Start JupyterLab as the default command
# CMD ["jupyter", "lab", "--ip=0.0.0.0", "--port=8888", "--no-browser", "--allow-root"]
# Set the working directory inside the container
WORKDIR /workspace

# Copy your C/C++ source code and other project files into the container
# If you have a project directory on your host, you can use a bind mount instead
# COPY . /workspace

# Entry point to keep the container running
CMD ["tail", "-f", "/dev/null"]