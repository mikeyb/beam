cmake_minimum_required(VERSION 3.13)

set(TARGET_NAME beam-wallet${BEAM_TARGET_SUFFIX})

add_executable(${TARGET_NAME} cli.cpp)

if(LINUX)
    target_link_libraries(${TARGET_NAME}
        PRIVATE 
            -static-libstdc++ 
            -static-libgcc)
endif()

configure_file("${PROJECT_SOURCE_DIR}/version.h.in" "${CMAKE_CURRENT_BINARY_DIR}/version.h")
target_include_directories(${TARGET_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(${TARGET_NAME}
    PRIVATE
        wallet
        cli
        mnemonic
)

if(BEAM_LASER_SUPPORT)
    target_compile_definitions(${TARGET_NAME} PRIVATE BEAM_LASER_SUPPORT)
    target_link_libraries(${TARGET_NAME} PRIVATE laser)
endif()

if(BEAM_ATOMIC_SWAP_SUPPORT)
    target_compile_definitions(${TARGET_NAME} PRIVATE BEAM_ATOMIC_SWAP_SUPPORT)
    target_link_libraries(${TARGET_NAME} PRIVATE swap)
endif()

if(BEAM_CONFIDENTIAL_ASSETS_SUPPORT)
    target_compile_definitions(${TARGET_NAME} PRIVATE BEAM_CONFIDENTIAL_ASSETS_SUPPORT)
    target_link_libraries(${TARGET_NAME} PRIVATE assets)
endif()

if (BEAM_SIGN_PACKAGE AND WIN32)
    add_custom_command(
        TARGET ${TARGET_NAME} POST_BUILD
        COMMAND ${SIGNTOOL_EXECUTABLE} sign ${SIGNTOOL_OPTIONS} "${CMAKE_CURRENT_BINARY_DIR}\\{TARGET_NAME}.exe"
    )
endif()
