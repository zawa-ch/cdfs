
add_library(cdfs
  builder.cpp
  cdfs.cpp
  checksum.cpp
  datatype.cpp
)
target_include_directories(cdfs PUBLIC include)
