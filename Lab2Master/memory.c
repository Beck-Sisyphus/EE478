//master device data query and store

int counter = 0;
int temp;
int count;
int i;

while(1)
{
    count = counter % 16; 
    //check for UART command
    C1 = 1;

    if (command)
	//send requested data
	
    
    temp = count;
    if (temp >= 8)
    {
	B3 = 1;
	temp = temp - 8;
    }
    
    if (temp >= 4)
    {
	B2 = 1;
	temp = temp - 4;
    }

    if (temp >= 2)
    {
	B1 = 1;
	temp = temp - 2;
    }

    if (temp >= 1)
	B0 = 1;

    //BP1
    B4 = 0;
    B5 = 0;
    //query slave for BP1 data
    C1 = 0; //notwrite to true
    //set A0-A7 to data out

    //BP2
    B4 = 1;
    B5 = 0;
    //query slave for BP1 data
    C1 = 0; //notwrite to true
    //set A0-A7 to data out

    //temp
    B4 = 0;
    B5 = 1;
    //query slave for BP1 data
    C1 = 0; //notwrite to true
    //set A0-A7 to data out

    //pulse
    B4 = 1;
    B5 = 1;
    //query slave for BP1 data
    C1 = 0; //notwrite to true
    //set A0-A7 to data out

    counter = counter + 1;
    
}


int randomAccess(int type, int number, int counter)
{
    int temp;
    switch(type)
    {
	case 1:
	    B4 = 0;
	    B5 = 0;
	    break;
	case 2:
	    B4 = 1;
	    B5 = 0;
	    break;
	case 3:
	    B4 = 0;
	    B5 = 1;
	    break;
	case 4:
	    B4 = 1;
	    B5 = 1;
	    break;
    }
    temp = counter - number;
    temp = temp % 16;
    if (temp >= 8)
    {
	B3 = 1;
	temp = temp - 8;
    }
    
    if (temp >= 4)
    {
	B2 = 1;
	temp = temp - 4;
    }

    if (temp >= 2)
    {
	B1 = 1;
	temp = temp - 2;
    }

    if (temp >= 1)
	B0 = 1;

    C0 = 0; //set output enable to true

    return A0 + A1*2 + A2*4 + A3*8 + A4*16 + A5*32 + A6*64 + A7*128;
    
}