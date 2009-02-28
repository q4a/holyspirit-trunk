#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "constantes.h"

#ifndef LIGHTH
#define LIGHTH

struct Wall
{
    Wall (sf::Vector2f p1,sf::Vector2f p2)
    {
        pt1=p1;
        pt2=p2;
        hauteur=32;
    }
    Wall (sf::Vector2f p1,sf::Vector2f p2,int hauteur)
    {
        pt1=p1;
        pt2=p2;
        hauteur=hauteur;
    }

    // Pt1 et Pt2 sont les deux extr�mit�s du mur
    sf::Vector2f pt1;
    sf::Vector2f pt2;
    int hauteur;

    // Position du mur
    sf::Vector2f position;
};

// Wall_Entity est une variable qui permet de repr�senter dans le programme un mur
struct Wall_Entity
{
    Wall_Entity (int id)
    {
        m_ID=id;
    }

    int ID() { return m_ID; }

    private:

    int m_ID;
};

// Light_Entity est une variable qui permet de repr�senter dans le programme une lumi�re
struct Light_Entity
{
    Light_Entity (){m_Dynamic=false,m_ID=-1;}
    Light_Entity (int id,bool d)
    {
        m_ID=id;
        m_Dynamic=d;
    }

    int ID() { return m_ID; }
    bool Dynamic() { return m_Dynamic; }

    private:

    int m_ID;
    bool m_Dynamic;
};

class Light
{
    public :

    // Constructeur et destructeur
    Light();
    Light(sf::Vector2f position, float intensity, float radius, int quality, sf::Color color);
    ~Light();

    // Afficher la lumi�re
    void Draw(sf::RenderWindow *App, coordonnee dimensionsMap);

    // Calculer la lumi�re
    void Generate(std::vector <Wall> &m_wall);

    // Ajouter un triangle � la lumi�re, en effet, les lumi�res sont compos�e de triangles
    void AddTriangle(sf::Vector2f pt1,sf::Vector2f pt2, int minimum_wall,std::vector <Wall> &m_wall);

    // Tester voir si un point "pt" se trouve dans le triangle [(0;0),"pt1","pt2"]
    bool CollisionWithPoint(sf::Vector2f &pt,sf::Vector2f &pt1,sf::Vector2f &pt2);

    // Tester voir si une ligne ["l1","l1"] traverse le triangle [(0;0),"pt1","pt2"]
    bool CollisionWithLine(sf::Vector2f &l1, sf::Vector2f &l2,sf::Vector2f &pt1,sf::Vector2f &ypt2);

    // Changer diff�rents attributs de la lumi�re
    void SetIntensity(float);
    void SetRadius(float);
    void SetQuality(int);
    void SetColor(sf::Color);
    void SetPosition(sf::Vector2f);


    // Retourner diff�rents attributs de la lumi�re
    float GetIntensity();
    float GetRadius();
    int GetQuality();
    sf::Color GetColor();
    sf::Vector2f GetPosition();

    // Une petite bool pour savoir si la lumi�re est allum�e ou �teinte
    bool m_actif;

    private :
    //Position � l'�cran
    sf::Vector2f m_position;
    //Intensit�, g�re la transparence ( entre 0 et 255 )
    float m_intensity;
    //Rayon de la lumi�re
    float m_radius;
    //Couleur de la lumi�re
    sf::Color m_color;
    //Qualit� de la lumi�re, c'est � dire le nombre de triangles par d�faut qui la compose.
    int m_quality;

    //Tableau dynamique de Shape, ce sont ces shapes de type triangle qui compose la lumi�re
    std::vector <sf::Shape> m_shape;

    std::vector <sf::Vector2f> m_dejaPasse;

    std::vector<sf::Vector2f>::iterator IterDejaPasse;
};

#endif

