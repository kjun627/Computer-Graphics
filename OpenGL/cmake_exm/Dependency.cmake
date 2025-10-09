
# ExternalProject 관련 명령어 셋 추가
include(ExternalProject)

# Dependency 관련 변수 설정
set(DEP_INSTALL_DIR ${PROJECT_BINARY_DIR}/install)
set(DEP_INCLUDE_DIR ${DEP_INSTALL_DIR}/include)
set(DEP_LIB_DIR ${DEP_INSTALL_DIR}/lib)

# stdlog: fast logger library
ExternalProject_Add(
    dep-spdlog
    GIT_REPOSITORY "https://github.com/gabime/spdlog.git"
    GIT_TAG "v1.x"
    GIT_SHALLOW 1
    UPDATE_COMMAND ""
    PATCH_COMMAND ""
    CMAKE_ARGS 
        # window 에서는 Multi configuration generator 지만
        # mac / linux 에서는 single 이므로 인자 두개 줘야함 debug mode
        -DCMAKE_INSTALL_PREFIX=${DEP_INSTALL_DIR}
        -DCMAKE_BUILD_TYPE=$<CONFIG>
    TEST_COMMAND ""
)

# Dependency 리스트 및 라이브 러리 파일 리스트 추가
set(DEP_LIST ${DEP_LIST} dep-spdlog)
set(DEP_LIBS ${DEP_LIBS} spdlog$<$<CONFIG:Debug>:d>)
# set(DEP_LIBS ${DEP_LIBS} spdlog)
