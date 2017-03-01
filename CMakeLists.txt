cmake_minimum_required(VERSION 2.8.9)
project(dds_sample C)

SET(CMAKE_VERBOSE_MAKEFILE ON)

SET(PUB_NAME pub)
SET(PUB2_NAME pub2)
SET(SUB1_NAME sub1)
SET(SUB2_NAME sub2)
SET(SUB3_NAME sub3)
SET(SUB4_NAME sub4)



# libdds location
SET(DDS_LIB_DIR /opt/PrismTech/Vortex_v2/Device/VortexLite/2.0.4/lib/linux_gcc_x86/libdds.so)

# dds include files
SET(DDS_HEADER_INCLUDE_DIR /opt/PrismTech/Vortex_v2/Device/VortexLite/2.0.4/include)

# source codes
SET(PUB_SOURCES  pub.c )
SET(PUB2_SOURCES pub2.c)
SET(SUB1_SOURCES sub1.c)
SET(SUB2_SOURCES sub2.c)
SET(SUB3_SOURCES sub3.c)
SET(SUB4_SOURCES sub4.c)


# cflag
SET(EXT_C_FLAGS "-m64 -pipe -Wall -fno-strict-aliasing -O3 -std=c99 -Wstrict-prototypes -Wmissing-prototypes -Dos_linux_gcc_x86 -DLITE=1 -DNDEBUG -D_GNU_SOURCE")

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${EXT_C_FLAGS}")

add_subdirectory(generated)
add_subdirectory(threadpool)
add_subdirectory(lftpool)

include_directories (${DDS_HEADER_INCLUDE_DIR} )
include_directories (${dds_sample_SOURCE_DIR}/generated)
include_directories (${dds_sample_SOURCE_DIR}/threadpool)
include_directories (${dds_sample_SOURCE_DIR}/lftpool)

# pub
ADD_EXECUTABLE(${PUB_NAME} ${PUB_SOURCES})
TARGET_LINK_LIBRARIES(${PUB_NAME} LINK_PUBLIC ${DDS_LIB_DIR} rt pthread gen)

# pub2
ADD_EXECUTABLE(${PUB2_NAME} ${PUB2_SOURCES})
TARGET_LINK_LIBRARIES(${PUB2_NAME} LINK_PUBLIC ${DDS_LIB_DIR} rt pthread gen)


# sub1
ADD_EXECUTABLE(${SUB1_NAME} ${SUB1_SOURCES})
TARGET_LINK_LIBRARIES(${SUB1_NAME} LINK_PUBLIC ${DDS_LIB_DIR} rt pthread gen)

# sub2
ADD_EXECUTABLE(${SUB2_NAME} ${SUB2_SOURCES})
TARGET_LINK_LIBRARIES(${SUB2_NAME} LINK_PUBLIC ${DDS_LIB_DIR} rt pthread gen)

# sub3
ADD_EXECUTABLE(${SUB3_NAME} ${SUB3_SOURCES})
TARGET_LINK_LIBRARIES(${SUB3_NAME} LINK_PUBLIC ${DDS_LIB_DIR} rt pthread gen pool)

# sub4
ADD_EXECUTABLE(${SUB4_NAME} ${SUB4_SOURCES})
TARGET_LINK_LIBRARIES(${SUB4_NAME} LINK_PUBLIC ${DDS_LIB_DIR} rt pthread gen lftpool)

