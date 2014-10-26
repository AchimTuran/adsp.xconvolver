project(adsp.template)

cmake_minimum_required(VERSION 2.6)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR})

enable_language(CXX)

find_package(xbmc REQUIRED)
#find_package(xbmcplatform REQUIRED)
#find_package(SampleRate REQUIRED)
#find_package(FFTW3 REQUIRED)

#include_directories(${xbmcplatform_INCLUDE_DIRS}
#                    ${XBMC_INCLUDE_DIR}
#                    ${FFTW3_INCLUDE_DIRS}
#                    ${SAMPLERATE_INCLUDE_DIRS}
#                    ${PROJECT_SOURCE_DIR}/src)
					
include_directories(${XBMC_INCLUDE_DIR}
					${XBMC_INCLUDE_DIR}/xbmc
                    ${PROJECT_SOURCE_DIR}/src
					${PROJECT_SOURCE_DIR}/src/platform/windows)
message("Test message start!")
message(STATUS ${XBMC_INCLUDE_DIR})
message("Test message end!")
					
set(TEMPLATE_SOURCES src/channelUpmixer.cpp
                     src/DSPProcessor.cpp
                     src/Dialogs/GUIDialogMasterProcess.cpp
                     src/Dialogs/GUIDialogOutputResample.cpp
                     src/Dialogs/GUIDialogPreProcess.cpp
                     src/Dialogs/GUIDialogPostProcess.cpp
                     src/ADDONOptional.cpp
                     src/template/client.cpp
                     src/template/GUIDialogBase.cpp
                     src/template/IADSPProcessor.cpp
                     src/template/ADSPProcessorHandle.cpp
                     src/template/IADDONOptional.cpp
                     src/template/configuration/templateConfiguration.cpp
                     src/template/ADSPAddonHandler.cpp
					 src/platform/windows/dlfcn-win32.cpp)

#set(DEPLIBS ${xbmcplatform_LIBRARIES}
#            ${FFTW3_LIBRARIES}
#            ${SAMPLERATE_LIBRARIES})

set(DEPLIBS)

build_addon(adsp.template TEMPLATE DEPLIBS)

# For generating the doxy
find_package(Doxygen)
if(DOXYGEN_FOUND)
  add_custom_target(doc ${DOXYGEN_EXECUTABLE} ${PROJECT_SOURCE_DIR}/doxygen_resources/Doxyfile
                    WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
                    COMMENT "Generating API documentation" VERBATIM)
endif()

include(CPack)
