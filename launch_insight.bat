SET NAME=LPC2378HelloW

REM This only connects gdb/insight to OpenOCD and prepares the target
rem arm-elf-insight -x .\prj\lpc2378_ram.gdb %NAME%.elf

REM This executes the extra gdb commands to put a breakpoint, load, and continue (until the breakpoint)
arm-elf-insight -x .\prj\eclipse_ram.gdb %NAME%.elf