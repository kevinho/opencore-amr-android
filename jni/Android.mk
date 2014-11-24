LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

PV_TOP := $(LOCAL_PATH)/opencore
PV_INCLUDES := $(LOCAL_PATH)/oscl \
					$(PV_TOP)/codecs_v2/audio/gsm_amr/amr_nb/common/include \
					$(PV_TOP)/codecs_v2/audio/gsm_amr/amr_nb/common/src \
					$(PV_TOP)/codecs_v2/audio/gsm_amr/amr_nb/dec/include \
					$(PV_TOP)/codecs_v2/audio/gsm_amr/amr_nb/dec/src \
					$(PV_TOP)/codecs_v2/audio/gsm_amr/amr_nb/enc/include \
					$(PV_TOP)/codecs_v2/audio/gsm_amr/amr_nb/enc/src \
					$(PV_TOP)/codecs_v2/audio/gsm_amr/common/dec/include

include $(PV_TOP)/codecs_v2/audio/gsm_amr/amr_nb/common/Android.mk
include $(PV_TOP)/codecs_v2/audio/gsm_amr/amr_nb/dec/Android.mk
include $(PV_TOP)/codecs_v2/audio/gsm_amr/amr_nb/enc/Android.mk

include $(CLEAR_VARS)

LOCAL_PATH := $(PV_TOP)/..
LOCAL_MODULE := amr-codec
LOCAL_SRC_FILES := $(LOCAL_PATH)/amr_encoder.cpp \
				$(LOCAL_PATH)/amr_decoder.cpp \
				$(LOCAL_PATH)/wrapper.cpp

LOCAL_C_INCLUDES := $(PV_INCLUDES)


LOCAL_LDLIBS := -L$(SYSROOT)/usr/lib -llog
LOCAL_SHARED_LIBRARIES := libpv_amr_nb_common_lib \
						libpvencoder_gsmamr \
						libpvdecoder_gsmamr


include $(BUILD_SHARED_LIBRARY)
