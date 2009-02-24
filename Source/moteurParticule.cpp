
#include "moteurParticule.h"
#include "globale.h"
#include <fstream>



ParticuleSysteme::ParticuleSysteme()
{

}
ParticuleSysteme::ParticuleSysteme(int modeleInt, ModeleParticuleSysteme *modele, coordonnee position, sf::Color color,float force,float angle)
{
     m_modele=modeleInt;
     Generer(force,modele,position,angle);
     m_color=color;
     m_son=modele->m_son;
}

ParticuleSysteme::~ParticuleSysteme()
{
    m_particules.clear();
}
void ParticuleSysteme::Afficher(sf::RenderWindow *ecran, ModeleParticuleSysteme *modele)
{
    for(int i=0;i<(int)m_particules.size();i++)
    {
        sf::Sprite sprite;
        if(m_particules[i].numero>=0&&m_particules[i].numero<(int)modele->m_particules.size())
        {
            sprite.SetImage(*moteurGraphique->getImage(modele->m_image));
            sprite.SetSubRect(sf::IntRect(modele->m_particules[m_particules[i].numero].positionImage.x,modele->m_particules[m_particules[i].numero].positionImage.y,modele->m_particules[m_particules[i].numero].positionImage.x+modele->m_particules[m_particules[i].numero].positionImage.w,modele->m_particules[m_particules[i].numero].positionImage.y+modele->m_particules[m_particules[i].numero].positionImage.h));
            sprite.SetCenter(modele->m_particules[m_particules[i].numero].positionImage.w/2,modele->m_particules[m_particules[i].numero].positionImage.h/2);
            sprite.SetRotation(m_particules[i].rotation);
            float scale=m_particules[i].position.z/200+1;
            sprite.SetScale(scale,scale);
            sprite.SetX(m_particules[i].position.x);
            sprite.SetY(m_particules[i].position.y-m_particules[i].position.z);
            if(configuration->Lumiere!=0)
                sprite.SetColor(sf::Color(m_particules[i].color.r,m_particules[i].color.g,m_particules[i].color.b,(int)m_particules[i].alpha));
            else
                sprite.SetColor(sf::Color(255,255,255,(int)m_particules[i].alpha));
            if(m_particules[i].position.z>32)
                moteurGraphique->AjouterCommande(&sprite,11,1);
            else
                moteurGraphique->AjouterCommande(&sprite,10,1);
        }
    }
}
void ParticuleSysteme::Generer(float force, ModeleParticuleSysteme *modele,coordonnee position,float angle)
{
    for(int i=0;i<(int)modele->m_particules.size();i++)
    {
        int nombre=(rand() % (modele->m_particules[i].max - modele->m_particules[i].min + 1)) + modele->m_particules[i].min;
        for(int j=0;j<nombre;j++)
        {
            angle+= (180-(rand() % 360))*force/100;
            m_particules.push_back(Particule ());
            m_particules.back().vie=100;
            m_particules.back().position.x=position.x;
            m_particules.back().position.y=position.y;
            m_particules.back().position.z=(rand() % (64 - 0 + 1)) + 0;
            m_particules.back().vecteur.x=cos(angle*M_PI/180);
            m_particules.back().vecteur.y=sin(angle*M_PI/180)/2;
            m_particules.back().vecteur.z=(rand() % (int)(force*0.6 - force*0.4 + 1)) + force*0.4;
            m_particules.back().vitesse=(rand() % (int)(force*1.25 - force*0.75 + 1)) + force*0.75;
            m_particules.back().vitesse_rotation=((rand() % (int)(force*1 - force*0.5 + 1)) + force*0.5)*5;
            if(m_particules.back().vitesse_rotation>100)
                m_particules.back().vitesse_rotation=100;
            m_particules.back().rotation=rand() % 360;
            m_particules.back().alpha=255;
            m_particules.back().numero=i;
            m_particules.back().color=sf::Color(255,255,255);
        }
    }
}
bool ParticuleSysteme::Gerer(float temps,int tailleMapY)
{
    bool efface=false;
    int i=0;
    for(Iter=m_particules.begin();Iter!=m_particules.end();++Iter,++i)
    {
        if(Iter->vie==100)
        {
            Iter->position.x+=Iter->vecteur.x*Iter->vitesse*temps*25;
            Iter->position.y+=Iter->vecteur.y*Iter->vitesse*temps*25;
            Iter->position.z+=Iter->vecteur.z*temps*25;

            Iter->vecteur.z-=temps*25;

            if(Iter->position.z<=1&&Iter->vie==100&&i%5==0)
            {
                coordonnee positionHero={0,0,0,0},position,positionCase;

                if((float)((Iter->position.y*2-Iter->position.x)/2)/64+tailleMapY/2<(float)tailleMapY/2)
                    positionCase.y=(int)((Iter->position.y*2-Iter->position.x)/2)/64+tailleMapY/2-1;
                else
                    positionCase.y=(int)((Iter->position.y*2-Iter->position.x)/2)/64+tailleMapY/2;

                positionCase.x=(int)(Iter->position.x+((Iter->position.y*2-Iter->position.x)/2))/64-tailleMapY/2;

                position.x=-(positionCase.x-positionCase.y-1+tailleMapY)/5;
                position.y=(positionCase.x+positionCase.y)/5;

                if((int)(Iter->vecteur.z*100)>10)
                    moteurSons->JouerSon(m_son,position,positionHero,0,(int)(Iter->vecteur.z*100));
            }

            if(Iter->position.z<0)
                Iter->position.z=0,Iter->vecteur.z=fabs(Iter->vecteur.z)/4;

            Iter->vitesse-=temps*10;
            Iter->vitesse_rotation-=temps*50;
            if(Iter->vitesse_rotation<0)
                Iter->vitesse_rotation=0;

             Iter->rotation+=Iter->vitesse_rotation*temps*10;
        }

        if(Iter->vitesse<=0)
            Iter->vitesse=0,Iter->vie-=temps*20;
        if(Iter->vie<=0)
             Iter->alpha-=temps*100;
        if(Iter->alpha<=1)
            efface=true;
    }

    if(efface)
        return 0;

    return 1;
}
