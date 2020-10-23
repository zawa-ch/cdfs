# zawa-ch/cdfs:/src/CMakeLists
# Copyright 2020 zawa-ch.
#

add_library(cdfs
  builder.cpp
  cdfs.cpp
  checksum.cpp
  datatype.cpp
  loader.cpp
)
target_include_directories(cdfs PUBLIC include)
