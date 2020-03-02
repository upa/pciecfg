
#include "libpciecfg.h"

#include <assert.h>

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
	printf("SYBSYS ID: %04x\n", pciecfg_subsys_id(&pcfg));
	printf("SYBSYS VENDORID: %04x\n", pciecfg_subsys_vendorid(&pcfg));
	return 0;
}
