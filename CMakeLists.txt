cmake_minimum_required(VERSION 3.19)

project("pnlo")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB PNLO_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_executable("pnlo" ${PNLO_SOURCES})

if(MSVC)
	# 源码为 UTF-8
	target_compile_options("pnlo" PRIVATE "/source-charset:utf-8")
else()
	target_compile_options("pnlo" PRIVATE "-finput-charset=utf-8")
	if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
		# Windows 控制台默认 GBK 编码，保持中文输出不乱码
		target_compile_options("pnlo" PRIVATE "-fexec-charset=GBK")
	endif()
	if(WIN32)
		# Windows 下静态链接运行时，避免依赖 MinGW DLL
		target_link_options("pnlo" PRIVATE "-static")
	endif()
endif()

# 产物统一输出到构建目录的 bin/，避免与源码目录同名冲突
set_target_properties("pnlo" PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# 多配置生成器（如 Visual Studio）默认会追加 <CONFIG> 子目录，这里统一掉
foreach(config Debug Release RelWithDebInfo MinSizeRel)
	string(TOUPPER "${config}" config_upper)
	set_target_properties("pnlo" PROPERTIES
		RUNTIME_OUTPUT_DIRECTORY_${config_upper} "${CMAKE_BINARY_DIR}/bin"
	)
endforeach()