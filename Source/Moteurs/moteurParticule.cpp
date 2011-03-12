

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



#include "moteurParticule.h"
#include "../globale.h"
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

    m_son_joue = false;
}

ParticuleSysteme::~ParticuleSysteme()
{
    m_particules.clear();
}
bool ParticuleSysteme::Afficher( ModeleParticuleSysteme *modele,float temps)
{
    int efface=0;
    int i=0;

    for (Iter=m_particules.begin();Iter!=m_particules.end();++Iter,++i)
    {
        if(Iter->alpha > 0)
        {
            if (!modele->m_particules.empty())
                if (Iter->numero>=0&&Iter->numero<(int)modele->m_particules.size())
                {
                    sf::Sprite sprite;
                    sprite.SetImage(*moteurGraphique->getImage(modele->m_image));
                    sprite.SetSubRect(sf::IntRect(modele->m_particules[Iter->numero].positionImage.x,
                                                  modele->m_particules[Iter->numero].positionImage.y,
                                                  modele->m_particules[Iter->numero].positionImage.w,
                                                  modele->m_particules[Iter->numero].positionImage.h));
                    sprite.SetOrigin(modele->m_particules[Iter->numero].positionImage.w/2,
                                     modele->m_particules[Iter->numero].positionImage.h/2);
                    sprite.SetRotation(Iter->rotation);
                    float scale=Iter->position.z/200+1;
                    sprite.SetScale(scale,scale);
                    sprite.SetX(Iter->position.x);
                    sprite.SetY(Iter->position.y-Iter->position.z);
                    if(!Iter->sang)
                        sprite.SetColor(sf::Color(m_color.r,m_color.g,m_color.b,(int)Iter->alpha));
                    else
                        sprite.SetColor(sf::Color(255,255,255,(int)Iter->alpha));

                    sprite.Scale(Iter->taille, Iter->taille);

                    if (Iter->position.z>32)
                        moteurGraphique->AjouterCommande(&sprite,11,1);
                    else
                        moteurGraphique->AjouterCommande(&sprite,8,1);

                    sprite.Move(0,Iter->position.z);
                    moteurGraphique->AjouterCommande(&sprite,9,1);

                    if(sprite.GetPosition().x + sprite.GetSize().x < GetViewRect(moteurGraphique->m_camera).Left
                    || sprite.GetPosition().x > GetViewRect(moteurGraphique->m_camera).Left + GetViewRect(moteurGraphique->m_camera).Width
                    || sprite.GetPosition().y + sprite.GetSize().y < GetViewRect(moteurGraphique->m_camera).Top
                    || sprite.GetPosition().y > GetViewRect(moteurGraphique->m_camera).Top + GetViewRect(moteurGraphique->m_camera).Height)
                        Iter->vie = 0, Iter->alpha = 0;
                }

            if (Iter->vie==100)
            {
                Iter->position.x+=Iter->vecteur.x*Iter->vitesse*temps*25;
                Iter->position.y+=Iter->vecteur.y*Iter->vitesse*temps*25;
                Iter->position.z+=Iter->vecteur.z*temps*25;

                Iter->vecteur.z-=temps*25;

                if (Iter->position.z<=1&&((i-1)%5==0))
                {
                    coordonnee position;

                    position.x = -(int)(Iter->position.x/64/5);
                    position.y =  (int)(Iter->position.y/32/5);

                    int nbr = 0;
                    int random = -1;

                    if(!m_son.empty())
                    random = rand()%m_son.size();

                    if(random != -1)
                    if (((int)fabs(Iter->vecteur.z*3)>10 && !m_son[random].unique) || m_son[random].unique && (int)fabs(Iter->vecteur.z*3)>10 && !m_son_joue)
                    {
                        while(!moteurSons->JouerSon(m_son[random++].no,position, m_son[random].unique) && nbr++ < m_son.size())
                        {
                            if(random >= m_son.size())
                                random = 0;
                        }

                        m_son_joue = true;
                    }
                       // moteurSons->JouerSon(m_son[random].no,position,m_son[random].unique);
                }

                if (Iter->position.z<0)
                {
                    Iter->position.z=0;
                    Iter->vecteur.z=fabs(Iter->vecteur.z)/20 * modele->m_particules[Iter->numero].rebond;
                }


                Iter->vitesse-=temps*10;
                Iter->vitesse_rotation-=temps*50;
                if (Iter->vitesse_rotation<0)
                    Iter->vitesse_rotation=0;

                Iter->rotation+=Iter->vitesse_rotation*temps*10;
            }

            if(Iter->position.z < 4)
            {
                Iter->vitesse -= temps*modele->m_particules[Iter->numero].frottement;
                Iter->taille += temps*10;

                if(Iter->taille > 1)
                    Iter->taille = 1;
            }

            if (Iter->vitesse<=0)
                Iter->vitesse=0;
            if (Iter->vitesse ==0 && Iter->position.z < 4)
                Iter->vie-=temps*5;
            if (Iter->vie<=0)
                Iter->alpha-=temps*100;
        }
        else
            Iter->alpha=0,efface++;
    }

    if (efface==(int)m_particules.size())
        return 0;

    return 1;
}
void ParticuleSysteme::Envoler(sf::Vector2f pos,int force, int type)
{
    sf::Vector2f position;

    position.x = ((pos.x - pos.y) * 64 / COTE_TILE);
    position.y = ((pos.x + pos.y) * 32 / COTE_TILE);


    for (Iter=m_particules.begin();Iter!=m_particules.end();++Iter)
    if(!Iter->sang)
    {
        float distance =  sqrt(   fabs(position.x - Iter->position.x) * fabs(position.x - Iter->position.x)
                                + fabs(position.y - Iter->position.y) * fabs(position.y - Iter->position.y));

        if(distance < 16 * force)
        if(Iter->position.z  < 64)
        {
            if(type == E_SOUFFLE)
            {
                if(Iter->vecteur.z < force * 0.5)
                {
                    Iter->vecteur.z = force * 0.5;
                    if(Iter->position.z < 4)
                        Iter->position.z = 4;
                }

                if(Iter->vitesse < force)
                {
                    float m = atan2((Iter->position.y - position.y) * 2, (Iter->position.x - position.x));

                    Iter->vecteur.x = cos(m);
                    Iter->vecteur.y = sin(m) * 0.5;

                    Iter->vitesse = force;
                }

                Iter->vie               = 100;
            }
            else if(type == E_TORNADE)
            {
                Iter->vecteur.z = (force * 32 / distance);

                if(Iter->position.z > 32)
                    Iter->vecteur.z = 0;

                Iter->vitesse = force * 256 / distance;

                if(Iter->position.z < 4)
                    Iter->position.z = 4;

                float m = atan2((Iter->position.y - position.y)
                           * 2, (Iter->position.x - position.x));

                Iter->vecteur.x = cos(m - M_PI_2);
                Iter->vecteur.y = sin(m - M_PI_2) * 0.5;

                Iter->vecteur.x -= 6 * cos(m) / Iter->position.z;
                Iter->vecteur.y -= 3 * sin(m) / Iter->position.z;

                Iter->vie               = 100;
            }
            else if(type == E_VERTICAL)
            {
                if(Iter->vecteur.z < force * 32 / distance)
                    Iter->vecteur.z = (force * 32 / distance);
                if(Iter->position.z > 32)
                    Iter->vecteur.z = 0;

                if(Iter->position.z < 4)
                    Iter->position.z = 4;

                Iter->vie               = 100;
            }
        }
    }
}


