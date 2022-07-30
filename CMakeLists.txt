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

# Import allocator
add_subdirectory ("src/circ-buffer")

################# END DEPENDENCIES ##################

################### CONFIGURATION ####################

# Type of linking for the library
set (linking SHARED CACHE STRING "Type of linking for the library")

# If the library is used as header only
option (header_only "Use CTL as header only" OFF)

if (${header_only})
    add_compile_definitions (HEADER_ONLY)
endif ()


# Give each source file an absolute path
list (TRANSFORM src_files PREPEND "${src_dir}/")

# Give each header file an absolute path
list (TRANSFORM inc_files PREPEND "${inc_dir}/")

################# END CONFIGURATION ##################

################### TARGETS ####################

if (${header_only})
    add_library (${PROJECT_NAME} INTERFACE)
    target_include_directories (${PROJECT_NAME} INTERFACE ${inc_dir})
else ()
    add_library (${PROJECT_NAME} ${linking} ${src_files} ${inc_files})
    target_include_directories (${PROJECT_NAME} PUBLIC ${inc_dir})
endif ()

################# END TARGETS ##################

