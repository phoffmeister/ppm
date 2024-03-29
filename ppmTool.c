#include <stdio.h>
#include <stdlib.h>

#include "ppmio.h"

void printUsage(char* progName){
	printf("Usage: \n%s -i input.ppm -o output.ppm [-rl]|[-rr]|[-mh]|[-mv]|[-inv]|[-max (1-255)]|[-min (1-255)]\n",progName);
	printf("-rl  : rotate left\n");
	printf("-rr  : rotate right\n");
	printf("-mh  : mirror horizontal\n");
	printf("-mv  : mirror vertical\n");
	printf("-inv : invert colors\n");
	printf("-max : set maximum value\n");
	printf("-min : set minimum value\n");
}

int main(int argc, char ** argv){
	int mh, mv, rr, rl, inv, max, min;
	char * path_in;
	char * path_out;
	pixmap * map;

	mh=0;mv=0;rr=0;rl=0;inv=0;max=0;min=0;
	
	if(argc <6 || argc>7){
		printUsage(argv[0]);
		return 0;
	}else{
		if(argv[1][1]!='i'){
			printUsage(argv[0]);
			return 0;
		}else{
			path_in = argv[2];
		}
		if(argv[3][1]!='o'){
			printUsage(argv[0]);
			return 0;
		}else{
			path_out = argv[4];
		}
			
		if(argv[5][1]=='r'){
			if(argv[5][2]=='r'){
				//rotate right
				rr=1;
			}else if(argv[5][2]=='l'){
				//rotate left
				rl=1;
			}else{
				printUsage(argv[0]);
				return 0;
			}			
		}
			
		if(argv[5][1]=='m' && argc==6){
			if(argv[5][2]=='h'){
				//mirror horizontal
				mh=1;
			}else if(argv[5][2]=='v'){
				//mirror vertical
				mv=1;
			}else{
				printUsage(argv[0]);
				return 0;
			}			
		}
		
		if(argv[5][1]=='i'){
			if(argv[5][2]=='n' && argv[5][3]=='v'){
				inv=1;	
			}else{
				printUsage(argv[0]);
				return 0;
			}
		}
		

		if(argc==7){
			if(argv[5][1]=='m' && argv[5][2]=='a' && argv[5][3]=='x'){
				max = atoi(argv[6]);
				if(max > 255){
					max=255;
				}
			}else if(argv[5][1]=='m' && argv[5][2]=='i' && argv[5][3]=='n'){
				min = atoi(argv[6]);
				if(min <1){
					min=1;
				}
			}else{
				printUsage(argv[0]);
				return 0;
			}
		}
		
		if(rr||rl||mv||mh||inv||max||min){
			
		}else{
			printUsage(argv[0]);
			return 0;
		}
		
	}
	
	map = openPixMap(path_in);
	
	if(rr){
		rotatePixMap(&map);
		rotatePixMap(&map);
		rotatePixMap(&map);
	}else if(rl){
		rotatePixMap(&map);
	}else if(mh){
		mirrorHorPixMap(map);
	}else if(mv){
		mirrorVerPixMap(map);
	}else if(inv){
		invertPixMap(map);
	}else if(max){
		setMaxPix(map,max);
	}else if(min){
		setMinPix(map,min);
	}
	writePixMap(path_out, map);
	freeMap(map);
	
	
	return 0;
}
