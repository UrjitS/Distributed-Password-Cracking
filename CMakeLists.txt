cmake_minimum_required(VERSION 3.22)

project(dpc-client
        VERSION 0.2.1
        DESCRIPTION "Client Code for Distributed Password Cracking"
        LANGUAGES C)

set(CMAKE_C_STANDARD 17)

if (DEFINED ENV{DC_BUILD_SANITIZE})
    set(SANITIZE $ENV{DC_BUILD_SANITIZE})
else ()
    set(SANITIZE FALSE)
endif ()

# CLIENT Path
set(SOURCE_DIR_CLIENT ${PROJECT_SOURCE_DIR}/client/src)
set(INCLUDE_DIR_CLIENT ${PROJECT_SOURCE_DIR}/client/include)

# Client Files
set(SOURCE_LIST_CLIENT
        ${SOURCE_DIR_CLIENT}/client-protocol.c
)
SET(SOURCE_MAIN_CLIENT ${SOURCE_DIR_CLIENT}/main.c)
set(HEADER_LIST_CLIENT
        ${INCLUDE_DIR_CLIENT}/client-protocol.h
)

# Host Path
set(SOURCE_DIR_HOST ${PROJECT_SOURCE_DIR}/host/src)
set(INCLUDE_DIR_HOST ${PROJECT_SOURCE_DIR}/host/include)

# Host Files
set(SOURCE_LIST_HOST
        ${SOURCE_DIR_HOST}/host-protocol.c
)
SET(SOURCE_MAIN_HOST ${SOURCE_DIR_HOST}/main.c)
set(HEADER_LIST_HOST
        ${INCLUDE_DIR_HOST}/host-protocol.h
)

# Password Cracker Lib Path
set(SOURCE_DIR_LIB ${PROJECT_SOURCE_DIR}/password_cracking/src)
set(INCLUDE_DIR_LIB ${PROJECT_SOURCE_DIR}/password_cracking/include)

# Password Cracker Files
set(SOURCE_LIST_LIB
        ${SOURCE_DIR_LIB}/password-cracker.c
)
set(HEADER_LIST_LIB
        ${INCLUDE_DIR_LIB}/password-cracker.h
)

add_compile_definitions(_POSIX_C_SOURCE=200809L)
add_compile_definitions(_XOPEN_SOURCE=700)
add_compile_definitions(_GNU_SOURCE)

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    add_compile_definitions(_DARWIN_C_SOURCE)
endif ()

if (${CMAKE_SYSTEM_NAME} MATCHES "FreeBSD")
    add_compile_definitions(__BSD_VISIBLE)
endif ()

include_directories(${INCLUDE_DIR_CLIENT})
include_directories(${INCLUDE_DIR_HOST})
include_directories(${INCLUDE_DIR_LIB})
include(CheckCCompilerFlag)

function(AddCompileOptions)
    foreach(FLAG IN LISTS ARGN)
        string(REPLACE "-" "" FLAG_NO_HYPHEN ${FLAG})
        check_c_compiler_flag(${FLAG} COMPILER_SUPPORTS_${FLAG_NO_HYPHEN})
        if (COMPILER_SUPPORTS_${FLAG_NO_HYPHEN})
            add_compile_options(${FLAG})
        endif ()
    endforeach()
endfunction()

list(APPEND COMPILER_FLAGS
        "-Wall"
        "-Wextra"
        "-Wpedantic"
        "-Wshadow"
        "-Wstrict-overflow=4"
        "-Wswitch-default"
        "-Wswitch-enum"
        "-Wunused"
        "-Wunused-macros"
        "-Wdate-time"
        "-Winvalid-pch"
        "-Wmissing-declarations"
        "-Wmissing-include-dirs"
        "-Wmissing-prototypes"
        "-Wstrict-prototypes"
        "-Wundef"
        "-Wnull-dereference"
        "-Wstack-protector"
        "-Wdouble-promotion"
        "-Wvla"
        "-Walloca"
        "-Woverlength-strings"
        "-Wdisabled-optimization"
        "-Winline"
        "-Wcast-qual"
        "-Wfloat-equal"
        "-Wformat=2"
        "-Wfree-nonheap-object"
        "-Wshift-overflow"
        "-Wwrite-strings"
        "-Wunsuffixed-float-constants"
        "-Warith-conversion"
        "-Wunsafe-loop-optimizations"
        "-Wvector-operation-performance"
        "-Walloc-zero"
        "-Wtrampolines"
        "-Wtsan"
        "-Wformat-overflow=2"
        "-Wformat-signedness"
        "-Wjump-misses-init"
        "-Wformat-truncation=2")

