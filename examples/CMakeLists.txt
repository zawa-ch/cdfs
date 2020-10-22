
add_executable(cdfs-example-uint128 uint128.cpp)
target_link_libraries(cdfs-example-uint128 cdfs)

add_executable(cdfs-example-crc32 crc32.cpp)
target_link_libraries(cdfs-example-crc32 cdfs)

add_executable(cdfs-example-cdfsbuilder cdfsbuilder.cpp)
target_link_libraries(cdfs-example-cdfsbuilder cdfs)

add_executable(cdfs-example-cdfsloader cdfsloader.cpp)
target_link_libraries(cdfs-example-cdfsloader cdfs)
