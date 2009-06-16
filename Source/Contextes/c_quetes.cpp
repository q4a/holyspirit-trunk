

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
#include "../jeu.h"

#include "../globale.h"

#include <iostream>
#include <fstream>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

using namespace sf;
using namespace std;

c_Quetes::c_Quetes()
{
    m_decalage=-600;
}

void c_Quetes::Utiliser(Jeu *jeu)
{
    if (m_decalage<=-600)
        m_afficher=1;

    temps_ecoule=jeu->Clock.GetElapsedTime();
    jeu->m_display=true;
    jeu->Clock.Reset();

    jeu->hero.PlacerCamera(jeu->map->getDimensions()); // On place la camera suivant ou se trouve le perso

    jeu->map->Afficher(&jeu->hero,0,jeu->m_jeu->alpha_map);
    jeu->menu.Afficher(2,jeu->m_jeu->alpha_map,&jeu->hero.m_classe);

    if (m_afficher)
        m_decalage+=temps_ecoule*2000;
    else
        m_decalage-=temps_ecoule*2000;

    if (m_decalage>0)
        m_decalage=0;
    if (m_decalage<-600)
    {
        m_decalage=-600;
        jeu->m_contexte=jeu->m_jeu,eventManager->StopEvenement(Key::Q,"ET");
    }

    jeu->menu.AfficherQuetes(m_decalage,&jeu->hero.m_classe);

    jeu->hero.AfficherQuetes(m_decalage);

    jeu->map->AfficherSac(jeu->hero.m_personnage.getCoordonnee(),m_decalage,jeu->hero.m_classe.position_sac_inventaire,jeu->hero.m_caracteristiques);

    jeu->menu.Afficher(1,255,&jeu->hero.m_classe);
    jeu->menu.AfficherDynamique(jeu->hero.m_caracteristiques,-1,jeu->hero.m_caracteristiques,&jeu->hero.m_classe);

    eventManager->AfficherCurseur();

    if (eventManager->getEvenement(Key::Q,"ET")||eventManager->getEvenement(Key::Escape,"ET"))
    {
        m_afficher=0;
        jeu->Clock.Reset();
        eventManager->StopEvenement(Key::Q,"ET");
    }

    if (eventManager->getEvenement(Mouse::Left,"C"))
    {
        eventManager->StopEvenement(Mouse::Left,"C");
        jeu->hero.m_queteSelectionnee = jeu->hero.m_quetePointee;
    }

    /*if (eventManager->getEvenement(Mouse::Left,"C"))
    {
        if (jeu->map->RamasserObjet(&jeu->hero,1))
            eventManager->StopEvenement(Mouse::Left,"C");
    }

    if (eventManager->getEvenement(Mouse::Left,"C"))
    {
        if (jeu->hero.PrendreEnMain(m_trader))
            if (jeu->hero.m_objetADeposer>=0)
                jeu->map->AjouterObjet(jeu->hero.DeposerObjet());
        eventManager->StopEvenement(Mouse::Left,"C");
    }

    jeu->hero.m_defilement_trader -= eventManager->getMolette();*/

    /*if(eventManager->getEvenement(Mouse::Left,"CA"))
        if (eventManager->getPositionSouris().x>jeu->hero.m_classe.scroll_button.position.x*configuration->Resolution.x/800
        &&eventManager->getPositionSouris().x<(jeu->hero.m_classe.scroll_button.position.x+jeu->hero.m_classe.scroll_button.position.w)*configuration->Resolution.x/800
        &&eventManager->getPositionSouris().y>jeu->hero.m_classe.scroll_button.position.y*configuration->Resolution.h/600
        &&eventManager->getPositionSouris().y-jeu->hero.m_classe.scroll_button.position.h<(jeu->hero.m_classe.scroll_button.position.y+jeu->hero.m_classe.position_contenu_marchand.h*24)*configuration->Resolution.h/600)
            jeu->hero.m_defilement_trader = (int)((((float)eventManager->getPositionSouris().y-jeu->hero.m_classe.scroll_button.position.h*0.25-(float)jeu->hero.m_classe.scroll_button.position.y)/((float)jeu->hero.m_classe.position_contenu_marchand.h*24))*(float)(jeu->hero.m_max_defilement_trader-jeu->hero.m_classe.position_contenu_marchand.h));

    if (jeu->hero.m_defilement_trader<0)
        jeu->hero.m_defilement_trader=0;
    if (jeu->hero.m_defilement_trader>jeu->hero.m_max_defilement_trader-jeu->hero.m_classe.position_contenu_marchand.h)
        jeu->hero.m_defilement_trader=jeu->hero.m_max_defilement_trader-jeu->hero.m_classe.position_contenu_marchand.h;*/
}