if (${SANITIZE})
    list(APPEND COMPILER_FLAGS
            "-fsanitize=pointer-compare"
            "-fsanitize=pointer-subtract"
            "-fsanitize=undefined"
            "-fsanitize-address-use-after-scope"
            "-fcf-protection=full"
            "-fharden-control-flow-redundancy"
            "-fstack-protector=all"
            "-finstrument-functions"
            "-fsanitize=leak"
            "-fno-delete-null-pointer-checks"
            "-fno-omit-frame-pointer"
    )

    add_compile_options("-fsanitize=address")
    add_link_options("-fsanitize=address")
    add_link_options("-fsanitize=bounds")
endif ()

AddCompileOptions(${COMPILER_FLAGS})

find_package(Doxygen
        REQUIRED
        REQUIRED dot
        OPTIONAL_COMPONENTS mscgen dia)

set(DOXYGEN_ALWAYS_DETAILED_SEC YES)
set(DOXYGEN_REPEAT_BRIEF YES)
set(DOXYGEN_EXTRACT_ALL YES)
set(DOXYGEN_JAVADOC_AUTOBRIEF YES)
set(DOXYGEN_OPTIMIZE_OUTPUT_FOR_C YES)
set(DOXYGEN_GENERATE_HTML YES)
set(DOXYGEN_WARNINGS YES)
set(DOXYGEN_QUIET YES)

doxygen_add_docs(doxygen
        ${HEADER_LIST_CLIENT}
        ${HEADER_LIST_HOST}
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        COMMENT "Generating Doxygen documentation for dpc-client")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CLANG_TIDY_CHECKS "*")
set(CLANG_TIDY_CHECKS "${CLANG_TIDY_CHECKS},-altera-id-dependent-backward-branch")
set(CLANG_TIDY_CHECKS "${CLANG_TIDY_CHECKS},-altera-struct-pack-align")
set(CLANG_TIDY_CHECKS "${CLANG_TIDY_CHECKS},-altera-unroll-loops")
set(CLANG_TIDY_CHECKS "${CLANG_TIDY_CHECKS},-android-cloexec-accept")
set(CLANG_TIDY_CHECKS "${CLANG_TIDY_CHECKS},-android-cloexec-creat")
set(CLANG_TIDY_CHECKS "${CLANG_TIDY_CHECKS},-android-cloexec-dup")
set(CLANG_TIDY_CHECKS "${CLANG_TIDY_CHECKS},-android-cloexec-open")
set(CLANG_TIDY_CHECKS "${CLANG_TIDY_CHECKS},-android-cloexec-pipe")
set(CLANG_TIDY_CHECKS "${CLANG_TIDY_CHECKS},-android-cloexec-socket")
set(CLANG_TIDY_CHECKS "${CLANG_TIDY_CHECKS},-bugprone-easily-swappable-parameters")
set(CLANG_TIDY_CHECKS "${CLANG_TIDY_CHECKS},-cert-dcl03-c")
set(CLANG_TIDY_CHECKS "${CLANG_TIDY_CHECKS},-clang-analyzer-deadcode.DeadStores")
set(CLANG_TIDY_CHECKS "${CLANG_TIDY_CHECKS},-clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling")
set(CLANG_TIDY_CHECKS "${CLANG_TIDY_CHECKS},-clang-analyzer-security.insecureAPI.strcpy")
set(CLANG_TIDY_CHECKS "${CLANG_TIDY_CHECKS},-clang-diagnostic-cast-align")
set(CLANG_TIDY_CHECKS "${CLANG_TIDY_CHECKS},-clang-diagnostic-format-nonliteral")
set(CLANG_TIDY_CHECKS "${CLANG_TIDY_CHECKS},-clang-diagnostic-incompatible-pointer-types-discards-qualifiers")
set(CLANG_TIDY_CHECKS "${CLANG_TIDY_CHECKS},-clang-diagnostic-unknown-warning-option")
set(CLANG_TIDY_CHECKS "${CLANG_TIDY_CHECKS},-clang-diagnostic-unused-but-set-variable")
set(CLANG_TIDY_CHECKS "${CLANG_TIDY_CHECKS},-clang-diagnostic-unused-function")
set(CLANG_TIDY_CHECKS "${CLANG_TIDY_CHECKS},-clang-diagnostic-unused-parameter")
set(CLANG_TIDY_CHECKS "${CLANG_TIDY_CHECKS},-clang-diagnostic-unused-variable")
set(CLANG_TIDY_CHECKS "${CLANG_TIDY_CHECKS},-clang-diagnostic-vla")
set(CLANG_TIDY_CHECKS "${CLANG_TIDY_CHECKS},-cppcoreguidelines-init-variables")
set(CLANG_TIDY_CHECKS "${CLANG_TIDY_CHECKS},-google-readability-todo")
set(CLANG_TIDY_CHECKS "${CLANG_TIDY_CHECKS},-hicpp-static-assert")
set(CLANG_TIDY_CHECKS "${CLANG_TIDY_CHECKS},-llvmlibc-restrict-system-libc-headers")
set(CLANG_TIDY_CHECKS "${CLANG_TIDY_CHECKS},-misc-static-assert")
set(CLANG_TIDY_CHECKS "${CLANG_TIDY_CHECKS},-misc-unused-parameters")
set(CLANG_TIDY_CHECKS "${CLANG_TIDY_CHECKS},-readability-identifier-length")

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    if (NOT DEFINED ENV{CMDLINE_TOOLS_VER})
        message( FATAL_ERROR "The CMDLINE_TOOLS_VER environment variable must be set" )
    endif ()

    if(NOT EXISTS /Library/Developer/CommandLineTools/usr/lib/clang/$ENV{CMDLINE_TOOLS_VER})
        message( FATAL_ERROR "/Library/Developer/CommandLineTools/usr/lib/clang/$ENV{CMDLINE_TOOLS_VER} does not exist" )
    endif ()

    set(CMAKE_C_CLANG_TIDY clang-tidy -extra-arg=-I/Library/Developer/CommandLineTools/usr/lib/clang/$ENV{CMDLINE_TOOLS_VER}/include -checks=${CLANG_TIDY_CHECKS};--quiet)
