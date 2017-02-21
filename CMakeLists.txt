cmake_minimum_required(VERSION 2.8.9)
project(dds_sample C)

SET(CMAKE_VERBOSE_MAKEFILE ON)

SET(PUB_NAME pub)
SET(SUB1_NAME sub1)
SET(SUB2_NAME sub2)


# libdds location
SET(DDS_LIB_DIR /opt/PrismTech/Vortex_v2/Device/VortexLite/2.0.4/lib/linux_gcc_x86/libdds.so)

# dds include files
SET(DDS_HEADER_INCLUDE_DIR /opt/PrismTech/Vortex_v2/Device/VortexLite/2.0.4/include)

# source codes
SET(PUB_SOURCES  pub.c )
SET(SUB1_SOURCES sub1.c)
SET(SUB2_SOURCES sub2.c)

# cflag
SET(EXT_C_FLAGS "-m64 -pipe -Wall -fno-strict-aliasing -O3 -std=c99 -Wstrict-prototypes -Wmissing-prototypes -Dos_linux_gcc_x86 -DLITE=1 -DNDEBUG -D_GNU_SOURCE")

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${EXT_C_FLAGS}")

add_subdirectory(generated)

include_directories (${DDS_HEADER_INCLUDE_DIR} )
include_directories (${dds_sample_SOURCE_DIR}/generated)

# pub
ADD_EXECUTABLE(${PUB_NAME} ${PUB_SOURCES})
TARGET_LINK_LIBRARIES(${PUB_NAME} LINK_PUBLIC ${DDS_LIB_DIR} rt pthread gen)

# sub1
ADD_EXECUTABLE(${SUB1_NAME} ${SUB1_SOURCES})
TARGET_LINK_LIBRARIES(${SUB1_NAME} LINK_PUBLIC ${DDS_LIB_DIR} rt pthread gen)

# sub2
ADD_EXECUTABLE(${SUB2_NAME} ${SUB2_SOURCES})
TARGET_LINK_LIBRARIES(${SUB2_NAME} LINK_PUBLIC ${DDS_LIB_DIR} rt pthread gen)