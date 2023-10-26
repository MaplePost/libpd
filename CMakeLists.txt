cmake_minimum_required (VERSION 2.8.12)
project (libpd C)

option(PD_UTILS  "Compile utilities" ON)
option(PD_EXTRA  "Compile extras" ON)
option(PD_MULTI  "Compile with multiple instance support" OFF)
option(PD_LOCALE "Set the LC_NUMERIC number format to the default C locale" ON)
option(PD_BUILD_C_EXAMPLES "Builds C API example programs" OFF)
option(PD_BUILD_JAVA "Build Java bindings" OFF)

if (MSVC)
    set(CMAKE_THREAD_LIBS_INIT CACHE PATH "Path to pthreads library binary file (ex. C:/src/vcpkg/packages/pthreads_x64-windows/lib/pthreadVC3.lib)")
    set(PTHREADS_INCLUDE_DIR CACHE PATH "Path to folder with pthreads library header files (ex. C:/src/vcpkg/packages/pthreads_x64-windows/include)")
    # We need pthreads.
    # Please provide the path to the pthreads library include path and
    # the path to the pthread library by specifying the following arguments
    # on the CMake command-line:
    # -DCMAKE_THREAD_LIBS_INIT:PATH=<path to library, either MSVC (for example pthreadVC3.lib) or GNUC version>
    # -DPTHREADS_INCLUDE_DIR:PATH=<path to the pthread header files>
    if (NOT CMAKE_THREAD_LIBS_INIT OR NOT PTHREADS_INCLUDE_DIR)
        message(FATAL_ERROR "Please provide a path to the pthreads library and its headers.")
    endif()
    # Prefer pthread implementation on platforms where multiple are available.
    set(CMAKE_THREAD_PREFER_PTHREAD ON)
endif()

if(WIN32)
    # Use Windows APIs compatible with most versions
    set(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -DWINVER=0x502 -DWIN32 -D_WIN32")
endif()
if (MSVC)
    set(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -DHAVE_STRUCT_TIMESPEC")
    add_definitions("/D_CRT_SECURE_NO_WARNINGS /wd4091 /wd4996")
    if(${CMAKE_SIZEOF_VOID_P} EQUAL 8)
        # Select appropriate long int type on 64-bit Windows
        set(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -DPD_LONGINTTYPE=\"long long\"")
    endif()
else()
    set(CMAKE_C_FLAGS         "${CMAKE_C_FLAGS} -Wno-int-to-pointer-cast -Wno-pointer-to-int-cast -Wno-format")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS} -ffast-math -funroll-loops -fomit-frame-pointer -O3")
    set(CMAKE_C_FLAGS_DEBUG   "${CMAKE_C_FLAGS} -g -O0")
    if(NOT APPLE AND NOT WIN32)
        set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-Bsymbolic")
    endif()
endif()
add_definitions(-DPD=1 -DUSEAPI_DUMMY=1)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_MACOSX_RPATH ON)

foreach (_BUILD_TYPE RELEASE DEBUG)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${_BUILD_TYPE} ${CMAKE_CURRENT_BINARY_DIR}/libs)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${_BUILD_TYPE} ${CMAKE_CURRENT_BINARY_DIR}/libs)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${_BUILD_TYPE} ${CMAKE_CURRENT_BINARY_DIR}/libs)
endforeach()

