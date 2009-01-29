#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>


#ifndef _CONTEXTEH
#define _CONTEXTEH

class Jeu;
class c_Jeu;

class Contexte
{
    public:
    virtual ~Contexte();

	virtual void Utiliser(Jeu *jeu) = 0;


    Contexte operator=(const c_Jeu &c_jeu);

};

#endif

