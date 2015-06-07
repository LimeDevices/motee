#ifndef _H_MOTEE_ARDUINO
#define _H_MOTEE_ARDUINO

#include <inttypes.h>
#include <Arduino.h>

class Motee {
    public:
        //initialize the library
        static void begin();

        //modes
        static const uint8_t 
                      STANDBY       = 0,
                      REVERSE       = 1,
                      FORWARD       = 2,
                      BRAKE         = 3,

        //return values
                      OK            = 0,
                      ERR_START     = 1,
                      ERR_ADDR      = 2,
                      ERR_SEND      = 3,
                      ERR_RESTART   = 4,

                      ERR_WID       = 5,
                      ERR_DIR       = 6,

        //faults bits
                      FAULT         = 0,
                      FAULT_OCP     = 1,
                      FAULT_UVLO    = 2,
                      FAULT_OTS     = 3,
                      FAULT_LIMIT   = 4,
                      FAULT_CLEAR   = 5;

        //constructor, takes device's number
        Motee(uint8_t id, bool reversed = false);

        //speed control
        int8_t setSpeed(uint8_t direction, uint8_t speed);
        int8_t setReverse(bool flag);
        int8_t setStandby();
        int8_t setBrake();

        //signed version of setSpeed
        int8_t setSpeed(int8_t speed);

        //change speed that is treated as negative when moving reverse
        int8_t changeSpeed(int8_t change);

        //blocking soft speed control
        int8_t softBlockingSet(uint8_t direction, uint8_t speed, uint32_t time);

        //soft speed control
        int8_t softSet(uint8_t direction, uint8_t speed, uint32_t time);
        static int8_t softUpdate(uint32_t time);

    private:
        uint8_t id;
};

#endif
