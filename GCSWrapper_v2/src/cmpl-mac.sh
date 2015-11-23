#!/bin/bash

#clang++ -I/usr/local/include/eigen3 *.cpp

#swipl-ld -c++ clang++ -I/usr/local/include/eigen3 -o clpqs-gcs.dylib -shared *.cpp

swipl-ld -c++ clang++ -I/usr/local/include/eigen3 -o clpqs-gcs.dylib -shared GCSWrapper.cpp Constraints.cpp GCS.cpp qp_eq.cpp SubSystem.cpp clpqs-gcs.cpp

