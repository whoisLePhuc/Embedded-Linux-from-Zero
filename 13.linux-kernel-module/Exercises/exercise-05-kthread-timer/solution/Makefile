KDIR ?= /lib/modules/$(shell uname -r)/build

obj-m := thread_timer.o

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
