#include <sys/proc.h>
#include <sys/conf.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#include <libkern/libkern.h>
#include <mach/mach_types.h>
#include <miscfs/devfs/devfs.h>

static int devfs_open(dev_t dev, int oflags, int devtype, struct proc *p) {
	printf(KEXTNAME_S ": opened\n");
}

static int devfs_close(dev_t dev, int fflag, int devtype, struct proc *p) {
	printf(KEXTNAME_S ": closed\n");
}

static int devfs_ioctl(dev_t dev, u_long cmd, caddr_t data, int fflag, struct proc *p) {
	printf(KEXTNAME_S ": ioctl\n");
}

static struct cdevsw devfs_functions = {
  /*.d_open     = */devfs_open,
  /*.d_close    = */devfs_close,
  /*.d_read     = */eno_rdwrt,
  /*.d_write    = */eno_rdwrt,
  /*.d_ioctl    = */devfs_ioctl,
  /*.d_stop     = */eno_stop,
  /*.d_reset    = */eno_reset,
  /*.d_ttys     = */NULL,
  /*.d_select   = */eno_select,
// OS X does not support memory-mapped devices through the mmap() function.
  /*.d_mmap     = */eno_mmap,
  /*.d_strategy = */eno_strat,
  /*.d_getc     = */eno_getc,
  /*.d_putc     = */eno_putc,
  /*.d_type     = */0
};
static int g_devfs_major;
static void *devfs_handle = NULL;

kern_return_t
example_start(__attribute__((unused)) kmod_info_t *ki,
              __attribute__((unused)) void *d) {
	printf(KEXTNAME_S ": start\n");

	g_devfs_major = cdevsw_add(-1, &devfs_functions);
  	if (g_devfs_major < 0) {
    	return KMOD_RETURN_FAILURE;
  	}

	devfs_handle = devfs_make_node(makedev(g_devfs_major, 0), DEVFS_CHAR, UID_ROOT, GID_WHEEL, 0666, "ccdille");

	return KERN_SUCCESS;
}

kern_return_t
example_stop(__attribute__((unused)) kmod_info_t *ki,
             __attribute__((unused)) void *d) {
	printf(KEXTNAME_S ": stop\n");
	devfs_remove(devfs_handle);
	return KERN_SUCCESS;
}

extern kern_return_t _start(kmod_info_t *ki, void *d);
extern kern_return_t _stop(kmod_info_t *ki, void *d);

KMOD_EXPLICIT_DECL(BUNDLEID, KEXTBUILD_S, _start, _stop)
__private_extern__ kmod_start_func_t *_realmain = example_start;
__private_extern__ kmod_stop_func_t *_antimain = example_stop;
__private_extern__ int _kext_apple_cc = __APPLE_CC__;

