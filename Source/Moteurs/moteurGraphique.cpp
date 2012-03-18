

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



#include "moteurGraphique.h"
#include "../globale.h"

#include <iostream>
#include <sstream>

MoteurGraphique::MoteurGraphique()
{
    LightManager = Light_Manager::GetInstance();

    m_images.push_back(Image_moteur ());
    m_images.front().img = new sf::Texture();
    sf::Image img;
    img.Create(8, 8, sf::Color(255, 255, 255));
    m_images.front().img->LoadFromImage(img);


    m_images.front().nom         = "O";
    m_images.front().importance  = -1;


    m_tileset.push_back(Tileset_moteur ());
    m_tileset.front().tileset = new Tileset();

    m_tileset.front().nom         = "O";
    m_tileset.front().importance  = -1;

    m_soleil.rouge               = 255;
    m_soleil.vert                = 255;
    m_soleil.bleu                = 255;
    m_soleil.intensite           = 255;

    decalageCamera.x             = 0;
    decalageCamera.y             = 0;
    decalageCameraSouhaite.x     = 0;
    decalageCameraSouhaite.y     = 0;

    cameraDecale                 = false;
}

MoteurGraphique::~MoteurGraphique()
{
    LightManager->Kill();
    Vider();
    for (unsigned i = 0; i < m_tileset.size(); ++i)
        if(m_tileset[i].tileset)
            delete m_tileset[i].tileset;
    m_systemeParticules.clear();
    m_modeleSystemeParticules.clear();
    m_ecran.Close();

    for (unsigned i = 0; i < m_images.size(); ++i)
        if(m_images[i].img)
            delete m_images[i].img;
    m_images.clear();
}

void MoteurGraphique::CreateNewWindow()
{
    if (!configuration->mode_fenetre)
        m_ecran.Create(sf::VideoMode(configuration->Resolution.x, configuration->Resolution.y),"HolySpirit : Act of Faith",sf::Style::Fullscreen);
    else
        m_ecran.Create(sf::VideoMode(configuration->Resolution.x, configuration->Resolution.y),"HolySpirit : Act of Faith",sf::Style::Titlebar);

  // if (configuration->syncronisation_verticale)
     //   m_ecran.EnableVerticalSync(true);

    m_ecran.ShowMouseCursor(false);

    if (configuration->postFX)
    {
        /*EffectBlur.SetCurrentTexture("texture");
        EffectBlur2.SetCurrentTexture("texture");
        EffectBlurScreen.SetCurrentTexture("texture");
        EffectMort.SetCurrentTexture("framebuffer");
        EffectFiltre.SetCurrentTexture("framebuffer");
        EffectWater.SetCurrentTexture("framebuffer");
        EffectDistortion.SetCurrentTexture("framebuffer");*/

        EffectDistortion.SetParameter("distortion_map",m_distortion_screen.GetTexture());
    }

    m_ecran.SetActive();

    bufferImage.Create(m_ecran.GetWidth(), m_ecran.GetHeight());
    m_water_screen.Create(m_ecran.GetWidth(), m_ecran.GetHeight());
    m_distortion_screen.Create(m_ecran.GetWidth(), m_ecran.GetHeight());
    m_light_screen.Create(m_ecran.GetWidth()+64, m_ecran.GetHeight()+64);
    m_light_screen2.Create(m_ecran.GetWidth()+64, m_ecran.GetHeight()+64);
}

