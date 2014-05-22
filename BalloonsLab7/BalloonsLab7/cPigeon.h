/*
=================
cPigeon.h
- Header file for class definition - SPECIFICATION
- Header file for the Sprite class
=================
*/
#ifndef _CPIGEON_H
#define _CPIGEON_H
#include "cSprite.h"

class cPigeon : public cSprite
{
private:
	int mScore;
	eDirection mDirection;

public:
	cPigeon();
	cPigeon(D3DXVECTOR3 sPosition, LPDIRECT3DDEVICE9 pd3dDevice, LPCSTR theFilename); // Constructor
	void update(float deltaTime);					// update the pigeons position based on the translation
	eDirection getDirection();      // Rturns the current direction of the sprite
	void setDirection(eDirection theDirection); // Sets the current direction for the sprite.
};
#endif