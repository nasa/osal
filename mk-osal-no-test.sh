mkdir -p ../build_osal
cd ../build_osal
cmake -DOSAL_SYSTEM_BSPTYPE=generic-linux \
-DCMAKE_INSTALL_PREFIX=../local \
../osal
make
make install

