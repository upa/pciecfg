
#include "libpciecfg.h"

int main(int argc, char **argv)
{
	int ret;
	char *cfgfile = "x520.cfg";
	struct pciecfg pcfg;


	printf("open '%s'\n", cfgfile);
	ret = pciecfg_open(cfgfile, &pcfg);
	assert(ret >= 0);

	printf("Vendor ID: %04x\n", pciecfg_vendorid(&pcfg));
	printf("Device ID: %04x\n", pciecfg_deviceid(&pcfg));
	printf("SUBSYS ID: %04x\n", pciecfg_subsys_id(&pcfg));
	printf("SUBSYS VENDORID: %04x\n", pciecfg_subsys_vendorid(&pcfg));

	printf("BAR0: %#lx\n", pciecfg_bar_addr(&pcfg, 0));
	printf("\t%s\n", pciecfg_bar_is_portio(pciecfg_bar(&pcfg, 0)) ?
	       "Port I/O" : "MMIO");
	printf("\t%s\n", pciecfg_bar_is_64bit(pciecfg_bar(&pcfg, 0)) ?
	       "64bit" : "32bit");
	printf("\t%s\n", pciecfg_bar_is_prefetch(pciecfg_bar(&pcfg, 0)) ?
	       "prefetchable" : "non-prefetchable");

	printf("BAR2: %#lx\n", pciecfg_bar_addr(&pcfg, 2));
	printf("\t%s\n", pciecfg_bar_is_portio(pciecfg_bar(&pcfg, 2)) ?
	       "Port I/O" : "MMIO");
	printf("\t%s\n", pciecfg_bar_is_64bit(pciecfg_bar(&pcfg, 2)) ?
	       "64bit" : "32bit");
	printf("\t%s\n", pciecfg_bar_is_prefetch(pciecfg_bar(&pcfg, 2)) ?
	       "prefetchable" : "non-prefetchable");

	printf("BAR4: %#lx\n", pciecfg_bar_addr(&pcfg, 4));
	printf("\t%s\n", pciecfg_bar_is_portio(pciecfg_bar(&pcfg, 4)) ?
	       "Port I/O" : "MMIO");
	printf("\t%s\n", pciecfg_bar_is_64bit(pciecfg_bar(&pcfg, 4)) ?
	       "64bit" : "32bit");
	printf("\t%s\n", pciecfg_bar_is_prefetch(pciecfg_bar(&pcfg, 4)) ?
	       "prefetchable" : "non-prefetchable");


	return 0;
}
