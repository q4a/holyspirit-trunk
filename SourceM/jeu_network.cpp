#include "constantes.h"
#include "globale.h"
#include "jeu.h"

#define NET_PORT 4570

sf::Packet& operator <<(sf::Packet& Packet, const Caracteristique& C)
{
    return Packet << (sf::Int32)C.vie << (sf::Int32)C.maxVie << (sf::Int16)C.niveau;
}
sf::Packet& operator >>(sf::Packet& Packet, Caracteristique& C)
{
    sf::Int32 vie, maxVie;
    sf::Int16 niveau;

    if((Packet >> vie >> maxVie >> niveau))
    {
        C.vie = vie;
        C.maxVie = maxVie;
        C.niveau = niveau;
    }

    return Packet;
}

sf::Packet& operator <<(sf::Packet& Packet, const coordonneeDecimal& C)
{
    return Packet << (sf::Int32)C.x << (sf::Int32)C.y << (sf::Int32)C.h << (sf::Int32)C.w;
}
sf::Packet& operator >>(sf::Packet& Packet, coordonneeDecimal& C)
{
    sf::Int32 x,y,h,w;
    if((Packet >> x >> y >> h >> w))
    {
         C.x = x;
        C.y = y;
        C.h = h;
        C.w = w;
    }

    return Packet;
}

sf::Packet& operator <<(sf::Packet& Packet, const coordonnee& C)
{
    return Packet << (sf::Int32)C.x << (sf::Int32)C.y << (sf::Int32)C.h << (sf::Int32)C.w;
}
sf::Packet& operator >>(sf::Packet& Packet, coordonnee& C)
{
    sf::Int32 x,y,h,w;
    if((Packet >> x >> y >> h >> w))
    {
        C.x = x;
        C.y = y;
        C.h = h;
        C.w = w;
    }

    return Packet;
}


sf::Packet& operator <<(sf::Packet& Packet,  Personnage& C)
{
    return Packet << C.getCaracteristique() << C.getCoordonneePixel() << (sf::Int8)C.getEtat()  << (sf::Int16)C.m_entite_graphique.m_noAnimation << (sf::Int16)C.getAngle();
}
sf::Packet& operator >>(sf::Packet& Packet, Personnage& C)
{
    coordonneeDecimal pos;
    sf::Int8 etat;
    sf::Int16 angle,pose;

    Caracteristique caract = C.getCaracteristique();

    if((Packet >> caract >> pos >> etat >> pose >> angle))
    {
        C.setJustCoordonnee(pos);
        C.setEtat(etat,pose);
        C.setForcedAngle(angle);
        C.setCaracteristique(caract);
    }

    return Packet;
}

int  Jeu::GetNoClient(const sf::IpAddress &address)
{
    int no = 0;
    for (std::list<sf::TcpSocket*>::iterator it = m_clientsTCP.begin(); it != m_clientsTCP.end(); ++it, ++no)
        if((*it)->GetRemoteAddress() == address)
            return no;

    return -1;
}


void GererReseauClientTCP(void* UserData)
{
    sf::Context context;
    Jeu *jeu = static_cast<Jeu*>(UserData);

    while(jeu->m_host)
    {
        sf::Packet packet;
        if (jeu->m_host->Receive(packet) == sf::Socket::Done)
           jeu->CheckPacketClient(packet);
    }
}
void GererReseauClientUDP(void* UserData)
{
    Jeu *jeu = static_cast<Jeu*>(UserData);

    sf::IpAddress ip;
    short unsigned int port;

    while(jeu->m_host)
    {
        sf::Packet packet;
        if(jeu->m_udp.Receive(packet, ip, port) == sf::Socket::Done)
            jeu->CheckPacketClient(packet);
    }
}

