/***********************************************************************
* @internal @copyright
*
*  				MAREngine - open source 3D game engine
*
* Copyright (C) 2020-present Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
* All rights reserved.
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*
************************************************************************/


#include "../../public/ServiceLocatorEditor.h"
#include "MainMenuBarImGui.h"
#include "FilesystemPopUpImGui.h"
#include "../../../Core/ecs/Scene.h"
#include "../../../ProjectManager.h"
#include "../../../MAREngine.h"
#include "../../../Core/ecs/SceneManagerEditor.h"
#include "../../../Core/filesystem/SceneSerializer.h"
#include "../../../Window/Window.h"


namespace marengine {


    static void setDefaultMAREngineDarkTheme();
    static bool displayInfoAboutEngineAuthor();
    static bool displayEngineInstructions();
    static bool displayWindowSettings(FWindow* pWindow);


    void FMainMenuBarWidgetImGui::create(FServiceLocatorEditor* pServiceLocator) {
        m_pSceneManagerEditor =
                pServiceLocator->retrieve<FHolderPtr<FSceneManagerEditor*>>()->pInstance;
        m_pWindow =
                pServiceLocator->retrieve<FHolderPtr<FWindow*>>()->pInstance;
        m_pFilesystem =
                pServiceLocator->retrieve<FFilesystemPopUpImGuiWidget>();

        setDefaultMAREngineDarkTheme();
    }

    void FMainMenuBarWidgetImGui::updateFrame() {
        const std::string newSceneName{"New Scene Widget" };
        const std::string openSceneName{ "Open Scene Widget" };
        const std::string saveSceneName{ "Save Scene Widget" };
        const std::string extMarscene{ ".json" };

        if (ImGui::BeginMainMenuBar()) {

            displaySceneManagementTab();
            displayEntitiesManagementTab();
            displaySettingsTab();
            displayAboutTab();

            ImGui::EndMainMenuBar();
        }

        if(m_infoAboutAuthorDisplay) {
            m_infoAboutAuthorDisplay = displayInfoAboutEngineAuthor();
        }
        if(m_infoAboutEngineDisplay) {
            m_infoAboutEngineDisplay = displayEngineInstructions();
        }
        if(m_windowSettingsDisplay) {
            m_windowSettingsDisplay = displayWindowSettings(m_pWindow);
        }
        if(m_newSceneDisplay) {
            m_pFilesystem->openWidget(newSceneName);
            m_newSceneDisplay = false;
        }
        if(m_openSceneDisplay) {
            m_pFilesystem->openWidget(openSceneName);
            m_openSceneDisplay = false;
        }
        if(m_saveSceneDisplay) {
            m_pFilesystem->openWidget(saveSceneName);
            m_saveSceneDisplay = false;
        }

        auto openSceneCallback = [](const std::string& path, const std::string& filename) {
            // TODO: implement displayOpenSceneWidget(), FProjectManager::retrieveProjectInfo is needed
            // we don't need special method as it does not operate on any important variable
            FProjectManager::retrieveProjectInfo(path, filename);
            FEngineState::setRestart();
        };

        auto newSceneCallback = [](const std::string& path, const std::string& filename) {
            // TODO: implement displayNewSceneWidget(), FProjectManager::fillProjectInfo is needed
            Scene* pScene{ Scene::createEmptyScene("NewScene") };
            FSceneSerializer::saveSceneToFile(path.c_str(), pScene);
            delete pScene;
            FEngineState::setRestart();
            FProjectManager::fillProjectInfo(path, filename);
        };

        m_pFilesystem->displayOpenWidget(openSceneName, extMarscene, openSceneCallback);
        m_pFilesystem->displaySaveWidget(newSceneName, extMarscene, newSceneCallback);

        const FFilesystemDialogInfo dialogInfo = m_pFilesystem->displaySaveWidget(saveSceneName,
                                                                                  extMarscene);
        if(dialogInfo.isValid()) {
            FSceneSerializer::saveSceneToFile(dialogInfo.pPath->c_str(),
                                              m_pSceneManagerEditor->getScene());
        }
    }

    void FMainMenuBarWidgetImGui::displaySceneManagementTab() {
        if (ImGui::BeginMenu("Scene")) {
            if (ImGui::MenuItem("New Scene")) {
                m_newSceneDisplay = true;
            }
            if (ImGui::MenuItem("Open Scene")) {
                m_openSceneDisplay = true;
            }
            if (ImGui::MenuItem("Save Scene")) {
                m_saveSceneDisplay = true;
            }
            if (ImGui::MenuItem("Exit")) {
                m_pWindow->close();
            }

            ImGui::EndMenu();
        }
    }

