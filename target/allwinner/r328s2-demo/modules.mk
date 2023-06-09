#
# Copyright (C) 2015-2016 Allwinner
#
# This is free software, licensed under the GNU General Public License v2.
# See /build/LICENSE for more information.

define KernelPackage/net-rtl8723ds
  SUBMENU:=$(WIRELESS_MENU)
  TITLE:=RTL8723DS support (staging)
  DEPENDS:= +r8723ds-firmware
  FILES:=$(LINUX_DIR)/drivers/net/wireless/rtl8723ds/8723ds.ko
  AUTOLOAD:=$(call AutoProbe,8723ds)
endef

define KernelPackage/net-rtl8723ds/description
 Kernel modules for RealTek RTL8723DS support
endef

$(eval $(call KernelPackage,net-rtl8723ds))

define KernelPackage/net-rtl8822cs
  SUBMENU:=$(WIRELESS_MENU)
  TITLE:=RTL8723CS support (staging)
  DEPENDS:=
  KCONFIG:=CONFIG_RTL8822CS
  FILES:=$(LINUX_DIR)/drivers/net/wireless/rtl8822cs/8822cs.ko
  AUTOLOAD:=$(call AutoProbe,8822cs)
endef

define KernelPackage/net-rtl8822cs/description
 Kernel modules for RealTek RTL8822CS support
endef

$(eval $(call KernelPackage,net-rtl8822cs))

define KernelPackage/net-xr829
  SUBMENU:=$(WIRELESS_MENU)
  TITLE:=xr829 support (staging)
  DEPENDS:=
  FILES:=$(LINUX_DIR)/drivers/net/wireless/xr829/wlan/xradio_core.ko
  FILES+=$(LINUX_DIR)/drivers/net/wireless/xr829/wlan/xradio_wlan.ko
  FILES+=$(LINUX_DIR)/drivers/net/wireless/xr829/umac/xradio_mac.ko
  AUTOLOAD:=$(call AutoProbe, xradio_mac xradio_core xradio_wlan)
endef

define KernelPackage/net-xr829/description
 Kernel modules for xr829 support
endef

$(eval $(call KernelPackage,net-xr829))

define KernelPackage/net-xr829btlpm
  SUBMENU:=$(WIRELESS_MENU)
  TITLE:=xradio 829 bt lpm support (staging)
  DEPENDS:=
  FILES:=$(LINUX_DIR)/drivers/bluetooth/xradio_btlpm.ko
  AUTOLOAD:=$(call AutoProbe,xradio_btlpm)
endef

define KernelPackage/net-xr829btlpm/description
 Kernel modules for xradio bt lpm support
endef

$(eval $(call KernelPackage,net-xr829btlpm))

define KernelPackage/net-cypress
  SUBMENU:=$(WIRELESS_MENU)
  TITLE:=cypress moudles support (staging)
  DEPENDS:=
  KCONFIG:=CONFIG_CYWDHD
  FILES:=$(LINUX_DIR)/drivers/net/wireless/cywdhd/bcmdhd.ko
  AUTOLOAD:=$(call AutoProbe,bcmdhd)
endef

define KernelPackage/net-cypress/description
 Kernel modules for cypress net support
endef

$(eval $(call KernelPackage,net-cypress))

define KernelPackage/net-xr819
  SUBMENU:=$(WIRELESS_MENU)
  TITLE:=xr819 support (staging)
  DEPENDS:=
  FILES:=$(LINUX_DIR)/drivers/net/wireless/xr819/wlan/xradio_core.ko
  FILES+=$(LINUX_DIR)/drivers/net/wireless/xr819/wlan/xradio_wlan.ko
  FILES+=$(LINUX_DIR)/drivers/net/wireless/xr819/umac/xradio_mac.ko
  AUTOLOAD:=$(call AutoProbe, xradio_mac xradio_core xradio_wlan)
endef

define KernelPackage/net-xr819/description
 Kernel modules for xr819 support
endef

$(eval $(call KernelPackage,net-xr819))

define KernelPackage/net-xr819btlpm
  SUBMENU:=$(WIRELESS_MENU)
  TITLE:=xradio 819 bt lpm support (staging)
  DEPENDS:=
  FILES:=$(LINUX_DIR)/drivers/bluetooth/xradio_btlpm.ko
  AUTOLOAD:=$(call AutoProbe,xradio_btlpm)
endef

define KernelPackage/net-xr819btlpm/description
 Kernel modules for xradio bt lpm support
endef

$(eval $(call KernelPackage,net-xr819btlpm))