set(PD_SOURCES
    pure-data/src/d_arithmetic.c
    pure-data/src/d_array.c
    pure-data/src/d_ctl.c
    pure-data/src/d_dac.c
    pure-data/src/d_delay.c
    pure-data/src/d_fft.c
    pure-data/src/d_fft_fftsg.c
    pure-data/src/d_filter.c
    pure-data/src/d_global.c
    pure-data/src/d_math.c
    pure-data/src/d_misc.c
    pure-data/src/d_osc.c
    pure-data/src/d_resample.c
    pure-data/src/d_soundfile.c
    pure-data/src/d_soundfile_aiff.c
    pure-data/src/d_soundfile_caf.c
    pure-data/src/d_soundfile_next.c
    pure-data/src/d_soundfile_wave.c
    pure-data/src/d_ugen.c
    pure-data/src/g_all_guis.c
    pure-data/src/g_all_guis.h
    pure-data/src/g_array.c
    pure-data/src/g_bang.c
    pure-data/src/g_canvas.c
    pure-data/src/g_canvas.h
    pure-data/src/g_clone.c
    pure-data/src/g_editor.c
    pure-data/src/g_editor_extras.c
    pure-data/src/g_graph.c
    pure-data/src/g_guiconnect.c
    pure-data/src/g_io.c
    pure-data/src/g_mycanvas.c
    pure-data/src/g_numbox.c
    pure-data/src/g_radio.c
    pure-data/src/g_readwrite.c
    pure-data/src/g_rtext.c
    pure-data/src/g_scalar.c
    pure-data/src/g_slider.c
    pure-data/src/g_template.c
    pure-data/src/g_text.c
    pure-data/src/g_toggle.c
    pure-data/src/g_traversal.c
    pure-data/src/g_undo.c
    pure-data/src/g_vumeter.c
    pure-data/src/m_atom.c
    pure-data/src/m_binbuf.c
    pure-data/src/m_class.c
    pure-data/src/m_conf.c
    pure-data/src/m_glob.c
    pure-data/src/m_imp.h
    pure-data/src/m_memory.c
    pure-data/src/m_obj.c
    pure-data/src/m_pd.c
    pure-data/src/m_pd.h
    pure-data/src/m_sched.c
    pure-data/src/s_audio.c
    pure-data/src/s_audio_dummy.c
    pure-data/src/s_inter.c
    pure-data/src/s_inter_gui.c
    pure-data/src/s_loader.c
    pure-data/src/s_main.c
    pure-data/src/s_net.c
    pure-data/src/s_path.c
    pure-data/src/s_print.c
    pure-data/src/s_stuff.h
    pure-data/src/s_utf8.c
    pure-data/src/s_utf8.h
    pure-data/src/x_acoustics.c
    pure-data/src/x_arithmetic.c
    pure-data/src/x_array.c
    pure-data/src/x_connective.c
    pure-data/src/x_file.c
    pure-data/src/x_gui.c
    pure-data/src/x_interface.c
    pure-data/src/x_list.c
    pure-data/src/x_midi.c
    pure-data/src/x_misc.c
    pure-data/src/x_net.c
    pure-data/src/x_scalar.c
    pure-data/src/x_text.c
    pure-data/src/x_time.c
    pure-data/src/x_vexp.c
    pure-data/src/x_vexp.h
    pure-data/src/x_vexp_fun.c
    pure-data/src/x_vexp_if.c
)

set(PD_EXTRA_SOURCES
    pure-data/extra/bob~/bob~.c
    pure-data/extra/bonk~/bonk~.c
    pure-data/extra/choice/choice.c
    pure-data/extra/fiddle~/fiddle~.c
    pure-data/extra/loop~/loop~.c
    pure-data/extra/lrshift~/lrshift~.c
    pure-data/extra/pd~/pdsched.c
    pure-data/extra/pd~/pd~.c
    pure-data/extra/pique/pique.c
    pure-data/extra/sigmund~/sigmund~.c
    pure-data/extra/stdout/stdout.c
)

set(LIBPD_SOURCES
    libpd_wrapper/s_libpdmidi.c
    libpd_wrapper/x_libpdreceive.c
    libpd_wrapper/x_libpdreceive.h
    libpd_wrapper/z_hooks.c
    libpd_wrapper/z_hooks.h
    libpd_wrapper/z_libpd.c
)
set(LIBPD_UTILS_SOURCES
    libpd_wrapper/util/ringbuffer.c
    libpd_wrapper/util/ringbuffer.h
    libpd_wrapper/util/z_print_util.c
    libpd_wrapper/util/z_print_util.h
    libpd_wrapper/util/z_queued.c
    libpd_wrapper/util/z_queued.h
)

