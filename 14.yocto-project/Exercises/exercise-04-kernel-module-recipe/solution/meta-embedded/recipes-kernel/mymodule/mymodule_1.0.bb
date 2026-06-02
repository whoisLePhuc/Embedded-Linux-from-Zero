SUMMARY = "Example kernel module"
DESCRIPTION = "An out-of-tree kernel module built by Yocto"
LICENSE = "GPL-2.0-only"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta/files/common-licenses/GPL-2.0-only;md5=801f80980d171dd6425610833a22dbe6"

inherit module

SRC_URI = "file://Makefile \
           file://hello.c \
           "

S = "${WORKDIR}"

do_configure[depends] += "virtual/kernel:do_shared_workdir"

KERNEL_MODULE_AUTOLOAD:append = " mymodule"
