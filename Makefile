all: test_00.exe \
	   test_01.exe \
		 test_02.exe \
		 test_03.exe \
		 test_04.exe \
		 test_05.exe \
		 test_06.exe \
		 test_07.exe \
		 test_08.exe \
		 test_09.exe \
		 test_0a.exe \
		 test_0b.exe \
		 test_0c.exe \
		 test_0d.exe \
		 test_0e.exe \
		 test_0f.exe \
		 test_10.exe \
		 test_11.exe \
		 test_12.exe \
		 test_13.exe \
		 test_14.exe \
		 test_15.exe \
		 test_16.exe \
		 test_17.exe \
		 test_18.exe \
		 test_19.exe \
		 test_1a.exe \
		 test_1b.exe \
		 test_1c.exe \
		 test_1d.exe \
		 test_1e.exe \
		 test_1f.exe

test_00.exe: test_00.c
	gcc -O3 -std=c99 test_00.c -o test_00.exe

test_01.exe: test_01.c
	gcc -O3 -std=c99 test_01.c -o test_01.exe

test_02.exe: test_02.c
	gcc -O3 -std=c99 test_02.c -o test_02.exe

test_03.exe: test_03.c
	gcc -O3 -std=c99 test_03.c -o test_03.exe

test_04.exe: test_04.c
	gcc -O3 -std=c99 test_04.c -o test_04.exe

test_05.exe: test_05.c
	gcc -O3 -std=c99 test_05.c -o test_05.exe

test_06.exe: test_06.c
	gcc -O3 -std=c99 test_06.c -o test_06.exe

test_07.exe: test_07.c
	gcc -O3 -std=c99 test_07.c -o test_07.exe

test_08.exe: test_08.c
	gcc -O3 -std=c99 test_08.c -o test_08.exe

test_09.exe: test_09.c
	gcc -O3 -std=c99 test_09.c -o test_09.exe

test_0a.exe: test_0a.c gf2_00.c gf2.h
	gcc -O3 -std=c99 test_0a.c gf2_00.c -o test_0a.exe

test_0b.exe: test_0b.c gf2_00.c gf2.h
	gcc -O3 -std=c99 test_0b.c gf2_00.c -o test_0b.exe

test_0c.exe: test_0c.c gf2_01.c gf2.h
	gcc -O3 -std=c99 test_0c.c gf2_01.c -o test_0c.exe

test_0d.exe: test_0d.c gf2_01.c gf2.h
	gcc -O3 -std=c99 test_0d.c gf2_01.c -o test_0d.exe

test_0e.exe: test_0e.c gf2_02.c gf2.h
	gcc -O3 -std=c99 test_0e.c gf2_02.c -o test_0e.exe

test_0f.exe: test_0f.c gf2_03.c gf2.h
	gcc -O3 -std=c99 test_0f.c gf2_03.c -o test_0f.exe

test_10.exe: test_10.c gf2_03.c gf2.h
	gcc -O3 -std=c99 test_10.c gf2_03.c -o test_10.exe

test_11.exe: test_11.c gf2_04.c gf2.h
	gcc -O3 -std=c99 test_11.c gf2_04.c -o test_11.exe

test_12.exe: test_12.c gf2_05.c gf2.h
	gcc -O3 -std=c99 test_12.c gf2_05.c -o test_12.exe

test_13.exe: test_13.c gf2_05.c gf2.h
	gcc -O3 -std=c99 test_13.c gf2_05.c -o test_13.exe

test_14.exe: test_14.c gf2_05.c gf2.h
	gcc -O3 -std=c99 test_14.c gf2_05.c -o test_14.exe

test_15.exe: test_15.c gf2_06.c gf2.h
	gcc -O3 -std=c99 test_15.c gf2_06.c -o test_15.exe

test_16.exe: test_16.c gf2_07.c html_01.c gf2.h html.h
	gcc -O3 -std=c99 test_16.c html_01.c gf2_07.c -o test_16.exe

test_17.exe: test_17.c gf2_07.c html_01.c gf2.h html.h
	gcc -O3 -std=c99 test_17.c html_01.c gf2_07.c -o test_17.exe

test_18.exe: test_18.c gf2_08.c html_02.c gf2.h html.h
	gcc -O3 -std=c99 test_18.c html_02.c gf2_08.c -o test_18.exe

test_19.exe: test_19.c gf2_08.c html_02.c gf2.h html.h
	gcc -O3 -std=c99 test_19.c html_02.c gf2_08.c -o test_19.exe

test_1a.exe: test_1a.c gf2_08.c html_02.c gf2.h html.h
	gcc -O3 -std=c99 test_1a.c html_02.c gf2_08.c -o test_1a.exe

test_1b.exe: test_1b.c gf2_09.c html_02.c gf2.h html.h
	gcc -O3 -std=c99 test_1b.c html_02.c gf2_09.c -o test_1b.exe

test_1c.exe: test_1c.c gf2_0a.c gf2_debug_00.c html_03.c gf2.h html.h
	gcc -O3 -std=c99 test_1c.c html_03.c gf2_0a.c gf2_debug_00.c -o test_1c.exe

test_1d.exe: test_1d.c gf2_0b.c gf2_debug_01.c html_03.c gf2.h html.h
	gcc -O3 -std=c99 test_1d.c html_03.c gf2_0b.c gf2_debug_01.c -o test_1d.exe

test_1e.exe: test_1e.c gf2_0b.c gf2_debug_01.c decode_00.c html_03.c gf2.h html.h decode.h
	gcc -O3 -std=c99 test_1e.c html_03.c gf2_0b.c gf2_debug_01.c decode_00.c -o test_1e.exe

test_1f.exe: test_1f.c gf2_0b.c gf2_debug_01.c decode_00.c html_03.c gf2.h html.h decode.h
	gcc -O3 -std=c99 test_1f.c html_03.c gf2_0b.c gf2_debug_01.c decode_00.c -o test_1f.exe
