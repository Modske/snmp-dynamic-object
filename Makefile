include $(TOPDIR)/rules.mk

PKG_NAME:=snmp-mod-dynamic
PKG_RELEASE:=1
PKG_VERSION:=1.0.0

include $(INCLUDE_DIR)/package.mk

define Package/snmp-mod-dynamic
	SUBMENU:=SNMP modules
	TITLE:=snmp-mod-dynamic
	SECTION:=base
	CATEGORY:=Base system
	DEPENDS:=+libnetsnmp +libuci
endef

define Package/snmp-mod-dynamic/description
	Package for testing snmp modules
endef

define Package/snmp-mod-dynamic/install
	$(INSTALL_DIR) $(1)/usr/lib/snmp-mod
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/str_variable.so $(1)/usr/lib/snmp-mod
endef

$(eval $(call BuildPackage,snmp-mod-dynamic))