/*
    Copyright (c) 2015 LimeDevices

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _H_MOTEE
#define _H_MOTEE

#include <inttypes.h>
#include <stdbool.h>

//return values
#define MOTEE_OK            0   //success
#define MOTEE_ERR_START     1   //can't start i2c
#define MOTEE_ERR_ADDR      2   //can't send slave address
#define MOTEE_ERR_SEND      3   //can't send byte
#define MOTEE_ERR_RESTART   4   //can't restart during receivin:
#define MOTEE_ERR_READ      5   //error during reading from motee
#define MOTEE_ERR_GETSPEED  128 //error during getting speed (bad id)

//errors caused by wrong arguments
#define MOTEE_ERR_WID       5   //wrong id (grater than 8, or 
                                //  not found during searching)
#define MOTEE_ERR_DIR       6   //wrong direction (not MOTEE_FORWARD 
                                //  or MOTEE_REVERSE)

//operation modes
#define MOTEE_STANDBY       0   //low-power shutdown mode
#define MOTEE_REVERSE       1   //rotate reverse
#define MOTEE_FORWARD       2   //rotate forward
#define MOTEE_BRAKE         3   //brake mode

//motor faults bits
#define MOTEE_FAULT         0   //if fault occures
#define MOTEE_FAULT_OCP     1   //overcurrent event
#define MOTEE_FAULT_UVLO    2   //undervoltage lockout
#define MOTEE_FAULT_OTS     3   //overtemperature
#define MOTEE_FAULT_LIMIT   4   //extended current limit
#define MOTEE_FAULT_CLEAR   7   //clear the fault status

/****** CONSTANTS ******/

//devices addresses, read address is higher by 1
extern const uint8_t motee_address[9], motee_reg_control, motee_reg_fault;

/****** INITIALIZATION ******/

//initialize motee 
void moteeInit();

//tries reading fault on each possible address
//returns number of motee's found
int8_t moteeSearch();

//true if device was found during search
extern bool motee_found[9];



/****** SPEED CONTROL ******/

//motee basic speed control
int8_t moteeSetSpeed (uint8_t id, uint8_t direction, uint8_t speed);
int8_t moteeReverse  (uint8_t id, bool reverse);
int8_t moteeStandby  (uint8_t id);
int8_t moteeBrake    (uint8_t id);

//signed version of moteeSetSpeed, sign of speed indicates direction
int8_t moteeSetSpeedS(uint8_t id, int8_t speed);

//positive value of means speeding up if current direction is forward, or slowing
//down if current direction is reverse; negative value does the opposite
int8_t moteeChangeSpeed(uint8_t id, int8_t change);

//blocking soft-mode
int8_t moteeSoftBlockingSet(uint8_t id, uint8_t direction, uint8_t speed, uint32_t time);

//non-blocking soft mode
int8_t moteeSoftUpdate(uint32_t time);
int8_t moteeSoftSet(uint8_t id, uint8_t direction, uint8_t speed, uint32_t time);

//get current speed, returns 128 on error
int8_t moteeGetSpeed(uint8_t id);

//true if device is 'reversed' i.e. directions are swapped
extern bool motee_reversed[9], motee_during_soft[9];



/****** FAULT CONTROL ******/

//read fault register's state from device and save it to *byte
int8_t moteeReadFault (uint8_t id, uint8_t *byte);

//clear fault register on device
int8_t moteeClearFault(uint8_t id);

#endif
