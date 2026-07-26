## What is does

Adds two digit numbers together


## Files in this project

- .clangd: to load all arduino specific packages correctly 
- Makefile: 'make all' builds project and deploys it to arduino
- local.cpp/h: files for your code 
- <projectName>.ino: proxy file for arduino lsp
- sketch.yaml: enviroment config and default env-vars
- images what the setup for the hardware looks like


## caveats 

#### hardware setup

The Buttons where designed to align with the '>' on the screen, however currently the HW setup isnt reflecting that.
Instead the mapping is as follows from left to right (see HW setup under images/):

AC ENTR ADD SUB


#### neovim setup

1. the LiquidCrystal lib for arduino has to be installed separately with the cli  

```bash
arduino-cli lib install "LiquidCrystal"
```
2. if the arduino has been plugged into a new port compared to the last time it worked, then this port has to be set in the sketch.yaml 

