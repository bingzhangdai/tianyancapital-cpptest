.PHONY: test

test:
	cmake -S . -B build && cmake --build build && cd build && ./apps/main && ctest --output-on-failure
