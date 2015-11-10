#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <linux/fb.h>

int main(){
	char filename[20];
	
	fb_var_screeninfo fbinfo[2]={0};
	
	for(int i=0;i<2;i++){
		sprintf(filename, "/dev/fb%d", 1);
		int fb = open(filename, O_RDWR);
		if (fb==-1) continue;
		if (ioctl(fb,FBIOGET_FSCREENINFO,&fbinfo[i])==-1){
			printf("Error reading fixed information\n");
			return 2;
		}		
		printf("fb %d %dx%d\n",i,fbinfo[i].xres,fbinfo[i].yres);
		close(fb);
	}

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

