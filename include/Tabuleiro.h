#ifndef TABULEIRO_H
#define TABULEIRO_H


#include "Rectangle.h"
#include "CGFscene.h"
#include "CGFshader.h"
#include "CGFaxis.h"
#include "CGFapplication.h"
#include "Peca.h"
#include <map>

#define NUM_ROWS 24
#define NUM_COLS 24

class Tabuleiro: public CGFobject {

	public:
		int text1;
		void draw(int text1);
		void drawbox(bool text2);
		Tabuleiro();
		void initTab();
		void placePieces();
		CGFappearance* dirt;
		CGFappearance* rock;
		CGFappearance* black;
		CGFappearance* white;
		vector<vector<string> > board;
		vector<Peca*> jog1Pecas,jog2Pecas,tabPecas;
		Rectangle obj;


		void move(int x1, int y1, int x2, int y2, Peca * obj);
		
		
		CGFappearance* wall;
//	Rectangle obj;
	Rectangle cube;

	Object *ark;
Object 	*tower;
	Object *seargent;
	 Object *knight;
	 Object *duke;
	 Object *prince;
	Object *squire;
	Object *king;
	Object *pike;
	map<string,Object*> mapObjects;
};

#endif