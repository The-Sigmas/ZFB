#!/bin/bash

# Used to combined all the lib files
# into one big library

LIB_DIR=build/lib

ar -M <<EOM
CREATE ${LIB_DIR}/libZFB_C.a
ADDLIB ${LIB_DIR}/libZFB.a
ADDLIB ${LIB_DIR}/libpng.a
ADDLIB ${LIB_DIR}/libz.a
SAVE
END
EOM