void MoteurGraphique::Charger()
{
    configuration->effetMort=0;
    if (configuration->postFX)
    {
        console->Ajouter("");
        console->Ajouter("Chargement des postFX :");

        if (!EffectBlur.LoadFromFile(configuration->chemin_fx+configuration->nom_effetBlur, sf::Shader::Fragment))
            console->Ajouter("Impossible de charger : "+configuration->chemin_fx+configuration->nom_effetBlur,1);
        else
            console->Ajouter("Chargement de : "+configuration->chemin_fx+configuration->nom_effetBlur,0);

        if (!EffectBlur2.LoadFromFile(configuration->chemin_fx+configuration->nom_effetBlur, sf::Shader::Fragment))
            console->Ajouter("Impossible de charger : "+configuration->chemin_fx+configuration->nom_effetBlur,1);
        else
            console->Ajouter("Chargement de : "+configuration->chemin_fx+configuration->nom_effetBlur,0);

        if (!EffectBlurScreen.LoadFromFile(configuration->chemin_fx+configuration->nom_effetBlur, sf::Shader::Fragment))
            console->Ajouter("Impossible de charger : "+configuration->chemin_fx+configuration->nom_effetBlur,1);
        else
            console->Ajouter("Chargement de : "+configuration->chemin_fx+configuration->nom_effetBlur,0);

        EffectBlur.SetParameter("offset",0.02);
        EffectBlur2.SetParameter("offset",0.005);

        if (!EffectMort.LoadFromFile(configuration->chemin_fx+configuration->nom_effetMort, sf::Shader::Fragment))
            console->Ajouter("Impossible de charger : "+configuration->chemin_fx+configuration->nom_effetMort,1);
        else
            console->Ajouter("Chargement de : "+configuration->chemin_fx+configuration->nom_effetMort,0);

        EffectMort.SetParameter("offset", 0);
        EffectMort.SetParameter("color",1, 1, 1);



        if (!EffectFiltre.LoadFromFile(configuration->chemin_fx+configuration->nom_effetFiltre, sf::Shader::Fragment))
            console->Ajouter("Impossible de charger : "+configuration->chemin_fx+configuration->nom_effetFiltre,1);
        else
            console->Ajouter("Chargement de : "+configuration->chemin_fx+configuration->nom_effetFiltre,0);

        if (!EffectWater.LoadFromFile(configuration->chemin_fx+configuration->nom_effetWater, sf::Shader::Fragment))
            console->Ajouter("Impossible de charger : "+configuration->chemin_fx+configuration->nom_effetWater,1);
        else
            console->Ajouter("Chargement de : "+configuration->chemin_fx+configuration->nom_effetWater,0);

        if (!EffectDistortion.LoadFromFile(configuration->chemin_fx+configuration->nom_effetDistortion, sf::Shader::Fragment))
            console->Ajouter("Impossible de charger : "+configuration->chemin_fx+configuration->nom_effetDistortion,1);
        else
            console->Ajouter("Chargement de : "+configuration->chemin_fx+configuration->nom_effetDistortion,0);

        m_img_water = AjouterImage(configuration->water_map,-1);
        EffectWater.SetParameter("water_map", *getImage(m_img_water));
    }

    console->Ajouter("");
    console->Ajouter("Chargement des polices d'�criture :");

    if (!m_font.LoadFromFile(configuration->chemin_fonts+configuration->font))
        console->Ajouter("Impossible de charger : "+configuration->chemin_fonts+configuration->font,1);
    else
        console->Ajouter("Chargement de : "+configuration->chemin_fonts+configuration->font,0);

    if (!m_font_titre.LoadFromFile(configuration->chemin_fonts+configuration->font_titre))
        console->Ajouter("Impossible de charger : "+configuration->chemin_fonts+configuration->font_titre,1);
    else
        console->Ajouter("Chargement de : "+configuration->chemin_fonts+configuration->font_titre,0);

    special_typo.Load(configuration->chemin_fonts+configuration->spec_font);
    special_typo_h.Load(configuration->chemin_fonts+configuration->spec_font_h);
    special_typo_p.Load(configuration->chemin_fonts+configuration->spec_font_p);

    m_img_item_background = AjouterImage(configuration->chemin_menus + configuration->nom_item_background,-1);
    m_img_item_unusable = AjouterImage(configuration->chemin_menus + configuration->nom_item_unusable,-1);

    m_ambientShadowTileset.Charger(configuration->ambientShadow_tileset);
}

