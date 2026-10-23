CMAKE_MINIMUM_REQUIRED(VERSION 4.2)

# This file is part of the `ZeroMQ-XOP` project and licensed under BSD-3-Clause.

option(WITH_LONG_NAMES "Use Long Names for Igor Pro 8" OFF)
option(MSVC_RUNTIME_DYNAMIC "Link dynamically against the MSVC runtime library" OFF)

PROJECT(MIESUtils)

IF(CMAKE_SIZEOF_VOID_P EQUAL 4)
  SET(bitness "")
  SET(bitnessWithDash "")
  SET(bitnessLibFolder "x86")
ELSEIF(CMAKE_SIZEOF_VOID_P EQUAL 8)
  SET(bitness "64")
  SET(bitnessWithDash "-64")
  SET(bitnessLibFolder "x64")
ENDIF()

SET(libname "${PROJECT_NAME}${bitnessWithDash}")

IF(NOT WIN32)
  MESSAGE(FATAL_ERROR "The MIESUtils XOP only supports Windows.")
ENDIF()

SET(RESOURCES "${PROJECT_NAME}.rc")
SET(installFolder "${CMAKE_SOURCE_DIR}/../output/win/${bitnessLibFolder}")

SET(SOURCES
  MIESUtils.cpp
  Helpers.cpp
  functions.cpp
  CustomExceptions.cpp
)

SET(HEADERS
  MIESUtils.h
  Helpers.h
  functions.h
  CustomExceptions.h
)

SOURCE_GROUP("Headers" FILES ${HEADERS})

IF(WITH_LONG_NAMES)
  ADD_DEFINITIONS(-DXOP_LONG_NAMES_AND_PATHS=1)
ENDIF()

INCLUDE_DIRECTORIES(../XOPSupport .)

ADD_LIBRARY(${libname} SHARED ${SOURCES} ${SOURCES_EXT} ${HEADERS} ${RESOURCES})

SET_TARGET_PROPERTIES(${libname} PROPERTIES SUFFIX ".xop")

# Link statically against the MSVC runtime by default, so that the XOP does not
# depend on the installed version of the Visual C++ Redistributable
SET_PROPERTY(TARGET ${libname} PROPERTY MSVC_RUNTIME_LIBRARY MultiThreaded$<$<CONFIG:Debug>:Debug>$<$<BOOL:${MSVC_RUNTIME_DYNAMIC}>:DLL>)

TARGET_LINK_LIBRARIES(${libname} version.lib
                      ${CMAKE_SOURCE_DIR}/../XOPSupport/IGOR${bitness}.lib
                      ${CMAKE_SOURCE_DIR}/../XOPSupport/VC/XOPSupport${bitness}.lib)

# Use parallel builds
IF(MSVC)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Zi")
  set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} /DEBUG /OPT:REF /OPT:ICF")
ENDIF()

# run clang-format before building
ADD_CUSTOM_COMMAND(TARGET ${libname}
                   PRE_BUILD
                   COMMAND clang-format -i ${SOURCES} ${HEADERS}
                   WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                   COMMENT "Running clang-format" VERBATIM)

FIND_PROGRAM(CLANG-TIDY clang-tidy)

ADD_CUSTOM_TARGET(clang-tidy
                  COMMAND ${CLANG-TIDY} -fix -p=${CMAKE_BINARY_DIR} -checks=* ${SOURCES}
                  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                  COMMENT "Running clang-tidy" VERBATIM)

IF(MSVC)
  INSTALL(TARGETS ${libname}
          RUNTIME
          DESTINATION ${installFolder})

  INSTALL(FILES ${CMAKE_BINARY_DIR}/Release/${libname}.pdb
          DESTINATION ${installFolder}
          CONFIGURATIONS Release)
ENDIF()
