upload: main.hex
	avrdude -c usbasp -p attiny13 -B 3 -U main.hex
main.hex: main.c
	avr-gcc main.c -mmcu=attiny13 -DF_CPU=9600000L -Os -o main.hex
