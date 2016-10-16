/* Motee arduino library 
	Date: 15.10.16
	Author: Kamil Foryszewski 
*/ 
#ifndef MOTEE_H
#define MOTEE_H

#include "Arduino.h"

class Motee
{
	public:
	Motee(int rxd, int txd, int lcr); 
	void showMe(); //indicares led to show each motee
	void fixMe(); //turns forvard/reverse dirction
	void forward(int speed,int synchro = 0); //set forward speed 0-100% of nominal motor speed deffault synchorinization disabled
	void forward(int speed. int min, int max); // -||- additional map parameters
	void forward(int speed. int timeout); //time to acheive speed in miliseconds. 
	void reverse(int speed); //set reverse speed 0-100% of nominal motor speed. 
	void reverse(int speed. int min, int max); // -||- additional map parameters
	void reverse(int speed, int timeout); //time to get speed in miliseconds
	int getActualSpeed(int min,int max); //returns actual speed from encoder mapped to min/max values
	void changeAddress(int address); //change default address
	void addSynchonousAddres(int address); //additional address for synchronous controlling group of motee
	void stopSynchronization(); //removes motee from synchronous mode
		 


	private:
	static int nOfMotees;
	int *  
	 
}
#endif


