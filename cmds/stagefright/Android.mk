ifeq ($(BUILD_WITH_STAGEFRIGHT),true)

LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_SRC_FILES:= \
	stagefright.cpp

LOCAL_SHARED_LIBRARIES := \
	libstagefright

LOCAL_C_INCLUDES:= \
	frameworks/base/media/libstagefright \
	$(TOP)/external/opencore/extern_libs_v2/khronos/openmax/include \
        $(TOP)/external/opencore/android

LOCAL_CFLAGS += -Wno-multichar

LOCAL_MODULE:= stagefright

include $(BUILD_EXECUTABLE)

################################################################################

include $(CLEAR_VARS)

LOCAL_SRC_FILES:=         \
        record.cpp

LOCAL_SHARED_LIBRARIES := \
	libstagefright

LOCAL_C_INCLUDES:= \
	frameworks/base/media/libstagefright \
	$(TOP)/external/opencore/extern_libs_v2/khronos/openmax/include \
        $(TOP)/external/opencore/android

LOCAL_CFLAGS += -Wno-multichar

LOCAL_MODULE:= record

include $(BUILD_EXECUTABLE)

################################################################################

# include $(CLEAR_VARS)
# 
# LOCAL_SRC_FILES:=         \
#         play.cpp
# 
# LOCAL_SHARED_LIBRARIES := \
# 	libstagefright
# 
# LOCAL_C_INCLUDES:= \
# 	frameworks/base/media/libstagefright \
# 	$(TOP)/external/opencore/extern_libs_v2/khronos/openmax/include \
#         $(TOP)/external/opencore/android
# 
# LOCAL_CFLAGS += -Wno-multichar
# 
# LOCAL_MODULE:= play
# 
# include $(BUILD_EXECUTABLE)

endif
