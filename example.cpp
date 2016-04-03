/*
 * Compiling with GCC: g++ -o example example.cpp hmc5883.cpp i2c.cpp
 * 
 * */

#include <iostream>
#include <stdlib.h>     // system()
#include <unistd.h>     // sleep()

#include "hmc5883.h"

using namespace std;

int main()
{
	HMC5883 mag(-0.3349);       // Magnetic declination of your location in radians
                                // You can get this value on <http://www.magnetic-declination.com/>

	while(true)
	{
		system("clear");
		mag.Read();
		cout << "X axe mag field: " << mag.getXMagData() << " gauss" << endl;
		cout << "Y axe mag field: " << mag.getYMagData() << " gauss" << endl;
		cout << "Z axe mag field: " << mag.getZMagData() << " gauss" << endl;
		cout << "Heading: " << mag.getHeading() << " degrees" << endl;
		sleep(1);
	}
	
	return 0;
}
