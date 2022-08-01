cmake_minimum_required (VERSION 3.16)
project ("CTL" LANGUAGES "C")


################### VARIABLES ####################

# Listings of directories
set (src_dir "${CMAKE_SOURCE_DIR}/src")
set (inc_dir "${CMAKE_SOURCE_DIR}/include")


# Listings of files
set (src_files "vector.c")
set (inc_files "vector/template.h")

set (std_files 
    "circ-buffer/standard.c"    # CIRCULAR BUFFER
    )

set (nostd_files "")


# Listings of unit test
set (test_dir "${CMAKE_SOURCE_DIR}/tests")
set (test_files "test-circ.c")

################# END VARIABLES ##################

################### DEPENDENCIES ####################

# Import dependencies for unit testing
if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    include (CTest)

    # Unit testing library libcheck
    find_package (check QUIET)
    if (NOT ${check_FOUND})
        add_subdirectory ("deps/check")
    endif ()
endif ()

################# END DEPENDENCIES ##################

################### CONFIGURATION ####################

# Type of linking for the library
set (linking SHARED CACHE STRING "Type of linking for the library")

# Type of implementation to use
option (use_std "Use the standard library implementation" ON)

# Build unit tests
option ("${PROJECT_NAME}_build_testing" "Build unit tests for CTL" ON)


# Give each source file an absolute path
list (TRANSFORM src_files PREPEND "${src_dir}/")

# Give each header file an absolute path
list (TRANSFORM inc_files PREPEND "${inc_dir}/")


# Give each standard source file an absolute path
list (TRANSFORM std_files PREPEND "${src_dir}/")

# Give each non-standard source file an absolute path
list (TRANSFORM nostd_files PREPEND "${src_dir}/")


#
# Setting compilers definition
#

# Define backend type
if (${use_std})
    add_compile_definitions ("CTL_USE_STD")
else ()
    add_compile_definitions ("CTL_USE_NOSTD")
endif ()

################# END CONFIGURATION ##################

################### TARGETS ####################

# Backend Implementations
if (${use_std})
    add_library (backend STATIC ${std_files})
else ()
    add_library (backend STATIC ${nostd_files})
endif ()

target_include_directories (backend PRIVATE ${inc_dir})


# CTL Library
add_library (${PROJECT_NAME} ${linking} ${src_files} ${inc_files})
target_include_directories (${PROJECT_NAME} PUBLIC ${inc_dir})

target_link_libraries (${PROJECT_NAME} backend)


# Data Structures implementations 
#
#       TODO
#

################# END TARGETS ##################

################### UNIT TESTS ####################

# Create individual executables and runner for each test
if ((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME OR "${PROJECT_NAME}_build_testing") AND BUILD_TESTING)
    foreach (test IN LISTS test_files)
        set (path "${test_dir}/${test}")
        set (target "${test}.run")

        # Build test executable
        add_executable ("${target}" ${path})
        target_link_libraries ("${target}" ${PROJECT_NAME} check pthread)

        # Create test runner
        add_test (NAME "${test}" COMMAND "${target}")
    endforeach ()
endif ()

################# END UNIT TESTS ##################