source_group(pd         FILES ${PD_SOURCES})
source_group(pdextra    FILES ${PD_EXTRA_SOURCES})
source_group(libpd      FILES ${LIBPD_SOURCES})
source_group(libpdutils FILES ${LIBPD_UTILS_SOURCES})

include_directories(libpd_wrapper)
include_directories(pure-data/src)

find_package(Threads REQUIRED)

# create final list of source files
set(SOURCE_FILES ${PD_SOURCES} ${LIBPD_SOURCES})
if(PD_UTILS)
    list(APPEND SOURCE_FILES ${LIBPD_UTILS_SOURCES})
endif()
if(PD_EXTRA)
    list(APPEND SOURCE_FILES ${PD_EXTRA_SOURCES})
endif()

add_library(libpd_static STATIC ${SOURCE_FILES})
add_library(libpd        SHARED ${SOURCE_FILES})

# set the output library name for libpd depending on the settings
set(LIBPD_OUTPUT_NAME     pd)
if (WIN32)
    set(LIBPD_OUTPUT_NAME libpd)
endif()
if (PD_MULTI)
    set(LIBPD_OUTPUT_NAME ${LIBPD_OUTPUT_NAME}-multi)
endif()
if (WIN32)
    set_target_properties(libpd_static PROPERTIES OUTPUT_NAME ${LIBPD_OUTPUT_NAME}-static)
else()
    set_target_properties(libpd_static PROPERTIES OUTPUT_NAME ${LIBPD_OUTPUT_NAME})
endif()
set_target_properties(libpd            PROPERTIES OUTPUT_NAME ${LIBPD_OUTPUT_NAME})

# add appropriate compile definitions
set(LIBPD_COMPILE_DEFINITIONS PD_INTERNAL)

if(NOT MSVC)
    list(APPEND LIBPD_COMPILE_DEFINITIONS HAVE_UNISTD_H=1)
endif()

if(PD_EXTRA)
    list(APPEND LIBPD_COMPILE_DEFINITIONS LIBPD_EXTRA=1)
endif()

if(PD_MULTI)
    list(APPEND LIBPD_COMPILE_DEFINITIONS PDINSTANCE=1 PDTHREADS=1)
endif()

if(NOT PD_LOCALE)
    list(APPEND LIBPD_COMPILE_DEFINITIONS LIBPD_NO_NUMERIC=1)
endif()

if(UNIX)
    list(APPEND LIBPD_COMPILE_DEFINITIONS HAVE_LIBDL)
    target_link_libraries(libpd PUBLIC ${CMAKE_DL_LIBS})
endif()

target_compile_definitions(libpd_static PRIVATE ${LIBPD_COMPILE_DEFINITIONS})
target_compile_definitions(libpd        PRIVATE ${LIBPD_COMPILE_DEFINITIONS})

# link against necessary libraries
if(MSVC)
    target_link_libraries(libpd PUBLIC Ws2_32)
    target_include_directories(libpd_static PUBLIC ${PTHREADS_INCLUDE_DIR})
    target_include_directories(libpd        PUBLIC ${PTHREADS_INCLUDE_DIR})
elseif(WIN32)
    target_link_libraries(libpd PUBLIC -Wl,--export-all-symbols ws2_32 kernel32 -static-libgcc)
endif()
target_link_libraries(libpd PUBLIC ${CMAKE_THREAD_LIBS_INIT})

if(NOT MSVC AND NOT APPLE)
    find_library(M_LIBRARIES m)
    if(M_LIBRARIES)
      target_link_libraries(libpd PUBLIC ${M_LIBRARIES})
    endif()
endif()

if(PD_BUILD_C_EXAMPLES)
    macro(ADD_EXAMPLE name)
        add_executable(${name} samples/c/${name}/${name}.c)
        target_link_libraries(${name} PUBLIC libpd)
        target_link_libraries(${name} PUBLIC m)
        if(DEFINED CMAKE_DEBUG_POSTFIX)
            set_target_properties(${name} PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})
        endif()
    endmacro(ADD_EXAMPLE)

    ADD_EXAMPLE(pdtest)
    if(PD_MULTI)
        ADD_EXAMPLE(pdtest_multi)
        if(NOT MSVC)
            # uses gettimeofday()
            ADD_EXAMPLE(pdtest_gui)
        endif()
    endif()
    ADD_EXAMPLE(pdtest_thread)
