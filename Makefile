

test: test.unit test.integration

test.unit: build.cmake
	cd build.commandline.dir && ./unit-tests/oscore-test/osal_core_UT
	cd build.commandline.dir && ./unit-tests/osfile-test/osal_file_UT
	cd build.commandline.dir && ./unit-tests/osfilesys-test/osal_filesys_UT
	# There are module files that the binary expects in the output folder.
        # Therefore cd'ing to the test binary's dir.
	cd build.commandline.dir/unit-tests/osloader-test && ./osal_loader_UT
	cd build.commandline.dir && ./unit-tests/ostimer-test/osal_timer_UT
	cd build.commandline.dir && ./unit-tests/osnetwork-test/osal_network_UT

test.integration: build.cmake
	cd build.commandline.dir && ./tests/bin-sem-flush-test
	cd build.commandline.dir && ./tests/bin-sem-test
	cd build.commandline.dir && ./tests/bin-sem-timeout-test
	cd build.commandline.dir && ./tests/count-sem-test
	cd build.commandline.dir && ./tests/file-api-test
	cd build.commandline.dir && ./tests/mutex-test
	cd build.commandline.dir && ./tests/osal-core-test
	cd build.commandline.dir && ./tests/queue-timeout-test
	cd build.commandline.dir && ./tests/sem-speed-test
	cd build.commandline.dir && ./tests/symbol-api-test
	cd build.commandline.dir && ./tests/timer-test

build.cmake:
	mkdir -p build.commandline.dir
	cd build.commandline.dir && cmake -G Ninja \
		-DCMAKE_C_FLAGS="-Werror" \
		-DENABLE_UNIT_TESTS=1 \
		-DOSAL_SYSTEM_OSTYPE=posix \
		-DOSAL_SYSTEM_BSPTYPE=pc-linux \
		-DOSAL_INCLUDEDIR=src/bsp/pc-linux/config \
		..
	cd build.commandline.dir && ninja
