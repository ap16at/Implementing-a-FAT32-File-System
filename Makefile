CFLAGS = -std=c99

project3: main.o lseek.o read.o cp.o write.o open.o close.o rmdir.o rm.o mv.o mkdir.o creat.o cd.o ls.o parse_boot_sector.o parse_table.o parse_directory.o parse_user_input.o info.o size.o
	gcc -o project3 main.o lseek.o read.o cp.o write.o open.o close.o rmdir.o rm.o mv.o mkdir.o creat.o cd.o ls.o parse_boot_sector.o parse_table.o parse_directory.o parse_user_input.o info.o size.o

main.o: main.c
	gcc $(CFLAGS) -c main.c

lseek.o: lseek.c
	gcc $(CFLAGS) -c lseek.c

read.o: read.c
	gcc $(CFLAGS) -c read.c

write.o: write.c
	gcc $(CFLAGS) -c write.c

cp.o: cp.c
	gcc $(CFLAGS) -c cp.c

open.o: open.c
	gcc $(CFLAGS) -c open.c

close.o: close.c
	gcc $(CFLAGS) -c close.c

rmdir.o: rmdir.c
	gcc $(CFLAGS) -c rmdir.c

rm.o: rm.c
	gcc $(CFLAGS) -c rm.c

mv.o: mv.c
	gcc $(CFLAGS) -c mv.c

mkdir.o: mkdir.c
	gcc $(CFLAGS) -c mkdir.c

creat.o: creat.c
	gcc $(CFLAGS) -c creat.c

cd.o: cd.c
	gcc $(CFLAGS) -c cd.c

ls.o: ls.c
	gcc $(CFLAGS) -c ls.c

parse_boot_sector.o: parse_boot_sector.c
	gcc $(CFLAGS) -c parse_boot_sector.c

parse_table.o: parse_table.c
	gcc $(CFLAGS) -c parse_table.c

parse_directory.o: parse_directory.c
	gcc $(CFLAGS) -c parse_directory.c

parse_user_input.o: parse_user_input.c
	gcc $(CFLAGS) -c parse_user_input.c

info.o: info.c
	gcc $(CFLAGS) -c info.c

size.o: size.c
	gcc $(CFLAGS) -c size.c

clean:
	rm -f *.o project3