endif()

if(PD_BUILD_JAVA)

# convert makefile to cmake
# here's the makefile parts that need to be converted:

#ifeq ($(UNAME), Darwin)  # Mac
#SOLIB_EXT = dylib
#PDNATIVE_SOLIB_EXT = jnilib
#PDNATIVE_PLATFORM = mac
#PDNATIVE_ARCH =
#PLATFORM_CFLAGS = -DHAVE_LIBDL \
#-I/System/Library/Frameworks/JavaVM.framework/Headers
#LDFLAGS = -dynamiclib -ldl -Wl,-no_compact_unwind
#ifeq ($(FAT_LIB), true)
## macOS universal "fat" lib compilation
#MAC_VER = $(shell sw_vers -productVersion | cut -f1 -f2 -d.)
#ifeq ($(shell expr $(MAC_VER) \<= 10.13), 1)
## universal1: macOS 10.6 - 10.13
#FAT_ARCHS ?= -arch i386 -arch x86_64
#endif
#ifeq ($(shell expr $(MAC_VER) \>= 11.0), 1)
## universal2: macOS 11.0+
#FAT_ARCHS ?= -arch arm64 -arch x86_64
#endif
#FAT_ARCHS ?= -arch $(PLATFORM_ARCH)
#PLATFORM_CFLAGS += $(FAT_ARCHS)
#LDFLAGS += $(FAT_ARCHS)
#endif
#CSHARP_LDFLAGS = $(LDFLAGS)
#JAVA_LDFLAGS = -framework JavaVM $(LDFLAGS)
#else
#ifeq ($(OS), Windows_NT)  # Windows, use Mingw
#CC ?= gcc
#SOLIB_EXT = dll
#SOLIB_PREFIX =
#LIBPD_IMPLIB = libs/libpd.lib
#LIBPD_DEF = libs/libpd.def
#PDNATIVE_PLATFORM = windows
#PLATFORM_CFLAGS = -DWINVER=0x502 -DWIN32 -D_WIN32 -DPD_INTERNAL \
#-I"$(JAVA_HOME)/include" -I"$(JAVA_HOME)/include/win32"
#MINGW_LDFLAGS = -shared -Wl,--export-all-symbols -lws2_32 -lkernel32 -static-libgcc
#LDFLAGS = $(MINGW_LDFLAGS) -Wl,--output-def=$(LIBPD_DEF) \
#-Wl,--out-implib=$(LIBPD_IMPLIB)
#CSHARP_LDFLAGS = $(MINGW_LDFLAGS) -Wl,--output-def=libs/libpdcsharp.def \
#-Wl,--out-implib=libs/libpdcsharp.lib
#JAVA_LDFLAGS = $(MINGW_LDFLAGS) -Wl,--kill-at
#else  # Linux or *BSD
#SOLIB_EXT = so
#PLATFORM_CFLAGS = -Wno-int-to-pointer-cast -Wno-pointer-to-int-cast -fPIC
#LDFLAGS = -shared -Wl,-Bsymbolic
#ifeq ($(UNAME), Linux)
#PDNATIVE_PLATFORM = linux
#JAVA_HOME ?= /usr/lib/jvm/default-java
#PLATFORM_CFLAGS += -I"$(JAVA_HOME)/include/linux" -I"$(JAVA_HOME)/include" -DHAVE_LIBDL
#LDFLAGS += -ldl
#else ifeq ($(UNAME), FreeBSD)
#PDNATIVE_PLATFORM = FreeBSD
#JAVA_HOME ?= /usr/local/openjdk8
#PLATFORM_CFLAGS += -I"$(JAVA_HOME)/include/"
#endif
#CSHARP_LDFLAGS = $(LDFLAGS)
#JAVA_LDFLAGS = $(LDFLAGS)
#endif
#endif



