#include "Tile.h"

#include <iostream>
#include <fstream>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

using namespace std;

Tile::Tile()
{
}
Tile::~Tile()
{
}

Tile Tile::operator=(const Tile &tile)
{
	m_positionDansLImage=tile.m_positionDansLImage;
	m_image=tile.m_image;
	m_collision=tile.m_collision;
	m_animation=tile.m_animation;
	m_son=tile.m_son;
	m_lumiere=tile.m_lumiere;
	m_ombre=tile.m_ombre;
	m_orientation=tile.m_orientation;
	m_transparent=tile.m_transparent;
    return *this;
}

void Tile::setTile(coordonnee positionDansLImage,int image,bool collision,int animation,int son,Lumiere lumiere,bool ombre,char orientation,bool transparent)
{
	m_positionDansLImage=positionDansLImage;
	m_image=image;
	m_collision=collision;
	m_animation=animation;
	m_son=son;
	m_lumiere=lumiere;
	m_ombre=ombre;
	m_orientation=orientation;
	m_transparent=transparent;
}

coordonnee Tile::getCoordonnee(){return m_positionDansLImage;}
int Tile::getImage(){return m_image;}
bool Tile::getCollision(){return m_collision;}
int Tile::getAnimation(){return m_animation;}
int Tile::getSon(){return m_son;}
Lumiere Tile::getLumiere(){return m_lumiere;}
bool Tile::getOmbre(){return m_ombre;}
bool Tile::getTransparent(){return m_transparent;}
char Tile::getOrientation(){return m_orientation;}
