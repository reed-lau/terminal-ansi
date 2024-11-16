all:main

main:main.cc
	g++ -o $@ $< -O2
