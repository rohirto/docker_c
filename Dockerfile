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

# Set the working directory inside the container
WORKDIR /workspace

# Copy your C/C++ source code and other project files into the container
# If you have a project directory on your host, you can use a bind mount instead
# COPY . /workspace

# Entry point to keep the container running
CMD ["tail", "-f", "/dev/null"]