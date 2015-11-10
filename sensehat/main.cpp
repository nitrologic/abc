#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/fb.h>
#include <string.h>

//#include <linux/i2c-dev.h>

int main(){
	char filename[20];

	fb_fix_screeninfo fix[2]={0};
	fb_var_screeninfo var[2]={0};

	for(int i=0;i<2;i++){
		sprintf(filename, "/dev/fb%d", i);
		int fb = open(filename, O_RDWR);
		if (fb==-1) continue;

		if (ioctl(fb,FBIOGET_FSCREENINFO,&fix[i])==-1){
			printf("Error reading fixed information\n");
			return 2;
		}

		if (ioctl(fb,FBIOGET_VSCREENINFO,&var[i])==-1){
			printf("Error reading fixed information\n");
			return 2;
		}

		int w=var[i].xres;
		int h=var[i].yres;
		int bpp=var[i].bits_per_pixel;
		long int n=w*h*bpp/8;

		if(i==0){
			close(fb);
			continue;
		}

		void *p=mmap(0,n,PROT_READ|PROT_WRITE,MAP_SHARED,fb,0);

		printf("fix fb%d %s caps=%d mem=%p\n",i,fix[i].id,fix[i].capabilities,p);
		printf("var fb%d %d x %d x %d\n",i,w,h,bpp);
//		printf("var      %dx%d\n",var[i].xres_virtual,var[i].yres_virtual);

		if ((int)p==-1) {
			printf("Error: failed to map framebuffer device to memory");
		}else{
			printf("The framebuffer device was mapped to memory successfully.\n");
			short *s=(short*)p;
			int dx,dy;
			while (1){
				dx++;
				for(int y=0;y<h;y++){
					for(int x=0;x<w;x++){
						int tx=dx+x;
						int ty=dy+y;
						s[x+y*w]=tx*tx*ty*ty;
					}
				}
			}
			munmap(p,n);
		}
		close(fb);
	}
#ifdef I2C
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
#endif

	return 0;
}

