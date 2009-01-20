///**************************************************\\\
||| Derni�re modification : Le 06/02/08 par Gregouar |||
///**************************************************///


#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <dirent.h>

#include "main.h"
#include "MoteurGraphique.h"
#include "MoteurSons.h"

using namespace std;
//using namespace sf;

Console console;
Configuration configuration;
MoteurGraphique moteurGraphique;
MoteurSons moteurSons;


int main ( int argc, char** argv )
{
    srand(time(NULL));

    console.Ajouter("---------------------------------------------------------------------------------");
    console.Ajouter("Demarrage du jeu",0);
    console.Ajouter("---------------------------------------------------------------------------------");
    console.Ajouter("");

    try
	{
	    ///Chargement de la configuration

        configuration.Charger();

        moteurGraphique.Charger();

        ///On d�marre le jeu
		Jeu m_jeu;

		m_jeu.Demarrer();
	}
	catch (const std::string& str)
	{
	    console.Ajouter("FATAL ERROR : ",1);
		console.Ajouter(str,1);

		return EXIT_FAILURE;
	}


	console.Ajouter("");
	console.Ajouter("Sauvegarde de la configuration");

    configuration.Sauvegarder();

    delete[] configuration.text_benedictions;

    console.Ajouter("");
    console.Ajouter("EXIT_SUCCESS");

    return EXIT_SUCCESS;
}
