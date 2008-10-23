#ifndef HEROCPP
#define HEROCPP

#include "Hero.h"
#include "Globale.h"

#include <iostream>
#include <fstream>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <math.h>
#include <dirent.h>

using namespace std;
using namespace sf;

int crypter(int);
int decrypter(int);


Hero::Hero()
{
   ///Je donnes des valeur � mes variables juste pour les tests
	m_personnage.setEtat(ARRET);

	m_chercherSac.x=-1;
	m_chercherSac.y=-1;

	coordonnee position;
	position.x=-10000;
	position.y=-10000;

	m_personnage.setCoordonnee(position);

	Lumiere lumiere;
	lumiere.intensite=160;
	lumiere.rouge=255;
	lumiere.vert=255;
	lumiere.bleu=255;
	lumiere.hauteur=20;

	m_modelePersonnage.setPorteeLumineuse(lumiere);

	Caracteristique temp;

	temp.vie=100;
	temp.maxVie=100;
	temp.degatsMin=1;
	temp.degatsMax=3;
	temp.vitesse=1;
	temp.pointAme=0;
	temp.ancienPointAme=0;
	temp.positionAncienAme=0;
	temp.niveau=1;
	temp.nom="H�ro";

    temp.modificateurTaille=1;

	m_personnage.setCaracteristique(temp);

	m_monstreVise=-1;

	for(int i=0;i<5;i++)
        for(int j=0;j<8;j++)
            m_caseInventaire[i][j]=0;

    m_objetEnMain=-1;
}

void Hero::Sauvegarder()
{
    ofstream fichier;
    fichier.open((configuration.chemin_saves+"hero.sav.hs").c_str(), ios::out);
    if(fichier)
    {
        fichier<<"* nom: "<<m_personnage.getCaracteristique().nom<<"\n";
        fichier<<"* maxVie: "<<crypter(m_personnage.getCaracteristique().maxVie)<<"\n";
        fichier<<"* dgtsMin: "<<crypter(m_personnage.getCaracteristique().degatsMin)<<"\n";
        fichier<<"* dgtsMax: "<<crypter(m_personnage.getCaracteristique().degatsMax)<<"\n";
        fichier<<"* vitesse: "<<crypter((int)m_personnage.getCaracteristique().vitesse*1000)<<"\n";
        fichier<<"* ptAme: "<<crypter(m_personnage.getCaracteristique().pointAme)<<"\n";
        fichier<<"* niveau: "<<crypter(m_personnage.getCaracteristique().niveau)<<"\n";

    }
    fichier.close();
}
void Hero::Charger()
{

    struct dirent *lecture;

	DIR *repertoire;
    repertoire = opendir(configuration.chemin_saves.c_str());
    while ((lecture = readdir(repertoire)))
    {
        string temp="hero.sav.hs";
        if(lecture->d_name==temp)
        {
            ifstream fichier;
            fichier.open((configuration.chemin_saves+"hero.sav.hs").c_str(), ios::in);
            if(fichier)
            {
                char caractere;
                Caracteristique charTemp;
                charTemp=m_personnage.getCaracteristique();
                do
                {
                    fichier.get(caractere);
                    if(caractere=='*')
                    {
                        int temp2;
                        string temp;
                        fichier>>temp;
                            if(temp == "nom:")  fichier>>charTemp.nom;
                            if(temp == "maxVie:")  fichier>>temp2,charTemp.maxVie=decrypter(temp2),charTemp.vie=charTemp.maxVie;
                            if(temp == "dgtsMin:")  fichier>>temp2,charTemp.degatsMin=decrypter(temp2);
                            if(temp == "dgtsMax:")  fichier>>temp2,charTemp.degatsMax=decrypter(temp2);
                            if(temp == "vitesse:")  fichier>>temp2,charTemp.vitesse=decrypter(temp2)/1000;
                            if(temp == "ptAme:")  fichier>>temp2,charTemp.pointAme=decrypter(temp2),charTemp.ancienPointAme=charTemp.pointAme,charTemp.positionAncienAme=charTemp.pointAme;
                            if(temp == "niveau:")  fichier>>temp2,charTemp.niveau=decrypter(temp2);
                    }
                }while(!fichier.eof());
                m_personnage.setCaracteristique(charTemp);
            }
            fichier.close();
        }
    }
    closedir(repertoire);

}