void MoteurGraphique::Gerer(float temps)
{
    if(!configuration->mode_fenetre)
    {
        if(sf::Mouse::GetPosition(m_ecran).x > configuration->Resolution.x - 1)
            sf::Mouse::SetPosition(sf::Vector2i(configuration->Resolution.x - 1,sf::Mouse::GetPosition(m_ecran).y),m_ecran);
        if(sf::Mouse::GetPosition(m_ecran).y > configuration->Resolution.y - 1)
            sf::Mouse::SetPosition(sf::Vector2i(sf::Mouse::GetPosition(m_ecran).x,configuration->Resolution.y - 1),m_ecran);
    }


    m_transWater.x += temps*0.005;
    m_transWater.x += decalageReflection.x / configuration->Resolution.x / configuration->zoom * 0.014 * 0.5;
    decalageReflection.x = 0;
    if(m_transWater.x > 0.5)
        m_transWater.x = 0;
    if(m_transWater.x < 0)
        m_transWater.x = 0.5;

    m_transWater.y += temps*0.005;
    m_transWater.y -= decalageReflection.y / configuration->Resolution.y / configuration->zoom * 0.014 * 0.5;
    decalageReflection.y = 0;
    if(m_transWater.y > 0.5)
        m_transWater.y = 0;
    if(m_transWater.y < 0)
        m_transWater.y = 0.5;


    int k=0;
    m_systemeParticules_iter=m_systemeParticules.begin();
    while(m_systemeParticules_iter!=m_systemeParticules.end())
    {
        if (m_systemeParticules_iter->m_modele>=0&&m_systemeParticules_iter->m_modele<(int)m_modeleSystemeParticules.size())
        {
            if (!m_systemeParticules_iter->Afficher(&m_modeleSystemeParticules[m_systemeParticules_iter->m_modele],temps))
            {
                m_systemeParticules.erase (m_systemeParticules_iter);
                if((int)m_systemeParticules.size()>k)
                    m_systemeParticules_iter=m_systemeParticules.begin()+k;
            }
            else
                ++m_systemeParticules_iter,++k;
        }
        else
            ++m_systemeParticules_iter,++k;
    }


    k=0;
    m_effetsEcran_iter=m_effetsEcran.begin();

    bool decalage = false;
    while(m_effetsEcran_iter!=m_effetsEcran.end())
    {
        m_effetsEcran_iter->temps += temps;

        if(m_effetsEcran_iter->type == TREMBLEMENT)
        {
            if(m_effetsEcran_iter->temps > 0.05 && cameraDecale)
            {
                decalage = true;

                m_effetsEcran_iter->temps   = 0;

                int valeur = (int)m_effetsEcran_iter->info1 ;
                if(((decalage && (fabs(decalageCameraSouhaite.x) < fabs(valeur) && fabs(valeur) != m_effetsEcran_iter->info1))
                 || fabs(valeur) == m_effetsEcran_iter->info1) || !decalage)
                        decalageCameraSouhaite.x = (1 - 2 * (rand()%100<50))*valeur;

                valeur = (int)m_effetsEcran_iter->info1 ;
                if(((decalage && (fabs(decalageCameraSouhaite.y) < fabs(valeur) && fabs(valeur) != m_effetsEcran_iter->info1))
                 || fabs(valeur) == m_effetsEcran_iter->info1) || !decalage)
                        decalageCameraSouhaite.y = (1 - 2 * (rand()%100<50))*valeur;

                m_effetsEcran_iter->info1  -= 1;
            }

            if (m_effetsEcran_iter->info1 < 0)
            {
                m_effetsEcran.erase (m_effetsEcran_iter);
                if((int)m_effetsEcran.size()>k)
                    m_effetsEcran_iter=m_effetsEcran.begin()+k;
            }
            else
                ++m_effetsEcran_iter,++k;
        }
        else if(m_effetsEcran_iter->type == ENVOLEMENT)
        {
            for(m_systemeParticules_iter=m_systemeParticules.begin();
                m_systemeParticules_iter!=m_systemeParticules.end();
                ++m_systemeParticules_iter)
                    m_systemeParticules_iter->Envoler(m_effetsEcran_iter->pos,(int)m_effetsEcran_iter->info1,(int)m_effetsEcran_iter->info2);

            m_effetsEcran.erase (m_effetsEcran_iter);
            if((int)m_effetsEcran.size()>k)
                m_effetsEcran_iter=m_effetsEcran.begin()+k;
        }
    }
    if(decalage)
        cameraDecale = false;

    if(decalageCameraSouhaite.x < decalageCamera.x)
    {
        decalageCamera.x -= temps * 500 * (fabs(decalageCameraSouhaite.x ) + 1);
        if(decalageCamera.x < decalageCameraSouhaite.x)
            decalageCamera.x = decalageCameraSouhaite.x;
    }
    else if(decalageCameraSouhaite.x > decalageCamera.x)
    {
        decalageCamera.x += temps * 500 * (fabs(decalageCameraSouhaite.x) + 1);
        if(decalageCamera.x > decalageCameraSouhaite.x)
            decalageCamera.x = decalageCameraSouhaite.x;
    }

    if(decalageCameraSouhaite.y < decalageCamera.y)
    {
        decalageCamera.y -= temps * 500 * (fabs(decalageCameraSouhaite.y) + 1);
        if(decalageCamera.y < decalageCameraSouhaite.y)
            decalageCamera.y = decalageCameraSouhaite.y;
    }
    else if(decalageCameraSouhaite.y > decalageCamera.y)
    {
        decalageCamera.y += temps * 500 * (fabs(decalageCameraSouhaite.y) + 1);
        if(decalageCamera.y > decalageCameraSouhaite.y)
            decalageCamera.y = decalageCameraSouhaite.y;
    }

    if(decalageCameraSouhaite.y == decalageCamera.y && decalageCameraSouhaite.x == decalageCamera.x)
        cameraDecale = true;
}

