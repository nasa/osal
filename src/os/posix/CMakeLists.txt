######################################################################
#
# CMAKE build recipe for POSIX OSAL implementation
#
######################################################################

# This CMake script generates targets specific to the POSIX implementation
# It defines an OBJECT target named "osal_posix_impl"

add_library(osal_posix_impl OBJECT 
	osapi.c
	osfileapi.c
	osfilesys.c
	osloader.c
	osnetwork.c
	osselect.c
	ostimer.c
)

target_link_libraries(osal_posix_impl posix-macos-addons)

# TODO-MAC: Defining this globally but can be made more focused.
# 1) In file included from /sandbox/cFS/osal/src/os/posix/osloader.c:32:
#/sandbox/cFS/osal/src/os/posix/../portable/os-impl-posix-dl.c:130:30: error: use of undeclared identifier 'RTLD_DEFAULT'
#    Function = dlsym((void *)RTLD_DEFAULT, SymbolName);
#                             ^
# 2) /usr/include/sys/ucred.h:96:11: error: unknown type name 'u_long'; did you mean 'long'?
#        volatile u_long         cr_ref;  /* reference count */
target_compile_definitions(osal_posix_impl PRIVATE -D_DARWIN_C_SOURCE)
