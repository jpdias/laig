#include "Tabuleiro.h"
#include <iostream>
using std::cout;

Tabuleiro::Tabuleiro(){
	text1=true;
	obj.x1=-0.5;
	obj.y1=-0.5;
	obj.x2=0.5;
	obj.y2=0.5;
	
	
	dirt=new CGFappearance("../data/dirt.jpg",1,1);
	rock=new CGFappearance("../data/rock.jpg",1,1);
	black = new CGFappearance("../data/black.jpg",1,1);
	white = new CGFappearance("../data/white.jpg",1,1);
}

void Tabuleiro::draw(int text1){
	obj.x1=-0.5;
	obj.y1=-0.5;
	obj.x2=0.5;
	obj.y2=0.5;
	glPushMatrix();
	glTranslatef(27.5, 0, -27.5);
	glRotatef(90,0,0,1);
	glScaled(5,5,5);
	float sp[4] = {0.1, 0.1,0.1, 0};
	float a[4] = {0.8, 0.8,0.8, 0};
	float z[4] = {0.3, 0.3,0.3, 0};
	rock->setSpecular(sp);
	rock->setAmbient(a);
	rock->setDiffuse(z);
	rock->setShininess(0.7);
	dirt->setSpecular(sp);
	dirt->setAmbient(a);
	dirt->setDiffuse(z);
	

	black->setSpecular(sp);
	black->setAmbient(a);
	black->setDiffuse(z);
	black->setShininess(0.7);
	white->setSpecular(sp);
	white->setAmbient(a);
	white->setDiffuse(z);
	rock->setShininess(0.7);
//	glPushName(-1);		// Load a default name

	// example 2: structured naming
	for (int r=0; r < NUM_ROWS; r++)
	{
		glPushMatrix();
		glTranslatef(0, r, 0);
		glLoadName(r);
		for (int c=0; c < NUM_COLS; c++)
		{
			
			glPushMatrix();
			glTranslatef(0,0,c);
			glRotatef(90,0,1,0);
			glPushName(c);
			glPushMatrix();
			if(c%2==0 && r%2==0)
				if(text1)
				rock->apply();
				else
					black->apply();
			else if(c%2!=0 && r%2==0)
				if(text1)
				dirt->apply();
				else
					white->apply();
			else if(c%2==0 && r%2!=0)
				if(text1)
				dirt->apply();
				else
					white->apply();
			else
				if(text1)
				rock->apply();
				else
					black->apply();
			obj.draw();
			
			glPopMatrix();
			glPopName();
			glPopMatrix();
		}
		glPopMatrix();
	}

		glPopMatrix();

}

