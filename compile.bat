@echo off 
set project_name=USBHost_CH559
set xram_size=0x0800
set xram_loc=0x0600
set code_size=0xEFFF
set dfreq_sys=48000000

if not exist "src\config.h" echo //add your personal defines here > src\config.h

mkdir output

SDCC\bin\sdcc -c -V -mmcs51 --model-large --xram-size %xram_size% --xram-loc %xram_loc% --code-size %code_size% -I/ -DFREQ_SYS=%dfreq_sys%  src\main.c
SDCC\bin\sdcc -c -V -mmcs51 --model-large --xram-size %xram_size% --xram-loc %xram_loc% --code-size %code_size% -I/ -DFREQ_SYS=%dfreq_sys%  src\util.c
SDCC\bin\sdcc -c -V -mmcs51 --model-large --xram-size %xram_size% --xram-loc %xram_loc% --code-size %code_size% -I/ -DFREQ_SYS=%dfreq_sys%  src\USBHost.c
SDCC\bin\sdcc -c -V -mmcs51 --model-large --xram-size %xram_size% --xram-loc %xram_loc% --code-size %code_size% -I/ -DFREQ_SYS=%dfreq_sys%  src\uart.c

SDCC\bin\sdcc main.rel util.rel USBHost.rel uart.rel -V -mmcs51 --model-large --xram-size %xram_size% --xram-loc %xram_loc% --code-size %code_size% -I/ -DFREQ_SYS=%dfreq_sys%  -o output\%project_name%.ihx

SDCC\bin\packihx output\%project_name%.ihx > output\%project_name%.hex

SDCC\bin\hex2bin -c output\%project_name%.hex

del output\%project_name%.lk
del output\%project_name%.map
del output\%project_name%.mem
del output\%project_name%.ihx

del *.asm
del *.lst
del *.rel
del *.rst
del *.sym


Rem This tool flashes the bin file directly to the ch559 chip, you need to install the libusb-win32 driver with the zadig( https://zadig.akeo.ie/ ) tool so the tool can access the usb device
tools\chflasher.exe output\%project_name%.bin
