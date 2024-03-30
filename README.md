# resource-monitoring-kernel-module
a reousrce monitoring kernel module which displays the amount of  memory (in kb) and CPU (in %) utilized  by each process 
along with the respective process ID and process name and also shows the total amount of memory utilized by all processes


# how to run?
open the linux terminal inside the given resource monitor folder 
run the following commands -

# make 
The make command invokes the Makefile, which contains rules for compiling the kernel module. It will compile the module's source files and generate the necessary object files.

# sudo insmod reource_monitor.ko 
Use the insmod command to load your kernel module into the kernel. 

# dmesg | tail 
The dmesg command displays the kernel ring buffer, which contains various messages produced by the kernel. The tail command is used to display the last few lines of the output, which often contain the most recent messages.

# sudo rmmod resource_monitor
The rmmod command is used to remove a module from the Linux kernel. Again, you need superuser privileges (sudo) to unload a kernel module. 

