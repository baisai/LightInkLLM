
cmake_minimum_required(VERSION 2.8)
option(LIGHTINK_LUAJIT "use lua jit" OFF)

project (LightInkLLMT)

set( includeRootPath ${LightInkLLMT_SOURCE_DIR}/../../include)
set( sourceRootPath ${LightInkLLMT_SOURCE_DIR}/../../src )
include_directories( ${includeRootPath} )


file(GLOB AtomicInc ${includeRootPath}/Atomic/*.h)
list(APPEND AtomicInc ${includeRootPath}/Atomic/Turf/turf.h)
file(GLOB AtomicSrc ${sourceRootPath}/Atomic/*.cpp)
list(APPEND fullSourcePath ${AtomicInc} ${AtomicSrc})


file(GLOB CommonInc ${includeRootPath}/Common/*.h
					${includeRootPath}/Common/*.cpp)
file(GLOB CommonInc_Memory ${includeRootPath}/Common/Memory/*.h)
file(GLOB CommonInc_Ptr ${includeRootPath}/Common/Ptr/*.h
					${includeRootPath}/Common/Ptr/*.cpp)
file(GLOB CommonInc_TypeTool ${includeRootPath}/Common/TypeTool/*.h)
file(GLOB CommonSrc ${sourceRootPath}/Common/*.cpp
					${sourceRootPath}/Common/*.c)
file(GLOB CommonSrc_Memory ${sourceRootPath}/Common/Memory/*.cpp)
list(APPEND fullSourcePath ${CommonInc}
							${CommonInc_Memory} 
							${CommonInc_Ptr} 
							${CommonInc_TypeTool} 
							${CommonSrc} 
							${CommonSrc_Memory})


file(GLOB fmtInc ${includeRootPath}/fmt/*.h
					${includeRootPath}/fmt/*.hpp)
list(APPEND fullSourcePath ${fmtInc})

file(GLOB kcpInc ${includeRootPath}/kcp/*.h)
file(GLOB kcpSrc ${sourceRootPath}/kcp/*.c)
list(APPEND fullSourcePath ${kcpInc} ${kcpSrc})


file(GLOB LogInc ${includeRootPath}/Log/*.h)
file(GLOB LogInc_async ${includeRootPath}/Log/async/*.h)
file(GLOB LogInc_channel ${includeRootPath}/Log/channel/*.h)
file(GLOB LogInc_helper ${includeRootPath}/Log/helper/*.h)
file(GLOB LogSrc ${sourceRootPath}/Log/*.cpp)
file(GLOB LogSrc_async ${sourceRootPath}/Log/async/*.cpp)
file(GLOB LogSrc_channel ${sourceRootPath}/Log/channel/*.cpp)
file(GLOB LogSrc_helper ${sourceRootPath}/Log/helper/*.cpp)
list(APPEND fullSourcePath ${LogInc}
							${LogInc_async}
							${LogInc_channel}
							${LogInc_helper}
							${LogSrc}
							${LogSrc_async}
							${LogSrc_channel}
							${LogSrc_helper})


file(GLOB LuaEngineInc ${includeRootPath}/LuaEngine/*.h
					${includeRootPath}/LuaEngine/*.cpp)
if (LIGHTINK_LUAJIT)
	file(GLOB LuaEngineInc_luajit ${includeRootPath}/LuaEngine/luajit/*.h)
else()
	file(GLOB LuaEngineInc_lua ${includeRootPath}/LuaEngine/lua/*.h)
endif()
file(GLOB LuaEngineSrc ${sourceRootPath}/LuaEngine/*.cpp)
if (LIGHTINK_LUAJIT)

else()
	file(GLOB LuaEngineSrc_lua ${sourceRootPath}/LuaEngine/lua/*.c)
endif()
list(APPEND fullSourcePath ${LuaEngineInc}
							${LuaEngineInc_luajit}
							${LuaEngineInc_lua}
							${LuaEngineSrc}
							${LuaEngineSrc_lua})

file(GLOB MsgpackInc ${includeRootPath}/Msgpack/*.h
					${includeRootPath}/Msgpack/*.cpp)
file(GLOB MsgpackInc_encrypter ${includeRootPath}/Msgpack/encrypter/*.h)
file(GLOB MsgpackSrc ${sourceRootPath}/Msgpack/*.cpp)
file(GLOB MsgpackSrc_encrypter ${sourceRootPath}/Msgpack/encrypter/*.cpp)
list(APPEND fullSourcePath ${MsgpackInc}
							${MsgpackInc_encrypter}
							${MsgpackSrc}
							${MsgpackSrc_encrypter})


file(GLOB TimerInc ${includeRootPath}/Timer/*.h)
file(GLOB TimerSrc ${sourceRootPath}/Timer/*.cpp)
list(APPEND fullSourcePath ${TimerInc} ${TimerSrc})


file(GLOB TinySocketInc ${includeRootPath}/TinySocket/*.h)
file(GLOB TinySocketSrc ${sourceRootPath}/TinySocket/*.cpp)
list(APPEND fullSourcePath ${TinySocketInc} ${TinySocketSrc})


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
add_definitions("-DLIGHTINK_EXPORTS")

if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

add_library( ${PROJECT_NAME} SHARED ${fullSourcePath} )

source_group(Atomic\\include FILES ${AtomicInc})
unset(AtomicInc)
source_group(Atomic\\source FILES ${AtomicSrc})
unset(AtomicSrc)
source_group(Common\\include FILES ${CommonInc})
unset(CommonInc)
source_group(Common\\include\\Memory FILES ${CommonInc_Memory})
unset(CommonInc_Memory)
source_group(Common\\include\\Ptr FILES ${CommonInc_Ptr})
unset(CommonInc_Ptr)
source_group(Common\\include\\TypeTool FILES ${CommonInc_TypeTool})
unset(CommonInc_TypeTool)
source_group(Common\\source FILES ${CommonSrc})
unset(CommonSrc)
source_group(Common\\source\\Memory FILES ${CommonSrc_Memory})
unset(CommonSrc_Memory)
source_group(fmt FILES ${fmtInc})
unset(fmtInc)
source_group(kcp FILES ${kcpInc} ${kcpSrc})
unset(kcpInc)
unset(kcpSrc)
source_group(Log\\include FILES ${LogInc})
unset(LogInc)
source_group(Log\\include\\async FILES ${LogInc_async})
unset(LogInc_async)
source_group(Log\\include\\channel FILES ${LogInc_channel})
unset(LogInc_channel)
source_group(Log\\include\\helper FILES ${LogInc_helper})
unset(LogInc_helper)
source_group(Log\\source FILES ${LogSrc})
unset(LogSrc)
source_group(Log\\source\\async FILES ${LogSrc_async})
unset(LogSrc_async)
source_group(Log\\source\\channel FILES ${LogSrc_channel})
unset(LogSrc_channel)
source_group(Log\\source\\helper FILES ${LogSrc_helper})
unset(LogSrc_helper)
source_group(LuaEngine\\include FILES ${LuaEngineInc})
unset(LuaEngineInc)
source_group(LuaEngine\\include\\luajit FILES ${LuaEngineInc_luajit})
unset(LuaEngineInc_luajit)
source_group(LuaEngine\\include\\lua FILES ${LuaEngineInc_lua})
unset(LuaEngineInc_lua)
source_group(LuaEngine\\source FILES ${LuaEngineSrc})
unset(LuaEngineSrc)
source_group(LuaEngine\\source\\lua FILES ${LuaEngineSrc_lua})
unset(LuaEngineSrc_lua)
source_group(Msgpack\\include FILES ${MsgpackInc})
unset(MsgpackInc)
source_group(Msgpack\\include\\encrypter FILES ${MsgpackInc_encrypter})
unset(MsgpackInc_encrypter)
source_group(Msgpack\\source FILES ${MsgpackSrc})
unset(MsgpackSrc)
source_group(Msgpack\\source\\encrypter FILES ${MsgpackSrc_encrypter})
unset(MsgpackSrc_encrypter)
source_group(Timer\\include FILES ${TimerInc})
unset(TimerInc)
source_group(Timer\\source FILES ${TimerSrc})
unset(TimerSrc)
source_group(TinySocket\\include FILES ${TinySocketInc})
unset(TinySocketInc)
source_group(TinySocket\\source FILES ${TinySocketSrc})
unset(TinySocketSrc)


if (WIN32)
 	list (APPEND LIBS ws2_32)
endif ()

find_package(Threads)
target_link_libraries(${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT} ${LIBS})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(LIBRARY_OUTPUT_PATH ${LightInkLLMT_SOURCE_DIR}/../../bin)
if (NOT WIN32)
	set(LIBRARY_OUTPUT_PATH ${LIBRARY_OUTPUT_PATH}/${CMAKE_BUILD_TYPE})
endif()

unset(fullSourcePath)
unset(includeRootPath)
unset(sourceRootPath)






