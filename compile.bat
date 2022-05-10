@echo off 
set project_name=USBHost_CH559
set xram_size=0x0800
set xram_loc=0x0600
set code_size=0xEFFF
set dfreq_sys=48000000

set src_dir=src
set out_dir=output

if not exist %src_dir%\config.h echo //add your personal defines here > %src_dir%\config.h

mkdir %out_dir%

SDCC\bin\sdcc -c -V -mmcs51 --model-large --xram-size %xram_size% --xram-loc %xram_loc% --code-size %code_size% -I/ -DFREQ_SYS=%dfreq_sys%  %src_dir%\main.c
SDCC\bin\sdcc -c -V -mmcs51 --model-large --xram-size %xram_size% --xram-loc %xram_loc% --code-size %code_size% -I/ -DFREQ_SYS=%dfreq_sys%  %src_dir%\bsp.c
SDCC\bin\sdcc -c -V -mmcs51 --model-large --xram-size %xram_size% --xram-loc %xram_loc% --code-size %code_size% -I/ -DFREQ_SYS=%dfreq_sys%  %src_dir%\USBHost.c
SDCC\bin\sdcc -c -V -mmcs51 --model-large --xram-size %xram_size% --xram-loc %xram_loc% --code-size %code_size% -I/ -DFREQ_SYS=%dfreq_sys%  %src_dir%\uart_protocol.c

SDCC\bin\sdcc main.rel bsp.rel USBHost.rel uart_protocol.rel -V -mmcs51 --model-large --xram-size %xram_size% --xram-loc %xram_loc% --code-size %code_size% -I/ -DFREQ_SYS=%dfreq_sys%  -o %out_dir%\%project_name%.ihx

SDCC\bin\packihx %out_dir%\%project_name%.ihx > %out_dir%\%project_name%.hex

SDCC\bin\hex2bin -c %out_dir%\%project_name%.hex

del %out_dir%\%project_name%.lk
del %out_dir%\%project_name%.map
del %out_dir%\%project_name%.mem
del %out_dir%\%project_name%.ihx

del *.asm
del *.lst
del *.rel
del *.rst
del *.sym


Rem This tool flashes the bin file directly to the ch559 chip, you need to install the libusb-win32 driver with the zadig( https://zadig.akeo.ie/ ) tool so the tool can access the usb device
Rem tools\chflasher.exe %out_dir%\%project_name%.bin