void MoteurGraphique::Afficher()
{
    sf::Sprite sprite;

    if (configuration->RafraichirOmbre==1&&configuration->Ombre&&m_soleil.intensite>32)
    {
        sf::View temp = m_camera;
        temp.SetSize(configuration->Resolution.x + 64, configuration->Resolution.y + 64);
        temp.Zoom(configuration->zoom);

        decalageOmbre=temp.GetCenter();



        m_light_screen2.SetView(temp);

        m_light_screen2.Clear(sf::Color(255,255,255,255));

        LightManager->DrawWallShadow(&m_light_screen2,&temp,m_angleOmbreSoleil,m_soleil);

        for (IterCommande=m_commandes[9].begin();IterCommande!=m_commandes[9].end();++IterCommande)
        {
            IterCommande->m_sprite.SetColor(sf::Color(0,0,0,IterCommande->m_sprite.GetColor().a));
            m_light_screen2.Draw(IterCommande->m_sprite);
        }

        m_light_screen2.SetView(m_light_screen2.GetDefaultView());

        sf::Sprite sprite3;
        sprite3.SetTexture(*getImage(0));
        sprite3.Resize(configuration->Resolution.w+64,configuration->Resolution.h+64);
        sprite3.SetColor(sf::Color(sf::Color((int)(128+128-m_soleil.intensite*0.5),(int)(128+128-m_soleil.intensite*0.5),(int)(128+128-m_soleil.intensite*0.5))));
        m_light_screen2.Draw(sprite3, sf::BlendAdd);

        m_light_screen2.Display();


        if(configuration->postFX)
        {
            EffectBlur2.SetParameter("direction_x", 1.0);
            sf::Sprite buf;
            buf.SetTexture(m_light_screen2.GetTexture());
            m_light_screen2.SetView(m_light_screen2.GetDefaultView());
            m_light_screen2.Draw(buf, &EffectBlur2);
            m_light_screen2.Display();
            EffectBlur2.SetParameter("direction_x", 0);
        }
    }

    if (configuration->Lumiere > 0 && configuration->RafraichirLumiere)
    {
        m_light_screen.SetView(m_light_screen.GetDefaultView());

        m_light_screen.Clear(sf::Color(m_soleil.rouge * m_soleil.intensite/255,
                                       m_soleil.vert  * m_soleil.intensite/255,
                                       m_soleil.bleu  * m_soleil.intensite/255,LIGHT_ALPHA));


        sf::View temp = m_camera;
        temp.SetSize(configuration->Resolution.x + 64, configuration->Resolution.y + 64);
        temp.Zoom(configuration->zoom);

        decalageLumiere=temp.GetCenter();

        m_light_screen.SetView(temp);

        LightManager->Draw(&m_light_screen,&temp);

        m_light_screen.Display();

        if(configuration->postFX)
        {
            EffectBlur.SetParameter("direction_x", 1.0);
            sf::Sprite buf;
            buf.SetTexture(m_light_screen.GetTexture());
            m_light_screen.SetView(m_light_screen.GetDefaultView());
            m_light_screen.Draw(buf, &EffectBlur);
            m_light_screen.Display();
            EffectBlur.SetParameter("direction_x", 0);
        }
    }

    if(configuration->Reflection && configuration->postFX)
        m_water_screen.Clear(sf::Color(0,0,0,255));
    else
        bufferImage.Clear(sf::Color(0,0,0,255));

    if(configuration->Distortion)
        m_distortion_screen.Clear(sf::Color(128,128,128,255));

    if(configuration->postFX && configuration->Distortion)
    for (IterCommande=m_distortion_commandes.begin();IterCommande!=m_distortion_commandes.end();++IterCommande)
    {
        if (IterCommande->m_utiliserCamera)
            m_distortion_screen.SetView(m_camera);
        else
            m_distortion_screen.SetView(m_distortion_screen.GetDefaultView());

        m_distortion_screen.Draw(IterCommande->m_sprite);
    }
    m_distortion_commandes.clear();

    for (int k=0;k<=20;++k)
    {
        if (k==12 && configuration->Lumiere)
        {
            sf::Sprite screen(m_light_screen.GetTexture());

            sf::RenderStates r(sf::BlendMultiply);

            screen.SetX(decalageLumiere.x-m_camera.GetCenter().x-32);
            screen.SetY(decalageLumiere.y-m_camera.GetCenter().y-32);

            bufferImage.SetView(bufferImage.GetDefaultView());

            if(configuration->postFX)
                r.Shader = &EffectBlur;

            bufferImage.Draw(screen, r);
        }

        if (k==10 && configuration->Ombre && m_soleil.intensite>32)
        {
            sf::Sprite screen(m_light_screen2.GetTexture());

            sf::RenderStates r(sf::BlendMultiply);

            screen.SetX(decalageOmbre.x-m_camera.GetCenter().x-32);
            screen.SetY(decalageOmbre.y-m_camera.GetCenter().y-32);

            bufferImage.SetView(bufferImage.GetDefaultView());

            if(configuration->postFX)
                r.Shader = &EffectBlur2;

            bufferImage.Draw(screen,r);
        }

        if (k==12 && configuration->postFX && configuration->Distortion)
        {
            bufferImage.SetView(bufferImage.GetDefaultView());

            m_distortion_screen.Display();

           // EffectDistortion.SetCurrentTexture("framebuffer");
            EffectDistortion.SetParameter("distortion_map", m_distortion_screen.GetTexture());

            bufferImage.Display();

            bufferImage.Draw(sf::Sprite(bufferImage.GetTexture()), &EffectDistortion);
        }

        if (k!=9)
        {
            for (IterCommande=m_commandes[k].begin();IterCommande!=m_commandes[k].end();++IterCommande)
            {
                if(k == 0)
                    IterCommande->m_sprite.SetColor(sf::Color(128+m_soleil.rouge*m_soleil.intensite/512,
                                                              128+m_soleil.vert*m_soleil.intensite/512,
                                                              128+m_soleil.bleu*m_soleil.intensite/512,IterCommande->m_sprite.GetColor().a));

                if(k == 0 && configuration->postFX && configuration->Reflection)
                {
                    if (IterCommande->m_utiliserCamera)
                        m_water_screen.SetView(m_camera);
                    else
                        m_water_screen.SetView(m_water_screen.GetDefaultView());

                    sf::RenderStates r(IterCommande->m_states);
                    r.Shader = &EffectFiltre;

                    m_water_screen.Draw(IterCommande->m_sprite,r);
                }
                else
                {
                    if (IterCommande->m_utiliserCamera)
                        bufferImage.SetView(m_camera);
                    else
                        bufferImage.SetView(bufferImage.GetDefaultView());


                    sf::RenderStates r(IterCommande->m_states);

                    if(k < 12 && configuration->postFX)
                        r.Shader = &EffectFiltre;

                    bufferImage.Draw(IterCommande->m_sprite,r);
                }
            }
        }

        bufferImage.SetView(bufferImage.GetDefaultView());

        if(k == 0 && configuration->postFX && configuration->Reflection)
        {
            m_water_screen.Display();
            EffectWater.SetParameter("translation", m_transWater.x, m_transWater.y);
            EffectWater.SetParameter("offset", 1/configuration->zoom , 1/configuration->zoom );
            bufferImage.Draw(sf::Sprite (m_water_screen.GetTexture()), &EffectWater);
        }

        for (IterTextes=m_textes[k].begin();IterTextes!=m_textes[k].end();++IterTextes)
            bufferImage.Draw(*IterTextes);


        if (k==13)
        {
            EffectFiltre.SetParameter("color", configuration->contrastes-1, configuration->contrastes-1, configuration->contrastes-1);
            EffectFiltre.SetParameter("luminosity", configuration->luminosite/128);

            if (configuration->effetMort>0 && configuration->postFX)
            {
                bufferImage.SetView(bufferImage.GetDefaultView());
                bufferImage.Display();
                bufferImage.Draw(sf::Sprite(bufferImage.GetTexture()), &EffectMort);
            }
        }


        if (k==18 && configuration->postFX)
        {
            if (m_blur>0)
            {
                bufferImage.SetView(bufferImage.GetDefaultView());
                EffectBlurScreen.SetParameter("offset",(float)m_blur);
                bufferImage.Display();
                bufferImage.Draw(sf::Sprite(bufferImage.GetTexture()), &EffectBlurScreen);
            }
        }

        if (configuration->effetNoir>0 && k == 18)
        {
            sf::Sprite sprite2;
            sprite2.SetTexture(*getImage(0));
            sprite2.Resize(configuration->Resolution.x,configuration->Resolution.y);
            sprite2.SetColor(sf::Color((int)(configuration->effetNoir*255),(int)(configuration->effetNoir*255),(int)(configuration->effetNoir*255),255));
            bufferImage.Draw(sprite2,sf::BlendMultiply);
        }

        m_commandes[k].clear();
        m_textes[k].clear();
    }

    bufferImage.Display();
    m_ecran.Draw(sf::Sprite(bufferImage.GetTexture()));
    m_ecran.Display();
}

