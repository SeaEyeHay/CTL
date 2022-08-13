cmake_minimum_required (VERSION 3.16)
project ("benchmarks")


################### VECTOR ####################

# C++ STL ground
add_executable (vector_stl "vector/stl.cpp")

# C CTL test
add_executable (vector_ctl "vector/ctl.c")
target_link_libraries (vector_ctl CTL)

################# END VECTOR ##################

