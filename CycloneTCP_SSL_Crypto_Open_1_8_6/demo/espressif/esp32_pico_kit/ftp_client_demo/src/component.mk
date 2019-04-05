CFLAGS += -DESP32_PICO_KIT -D_WINSOCK_H -D__error_t_defined

COMPONENT_SRCDIRS := . \
	common \
	cyclone_tcp/drivers/mac \
	cyclone_tcp/drivers/phy \
	cyclone_tcp/drivers/wifi \
	cyclone_tcp/core \
	cyclone_tcp/dhcp \
	cyclone_tcp/dns \
	cyclone_tcp/ipv4 \
	cyclone_tcp/ipv6 \
	cyclone_tcp/netbios \
	cyclone_tcp/ftp

COMPONENT_ADD_INCLUDEDIRS := . \
	common \
	cyclone_tcp