################################ 工程配置 ##################################
# 指定 CMake 最低版本
CMAKE_MINIMUM_REQUIRED(VERSION 3.16)

# 设置 C++ 语言标准为 C++11
SET(CMAKE_CXX_STANDARD 11)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)

# 构建项目的名称
PROJECT(wlib)

# 添加编译选项（根据平台设置）
IF(MSVC)
    # MSVC 特定选项
    ADD_COMPILE_OPTIONS("$<$<CXX_COMPILER_ID:MSVC>:/source-charset:utf-8>")
ELSE()
    # GCC/Clang 特定选项（如需要）
    ADD_COMPILE_OPTIONS("-Wall" "-Wextra" "-Wpedantic")
ENDIF()

################################ 引用目录 ##################################
# 添加头文件路径
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/thirdparty/httplib)

################################ utf8cpp 集成 ##################################
# 添加 utf8cpp 子模块
add_subdirectory(${PROJECT_SOURCE_DIR}/thirdparty/utf8cpp)

################################ 源码文件 ##################################
# 搜索 src 目录及其子目录下的所有源文件（排除 example 文件夹）
FILE(GLOB_RECURSE SRC_LIST ${PROJECT_SOURCE_DIR}/src/*.cpp)
FILE(GLOB_RECURSE EXAMPLE_SRC_LIST ${PROJECT_SOURCE_DIR}/src/example/*.cpp)
LIST(REMOVE_ITEM SRC_LIST ${EXAMPLE_SRC_LIST})

################################ 静态库 ##################################
# 生成静态库
ADD_LIBRARY(${PROJECT_NAME}_static STATIC ${SRC_LIST})
SET_TARGET_PROPERTIES(${PROJECT_NAME}_static PROPERTIES
    OUTPUT_NAME ${PROJECT_NAME}  # 静态库名称为 wlib
    ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/lib/static/debug
    ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/lib/static/release
    DEBUG_POSTFIX "d"  # Debug 版本加后缀 d
)

# 链接 utf8cpp 到静态库
target_link_libraries(${PROJECT_NAME}_static PRIVATE utf8cpp)

################################ 动态库 ##################################
# 生成动态库
ADD_LIBRARY(${PROJECT_NAME}_shared SHARED ${SRC_LIST})
SET_TARGET_PROPERTIES(${PROJECT_NAME}_shared PROPERTIES
    OUTPUT_NAME ${PROJECT_NAME}  # 动态库名称也为 wlib
    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/lib/shared/debug
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/lib/shared/release
    DEBUG_POSTFIX "d"  # Debug 版本加后缀 d
)

# 链接 utf8cpp 到动态库
target_link_libraries(${PROJECT_NAME}_shared PRIVATE utf8cpp)

################################ 示例子目录 ##################################
# 遍历 example 文件夹中的子目录
FILE(GLOB EXAMPLE_DIRS ${PROJECT_SOURCE_DIR}/src/example/*)

FOREACH(EXAMPLE_DIR ${EXAMPLE_DIRS})
    IF(IS_DIRECTORY ${EXAMPLE_DIR})
        # 获取子目录名称
        GET_FILENAME_COMPONENT(EXAMPLE_NAME ${EXAMPLE_DIR} NAME)

        # 搜索子目录中的源文件
        FILE(GLOB EXAMPLE_SRC ${EXAMPLE_DIR}/*.cpp)

        # 添加可执行文件
        ADD_EXECUTABLE(${EXAMPLE_NAME} ${EXAMPLE_SRC})

        # 设置可执行文件的输出目录
        SET_TARGET_PROPERTIES(${EXAMPLE_NAME} PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/bin/debug
            RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/bin/release
            DEBUG_POSTFIX "d"  # Debug 版本加后缀 d
        )

        # 链接静态库到可执行文件
        TARGET_LINK_LIBRARIES(${EXAMPLE_NAME} PRIVATE ${PROJECT_NAME}_static utf8cpp)
    ENDIF()
ENDFOREACH()