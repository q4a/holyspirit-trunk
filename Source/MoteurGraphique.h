#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#ifndef MOTEURGRAPHIQUEH
#define MOTEURGRAPHIQUEH

#include "map.h"
#include "commande.h"
#include "moteurParticule.h"

struct Image_moteur
{
    sf::Image img;
    std::string nom;
    int importance;
};

class MoteurGraphique
{
	public:
	MoteurGraphique();
	~MoteurGraphique();
	void Afficher(sf::RenderWindow *, sf::View *);

	void Gerer(sf::RenderWindow *,float,int);
	void CalculerLumiereParticules(LumiereOmbrage tableauDesLampes[30][30],coordonnee ,sf::RenderWindow *,int tailleMapY);

	int AjouterImage(std::string,int importance = 3);
	int AjouterImage(const char *Data, std::size_t SizeInBytes, std::string nom,int importance =5);
	int AjouterModeleSystemeParticules(std::string);

	void AjouterSystemeParticules(int ID,coordonnee position,sf::Color color,float force,float angle);
	void AjouterCommande(sf::Sprite*,int=0, bool=0);
	void AjouterTexte(sf::String*, int couche=0,bool titre=false);

	void DecrementerImportance();

	void Charger();

	void Vider();
	void ViderParticules();

	void calculerOmbresEtLumieres(sf::RenderWindow* ecran,Hero *hero,sf::View *camera,Map *map);
	void AjouterCommandesMap(Map *map);

	sf::Image* getImage(int IDimage);
	ModeleParticuleSysteme* getModeleMoteurParticules(int ID);

	std::string getCheminImage(int IDimage);



	sf::PostFX EffectBlur,EffectMort,EffectNoir,EffectContrastes;

	sf::Font m_font_titre;

	float m_blur;

	private:

	std::vector <Image_moteur> m_images;

	std::vector <ModeleParticuleSysteme> m_modeleSystemeParticules;
	//std::vector <std::string> m_cheminsImages;//Permet de s'assurer d'avoir des singletons


	std::vector <sf::String> m_textes[20];
	std::vector <Commande> m_commandes[20];
	std::vector <ParticuleSysteme> m_systemeParticules;


};

#endif



