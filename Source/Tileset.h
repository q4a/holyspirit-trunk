///**************************************************\\\
||| Derni�re modification : Le 06/02/08 par Gregouar |||
///**************************************************///




#ifndef TILESETH
#define TILESETH

#include <iostream>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "tile.h"
#include "Hero.h"

class Tileset
{
	public:
	Tileset();
	Tileset(std::string chemin);
	~Tileset();
	Tileset operator=(const Tileset &tileset);
	bool Charger(std::string chemin);
	int getImage(int tile);
	coordonnee getPositionDuTile(int tile);
	bool getCollisionTile(int tile);
	int getAnimationTile(int tile);
	int getSonTile(int tile);
	Lumiere Tileset::getLumiereDuTile(int tile);
	bool getOmbreDuTile(int tile);
	bool getTransparentDuTile(int tile);
	char getOrientationDuTile(int tile);
	coordonnee getCentreDuTile(int tile);
	std::string getChemin(){return m_chemin;}
	float getTempsDuTile(int tile);

	int getTaille();

	void jouerSon(int numeroSon,coordonnee position,coordonnee positionHero);
	//void remiseAZeroDesSons();

	void deleteTiles();

	private:
	//std::vector <bool> m_sonAZero;
	std::vector <int> m_image;
	std::vector <Tile> m_tile;
	std::vector <int> m_sons;
	//std::vector <int> m_sons;
	//std::vector <sf::Sound> m_sons;
	std::string m_chemin;
};

class Herbe : public Tileset
{
    private:

};

#endif


