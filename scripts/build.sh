#!/bin/sh

mkdir -p bin
cd bin 
mkdir -p release
mkdir -p debug

if [ "$1" = "release" ]
then
    cd release
    cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -DOpenGL_GL_PREFERENCE="GLVND" -DCMAKE_BUILD_TYPE=Release ../..
    make
    echo "Built target in bin/release/"
else
    cd debug 
    cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -DOpenGL_GL_PREFERENCE="GLVND" -DCMAKE_BUILD_TYPE=Debug ../..
	rm ../../compile_commands.json
	cp ./compile_commands.json ../../
    make
    echo "\nBuilt target in bin/debug/"
    echo "Use argument \"release\" to build in release mode."
fi

