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


#include "../../../../mar.h"
#include "../../../../Window/Window.h"
#include "MainMenuBarImGuiWidget.h"
#include "FilesystemPopUpImGuiWidget.h"


namespace marengine {


    static void setDefaultMAREngineDarkTheme();
    static bool displayInfoAboutEngineAuthor();
    static bool displayEngineInstructions();
    static bool displayWindowSettings();


    void FMainMenuBarImGuiWidget::create(FFilesystemPopUpImGuiWidget* pFilesystem) {
        m_pFilesystem = pFilesystem;
        setDefaultMAREngineDarkTheme();
    }

    void FMainMenuBarImGuiWidget::updateFrame() {
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
            m_windowSettingsDisplay = displayWindowSettings();
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

        constexpr auto newSceneCallback = [](const std::string& path, const std::string& filename) {
            //Scene* newScene = Scene::createEmptyScene(filename);
            //FSceneSerializer::saveSceneToFile(path.c_str(), newScene);
            //delete newScene;

            //ProjectManager::Instance->addNewSceneToCurrentProject(filename);

            //WEntityWidgetPanel::Instance->reset();
            //WScriptIDE::Instance->reset();

            //MAREngine::Instance->setRestart();
        };

        constexpr auto saveSceneCallback = [](const std::string& path, const std::string& filename) {
            //FSceneSerializer::saveSceneToFile(path.c_str(), FSceneManagerEditor::Instance->getScene());
        };

        constexpr auto openSceneCallback = [](const std::string& path, const std::string& filename) {
            //ProjectManager::Instance->setNewSceneToLoad(filename);

            //WEntityWidgetPanel::Instance->reset();
            //WScriptIDE::Instance->reset();

            //MAREngine::Instance->setRestart();
        };

        m_pFilesystem->displaySaveWidget(newSceneName, extMarscene, newSceneCallback);
        m_pFilesystem->displayOpenWidget(openSceneName, extMarscene, openSceneCallback);
        m_pFilesystem->displaySaveWidget(saveSceneName, extMarscene, saveSceneCallback);
    }

    void FMainMenuBarImGuiWidget::displaySceneManagementTab() {
        // TODO: add scene filesystem management options to main menu bar
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
                Window::endRenderLoop();
            }

            ImGui::EndMenu();
        }
    }

    void FMainMenuBarImGuiWidget::displayEntitiesManagementTab() {
        // TODO: add entities management options to main menu bar
        if (ImGui::BeginMenu("Entities")) {
            if (ImGui::MenuItem("Load external .obj file")) { }
            if (ImGui::MenuItem("Copy selected entity")) { }

            ImGui::EndMenu();
        }
    }

    void FMainMenuBarImGuiWidget::displaySettingsTab() {
        // TODO: add settings menu options
        if (ImGui::BeginMenu("Settings")) {
            if (ImGui::MenuItem("Window Settings")) { m_windowSettingsDisplay = true; }

            ImGui::EndMenu();
        }
    }

    void FMainMenuBarImGuiWidget::displayAboutTab() {
        if (ImGui::BeginMenu("About")) {
            if (ImGui::MenuItem("About Engine")) { m_infoAboutAuthorDisplay = true; }
            if (ImGui::MenuItem("Instruction")) { m_infoAboutEngineDisplay = true; }

            ImGui::EndMenu();
        }
    }

    static bool displayInfoAboutEngineAuthor() {
        ImGui::Begin("About");

        const char* aboutEngine =
                "MAREngine is an educational 3D game engine written in modern C++.\n"
                "It is created with a dream of easy and intuitive interface for everyone.\n"
                "You can download MAREngine completely freely and release your creativity - create games you imagine.";

        const char* aboutAuthor =
                "Mateusz Rzeczyca is C++ / Python programmer and enthusiast of Augmented Reality and Machine Learning.\n"
                "\n"
                "I am delighted that you are using MAREngine! Thank you!";

        ImGui::Text("About Engine");
        ImGui::Text(aboutEngine);

        ImGui::Separator();

        ImGui::Text("About Author");
        ImGui::Text(aboutAuthor);

        if (ImGui::Button("Close")) {
            ImGui::End();
            return false;
        }

        ImGui::End();
        return true;
    }

    static bool displayEngineInstructions() {
        ImGui::Begin("Instructions");

        if (ImGui::Button("Close")) {
            ImGui::End();
            return false;
        }

        ImGui::End();
        return true;
    }

    static bool displayWindowSettings() {
        ImGui::Begin("Window Settings");

        static bool verticalSync{ true };
        ImGui::Checkbox("Window Vertical Synchronization (when turned off increases FPS)", &verticalSync);

        if (verticalSync) {
            Window::setVerticalSync(1);
        }
        else {
            Window::setVerticalSync(0);
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