# javalib: $(JNIH_FILE) $(PDJAVA_JAR)
#
#$(JNIH_FILE): $(JAVA_BASE)
#javac -classpath java $^
#javah -o $@ -classpath java org.puredata.core.PdBase
#
#$(PDJAVA_NATIVE): ${PD_FILES:.c=.o} ${LIBPD_UTILS:.c=.o} ${EXTRA_FILES:.c=.o} ${JNI_FILE:.c=.o}
#mkdir -p $(PDJAVA_DIR)
#$(CC) -o $(PDJAVA_NATIVE) $^ -lm -lpthread $(JAVA_LDFLAGS)
#cp $(PDJAVA_NATIVE) libs/
#
#$(PDJAVA_JAR): $(PDJAVA_NATIVE) $(PDJAVA_JAR_CLASSES)
#javac -classpath java -d $(PDJAVA_BUILD) $(PDJAVA_JAR_CLASSES)
#jar -cvf $(PDJAVA_JAR) -C $(PDJAVA_BUILD) org/puredata/
#
#javadoc: $(PDJAVA_JAR_CLASSES)
#javadoc -d $(PDJAVA_DOC) -sourcepath java org.puredata.core
#
#javasrc: $(PDJAVA_SRC)

# portaudio backend?
#ifeq ($(PORTAUDIO), true)
#JNI_SOUND = jni/z_jni_pa.c
#JAVA_LDFLAGS := $(JAVA_LDFLAGS) -lportaudio
#ifeq ($(UNAME), Darwin)  # Mac
#JAVA_LDFLAGS := $(JAVA_LDFLAGS) \
#-framework CoreAudio -framework AudioToolbox \
#-framework AudioUnit -framework CoreServices
#endif
#endif

#JNI_FILE = libpd_wrapper/util/ringbuffer.c libpd_wrapper/util/z_queued.c $(JNI_SOUND)
#JNIH_FILE = jni/z_jni.h
#JAVA_BASE = java/org/puredata/core/PdBase.java

#PDJAVA_BUILD = java-build
#PDJAVA_DIR = $(PDJAVA_BUILD)/org/puredata/core/natives/$(PDNATIVE_PLATFORM)/$(PDNATIVE_ARCH)
#PDJAVA_NATIVE = $(PDJAVA_DIR)/$(SOLIB_PREFIX)pdnative.$(PDNATIVE_SOLIB_EXT)
#PDJAVA_JAR = libs/libpd.jar
#PDJAVA_SRC = libs/libpd-sources.jar
#PDJAVA_DOC = javadoc
#PDJAVA_JAR_CLASSES = \
#java/org/puredata/core/PdBase.java \
#java/org/puredata/core/PdBaseLoader.java \
#java/org/puredata/core/NativeLoader.java \
#java/org/puredata/core/PdListener.java \
#java/org/puredata/core/PdMidiListener.java \
#java/org/puredata/core/PdMidiReceiver.java \
#java/org/puredata/core/PdReceiver.java \
#java/org/puredata/core/utils/IoUtils.java \
#java/org/puredata/core/utils/PdDispatcher.java




# the cmake version:

set (PDJAVA_JAR_CLASSES
        java/org/puredata/core/PdBase.java
        java/org/puredata/core/PdBaseLoader.java
        java/org/puredata/core/NativeLoader.java
        java/org/puredata/core/PdListener.java
        java/org/puredata/core/PdMidiListener.java
        java/org/puredata/core/PdMidiReceiver.java
        java/org/puredata/core/PdReceiver.java
        java/org/puredata/core/utils/IoUtils.java
        java/org/puredata/core/utils/PdDispatcher.java
)



set (PDJAVA_SAMPLES_CLASSES
        samples/java/sound/com/noisepages/nettoyeur/libpd/sample/JavaSoundSample.java
        samples/java/sound/com/noisepages/nettoyeur/libpd/sample/JavaSoundThread.java
 #       samples/java/test/org/puredata/core/PdBaseTest.java
 #       samples/java/test/org/puredata/core/utils/PdDispatcherTest.java
        )