    void FMainMenuBarWidgetImGui::displayEntitiesManagementTab() {
        // TODO: Add entities management options to main menu bar (like load .obj file / copy entity)
        if (ImGui::BeginMenu("Entities")) {
            if (ImGui::MenuItem("Load external .obj file")) { }
            if (ImGui::MenuItem("Copy selected entity")) { }

            ImGui::EndMenu();
        }
    }

    void FMainMenuBarWidgetImGui::displaySettingsTab() {
        // TODO: Add more settings to MainMenuBar
        if (ImGui::BeginMenu("Settings")) {
            if (ImGui::MenuItem("Window Settings")) { m_windowSettingsDisplay = true; }

            ImGui::EndMenu();
        }
    }

    void FMainMenuBarWidgetImGui::displayAboutTab() {
        if (ImGui::BeginMenu("About")) {
            if (ImGui::MenuItem("About Engine")) { m_infoAboutAuthorDisplay = true; }
            if (ImGui::MenuItem("Instruction")) { m_infoAboutEngineDisplay = true; }

            ImGui::EndMenu();
        }
    }

    static bool displayInfoAboutEngineAuthor() {
        // TODO: Implement better info about engine and author window
        ImGui::Begin("About");

        const char* const aboutEngine =
                "MAREngine is an educational 3D game engine written in modern C++.\n"
                "It is created with a dream of easy and intuitive interface for everyone.\n"
                "You can download MAREngine completely freely and release your creativity - create games you imagine.";

        const char* const aboutAuthor =
                "Mateusz Rzeczyca is C++ / Python programmer and enthusiast of Augmented Reality and Machine Learning.\n"
                "\n"
                "I am delighted that you are using MAREngine! Thank you!";

        ImGui::Text("About Engine");
        ImGui::Text("%s", aboutEngine);

        ImGui::Separator();

        ImGui::Text("About Author");
        ImGui::Text("%s", aboutAuthor);

        if (ImGui::Button("Close")) {
            ImGui::End();
            return false;
        }

        ImGui::End();
        return true;
    }

    static bool displayEngineInstructions() {
        // TODO: Implement instructions about engine window
        ImGui::Begin("Instructions");

        if (ImGui::Button("Close")) {
            ImGui::End();
            return false;
        }

        ImGui::End();
        return true;
    }

    static bool displayWindowSettings(FWindow* pWindow) {
        ImGui::Begin("Window Settings");

        static bool verticalSync{ true };
        ImGui::Checkbox("Window Vertical Synchronization (when turned off increases FPS)", &verticalSync);

        if (verticalSync) {
            pWindow->setVerticalSync(1);
        }
        else {
            pWindow->setVerticalSync(0);
        }

        if (ImGui::Button("Close")) {
            ImGui::End();
            return false;
        }

        ImGui::End();
        return true;
    }

    void setDefaultMAREngineDarkTheme() {
        ImGuiIO& io = ImGui::GetIO();
        io.Fonts->AddFontFromFileTTF("resources/fonts/Ruda-Bold.ttf", 15.0f);

        ImGuiStyle* style = &ImGui::GetStyle();

        style->WindowPadding = ImVec2(15, 15);
        style->WindowRounding = 5.0f;
        style->FramePadding = ImVec2(5, 5);
        style->FrameRounding = 4.0f;
        style->ItemSpacing = ImVec2(12, 8);
        style->ItemInnerSpacing = ImVec2(8, 6);
        style->IndentSpacing = 25.0f;
        style->ScrollbarSize = 15.0f;
        style->ScrollbarRounding = 9.0f;
        style->GrabMinSize = 5.0f;
        style->GrabRounding = 3.0f;

        style->Colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.83f, 1.00f);
        style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
        style->Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
        style->Colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
        style->Colors[ImGuiCol_Border] = ImVec4(0.80f, 0.80f, 0.83f, 0.88f);
        style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
        style->Colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
        style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
        style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
        style->Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
        style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 0.98f, 0.95f, 0.75f);
        style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
        style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
        style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
        style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
        style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
        style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
        style->Colors[ImGuiCol_CheckMark] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
        style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
        style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
        style->Colors[ImGuiCol_Button] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
        style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
        style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
        style->Colors[ImGuiCol_Header] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
        style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
        style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
        style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
        style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
        style->Colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
        style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
        style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
        style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
        style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);
    }


}
