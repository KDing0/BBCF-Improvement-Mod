#include "ScrWindow.h"

#include "Core/interfaces.h"
#include "Core/Settings.h"
#include "Core/utils.h"
#include "Game/gamestates.h"
#include "Overlay/NotificationBar/NotificationBar.h"
#include "Overlay/WindowManager.h"
#include "Overlay/Window/HitboxOverlay.h"

#include "Psapi.h"
#include <ctime>
#include <cstdlib>

void ScrWindow::Draw()
{
    if (m_showDemoWindow)
    {
        ImGui::ShowDemoWindow(&m_showDemoWindow);
    }

    DrawStatesSection();
}

void ScrWindow::DrawStatesSection()
{
    if (!ImGui::CollapsingHeader("States"))
        return;
    if (ImGui::Button("Load P2 Script")) {
        char* bbcf_base_adress = GetBbcfBaseAdress();
        std::vector<scrState*> states = parse_scr(bbcf_base_adress, 2);
        g_interfaces.player2.SetScrStates(states);
        g_interfaces.player2.states = states;
    }
    auto states = g_interfaces.player2.states;
    static int selected = 0;
    //std::srand(std::time(0));
    {
        ImGui::BeginChild("left pane", ImVec2(200, 0), true);
        for (int i = 0; i < g_interfaces.player2.states.size(); i++)
        {
            std::string label= g_interfaces.player2.states[i]->name;
            if (ImGui::Selectable(label.c_str(), selected == i))
                selected = i;
        }
        ImGui::EndChild();
        ImGui::SameLine();
    }
    // Right
    {
        ImGui::BeginGroup();
        static bool isActive_old;
        static bool isActive = false;
        if (ImGui::Checkbox("Naoto EN specials toggle", &isActive)) {
               memset(&g_interfaces.player2.GetData()->slot2_or_slot4, 0x00000018, 4);
        }
        if (isActive) {
            memset(&g_interfaces.player2.GetData()->slot2_or_slot4, 0x00000018, 4);
        }
        else {
            if (isActive != isActive_old) {
                memset(&g_interfaces.player2.GetData()->slot2_or_slot4, 0, 4);
            }
        }
        isActive_old = isActive;

        ImGui::BeginChild("item view", ImVec2(0, -ImGui::GetFrameHeightWithSpacing() - 100)); // Leave room for 1 line below us
        if (states.size()>0){
            auto selected_state = states[selected];
            ImGui::Text("%s", selected_state->name.c_str());
            ImGui::Separator();
            ImGui::Text("Addr: %d", selected_state->addr);
            ImGui::Text("Frames: %d", selected_state->frames);
            ImGui::Text("Damage: %d",  selected_state->damage);
            ImGui::Text("Atk_level: %d", selected_state->atk_level);
            ImGui::Text("Hitstun: %d", selected_state->hitstun);
            ImGui::Text("Blockstun: %d", selected_state->blockstun);
            ImGui::Text("Hitstop: %d", selected_state->hitstop);
            ImGui::Text("Starter_rating: %d", selected_state->starter_rating);
            ImGui::Text("Atk_P1: %d", selected_state->attack_p1);
            ImGui::Text("Atk_P2: %d", selected_state->attack_p2);
            ImGui::Text("Hit_overhead: %d", selected_state->hit_overhead);
            ImGui::Text("Hit_low: %d", selected_state->hit_low);
            ImGui::Text("Hit_air_ublockable: %d", selected_state->hit_air_unblockable);
            ImGui::Text("fatal_counter: %d", selected_state->fatal_counter);
            ImGui::Text("Whiff_cancels:", selected_state->fatal_counter);
            for (std::string name: selected_state->whiff_cancel) {
                ImGui::Text("    %s", name.c_str());
            }
            ImGui::Text("Hit_or_block_cancels(gatlings):", selected_state->fatal_counter);
            for (std::string name : selected_state->hit_or_block_cancel) {
                ImGui::Text("    %s", name.c_str());
            }
 
        }
        ImGui::EndChild();


        

        if (ImGui::Button("Set as wakeup action")) {
            states = g_interfaces.player2.states;
            auto selected_state = states[selected];
            std::string substr = "CmnActUkemiLandNLanding";
            std::vector<std::string> matches = {};
            for (auto state : states) {
                std::string name = state->name;
                if (name.find(substr) != std::string::npos) {
                    if (!state->replaced_state_script[0]) {
                        memcpy(state->replaced_state_script, state->addr + 36, 36);
                    }
                    override_state(state->addr, &selected_state->name[0]);

                }
            }
        }
        ImGui::SameLine();
        if (ImGui::Button("Set as gap action")) {
            states = g_interfaces.player2.states;
            auto selected_state = states[selected];
            std::string substr = "GuardEnd";
            std::vector<std::string> matches = {};
            for (auto state : states) {
                std::string name = state->name;
                if (name.find(substr) != std::string::npos) {
                    if (!state->replaced_state_script[0]) {
                        memcpy(state->replaced_state_script, state->addr + 36, 36);
                    }
                    override_state(state->addr, &selected_state->name[0]);

                }
            }
        }
        ImGui::SameLine();
        if (ImGui::Button("Use")) {
            states = g_interfaces.player2.states;
            auto selected_state = states[selected];
            memcpy(&(g_interfaces.player2.GetData()->currentScriptActionLocationInMemory), &(selected_state->addr),4);
        }
        ImGui::SameLine();
        if (ImGui::Button("Reset")) {
            states = g_interfaces.player2.states;
            for (auto state : states) {
                if (state->replaced_state_script[0]) {
                    memcpy(state->addr + 36, state->replaced_state_script, 36);
                    state->replaced_state_script[0] = 0;
                
                }
            }
            gap_register = {};
            wakeup_register = {};
        }

    
        if (ImGui::CollapsingHeader("Gap/wakeup random actions")) {
            ImGui::Columns(2);
            if (ImGui::Button("Add to wakeup action")) {
                states = g_interfaces.player2.states;
                wakeup_register.push_back(states[selected]);
            }
            ImGui::BeginChild("wakeup_register_display");
            for (auto e : wakeup_register) {
                ImGui::Text(e->name.c_str());
            }
            ImGui::EndChild();
            ImGui::NextColumn();
            if (ImGui::Button("Add to gap action")) {
                states = g_interfaces.player2.states;
                gap_register.push_back(states[selected]);
            }
        
            ImGui::BeginChild("gap_register_display");
            for (auto e : gap_register) {
                ImGui::Text(e->name.c_str());
            }
            ImGui::EndChild();
            if (!wakeup_register.empty()) {
                states = g_interfaces.player2.states;
                auto selected_state = states[selected];
                int random_pos = std::rand() % wakeup_register.size();
                std::string substr = "CmnActUkemiLandNLanding";
                for (auto state : states) {
                    std::string name = state->name;
                    if (name.find(substr) != std::string::npos) {
                        if (!state->replaced_state_script[0]) {
                            memcpy(state->replaced_state_script, state->addr + 36, 36);
                        }
                        override_state(state->addr, &wakeup_register[random_pos]->name[0]);

                    }
                }
            }


            if (!gap_register.empty()) {
                states = g_interfaces.player2.states;
                auto selected_state = states[selected];
                int random_pos = std::rand() % gap_register.size();
                std::string substr = "GuardEnd";
                for (auto state : states) {
                    std::string name = state->name;
                    if (name.find(substr) != std::string::npos) {
                        if (!state->replaced_state_script[0]) {
                            memcpy(state->replaced_state_script, state->addr + 36, 36);
                        }
                        override_state(state->addr, &gap_register[random_pos]->name[0]);
                    }
                }
            }
        }
        ImGui::EndGroup(); 
    
    }

}
