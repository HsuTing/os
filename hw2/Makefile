obj-m += hw2.o
KVERSION := $(shell uname -r)

all:
	$(MAKE) -C /lib/modules/$(KVERSION)/build M=$(PWD) modules
	gcc -o user_program main.c
clean:
	$(MAKE) -C /lib/modules/$(KVERSION)/build M=$(PWD) clean