void ParticuleSysteme::Generer(float force, ModeleParticuleSysteme *modele,coordonnee position,float angle)
{
    for (int i=0;i<(int)modele->m_particules.size();i++)
    {
        int nombre=(rand() % (modele->m_particules[i].max - modele->m_particules[i].min + 1)) + modele->m_particules[i].min;
        for (int j=0;j<nombre;j++)
        {
            angle+= (180-(rand() % 360))*force/100;
            m_particules.push_back(Particule ());
            m_particules.back().numero=i;
            m_particules.back().vie=100;
            m_particules.back().position.x=position.x;
            m_particules.back().position.y=position.y;
            m_particules.back().position.z=((rand() % (64 - 0 + 1)) + 0)/ (modele->m_particules[i].poids);
            m_particules.back().vecteur.x=cos(angle*M_PI/180);
            m_particules.back().vecteur.y=sin(angle*M_PI/180)/2;
            m_particules.back().vecteur.z=((rand() % (int)(force*0.6 - force*0.4 + 1)) + force*0.4) / (modele->m_particules[i].poids);
            m_particules.back().vitesse=((rand() % (int)(force*1.25 - force*0.75 + 1)) + force*0.75) /*/ (modele->m_particules[i].poids)*/;
            m_particules.back().vitesse_rotation=(((rand() % (int)(force*1 - force*0.5 + 1)) + force*0.5)*5) / (modele->m_particules[i].poids) * (modele->m_particules[i].rotation);
            if (m_particules.back().vitesse_rotation>100)
                m_particules.back().vitesse_rotation=100;
            m_particules.back().rotation=rand() % 360;
            m_particules.back().alpha=255;

            if(modele->m_particules[i].sang)
                m_particules.back().taille = 0.1;
            else
                m_particules.back().taille = 1;

            m_particules.back().sang = modele->m_particules[i].sang;
            m_particules.back().poids = modele->m_particules[i].poids;
            m_particules.back().seed = rand()%100;
        }
    }
}

