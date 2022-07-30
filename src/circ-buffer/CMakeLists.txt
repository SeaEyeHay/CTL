cmake_minimum_required (VERSION 3.16)
project ("circular-buffer" LANGUAGES "C")


#################### VARIABLES ####################

# Standard C implmentation
set (std_cbuf_files "standard.c")

# OS specifique implementations
set (cbuf_files )

################## END VARIABLES ##################

#################### CONFIGURATION ####################

option (use_std_cbuf "Use the standard implementation of the circular buffer" ON)

if (${use_std_cbuf})
    set (cbuf_files "${std_cbuf_files}")
endif ()

################## END CONFIGURATION ##################

#################### TARGET ####################

add_library (cbuf STATIC ${cbuf_files})
target_include_directories (cbuf PRIVATE ${inc_dir})

################## END TARGET ##################
