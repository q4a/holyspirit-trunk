

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


#include "decor.h"
#include "../globale.h"

Decor::Decor(int tileset,int tile,const std::vector<int> &evenement,const std::vector<int> &monstre,int herbe, int couche,int hauteur)
{
    m_tileset=tileset;
    m_tile=tile;
    m_evenement=evenement;
    m_monstre=monstre;
    m_herbe=herbe;
    m_numeroHerbe=0;
    m_couche=couche;
    m_hauteur=hauteur;

    added_minimap = false;
}

Decor::Decor(int tileset,int tile,const std::vector<int> &evenement,const std::vector<int> &monstre,int herbe, int couche,int hauteur,std::vector <Objet> objets)
{
    m_tileset=tileset;
    m_tile=tile;
    m_evenement=evenement;
    m_monstre=monstre;
    m_herbe=herbe;
    m_numeroHerbe=0;
    m_couche=couche;
    m_hauteur=hauteur;

    m_objets=objets;

    added_minimap = false;
}

Decor::~Decor()
{
    m_evenement.clear();
    m_objets.clear();
}

Decor Decor::operator=(const Decor &Decor)
{
    m_tileset=Decor.m_tileset;
    m_tile=Decor.m_tile;
    m_monstre=Decor.m_monstre;
    m_projectile=Decor.m_projectile;
    m_effet=Decor.m_effet;
    m_couche=Decor.m_couche;
    m_position=Decor.m_position;
    m_herbe=Decor.m_herbe;
    m_numeroHerbe=Decor.m_numeroHerbe;
    m_herbe_taille=Decor.m_herbe_taille;
    m_herbe_decalage=Decor.m_herbe_decalage;
    m_herbe_couleur=Decor.m_herbe_couleur;
    m_hauteur=Decor.m_hauteur;

    m_evenement.resize((int)Decor.m_evenement.size(), 0);
    for (int i=0;i<(int)Decor.m_evenement.size();++i)
        m_evenement[i]=Decor.m_evenement[i];

    m_objets.resize(Decor.m_objets.size(),Objet ());
    for (int i=0;i<(int)Decor.m_objets.size();++i)
        m_objets[i]=Decor.m_objets[i];


    m_entite_graphique  = Decor.m_entite_graphique;
    m_entite_herbe      = Decor.m_entite_herbe;

    color=Decor.color;
    m_herbe_couleur=Decor.m_herbe_couleur;
    m_herbe_decalage=Decor.m_herbe_decalage;

    return *this;
}

void Decor::setDecor(int tileset,int tile,const std::vector<int> &evenement,const std::vector<int> &monstre,int herbe, int couche,int hauteur)
{
    m_tileset=tileset;
    m_tile=tile;
    m_evenement=evenement;
    m_monstre=monstre;
    m_herbe=herbe;
    m_numeroHerbe=0;
    m_couche=couche;
    m_hauteur=hauteur;

    if (m_couche>4)
        m_couche=4;
    if (m_couche<0)
        m_couche=0;
}

void Decor::setDecor(int tileset,int tile,const std::vector<int> &evenement,const std::vector<int> &monstre,int herbe, int couche,int hauteur,std::vector <Objet> objets)
{
    m_tileset=tileset;
    m_tile=tile;
    m_evenement=evenement;
    m_monstre=monstre;
    m_herbe=herbe;
    m_numeroHerbe=0;
    m_couche=couche;
    m_hauteur=hauteur;

    if (m_couche>4)
        m_couche=4;
    if (m_couche<0)
        m_couche=0;
    m_objets=objets;
}

bool Decor::AfficherTexteObjet(coordonnee position,int objet, float *decalage)
{
    bool retour=false;
    if(objet>=0&&objet<(int)m_objets.size())
    {

        float alpha = m_objets[objet].m_alpha;
        if(alpha > 255)
            alpha = 255;
        alpha /= 255;

        sf::Text texte;
        sf::Sprite sprite;

        texte.SetColor(sf::Color(   GetItemColor(m_objets[objet].getRarete()).r,
                                    GetItemColor(m_objets[objet].getRarete()).g,
                                    GetItemColor(m_objets[objet].getRarete()).b,
                                    (int)(255.0f*alpha)));

        texte.SetFont(moteurGraphique->m_font);

        texte.SetString(m_objets[objet].getNom());
        texte.SetCharacterSize(14*configuration->Resolution.w/800);
        texte.SetY(((position.y-GetViewRect(moteurGraphique->m_camera).Top)/configuration->zoom));
        texte.SetX(((position.x-GetViewRect(moteurGraphique->m_camera).Left)/configuration->zoom));

        moteurGraphique->AjouterTexteNonChevauchable(&texte,12);

        sprite.SetImage(*moteurGraphique->getImage(0));

        sprite.SetY(texte.GetPosition().y);
        sprite.SetX(texte.GetPosition().x-4);
        sprite.SetColor(sf::Color(0,0,0,(int)(224.0f*alpha)));
        sprite.Resize(texte.GetRect().Width +8 , texte.GetRect().Height+4);

        if(eventManager->getEvenement(sf::Key::LAlt,EventKey))
            if(eventManager->getPositionSouris().x>sprite.GetPosition().x
             &&eventManager->getPositionSouris().y>sprite.GetPosition().y
             &&eventManager->getPositionSouris().x<sprite.GetPosition().x + texte.GetRect().Width +8
             &&eventManager->getPositionSouris().y<sprite.GetPosition().y + texte.GetRect().Height +4)
                retour = true,sprite.SetColor(sf::Color(32,32,32,224));

        moteurGraphique->AjouterCommande(&sprite,12,0);

        if(decalage != NULL)
            *decalage = (sprite.GetPosition().y*configuration->zoom + GetViewRect(moteurGraphique->m_camera).Top) - sprite.GetSize().y;
    }
    return (retour);
}

