# Set VCPKG_ROOT to your vcpkg installation directory or add the following to your cmake call:
# -DCMAKE_TOOLCHAIN=<path_to_vcpkg>/scripts/buildsystems/vcpkg.cmake

cmake_minimum_required(VERSION 3.16)
set(CMAKE_VERBOSE_MAKEFILE ON)

if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")  
endif()
if(DEFINED ENV{VCPKG_DEFAULT_TRIPLET} AND NOT DEFINED VCPKG_TARGET_TRIPLET)
  set(VCPKG_TARGET_TRIPLET "$ENV{VCPKG_DEFAULT_TRIPLET}" CACHE STRING "")
endif()

project(cpp-20 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

function(add_sanitizer flag)
  include(CheckCXXCompilerFlag)
  if (CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
      message("Looking for -fsanitize=${flag}")
      set(CMAKE_REQUIRED_FLAGS "-Werror -fsanitize=${flag}")
      check_cxx_compiler_flag(-fsanitize=${flag} HAVE_FLAG_SANITIZER)
      if (HAVE_FLAG_SANITIZER)
          message("Adding -fsanitize=${flag}")
          set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=${flag} -fno-omit-frame-pointer")
          set(DCMAKE_C_FLAGS "${DCMAKE_C_FLAGS} -fsanitize=${flag} -fno-omit-frame-pointer")
          set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=${flag}")
          set(DCMAKE_MODULE_LINKER_FLAGS "${DCMAKE_MODULE_LINKER_FLAGS} -fsanitize=${flag}")
      else ()
          message("-fsanitize=${flag} unavailable")
      endif ()
  endif ()
endfunction()

add_sanitizer("address")
add_sanitizer("thread")


# set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
# set (CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")

find_package(Catch2 3)

if(NOT Catch2_FOUND)
  Include(FetchContent)

  FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        v3.2.1 # or a later release
  )
  FetchContent_MakeAvailable(Catch2)
endif()

enable_testing()

add_subdirectory(basic-types)
add_subdirectory(containers-and-iterators)
add_subdirectory(memory-management)
add_subdirectory(enums)
add_subdirectory(functions)
add_subdirectory(oop)
add_subdirectory(polymorphism)
add_subdirectory(type-deduction)
add_subdirectory(vector)
add_subdirectory(move-semantics)
add_subdirectory(optional-variant)

add_subdirectory(_exercises/ex_primes)
add_subdirectory(_exercises/ex_palindromes)
add_subdirectory(_exercises/ex_vector2d)