/*
=================
cPigeon.cpp
- Header file for class definition - IMPLEMENTATION
=================
*/
#include "cPigeon.h"
/*
=================
- Data constructor initializes the cPigeon to the data passed to 
- the constructor from the paramater sPosition.
- Is always called, thus ensures all sprite objects are in a consistent state.
=================
*/
cPigeon::cPigeon(): cSprite() 			// Default constructor
{
	/* initialize random seed: */
	srand ( (unsigned int)time(NULL) );
	/* generate secret number: */
	mScore = (rand() % 10 + 1)*10;
	mDirection = static_cast<eDirection>(rand() % 2);
}
cPigeon::cPigeon(D3DXVECTOR3 sPosition, LPDIRECT3DDEVICE9 pd3dDevice, LPCSTR theFilename): cSprite(sPosition,pd3dDevice,theFilename)  // Constructor
{
	/* initialize random seed: */
	srand ( (unsigned int)time(NULL) );
	/* generate secret number: */
	mScore = (rand() % 10 + 1)*10;
	mDirection = static_cast<eDirection>(rand() % 2);
}
/*
=================
- update the position etc of the sprite.
=================
*/
void cPigeon::update(float deltaTime) 					// update the pigeons position based on the translation
{
	D3DXVECTOR2 tempV2;
	D3DXVECTOR3 tempV3;
	tempV2 = cPigeon::getSpritePos2D() + (cPigeon::getTranslation())*deltaTime;
	tempV3 = D3DXVECTOR3(tempV2.x,tempV2.y,0.0f);
	cPigeon::setSpritePos(tempV3);
	cSprite::update();
}
/*
=================
- Returns the current direction of the sprite.
=================
*/
eDirection cPigeon::getDirection()       // Rturns the current direction of the sprite
{
	return cPigeon::mDirection;
}
/*
=================
- Sets the current direction for the sprite.
=================
*/
void cPigeon::setDirection(eDirection theDirection)  // Sets the current direction for the sprite.
{
	cPigeon::mDirection = theDirection;
}
