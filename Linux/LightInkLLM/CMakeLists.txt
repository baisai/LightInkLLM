
cmake_minimum_required(VERSION 2.8)
option(LIGHTINK_LUAJIT "use lua jit" OFF)

project(LightInkLLM)

include_directories( ${LightInkLLM_SOURCE_DIR}/../../include )

set( sourceRootPath ${LightInkLLM_SOURCE_DIR}/../../src )

set( middlePath Common )
set( sourceFile 
	ArgParser.cpp
	LightInkVersion.cpp
	OsHelper.cpp
	RuntimeError.cpp
	SmallObject.cpp
	WinSnprintf.c
)
foreach(k ${sourceFile} )
  set( fullSourcePath ${fullSourcePath} ${sourceRootPath}/${middlePath}/${k} )
endforeach(k)

set( middlePath Log )
set( sourceFile
	LogFormat.cpp
	Logger.cpp
	LogOption.cpp
	LoggerMgr.cpp
	LogThreadType.cpp
	channel/DateNameStrategy.cpp
	channel/LogChannelAndroid.cpp
	channel/LogChannelStdColor.cpp
	helper/FileHelper.cpp
	helper/FormatHelper.cpp
	async/AsyncMsg.cpp
	async/AsyncThread.cpp
)
foreach(k ${sourceFile} )
  set( fullSourcePath ${fullSourcePath} ${sourceRootPath}/${middlePath}/${k} )
endforeach(k)

set( middlePath LuaEngine )
if (LIGHTINK_LUAJIT)
set( sourceFile
	LuaEngine.cpp
	LuaIndexMetatable.cpp
	LuaInt64.cpp
	LuaIterator.cpp
	LuaModule.cpp
	LuaRef.cpp
	LuaTableRef.cpp
	)
else ()
set( sourceFile
	LuaEngine.cpp
	LuaIndexMetatable.cpp
	LuaInt64.cpp
	LuaIterator.cpp
	LuaModule.cpp
	LuaRef.cpp
	LuaTableRef.cpp
	lua/lapi.c
	lua/lauxlib.c
	lua/lbaselib.c
	lua/lcode.c
	lua/ldblib.c
	lua/ldebug.c
	lua/ldo.c
	lua/ldump.c
	lua/lfunc.c
	lua/lgc.c
	lua/linit.c
	lua/liolib.c
	lua/llex.c
	lua/lmathlib.c
	lua/lmem.c
	lua/loadlib.c
	lua/lobject.c
	lua/lopcodes.c
	lua/loslib.c
	lua/lparser.c
	lua/lstate.c
	lua/lstring.c
	lua/lstrlib.c
	lua/ltable.c
	lua/ltablib.c
	lua/ltm.c
	lua/lundump.c
	lua/lvm.c
	lua/lzio.c
	lua/print.c
	)
endif()

foreach(k ${sourceFile} )
  set( fullSourcePath ${fullSourcePath} ${sourceRootPath}/${middlePath}/${k} )
endforeach(k)


set( middlePath Msgpack )
set( sourceFile 
	DataBuffer.cpp
	DataRefBuffer.cpp
	encrypter/CapCompresser.cpp
	encrypter/XxteaEncrypter.cpp
	encrypter/Crc32Encrypter.cpp
)
foreach(k ${sourceFile} )
  set( fullSourcePath ${fullSourcePath} ${sourceRootPath}/${middlePath}/${k} )
endforeach(k)

set( middlePath Atomic )
set( sourceFile 
	SpinMutex.cpp
)
foreach(k ${sourceFile} )
  set( fullSourcePath ${fullSourcePath} ${sourceRootPath}/${middlePath}/${k} )
endforeach(k)

if (CMAKE_BUILD_TYPE STREQUAL Release)
	add_definitions("-DNDEBUG")
else ()
	add_definitions("-D_DEBUG")
	set( CMAKE_BUILD_TYPE Debug )
endif ()

add_definitions("-DLIGHTINK_AS_DLL")
add_definitions("-DLIGHTINK_EXPORTS")

#if (LIGHTINK_LUAJIT)
#	if (CMAKE_SIZEOF_VOID_P EQUAL 8)
#		set (luajit ${sourceRootPath}/LuaEngine/luajit/lib/linux/x64/libluajit.a)
#	else()
#		set (luajit ${sourceRootPath}/LuaEngine/luajit/lib/linux/x86/libluajit.a)
#	endif()
#endif()

add_library( ${PROJECT_NAME} SHARED ${fullSourcePath} )

find_package(Threads)
find_library(RT_LIB NAMES rt)
target_link_libraries(${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT}  ${RT_LIB}
						"-Wl,--whole-archive" ${luajit} "-Wl,--no-whole-archive")

if (CMAKE_BUILD_TYPE STREQUAL Debug)
set(LIBRARY_OUTPUT_PATH ${LightInkLLM_SOURCE_DIR}/../../bin/Debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb -W -pipe -fPIC")
else()
set(LIBRARY_OUTPUT_PATH ${LightInkLLM_SOURCE_DIR}/../../bin/Release)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -W -pipe -fPIC")
endif()

unset(middlePath)
unset(sourceFile)
unset(fullSourcePath)
unset(sourceRootPath)






