

/*Copyright (C) 2009 Naisse Gr�goire

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.*/


#include "c_jeu.h"
#include "jeu.h"

#include "Globale.h"

#include <iostream>
#include <fstream>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

using namespace sf;
using namespace std;

c_Chargement::c_Chargement(Jeu *jeu)
{
    z=0;
    augmenterNoir = true;
}

void c_Chargement::setC_Chargement(int numeroMap,coordonnee coordonneePerso,bool debut)
{
    moteurGraphique->LightManager->Delete_All_Light();
    moteurGraphique->LightManager->Delete_All_Wall();


    tempsActuel=0,tempsPrecedent=0,temps_ecoule=0,tempsEcouleDepuisDernierAffichage=0;

    m_coordonneePerso.x=coordonneePerso.x;
    m_coordonneePerso.y=coordonneePerso.y;
    numeroProchaineMap=numeroMap;
    z=50;
    augmenterNoir = true;

    m_debut=debut;

    char chemin[255];
    vector <string> cheminFond;


    cDAT reader;
    reader.Read(configuration->chemin_maps);
    sprintf(chemin,"map%i.map.hs",numeroMap);

    ifstream *fichier=reader.GetInfos(chemin);
    if(fichier)
    {
        char caractere;
        do
        {
            //Chargement du nom
            fichier->get(caractere);
            if(caractere=='*')
            {
                *fichier>>nomMap;
                for(int i=0;i<(int)nomMap.size();i++)
                    if(nomMap[i]=='_')
                        nomMap[i]=' ';
            }
        }while(caractere!='$');

        do
        {
            //Chargement du nom
            fichier->get(caractere);
            if(caractere=='*')
            {
                string temp;
                *fichier>>temp;
                cheminFond.push_back(temp);

            }
        }while(caractere!='$');

        fichier->close();
    }


    delete fichier;

    int random=rand()%cheminFond.size();

    m_fond=moteurGraphique->AjouterImage(cheminFond.at(random));

    cheminFond.clear();

    allerVersImageChargement=true;
    m_tempsChargement=0;
}