else ()
    set(CMAKE_C_CLANG_TIDY clang-tidy -checks=${CLANG_TIDY_CHECKS};--quiet)
endif ()

# Add the path to the directory containing the password-cracker library
link_directories(${CMAKE_SOURCE_DIR}/password_cracking)

add_executable(dpc-client ${SOURCE_MAIN_CLIENT} ${SOURCE_LIST_CLIENT} ${HEADER_LIST_CLIENT})
add_executable(dpc-host ${SOURCE_LIST_HOST} ${SOURCE_MAIN_HOST} ${HEADER_LIST_HOST})
# Add the password cracker library
add_library(password-cracker SHARED ${SOURCE_LIST_LIB} ${HEADER_LIST_LIB})

target_include_directories(dpc-client PRIVATE include)
target_include_directories(dpc-client PRIVATE /usr/local/include)
target_link_directories(dpc-client PRIVATE /usr/local/lib)

target_include_directories(password-cracker PRIVATE include)
target_include_directories(password-cracker PRIVATE /usr/local/include)
target_link_directories(password-cracker PRIVATE /usr/local/lib)

# Link the password-cracker library to the dpc-client target
target_link_libraries(dpc-client password-cracker)

if (NOT ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    target_include_directories(dpc-client PRIVATE /usr/include)
    target_include_directories(password-cracker PRIVATE /usr/include)
endif ()

set_target_properties(dpc-client PROPERTIES
        VERSION ${PROJECT_VERSION}
        SOVERSION ${PROJECT_VERSION_MAJOR})

set_target_properties(password-cracker PROPERTIES
        VERSION ${PROJECT_VERSION}
        SOVERSION ${PROJECT_VERSION_MAJOR})

set_target_properties(dpc-client PROPERTIES OUTPUT_NAME "dpc-client")
set_target_properties(password-cracker PROPERTIES OUTPUT_NAME "password-cracker")

install(TARGETS dpc-client DESTINATION bin)
install(TARGETS password-cracker DESTINATION bin)

add_dependencies(dpc-client doxygen)
add_dependencies(password-cracker doxygen)