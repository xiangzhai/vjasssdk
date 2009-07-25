# Install script for directory: /home/tamino/Dokumente/Projekte/Informatik/C++/vjasssdk/vjassdoc/src

# Set the install prefix
IF(NOT DEFINED CMAKE_INSTALL_PREFIX)
  SET(CMAKE_INSTALL_PREFIX "")
ENDIF(NOT DEFINED CMAKE_INSTALL_PREFIX)
STRING(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
IF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  IF(BUILD_TYPE)
    STRING(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  ELSE(BUILD_TYPE)
    SET(CMAKE_INSTALL_CONFIG_NAME "")
  ENDIF(BUILD_TYPE)
  MESSAGE(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
ENDIF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)

# Set the component getting installed.
IF(NOT CMAKE_INSTALL_COMPONENT)
  IF(COMPONENT)
    MESSAGE(STATUS "Install component: \"${COMPONENT}\"")
    SET(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  ELSE(COMPONENT)
    SET(CMAKE_INSTALL_COMPONENT)
  ENDIF(COMPONENT)
ENDIF(NOT CMAKE_INSTALL_COMPONENT)

# Install shared libraries without execute permission?
IF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  SET(CMAKE_INSTALL_SO_NO_EXE "1")
ENDIF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" MATCHES "^(Unspecified)$")
  FILE(INSTALL DESTINATION "/usr/include/vjassdoc" TYPE FILE FILES
    "/home/tamino/Dokumente/Projekte/Informatik/C++/vjasssdk/vjassdoc/src/object.h"
    "/home/tamino/Dokumente/Projekte/Informatik/C++/vjasssdk/vjassdoc/src/comment.h"
    "/home/tamino/Dokumente/Projekte/Informatik/C++/vjasssdk/vjassdoc/src/keyword.h"
    "/home/tamino/Dokumente/Projekte/Informatik/C++/vjasssdk/vjassdoc/src/key.h"
    "/home/tamino/Dokumente/Projekte/Informatik/C++/vjasssdk/vjassdoc/src/textmacro.h"
    "/home/tamino/Dokumente/Projekte/Informatik/C++/vjasssdk/vjassdoc/src/textmacroinstance.h"
    "/home/tamino/Dokumente/Projekte/Informatik/C++/vjasssdk/vjassdoc/src/type.h"
    "/home/tamino/Dokumente/Projekte/Informatik/C++/vjasssdk/vjassdoc/src/local.h"
    "/home/tamino/Dokumente/Projekte/Informatik/C++/vjasssdk/vjassdoc/src/global.h"
    "/home/tamino/Dokumente/Projekte/Informatik/C++/vjasssdk/vjassdoc/src/member.h"
    "/home/tamino/Dokumente/Projekte/Informatik/C++/vjasssdk/vjassdoc/src/functioninterface.h"
    "/home/tamino/Dokumente/Projekte/Informatik/C++/vjasssdk/vjassdoc/src/function.h"
    "/home/tamino/Dokumente/Projekte/Informatik/C++/vjasssdk/vjassdoc/src/method.h"
    "/home/tamino/Dokumente/Projekte/Informatik/C++/vjasssdk/vjassdoc/src/parameter.h"
    "/home/tamino/Dokumente/Projekte/Informatik/C++/vjasssdk/vjassdoc/src/implementation.h"
    "/home/tamino/Dokumente/Projekte/Informatik/C++/vjasssdk/vjassdoc/src/hook.h"
    "/home/tamino/Dokumente/Projekte/Informatik/C++/vjasssdk/vjassdoc/src/interface.h"
    "/home/tamino/Dokumente/Projekte/Informatik/C++/vjasssdk/vjassdoc/src/struct.h"
    "/home/tamino/Dokumente/Projekte/Informatik/C++/vjasssdk/vjassdoc/src/module.h"
    "/home/tamino/Dokumente/Projekte/Informatik/C++/vjasssdk/vjassdoc/src/scope.h"
    "/home/tamino/Dokumente/Projekte/Informatik/C++/vjasssdk/vjassdoc/src/library.h"
    "/home/tamino/Dokumente/Projekte/Informatik/C++/vjasssdk/vjassdoc/src/sourcefile.h"
    "/home/tamino/Dokumente/Projekte/Informatik/C++/vjasssdk/vjassdoc/src/doccomment.h"
    "/home/tamino/Dokumente/Projekte/Informatik/C++/vjasssdk/vjassdoc/src/syntaxerror.h"
    "/home/tamino/Dokumente/Projekte/Informatik/C++/vjasssdk/vjassdoc/src/file.h"
    "/home/tamino/Dokumente/Projekte/Informatik/C++/vjasssdk/vjassdoc/src/parser.h"
    "/home/tamino/Dokumente/Projekte/Informatik/C++/vjasssdk/vjassdoc/src/compiler.h"
    "/home/tamino/Dokumente/Projekte/Informatik/C++/vjasssdk/vjassdoc/src/vjassdoc.h"
    "/home/tamino/Dokumente/Projekte/Informatik/C++/vjasssdk/vjassdoc/src/utilities.h"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" MATCHES "^(Unspecified)$")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" MATCHES "^(Unspecified)$")
  FILE(INSTALL DESTINATION "/usr/lib" TYPE FILE FILES "/home/tamino/Dokumente/Projekte/Informatik/C++/vjasssdk/vjassdoc/src/libvjassdoclib.so")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" MATCHES "^(Unspecified)$")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" MATCHES "^(Unspecified)$")
  FILE(INSTALL DESTINATION "/usr/bin" TYPE FILE FILES "/home/tamino/Dokumente/Projekte/Informatik/C++/vjasssdk/vjassdoc/src/vjassdoc")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" MATCHES "^(Unspecified)$")

