FROM mcr.microsoft.com/devcontainers/base:ubuntu

RUN apt-get update && \
    apt-get install -y \
        build-essential \
        gdb \
        valgrind \
        cmake \
        && apt-get clean && rm -rf /var/lib/apt/lists/*
