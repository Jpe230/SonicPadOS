##############################################
# OpenWrt Makefile for eyesee-mpp-framework
#
#
# Most of the variables used here are defined in
# the include directives below. We just need to
# specify a basic description of the package,
# where to build our program, where to find
# the source files, and where to install the
# compiled program on the router.
#
# Be very careful of spacing in this file.
# Indents should be tabs, not spaces, and
# there should be no trailing whitespace in
# lines that are not commented.
#
##############################################
include $(TOPDIR)/rules.mk
include $(BUILD_DIR)/kernel.mk

# Name and release number of this package
PKG_NAME:=eyesee-mpp-V316SdvDragonBoard
PKG_VERSION:=1.0
PKG_RELEASE:=1

PKG_MAINTAINER:=AW-PDC-PD5
# This specifies the directory where we're going to build the program.
# The root build directory, $(BUILD_DIR), is by default the build_mipsel
# directory in your OpenWrt SDK directory
PKG_BUILD_DIR := $(COMPILE_DIR)/$(PKG_NAME)
SRC_COMPILE_DIR := $(PKG_BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)
SRC_CODE_DIR := $(TOPDIR)/external/eyesee-mpp/V316SdvDragonBoard

# This specifies the directory where file depends. when files in it update, increasing compile will be enable.
PKG_FILE_DEPENDS:= $(SRC_CODE_DIR)

include $(BUILD_DIR)/package.mk

############################################################################
#below items are used for mpp configuration of previous EyeseeLinux,
#for smooth migration to tina, we reserve these config items, define them here and export.
#ref to mpp_config.mk of EyeseeLinux.
#[config item], valid in makefile.
#config chip version: AW1721
ifeq ($(TARGET_CHIP), sun8iw12p1)
    MPPCFG_CHIP := AW1721
else ifeq ($(TARGET_CHIP), sun8iw16p1)
    MPPCFG_CHIP := AW1816
else
#    $(error fatal error! $(TARGET_CHIP) is not support by eyesee-mpp!)
endif
export MPPCFG_CHIP
#config write file method: LINUX_VFS, DIRECT_IO
ifeq ($(CONFIG_mpp_fwrite_method_vfs),y)
    MPPCFG_FILE_SYSTEM := LINUX_VFS
else ifeq ($(CONFIG_mpp_fwrite_method_directIO),y)
    MPPCFG_FILE_SYSTEM := DIRECT_IO
else
    MPPCFG_FILE_SYSTEM := LINUX_VFS
endif
export MPPCFG_FILE_SYSTEM
#config if use hw display method: true, false
ifeq ($(CONFIG_mpp_hw_display),y)
    MPPCFG_HW_DISPLAY := true
else
    MPPCFG_HW_DISPLAY := false
endif
export MPPCFG_HW_DISPLAY
#config if include libmpp_ise.so and enable ise function.
ifeq ($(CONFIG_mpp_ise),y)
    MPPCFG_ISE := Y
else
    MPPCFG_ISE := N
endif
export MPPCFG_ISE
#config if include libmpp_ise.so and enable ise mo module.
ifeq ($(CONFIG_mpp_ise_mo),y)
    MPPCFG_ISE_MO := Y
else
    MPPCFG_ISE_MO := N
endif
export MPPCFG_ISE_MO
#config if include libmpp_ise.so and enable ise bi module.
ifeq ($(CONFIG_mpp_ise_bi),y)
    MPPCFG_ISE_BI := Y
else
    MPPCFG_ISE_BI := N
endif
export MPPCFG_ISE_BI
#config if include libmpp_ise.so and enable ise bi soft module.
ifeq ($(CONFIG_mpp_ise_bi_soft),y)
    MPPCFG_ISE_BI_SOFT := Y
else
    MPPCFG_ISE_BI_SOFT := N
endif
export MPPCFG_ISE_BI_SOFT
#config if include libmpp_ise.so and enable ise sti module.
ifeq ($(CONFIG_mpp_ise_sti),y)
    MPPCFG_ISE_STI := Y
else
    MPPCFG_ISE_STI := N
