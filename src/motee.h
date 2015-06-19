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

//true if device is 'reversed' i.e. directions are swapped
extern bool motee_reversed[9];



/****** FAULT CONTROL ******/

//read fault register's state from device and save it to *byte
int8_t moteeReadFault (uint8_t id, uint8_t *byte);

//clear fault register on device
int8_t moteeClearFault(uint8_t id);

#endif
