SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS On)

SET(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS ON)

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(libscript_lua)

#########################################################################################
## GLOBAL VARIABLE
#########################################################################################

SET(PROJECT_DIR ${CMAKE_SOURCE_DIR})

SET(DEPENDENCIES_PATH ${PROJECT_SOURCE_DIR}/dependencies)

SET(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin/)

SET(CMAKE_DEBUG_POSTFIX "d" CACHE STRING "add a postfix, usually d on windows")
SET(CMAKE_RELEASE_POSTFIX "" CACHE STRING "add a postfix, usually empty on windows")
SET(CMAKE_RELWITHDEBINFO_POSTFIX "" CACHE STRING "add a postfix, usually empty on windows")
SET(CMAKE_MINSIZEREL_POSTFIX "" CACHE STRING "add a postfix, usually empty on windows")

#########################################################################################
## COMPILER OPTIONS
#########################################################################################

IF(APPLE)
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lpthread")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
ELSEIF(MSVC)
    #ADD_DEFINITIONS(-DUNICODE -D_UNICODE)
    ADD_DEFINITIONS(-D_WINDOWS)
    ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /GR")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHa")
    SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /DELAYLOAD:advapi32.dll")
    SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /DELAYLOAD:psapi.dll")
    SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /DELAYLOAD:version.dll")
    SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /DELAYLOAD:dbghelp.dll")
    SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /DELAY:unload")
    SET(COMPILER_NAME "vc")
    IF(MSVC_VERSION GREATER 1600)
        SET(COMPILER_NAME "11")
    ELSEIF(MSVC_VERSION GREATER 1500)
        SET(COMPILER_NAME "10")
    ELSEIF(MSVC_VERSION GREATER 1400)
        SET(COMPILER_NAME "9")
    ENDIF()
ENDIF()

#########################################################################################
## MACROS
#########################################################################################

MACRO(ADD_DEPENDENCIES_LIB libname)
    TARGET_LINK_LIBRARIES(${LIB_NAME} ${libname})
    INCLUDE_DIRECTORIES(${DEPENDENCIES_PATH}/${libname})
ENDMACRO(ADD_DEPENDENCIES_LIB)

#########################################################################################
## LINK PATHS
#########################################################################################

LINK_DIRECTORIES(${LIBRARY_OUTPUT_PATH})

ADD_SUBDIRECTORY(dependencies)
ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(sample)