SUMMARY = "Demo embedded Linux image with systemd and custom application"
DESCRIPTION = "Core image with systemd init, myapp package, and auto-start service"

require recipes-core/images/core-image-minimal.bb

IMAGE_INSTALL:append = " myapp"

inherit core-image
