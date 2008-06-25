
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <math.h>
#include "eventManager.h"
#include "prototypes.h"
#include "Globale.h"
#include <iostream>

using namespace sf;

EventManager::EventManager()
{
	for(int i=0;i<500;i++)
	    m_EventTableau[i]=0;
	for(int i=0;i<5;i++)
	    m_Clic[i]=0;
	for(int i=0;i<5;i++)
	    m_ClicAncien[i]=0;

	m_positionSouris.x=450;
	m_positionSouris.y=300;

	console.Ajouter("",0);
    console.Ajouter("Chargement des curseurs :",0);

	if(!m_curseur.LoadFromFile(configuration.chemin_curseurs+configuration.nom_curseur_base))
         console.Ajouter("Impossible de charger : "+configuration.chemin_curseurs+configuration.nom_curseur_base,1);
    else
        console.Ajouter("Chargement de : "+configuration.chemin_curseurs+configuration.nom_curseur_base,0);

}

void EventManager::GererLesEvenements(RenderWindow *ecran,View *camera,bool *continuer,float temps,coordonnee tailleMap)
{
	Event Event;
	while(ecran->GetEvent(Event))
	{
		switch(Event.Type)
   		{
        	case Event::KeyPressed:
        	    if(Event.Key.Code>=0&&Event.Key.Code<500)
					m_EventTableau[Event.Key.Code] = 1;
            	break;
        	case Event::KeyReleased:
				if(Event.Key.Code>=0&&Event.Key.Code<500)
					m_EventTableau[Event.Key.Code] = 0;
            	break;
            case Event::MouseMoved:
                m_positionSouris.x=Event.MouseMove.X;
                m_positionSouris.y=Event.MouseMove.Y;
                break;
            case Event::MouseButtonPressed:
                m_Clic[Event.MouseButton.Button]=1;
                break;
            case Event::MouseButtonReleased:
                m_Clic[Event.MouseButton.Button]=0;
                break;
    	}
    	if (Event.Type == Event::Closed)
        *continuer = false;
	}

    if(m_EventTableau[Key::Escape])
		*continuer=false;
	if(m_EventTableau[Key::PageUp])
		configuration.zoom*=(1+(0.5*temps));
	if(m_EventTableau[Key::PageDown])
		configuration.zoom*=((1-(0.5*temps)));

    if(configuration.zoom<0.75+((float)configuration.Resolution.x/800-1)/2)
		configuration.zoom=0.75+((float)configuration.Resolution.x/800-1)/2;
	if(configuration.zoom>1.5+((float)configuration.Resolution.x/800-1))
		configuration.zoom=1.5+((float)configuration.Resolution.x/800-1);

    if(m_EventTableau[Key::Add])
		configuration.volume+=temps*50;
    if(m_EventTableau[Key::Subtract])
		configuration.volume-=temps*50;

    if(m_EventTableau[Key::M])
    {
        if(!configuration.Minimap)
            configuration.Minimap=true;
        else
            configuration.Minimap=false;
        m_EventTableau[Key::M]=false;
    }

    if(m_EventTableau[Key::Up])
    {
        console.defiler(0);
        m_EventTableau[Key::Up]=false;
    }
    if(m_EventTableau[Key::Down])
    {
         console.defiler(1);
         m_EventTableau[Key::Down]=false;
    }

    if(m_EventTableau[Key::Space])
        configuration.minute+=200*temps;

    if(m_EventTableau[Key::Tab])
    {
        if(configuration.console)
            configuration.console=false;
        else
            configuration.console=true;
        m_EventTableau[Key::Tab]=false;
    }

	ecran->SetView(*camera);
	FloatRect ViewRect = camera->GetRect();

	coordonnee positionSourisTotale;
	positionSourisTotale.x=(int)ecran->ConvertCoords(ecran->GetInput().GetMouseX(),ecran->GetInput().GetMouseY()).x;
    positionSourisTotale.y=(int)ecran->ConvertCoords(ecran->GetInput().GetMouseX(), ecran->GetInput().GetMouseY()).y;


    //Conversion des coord cartÚsienne en coord iso
    if((float)((positionSourisTotale.y*2-positionSourisTotale.x)/2)/64+tailleMap.y/2<(float)tailleMap.y/2)
        m_casePointee.y=((positionSourisTotale.y*2-positionSourisTotale.x)/2)/64+tailleMap.y/2-1;
    else
        m_casePointee.y=((positionSourisTotale.y*2-positionSourisTotale.x)/2)/64+tailleMap.y/2;

    m_casePointee.x=(positionSourisTotale.x+((positionSourisTotale.y*2-positionSourisTotale.x)/2))/64-tailleMap.y/2;

    if(m_casePointee.x<0)
        m_casePointee.x=0;
    if(m_casePointee.y<0)
        m_casePointee.y=0;
    if(m_casePointee.x>tailleMap.x)
        m_casePointee.x=tailleMap.x;
    if(m_casePointee.y>tailleMap.y)
        m_casePointee.y=tailleMap.y;
}

void EventManager::AfficherCurseur(sf::RenderWindow *ecran)
{
	Sprite Sprite;
	Sprite.SetImage(m_curseur);
	Sprite.SetX(m_positionSouris.x);
	Sprite.SetY(m_positionSouris.y);
	ecran->Draw(Sprite);
}

bool EventManager::getEvenement(int numeroEvenement,std::string evenement)
{
	if(evenement=="ET")
	    if(numeroEvenement>=0&&numeroEvenement<500)
	        return m_EventTableau[numeroEvenement];

	if(evenement=="C")
	    if(numeroEvenement>=0&&numeroEvenement<=5)
	        return m_Clic[numeroEvenement];

	if(evenement=="CA")
	    if(numeroEvenement>=0&&numeroEvenement<=5)
	        return m_ClicAncien[numeroEvenement];
}

void EventManager::StopEvenement(int numeroEvenement,std::string evenement)
{
	if(evenement=="ET")
	    if(numeroEvenement>=0&&numeroEvenement<500)
	        m_EventTableau[numeroEvenement]=0;

	if(evenement=="C")
	    if(numeroEvenement>=0&&numeroEvenement<=5)
	        m_Clic[numeroEvenement]=0;

	if(evenement=="CA")
	    if(numeroEvenement>=0&&numeroEvenement<=5)
	        m_ClicAncien[numeroEvenement]=0;
}



coordonnee EventManager::getCasePointee(){return m_casePointee;}
coordonnee EventManager::getPositionSouris(){return m_positionSouris;}

void EventManager::arreterClique(){m_Clic[Mouse::Left]=false;}



