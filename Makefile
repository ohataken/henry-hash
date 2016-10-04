.c.o:
	$(CC) -Wall -c $<

henry-hash.o: main.o
	$(CC) -o henry-hash.o $^

run: henry-hash.o
	./$<
