mkdir -p ../build_osal_test
cd ../build_osal_test
cmake -DENABLE_UNIT_TESTS=true \
-DOSAL_SYSTEM_BSPTYPE=generic-linux \
-DOSAL_CONFIG_DEBUG_PERMISSIVE_MODE=TRUE \
-DCMAKE_INSTALL_PREFIX=../local \
../osal 
make
make test
make install