endif
export MPPCFG_ISE_STI
#config if include libmpp_eis.so and enable eis function.(Electronic Image Stabilization)
ifeq ($(CONFIG_mpp_eis),y)
    MPPCFG_EIS := Y
else
    MPPCFG_EIS := N
endif
export MPPCFG_EIS
#config if include libai_MOD.so and enable MOD(Motion Object Detection) detect.
ifeq ($(CONFIG_mpp_mod),y)
    MPPCFG_MOD := Y
else
    MPPCFG_MOD := N
endif
export MPPCFG_MOD
#config if include libeve_face.so and enable face detect.
ifeq ($(CONFIG_mpp_eveface),y)
    MPPCFG_EVEFACE := Y
else
    MPPCFG_EVEFACE := N
endif
export MPPCFG_EVEFACE
#config if include libai_VLPR.so and enable VLPR(Vehicle License Plate Recognition) detect.
ifeq ($(CONFIG_mpp_vlpr),y)
    MPPCFG_VLPR := Y
else
    MPPCFG_VLPR := N
endif
export MPPCFG_VLPR
#config if include libai_HCNT.so and enable HCNT(human count) detect.
ifeq ($(CONFIG_mpp_hcnt),y)
    MPPCFG_HCNT := Y
else
    MPPCFG_HCNT := N
endif
export MPPCFG_HCNT
#config if include libai_BDII.so and enable BDII() detect.
ifeq ($(CONFIG_mpp_bdii),y)
    MPPCFG_BDII := Y
else
    MPPCFG_BDII := N
endif
export MPPCFG_BDII
#config if enable kfc.
ifeq ($(CONFIG_mpp_kfc),y)
    MPPCFG_USE_KFC := Y
else
    MPPCFG_USE_KFC := N
endif
export MPPCFG_USE_KFC
#config if include libmpp_uvc.so
ifeq ($(CONFIG_mpp_uvc),y)
    MPPCFG_UVC := Y
else
    MPPCFG_UVC := N
endif
export MPPCFG_UVC
#prebuild all AI libs and component by static
ifeq ($(CONFIG_mpp_compile_static_lib),y)
    MPPCFG_COMPILE_STATIC_LIB := Y
else
    MPPCFG_COMPILE_STATIC_LIB := N
endif
export MPPCFG_COMPILE_STATIC_LIB
#build component to dynamic so if possible
ifeq ($(CONFIG_mpp_compile_dynamic_lib),y)
    MPPCFG_COMPILE_DYNAMIC_LIB := Y
else
    MPPCFG_COMPILE_DYNAMIC_LIB := N
endif
export MPPCFG_COMPILE_DYNAMIC_LIB

############################################################################
#below items are used for mpp custom_aw configuration
#config in eyesee-mpp-middleware, custom_aw alse need to export.
ifeq ($(CONFIG_imx317),y)
    SENSOR_NAME:=imx317
else ifeq ($(CONFIG_imx278),y)
    SENSOR_NAME:=imx278
else ifeq ($(CONFIG_imx386),y)
    SENSOR_NAME:=imx386
else
    SENSOR_NAME:=
endif
export SENSOR_NAME
#export BOARD_TYPE by TARGET_PRODUCT.
#ifneq (,$(findstring $(strip $(TARGET_PRODUCT)),v316_perfnor v316_sdv))
ifeq ("$(strip $(TARGET_PRODUCT))", "v316_perfnor")
    BOARD_TYPE := PER1
else ifeq ("$(strip $(TARGET_PRODUCT))", "v316_sdv")
    BOARD_TYPE := PRO
else ifeq ("$(strip $(TARGET_PRODUCT))", "v316_SdvDragonBoard")
    BOARD_TYPE := PRO
else ifeq ("$(strip $(TARGET_PRODUCT))", "v536_cdr")
    BOARD_TYPE := C26A
else
    $(warning "ERROR: TARGET_PRODUCT:$(TARGET_PRODUCT) is unknown, default config will be used")
    BOARD_TYPE := PER1
endif
export BOARD_TYPE
#config if boot mode is normal or fast
export BOARD_BOOT_TYPE="normal"

