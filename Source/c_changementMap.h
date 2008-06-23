
#ifndef C_CHARGEMENT
#define C_CHARGEMENT

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "Contexte.h"

class c_Chargement : public Contexte
{
    public:
    c_Chargement(Jeu *jeu);
    void Utiliser(Jeu *jeu);
    void setC_Chargement(int numeroMap,coordonnee coordonneePerso);

    sf::View camera;

    private:

	sf::PostFX EffectNoir;

	float tempsActuel,tempsPrecedent,temps_ecoule,tempsEcouleDepuisDernierAffichage;
	float z;
	bool augmenterNoir;
	coordonnee m_coordonneePerso;
	int numeroProchaineMap;

};

#endif
