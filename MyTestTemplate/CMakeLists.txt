cmake_minimum_required(VERSION 2.8)
project(xhl_test C CXX)
enable_testing()


set(CXX_FLAGS
 -g
 # -DVALGRIND
 -DCHECK_PTHREAD_RETURN_VALUE
 -D_FILE_OFFSET_BITS=64
 -Wall
 -Wextra
# -Werror
 -Wconversion
 -Wno-unused-parameter
 -Wold-style-cast
 -Woverloaded-virtual
 -Wpointer-arith
 -Wshadow
 -Wwrite-strings
 -march=native
 # -MMD
 -std=c++11
 -rdynamic
)
string(REPLACE ";" " " CMAKE_CXX_FLAGS "${CXX_FLAGS}")
set(CMAKE_CXX_FLAGS_DEBUG "-O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")


add_subdirectory(mod1)
add_subdirectory(mod2)
add_subdirectory(curl)

include_directories("./mod1/include")
include_directories("./mod2/include")
include_directories("./curl/include")

include_directories("./include")

link_directories(${PROJECT_SOURCE_DIR}/lib)


#message(${PROJECT_SOURCE_DIR})

add_executable(test
			main.cpp
			)
target_link_libraries(test
	-Wl,--start-group
	mod1
	mod2
	pthread
	muduo_base
	muduo_cdns
	muduo_curl
	muduo_http
	muduo_inspect
	muduo_net
	muduo_curl
	curl
	 -Wl,--end-group
	)

