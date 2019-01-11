/*  Communication Systems
 *  DD12KSVT
 *  Nguyen Phan Hoang Hai - Pham Huu Vu
 *  11/2015
 */

#include "stdio.h"
#include "usbstk5515.h"
#include "sar.h"
#include "aic3204.h"
#include "string.h"
extern Int16 oled_test(Uint16 fc);

Int16 bits[20] = {	1,1,0,0,1,
					0,0,1,0,1,
					1,1,1,0,0,
					1,0,0,0,1};
Int16 phasebegin[10];					
Int16 tmp;
Int16 i;
Int16 j;
Int16 check;
Int16 Stoptimes;					
Int16 sinTable[96] = {
        0x0000, 0x10b4, 0x2120, 0x30fb, 0x3fff, 0x4dea, 0x5a81, 0x658b,
        0x6ed8, 0x763f, 0x7ba1, 0x7ee5, 0x7ffd, 0x7ee5, 0x7ba1, 0x76ef,//0x7ffd/1
        0x6ed8, 0x658b, 0x5a81, 0x4dea, 0x3fff, 0x30fb, 0x2120, 0x10b4,
        0x0000, 0xef4c, 0xdee0, 0xcf06, 0xc002, 0xb216, 0xa57f, 0x9a75,
        0x9128, 0x89c1, 0x845f, 0x811b, 0x8002, 0x811b, 0x845f, 0x89c1,//0x8002/-1
        0x9128, 0x9a76, 0xa57f, 0xb216, 0xc002, 0xcf06, 0xdee0, 0xef4c,
        0x0000, 0x10b4, 0x2120, 0x30fb, 0x3fff, 0x4dea, 0x5a81, 0x658b,
        0x6ed8, 0x763f, 0x7ba1, 0x7ee5, 0x7ffd, 0x7ee5, 0x7ba1, 0x76ef,//0x7ffd/1
        0x6ed8, 0x658b, 0x5a81, 0x4dea, 0x3fff, 0x30fb, 0x2120, 0x10b4,
        0x0000, 0xef4c, 0xdee0, 0xcf06, 0xc002, 0xb216, 0xa57f, 0x9a75,
        0x9128, 0x89c1, 0x845f, 0x811b, 0x8002, 0x811b, 0x845f, 0x89c1,//0x8002/-1
        0x9128, 0x9a76, 0xa57f, 0xb216, 0xc002, 0xcf06, 0xdee0, 0xef4c              
    };
void TEST_execute( Int16 ( *funchandle )( ), char *testname, Int16 testid )
{
    Int16 status;

    /* Display test ID */
    printf( "%02d  Testing %s...\n", testid, testname );

    /* Call test function */
    status = funchandle( );

    /* Check for test fail */
    if ( status != 0 )
    {
        /* Print error message */
        printf( "     FAIL... error code %d... quitting\n", status );

        /* Software Breakpoint to Code Composer */
        SW_BREAKPOINT;
    }
    else
    {
        /* Print error message */
        printf( "    PASS\n" );
    }
}

/* ------------------------------------------------------------------------ *
 *  main( )                                                                 *
 * ------------------------------------------------------------------------ */
void main( void )
{
    /* Init KIT, SAR, AIC3204 */
    USBSTK5515_init( );
    //Init_SAR();  
    aic3204_init(); 
    set_sampling_frequency_and_gain(48000,5); 
	TEST_execute( oled_test, "OLED", 1 );
	 
	for (i=0;i<20;i++)
	{
		tmp = bits[i]*2+bits[i];
		switch(tmp)
		{
			case 0:phasebegin[i/2]=7;break;
			case 1:phasebegin[i/2]=19;break;
			case 2:phasebegin[i/2]=42;break;
			case 3:phasebegin[i/2]=31;break;
		}
	}
	i=0;
	j=0;
	check=0;	
	Stoptimes = 1;
	while(1)
    {   
    	while((XmitR & I2S0_IR) == 0);
    	I2S0_W0_MSW_W = sinTable[j+phasebegin[i]]/10;
 		I2S0_W1_MSW_W = sinTable[j+phasebegin[i]]/10;
 		check++;
		j=j+1;
		if (j>47) j=0;

		if(check==48)
		{
			check=0;
			i++;
		}
		if (i==10) i=0;
	}
}