void c_Chargement::Utiliser(Jeu *jeu)
{
    jeu->m_display=true;
    jeu->hero.placerCamera(&jeu->camera,jeu->map.getDimensions());
    jeu->ecran.SetView(jeu->camera);

    if(configuration->Lumiere)
        jeu->map.calculerOmbresEtLumieres();

    temps_ecoule=0;
    temps_ecoule=jeu->Clock.GetElapsedTime();
    tempsEcouleDepuisDernierAffichage+=temps_ecoule;
    jeu->Clock.Reset();

    jeu->eventManager.GererLesEvenements(&jeu->ecran,&jeu->camera,&jeu->m_run,temps_ecoule,jeu->map.getDimensions());

    //moteurGraphique->LightManager->Generate(&jeu->camera);


    if(z>=49&&!augmenterNoir&&allerVersImageChargement)
    {
        jeu->Clock.Reset();
        jeu->hero.m_personnage.setCoordonnee(m_coordonneePerso);

        moteurGraphique->ViderParticules();

        if(!m_debut)
            jeu->map.Sauvegarder(&jeu->hero);

        jeu->map.Detruire();

        jeu->hero.m_personnage.m_light=moteurGraphique->LightManager->Add_Dynamic_Light(sf::Vector2f(0,0),224,384,16,sf::Color(255,255,255));

        jeu->hero.Sauvegarder();

        jeu->hero.ChargerModele(true);

        if(!jeu->map.Charger(numeroProchaineMap,&jeu->hero))
            console->Ajouter("CRITICAL ERROR"), throw  "CRITICAL ERROR";

        moteurGraphique->DecrementerImportance();

        jeu->hero.placerCamera(&jeu->camera,jeu->map.getDimensions());
        jeu->camera.Zoom(configuration->zoom);

        coordonnee position;
        position.x=(jeu->hero.m_personnage.getCoordonnee().x-jeu->hero.m_personnage.getCoordonnee().y-1+jeu->map.getDimensions().y)/5;
        position.y=(jeu->hero.m_personnage.getCoordonnee().x+jeu->hero.m_personnage.getCoordonnee().y)/5;
        Listener::SetGlobalVolume((float)configuration->volume);
        Listener::SetPosition(-position.x, 0, position.y);
        Listener::SetTarget(0, 0, 1);
        jeu->map.musiquePlay(position);

        if(configuration->Lumiere)
            jeu->map.calculerOmbresEtLumieres();

        configuration->RafraichirLumiere=true;

        sf::Vector2f pos;
        pos.x=(((jeu->hero.m_personnage.getCoordonneePixel().x-jeu->hero.m_personnage.getCoordonneePixel().y)*64/COTE_TILE+jeu->map.getDimensions().y*64));
        pos.y=(((jeu->hero.m_personnage.getCoordonneePixel().x+jeu->hero.m_personnage.getCoordonneePixel().y)*64/COTE_TILE)/2+32)*2;

        moteurGraphique->LightManager->SetPosition(jeu->hero.m_personnage.m_light,pos);
        allerVersImageChargement=false;

        jeu->Clock.Reset();
    }


     if(z<=1&&augmenterNoir)
        augmenterNoir=false,z=1;
    if(z>=49&&!augmenterNoir)
        augmenterNoir=true,z=49;

    if(augmenterNoir)
        z-=temps_ecoule*200;
    else
        z+=temps_ecoule*200;


    if(z>50)
        z=50;
    if(z<0)
        z=0;


    if (sf::PostFX::CanUsePostFX() == true&&configuration->postFX)
    {
        if(allerVersImageChargement&&z<49&&augmenterNoir||!allerVersImageChargement&&z>0&&!augmenterNoir)
        {
            jeu->camera.Zoom(configuration->zoom);
            jeu->map.setVolumeMusique((int)(z*(float)configuration->volume/50));
            if(!m_debut&&augmenterNoir||!augmenterNoir)
            {
                coordonnee temp;
                jeu->map.Afficher(&jeu->ecran,&jeu->camera,1,&jeu->hero,temp,0);

                if(configuration->Minimap)
                    jeu->menu.Afficher(&jeu->ecran,2,255,&jeu->hero.m_classe);

                jeu->menu.Afficher(&jeu->ecran,1,255,&jeu->hero.m_classe);
                jeu->menu.AfficherDynamique(&jeu->ecran,jeu->hero.m_caracteristiques,0,jeu->hero.m_personnage.getCaracteristique(),&jeu->hero.m_classe);
            }
        }
        else
        jeu->menu.AfficherChargement(&jeu->ecran,nomMap,m_fond,50);

        configuration->effetNoir=((float)z)/50;
    }
    else
    {
        if(allerVersImageChargement&&z<49&&augmenterNoir||!allerVersImageChargement&&z>0&&!augmenterNoir)
        {
            jeu->map.setVolumeMusique((int)(z*(float)configuration->volume/50));
            if(!m_debut&&augmenterNoir||!augmenterNoir)
            {
                coordonnee temp;
                jeu->map.Afficher(&jeu->ecran,&jeu->camera,1,&jeu->hero,temp,0);
                if(configuration->Minimap)
                {
                    jeu->menu.Afficher(&jeu->ecran,1,255,&jeu->hero.m_classe);
                    jeu->map.Afficher(&jeu->ecran,&jeu->camera,2,&jeu->hero,temp,0);
                    jeu->menu.Afficher(&jeu->ecran,2,255,&jeu->hero.m_classe);
                }

                jeu->menu.AfficherDynamique(&jeu->ecran,jeu->hero.m_caracteristiques,0,jeu->hero.m_personnage.getCaracteristique(),&jeu->hero.m_classe);
            }
        }
        else
            jeu->menu.AfficherChargement(&jeu->ecran,nomMap,m_fond,(int)z);
    }

    if(z>=49&&!augmenterNoir&&!allerVersImageChargement)
    {
        jeu->m_contexte = jeu->m_jeu;
       // jeu->m_contexte->CopierCamera(jeu->m_jeu->camera);
    }
}
