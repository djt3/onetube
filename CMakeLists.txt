cmake_minimum_required(VERSION 2.8)
project(test)
include_directories(/usr/local/include /usr/include)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -Iinclude -lfcgi++ -lfcgi")
set(CMAKE_SHARED_LINKER_FLAGS "-L/usr/lib -Wl,-O1,--sort-common,--as-needed,-z,relro,-z,now -lfcgi++ -lfcgi")

file(GLOB onetube_SRC
    "src/*.hpp"
    "videx/src/*.hpp"
    "src/*.cpp"
    "videx/src/*.cpp"
)

add_executable(onetube ${onetube_SRC})
target_link_libraries(onetube ${CURLPP_LDFLAGS})