int MoteurGraphique::AjouterImage(const char *Data, std::size_t SizeInBytes, std::string nom,int importance, bool newimage)
{
    if(!newimage)
    {
        for (unsigned i=0; i < m_images.size(); i++)
        {
            if (m_images[i].nom==nom && m_images[i].img != NULL)
            {
                m_images[i].importance=importance;
                return i;
            }
        }
        for (unsigned i=0; i < m_images.size(); i++)
        {
            if (m_images[i].img == NULL)
            {
                m_images[i].nom=nom;

                m_images[i].img = new sf::Texture();
                m_images[i].img->SetSmooth(true);

                if (!configuration->lissage)
                    m_images[i].img->SetSmooth(false);

                if (!m_images[i].img->LoadFromMemory(Data,SizeInBytes))
                {
                    console->Ajouter("Impossible de charger : "+nom,1);
                    return -1;
                }
                else
                    console->Ajouter("Chargement de : "+nom,0);

                m_images[i].importance=importance;

                return i;
            }
        }
    }


    m_images.push_back(Image_moteur ());
    m_images.back().nom=nom;

    m_images.back().img = new sf::Texture();
    m_images.back().img->SetSmooth(true);

    if (!configuration->lissage)
        m_images.back().img->SetSmooth(false);

    if (!m_images.back().img->LoadFromMemory(Data,SizeInBytes))
    {
        console->Ajouter("Impossible de charger depuis la m�moire : "+nom,1);
        return -1;
    }
    else
        console->Ajouter("Chargement de : "+nom,0);

    m_images.back().importance=importance;

    return m_images.size()-1;
}

int MoteurGraphique::AjouterImage(std::string chemin,int importance,bool newimage)
{
    if(!newimage)
    {
        for (unsigned i=0; i < m_images.size(); i++)
        {
            if (m_images[i].nom==chemin && m_images[i].img != NULL)
            {
                m_images[i].importance=importance;
                return i;
            }
        }
        for (unsigned i=0; i < m_images.size(); i++)
        {
            if (m_images[i].img == NULL)
            {
                m_images[i].nom=chemin;

                m_images[i].img = new sf::Texture();
                m_images[i].img->SetSmooth(true);

                if (!configuration->lissage)
                    m_images[i].img->SetSmooth(false);

                if (!m_images[i].img->LoadFromFile(chemin.c_str()))
                {
                    console->Ajouter("Impossible de charger : "+chemin,1);
                    return -1;
                }
                else
                    console->Ajouter("Chargement de : "+chemin,0);

                m_images[i].importance=importance;

                return i;
            }
        }
    }


    m_images.push_back(Image_moteur ());
    m_images.back().nom=chemin;

    m_images.back().img = new sf::Texture();
    m_images.back().img->SetSmooth(true);

    if (!configuration->lissage)
        m_images.back().img->SetSmooth(false);

    if (!m_images.back().img->LoadFromFile(chemin.c_str()))
    {
        console->Ajouter("Impossible de charger : "+chemin,1);
        return -1;
    }
    else
        console->Ajouter("Chargement de : "+chemin,0);

    m_images.back().importance=importance;

    return m_images.size()-1;
}


