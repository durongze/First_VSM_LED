cmake_minimum_required(VERSION 3.15)
project (First_VSM_LED)

#add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/source-charset:utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/execution-charset:utf-8>")

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

set(CMAKE_MFC_FLAG 1) 
#add_definitions(-DUNICODE -D_UNICODE)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(ENV{HomeDir}      ${CMAKE_CURRENT_LIST_DIR}/out/${CMAKE_SYSTEM_NAME})

set (VSM_LED_BIN_DIR  ${CMAKE_CURRENT_BINARY_DIR})
set (VSM_LED_TOP_DIR  ${PROJECT_SOURCE_DIR}      )  # CMAKE_CURRENT_LIST_DIR CMAKE_CURRENT_SOURCE_DIR
set (VSM_LED_SRC_DIR  ${VSM_LED_TOP_DIR}/src   )

set (VSM_LED_INC_DIRS ${VSM_LED_TOP_DIR}/    ${VSM_LED_TOP_DIR}/sdk        ${VSM_LED_TOP_DIR}/src    )

#  Set Target Info
set(CMAKE_INSTALL_PREFIX           ${CMAKE_BINARY_DIR}/install   CACHE PATH   "Where to install output files")
#set(CMAKE_DEBUG_POSTFIX            d                      )
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/)

set(CMAKE_C_FLAGS         "${CMAKE_C_FLAGS}   -Wall ")
set(CMAKE_CXX_FLAGS       "${CMAKE_CXX_FLAGS} -Wall ")
set(GENERIC_LIB_VERSION   "8.0.0")
set(GENERIC_LIB_SOVERSION "8"    )

#add_definitions(-D DEBUG)
 

file(GLOB First_VSM_LED_SRCS      RELATIVE ${VSM_LED_TOP_DIR} ./src/*.c* ./src/*.h*)
file(GLOB Resource_Files          RELATIVE ${VSM_LED_TOP_DIR} "res/*.res" )

set(VSM_LED_SRC_LIST ${First_VSM_LED_SRCS} )
set(VSM_LED_APP_LIST ${First_VSM_LED_SRCS} )

source_group("Resource Files"         FILES ${Resource_Files}   )
source_group(TREE ${VSM_LED_TOP_DIR}  FILES ${VSM_LED_SRC_LIST} )

#list(REMOVE_ITEM VSM_LED_SRC_LIST ./src/main.c* )

message("[INFO] DepLibs: ${dep_libs}")

set ( VSM_LED_DEP_DEFS   WIN32    NDEBUG    _WINDOWS    _USRDLL    _AFXDLL   )

set                       (  VSM_LED_STATIC                 vsm_led_static)
add_library               (${VSM_LED_STATIC}   STATIC     ${VSM_LED_SRC_LIST} )
target_compile_definitions(${VSM_LED_STATIC}   PRIVATE    ${VSM_LED_DEP_DEFS} )
target_include_directories(${VSM_LED_STATIC}   PUBLIC     ${VSM_LED_INC_DIRS} )
target_link_directories   (${VSM_LED_STATIC}   PUBLIC     ${VSM_LED_LIB_DIRS} )
target_link_libraries     (${VSM_LED_STATIC}              ${VSM_LED_DEP_LIBS}           ${sys_dep_libs} )
target_compile_options    (${VSM_LED_STATIC}   PRIVATE      /Z7     /W4       )
set_target_properties     (${VSM_LED_STATIC}   PROPERTIES CLEAN_DIRECT_OUTPUT 1         OUTPUT_NAME "vsm_led_static"   )

set                       (  VSM_LED_SHARED                 vsm_led_shared    )
add_library               (${VSM_LED_SHARED}   SHARED     ${VSM_LED_SRC_LIST} )
target_compile_definitions(${VSM_LED_SHARED}   PRIVATE    ${VSM_LED_DEP_DEFS} )
target_include_directories(${VSM_LED_SHARED}   PUBLIC     ${VSM_LED_INC_DIRS} )
target_link_directories   (${VSM_LED_SHARED}   PUBLIC     ${VSM_LED_LIB_DIRS} )
target_link_libraries     (${VSM_LED_SHARED}              ${VSM_LED_DEP_LIBS}           ${sys_dep_libs} )  
target_compile_options    (${VSM_LED_SHARED}   PRIVATE     /Z7     /W4          )
set_target_properties     (${VSM_LED_SHARED}   PROPERTIES CLEAN_DIRECT_OUTPUT 1         OUTPUT_NAME "VSM_LED_SHARED"   )
set_target_properties     (${VSM_LED_SHARED}   PROPERTIES PUBLIC_HEADER                 "${VSM_LED_HDRS_LIST}"         )
set_target_properties     (${VSM_LED_SHARED}   PROPERTIES COMPILE_DEFINITIONS "VSM_LED_EXPORTS"    VERSION "${GENERIC_LIB_VERSION}"    SOVERSION "${GENERIC_LIB_SOVERSION}")


#set                       (  VSM_LED_APP                  lcd12864b_app     )
#add_executable            (${VSM_LED_APP}               ${VSM_LED_APP_LIST} )
#target_compile_definitions(${VSM_LED_APP}   PRIVATE     ${VSM_LED_DEP_DEFS} )
#target_include_directories(${VSM_LED_APP}   PUBLIC      ${VSM_LED_INC_DIRS} )
#target_link_directories   (${VSM_LED_APP}   PUBLIC      ${VSM_LED_LIB_DIRS} )
#target_link_libraries     (${VSM_LED_APP}               ${VSM_LED_STATIC}   )  # VSM_LED_LIB_STATIC
#target_link_libraries     (${VSM_LED_APP}               ${VSM_LED_SHARED}   )  # must be REALSE
#set_target_properties     (${VSM_LED_APP}   PROPERTIES  LINK_FLAGS "/SUBSYSTEM:WINDOWS " ) # /SAFESEH:MO

SET(install_targets ${VSM_LED_STATIC}    ${VSM_LED_SHARED}    ${VSM_LED_APP})
SET(install_hdr_dir include)

INSTALL(TARGETS       ${install_targets}
        RUNTIME       DESTINATION bin
        LIBRARY       DESTINATION lib
        ARCHIVE       DESTINATION lib
        PUBLIC_HEADER DESTINATION ${install_hdr_dir}
       )

install(FILES      ${VSM_LED_HDRS_LIST} DESTINATION ${install_hdr_dir})
#install(DIRECTORY  "inc_sub_dir"          DESTINATION ${install_hdr_dir} FILES_MATCHING PATTERN "*.h")