void GererReseauHost(void* UserData)
{
    sf::Context context;
    Jeu *jeu = static_cast<Jeu*>(UserData);

    while(jeu->m_runTCPHost)
    {
        if (jeu->m_selector.Wait(500))
        {
            if (jeu->m_selector.IsReady(jeu->m_listener))
            {
                sf::TcpSocket* clientTCP = new sf::TcpSocket;

                if (jeu->m_listener.Accept(*clientTCP) == sf::Socket::Done)
                    jeu->AddClient(clientTCP);
                else
                    delete clientTCP;
            }
            else
            {
                int no = 0;
                for (std::list<sf::TcpSocket*>::iterator it = jeu->m_clientsTCP.begin(); it != jeu->m_clientsTCP.end(); ++it, ++no)
                {
                    sf::TcpSocket& client = **it;
                    if (jeu->m_selector.IsReady(client))
                    {
                        sf::Packet packet;
                        if (client.Receive(packet) == sf::Socket::Done)
                            jeu->CheckPacketHost(packet, no, &client, &client);
                    }
                }

                if (jeu->m_selector.IsReady(jeu->m_udp))
                {
                    sf::IpAddress ip;
                    short unsigned int port;
                    sf::Packet packet;
                    if (jeu->m_udp.Receive(packet,ip,port) == sf::Socket::Done) {
                        jeu->CheckPacketHost(packet, jeu->GetNoClient(ip), &jeu->m_udp, NULL);
                    }
                }
            }
        }
    }
}



void Jeu::GererMultijoueur()
{
   // m_net_send = false;
    if(m_net_clock.GetElapsedTime() > (float)configuration->net_rate)
    {
        m_net_send = true;
        m_net_clock.Reset();

        if(configuration->hote)
        {
            int no = 0;
            for (std::list<sf::TcpSocket*>::iterator it = m_clientsTCP.begin(); it != m_clientsTCP.end(); ++it, ++no)
            {
                sf::TcpSocket& client = **it;

                sf::Packet ping;
                ping<<(sf::Int8)(-1);
                if(client.Send(ping) != sf::TcpSocket::Done)
                {
                    m_selector.Remove(*(*it));
                    m_clientsTCP.erase(it);
                    //delete *&*it;

                    if(!DeletePersonnageClient(no))
                        std::cout<<"Un joueur est partit"<<std::endl;
                    int no2 = 1;
                    for (std::list<sf::TcpSocket*>::iterator it2 = m_clientsTCP.begin(); it2 != m_clientsTCP.end(); ++it2,++no2)
                    {
                        sf::Packet packet2;
                        packet2<<(sf::Int8)P_DELETEPLAYER<<(sf::Int8)(no+(no>no2));
                        sf::TcpSocket& client2 = **it2;
                        client2.Send(packet2);
                    }

                    it = m_clientsTCP.begin();
                    no = 0;
                }
                else
                {
                    sf::Packet packet2;
                    packet2<<(sf::Int8)P_PLAYERCARACT<<(sf::Int8)0<<hero.m_personnage;
                    sf::IpAddress ip = client.GetRemoteAddress();
                    short unsigned int port = NET_PORT;
                    m_udp.Send(packet2, ip, port);
                }
            }
        }
        else if(m_host)
        {
            sf::Packet ping;
            ping<<(sf::Int8)(-1);
            if(!(m_host->Send(ping) == sf::TcpSocket::Done))
            {
                m_mainMenu->m_save  = true;
                m_mainMenu->m_reset = true;
                m_contexte = m_mainMenu;
                std::cout<<"L'h�te � quitt� la partie"<<std::endl;

                Disconnect();
                CloseServer();
            }
            else
            {
                sf::Packet packet;
                //sf::UdpSocket upd;
                packet<<(sf::Int8)P_PLAYERCARACT<<hero.m_personnage;
                sf::IpAddress ip = m_host->GetRemoteAddress();
                short unsigned int port = NET_PORT;
                m_udp.Send(packet, ip, port);
            }
        }
    }
}

