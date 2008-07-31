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


using namespace std;
//using namespace sf;

Console console;
Configuration configuration;
MoteurGraphique moteurGraphique;


int main ( int argc, char** argv )
{
    srand(time(NULL));

    coordonnee resolution;
    bool video;
    string chemin_video_developpeur,nom_video_developpeur;

    ///Chargement de la configuration

    ifstream fichier;
    fichier.open("configuration.conf", ios::in);
    if(fichier)
    {
    	char caractere;
    	string chaine;
    	while(fichier>>chaine)
    	{

    			if(chaine=="resolution:") { fichier>>resolution.x>>resolution.y; }
    			if(chaine=="luminosite:") { fichier>>configuration.luminosite; }
    			if(chaine== "mode_fenetre:") { fichier>>configuration.mode_fenetre; }
    			if(chaine== "videos:") { fichier>>video; }
    			if(chaine== "postFX:") { fichier>>configuration.postFX; }
    			if(chaine== "sang:") { fichier>>configuration.sang; }
    			if(chaine== "ombres:") { fichier>>configuration.Ombre; }
    			if(chaine== "lumieres:") { fichier>>configuration.Lumiere; }
    			if(chaine== "herbes:") { fichier>>configuration.Herbes; }
    			if(chaine== "syncronisation_verticale:") { fichier>>configuration.syncronisation_verticale; }
                if(chaine== "volume:") { fichier>>configuration.volume; }
                if(chaine== "minimap:") { fichier>>configuration.Minimap; }
                if(chaine== "lumiere_fondu:") { fichier>>configuration.FonduLumiere; }
                if(chaine== "amelioration_des_lampes:") { fichier>>configuration.amelioration_lampes; }
                if(chaine== "frequence_lumiere:") { fichier>>configuration.frequence_lumiere; }
                if(chaine== "console:") { fichier>>configuration.console; }
                if(chaine== "version:") { fichier>>configuration.version; }

                if(chaine== "maps:") { fichier>>configuration.chemin_maps; }
                if(chaine== "temps:") { fichier>>configuration.chemin_temps; }
                if(chaine== "saves:") { fichier>>configuration.chemin_saves; }
                if(chaine== "evenements:") { fichier>>configuration.chemin_evenements; }
                if(chaine== "curseur:") { fichier>>configuration.chemin_curseurs; }
                if(chaine== "menus:") { fichier>>configuration.chemin_menus; }
                if(chaine== "fonts:") { fichier>>configuration.chemin_fonts; }
                if(chaine== "fx:") { fichier>>configuration.chemin_fx; }
                if(chaine== "chemin_video_developpeurs:") { fichier>>chemin_video_developpeur; }
                if(chaine== "nom_video_developpeurs:") { fichier>>nom_video_developpeur; }

                if(chaine== "curseur_base:") { fichier>>configuration.nom_curseur_base; }
                if(chaine== "fxNoir:") { fichier>>configuration.nom_effetNoir; }
                if(chaine== "fxMort:") { fichier>>configuration.nom_effetMort; }
                if(chaine== "fxLuminosite:") { fichier>>configuration.nom_effetLuminosite; }

                if(chaine== "menu_hud:") { fichier>>configuration.nom_hud; }
                if(chaine== "menu_minimap:") { fichier>>configuration.nom_minimap; }
                if(chaine== "menu_bulle_vie:") { fichier>>configuration.nom_bulle_vie; }
                if(chaine== "menu_ame:") { fichier>>configuration.nom_ame; }
                if(chaine== "menu_barre_ame:") { fichier>>configuration.nom_barre_ame; }
                if(chaine== "menu_barre_vie:") { fichier>>configuration.nom_barre_vie; }
                if(chaine== "menu_barre_vie_vide:") { fichier>>configuration.nom_barre_vie_vide; }
                if(chaine== "menu_inventaire:") { fichier>>configuration.nom_inventaire; }


                if(chaine== "menu_sang:") { fichier>>configuration.nom_sang; }
                if(chaine== "nom_video_developpeurs: ") { fichier>>nom_video_developpeur; }

                if(chaine== "frequence_sauvegarde:") { fichier>>configuration.frequence_sauvegarde; }

                if(chaine== "son_mort:") { fichier>>configuration.chemin_son_mort; }

    	}
    	 fichier.close();
    }
    else
    {

        fstream fichier2("configuration.conf", ios::in | ios::out | ios::trunc) ;
        fichier2<<"AFFICHAGE\nresolution: 800 600\nluminosite: 50\nsyncronisation_verticale: 0\nmode_fenetre: 1\nombres: 1\nlumieres: 1\nlumiere_fondu: 0\namelioration_des_lampes: 1\nfrequence_lumiere: 0.054\nherbes: 1\npostFX: 1\nsang: 1\n\nMUSIQUES\nvolume: 100\nsons_mort: Data/Sons/Coeur.wav\n\nINTERFACE\nminimap: 1\nconsole: 0\n\nVERSION\nversion: 0.0.86\n\nCHEMINS\nmaps: Data/Maps/map\ntemps: Data/Temps/\nsaves: Data/Saves/\nevenements: Data/Maps/evenement\ncurseur: Data/Menus/Curseurs/\nmenus: Data/Menus/\nfonts: Data/Fonts/Cup and Talon.ttf\nfx: Data/Effets/\n\ncurseur_base: curseurMenu.png\n\nfxNoir: EffetNoir.sfx\nfxMort: EffetMort.sfx\nfxLuminosite: EffetLuminosite.sfx\n\nmenu_hud: Hud.png\nmenu_minimap: minimap.png\nmenu_bulle_vie: bulle_vie.png\nmenu_ame: Ame.png\nmenu_barre_ame: Barre_ame.png\nmenu_barre_vie: barre_vie.png\nmenu_barre_vie_vide: barre_vie_vide.png\nmenu_sang: sang.png\nmenu_inventaire: inventaire.png\n\nAUTRE\nvideos: 1\nchemin_video_developpeurs: Data/Videos/Naiponcin logo/\nnom_video_developpeurs: Naiponcin logo\n\nfrequence_sauvegarde: 5\n";
        fichier2.seekg(0, ios::beg);

        //fichier.open("configuration.txt", ios::in);
        //if(fichier)
        //{
            char caractere;
            string chaine;
            while(fichier2>>chaine)
            {
                    if(chaine=="resolution:") { fichier2>>resolution.x>>resolution.y; }
                    if(chaine=="luminosite:") { fichier2>>configuration.luminosite; }
                    if(chaine== "mode_fenetre:") { fichier2>>configuration.mode_fenetre; }
                    if(chaine== "videos:") { fichier2>>video; }
                    if(chaine== "postFX:") { fichier2>>configuration.postFX; }
                    if(chaine== "sang:") { fichier2>>configuration.sang; }
                    if(chaine== "ombres:") { fichier2>>configuration.Ombre; }
                    if(chaine== "lumieres:") { fichier2>>configuration.Lumiere; }
                    if(chaine== "herbes:") { fichier>>configuration.Herbes; }
                    if(chaine== "syncronisation_verticale:") { fichier2>>configuration.syncronisation_verticale; }
                    if(chaine== "volume:") { fichier2>>configuration.volume; }
                    if(chaine== "minimap:") { fichier2>>configuration.Minimap; }
                    if(chaine== "lumiere_fondu:") { fichier2>>configuration.FonduLumiere; }
                    if(chaine== "amelioration_des_lampes:") { fichier2>>configuration.amelioration_lampes; }
                    if(chaine== "frequence_lumiere:") { fichier2>>configuration.frequence_lumiere; }
                    if(chaine== "console:") { fichier2>>configuration.console; }
                    if(chaine== "version:") { fichier2>>configuration.version; }

                    if(chaine== "maps:") { fichier2>>configuration.chemin_maps; }
                    if(chaine== "temps:") { fichier2>>configuration.chemin_temps; }
                    if(chaine== "saves:") { fichier2>>configuration.chemin_saves; }
                    if(chaine== "evenements:") { fichier2>>configuration.chemin_evenements; }
                    if(chaine== "curseur:") { fichier2>>configuration.chemin_curseurs; }
                    if(chaine== "menus:") { fichier2>>configuration.chemin_menus; }
                    if(chaine== "fonts:") { fichier2>>configuration.chemin_fonts; }
                    if(chaine== "fx:") { fichier2>>configuration.chemin_fx; }
                    if(chaine== "chemin_video_developpeurs:") { fichier2>>chemin_video_developpeur; }
                    if(chaine== "nom_video_developpeurs:") { fichier2>>nom_video_developpeur; }

                    if(chaine== "curseur_base:") { fichier2>>configuration.nom_curseur_base; }
                    if(chaine== "fxNoir:") { fichier2>>configuration.nom_effetNoir; }
                    if(chaine== "fxMort:") { fichier>>configuration.nom_effetMort; }
                    if(chaine== "fxLuminosite:") { fichier2>>configuration.nom_effetLuminosite; }
                    if(chaine== "menu_hud:") { fichier2>>configuration.nom_hud; }
                    if(chaine== "menu_minimap:") { fichier2>>configuration.nom_minimap; }
                    if(chaine== "menu_ame:") { fichier2>>configuration.nom_ame; }
                    if(chaine== "menu_barre_ame:") { fichier2>>configuration.nom_barre_ame; }
                    if(chaine== "menu_barre_vie:") { fichier2>>configuration.nom_barre_vie; }
                    if(chaine== "menu_barre_vie_vide:") { fichier2>>configuration.nom_barre_vie_vide; }
                    if(chaine== "menu_sang:") { fichier2>>configuration.nom_sang; }
                    if(chaine== "menu_bulle_vie:") { fichier2>>configuration.nom_bulle_vie; }
                    if(chaine== "menu_inventaire:") { fichier2>>configuration.nom_inventaire; }

                    if(chaine== "frequence_sauvegarde: ") { fichier2>>configuration.frequence_sauvegarde; }

                    if(chaine== "son_mort: ") { fichier2>>configuration.chemin_son_mort; }


            }
             fichier2.close();
        //}
        //else
        //return EXIT_FAILURE;
    }

    ///Application de la configuration

    configuration.Resolution.x=resolution.x;
    configuration.Resolution.y=resolution.y;

    configuration.zoom=1;

    if(!configuration.Lumiere)
        configuration.Ombre=0;

    //sf::RenderWindow ecran;

    ///Chargement et affichage de la play-intro

   /* if(video)
    {
    ecran.Create(sf::VideoMode(resolution.x, resolution.y, 32), "HolySpirit : Act of Faith");
    if(configuration.plein_ecran==true)
		ecran.Create(sf::VideoMode(resolution.x, resolution.y, 32), "HolySpirit : Act of Faith", sf::Style::Fullscreen);

	ecran.ShowMouseCursor(false);

	struct dirent *lecture;
	int tailleVideo=-2;

	DIR *repertoire;
    repertoire = opendir(chemin_video_developpeur.c_str());
    while ((lecture = readdir(repertoire)))
    {
       tailleVideo++;
    }
    closedir(repertoire);

	lireVideo(&ecran,chemin_video_developpeur+nom_video_developpeur,tailleVideo);
	cout<<chemin_video_developpeur+nom_video_developpeur;
    }*/


    ///Cr�ation de la fen�tre

	/*ecran.Create(sf::VideoMode(resolution.x, resolution.y, 32), "HolySpirit : Act of Faith");
    if(configuration.plein_ecran==true)
		ecran.Create(sf::VideoMode(resolution.x, resolution.y, 32), "HolySpirit : Act of Faith", sf::Style::Fullscreen);
    if(configuration.syncronisation_verticale)
        ecran.UseVerticalSync(true);
    else
        ecran.UseVerticalSync(false);

    ecran.OptimizeForNonOpenGL(true);*/

    ///On d�marre le jeu

    try
	{
		Jeu m_jeu;

		m_jeu.Demarrer();
	}
	catch (char *str)
	{
	    console.Ajouter("FATAL ERROR : ",1);
		console.Ajouter(str,1);
		console.Rapport();

		return EXIT_FAILURE;
	}



    //if(!Jeu(&ecran))
      //  return EXIT_FAILURE;

    return EXIT_SUCCESS;
}
