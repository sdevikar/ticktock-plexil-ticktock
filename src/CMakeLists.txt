project(TickTockController)

cmake_minimum_required(VERSION 2.6)

# Set some variables here that will be used later below.
# For this to work, the ENV variables must be properly set as
# mentioned in PLEXIL documentation.
set(PLEXIL_HOME "$ENV{PLEXIL_HOME}")
set(PLEXIL_INCLUDE "$ENV{PLEXIL_HOME}/include")



# These are the required libraries
# All the .so shared libraries are generated when Plexil source code is built
# In the had-ssc project, all these shared libraries were combined into one libPlexilCombo.a
# (for portability)
# Here, I have listed all the shared libraries that are generated when Plexil is built
# however, there is a better way to do this. In particular, we can have CMake find the
# Plexil module using find_package command. However, we will have to write findPlexil.cmake
# script for that and it's too much work. So for demonstration purposes, this should suffice
set(PLEXIL_LIBRARIES
  rt
  dl
  libGanttListener.so
  libLauncher.so
  libPlexilAppFramework.so
  libPlexilIntfc.so
  libPlexilValue.so
  libSampleAdapter.so
  libUdpUtils.so
  libIpcAdapter.so
  libLuvListener.so
  libPlexilExec.so
  libPlexilSockets.so
  libPlexilXmlParser.so
  libstandalonesimulator.so
  libIpcUtils.so
  libPlanDebugListener.so
  libPlexilExpr.so
  libPlexilUtils.so
  libpugixml.so
  libUdpAdapter.so
)


# Check whether C++11 is supported and set compiler flags accordingly
# This is standard boilerplate stuff.
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



# Find the required packages here. Threads package is required
# As mentioned earlier, we could also do find_package(Plexil REQUIRED) here
find_package (Threads  REQUIRED)



# This is the path where CMake will look for the required shared libraries
link_directories("$ENV{PLEXIL_HOME}/lib")



# This is generating TickTockController executable from the source code
add_executable(TickTockController main.cpp TickTockAdapter.cpp)


# We are specifying the path to header files here
include_directories(
  ${CMAKE_CURRENT_BINARY_DIR}
  ${PLEXIL_INCLUDE}
  )



# Finally, after compilation, when linking happens, TickTockController will be
# linked against these libraries.
# e.g. PLEXIL_LIBRARIES is an array that was set earlier above
target_link_libraries(TickTockController LINK_PUBLIC ${PLEXIL_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} )
