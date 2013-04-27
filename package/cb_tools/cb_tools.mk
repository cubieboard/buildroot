CB_TOOLS_DIR := $(BUILD_DIR)/cb_tools

$(CB_TOOLS_DIR)/.source : 
	mkdir -pv $(CB_TOOLS_DIR)
	cp -rf package/cb_tools/src/* $(CB_TOOLS_DIR)
	touch $@

$(CB_TOOLS_DIR)/.configured : $(CB_TOOLS_DIR)/.source
	touch $@


cb_tools-binary: $(CB_TOOLS_DIR)/.configured libsysfs
	$(MAKE) CC="$(TARGET_CC)" -C $(CB_TOOLS_DIR)


cb_tools: cb_tools-binary
	$(MAKE) DESTDIR="$(TARGET_DIR)" -C $(CB_TOOLS_DIR) install
	rm -rf $(CB_TOOLS_DIR)/.source $(CB_TOOLS_DIR)/.configured


##############################################################
#
# Add our target
#
#############################################################
ifeq ($(BR2_PACKAGE_CB_TOOLS),y)
TARGETS += cb_tools
endif
