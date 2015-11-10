#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/i2c-dev.h>

int main(){
	char filename[20];
	sprintf(filename, "/dev/i2c-%d", 1);
	int file = open(filename, O_RDWR);
	if (file<0) {
		printf("Unable to open I2C bus!");
		return 1;
	}
	
	if (ioctl(file, I2C_SLAVE, 0x6a) < 0) {
		printf("Error: Could not select magnetometer\n");
	}


	close(file);
	return 0;
}

