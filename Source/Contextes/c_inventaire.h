

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




#ifndef C_INVENTAIRE
#define C_INVENTAIRE

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "contexte.h"

class c_Inventaire : public Contexte
{
public:
    c_Inventaire(Jeu *jeu);
    void Utiliser(Jeu *jeu);

    void setTrader(std::vector<Objet>,Classe *);

    sf::View camera;

private:
    float tempsActuel,tempsPrecedent,temps_ecoule,tempsEcouleDepuisDernierAffichage,m_decalage;
    bool m_afficher;

    std::vector<Objet> m_trader;

    void AfficherTrader();
};

#endif


