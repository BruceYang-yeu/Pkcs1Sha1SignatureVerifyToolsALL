# Copyright 2013 The Android Open Source Project

LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := verify_test
LOCAL_SRC_FILES := \
            test/verify_test.c \
            rsalib/rsa.c  \
            rsalib/sha.c  \
            rsalib/verifier.c\
LOCAL_C_INCLUDES := $(LOCAL_PATH)/inc            
include $(BUILD_EXECUTABLE)

