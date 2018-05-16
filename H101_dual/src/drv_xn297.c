
#include "binary.h"
#include "drv_spi.h"

#include "gd32f1x0.h"
#include "xn297.h"

#define gpioset( port , pin) port->BOR = (0x0001 << pin)
#define gpioreset( port , pin) port->BCR = (0x0001 << pin)

#define SPIOFF gpioset( GPIOB, 5)

void xn_writereg(int reg, int val)
{
	reg = reg & 0x0000003F;
	reg = reg | W_REGISTER;
	spi_cson();
	spi_sendbyte(reg);
	spi_sendbyte(val);
	spi_csoff();
}

int xn_readreg(int reg)
{
	reg = reg & REGISTER_MASK;
	spi_cson();
	spi_sendrecvbyte(reg);
	reg = spi_sendrecvbyte(0);
	spi_csoff();
	return reg;
}

int xn_command(int command)
{
	spi_cson();
	int status = spi_sendrecvbyte(command);
	spi_csoff();
	return status;
}


void _spi_write_address(int reg, int val)
{
	spi_cson();
	spi_sendbyte(reg);
	spi_sendbyte(val);
	spi_csoff();
}

/*
void xn_readpayload2( int *data , int size )
{
//	int index = 0;
	spi_cson();
	spi_sendbyte( R_RX_PAYLOAD ); // read rx payload
	while(size!=0)
	{
	data++[0]=	spi_sendzerorecvbyte(  );
	//data++;
	size--;
	}
	spi_csoff();
}
*/

void xn_readpayload(int *data, int size)
{
	int index = 0;
	spi_cson();
	spi_sendbyte(R_RX_PAYLOAD);	// read rx payload
	while (index < size)
	  {
		  data[index] = spi_sendzerorecvbyte();
		  index++;
	  }
	spi_csoff();
}



void xn_writerxaddress(int *addr)
{
	int index = 0;
	spi_cson();
	spi_sendbyte(W_REGISTER | RX_ADDR_P0);
	while (index < 5)
	  {
		  spi_sendbyte(addr[index]);
		  index++;
	  }
	spi_csoff();
}


void xn_writetxaddress(  int *addr )	
{
 int index = 0;
spi_cson();
spi_sendbyte(W_REGISTER|TX_ADDR);
	while(index<5)
	{
	spi_sendbyte( addr[index] );
	index++;
	}
spi_csoff();
}


void xn_writepayload( int data[] , int size )
{
	int index = 0;
	spi_cson();
	spi_sendrecvbyte( W_TX_PAYLOAD ); // write tx payload
	while(index<size)
	{
	spi_sendrecvbyte( data[index] );
	index++;
	}
	spi_csoff();
}
