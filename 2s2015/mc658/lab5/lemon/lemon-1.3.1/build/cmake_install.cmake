# Install script for directory: /home/marceloazevedo/Unicamp/unicamp/2s2015/mc658/lab4/lemon/lemon-1.3.1

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/lemon/cmake" TYPE FILE FILES "/home/marceloazevedo/Unicamp/unicamp/2s2015/mc658/lab4/lemon/lemon-1.3.1/build/cmake/LEMONConfig.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/home/marceloazevedo/Unicamp/unicamp/2s2015/mc658/lab4/lemon/lemon-1.3.1/build/lemon/cmake_install.cmake")
  include("/home/marceloazevedo/Unicamp/unicamp/2s2015/mc658/lab4/lemon/lemon-1.3.1/build/contrib/cmake_install.cmake")
  include("/home/marceloazevedo/Unicamp/unicamp/2s2015/mc658/lab4/lemon/lemon-1.3.1/build/demo/cmake_install.cmake")
  include("/home/marceloazevedo/Unicamp/unicamp/2s2015/mc658/lab4/lemon/lemon-1.3.1/build/tools/cmake_install.cmake")
  include("/home/marceloazevedo/Unicamp/unicamp/2s2015/mc658/lab4/lemon/lemon-1.3.1/build/doc/cmake_install.cmake")
  include("/home/marceloazevedo/Unicamp/unicamp/2s2015/mc658/lab4/lemon/lemon-1.3.1/build/test/cmake_install.cmake")

endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/home/marceloazevedo/Unicamp/unicamp/2s2015/mc658/lab4/lemon/lemon-1.3.1/build/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