if (APPLE)
message (STATUS "Building Java JNI for macOS")

    include(FindJNI)
    find_package(JNI REQUIRED)

    if (NOT JNI_FOUND)
        message (STATUS "Could not find JNI, retrying for JDK.")
        execute_process(COMMAND /usr/libexec/java_home OUTPUT_VARIABLE JAVA_HOME OUTPUT_STRIP_TRAILING_WHITESPACE)
        message (STATUS "my JAVA_HOME=${JAVA_HOME}")
        if (NOT JAVA_HOME)
            message (FATAL_ERROR "Could not find JDK Java home, fatal.")
            return()
        endif ()

        set(JAVA_JNI_INCLUDE ${JAVA_HOME}/include)
        if (APPLE)
            set(JAVA_JNI_INCLUDE ${JAVA_JNI_INCLUDE} ${JAVA_JNI_INCLUDE}/darwin )
        endif()
        message (STATUS "my JAVA_JNI_INCLUDE=${JAVA_JNI_INCLUDE}")

        message(STATUS "here ${JAVA_HOME}/lib/jli")
        find_library(JAVA_JNI_LIB NAMES jli HINTS "${JAVA_HOME}/lib/jli" "${JAVA_HOME}/jre/lib/jli")
        message (STATUS "my JAVA_JNI_LIB=${JAVA_JNI_LIB}")

        if (NOT (JAVA_JNI_INCLUDE OR NOT JAVA_JNI_LIB))
            message (FATAL_ERROR "Could not find JDK Java JNI, fatal.")
            return()
        else ()
            message (STATUS "JDK resolved.")
            include_directories(${JAVA_JNI_INCLUDE})
        endif ()
    else()
        message (STATUS "JNI_INCLUDE_DIRS=${JNI_INCLUDE_DIRS}")
        set(JAVA_JNI_INCLUDE ${JNI_INCLUDE_DIRS})
        message (STATUS "JNI_LIBRARIES=${JNI_LIBRARIES}")
        set(JAVA_JNI_LIB ${JNI_LIBRARIES})
    endif()
else ()
    if(NOT ANDROID)
        #include(FindJNI)

        include(UseJava)
        if(DEFINED ENV{JAVA_HOME})
            set(JAVA_HOME "$ENV{JAVA_HOME}")
        else()
            set(JAVA_HOME /opt/jdk/jdk8)
        endif()
        message(STATUS "JAVA_HOME  variable is defined or set as '${JAVA_HOME}'")

        # set(JAVA_AWT_LIBRARY "${JAVA_HOME}/lib/amd64") #the path to the Java AWT Native Interface (JAWT) library
        # set(JAVA_JVM_LIBRARY "${JAVA_HOME}/lib/amd64/server") #the path to the Java Virtual Machine (JVM) library
        # set(JAVA_INCLUDE_PATH "${JAVA_HOME}/include")  #the include path to jni.h
        # set(JAVA_INCLUDE_PATH2 "${JAVA_HOME}/include/linux") # the include path to jni_md.h and jniport.h
        # set(JAVA_AWT_INCLUDE_PATH "${JAVA_HOME}/include")    # the include path to jawt.h
        #   set(CMAKE_FIND_ROOT_PATH "${JAVA_HOME}")
        #   set(CMAKE_FIND_USE_CMAKE_SYSTEM_PATH "${JAVA_HOME}")

        set(CMAKE_FIND_DEBUG_MODE TRUE)
        find_package(JNI ) #TODO@ add REQUIRED, But cant use PATHS ${JAVA_HOME} as it triggers FindProjname.cmake mode
        set(CMAKE_FIND_DEBUG_MODE FALSE)

        if(JNI_FOUND)
            message (STATUS " JNI_INCLUDE_DIRS=${JNI_INCLUDE_DIRS}")
            message (STATUS " JNI_LIBRARIES=${JNI_LIBRARIES}")
        else()
            message (STATUS " JNI is NOT FOUND")
        endif()

        if (NOT JNI_FOUND)
            message (FATAL_ERROR "No JNI found, cannot build")
        else()
            # message (STATUS "JNI_INCLUDE_DIRS=${JNI_INCLUDE_DIRS}")
            set(JAVA_JNI_INCLUDE ${JNI_INCLUDE_DIRS})
            # message (STATUS "JNI_LIBRARIES=${JNI_LIBRARIES}")
            set(JAVA_JNI_LIB ${JNI_LIBRARIES})
        endif()
    endif(NOT ANDROID)