void Hero::afficherInventaire(sf::RenderWindow *ecran,coordonnee positionSouris)
{

    for(int i=0;i<m_inventaire.size();i++)
    {
         sf::Sprite sprite;

        sprite.SetImage(*moteurGraphique.getImage(0));
        if(m_inventaire[i].getRarete()==NORMAL)
            sprite.SetColor(sf::Color(224,224,224,128));
        if(m_inventaire[i].getRarete()==BONNEFACTURE)
            sprite.SetColor(sf::Color(128,0,128,128));
        if(m_inventaire[i].getRarete()==BENI)
            sprite.SetColor(sf::Color(0,64,128,128));
        if(m_inventaire[i].getRarete()==SACRE)
            sprite.SetColor(sf::Color(255,255,128,128));
        if(m_inventaire[i].getRarete()==SANCTIFIE)
            sprite.SetColor(sf::Color(128,255,255,128));
        if(m_inventaire[i].getRarete()==DIVIN)
            sprite.SetColor(sf::Color(255,164,32,128));
        if(m_inventaire[i].getRarete()==INFERNAL)
            sprite.SetColor(sf::Color(224,0,0,128));
        if(m_inventaire[i].getRarete()==CRAFT)
            sprite.SetColor(sf::Color(128,64,0,128));

        sprite.Resize(m_inventaire[i].getTaille().x*32*configuration.Resolution.x/800,m_inventaire[i].getTaille().y*32*configuration.Resolution.y/600);
        sprite.SetX((m_inventaire[i].getPosition().x*32+477)*configuration.Resolution.x/800);
        sprite.SetY(((m_inventaire[i].getPosition().y-1)*32+399)*configuration.Resolution.y/600);

        moteurGraphique.AjouterCommande(&sprite,0);

        sprite.SetColor(sf::Color(255,255,255,255));

        sprite.SetImage(*moteurGraphique.getImage(m_inventaire[i].getImage()));
        sprite.SetSubRect(IntRect(m_inventaire[i].getPositionImage().x, m_inventaire[i].getPositionImage().y, m_inventaire[i].getPositionImage().x+m_inventaire[i].getPositionImage().w, m_inventaire[i].getPositionImage().y+m_inventaire[i].getPositionImage().h));
        sprite.Resize(m_inventaire[i].getTaille().x*32*configuration.Resolution.x/800,m_inventaire[i].getTaille().y*32*configuration.Resolution.y/600);
        sprite.SetX((m_inventaire[i].getPosition().x*32+477)*configuration.Resolution.x/800);
        sprite.SetY(((m_inventaire[i].getPosition().y-1)*32+399)*configuration.Resolution.y/600);

        moteurGraphique.AjouterCommande(&sprite,0);
    }

    if(m_objetEnMain>=0&&m_objetEnMain<m_inventaire.size())
    {
        sf::Sprite sprite;
        sprite.SetImage(*moteurGraphique.getImage(m_inventaire[m_objetEnMain].getImage()));
        sprite.SetSubRect(IntRect(m_inventaire[m_objetEnMain].getPositionImage().x, m_inventaire[m_objetEnMain].getPositionImage().y, m_inventaire[m_objetEnMain].getPositionImage().x+m_inventaire[m_objetEnMain].getPositionImage().w, m_inventaire[m_objetEnMain].getPositionImage().y+m_inventaire[m_objetEnMain].getPositionImage().h));
        sprite.Resize(m_inventaire[m_objetEnMain].getTaille().x*32*configuration.Resolution.x/800,m_inventaire[m_objetEnMain].getTaille().y*32*configuration.Resolution.y/600);
        sprite.SetX(positionSouris.x);
        sprite.SetY(positionSouris.y);

        moteurGraphique.AjouterCommande(&sprite,0);
    }

}

void Hero::placerCamera(sf::View *camera,coordonnee dimensionsMap)
{
	m_positionAffichage.y=(((m_personnage.getCoordonneePixel().x+m_personnage.getCoordonneePixel().y)*64/COTE_TILE)/2);
	m_positionAffichage.x=(((m_personnage.getCoordonneePixel().x-m_personnage.getCoordonneePixel().y)*64/COTE_TILE+dimensionsMap.y*64)-64);

	coordonneeDecimal positionCamera;
	positionCamera.y=m_positionAffichage.y-250*configuration.Resolution.y/600;
	positionCamera.x=m_positionAffichage.x-((400*configuration.Resolution.x/800))+64;

	camera->SetFromRect(sf::FloatRect(positionCamera.x,positionCamera.y,positionCamera.x+configuration.Resolution.x,positionCamera.y+configuration.Resolution.y));
}

