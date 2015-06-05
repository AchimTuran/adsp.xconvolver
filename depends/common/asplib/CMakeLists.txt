project(asplib)

cmake_minimum_required(VERSION 2.8)

set(asplib_NAME asplib)
set(asplib_DESCRIPTION "asplib (Achim's Signal Processing LIBrary), is a small and lightweight C++ library for digital signal processing.")
set(asplib_VERSION_MAJOR ${APP_VERSION_MAJOR})
set(asplib_VERSION_MINOR ${APP_VERSION_MINOR})

set(SOURCES Biquads/apslib_BiquadFactory.cpp
			Biquads/Biquad_Native/asplib_Biquad_Native.cpp)

if(WIN32)
  add_definitions(-DWIN32 -D_LIB)
endif()

add_library(asplib ${SOURCES})

include_directories(${PROJECT_SOURCE_DIR}
					${PROJECT_SOURCE_DIR}/Biquads)

set(HEADERS ${PROJECT_SOURCE_DIR}/Biquads/apslib_BiquadFactory.h
            ${PROJECT_SOURCE_DIR}/Biquads/Biquad_Native/asplib_Biquad_Native.h
			${PROJECT_SOURCE_DIR}/constants_typedefs/asplib_constants.h
			${PROJECT_SOURCE_DIR}/constants_typedefs/asplib_typedefs.h
			${PROJECT_SOURCE_DIR}/interfaces/asplib_IBaseBiquad.h)

install(FILES ${HEADERS} DESTINATION ${CMAKE_INSTALL_PREFIX}/include/asplib)
install(TARGETS asplib DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