endif ()




# set JNI_FILE
set(JNI_FILE
    libpd_wrapper/util/ringbuffer.c
    libpd_wrapper/util/z_queued.c
    jni/z_jni_plain.c
)

# set include directories to have JAVA_JNI_INCLUDE
include_directories(${JAVA_JNI_INCLUDE})

#determin SOLIB_PREFIX
if (WIN32)
    set(SOLIB_PREFIX "")
else()
    set(SOLIB_PREFIX "lib")
endif()

#determine SOLIB_EXT
if (WIN32)
    set(SOLIB_EXT "dll")
elseif(APPLE)
    set(SOLIB_EXT "dylib")
else()
    set(SOLIB_EXT "so")
endif()


# determine PDNATIVE_PLATFORM
if (WIN32)
    set(PDNATIVE_PLATFORM windows)
elseif (APPLE)
    set(PDNATIVE_PLATFORM mac)
elseif (UNIX)
    set(PDNATIVE_PLATFORM linux)
endif()

#determin PDNATIVE_ARCH
if (CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(PDNATIVE_ARCH x86_64)
elseif (CMAKE_SIZEOF_VOID_P EQUAL 4)
    set(PDNATIVE_ARCH i386)
endif()

# set PD_JAVA_DIR
set(PD_JAVA_DIR ${CMAKE_CURRENT_BINARY_DIR}/classes/org/puredata/core/natives/${PDNATIVE_PLATFORM}/${PDNATIVE_ARCH})

# add all file recusrively from folder to variable
file(GLOB_RECURSE PD_JAVA_JAR ${CMAKE_CURRENT_SOURCE_DIR}/java/org/puredata/core/*.java)
message (" inputs " ${PD_JAVA_JAR})

# set PD_JAVA_JAR SOURCE files
# to all the .java files in /java/
# recursively


add_library(pdnative SHARED  ${JNI_FILE})
# target_link_libraries(libpd_java PUBLIC ${CMAKE_THREAD_LIBS_INIT})

target_link_libraries(pdnative PUBLIC libpd_static)
message ("Output directory ${PD_JAVA_DIR}")
# set the binary build output directory to PD_JAVA_DIR
# for target libpd_java
set_target_properties( pdnative
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${PD_JAVA_DIR}"
        LIBRARY_OUTPUT_DIRECTORY "${PD_JAVA_DIR}"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
        )



# target_link_libraries(libpd_java PUBLIC ${M_LIBRARIES})

# run a post build command to compile and jar the java classes
add_custom_command(TARGET pdnative POST_BUILD
    COMMAND javac -classpath java -d ${CMAKE_CURRENT_BINARY_DIR}/classes ${PDJAVA_JAR_CLASSES}
        COMMAND mkdir -p ${PD_JAVA_DIR}
        COMMAND cp ${CMAKE_CURRENT_BINARY_DIR}/libs/${SOLIB_PREFIX}pdnative.${SOLIB_EXT} ${PD_JAVA_DIR}
        COMMAND jar -cvf ${CMAKE_CURRENT_BINARY_DIR}/pd_java.jar -C ${CMAKE_CURRENT_BINARY_DIR}/classes org
        COMMAND javac -classpath samples/java -classpath ${CMAKE_CURRENT_BINARY_DIR}/pd_java.jar -d ${CMAKE_CURRENT_BINARY_DIR}/sclasses ${PDJAVA_SAMPLES_CLASSES}

        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Compiling and packaging Java classes"
)


endif(PD_BUILD_JAVA)