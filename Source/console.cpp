#include "console.h"
#include "constantes.h"
#include "Globale.h"
#include <iostream>
#include <fstream>

using namespace std;


void Console::Afficher(sf::RenderWindow* ecran)
{
    int min=m_textes.size()-27+m_defilement;
    if(min<0)
    min=0;
    for(int i=min;i<m_textes.size()+m_defilement;i++)
    {
        sf::String texte;
        texte.SetText(m_textes[i].c_str());
        texte.SetSize(12.f);
        texte.SetTop((i-m_textes.size()+27-m_defilement)*15+60);

        if(m_erreur[i])
            texte.SetColor(sf::Color(255,0,0,255));

        ecran->Draw(texte);

    }
}
void Console::Ajouter(std::string texte,bool erreur)
{
    m_textes.push_back(texte);
    m_erreur.push_back(erreur);
}

void Console::AjouterNombre(int nombre,bool erreur)
{
    char temp[10];
    sprintf(temp,"%ld",nombre);
    m_textes.push_back(temp);
    m_erreur.push_back(erreur);
}

int Console::getTaille(){return m_textes.size();}

void Console::defiler(bool direction)
{
    if(direction==0)
    {
        m_defilement--;
        if(m_defilement<-m_textes.size())
            m_defilement=-m_textes.size()+1;
    }
    if(direction==1)
    {
        m_defilement++;
        if(m_defilement>0)
            m_defilement=0;
    }
}

void Console::Rapport()
{
    ofstream fichier("Log.txt", ios::out | ios::trunc);
    if(fichier)  // si l'ouverture a r�ussi
    {
        for(int i=0;i<m_erreur.size();i++)
            if(m_erreur[i]==1)
                fichier<<m_textes[i]<<'\n';
        fichier.close();
    }
}

