add_library(cpptest_q1 INTERFACE)
add_library(cpptest::q1 ALIAS cpptest_q1)

target_sources(cpptest_q1
  INTERFACE
    q1/market_data_a.hpp
    q1/market_data_b.hpp
    q1/market_data_processor_base.hpp
    q1/market_data_processor_a.hpp
    q1/market_data_processor_b.hpp
    q1/market_data_processor.hpp
    q2/order.hpp
    q2/risk.hpp
)

target_include_directories(cpptest_q1 INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})