void Hero::testMontreVise(Monstre *monstre,int hauteurMap)
{
    if(m_monstreVise>-1&&m_personnage.getCaracteristique().vie>0)
    {
        if(monstre->getCaracteristique().vitesse>0&&fabs(m_personnage.getCoordonnee().x-monstre->getProchaineCase().x)>1||monstre->getCaracteristique().vitesse>0&&fabs(m_personnage.getCoordonnee().y-monstre->getProchaineCase().y)>1
        ||monstre->getCaracteristique().vitesse<=0&&fabs(m_personnage.getCoordonnee().x-monstre->getCoordonnee().x)>1||monstre->getCaracteristique().vitesse<=0&&fabs(m_personnage.getCoordonnee().y-monstre->getCoordonnee().y)>1)
        {
                m_personnage.setArrivee(monstre->getProchaineCase());
        }
        else if(m_personnage.getArrivee().x==m_personnage.getCoordonnee().x&&m_personnage.getArrivee().x==m_personnage.getCoordonnee().x)
        {
            m_personnage.setArrivee(m_personnage.getProchaineCase());
            coordonnee temp,temp2;
            temp.x=(m_personnage.getCoordonneePixel().x-m_personnage.getCoordonneePixel().y-1+hauteurMap)*64;
            temp.y=(m_personnage.getCoordonneePixel().x+m_personnage.getCoordonneePixel().y)*32;
            temp2.x=(monstre->getCoordonneePixel().x-monstre->getCoordonneePixel().y-1+hauteurMap)*64;
            temp2.y=(monstre->getCoordonneePixel().x+monstre->getCoordonneePixel().y)*32;
            m_personnage.frappe(temp2,temp);
        }
    }
}

void Hero::augmenterAme(float temps)
{
    Caracteristique temp = m_personnage.getCaracteristique();

    temp.ancienPointAme+=(temp.pointAme-temp.positionAncienAme)*temps*0.7;

    if(temp.ancienPointAme>=temp.pointAme)
        temp.ancienPointAme=temp.pointAme,temp.positionAncienAme=temp.ancienPointAme;

    if(temp.ancienPointAme>=CALCUL_PA_PROCHAIN_NIVEAU)
    {
        temp.niveau++;
        temp.maxVie+=25;
        temp.vie=temp.maxVie;
        temp.degatsMax++;
        temp.degatsMin++;
    }

    m_personnage.setCaracteristique(temp);
}

bool Hero::ajouterObjet(Objet objet)
{
    bool ramasser=false;
    for(int i=0;i<5;i++)
        for(int j=0;j<8;j++)
        {
            if(!m_caseInventaire[i][j])
            {
                bool ok=true;
                for(int x=j;x<j+objet.getTaille().x;x++)
                    for(int y=i;y<i+objet.getTaille().y;y++)
                        if(x<8&&y<5)
                        {
                            if(m_caseInventaire[y][x])
                                ok=false;
                        }
                        else
                        {
                            ok=false;
                        }

                if(ok)
                {
                    objet.setPosition(j,i);
                    m_inventaire.push_back(objet);

                    for(int x=j;x<j+objet.getTaille().x;x++)
                        for(int y=i;y<i+objet.getTaille().y;y++)
                            m_caseInventaire[y][x]=1;

                    ramasser=true;

                    i=5,j=8;
                }
            }
        }

    return ramasser;
}

void Hero::prendreEnMain(coordonnee positionSouris)
{

    if(positionSouris.x>477&&positionSouris.x<477+32*8&&positionSouris.y>367&&positionSouris.y<399+32*5)
    {
        m_objetEnMain=-1;
        for(int i=0;i<m_inventaire.size();i++)
            if(positionSouris.x>m_inventaire[i].getPosition().x*32+477&&positionSouris.x<m_inventaire[i].getPosition().x*32+477+m_inventaire[i].getTaille().x*32
             &&positionSouris.y>m_inventaire[i].getPosition().y*32+367&&positionSouris.y<m_inventaire[i].getPosition().y*32+367+m_inventaire[i].getTaille().y*32)
                m_objetEnMain=i;
    }
    else if(m_objetEnMain>=0&&m_objetEnMain<m_inventaire.size())
    {
        for(int x=0;x<m_inventaire[m_objetEnMain].getTaille().x;x++)
            for(int y=0;y<m_inventaire[m_objetEnMain].getTaille().y;y++)
                m_caseInventaire[y+m_inventaire[m_objetEnMain].getPosition().y][x+m_inventaire[m_objetEnMain].getPosition().x]=0;
        m_inventaire.erase(m_inventaire.begin()+m_objetEnMain);

        m_objetEnMain=-1;
    }

}

void Hero::setMonstreVise(int monstre){m_monstreVise=monstre;}
void Hero::setChercherSac(coordonnee a){m_chercherSac=a;}

coordonnee Hero::getChercherSac(){return m_chercherSac;};
int Hero::getMonstreVise(){return m_monstreVise;}
#endif
