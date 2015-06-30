#makefile that compiles exmaples and install them on atmega8
#to make it work for other platfors, change -mmcu for avr-gcc
#and device for avrdude

%.bin: %.c
	avr-gcc $< ../motee.c -O2 -mmcu=atmega8 -o $(basename $@).bin -D F_CPU=8000000

%.hex: %.bin
	avr-objcopy -O ihex $(basename $@).bin $(basename $@).hex

%-install: %.hex
	avrdude -U flash:w:$< -c usbasp -p m8

clean:
	rm -v *.bin *.hex
