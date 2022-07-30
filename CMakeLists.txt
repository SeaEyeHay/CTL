cmake_minimum_required (VERSION 3.16)
project ("CTL" LANGUAGES "C")


################### VARIABLES ####################

# Listings of directories
set (src_dir "${CMAKE_SOURCE_DIR}/src")
set (inc_dir "${CMAKE_SOURCE_DIR}/include")

# Listings of files
set (src_files "vector.c")
set (inc_files "vector.h")

################# END VARIABLES ##################

################### DEPENDENCIES ####################

# Import circular buffer implementation
add_subdirectory ("src/circ-buffer")

# Import dependencies for unit testing
if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    include (CTest)

    # Unit testing library libcheck
    find_package (check)
    if (NOT DEFINED check_FOUND)
        add_subdirectory ("deps/check")
    endif ()
endif ()

################# END DEPENDENCIES ##################

################### CONFIGURATION ####################

# Type of linking for the library
set (linking SHARED CACHE STRING "Type of linking for the library")


# Give each source file an absolute path
list (TRANSFORM src_files PREPEND "${src_dir}/")

# Give each header file an absolute path
list (TRANSFORM inc_files PREPEND "${inc_dir}/")

################# END CONFIGURATION ##################

################### TARGETS ####################

add_library (${PROJECT_NAME} ${linking} ${src_files} ${inc_files})
target_include_directories (${PROJECT_NAME} PUBLIC ${inc_dir})

################# END TARGETS ##################

