cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
project(demo_1)

message ("${CMAKE_CURRENT_SOURCE_DIR}")


set(CMAKE_CXX_STANDARD 17)

#include_directories("include")
include_directories(
        include
        include/portaudio
		portaudio/lib
		portaudio/src/common
		)

set(REQUIRED_LIBS "stdc++fs")


set(CMAKE_C_FLAGS "-pthread")
set(SOURCE_FILES demo.cpp)

add_executable(demo ${SOURCE_FILES})
SET_TARGET_PROPERTIES(demo PROPERTIES LINK_FLAGS "-no-pie")
target_link_libraries(demo
                    
                    ${CMAKE_CURRENT_SOURCE_DIR}/prebuilt/armv7/lib/ARMv7_Allwinner/libportaudio.a
                    ${CMAKE_CURRENT_SOURCE_DIR}/prebuilt/armv7/lib/ARMv7_Allwinner/libasound.so
                    ${CMAKE_CURRENT_SOURCE_DIR}/prebuilt/armv7/lib/ARMv7_Allwinner/libtensorflow-lite.so
                    ${CMAKE_CURRENT_SOURCE_DIR}/prebuilt/armv7/lib/lib_tf_shared/libruy.so
                    ${CMAKE_CURRENT_SOURCE_DIR}/prebuilt/armv7/lib/lib_tf_shared/libfft2d_fftsg2d.so
                    ${CMAKE_CURRENT_SOURCE_DIR}/prebuilt/armv7/lib/lib_tf_shared/libfft2d_fftsg.so
                    ${CMAKE_CURRENT_SOURCE_DIR}/prebuilt/armv7/lib/lib_tf_shared/libfarmhash.so
                    ${CMAKE_CURRENT_SOURCE_DIR}/prebuilt/armv7/lib/lib_tf_shared/libabsl_wyhash.so
                    ${CMAKE_CURRENT_SOURCE_DIR}/prebuilt/armv7/lib/lib_tf_shared/libabsl_time_zone.so
                    ${CMAKE_CURRENT_SOURCE_DIR}/prebuilt/armv7/lib/lib_tf_shared/libabsl_time.so
                    ${CMAKE_CURRENT_SOURCE_DIR}/prebuilt/armv7/lib/lib_tf_shared/libabsl_throw_delegate.so
                    ${CMAKE_CURRENT_SOURCE_DIR}/prebuilt/armv7/lib/lib_tf_shared/libabsl_synchronization.so
                    ${CMAKE_CURRENT_SOURCE_DIR}/prebuilt/armv7/lib/lib_tf_shared/libabsl_symbolize.so
                    ${CMAKE_CURRENT_SOURCE_DIR}/prebuilt/armv7/lib/lib_tf_shared/libabsl_strings_internal.so
                    ${CMAKE_CURRENT_SOURCE_DIR}/prebuilt/armv7/lib/lib_tf_shared/libabsl_strings.so
                    ${CMAKE_CURRENT_SOURCE_DIR}/prebuilt/armv7/lib/lib_tf_shared/libabsl_str_format_internal.so
                    ${CMAKE_CURRENT_SOURCE_DIR}/prebuilt/armv7/lib/lib_tf_shared/libabsl_status.so
                    ${CMAKE_CURRENT_SOURCE_DIR}/prebuilt/armv7/lib/lib_tf_shared/libabsl_stacktrace.so
                    ${CMAKE_CURRENT_SOURCE_DIR}/prebuilt/armv7/lib/lib_tf_shared/libabsl_spinlock_wait.so
                    ${CMAKE_CURRENT_SOURCE_DIR}/prebuilt/armv7/lib/lib_tf_shared/libabsl_raw_logging_internal.so
                    ${CMAKE_CURRENT_SOURCE_DIR}/prebuilt/armv7/lib/lib_tf_shared/libabsl_raw_hash_set.so
                    ${CMAKE_CURRENT_SOURCE_DIR}/prebuilt/armv7/lib/lib_tf_shared/libabsl_malloc_internal.so
                    ${CMAKE_CURRENT_SOURCE_DIR}/prebuilt/armv7/lib/lib_tf_shared/libabsl_log_severity.so
                    ${CMAKE_CURRENT_SOURCE_DIR}/prebuilt/armv7/lib/lib_tf_shared/libabsl_int128.so
                    ${CMAKE_CURRENT_SOURCE_DIR}/prebuilt/armv7/lib/lib_tf_shared/libabsl_hashtablez_sampler.so
                    ${CMAKE_CURRENT_SOURCE_DIR}/prebuilt/armv7/lib/lib_tf_shared/libabsl_hash.so
                    ${CMAKE_CURRENT_SOURCE_DIR}/prebuilt/armv7/lib/lib_tf_shared/libabsl_graphcycles_internal.so
                    ${CMAKE_CURRENT_SOURCE_DIR}/prebuilt/armv7/lib/lib_tf_shared/libabsl_flags_reflection.so
                    ${CMAKE_CURRENT_SOURCE_DIR}/prebuilt/armv7/lib/lib_tf_shared/libabsl_flags_program_name.so
                    ${CMAKE_CURRENT_SOURCE_DIR}/prebuilt/armv7/lib/lib_tf_shared/libabsl_flags_private_handle_accessor.so
                    ${CMAKE_CURRENT_SOURCE_DIR}/prebuilt/armv7/lib/lib_tf_shared/libabsl_flags_marshalling.so
                    ${CMAKE_CURRENT_SOURCE_DIR}/prebuilt/armv7/lib/lib_tf_shared/libabsl_flags_internal.so
                    ${CMAKE_CURRENT_SOURCE_DIR}/prebuilt/armv7/lib/lib_tf_shared/libabsl_flags_config.so
                    ${CMAKE_CURRENT_SOURCE_DIR}/prebuilt/armv7/lib/lib_tf_shared/libabsl_flags_commandlineflag_internal.so
                    ${CMAKE_CURRENT_SOURCE_DIR}/prebuilt/armv7/lib/lib_tf_shared/libabsl_flags_commandlineflag.so
                    ${CMAKE_CURRENT_SOURCE_DIR}/prebuilt/armv7/lib/lib_tf_shared/libabsl_flags.so
                    ${CMAKE_CURRENT_SOURCE_DIR}/prebuilt/armv7/lib/lib_tf_shared/libabsl_exponential_biased.so
                    ${CMAKE_CURRENT_SOURCE_DIR}/prebuilt/armv7/lib/lib_tf_shared/libabsl_demangle_internal.so
                    ${CMAKE_CURRENT_SOURCE_DIR}/prebuilt/armv7/lib/lib_tf_shared/libabsl_debugging_internal.so
                    ${CMAKE_CURRENT_SOURCE_DIR}/prebuilt/armv7/lib/lib_tf_shared/libabsl_cord.so
                    ${CMAKE_CURRENT_SOURCE_DIR}/prebuilt/armv7/lib/lib_tf_shared/libabsl_civil_time.so
                    ${CMAKE_CURRENT_SOURCE_DIR}/prebuilt/armv7/lib/lib_tf_shared/libabsl_city.so
                    ${CMAKE_CURRENT_SOURCE_DIR}/prebuilt/armv7/lib/lib_tf_shared/libabsl_base.so
                    ${CMAKE_CURRENT_SOURCE_DIR}/prebuilt/armv7/lib/lib_tf_shared/libabsl_bad_variant_access.so
                    ${CMAKE_CURRENT_SOURCE_DIR}/prebuilt/armv7/lib/lib_tf_shared/libabsl_bad_optional_access.so
                    -lpthread
                    )