# Specify package information for this program.
# The variables defined here should be self explanatory.
# If you are running Kamikaze, delete the DESCRIPTION
# variable below and uncomment the Kamikaze define
# directive for the description below
define Package/$(PKG_NAME)
  SECTION:=eyesee-mpp-V316SdvDragonBoard
  CATEGORY:=Allwinner
  SUBMENU:=eyesee-mpp
  TITLE:=eyesee-mpp-V316SdvDragonBoard for Tina Linux
  DEPENDS:= +eyesee-mpp-framework +eyesee-mpp-external +libjpeg +libpng +eyesee-minigui +tslib +libnl-tiny
endef

define Package/$(PKG_NAME)/config
#source "$(SOURCE)/Config.in"
endef

# Uncomment portion below for Kamikaze and delete DESCRIPTION variable above
define Package/$(PKG_NAME)/description
  eyesee-mpp-V316SdvDragonBoard is our solutions for v316 sdv dragonboard. It mainly use eyesee-mpp-framework to implement
  multimedia function.
endef

# Specify what needs to be done to prepare for building the package.
# In our case, we need to copy the source files to the build directory.
# This is NOT the default.  The default uses the PKG_SOURCE_URL and the
# PKG_SOURCE which is not defined here to download the source from the web.
# In order to just build a simple program that we have just written, it is
# much easier to do it this way.

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	if [ -d $(SRC_COMPILE_DIR) ]; then \
		rm -rf $(SRC_COMPILE_DIR); \
	fi
	ln -s $(SRC_CODE_DIR) $(SRC_COMPILE_DIR)
endef

define Build/Configure
endef

define Build/Compile
	$(MAKE) -C $(SRC_COMPILE_DIR) \
		ARCH="$(TARGET_ARCH)" \
		AR="$(TARGET_AR)" \
		CC="$(TARGET_CC)" \
		CXX="$(TARGET_CXX)" \
		CFLAGS="$(TARGET_CFLAGS)" \
		CXXFLAGS="$(TARGET_CXXFLAGS)" \
		CPPFLAGS="$(TARGET_CPPFLAGS)" \
		LDFLAGS="$(TARGET_LDFLAGS)" \
		all
endef

define Build/Clean
	if [ -d $(SRC_COMPILE_DIR) ]; then \
		MPPCFG_COMPILE_DYNAMIC_LIB=Y \
		MPPCFG_COMPILE_STATIC_LIB=Y \
		$(MAKE) -C $(SRC_COMPILE_DIR) \
			ARCH="$(TARGET_ARCH)" \
			AR="$(TARGET_AR)" \
			CC="$(TARGET_CC)" \
			CXX="$(TARGET_CXX)" \
			CFLAGS="$(TARGET_CFLAGS)" \
			CXXFLAGS="$(TARGET_CXXFLAGS)" \
			CPPFLAGS="$(TARGET_CPPFLAGS)" \
			LDFLAGS="$(TARGET_LDFLAGS)" \
			clean; \
	else \
		echo "$(SRC_COMPILE_DIR) is not exist!"; \
	fi
endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/lib/eyesee-mpp
	$(INSTALL_DIR) $(1)/usr/include/eyesee-mpp/V316SdvDragonBoard
	$(CP) -r $(SRC_COMPILE_DIR)/include $(1)/usr/include/eyesee-mpp/V316SdvDragonBoard
endef

# We do not need to define Build/Configure or Build/Compile directives
# The defaults are appropriate for compiling a simple program such as this one


