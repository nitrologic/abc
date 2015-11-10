#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/fb.h>
#include <string.h>

//#include <linux/i2c-dev.h>
/*

    SENSE_HAT_EVDEV_NAME = 'Raspberry Pi Sense HAT Joystick'
    EVENT_FORMAT = native_str('llHHI')
    EVENT_SIZE = struct.calcsize(EVENT_FORMAT)


for evdev in glob.glob('/sys/class/input/event*'):
            try:
                with io.open(os.path.join(evdev, 'device', 'name'), 'r') as f:
                    if f.read().strip() == self.SENSE_HAT_EVDEV_NAME:
                        return os.path.join('/dev', 'input', os.path.basename(evdev))
            except IOError as e:
                if e.errno != errno.ENOENT:
                    raise
        raise RuntimeError('unable to locate SenseHAT joystick device')
*/

const char *hatname="Raspberry Pi Sense HAT Joystick";

int main(){
	char filename[128];
	char devicename[512];

	int hat=-1;

	for(int i=0;i<4;i++){
		sprintf(filename,"/sys/class/input/event%d/device/name",i);
		int eb=open(filename,O_RDONLY);
		if(eb==-1) continue;
		ssize_t n=read(eb,devicename,512);
		ssize_t nn=strlen(hatname);
		if(memcmp(hatname,devicename,nn)==0){
			devicename[n]=0;
			sprintf(filename,"/dev/input/event%d",i);
			hat=open(filename,O_RDONLY);
			printf("%s open %d\n",devicename,hat);
			break;
		}
		close(eb);
		eb=-1;
	}

	while (1){
		char ev[5];
		ssize_t n=read(hat,ev,5);
		if(n==-1) break;
		if(n){
			printf("[%d] %d %d  %d  %d %d\n",n,ev[0],ev[1],ev[2],ev[3],ev[4]);
		}
	}

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

		if(i==1){
			close(fb);
			continue;
		}

//		void *p=mmap(0,n,PROT_READ|PROT_WRITE,MAP_SHARED,fb,0);
		void *p=mmap(0,n,PROT_WRITE,MAP_SHARED,fb,0);

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

