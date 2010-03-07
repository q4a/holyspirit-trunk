#ifndef GAMEPANEL_HPP
#define GAMEPANEL_HPP

#include "Global.hpp"
#include "Bejeweled.hpp"
#include "GUI/Button.hpp"
#include "GUI/GUIProgressBar.hpp"
#include "GUI/TabBar.hpp"
#include "GUI/Label.hpp"
#include "GUI/GUIImage.hpp"
#include "GUI/GUIAnimatedImage.hpp"
#include "GUI/GUIWindow.hpp"

class GamePanel : public Widget
{
    public:
        GamePanel();
        ~GamePanel();
        void Update();

    private:
        Tab         *m_tab_food,
                    *m_tab_material,
                    *m_tab_science,
                    *m_tab_army,
                    *m_tab_religion;

        Label       *m_label;

        GUIImage    *m_panel_ressources,
                    *m_panel_map,
                    *m_panel_bejeweled,
                    *m_panel_city;

        GUIImage    *m_cadran_bejeweled,
                    *m_cadran_map,
                    *m_cadran_resources,
                    *m_map;

        TabBar      *m_tab_resources;

        Widget      *m_panel_food,
                    *m_panel_material,
                    *m_panel_science,
                    *m_panel_army,
                    *m_panel_religion;

        GUIImage    *m_panel_army_deco;

        Bejeweled   *m_bejeweled;

        GUIAnimatedImage    *m_animated_tab_army_off,
                            *m_animated_tab_army_on,
                            *m_animated_tab_religion_off,
                            *m_animated_tab_religion_on,
                            *m_animated_tab_science_off,
                            *m_animated_tab_science_on,
                            *m_animated_tab_food_off,
                            *m_animated_tab_food_on,
                            *m_animated_tab_material_off,
                            *m_animated_tab_material_on;

        GUIProgressBar      *m_fish_ressources,
                            *m_weath_ressources,
                            *m_meat_ressources,
                            *m_wood_ressources,
                            *m_rock_ressources;
};

#endif