int MoteurGraphique::AjouterImage(const sf::Texture &img,int importance)
{
    m_images.push_back(Image_moteur ());
    m_images.back().nom="";

    m_images.back().img = new sf::Texture();
    (*m_images.back().img) = img;
    m_images.back().img->SetSmooth(true);

    //if (!configuration->lissage)
       // m_images.back().img->SetSmooth(false);

    m_images.back().importance=importance;

    return m_images.size()-1;
}

int MoteurGraphique::AjouterTileset(std::string chemin,int importance)
{
    std::ifstream fichier;
    fichier.open(chemin.c_str(), std::ios::in);
    int retour = AjouterTileset(fichier, chemin, importance);
    console->Ajouter("Chargement de : "+chemin,0);
    fichier.close();
    return retour;
}


int MoteurGraphique::AjouterTileset(std::ifstream &fichier, std::string chemin,int importance)
{
    if(chemin != "")
        for (unsigned i=0; i < m_tileset.size(); i++)
        {
            if (m_tileset[i].nom == chemin)
            {
                m_tileset[i].tileset->Charger(fichier);
                m_tileset[i].importance = importance;

               // Tileset buffer(fichier);

                return i;
            }
        }

    m_tileset.push_back(Tileset_moteur ());

    m_tileset.back().nom = chemin;
    m_tileset.back().tileset = new Tileset(fichier);
    m_tileset.back().tileset->m_chemin = chemin;
    m_tileset.back().importance = importance;

    return m_tileset.size()-1;
}


void MoteurGraphique::DecrementerImportance()
{
    for (unsigned i=0; i < m_images.size(); i++)
        if (m_images[i].importance!=-1 && m_images[i].img != NULL)
        {
            m_images[i].importance--;
            if (m_images[i].importance<=0)
            {
                delete m_images[i].img;
                m_images[i].img = NULL;
                m_images[i].nom="",m_images[i].importance=0;
            }
        }


    for (unsigned i = 0; i < m_tileset.size(); ++i)
        if(m_tileset[i].tileset)
            delete m_tileset[i].tileset;
    m_tileset.clear();
}

int MoteurGraphique::AjouterModeleSystemeParticules(std::string chemin)
{
    for (unsigned i=0;i<m_modeleSystemeParticules.size();i++)
        if (m_modeleSystemeParticules[i].m_chemin==chemin)
            return i;

    m_modeleSystemeParticules.push_back(ModeleParticuleSysteme (chemin));

    return m_modeleSystemeParticules.size()-1;
}


void MoteurGraphique::AjouterSystemeParticules(int ID,coordonnee position,sf::Color color,float force,float angle)
{
    if (ID>=0&&ID<(int)m_modeleSystemeParticules.size())
        m_systemeParticules.push_back(ParticuleSysteme (ID,&m_modeleSystemeParticules[ID],position,color,force,angle));
}

