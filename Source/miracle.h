
#include "constantes.h"
#include "tile.h"
#include "light.h"

#ifndef MIRACLEH
#define MIRACLEH

enum  {PROJECTILE,CORPS_A_CORPS,DEGATS,EFFET_GRAPHIQUE,INVOCATION};

class Projectile
{
    public:
    void Afficher(sf::RenderWindow* ecran,sf::View *camera,coordonnee position,coordonnee dimensionsMap);
    void Deplacer(float temp,int);

    bool m_monstre,m_actif;
    int m_degats;
    coordonneeDecimal m_position,m_vecteur;
    coordonnee m_positionCase;
    float m_rotation,m_rotationReelle;

    int m_image;
    coordonnee m_positionImage;
    coordonnee m_centre;

    Lumiere m_lumiere;

    float m_animation;
    int m_couche;

    bool m_dejaDeplace;

    Light_Entity m_light;
};

class EffetGraphique
{
    public:
    void Afficher(sf::RenderWindow* ecran,sf::View *camera,coordonnee position,coordonnee dimensionsMap);

    bool m_actif;
    coordonneeDecimal m_position;

    int m_image;
    int m_compteur;
    coordonnee m_positionImage;

    Lumiere m_lumiere;

    float m_animation;
    int m_couche;

    Light_Entity m_light;
};

struct Effet
{
    Effet()
    {
        m_type=0;
        m_sequence=0;

        m_informations[0]=0;
        m_informations[1]=0;
        m_informations[2]=0;
    }

    ~Effet()
    {
        m_lien.clear();
    }

    std::vector <int> m_lien;
    int m_type;
    int m_sequence;

    int m_informations[3];
    std::string m_chaine;
};

class Miracle
{
    public:
    Miracle();
    Miracle(std::string chemin);
    ~Miracle();
    void Charger(std::string chemin);
    void jouerSon(int numeroSon,coordonnee position,coordonnee positionHero);

    std::vector < std::vector <Tile> > m_tile;
    std::vector <Effet> m_effets;


    std::vector <int> m_image;
	std::vector <int> m_sons;
	std::string m_chemin;

    int m_coutFoi;
};

struct infosEntiteMiracle
{
    infosEntiteMiracle()
    {
        m_effetEnCours=0;
        m_imageEnCours=0;
        m_IDObjet=-1;

        m_position.x=0;
        m_position.y=0;
        m_position.w=0;
        m_position.h=0;
    }

    int m_effetEnCours;
    int m_imageEnCours;
    int m_IDObjet;

    coordonneeDecimal m_position;
};

class EntiteMiracle
{
    public:
    std::vector<infosEntiteMiracle> m_infos;
    int m_modele;
};

#endif
