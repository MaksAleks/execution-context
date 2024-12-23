#!/bin/sh

set -e -x

echo "Install deps"

apt-get update

apt-get install -y software-properties-common

apt-get install -y wget rsync

# CMake
wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc | apt-key add -
apt-add-repository 'deb https://apt.kitware.com/ubuntu/ jammy main'

apt-get update

apt-get install -y \
        ssh \
        make \
        cmake \
        build-essential \
        ninja-build \
        git \
        linux-tools-common \
        linux-tools-generic \
        gcc-12 \
        gdb \
        binutils-dev \
        libdwarf-dev \
        libdw-dev \
        python3 \
        python3-pip \
        python3-venv \
        ca-certificates \
        openssh-server \
        vim \
        autoconf

pip3 install \
        click \
        gitpython \
        python-gitlab \
        termcolor \
        virtualenv