void Jeu::LaunchServer()
{
    CloseServer();


    m_udp.Bind(NET_PORT);
    m_listener.Listen(NET_PORT);

    m_selector.Add(m_udp);
    m_selector.Add(m_listener);

    m_runTCPHost = true;
    m_thread_host = new sf::Thread(&GererReseauHost, this);
    m_thread_host->Launch();
}

void Jeu::CloseServer()
{
    m_runTCPHost = false;
    m_listener.Close();
    m_udp.Unbind();

    if(m_thread_host)
    {
        m_thread_host->Wait();
        delete m_thread_host;
    }
    m_thread_host = NULL;

    m_personnageClients.clear();

    for (std::list<sf::TcpSocket*>::iterator it = m_clientsTCP.begin(); it != m_clientsTCP.end(); ++it)
        delete *it;
    m_clientsTCP.clear();

    m_selector.Clear();
}

bool Jeu::Connect(sf::IpAddress ServerAddress)
{
    m_host = new sf::TcpSocket();

    if (m_host->Connect(ServerAddress, NET_PORT, 3000) != sf::TcpSocket::Done)
    {
        delete m_host;
        m_host = NULL;
        return false;
    }

    m_udp.Bind(NET_PORT);

    m_thread_clientTCP = new sf::Thread(&GererReseauClientTCP, this);
    m_thread_clientTCP->Launch();

    m_thread_clientUDP = new sf::Thread(&GererReseauClientUDP, this);
    m_thread_clientUDP->Launch();

    return true;
}

void Jeu::Disconnect()
{
    std::cout<<"Vous avez ete deconnecte"<<std::endl;

   // GlobalMutex.Lock();
    if(m_host)
    m_host->Disconnect();
    if(m_host)
        delete m_host;
    m_host = NULL;
    m_udp.Unbind();
   // GlobalMutex.Unlock();

    if(m_thread_clientTCP)
    {
        //m_thread_clientTCP->Terminate();
        m_thread_clientTCP->Wait();
        delete m_thread_clientTCP;
    }

    if(m_thread_clientUDP)
    {
        //m_thread_clientUDP->Terminate();
        m_thread_clientUDP->Wait();
        delete m_thread_clientUDP;
    }

    m_thread_clientTCP = NULL;
    m_thread_clientUDP = NULL;
}


void Jeu::AddClient(sf::TcpSocket* clientTCP)
{
    sf::Packet packet;

    m_clientsTCP.push_back(clientTCP);

    std::cout<<"Un joueur arrive !"<<std::endl;

    m_selector.Add(*clientTCP);

    packet<<(sf::Int8)P_NEWPLAYER<<hero.m_caracteristiques.nom<<hero.m_caracteristiques<<hero.m_cheminClasse;
    clientTCP->Send(packet);

    packet.Clear();
    packet<<(sf::Int8)P_PLAYERSKIN<<(sf::Int8)0<<(sf::Int8)hero.m_cas;
    for (int i=0; i<NOMBRE_MORCEAU_PERSONNAGE; ++i)
        packet<<hero.m_cheminModele[i]<<hero.m_pasEquipe[i];
    clientTCP->Send(packet);

    int no2 = 1;
    for (std::list<Hero>::iterator p = m_personnageClients.begin();
            p != m_personnageClients.end(); ++p, ++no2)
    {
        packet.Clear();
        packet<<(sf::Int8)P_NEWPLAYER<<p->m_caracteristiques.nom<<p->m_caracteristiques<<p->m_cheminClasse;
        clientTCP->Send(packet);

        packet.Clear();
        packet<<(sf::Int8)P_PLAYERSKIN<<(sf::Int8)no2<<(sf::Int8)p->m_cas;
        for (int i=0; i<NOMBRE_MORCEAU_PERSONNAGE; ++i)
            packet<<p->m_cheminModele[i]<<p->m_pasEquipe[i];
        clientTCP->Send(packet);
    }
}

