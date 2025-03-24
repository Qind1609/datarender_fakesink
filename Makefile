CUDA_VER?=
CUDA_HOME:=/usr/local/cuda

ifeq ($(CUDA_VER),)
  $(warning CUDA_VER is not set, use default CUDA_HOME)
else
  CUDA_HOME:=/usr/local/cuda-$(CUDA_VER)
endif
$(info CUDA_HOME=$(CUDA_HOME))

NVDS_VERSION?=
DS_PATH=/opt/nvidia/deepstream/deepstream

ifeq ($(NVDS_VERSION),)
  $(info NVDS_VERSION is not set, trying to figure out)
  FULL_PATH:=$(realpath $(DS_PATH))
  ifeq ($(FULL_PATH),)
    $(error $(DS_PATH) is not exist)
  endif
  NVDS_VERSION:=$(shell basename $(FULL_PATH) | cut -f2 -d -)
endif

DS_HOME=/opt/nvidia/deepstream/deepstream-$(NVDS_VERSION)
LIB_INSTALL_DIR=$(DS_HOME)/lib

$(info NVDS_VERSION=$(NVDS_VERSION))
$(info DS_HOME=$(DS_HOME))

TARGET_LIB:= libnvds_3d_fakesink.so

CC:= g++
TARGET_DEVICE = $(shell gcc -dumpmachine | cut -f1 -d -)

CFLAGS:= -fvisibility=hidden -Wall #-Werror

ifeq ($(TARGET_DEVICE),aarch64)
  CFLAGS+= -DPLATFORM_TEGRA
endif

PKGS:= gstreamer-1.0 yaml-cpp

CFLAGS+= -I $(DS_HOME)/sources/includes \
         -I $(DS_HOME)/sources/libs \
         -I $(CUDA_HOME)/include \
         -std=c++14 -fPIC

CFLAGS+= $(shell pkg-config --cflags $(PKGS))

LIBS:= $(shell pkg-config --libs $(PKGS))

LIBS+= -L$(CUDA_HOME)/lib64/ -lcudart -lnvdsgst_helper -lm \
       -L$(LIB_INSTALL_DIR) -lnvdsgst_3d_gst \
		-L$(LIB_INSTALL_DIR) -lnvds_3d_common \
		-lcuda -Wl,-rpath,$(LIB_INSTALL_DIR)

TARGET_LIB_SRCS:= $(wildcard ./*.cpp)
TARGET_LIB_INCS:= $(wildcard ./*.h)
TARGET_LIB_OBJS:= $(TARGET_LIB_SRCS:.cpp=.o)

LDFLAGS:= -shared -Wl,--start-group $(LIBS) -Wl,--end-group

all: $(TARGET_LIB)

$(TARGET_LIB): $(TARGET_LIB_OBJS) Makefile
	$(CC) -o $(TARGET_LIB) $(TARGET_LIB_OBJS) $(LDFLAGS)

%.o: %.cpp $(TARGET_LIB_INCS) Makefile
	$(CC) -c -o $@ $(CFLAGS) $<

install: $(TARGET_LIB)
	cp -rv $(TARGET_LIB) $(LIB_INSTALL_DIR)

clean:
	rm -rf $(TARGET_LIB_OBJS) $(TARGET_LIB)