void MoteurGraphique::AjouterEntiteGraphique(Entite_graphique *entite,  const sf::RenderStates& states)
{
    if(entite->m_tileset != NULL)
    {
        if(entite->m_ambientShadow >= 0)
        {
            sf::Sprite sprite2;
            coordonnee positionPartieDecor = m_ambientShadowTileset.getPositionDuTile(entite->m_ambientShadow);

            sprite2.SetTexture(*moteurGraphique->getImage(m_ambientShadowTileset.getImage(entite->m_ambientShadow)));
            sprite2.SetTextureRect(sf::IntRect(positionPartieDecor.x, positionPartieDecor.y,
                                          positionPartieDecor.w, positionPartieDecor.h - 1));

            sprite2.SetOrigin(m_ambientShadowTileset.getCentreDuTile(entite->m_ambientShadow).x,
                              m_ambientShadowTileset.getCentreDuTile(entite->m_ambientShadow).y);

            sprite2.SetPosition(entite->GetPosition());

            sprite2.Move(entite->m_decalage.x,entite->m_decalage.y);

            AjouterCommande(&sprite2, 10 + entite->m_decalCouche, !entite->m_fixed);
        }

        if(entite->GetGlobalBounds().Width > 0 && entite->GetGlobalBounds().Height > 0)
        {
            {
                sf::Sprite sprite = *entite;
                sprite.Move(entite->m_decalage.x,entite->m_decalage.y);

                if((sprite.GetPosition().x + sprite.GetGlobalBounds().Width - sprite.GetOrigin().x     >= GetViewRect(m_camera).Left
                 && sprite.GetPosition().x - sprite.GetOrigin().x                          <  GetViewRect(m_camera).Left + GetViewRect(m_camera).Width
                 && sprite.GetPosition().y + sprite.GetGlobalBounds().Height - sprite.GetOrigin().y     >= GetViewRect(m_camera).Top
                 && sprite.GetPosition().y - sprite.GetOrigin().y                          <  GetViewRect(m_camera).Top + GetViewRect(m_camera).Height)
                || sprite.GetRotation() != 0 || entite->m_fixed)
                    AjouterCommande(&sprite, entite->m_couche + entite->m_decalCouche, !entite->m_fixed, states);
            }

            if(entite->m_shadow && configuration->Ombre && entite->m_couche + entite->m_decalCouche >= 10)
            {
                sf::Sprite sprite;
                sprite = *entite;
                sprite.Move(entite->m_decalage.x,entite->m_decalage.y);

                sprite.SetScale(1, (100-(float)m_soleil.hauteur)/50);
                sprite.SetRotation(-m_angleOmbreSoleil);

                AjouterCommande(&sprite, 9, !entite->m_fixed);
            }

            if(entite->m_reflect && configuration->Reflection)
            {
                sf::Sprite sprite;
                sprite = *entite;
                sprite.Move(entite->m_decalage.x,entite->m_decalage.y);

                sprite.Scale(1,-1);
                //sprite.FlipX(true);
                sprite.SetOrigin(sprite.GetOrigin().x, sprite.GetGlobalBounds().Height - sprite.GetOrigin().y);

                if(sprite.GetPosition().x + sprite.GetGlobalBounds().Width - sprite.GetOrigin().x     >= GetViewRect(m_camera).Left
                && sprite.GetPosition().x - sprite.GetOrigin().x                          <  GetViewRect(m_camera).Left + GetViewRect(m_camera).Width
                && sprite.GetPosition().y + sprite.GetGlobalBounds().Height - sprite.GetOrigin().y     >= GetViewRect(m_camera).Top
                && sprite.GetPosition().y - sprite.GetOrigin().y                          <  GetViewRect(m_camera).Top + GetViewRect(m_camera).Height)
                    AjouterCommande(&sprite, 0, !entite->m_fixed);
            }
        }



        if(entite->m_distort && configuration->Distortion)
        {
           if((entite->m_sprite_distortion.GetPosition().x + entite->m_sprite_distortion.GetGlobalBounds().Width - entite->m_sprite_distortion.GetOrigin().x    >= GetViewRect(m_camera).Left
            && entite->m_sprite_distortion.GetPosition().x - entite->m_sprite_distortion.GetOrigin().x                                              <  GetViewRect(m_camera).Left + GetViewRect(m_camera).Width
            && entite->m_sprite_distortion.GetPosition().y + entite->m_sprite_distortion.GetGlobalBounds().Height - entite->m_sprite_distortion.GetOrigin().y    >= GetViewRect(m_camera).Top
            && entite->m_sprite_distortion.GetPosition().y - entite->m_sprite_distortion.GetOrigin().y)                                           <  GetViewRect(m_camera).Top + GetViewRect(m_camera).Height
            || entite->m_sprite_distortion.GetRotation() != 0)
                m_distortion_commandes.push_back(Commande (&entite->m_sprite_distortion, !entite->m_fixed, sf::BlendAlpha));
        }



        for(unsigned i = 0 ; i < entite->m_sprite_shadowmap.size() ; ++i)
        {
            if(fabs((float)entite->m_angle_shadowmap[i] - m_angleOmbreSoleil + 135) > 90)
            {
                sf::Sprite sprite = entite->m_sprite_shadowmap[i];
                sprite.Move(entite->m_decalage.x,entite->m_decalage.y);
                float intensity = (fabs((float)entite->m_angle_shadowmap[i] - m_angleOmbreSoleil + 135) - 90)/45 * 96;

                if(fabs((float)entite->m_angle_shadowmap[i] - m_angleOmbreSoleil + 135) > 135)
                    intensity = 96;

                if(m_soleil.intensite < 128)
                     intensity *= (float)m_soleil.intensite / 128;

                sprite.SetColor(sf::Color(255,255,255,(int)intensity));
                AjouterCommande(&sprite, entite->m_couche + entite->m_decalCouche, !entite->m_fixed);
            }

        }
    }
}

void MoteurGraphique::AjouterCommande(sf::Sprite *sprite, int couche, bool camera, const sf::RenderStates &s)
{
    if (couche>=0&&couche<=20)
        m_commandes[couche].push_back(Commande (sprite,camera,s));
}

void MoteurGraphique::AjouterTexte(const std::string &txt, coordonnee pos, Border &border, int couche, bool titre, int size, sf::Color color)
{
    sf::Text temp;
    temp.SetFont(m_font);
    temp.SetString(txt);
    temp.SetPosition(pos.x, pos.y);
    temp.SetCharacterSize(size);
    temp.SetColor(color);
    AjouterTexte(&temp, couche, titre);

    pos.x -= 6;
    pos.y -= 2;

    coordonnee s;
    s.x = (int)temp.GetGlobalBounds().Width + 13;
    s.y = (int)temp.GetGlobalBounds().Height + 6;
    border.Afficher(pos, s, couche, sf::Color(224,224,224,color.a));
}

void MoteurGraphique::AjouterTexte(const std::string &txt, coordonnee pos, int couche, bool titre, int size, sf::Color color)
{
    sf::Text temp;
    temp.SetFont(m_font);
    temp.SetString(txt);
    temp.SetPosition(pos.x, pos.y);
    temp.SetCharacterSize(size);
    temp.SetColor(color);
    AjouterTexte(&temp, couche, titre);
}

