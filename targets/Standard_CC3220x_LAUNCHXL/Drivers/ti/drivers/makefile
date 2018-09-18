#
#  ======== makefile ======== 
#

CORESDK_INSTALL_DIR ?= $(abspath ../../..)
include $(CORESDK_INSTALL_DIR)/imports.mak

#
# Set XDCARGS to some of the variables above.  XDCARGS are passed
# to the XDC build engine... which will load tirtos.bld... which will
# extract these variables and use them to determine what to build and which
# toolchains to use.
#
# Note that not all of these variables need to be set to something valid.
# If you leave one or more of them unassigned, the libraries for that target
# will not be built.
#
# Some background is here:
#     http://rtsc.eclipse.org/docs-tip/Command_-_xdc#Environment_Variables
#
XDCARGS= \
    ti.targets.arm.elf.M4=\"$(CCS_ARMCOMPILER)\" \
    gnu.targets.arm.M4=\"$(GCC_ARMCOMPILER)\" \
    iar.targets.arm.M4=\"$(IAR_ARMCOMPILER)\" \
    CC32XXWARE=$(CORESDK_INSTALL_DIR)

#
# Set XDCPATH to contain necessary repositories.
#
XDCPATH=$(CORESDK_INSTALL_DIR)/source;$(CORESDK_INSTALL_DIR)/kernel/tirtos/packages

#
# Set XDCOPTIONS.  Use -v for a verbose build.
#
# XDCOPTIONS=v
XDCOPTIONS=
export XDCOPTIONS

j = 1
#
# Set XDC executable command
# Note that XDCBUILDCFG points to the coresdk.bld file which uses
# the arguments specified by XDCARGS
#
XDC = $(XDC_INSTALL_DIR)/xdc -j $(j) XDCBUILDCFG=$(CORESDK_INSTALL_DIR)/kernel/tirtos/packages/coresdk.bld

all:
	@ echo building packages ...
	@ $(XDC) XDCARGS="$(XDCARGS)" XDCPATH="$(XDCPATH)" all -PR .

clean:
	@ echo cleaning packages ...
	@ $(XDC) XDCARGS="$(XDCARGS)" XDCPATH="$(XDCPATH)" clean -PR .
