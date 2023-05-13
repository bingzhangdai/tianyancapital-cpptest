include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG main
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(cpptest)
target_sources(cpptest
  PRIVATE
    q1.cpp
    q2.cpp
)

target_link_libraries(cpptest GTest::gtest_main GTest::gmock_main cpptest::q1)

include(GoogleTest)
gtest_discover_tests(cpptest)
