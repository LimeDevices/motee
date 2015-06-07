frontend in C
-------------
Generic functions that can be adjusted to many systems by
changing backend. Backend must provide 4 functions:
* `i2cInit`  
    is called durint initialization
* `moteeSendByte`  
    sends byte to device with given addresses
* `moteeRecvByte`  
    receive byte from device
* `_delay`   
    wait 100ms (if desired, can be changed)
first three functions should be in library called i2c (with
files i2c.c and i2c.h), last one should be in file utils.h.
