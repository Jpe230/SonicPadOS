# SonicPad OS (Debian)

## Original instructions

It is recommended to use Ubuntu18.04 to compile sonic_pad_os, and install the related Package dependency "sudo apt install git gcc gawk flex libc6:i386 libstdc++6:i386 lib32z1 libncurses5 libncurses5-dev python g++ libz-dev libssl-dev make p7zip-full"
1. Download the code: git clone https://github.com/CrealityTech/sonic_pad_os.git
2. Download the dl package, save the downloaded dl package in the sonic_pad_os/dl directory.
   Download link: https://klipper.cxswyjy.com/download/sonic_dl/
3. Enter the root directory of sonic_pad_os and execute the script ./scripts/prepare.sh
4. Compile steps:  
  4.1 `source build/envsetup.sh`  
  4.2 `lunch 6`  
  4.3 `make -j2 && pack`  
  4.4 `swupdate_pack_swu -ab`  

5. U flash drive upgrade method: Copy the config.ini and t800-sonic_lcd-ab_1.0.6.48.57.swu files in the sonic_pad_os/out/r818-sonic_lcd/ directory to the root directory of the U flash drive. The firmware version number must be greater than the current version number of the device, otherwise the upgrade box will not pop up.
6. Computer upgrade method: The image path generated after compilation is sonic_pad_os/out/r818-sonic_lcd/t800-sonic_lcd_uart0.img, please refer to the link for the burning tool and upgrade method: https://github.com/CrealityOfficial/Creality_Sonic_Pad_Firmware


## Replace Tine with Debian or other Distro

Since the SOC used in the SonicPad isn't well supported by the mainline kernel, we will need to use the exisiting kernel from Allwinner, and replace the generated `rootfs.img` with a rootfs of our choice, the steps are as follow:

### 1) Edit `sys_partions.fex`

  1. Increase the `rootfs` partition inside `sys_partition.fex` (size is in blocks. 1 block = 512b)   
  2. Remove unused partitions

### 2) Compile Tina/OpenWRt with EXT4 support: 
 
  1. `make menuconfig`  
  2. Go to `Target Image`  
  3. Deselect `squashfs`  
  4. Select `ext4` and configure the inodes and journaling  
  5. Increase `Root filesystem partition size` to fit your custom rootfs  
    * Dont make the fs partition the size of rootfs otherwise the final image will be too big, you can always resize the fs with `resize2fs`       

### 3) Create a rootfs  

You are free to use whatever rootfs you want, I personally used `debootrap` to create a fs. There a several guides or even scripts to generate a clean `rootfs`

Keep in mind that if you are not willing to take apart the SonicPad for UART, you will need to configure SSH, networking and create init scripts for loading the kernel modules for WIFI.

### 4) Mount and replace rootfs

Assuming you already compiled Tina with a ext4 fs:
  1. Mount the Tina `rootfs.img`  
  2. Remove contents
  3. Replace with your rootfs
  4. Copy compiled kernel modules
  5. Unmount the image
  5. Repair image:
  7. Run `pack`
  8. Flash the pad using LiveSuit or similar

To repair the image you can use:
```
tune2fs -O^resize_inode rootfs.img
fsck.ext4 rootfs.img
```

### 5) Configure WIFI (Debian)

  1. Load XR819 kernel module
  2. Create a `wpa_supplicant.conf`
  3. Enable the interface `wlan0` (possibly twice(?))
  4. Execute: `wpa_supplicant -D nl80211 -i wlan0 -c *your conf*  -B`
  5. Execute: `dhclient -v wlan0`

You can create a service to do this automatically at boot, example:

```
#!/bin/bash

insmod /lib/modules/4.9.191/xr819_mac.ko
insmod /lib/modules/4.9.191/xr819_core.ko
insmod /lib/modules/4.9.191/xr819_wlan.ko

ifconfig wlan0 up
ifconfig wlan0 up # Needs to be called twice, dunno why it fails the first time

wpa_supplicant -D nl80211 -i wlan0 -c /etc/wpa_supplicant.conf  -B
dhclient -v wlan0
```

### 6) Enable USB host

Since the "CAM" port acts as USB Client as default, you can reenable it to act as a USB Host by using:

```
cat /sys/devices/platform/soc/usbc0/usb_host
```

### 7) Brightness

Cross-compile or compile directly with the Pad, the package: "Creality/Brightness", you will need to source the header `sunxi_display_v2.h` from the package `libuapi` from the Allwinner packages folder.

### Additional

#### Display

The framebuffer is located at `/dev/fb0`. I haven't tried using HW acceleration or any GUI app.