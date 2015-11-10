#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/fb.h>
#include <string.h>

#include <linux/i2c-dev.h>

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

	sprintf(filename, "/dev/i2c-%d", 1);

	int file = open(filename, O_RDWR);
	if (file<0) {
		printf("Unable to open I2C bus!");
		return 1;
	}

	if (ioctl(file, I2C_SLAVE, 0x6a) < 0) {
		printf("Error: Could not select gyro\n");
	}
// Enable Gyro
	writeGyrReg(CTRL_REG1_G, 0b00001111); // Normal power mode, all axes enabled
	writeGyrReg(CTRL_REG4_G, 0b00110000); // Continuos update, 2000 dps full scale	
	
	while(true){
        uint8_t b[6];
        readBlock(0x80 | OUT_X_L_A, sizeof(b), b); 		
		printf("packet $d $d $d  $d $d $d\n,b[0],b[1],b[2],b[3],b[4],b[5]);
	}
	
// Enable accelerometer.	
//	writeAccReg(CTRL_REG1_XM, 0b01100111); //  z,y,x axis enabled, continuos update,  100Hz data rate
//	writeAccReg(CTRL_REG2_XM, 0b00100000); // +/- 16G full scale	
	
	close(file);

	return 0;
}

