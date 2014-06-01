show remotetimeout
set remotetimeout 180
target remote localhost:3333
monitor reset run_and_halt
monitor sleep 500
monitor poll
monitor flash probe 0
monitor flash erase 0 0 26
monitor reset run_and_halt
monitor sleep 500
monitor poll
monitor flash write 0 D:\UWScratch\ph5\ph5.bin 0x0
monitor soft_reset_halt
monitor arm7_9 force_hw_bkpts enable
set language c
symbol-file ph5.elf
break main
set remotetimeout 10
continue