void Jeu::CheckPacketHost(sf::Packet &packet, int no, sf::Socket* it, sf::TcpSocket* tcp)
{
    sf::Int8 type;
    if((packet>>type))
    {
        if(type == P_NEWPLAYER)
        {
            Caracteristique caract;
            std::string cheminclasse;
            if((packet>>caract.nom>>caract>>cheminclasse))
            {
                std::cout<<caract.nom<<" s'est connecte a la partie"<<std::endl;
                m_personnageClients.push_back(Hero ());
                m_personnageClients.back().m_caracteristiques = caract;
                m_personnageClients.back().m_personnage.setCaracteristique(caract);
                m_personnageClients.back().m_personnage.m_heroic = true;

                m_personnageClients.back().m_classe.Charger(cheminclasse,std::vector<int> (),caract);
                m_personnageClients.back().ChargerModele();

                hero.m_amis.push_back(&m_personnageClients.back().m_personnage);

                sf::Packet packet2;
                packet2<<(sf::Int8)P_NEWPLAYER<<caract.nom<<caract<<cheminclasse;
                for (std::list<sf::TcpSocket*>::iterator it2 = m_clientsTCP.begin(); it2 != m_clientsTCP.end(); ++it2)
                    if(*it2 != it)
                    {
                        sf::TcpSocket& client2 = **it2;
                        client2.Send(packet2);
                    }

                sf::Packet packet3;
                packet3<<(sf::Int8)P_CHANGEMAP<<map->getNom()<<hero.m_personnage.getCoordonnee();
                tcp->Send(packet3);
            }
        }
        else if(type == P_PLAYERSKIN)
        {
            int i = 0;
            for (std::list<Hero>::iterator p = m_personnageClients.begin();
                    p != m_personnageClients.end() && i <= no; ++p, ++i)
                if(i == no)
                {
                    sf::Int8 cas;
                    if((packet>>cas))
                    {
                       p->m_cas = cas;
                        for (int k=0; k<NOMBRE_MORCEAU_PERSONNAGE; ++k)
                            packet>>p->m_cheminModeleNouveau[k]>>p->m_pasEquipe[k];
                        p->ChargerGraphics();
                    }
                }
        }
        else if(type == P_CHANGEMAP)
        {
            std::string prochaineMap;
            coordonnee coordonneePerso;
            if((packet>>prochaineMap>>coordonneePerso))
            {
                SendChangeMap(prochaineMap, coordonneePerso);

                GlobalMutex.Lock();
                m_chargement->setC_Chargement(prochaineMap,coordonneePerso);
                m_contexte = m_chargement;
                GlobalMutex.Unlock();
            }
        }
        else if(type == P_DEGATS && map && map->m_loaded)
        {
            sf::Int16  no2;
            sf::Int16 degats;
            sf::Int8  typeD;
            sf::Int16 temps;
            if((packet>>no2>>degats>>typeD>>temps))
            {
                int i = 0;
                for (std::list<Hero>::iterator p = m_personnageClients.begin();
                        p != m_personnageClients.end() && i <= no; ++p, ++i)
                    if(i == no)
                        map->InfligerDegats((int)no2,&p->m_personnage,(float)degats,(int)typeD,this,(float)temps);
            }
        }
        else if(type == P_PLAYERCARACT)
        {
            Personnage *perso = NULL;
            int i = 0;
            for (std::list<Hero>::iterator p = m_personnageClients.begin();
                    p != m_personnageClients.end() && i <= no; ++p, ++i)
                if(i == no)
                {
                    packet>>p->m_personnage;
                    perso = &p->m_personnage;

                    p->m_caracteristiques.vie =  p->m_personnage.getCaracteristique().vie;
                }

            int no2 = 0;

            if(perso)
                for (std::list<sf::TcpSocket*>::iterator it2 = m_clientsTCP.begin(); it2 != m_clientsTCP.end(); ++it2,++no2)
                    if(*it2 != it)
                    {
                        sf::TcpSocket& client2 = **it2;
                        sf::Packet packet2;
                        packet2<<(sf::Int8)P_PLAYERCARACT<<(sf::Int8)(no+(no<no2))<<*perso;

                        sf::IpAddress ip2 = client2.GetRemoteAddress();
                        m_udp.Send(packet, ip2, NET_PORT);
                    }
        }
        else if(type == P_MSGCHAT)
        {
            std::string msg;

            GlobalMutex.Lock();

            int i = 0;
            for (std::list<Hero>::iterator p = m_personnageClients.begin();
                    p != m_personnageClients.end() && i <= no; ++p, ++i)
                if(i == no)
                {
                    packet>>msg;
                    p->m_personnage.m_speak = msg;
                    p->m_personnage.m_speak_time = 5000;

                    console->Ajouter(p->m_caracteristiques.nom + " : "+msg);
                }

            int no2 = 0;

            for (std::list<sf::TcpSocket*>::iterator it2 = m_clientsTCP.begin(); it2 != m_clientsTCP.end(); ++it2,++no2)
                if(*it2 != it)
                {
                    sf::TcpSocket& client2 = **it2;
                    sf::Packet packet2;
                    packet2<<(sf::Int8)P_MSGCHAT<<(sf::Int8)(no+(no<no2))<<msg;

                    client2.Send(packet);
                }

            GlobalMutex.Unlock();
        }
    }
}

