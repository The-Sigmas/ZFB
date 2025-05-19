#!/bin/bash

# Used to combined all the lib files
# into a big fat one

ar -M <<EOM
CREATE libZFB_fat.a
ADDLIB libzfb.a
ADDLIB libpng.a
ADDLIB libz.a
SAVE
END
EOM
