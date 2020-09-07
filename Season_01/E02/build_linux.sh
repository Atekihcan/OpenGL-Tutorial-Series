#!/bin/bash -e

# Get current directory
export CURRENT_DIR=$PWD

# export build and install directories
export BUILD_DIR=$CURRENT_DIR/_build_linux
export INSTALL_DIR=$CURRENT_DIR/_out

# export build configurations
export BUILD_TYPE=Debug

# Select CMake compiler
export CMAKE_GENERATOR="Unix Makefiles"

# If installation directory doesn't exist, create it
if [ ! -d $INSTALL_DIR ]; then
    mkdir $INSTALL_DIR
fi

# If build directory doesn't exist, create and issue CMake generator command
if [ ! -d $BUILD_DIR ]; then
    mkdir $BUILD_DIR
    cmake -H. -B$BUILD_DIR -"G$CMAKE_GENERATOR" -DCMAKE_INSTALL_PREFIX:PATH=$INSTALL_DIR
fi

# Build and install
cmake --build $BUILD_DIR --config $BUILD_TYPE
cmake --install $BUILD_DIR --config $BUILD_TYPE
