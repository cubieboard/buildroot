#include <stdio.h>
#include <stdint.h>
#include <linux/input.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/inotify.h>
#include <sys/stat.h>
#include <limits.h>
#include <sys/errno.h>
#include <sysfs/dlist.h>
#include <sysfs/libsysfs.h>

#define SUNXI_IR_NAME "sun4i-ir"
#define HOOK_PROGRAM "/tools/ir-hook"

int get_sunxi_ir_device(char *buf_name, size_t buf_len)
{
	struct sysfs_class *input_class = NULL;
	struct dlist *input_devices = NULL;
	struct sysfs_class_device *cls_dev = NULL;
	struct sysfs_device *tdev = NULL;
	struct sysfs_attribute *tattr = NULL;
	int ret = -1;

	input_class = sysfs_open_class("input");
	input_devices = sysfs_get_class_devices(input_class);

	dlist_for_each_data(input_devices, cls_dev, struct sysfs_class_device) {
		tdev = sysfs_get_classdev_device(cls_dev);
		if (tdev != NULL) {
			tattr = sysfs_get_device_attr(tdev, "name");
			if (tattr == NULL)
				continue;
			if (tattr->value == NULL)
				continue;

			if (strncmp(tattr->value, SUNXI_IR_NAME, 8)) {
				continue;
			}
			memset(buf_name, buf_len, 0);
			strncpy(buf_name, cls_dev->name, buf_len - 1);
			ret = 0;

		}
	}

	sysfs_close_class(input_class);
	return ret;
}


int main (int argc, char **argv)
{
	int fd, rd, i, ret;
	struct input_event ev[64];
	char name_buf1[128];
	char name_buf2[128];

	ret = get_sunxi_ir_device(name_buf1, sizeof(name_buf1));
	if (ret) {
		printf("Please insmod sunxi-ir.ko\n");
		return -1;
	}

	memset(name_buf2, sizeof(name_buf2), 0);
	snprintf(name_buf2, sizeof(name_buf2), "/dev/input/%s", name_buf1);

	printf("ir: %s\n", name_buf2);

	if ((fd = open(name_buf2, O_RDONLY)) < 0) {
		perror("evtest");
		return 1;
	}

	daemon(0, 1);

	while (1) {
		rd = read(fd, ev, sizeof(struct input_event) * 64);

		if (rd < (int) sizeof(struct input_event)) {
			perror("read");
			return 1;
		}

		for (i = 0; i < rd / sizeof(struct input_event); i++) {
			if (ev[i].type == 1) {
				if (ev[i].value == 1) {
					printf("IR: %d DOWN\n", ev[i].code);
				} else {
					printf("IR: %d UP\n", ev[i].code);
				}
				if (!access(HOOK_PROGRAM, X_OK)) {
					memset(name_buf1, sizeof(name_buf1), 0);
					snprintf(name_buf1, sizeof(name_buf1), "%s %d %d",
						 HOOK_PROGRAM, ev[i].code, ev[i].value);
					system(name_buf1);
				}
			}
		}
	}

	close(fd);
	return 0;
}
