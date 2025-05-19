#!/bin/bash

# Used to combined all the lib files
# into one big library

LIB_DIR=build/lib

ar -M <<EOM
CREATE ${LIB_DIR}/libZFB_fat.a
ADDLIB ${LIB_DIR}/libzfb.a
ADDLIB ${LIB_DIR}/libpng.a
ADDLIB ${LIB_DIR}/libz.a
SAVE
END
EOM
