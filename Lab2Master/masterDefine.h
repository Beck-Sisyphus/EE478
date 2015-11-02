/* 
 * File:   masterDefine.h
 * Author: Beck
 *
 * Created on November 1, 2015, 1:26 PM
 */

#ifndef MASTER_H
#define	MASTER_H

	void delay(int aDelay);
	// For USART
	void UART_init();
	void UART_write(const char* commandPtr, char* readingPtr);
	void UART_read(char* commandPtr);
	// For SPI
	void SPI_init();
 	// For SRAM


#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* MASTER_H */