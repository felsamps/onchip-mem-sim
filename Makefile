target:
	g++ -ggdb -c src/*.cpp
	g++ -o onchip_sim *.o

clean:
	rm *.o
