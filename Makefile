
default:
	mkdir -p build
	cd build && cmake -DENABLE_UNIT_TESTS=true -DCMAKE_VERBOSE_MAKEFILE=1 -DCMAKE_CXX_COMPILER_LAUNCHER=ccache -DOSAL_SYSTEM_BSPTYPE=generic-linux -DOSAL_SYSTEM_OSTYPE=posixmacos --graphviz=test.dot ..
	# dot -Tpng build/test.dot -o build/graph.png
	cd build && make

test: default
	cd build && CTEST_OUTPUT_ON_FAILURE=1 make test

test_flaky: default
	cd build && CTEST_OUTPUT_ON_FAILURE=1 ctest --output-on-failure -R timer-add-api-test
	cd build && CTEST_OUTPUT_ON_FAILURE=1 ctest --output-on-failure -R queue-test
	cd build && CTEST_OUTPUT_ON_FAILURE=1 ctest --output-on-failure -R sem-speed-test
