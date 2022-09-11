cmake_minimum_required (VERSION 3.16)
project ("CTL" LANGUAGES "C")


################### VARIABLES ####################

# Listings of directories
set (src_dir "${CMAKE_SOURCE_DIR}/src")
set (inc_dir "${CMAKE_SOURCE_DIR}/include")


# Listing of headers
set (inc_files 
    "macros.h" "template.h" "undef-list.h"      # Pre-proc stuffs

    "iter.h"

    "stack.h"  "stack-std.h"                    # Stacks
    "deque.h"  "deque-std.h"                    # Deques
    "link.h"   "link-std.h"                     # linked-lists
)


# Listings of source files
set (src_files "stack.c" "deque.c" "iter.c" "link.c")

# Listings of implementation files
set (impl_files "deque-std.c" "link-std.c" "stack-std.c")


# Listings of unit test
set (test_dir "${CMAKE_SOURCE_DIR}/tests")
set (test_files )

################# END VARIABLES ##################

################### CONFIGURATION ####################

# Type of linking for the library
set (linking SHARED CACHE STRING "Type of linking for the library")

# Build unit tests
option ("${PROJECT_NAME}_build_testing" "Build unit tests for CTL" ON)

# Build Data Structures implementation for standard types
option (build_std_type_impl "Build implementations for standard Types" ON)


# Give each source file an absolute path
list (TRANSFORM src_files PREPEND "${src_dir}/")

# Give each header file an absolute path
list (TRANSFORM inc_files PREPEND "${inc_dir}/")


# Give each standard implementation source file an absolute path
list (TRANSFORM impl_files PREPEND "${src_dir}/")

################# END CONFIGURATION ##################

################### DEPENDENCIES ####################

# Import dependencies for unit testing
if ((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME OR "${PROJECT_NAME}_build_testing") AND BUILD_TESTING)
    include (CTest)

    # Unit testing library libcheck
    find_package (check QUIET)
    if (NOT ${check_FOUND})
        add_subdirectory ("deps/check")
    endif ()
endif ()

################# END DEPENDENCIES ##################

################### TARGETS ####################

# CTL Library
add_library (${PROJECT_NAME} ${linking} ${src_files} ${inc_files})
target_include_directories (${PROJECT_NAME} PUBLIC ${inc_dir})


# Data Structures implementations 
if (${build_std_type_impl})
    add_library ("${PROJECT_NAME}std" ${linking} ${impl_files})
    target_link_libraries ("${PROJECT_NAME}std" ${PROJECT_NAME})
endif ()

################# END TARGETS ##################

################### UNIT TESTS ####################

if ((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME OR "${PROJECT_NAME}_build_testing") AND BUILD_TESTING)
    
    # Create individual executables and runner for each test
    foreach (test IN LISTS test_files)
        set (path "${test_dir}/${test}")
        set (target "${test}.run")

        # Build test executable
        add_executable ("${target}" ${path})
        target_link_libraries ("${target}" ${PROJECT_NAME} check pthread)

        # Create test runner
        add_test (NAME "${test}" COMMAND "${target}")
    endforeach ()

    # Include benchmarks
    # add_subdirectory ("benchmarks")

endif ()

################# END UNIT TESTS ##################

