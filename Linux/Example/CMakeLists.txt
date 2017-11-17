
cmake_minimum_required(VERSION 2.8)
PROJECT(Example)

include_directories( ${Example_SOURCE_DIR}/../../include )

set( sourceRootPath ${Example_SOURCE_DIR}/../../Example )


set( sourceFile 
	Main.cpp
	Log/ExampleLog.cpp
	Lua/BindClass.cpp
	Lua/BindInherit.cpp
	Lua/BindStatic.cpp
	Lua/ExampleLua.cpp
	MsgPack/CompressEncrypt.cpp
	MsgPack/ExampleMsgPack.cpp
	MsgPack/PackClass.cpp
	MsgPack/PackType.cpp
)
foreach(k ${sourceFile} )
  set( fullSourcePath ${fullSourcePath} ${sourceRootPath}/${k} )
endforeach(k)


if (CMAKE_BUILD_TYPE STREQUAL Release)
	add_definitions("-DNDEBUG")
else ()
	add_definitions("-D_DEBUG")
	set( CMAKE_BUILD_TYPE Debug )
endif ()

add_definitions("-DLIGHTINK_AS_DLL")

add_executable( ${PROJECT_NAME} ${fullSourcePath} )

add_dependencies(${PROJECT_NAME} LightInkLLM)

target_link_libraries(${PROJECT_NAME} LightInkLLM )

if (CMAKE_BUILD_TYPE STREQUAL Debug)
set(EXECUTABLE_OUTPUT_PATH ${Example_SOURCE_DIR}/../../bin/Debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb -W -pipe -fPIC")
else()
set(EXECUTABLE_OUTPUT_PATH ${Example_SOURCE_DIR}/../../bin/Release)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -W -pipe -fPIC")
endif()


unset(sourceFile)
unset(fullSourcePath)
unset(sourceRootPath)