int Decor::AfficherTexteObjets(coordonnee position, int objetPointe)
{
    int retour = -1;
    float decalage = position.y-18;

    for (int z=0;z<(int)m_objets.size();z++)
    {
        if(m_objets[z].m_alpha > 0)
            if(AfficherTexteObjet(coordonnee ((int)position.x, (int)decalage), z, &decalage))
                retour=z;
    }

    return (retour);
}

void Decor::AlphaObjets(int alpha)
{
    for(unsigned i = 0; i < m_objets.size() ; ++i)
        if(m_objets[i].m_alpha < alpha)
            m_objets[i].m_alpha = alpha;
}


void Decor::setNumeroHerbe(int numero)
{
    m_numeroHerbe=numero;
    m_herbe_taille = (rand()% (125 - 75 )) + 75;
    m_herbe_decalage.x = 16 - (rand()%32);
    m_herbe_decalage.y = 0/*16 - (rand()%32)*/;

    int randomIntensite=(int)((float)(rand() % 128) + 128);
    int rougatre=rand() % (32);

    m_herbe_couleur.r=randomIntensite;
    m_herbe_couleur.g=randomIntensite - rougatre;
    m_herbe_couleur.b=randomIntensite - rougatre;
}
void Decor::setMonstre(int monstre)
{
    m_monstre.push_back(monstre);
}

void Decor::delMonstre(int monstre)
{
    for(int i = 0 ; i < (int)m_monstre.size() ; ++i)
        if(m_monstre[i]==monstre)
            m_monstre.erase(m_monstre.begin() + i), i --;
}

void Decor::setEffetGraphique(int monstre)
{
    m_effet.push_back(monstre);
}

void Decor::delEffetGraphique(int monstre)
{
    for(int i = 0 ; i < (int)m_effet.size() ; ++i)
        if(m_effet[i]==monstre)
            m_effet.erase(m_effet.begin() + i), i = -1;
}


void Decor::setProjectile(int monstre)
{
    m_projectile.push_back(monstre);
}

void Decor::delProjectile(int monstre)
{
    for(unsigned i = 0 ; i < m_projectile.size() ; ++i)
        if(m_projectile[i] == monstre)
            m_projectile.erase(m_projectile.begin() + i), i --;
}


void Decor::setEvenement(int evenement, int numero)
{
    if (numero>=0&&numero<(int)m_evenement.size())
        m_evenement[numero]=evenement;
}
void Decor::ajouterEvenement(int evenement)
{
    m_evenement.push_back(evenement);
}
void Decor::setTileset(int tileset)
{
    m_tileset=tileset;
}
void Decor::setTile(int tile)
{
    m_tile=tile;
}
void Decor::setCouche(int couche)
{
    m_couche=couche;
}
void Decor::AjouterObjet(Objet objet)
{
    coordonnee position;

    m_objets.push_back(objet);
    bool continuer = true;
    for(position.y = 0;continuer;++position.y)
        for(position.x = 0;position.x < 2 && continuer;++position.x)
        {
            bool ok = true;
            for(int i = 0;i < (int)m_objets.size() - 1;++i)
                if(m_objets[i].getPosition().x == position.x && m_objets[i].getPosition().y == position.y)
                    ok = false;

            if(ok)
                continuer = false, m_objets.back().setPosition(position.x, position.y);
        }

    m_objets.back().m_alpha = 0;
    m_objets.back().m_hauteur = rand()%64;
    m_objets.back().m_monter = 1;
    m_objets.back().m_rotation = rand() % 360;
}


void Decor::supprimerObjet(int numero)
{
    if (numero>=0&&numero<(int)m_objets.size())
    {
        m_objets.erase(m_objets.begin()+numero);

        int x = 0, y = 0;
        if((int)m_objets.size() == 4)
            for(int i = 0;i < (int)m_objets.size();++i)
            {
                m_objets[i].setPosition(x, y);
                x++;
                if(x>=2)
                    x = 0, y++;
            }
    }
}


int Decor::getTile()
{
    return m_tile;
}
int Decor::getTileset()
{
    return m_tileset;
}
const std::vector<int> &Decor::getEvenement()
{
    return m_evenement;
}
const std::vector<int> &Decor::getMonstre()
{
    return m_monstre;
}
const std::vector<int> & Decor::getProjectile()
{
    return m_projectile;
}
const std::vector<int> & Decor::getEffetGraphique()
{
    return m_effet;
}
int Decor::getCouche()
{
    return m_couche;
}

int Decor::getHerbe()
{
    return m_herbe;
}
int Decor::getNumeroHerbe()
{
    return m_numeroHerbe;
}
int Decor::getTailleHerbe()
{
    return m_herbe_taille;
}
const sf::Color &Decor::getCouleurHerbe()
{
    return m_herbe_couleur;
}
const coordonnee &Decor::getDecalageHerbe()
{
    return m_herbe_decalage;
}


Objet* Decor::getObjet(int numero)
{
    if (numero>=0&&numero<(int)m_objets.size()) return &m_objets[numero];
    return NULL;
}
const std::vector<Objet> &Decor::getObjets()
{
    return m_objets;
}
std::vector<Objet>* Decor::getPointeurObjets()
{
    return &m_objets;
}
int Decor::getNombreObjets()
{
    return m_objets.size();
}
int Decor::getHauteur()
{
    return m_hauteur;
}



