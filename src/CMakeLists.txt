project(TickTockController)

cmake_minimum_required(VERSION 2.6)
get_filename_component (ROOT_DIR "${CMAKE_SOURCE_DIR}../../.." ABSOLUTE)
get_filename_component (PROJ_DIR "${CMAKE_SOURCE_DIR}../.." ABSOLUTE)

# Include plexil source
set(PLEXIL_INCLUDE "${PROJ_DIR}/plexil/include")
set(PLEXIL_HOME "$ENV{PLEXIL_HOME}")
set(PLEXIL_LIBRARIES libPlexilCombo.a rt dl)

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
    message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

find_package (Threads  REQUIRED)

# If we have PLEXIL installed, build the .so
link_directories("${PROJ_DIR}/plexil/lib")

add_executable(TickTockController main.cpp TickTockAdapter.cpp)

INCLUDE(CPack)

include_directories(
  ${CMAKE_CURRENT_BINARY_DIR}
  ${PLEXIL_INCLUDE}
  )

target_link_libraries(TickTockController LINK_PUBLIC ${PLEXIL_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} )
