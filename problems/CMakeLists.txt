add_library(cpptest_q1 INTERFACE)
add_library(cpptest::q1 ALIAS cpptest_q1)

target_sources(cpptest_q1
  INTERFACE
    q1/market_data_a.hpp
    q1/market_data_b.hpp
    q1/market_data_processor.hpp
    q2/order.hpp
    q2/risk.hpp
    q3/api.hpp
    q3/order_rtn.hpp
    q3/singleton.hpp
    q4/concurrent_queue.hpp
    q4/strategy.hpp
)

target_include_directories(cpptest_q1 INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

include(FetchContent)

# FetchContent_Declare(
#   zeromq
#   GIT_REPOSITORY https://github.com/zeromq/libzmq.git
#   GIT_TAG master
# )

# FetchContent_Declare(
#   cppzmq
#   GIT_REPOSITORY https://github.com/zeromq/cppzmq.git
#   GIT_TAG master
# )

# FetchContent_MakeAvailable(zeromq cppzmq)


# target_link_libraries(cpptest_q1 INTERFACE cppzmq)