void Jeu::CheckPacketClient(sf::Packet &packet)
{
    sf::Int8 type;
    if((packet>>type))
    {
        if(type == P_PLAYERCARACT)
        {
            sf::Int8 no;
            if((packet>>no))
            {
                int i = 0;
                for (std::list<Hero>::iterator p = m_personnageClients.begin();
                        p != m_personnageClients.end() && i <= no; ++p, ++i)
                    if(i == no)
                    {
                        packet>>p->m_personnage;
                        p->m_caracteristiques.vie = p->m_personnage.getCaracteristique().vie;
                    }
            }
        }
        else if(type == P_INFOSMONSTRE && map)
        {
            do
            {
                type = (sf::Int8)-1;
                sf::Int16 no;
                if((packet>>no))
                {
                    if(map->m_loaded)
                    {
                        GlobalMutex.Lock();

                        bool enVie = map->getEntiteMonstre(no)->EnVie();
                        int x = (int)((map->getEntiteMonstre(no))->getCoordonneePixel().x/COTE_TILE + 0.5);
                        int y = (int)((map->getEntiteMonstre(no))->getCoordonneePixel().y/COTE_TILE + 0.5);
                        packet>>*(map->getEntiteMonstre(no));
                        map->DeplacerMonstreCase(no,x,y);

                        if(enVie != map->getEntiteMonstre(no)->EnVie())
                            map->KillMonstre(map->getEntiteMonstre(no),0,0,this);

                        GlobalMutex.Unlock();

                        if(!packet.EndOfPacket())
                            if(!(packet>>type))
                                type = (sf::Int8)-1;
                    }
                }
            }while(type == sf::Int8(P_INFOSMONSTRE));
        }
        else if(type == P_NEWPLAYER)
        {
            std::string cheminclasse, nom;
            Caracteristique caract;
            if((packet>>caract.nom>>caract>>cheminclasse))
            {
                m_personnageClients.push_back(Hero ());
                m_personnageClients.back().m_caracteristiques = caract;
                m_personnageClients.back().m_personnage.setCaracteristique(caract);
                m_personnageClients.back().m_personnage.m_heroic = true;

                GlobalMutex.Lock();

                m_personnageClients.back().m_classe.Charger(cheminclasse,std::vector<int> (),caract);
                m_personnageClients.back().ChargerModele();

                hero.m_amis.push_back(&m_personnageClients.back().m_personnage);
                std::cout<<"Joueur " << caract.nom << std::endl;

                GlobalMutex.Unlock();
            }
        }
        else if(type == P_DELETEPLAYER)
        {
            sf::Int8 no;
            if((packet>>no))
                DeletePersonnageClient((int)no);
        }
        else if(type == P_PLAYERSKIN)
        {
            int i = 0;
            sf::Int8 no;

            if((packet>>no))
            for (std::list<Hero>::iterator p = m_personnageClients.begin();
                    p != m_personnageClients.end() && i <= no; ++p, ++i)
                if(i == no)
                {
                    sf::Int8 cas;
                    if((packet>>cas))
                    {
                       p->m_cas = cas;
                        for (int k=0; k<NOMBRE_MORCEAU_PERSONNAGE; ++k)
                            packet>>p->m_cheminModeleNouveau[k]>>p->m_pasEquipe[k];

                        GlobalMutex.Lock();
                        p->ChargerGraphics();
                        GlobalMutex.Unlock();
                    }
                }
        }
        else if(type == P_CHANGEMAP)
        {
            std::string prochaineMap;
            coordonnee coordonneePerso;
            if((packet>>prochaineMap>>coordonneePerso))
            {
               GlobalMutex.Lock();
                m_chargement->setC_Chargement(prochaineMap,coordonneePerso);
                m_contexte = m_chargement;
                GlobalMutex.Unlock();
            }
        }
        else if(type == P_DEGATS)
        {
            sf::Int16 degats;
            sf::Int8  typeD;
            sf::Int16 temps;
            if((packet>>degats>>typeD>>temps))
                hero.m_personnage.InfligerDegats(degats, typeD, temps);
        }
        else if(type == P_KILLMONSTRE && map)
        {
            sf::Int16 no,angle;
            sf::Int16 degats;

            if((packet>>no>>angle>>degats))
            if(map->m_loaded)
                map->KillMonstre(map->getEntiteMonstre(no),angle,degats,this);
        }
        else if(type == P_MSGCHAT)
        {
            int i = 0;
            sf::Int8 no;

            GlobalMutex.Lock();

            if((packet>>no))
            for (std::list<Hero>::iterator p = m_personnageClients.begin();
                    p != m_personnageClients.end() && i <= no; ++p, ++i)
                if(i == no)
                {
                    packet>>p->m_personnage.m_speak;
                    p->m_personnage.m_speak_time = 5000;
                    console->Ajouter(p->m_caracteristiques.nom + " : "+p->m_personnage.m_speak);
                }

            GlobalMutex.Unlock();
        }

    }
}

