VAR := $(shell basename $(shell pwd))

compile:
	arduino-cli compile --fqbn arduino:avr:uno ./$(VAR).ino

upload:
	arduino-cli upload 
all:
	arduino-cli compile --fqbn arduino:avr:uno ./$(VAR).ino
	arduino-cli upload 
