SET(CMAKE_LEGACY_CYGWIN_WIN32 0)
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

SET(CMAKE_CXX_STANDARD 14)
SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO
    "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -pg"
)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY
    ${PROJECT_SOURCE_DIR}/${CMAKE_BUILD_TYPE}/bin
)
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY
    ${PROJECT_SOURCE_DIR}/${CMAKE_BUILD_TYPE}/lib
)
SET(ECCRYPTO_SOURCE_FILES
    encoding.cpp
    hash.cpp
    helper.cpp
    random.cpp
    streaming.cpp
)
SET(ECCTEST_SOURCE_FILES
    test/ecctest.cpp
    test/tbinteger.cpp
    test/tecurve.cpp
    test/tencoding.cpp
    test/thash.cpp
    test/thelper.cpp
    test/trandom.cpp
    test/tstreaming.cpp
)
SET(ECCPROFILE_SOURCE_FILES
    profile/eccprofile.cpp
    profile/pbinteger.cpp
    profile/pecurve.cpp
)

IF(CMAKE_BUILD_TYPE MATCHES Release)
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")
    ADD_LIBRARY(eccrypto ${ECCRYPTO_SOURCE_FILES})
ELSEIF(CMAKE_BUILD_TYPE MATCHES Debug)
    ADD_EXECUTABLE(ecctest
        ${ECCRYPTO_SOURCE_FILES}
        ${ECCTEST_SOURCE_FILES}
    )
    TARGET_LINK_LIBRARIES(ecctest CppUTest CppUTestExt)
ELSEIF(CMAKE_BUILD_TYPE MATCHES RelWithDebInfo)
    ADD_EXECUTABLE(eccprofile
        ${ECCRYPTO_SOURCE_FILES}
        ${ECCPROFILE_SOURCE_FILES}
    )
    TARGET_LINK_LIBRARIES(eccprofile CppUTest CppUTestExt)
ENDIF()
