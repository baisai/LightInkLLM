
cmake_minimum_required(VERSION 2.8)

project (Example)

include_directories( ${Example_SOURCE_DIR}/../../include )
set( sourceRootPath ${Example_SOURCE_DIR}/../../Example )


file(GLOB ExampleSrc ${sourceRootPath}/*.cpp)
list(APPEND fullSourcePath ${ExampleSrc})


file(GLOB ExampleSrc_Log ${sourceRootPath}/Log/*.cpp)
list(APPEND fullSourcePath ${ExampleSrc_Log})


file(GLOB ExampleSrc_Lua ${sourceRootPath}/Lua/*.cpp)
list(APPEND fullSourcePath ${ExampleSrc_Lua})


file(GLOB ExampleSrc_MsgPack ${sourceRootPath}/MsgPack/*.cpp)
list(APPEND fullSourcePath ${ExampleSrc_MsgPack})


file(GLOB ExampleSrc_Socket ${sourceRootPath}/Socket/*.cpp)
list(APPEND fullSourcePath ${ExampleSrc_Socket})

if (NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Debug)
endif()
set(BuildTylpeValid FALSE)
foreach(var Debug Release RelWitchDebInfo MinSizeRel)
	if(CMAKE_BUILD_TYPE MATCHES ${var})
		set(BuildTylpeValid TRUE)
	endif()
endforeach()
if (NOT BuildTylpeValid)
	message(FATAL_ERROR "unknow build type " ${CMAKE_BUILD_TYPE})
endif()
unset(BuildTylpeValid)


add_definitions("-DLIGHTINK_AS_DLL")

if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

add_executable( ${PROJECT_NAME} ${fullSourcePath} )

add_dependencies(${PROJECT_NAME} LightInkLLMT)

target_link_libraries(${PROJECT_NAME} LightInkLLMT )

source_group(Example FILES ${ExampleSrc})
unset(ExampleSrc)
source_group(Log FILES ${ExampleSrc_Log})
unset(ExampleSrc_Log)
source_group(Lua FILES ${ExampleSrc_Lua})
unset(ExampleSrc_Lua)
source_group(MsgPack FILES ${ExampleSrc_MsgPack})
unset(ExampleSrc_MsgPack)
source_group(Socket FILES ${ExampleSrc_Socket})
unset(ExampleSrc_Socket)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(EXECUTABLE_OUTPUT_PATH ${Example_SOURCE_DIR}/../../bin)
if (NOT WIN32)
	set(EXECUTABLE_OUTPUT_PATH ${EXECUTABLE_OUTPUT_PATH}/${CMAKE_BUILD_TYPE})
endif()

unset(fullSourcePath)
unset(sourceRootPath)