# Specify where and how to install the program. Since we only have one file,
# the helloworld executable, install it by copying it to the /bin directory on
# the router. The $(1) variable represents the root directory on the router running
# OpenWrt. The $(INSTALL_DIR) variable contains a command to prepare the install
# directory if it does not already exist.  Likewise $(INSTALL_BIN) contains the
# command to copy the binary file from its current location (in our case the build
# directory) to the install directory.
define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/usr/lib/eyesee-mpp
	$(INSTALL_DIR) $(1)/usr/bin
	find $(SRC_COMPILE_DIR)/lib/out -maxdepth 1 \( -name "libminigui_ths*.so*" -o -name "*.so" \) -exec $(CP) {} $(1)/usr/lib/eyesee-mpp \;
	$(INSTALL_DIR) $(1)/etc
	$(CP) $(SRC_COMPILE_DIR)/apps/DragonBoard/config/out/etc/MiniGUI.cfg $(1)/etc/MiniGUI.cfg
	$(CP) $(SRC_COMPILE_DIR)/apps/DragonBoard/config/out/etc/sunxi-keyboard.kl $(1)/etc/sunxi-keyboard.kl
	$(CP) $(SRC_COMPILE_DIR)/apps/DragonBoard/config/out/etc/ts.conf $(1)/etc/ts.conf
	$(INSTALL_DIR) $(1)/etc/init.d
	$(CP) $(SRC_COMPILE_DIR)/apps/DragonBoard/config/out/etc/init.d/S01app $(1)/etc/init.d/S01app
	$(INSTALL_DIR) $(1)/usr/share/minigui/res/audio
	$(CP) $(SRC_COMPILE_DIR)/apps/DragonBoard/res/out/audio/startup.wav $(1)/usr/share/minigui/res/audio
	#$(CP) $(SRC_COMPILE_DIR)/apps/DragonBoard/config/out/etc/init.d/S01app $(1)/etc/init.d/S01app
	$(CP) $(SRC_COMPILE_DIR)/apps/DragonBoard/testcase/tftester/tftester $(1)/usr/bin
	$(CP) $(SRC_COMPILE_DIR)/apps/DragonBoard/testcase/keytester/keytester $(1)/usr/bin
	$(CP) $(SRC_COMPILE_DIR)/apps/DragonBoard/testcase/rtctester/rtctester $(1)/usr/bin
	$(CP) $(SRC_COMPILE_DIR)/apps/DragonBoard/testcase/ddrtester/ddrtester $(1)/usr/bin
	$(CP) $(SRC_COMPILE_DIR)/apps/DragonBoard/testcase/nortester/nortester $(1)/usr/bin
	$(CP) $(SRC_COMPILE_DIR)/apps/DragonBoard/testcase/sample_gyro/sample_gyro $(1)/usr/bin
	$(CP) $(SRC_COMPILE_DIR)/apps/DragonBoard/testcase/wifitester/wifitester $(1)/usr/bin
	$(INSTALL_DIR) $(1)/usr/sbin
	$(CP) $(SRC_COMPILE_DIR)/apps/DragonBoard/testcase/wifitester/wpa_supplicant $(1)/usr/sbin
	$(CP) $(SRC_COMPILE_DIR)/apps/DragonBoard/testcase/wifitester/wpa_cli $(1)/usr/sbin
	$(CP) $(SRC_COMPILE_DIR)/apps/DragonBoard/testcase/sample_virvi2voHdmi/virvi2vohdmi $(1)/usr/bin
	$(CP) $(SRC_COMPILE_DIR)/apps/DragonBoard/testcase/sample_virvi2voHdmi/sample_virvi2vohdmi.conf $(1)/usr/bin
	$(CP) $(SRC_COMPILE_DIR)/apps/DragonBoard/testcase/sample_virvi2vo/virvi2vo $(1)/usr/bin
	$(CP) $(SRC_COMPILE_DIR)/apps/DragonBoard/testcase/sample_virvi2vo/sample_virvi2volcd.conf $(1)/usr/bin
	$(CP) $(SRC_COMPILE_DIR)/apps/DragonBoard/testcase/mictester/mictester $(1)/usr/bin
	$(CP) $(SRC_COMPILE_DIR)/apps/DragonBoard/testcase/mictester/sample_ai.conf $(1)/usr/bin
	$(CP) $(SRC_COMPILE_DIR)/apps/DragonBoard/testcase/mictester/sample_ao.conf $(1)/usr/bin
	$(CP) $(SRC_COMPILE_DIR)/apps/DragonBoard/core/dragonboard $(1)/usr/bin
endef

#define Package/$(PKG_NAME)/InstallDev
#endef

# This line executes the necessary commands to compile our program.
# The above define directives specify all the information needed, but this
# line calls BuildPackage which in turn actually uses this information to
# build a package.

$(eval $(call BuildPackage,$(PKG_NAME)))