void MoteurGraphique::AjouterTexteNonChevauchable(sf::Text* string, int couche, bool titre)
{
    if (couche>=0&&couche<=20)
    {
        for (IterTextes=m_textes[couche].begin();IterTextes!=m_textes[couche].end();++IterTextes)
        {
            if(string->GetGlobalBounds().Left + string->GetGlobalBounds().Width > IterTextes->GetGlobalBounds().Left - 8
            && string->GetGlobalBounds().Left                           < IterTextes->GetGlobalBounds().Left + IterTextes->GetGlobalBounds().Width + 8
            && string->GetGlobalBounds().Top  + string->GetGlobalBounds().Height> IterTextes->GetGlobalBounds().Top - 8
            && string->GetGlobalBounds().Top                            < IterTextes->GetGlobalBounds().Top + IterTextes->GetGlobalBounds().Height + 8)
                string->SetPosition(string->GetPosition().x, IterTextes->GetGlobalBounds().Top - string->GetGlobalBounds().Height - 17), IterTextes=m_textes[couche].begin();
        }

        AjouterTexte(string, couche, titre);
    }
}

void MoteurGraphique::AjouterTexte(sf::Text* string, int couche,bool titre)
{

    if (couche>=0&&couche<=20)
    {
        sf::Text temp(*string);
        temp.SetPosition((int)temp.GetPosition().x,(int)temp.GetPosition().y);
        temp.SetFont(m_font);

        if (titre)
        {
            temp.SetFont(m_font_titre);

            /*temp.SetStyle(sf::Text::Bold);
            temp.SetColor(string->GetColor());

            m_textes[couche].push_back(temp);

            temp.SetColor(sf::Color((int)(string->GetColor().r*0.15),(int)(string->GetColor().g*0.15),(int)(string->GetColor().b*0.15),string->GetColor().a));
            temp.SetStyle(sf::Text::Regular);*/
            //m_textes[couche].push_back(temp);
        }
      //  else
        {
            temp.Move(2,2);
            temp.SetColor(sf::Color((int)(string->GetColor().r*0.05),(int)(string->GetColor().g*0.05),(int)(string->GetColor().b*0.05),string->GetColor().a));
            m_textes[couche].push_back(temp);
            temp.Move(-1,-1);
            temp.SetColor(string->GetColor());
        }

        m_textes[couche].push_back(temp);
    }
}

void MoteurGraphique::Vider()
{
    for (int i=0;i<=20;i++)
    {
        m_commandes[i].clear();
        m_textes[i].clear();
    }
}
void MoteurGraphique::ViderParticules()
{
    m_systemeParticules.clear();
}

Entite_graphique MoteurGraphique::getEntiteGraphique(int noTileset, int noTile, int couche)
{
    Entite_graphique entite;

    if(noTileset >= 0 && noTileset < (int)m_tileset.size())
        entite.m_tileset    = m_tileset[noTileset].tileset;
    else
        entite.m_tileset    = NULL;

    entite.m_noAnimation    = noTile;
    entite.m_couche         = couche;
    //entite.Generer();

    return entite;
}

sf::Texture* MoteurGraphique::getImage(int IDimage)
{
    if (IDimage>=0&&IDimage<(int)m_images.size())
        return m_images[(unsigned)IDimage].img;
    return m_images.front().img;
}

Tileset* MoteurGraphique::getTileset(int IDtileset)
{
    if (IDtileset>=0 && IDtileset<(int)m_tileset.size())
        return m_tileset[(unsigned)IDtileset].tileset;
    return m_tileset.front().tileset;
}

ModeleParticuleSysteme* MoteurGraphique::getModeleMoteurParticules(int ID)
{
    if (ID>=0&&ID<(int)m_modeleSystemeParticules.size())
        return &m_modeleSystemeParticules[ID];
    else
        return &m_modeleSystemeParticules[0];
}

std::string MoteurGraphique::getCheminImage(int IDimage)
{
    if (IDimage>=0&&IDimage<(int)m_images.size())
        return m_images[(unsigned)IDimage].nom;
    return "";
}

bool MoteurGraphique::getEvent(sf::Event &EventReceived)
{
    return m_ecran.PollEvent(EventReceived);
}

sf::Vector2f MoteurGraphique::getPositionSouris()
{
    return m_ecran.ConvertCoords(sf::Mouse::GetPosition(m_ecran).x, sf::Mouse::GetPosition(m_ecran).y, m_camera);
}

sf::Vector2f MoteurGraphique::getPositionSourisEcran()
{
    return sf::Vector2f(sf::Mouse::GetPosition(m_ecran).x, sf::Mouse::GetPosition(m_ecran).y);
}

void MoteurGraphique::Printscreen()
{
    std::ostringstream buf;
    buf<<"screenshot"<<configuration->numero_screen<<".png";
    m_ecran.Capture().SaveToFile(buf.str());
    configuration->numero_screen++;
}

int MoteurGraphique::GetFPS()
{
    return (int)( 1.f / m_ecran.GetFrameTime());
}

