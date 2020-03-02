#ifndef _LIBPCIECFG_H_
#define _LIBPCIECFG_H_

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>

struct pciecfg {
	char *file;		/* config dump file from sysfs */
	char config[4096];	/* config dump read from the file */
	size_t size;		/* length of config */
};


int pciecfg_open(char *file, struct pciecfg *pcfg)
{
	/* Open and load the config file to the config (cache) */
	int fd, ret;

	memset(pcfg, 0, sizeof(*pcfg));

	fd = open(file, O_RDONLY);
	if (fd < 0)
		return fd;

	ret = read(fd, pcfg->config, 4096);
	if (ret < 0)
		return ret;
	pcfg->size = ret;

	return 0;
}

uint8_t pciecfg_get8(struct pciecfg *pcfg, size_t offset)
{
	/* Get 8bit from the config with specified offset */
	uint8_t v;
	assert(offset + 1 <= pcfg->size);
	memcpy(&v, pcfg->config + offset, 1);
	return v;
}

uint16_t pciecfg_get16(struct pciecfg *pcfg, size_t offset)
{
	/* Get 16bit from the config with specified offset */
	uint16_t v;
	assert(offset + 2 <= pcfg->size);
	memcpy(&v, pcfg->config + offset, 2);
	return v;
}

uint32_t pciecfg_get32(struct pciecfg *pcfg, size_t offset)
{
	/* Get 32bit from the config with specified offset */
	uint32_t v;
	assert(offset + 4 <= pcfg->size);
	memcpy(&v, pcfg->config + offset, 4);
	return v;
}



/* 64-byte PCI Common Configuration Header. */
#define PCI_CFG_VENDORID		0x00	/* 16bit */
#define PCI_CFG_DEVICEID		0x02	/* 16bit */

#define PCI_CFG_COMMAND			0x04	/* 16bit */
#define PCI_CFG_STATUS			0x06	/* 16bit */

#define PCI_CFG_REVISION		0x08	/* 8bit */
#define PCI_CFG_PROG_INTERFACE		0x09	/* 8bit */
#define PCI_CFG_SUBCLASS		0x0A	/* 8bit */
#define PCI_CFG_BASECLASS		0x0B	/* 8bit */

#define PCI_CFG_CACHELINESZ		0x0C	/* 8bit */
#define PCI_CFG_LATENCY_TIMER		0x0D	/* 8bit */
#define PCI_CFG_HEADER_TYPE		0x0E	/* 8bit */
#define PCI_CFG_SELF_TEST		0x0F	/* 8bit */

#define PCI_CFG_BAR0			0x10	/* 32bit */
#define PCI_CFG_BAR1			0x14	/* 32bit */
#define PCI_CFG_BAR2			0x18	/* 32bit */
#define PCI_CFG_BAR3			0x1C	/* 32bit */
#define PCI_CFG_BAR4			0x20	/* 32bit */
#define PCI_CFG_BAR5			0x24	/* 32bit */

#define PCI_CFG_CARDBUS_PTR		0x28	/* 8bit */

#define PCI_CFG_SUBSYS_VENDORID	0x2C	/* 16bit */
#define PCI_CFG_SUBSYS_ID		0x2E	/* 16bit */

#define PCI_CFG_ROM_BASE_ADDR		0x30	/* 32bit */

#define PCI_CFG_CAP_PTR			0x34	/* 8bit */


#define PCI_CFG_DEFUN(FN, OF, BW) \
	uint##BW##_t FN (struct pciecfg *pcfg) {	\
		return pciecfg_get##BW(pcfg, OF);	\
	}

PCI_CFG_DEFUN(pciecfg_deviceid,	PCI_CFG_DEVICEID, 16);
PCI_CFG_DEFUN(pciecfg_command,	PCI_CFG_COMMAND, 16);

PCI_CFG_DEFUN(pciecfg_vendorid,	PCI_CFG_VENDORID, 16);
PCI_CFG_DEFUN(pciecfg_status,	PCI_CFG_STATUS, 16);

PCI_CFG_DEFUN(pciecfg_revision,		PCI_CFG_STATUS, 8);
PCI_CFG_DEFUN(pciecfg_prog_interface,	PCI_CFG_PROG_INTERFACE, 8);
PCI_CFG_DEFUN(pciecfg_subclass,		PCI_CFG_SUBCLASS, 8);
PCI_CFG_DEFUN(pciecfg_baseclass,	PCI_CFG_BASECLASS, 8);

PCI_CFG_DEFUN(pciecfg_cachelinesz, 	PCI_CFG_CACHELINESZ, 8);
PCI_CFG_DEFUN(pciecfg_latency_timer,	PCI_CFG_LATENCY_TIMER, 8);
PCI_CFG_DEFUN(pciecfg_header_type,	PCI_CFG_HEADER_TYPE, 8);
PCI_CFG_DEFUN(pciecfg_self_test,	PCI_CFG_SELF_TEST, 8);


PCI_CFG_DEFUN(pciecfg_carbus_ptr,	PCI_CFG_CARDBUS_PTR, 8);
PCI_CFG_DEFUN(pciecfg_subsys_vendorid,	PCI_CFG_SUBSYS_VENDORID, 16);
PCI_CFG_DEFUN(pciecfg_subsys_id,	PCI_CFG_SUBSYS_ID, 16);
PCI_CFG_DEFUN(pciecfg_rom_base_addr,	PCI_CFG_ROM_BASE_ADDR, 32);
PCI_CFG_DEFUN(pciecfg_cap_ptr,		PCI_CFG_CAP_PTR, 8);


uint32_t pciecfg_bar(struct pciecfg *pcfg, int bar)
{
	return pciecfg_get32(pcfg, PCI_CFG_BAR0 + (bar << 2));
}


#define pciecfg_bar_is_portio(bar)	(bar & 0x01)
#define pciecfg_bar_is_64bit(bar)	(bar & 0x04)
#define pciecfg_bar_is_prefetch(bar)	(bar & 0x08)

uint64_t pciecfg_bar_addr(struct pciecfg *pcfg, int bar)
{
	uint64_t addr = 0, tmp = 0;

	addr = pciecfg_bar(pcfg, bar);
	if (pciecfg_bar_is_64bit(addr)) {
		/* 64bit BAR */
		tmp = pciecfg_bar(pcfg, bar + 1);
		addr |= (tmp << 32);
	}

	return addr & (~(uint64_t)0x0F);
}




#endif /* _LIBPCIECFG_H_ */