bool Jeu::DeletePersonnageClient(int no)
{
    int i = 0;
    for (std::list<Hero>::iterator p = m_personnageClients.begin();
            p != m_personnageClients.end() && i <= no; ++p, ++i)
        if(i == no)
        {
            std::cout<<p->m_caracteristiques.nom<<" est partit"<<std::endl;

            GlobalMutex.Lock();
            for(unsigned j = 0 ; j < hero.m_amis.size() ; ++j)
                if(hero.m_amis[j] == &p->m_personnage)
                    hero.m_amis.erase(hero.m_amis.begin() + j);
            m_personnageClients.erase(p);
            GlobalMutex.Unlock();

            return true;
        }
    return false;
}

void Jeu::SendSkin()
{
    if(configuration->multi)
    {
        sf::Packet packet;
        packet<<(sf::Int8)P_PLAYERSKIN;

        if(configuration->hote)
            packet<<(sf::Int8)0;

        packet<<(sf::Int8)hero.m_cas;

        for (int i=0; i<NOMBRE_MORCEAU_PERSONNAGE; ++i)
            packet<<hero.m_cheminModele[i]<<hero.m_pasEquipe[i];

        if(configuration->hote)
        {
            for (std::list<sf::TcpSocket*>::iterator it = m_clientsTCP.begin(); it != m_clientsTCP.end(); ++it)
            {
                sf::TcpSocket& client = **it;
                client.Send(packet);
            }
        }
        else
            m_host->Send(packet);
    }
}

