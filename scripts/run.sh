#!/bin/sh

if [ "$1" = "release" ]
then
    echo "\nRunning release build."
    ./bin/release/Mine-C-
else
    echo "\nRunning debug build."
    echo "Use argument \"release\" to run in release mode."
    ./bin/debug/Mine-C-
fi

