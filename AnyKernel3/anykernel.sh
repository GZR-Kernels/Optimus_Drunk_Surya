# AnyKernel3 Ramdisk Mod Script
# osm0sis @ xda-developers

## AnyKernel setup
# begin properties
properties() { '
kernel.string=Optimus Drunk Kernel by GtrCraft
do.devicecheck=1
do.modules=0
do.cleanup=1
do.cleanuponabort=0
device.name1=surya
device.name2=karna
device.name3=
device.name4=
device.name5=
supported.versions=
'; } # end properties

# shell variables
block=/dev/block/bootdevice/by-name/boot;
is_slot_device=auto;
ramdisk_compression=auto;


## AnyKernel methods (DO NOT CHANGE)
# import patching functions/variables - see for reference
. tools/ak3-core.sh;


## AnyKernel file attributes
# set permissions/ownership for included ramdisk files
set_perm_recursive 0 0 750 750 $ramdisk/*;


## AnyKernel install
dump_boot;

# Begin Ramdisk Changes

# migrate from /overlay to /overlay.d to enable SAR Magisk
if [ -d $ramdisk/overlay ]; then
  rm -rf $ramdisk/overlay;
fi;

write_boot;
## end install