void Jeu::SendChangeMap(const std::string &prochaineMap,const coordonnee &coordonneePerso)
{
    if(configuration->multi)
    {
        sf::Packet packet;
        packet<<(sf::Int8)P_CHANGEMAP<<prochaineMap<<coordonneePerso;

        if(configuration->hote)
        {
            for (std::list<sf::TcpSocket*>::iterator it = m_clientsTCP.begin(); it != m_clientsTCP.end(); ++it)
            {
                sf::TcpSocket& client = **it;
                client.Send(packet);
            }
        }
        else
            m_host->Send(packet);
    }
}

void Jeu::SendInfosMonstre(sf::Packet &packet)
{
    if(configuration->multi && configuration->hote)
    {
        for (std::list<sf::TcpSocket*>::iterator it = m_clientsTCP.begin(); it != m_clientsTCP.end(); ++it)
        {
            sf::TcpSocket& client = **it;
            //client.Send(packet);

           // sf::UdpSocket upd;
            sf::IpAddress ip = client.GetRemoteAddress();
            short unsigned int port = NET_PORT;
            m_udp.Send(packet, ip, port);
        }
    }
}

void Jeu::SendDegats(Hero *hero2, float degats, int type, float temps)
{
    if(configuration->multi && configuration->hote)
    {
        sf::Packet packet;
        packet<<(sf::Int8)P_DEGATS<<(sf::Int16)degats<<(sf::Int8)type<<(sf::Int16)temps;

        int no = 0;
        for (std::list<Hero>::iterator p = m_personnageClients.begin();
                p != m_personnageClients.end() && &*p != hero2; ++p, ++no) {}

        int no2 = 0;
        for (std::list<sf::TcpSocket*>::iterator it = m_clientsTCP.begin(); it != m_clientsTCP.end() && no2 < no; ++it, ++no2)
            if(no == no2 + 1)
            {
                sf::TcpSocket& client = **it;
                client.Send(packet);
            }
    }
}

void Jeu::SendDegats(int no, float degats, int type, float temps)
{
    if(configuration->multi && !configuration->hote)
    {
        sf::Packet packet;
        packet<<(sf::Int8)P_DEGATS<<(sf::Int16)no<<(sf::Int16)degats<<(sf::Int8)type<<(sf::Int16)temps;
      //  m_host->Send(packet);
        m_udp.Send(packet,m_host->GetRemoteAddress(),NET_PORT);
    }
}

void Jeu::SendKillMonstre(int no, int angle, float degats)
{
    if(configuration->multi && configuration->hote)
    {
        sf::Packet packet;
        packet<<(sf::Int8)P_KILLMONSTRE<<(sf::Int16)no<<(sf::Int16)angle<<(sf::Int16)degats;

        for (std::list<sf::TcpSocket*>::iterator it = m_clientsTCP.begin(); it != m_clientsTCP.end(); ++it)
        {
            sf::TcpSocket& client = **it;
           // client.Send(packet);
            m_udp.Send(packet,client.GetRemoteAddress(),NET_PORT);
        }
    }
}

void Jeu::SendMessage(std::string msg)
{
    if(!msg.empty())
    if(configuration->multi)
    {
        sf::Packet packet;
        packet<<(sf::Int8)P_MSGCHAT;

        if(configuration->hote)
            packet<<(sf::Int8)0;

        packet<<msg;

        if(configuration->hote)
        {
            for (std::list<sf::TcpSocket*>::iterator it = m_clientsTCP.begin(); it != m_clientsTCP.end(); ++it)
            {
                sf::TcpSocket& client = **it;
                client.Send(packet);
            }
        }
        else
            m_host->Send(packet);
    }
}



