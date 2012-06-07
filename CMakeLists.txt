PROJECT(The-Dark-Crystal)
cmake_minimum_required(VERSION 2.8)


set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif()

if(WIN32)
    set(Boost_USE_STATIC_LIBS ON)
else()
    set(Boost_USE_STATIC_LIBS OFF)
endif()
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

set(ENV{QTDIR} "${PROJECT_SOURCE_DIR}/libs/qt4")
file(WRITE "${PROJECT_SOURCE_DIR}/libs/qt4/bin/qt.conf" "[Paths]\nPrefix = ${PROJECT_SOURCE_DIR}/libs/qt4")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
file(GLOB_RECURSE DLLS_LIST ${PROJECT_SOURCE_DIR}/libs/*.dll)
file(GLOB_RECURSE LIBS_LIST ${PROJECT_SOURCE_DIR}/libs/*.lib)

MESSAGE(STATUS "RJ ${DLLS_LIST}")

file(COPY ${DLLS_LIST} DESTINATION "${PROJECT_SOURCE_DIR}/bin/Debug")
file(COPY ${DLLS_LIST} DESTINATION "${PROJECT_SOURCE_DIR}/bin/Release")

#dependencies' header
include_directories(${PROJECT_SOURCE_DIR}/libs/boost
					${PROJECT_SOURCE_DIR}/libs/btogre/include
					${PROJECT_SOURCE_DIR}/libs/bullet/src					
					${PROJECT_SOURCE_DIR}/libs/ogre/include/OGRE
					${PROJECT_SOURCE_DIR}/libs/ogre/include/OIS
					${PROJECT_SOURCE_DIR}/libs/ogre-procedural/library/include
					${PROJECT_SOURCE_DIR}/libs/ogre-paged/include
					${PROJECT_SOURCE_DIR}/libs/qt4/include
					${PROJECT_SOURCE_DIR}/libs/sfml/include
					${PROJECT_SOURCE_DIR}/libs/yaml-cpp/include
					${PROJECT_SOURCE_DIR}/libs/mygui/MyGUIEngine/include
					${PROJECT_SOURCE_DIR}/libs/mygui/Common
					${PROJECT_SOURCE_DIR}/libs/mygui/Platforms/Ogre/OgrePlatform/include
					${PROJECT_SOURCE_DIR}/libs/mygui/Platforms/DirectX/DirectXPlatform/include
					${PROJECT_SOURCE_DIR}/libs/mygui/Platforms/OpenGL/OpenGLPlatform/include
					)

include_directories(${PROJECT_SOURCE_DIR}/libs/ducttape/include)
include_directories(${PROJECT_SOURCE_DIR}/src/main)

# Qt4 stuff
set(QT_USE_QTSCRIPT TRUE)
set(QT_DONT_USE_QTGUI TRUE)
set(QT_USE_QTXML TRUE)
find_package(Qt4 REQUIRED)
include(${QT_USE_FILE})
MESSAGE(STATUS "QT_USE_FILE ${QT_USE_FILE}")
add_definitions(${QT_DEFINITONS})


if(MSVC) # Sort the odd stuff out
    set(CMAKE_CXX_FLAGS "/DCOMPILER_MSVC /DOgreProcedural_STATIC /WX /MP /EHsc")
    set(CMAKE_CXX_FLAGS_RELEASE "/O2 /DNDEBUG") # Odd duplicate symbols with /GL
    set(CMAKE_CXX_FLAGS_DEBUG "/Zi")
else() # GCC/Mingw
    set(CMAKE_CXX_FLAGS "-DCOMPILER_GCC -Wall -Wextra -Werror -Wno-unused-parameter -Wno-unused-but-set-parameter -pedantic -std=c++0x -fvisibility=hidden -fPIC")
    set(CMAKE_CXX_FLAGS_RELEASE "-O2 -flto -DNDEBUG")
    set(CMAKE_CXX_FLAGS_DEBUG "-g")
endif()


#add app

#add_subdirectory(simplepong)



MESSAGE(STATUS "This is The-Dark-Crystal")
aux_source_directory(src/main main_CPP)
file(GLOB_RECURSE main_HPP src/main/*.hpp)
file(GLOB_RECURSE main_H src/main/*.h)
qt4_wrap_cpp(main_HEADERS_MOC ${main_HPP} ${main_H})

MESSAGE(STATUS "This is MOC${main_HEADERS_MOC}")
ADD_EXECUTABLE(The-Dark-Crystal ${main_CPP} ${main_HEADERS_MOC})

target_link_libraries(The-Dark-Crystal ${LIBS_LIST})



MESSAGE(STATUS "This is TEST")
aux_source_directory(src/test TEST_CPP)
file(GLOB_RECURSE TEST_HPP src/test/*.hpp)
file(GLOB_RECURSE TEST_H src/test/*.h)

qt4_wrap_cpp(TEST_HEADERS_MOC ${TEST_HPP} ${TEST_H})

MESSAGE(STATUS "This is MOC${TEST_HEADERS_MOC}")
ADD_EXECUTABLE(TEST ${TEST_CPP} ${TEST_HEADERS_MOC} ${main_CPP} ${main_HEADERS_MOC})

target_link_libraries(TEST ${LIBS_LIST})






