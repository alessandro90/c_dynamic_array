
cmake_minimum_required (VERSION 3.21.0)

add_compile_options(
    -Wall
    -Wextra
    -Wpedantic
    -Wdouble-promotion
    -Wformat=2
    -Wformat-security
    -Wformat-signedness
    -Wnull-dereference
    -Wtrivial-auto-var-init
    -Wunused-parameter
    -Wunused-const-variable=2
    -Wuninitialized
    -Wmaybe-uninitialized
    -Wstringop-overflow=4
    -Wconversion
    -Wfloat-conversion
    -Wsign-conversion
    -Warith-conversion
    -Wbool-compare
    -Wduplicated-branches
    -Wduplicated-cond
    -Wfloat-equal
    -Wshadow
    -Wundef
    -Wunused-macros
    -Wcast-qual
    -Wcast-align=strict
    -Wlogical-op
    -Wmissing-declarations
    -Wredundant-decls
    # -Winline
    -Wlong-long
    -Wimplicit-fallthrough=5
    -Wmissing-include-dirs
)

set(
    SRC_FILES
    main.c
)

include(CheckIPOSupported)
check_ipo_supported(RESULT supported OUTPUT error)

add_executable(vector_test ${SRC_FILES})

add_test(NAME "vector_test" COMMAND vector_test)

if( supported )
    message(STATUS "IPO / LTO enabled")
    set_property(TARGET vector_test PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
else()
    message(STATUS "IPO / LTO not supported: <${error}>")
endif()
