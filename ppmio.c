#include <stdio.h>
#include <stdlib.h>

#include "ppmio.h"



pixmap* createPixMap(int x, int y, int maxval){
	pixmap* map;
	map = malloc(sizeof (pixmap));
	map->map_r = malloc(x*y);
	map->map_g = malloc(x*y);
	map->map_b = malloc(x*y);	
	map->dim.w=x;
	map->dim.h=y;
	map->max=maxval;
	
	return map;
}

void setPix(pixmap* map, color col, int x, int y){
	int w=map->dim.w;
	
	(map->map_r)[x+w*y] = col.r;
	(map->map_g)[x+w*y] = col.g;
	(map->map_b)[x+w*y] = col.b;
}

void writePixMap(char* path, pixmap* map){
	FILE* datei;
	int n,m;
	
	datei = fopen(path, "w");
	fprintf(datei, "P6\n%d %d\n255\n",map->dim.w,map->dim.h);
	for(n=0; n<map->dim.h; n++){
		for(m=0; m<map->dim.w; m++){
			fwrite((map->map_r)+n*map->dim.w+m,1,1,datei);
			fwrite((map->map_g)+n*map->dim.w+m,1,1,datei);
			fwrite((map->map_b)+n*map->dim.w+m,1,1,datei);
		}
	}
	fclose(datei);
}

void freeMap(pixmap* map){
	free(map->map_r);
	free(map->map_g);
	free(map->map_b);
	free(map);
}

color getPix(pixmap* map, int x, int y){
	color col;
	col.r=*((map->map_r)+y*(map->dim.w)+x);
	col.g=*((map->map_g)+y*(map->dim.w)+x);
	col.b=*((map->map_b)+y*(map->dim.w)+x);
	return col;
}

void invertPixMap(pixmap* map){
	int x,y,w,h,max;
	color col;
	
	w = map->dim.w;
	h = map->dim.h;
	max = map->max;
	
	for(y=0; y<h; y++){
		for(x=0; x<w; x++){
			col = getPix(map,x,y);
			col.r=max-col.r;
			col.g=max-col.g;
			col.b=max-col.b;
			setPix(map,col,x,y);
		}
	}
}

pixmap* openPixMap(char* path){
	pixmap* map;
	FILE* datei;
	unsigned char input;
	int w,h,maxval,n,m,count;
	
	datei = fopen(path, "r");
	
	input = fgetc(datei);
	if(input!='P'){
		printf("wrongFormat");
		return NULL;
	}
	
	input = fgetc(datei);
	if(input!='6'){
		printf("wrongFormat");
		return NULL;
	}
	
	input = fgetc(datei);
	while(input=='\n' || input ==' ' || input =='\r' || input =='\t'){
		input = fgetc(datei);
	}
	fseek(datei,-1,SEEK_CUR);
	
	fscanf(datei, "%d", &w);
	
	input = fgetc(datei);
	while(input=='\n' || input ==' ' || input =='\r' || input =='\t'){
		input = fgetc(datei);
	}
	fseek(datei,-1,SEEK_CUR);
	
	fscanf(datei, "%d", &h);
	
	input = fgetc(datei);
	while(input=='\n' || input ==' ' || input =='\r' || input =='\t'){
		input = fgetc(datei);
	}
	fseek(datei,-1,SEEK_CUR);
	
	fscanf(datei, "%d", &maxval);
	if(maxval>255){
		printf("Dateiformat nicht unterstützt");
		return NULL;
	}
	input = fgetc(datei);
	
	if(!(input=='\n' || input ==' ' || input =='\r' || input =='\t')){
		printf("wrong format");
		return 0;
	}
	
	map = createPixMap(w,h,maxval);
	
	count=0;
	for(n=0; n<h; n++){
		for(m=0; m<w; m++){
			fread((map->map_r)+count,1,1,datei);
			fread((map->map_g)+count,1,1,datei);
			fread((map->map_b)+count,1,1,datei);
			count++;
		}
	}
	fclose(datei);
	return map;
}

void mirrorHorPixMap(pixmap* map){
	int x,y,w,h;
	color col;
	w=map->dim.w;
	h=map->dim.h;
	
	for(x=0; x<w; x++){
		for(y=0; y<h/2; y++){
			col= getPix(map,x,h-y);
			setPix(map,getPix(map,x,y) ,x,h-y);
			setPix(map,col,x,y); 
		}
	}
}

void mirrorVerPixMap(pixmap* map){
	int x,y,w,h;
	color col;
	w=map->dim.w;
	h=map->dim.h;
	
	
	for(y=0; y<h; y++){
		for(x=0; x<w/2; x++){
			col= getPix(map,w-x,y);
			setPix(map,getPix(map,x,y) ,w-x,y);
			setPix(map,col,x,y); 
		}
	}
}

void rotatePixMap(pixmap** map){
/*
 * 90 degree counter clockwise
 */
	int x,y,w,h;
	pixmap* newMap;
	pixmap* oldMap;
	
	w=(*map)->dim.h;
	h=(*map)->dim.w;
	
	oldMap = *map;
	newMap = createPixMap( w, h, (*map)->max);
	
	
	for(y=0; y<h; y++){
		for(x=0; x<w; x++){
			setPix(newMap, getPix(oldMap,y,x),x,y);
		}
	}
	mirrorHorPixMap(newMap);
	*map = newMap;
}
void setMinPix(pixmap* map,unsigned char min){
	int w,h,x,y;
	w=map->dim.w;
	h=map->dim.h;
	for(y=0; y<h; y++){
		for(x=0; x<w; x++){		
			if(map->map_r[w*y+x] < min){
				map->map_r[w*y+x]=min;
			}
			if(map->map_g[w*y+x] < min){
				map->map_g[w*y+x]=min;
			}
			if(map->map_b[w*y+x] < min){
				map->map_b[w*y+x]=min;
			}
		}
	}
}
void setMaxPix(pixmap* map, unsigned char max){
	int w,h,x,y;
	w=map->dim.w;
	h=map->dim.h;
	for(y=0; y<h; y++){
		for(x=0; x<w; x++){		
			if(map->map_r[w*y+x] > max){
				map->map_r[w*y+x]=max;
			}
			if(map->map_g[w*y+x] > max){
				map->map_g[w*y+x]=max;
			}
			if(map->map_b[w*y+x] > max){
				map->map_b[w*y+x]=max;
			}
		}
	}	
}
