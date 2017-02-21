all: test_00 \
     test_01 \
     test_02 \
     test_03 \
     test_04 \
     test_05 \
     test_06 \
     test_07 \
     test_08 \
     test_09 \
     test_0a \
     test_0b \
     test_0c \
     test_0d \
     test_0e \
     test_0f \
     test_10 \
     test_11 \
     test_12 \
     test_13 \
     test_14 \
     test_15 \
     test_16 \
     test_17 \
     test_18 \
     test_19 \
     test_1a \
     test_1b \
     test_1c \
     test_1d \
     test_1e \
     test_1f

test_00: test_00.c
	gcc -O3 -std=c99 test_00.c -o test_00

test_01: test_01.c
	gcc -O3 -std=c99 test_01.c -o test_01

test_02: test_02.c
	gcc -O3 -std=c99 test_02.c -o test_02

test_03: test_03.c
	gcc -O3 -std=c99 test_03.c -o test_03

test_04: test_04.c
	gcc -O3 -std=c99 test_04.c -o test_04

test_05: test_05.c
	gcc -O3 -std=c99 test_05.c -o test_05

test_06: test_06.c
	gcc -O3 -std=c99 test_06.c -o test_06

test_07: test_07.c
	gcc -O3 -std=c99 test_07.c -o test_07

test_08: test_08.c
	gcc -O3 -std=c99 test_08.c -o test_08

test_09: test_09.c
	gcc -O3 -std=c99 test_09.c -o test_09

test_0a: test_0a.c gf2_00.c gf2.h
	gcc -O3 -std=c99 test_0a.c gf2_00.c -o test_0a

test_0b: test_0b.c gf2_00.c gf2.h
	gcc -O3 -std=c99 test_0b.c gf2_00.c -o test_0b

test_0c: test_0c.c gf2_01.c gf2.h
	gcc -O3 -std=c99 test_0c.c gf2_01.c -o test_0c

test_0d: test_0d.c gf2_01.c gf2.h
	gcc -O3 -std=c99 test_0d.c gf2_01.c -o test_0d

test_0e: test_0e.c gf2_02.c gf2.h
	gcc -O3 -std=c99 test_0e.c gf2_02.c -o test_0e

test_0f: test_0f.c gf2_03.c gf2.h
	gcc -O3 -std=c99 test_0f.c gf2_03.c -o test_0f

test_10: test_10.c gf2_03.c gf2.h
	gcc -O3 -std=c99 test_10.c gf2_03.c -o test_10

test_11: test_11.c gf2_04.c gf2.h
	gcc -O3 -std=c99 test_11.c gf2_04.c -o test_11

test_12: test_12.c gf2_05.c gf2.h
	gcc -O3 -std=c99 test_12.c gf2_05.c -o test_12

test_13: test_13.c gf2_05.c gf2.h
	gcc -O3 -std=c99 test_13.c gf2_05.c -o test_13

test_14: test_14.c gf2_05.c gf2.h
	gcc -O3 -std=c99 test_14.c gf2_05.c -o test_14

test_15: test_15.c gf2_06.c gf2.h
	gcc -O3 -std=c99 test_15.c gf2_06.c -o test_15

test_16: test_16.c gf2_07.c html_01.c gf2.h html.h
	gcc -O3 -std=c99 test_16.c html_01.c gf2_07.c -o test_16

test_17: test_17.c gf2_07.c html_01.c gf2.h html.h
	gcc -O3 -std=c99 test_17.c html_01.c gf2_07.c -o test_17

test_18: test_18.c gf2_08.c html_02.c gf2.h html.h
	gcc -O3 -std=c99 test_18.c html_02.c gf2_08.c -o test_18

test_19: test_19.c gf2_08.c html_02.c gf2.h html.h
	gcc -O3 -std=c99 test_19.c html_02.c gf2_08.c -o test_19

test_1a: test_1a.c gf2_08.c html_02.c gf2.h html.h
	gcc -O3 -std=c99 test_1a.c html_02.c gf2_08.c -o test_1a

test_1b: test_1b.c gf2_09.c html_02.c gf2.h html.h
	gcc -O3 -std=c99 test_1b.c html_02.c gf2_09.c -o test_1b

test_1c: test_1c.c gf2_0a.c gf2_debug_00.c html_03.c gf2.h html.h
	gcc -O3 -std=c99 test_1c.c html_03.c gf2_0a.c gf2_debug_00.c -o test_1c

test_1d: test_1d.c gf2_0b.c gf2_debug_01.c html_03.c gf2.h html.h
	gcc -O3 -std=c99 test_1d.c html_03.c gf2_0b.c gf2_debug_01.c -o test_1d

test_1e: test_1e.c gf2_0b.c gf2_debug_01.c decode_00.c html_03.c gf2.h html.h decode.h
	gcc -O3 -std=c99 test_1e.c html_03.c gf2_0b.c gf2_debug_01.c decode_00.c -o test_1e

test_1f: test_1f.c gf2_0b.c gf2_debug_01.c decode_00.c html_03.c gf2.h html.h decode.h
	gcc -O3 -std=c99 test_1f.c html_03.c gf2_0b.c gf2_debug_01.c decode_00.c -o test_1f
