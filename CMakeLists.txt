################################ 工程配置 ##################################
# 指定 CMake 最低版本
CMAKE_MINIMUM_REQUIRED(VERSION 3.16)
# 设置C++语言标准为C++11
SET(CMAKE_CXX_STANDARD 11) 
# 确保编译器遵循所设置的标准
SET(CMAKE_CXX_STANDARD_REQUIRED ON) 
# 构建项目的名称
PROJECT(wlib)
# 项目添加编码虚选项
ADD_COMPILE_OPTIONS("$<$<CXX_COMPILER_ID:MSVC>:/source-charset:utf-8>") 
################################ 引用目录 ##################################
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include)
################################ 源码文件 ##################################
# 搜索源文件
FILE(GLOB SRC_LIST ${PROJECT_SOURCE_DIR}/src/*.cpp)
################################ 编译文件 ##################################
# 生成静态库
ADD_LIBRARY(${PROJECT_NAME} STATIC ${HEADER_LIST} ${SRC_LIST})
################################ 属性配置 ##################################
# 设置库文件输出目录
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
# 设置可执行文件输出目录(Windows动态库为可执行文件)
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

# 设置静态库的输出目录
SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES
    MSVC_RUNTIME_LIBRARY "MultiThreaded"
    # 除了debug和release其他类型库的输出
    ARCHIVE_OUTPUT_DIRECTORY ${LIBRARY_OUTPUT_PATH}/other
    # debug库的输出
    ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${LIBRARY_OUTPUT_PATH}/debug
    # release库的输出
    ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${LIBRARY_OUTPUT_PATH}/release
    OUTPUT_NAME ${PROJECT_NAME}
    # debug 版本加后缀
    DEBUG_POSTFIX "d"
)


################################ 属性配置 ##################################

ADD_SUBDIRECTORY(${PROJECT_SOURCE_DIR}/src/example/xorstr)