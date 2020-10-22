
add_library(cdfs
  builder.cpp
  cdfs.cpp
  checksum.cpp
  datatype.cpp
  loader.cpp
)
target_include_directories(cdfs PUBLIC include)
