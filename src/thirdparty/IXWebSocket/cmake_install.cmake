# Install script for directory: /Users/timur/Documents/src/IXWebSocket

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
    set(CMAKE_INSTALL_CONFIG_NAME "")
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

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/Library/Developer/CommandLineTools/usr/bin/objdump")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/Users/timur/Documents/src/IXWebSocket/libixwebsocket.a")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libixwebsocket.a" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libixwebsocket.a")
    execute_process(COMMAND "/Library/Developer/CommandLineTools/usr/bin/ranlib" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libixwebsocket.a")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/ixwebsocket" TYPE FILE FILES
    "/Users/timur/Documents/src/IXWebSocket/ixwebsocket/IXBase64.h"
    "/Users/timur/Documents/src/IXWebSocket/ixwebsocket/IXBench.h"
    "/Users/timur/Documents/src/IXWebSocket/ixwebsocket/IXCancellationRequest.h"
    "/Users/timur/Documents/src/IXWebSocket/ixwebsocket/IXConnectionState.h"
    "/Users/timur/Documents/src/IXWebSocket/ixwebsocket/IXDNSLookup.h"
    "/Users/timur/Documents/src/IXWebSocket/ixwebsocket/IXExponentialBackoff.h"
    "/Users/timur/Documents/src/IXWebSocket/ixwebsocket/IXGetFreePort.h"
    "/Users/timur/Documents/src/IXWebSocket/ixwebsocket/IXGzipCodec.h"
    "/Users/timur/Documents/src/IXWebSocket/ixwebsocket/IXHttp.h"
    "/Users/timur/Documents/src/IXWebSocket/ixwebsocket/IXHttpClient.h"
    "/Users/timur/Documents/src/IXWebSocket/ixwebsocket/IXHttpServer.h"
    "/Users/timur/Documents/src/IXWebSocket/ixwebsocket/IXNetSystem.h"
    "/Users/timur/Documents/src/IXWebSocket/ixwebsocket/IXProgressCallback.h"
    "/Users/timur/Documents/src/IXWebSocket/ixwebsocket/IXSelectInterrupt.h"
    "/Users/timur/Documents/src/IXWebSocket/ixwebsocket/IXSelectInterruptFactory.h"
    "/Users/timur/Documents/src/IXWebSocket/ixwebsocket/IXSelectInterruptPipe.h"
    "/Users/timur/Documents/src/IXWebSocket/ixwebsocket/IXSelectInterruptEvent.h"
    "/Users/timur/Documents/src/IXWebSocket/ixwebsocket/IXSetThreadName.h"
    "/Users/timur/Documents/src/IXWebSocket/ixwebsocket/IXSocket.h"
    "/Users/timur/Documents/src/IXWebSocket/ixwebsocket/IXSocketConnect.h"
    "/Users/timur/Documents/src/IXWebSocket/ixwebsocket/IXSocketFactory.h"
    "/Users/timur/Documents/src/IXWebSocket/ixwebsocket/IXSocketServer.h"
    "/Users/timur/Documents/src/IXWebSocket/ixwebsocket/IXSocketTLSOptions.h"
    "/Users/timur/Documents/src/IXWebSocket/ixwebsocket/IXStrCaseCompare.h"
    "/Users/timur/Documents/src/IXWebSocket/ixwebsocket/IXUdpSocket.h"
    "/Users/timur/Documents/src/IXWebSocket/ixwebsocket/IXUniquePtr.h"
    "/Users/timur/Documents/src/IXWebSocket/ixwebsocket/IXUrlParser.h"
    "/Users/timur/Documents/src/IXWebSocket/ixwebsocket/IXUuid.h"
    "/Users/timur/Documents/src/IXWebSocket/ixwebsocket/IXUtf8Validator.h"
    "/Users/timur/Documents/src/IXWebSocket/ixwebsocket/IXUserAgent.h"
    "/Users/timur/Documents/src/IXWebSocket/ixwebsocket/IXWebSocket.h"
    "/Users/timur/Documents/src/IXWebSocket/ixwebsocket/IXWebSocketCloseConstants.h"
    "/Users/timur/Documents/src/IXWebSocket/ixwebsocket/IXWebSocketCloseInfo.h"
    "/Users/timur/Documents/src/IXWebSocket/ixwebsocket/IXWebSocketErrorInfo.h"
    "/Users/timur/Documents/src/IXWebSocket/ixwebsocket/IXWebSocketHandshake.h"
    "/Users/timur/Documents/src/IXWebSocket/ixwebsocket/IXWebSocketHandshakeKeyGen.h"
    "/Users/timur/Documents/src/IXWebSocket/ixwebsocket/IXWebSocketHttpHeaders.h"
    "/Users/timur/Documents/src/IXWebSocket/ixwebsocket/IXWebSocketInitResult.h"
    "/Users/timur/Documents/src/IXWebSocket/ixwebsocket/IXWebSocketMessage.h"
    "/Users/timur/Documents/src/IXWebSocket/ixwebsocket/IXWebSocketMessageType.h"
    "/Users/timur/Documents/src/IXWebSocket/ixwebsocket/IXWebSocketOpenInfo.h"
    "/Users/timur/Documents/src/IXWebSocket/ixwebsocket/IXWebSocketPerMessageDeflate.h"
    "/Users/timur/Documents/src/IXWebSocket/ixwebsocket/IXWebSocketPerMessageDeflateCodec.h"
    "/Users/timur/Documents/src/IXWebSocket/ixwebsocket/IXWebSocketPerMessageDeflateOptions.h"
    "/Users/timur/Documents/src/IXWebSocket/ixwebsocket/IXWebSocketProxyServer.h"
    "/Users/timur/Documents/src/IXWebSocket/ixwebsocket/IXWebSocketSendData.h"
    "/Users/timur/Documents/src/IXWebSocket/ixwebsocket/IXWebSocketSendInfo.h"
    "/Users/timur/Documents/src/IXWebSocket/ixwebsocket/IXWebSocketServer.h"
    "/Users/timur/Documents/src/IXWebSocket/ixwebsocket/IXWebSocketTransport.h"
    "/Users/timur/Documents/src/IXWebSocket/ixwebsocket/IXWebSocketVersion.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/ixwebsocket" TYPE FILE FILES "/Users/timur/Documents/src/IXWebSocket/ixwebsocket-config.cmake")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/ixwebsocket/ixwebsocket-targets.cmake")
    file(DIFFERENT _cmake_export_file_changed FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/ixwebsocket/ixwebsocket-targets.cmake"
         "/Users/timur/Documents/src/IXWebSocket/CMakeFiles/Export/dbc99e06a99e696141dafd40631f8060/ixwebsocket-targets.cmake")
    if(_cmake_export_file_changed)
      file(GLOB _cmake_old_config_files "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/ixwebsocket/ixwebsocket-targets-*.cmake")
      if(_cmake_old_config_files)
        string(REPLACE ";" ", " _cmake_old_config_files_text "${_cmake_old_config_files}")
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/ixwebsocket/ixwebsocket-targets.cmake\" will be replaced.  Removing files [${_cmake_old_config_files_text}].")
        unset(_cmake_old_config_files_text)
        file(REMOVE ${_cmake_old_config_files})
      endif()
      unset(_cmake_old_config_files)
    endif()
    unset(_cmake_export_file_changed)
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/ixwebsocket" TYPE FILE FILES "/Users/timur/Documents/src/IXWebSocket/CMakeFiles/Export/dbc99e06a99e696141dafd40631f8060/ixwebsocket-targets.cmake")
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^()$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/ixwebsocket" TYPE FILE FILES "/Users/timur/Documents/src/IXWebSocket/CMakeFiles/Export/dbc99e06a99e696141dafd40631f8060/ixwebsocket-targets-noconfig.cmake")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/Users/timur/Documents/src/IXWebSocket/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
