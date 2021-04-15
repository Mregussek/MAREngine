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


#include "ScriptWidgetImGui.h"
#include "../ImGuiEditorServiceLocator.h"


namespace marengine {

    static void definePythonLanguage(TextEditor::LanguageDefinition& langDefinition);


    void FScriptWidgetImGui::create(FImGuiEditorServiceLocator* serviceLocator) {
        definePythonLanguage(m_languageDefinition);
        editor.SetLanguageDefinition(m_languageDefinition);

        reset();
    }

    void FScriptWidgetImGui::reset() {
        editor.SetText(getDefaultEditorSourceCode());
        setEditorTitle("");
    }

    void FScriptWidgetImGui::updateFrame() {
        ImGui::Begin("Script Editor", nullptr, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_MenuBar);

        displayMainMenuBar();
        editorRender();

        ImGui::End();
    }

    void FScriptWidgetImGui::editorRender() {
        const auto cursorPosition{ editor.GetCursorPosition() };

        ImGui::Text("%6d/%-6d %6d lines  | %s | %s | %s | %s",
                    cursorPosition.mLine + 1,
                    cursorPosition.mColumn + 1,
                    editor.GetTotalLines(),
                    editor.IsOverwrite() ? "Ovr" : "Ins",
                    editor.CanUndo() ? "*" : " ",
                    editor.GetLanguageDefinition().mName.c_str(),
                    m_title.c_str()
        );

        editor.Render(m_title.c_str());
    }

    void FScriptWidgetImGui::displayMainMenuBar() {
        if (ImGui::BeginMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("New", "Ctrl-N")) {
                    // TODO: Add "Create New script" option
                    //WScriptIDEFilesystemWidgets::Instance->openCreateNewEditorScriptWidget();
                }
                if (ImGui::MenuItem("Open", "Ctrl-O")) {
                    // TODO: Add "Open script" option
                }
                if (ImGui::MenuItem("Save", "Ctrl-S")) {
                    // TODO: Add "Save script" option
                    //FFileManager::saveAsFile(editor.GetText(), m_pathToScript.c_str());
                }
                if (ImGui::MenuItem("Save as")) {
                    // TODO: Add "Save as" script option
                    //WScriptIDEFilesystemWidgets::Instance->openSaveAsEditorScriptWidget();
                }

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Edit")) {
                const bool ro{ editor.IsReadOnly() };

                if (ImGui::MenuItem("Read-only mode", nullptr, ro)) { editor.SetReadOnly(ro); }

                ImGui::Separator();

                if (ImGui::MenuItem("Undo", "ALT-Backspace", nullptr, !ro && editor.CanUndo())) { editor.Undo(); }
                if (ImGui::MenuItem("Redo", "Ctrl-Y", nullptr, !ro && editor.CanRedo())) { editor.Redo(); }

                ImGui::Separator();

                if (ImGui::MenuItem("Copy", "Ctrl-C", nullptr, editor.HasSelection())) { editor.Copy(); }
                if (ImGui::MenuItem("Cut", "Ctrl-X", nullptr, !ro && editor.HasSelection())) { editor.Cut(); }
                if (ImGui::MenuItem("Delete", "Del", nullptr, !ro && editor.HasSelection())) { editor.Delete(); }
                if (ImGui::MenuItem("Paste", "Ctrl-V", nullptr, !ro && ImGui::GetClipboardText() != nullptr)) { editor.Paste(); }

                ImGui::Separator();

                if (ImGui::MenuItem("Select all", nullptr, nullptr)) {
                    editor.SetSelection(TextEditor::Coordinates(), TextEditor::Coordinates(editor.GetTotalLines(), 0));
                }

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("View")) {
                if (ImGui::MenuItem("Dark palette")) { editor.SetPalette(TextEditor::GetDarkPalette()); }
                if (ImGui::MenuItem("Light palette")) { editor.SetPalette(TextEditor::GetLightPalette()); }
                if (ImGui::MenuItem("Retro blue palette")) { editor.SetPalette(TextEditor::GetRetroBluePalette()); }

                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }
    }

    void FScriptWidgetImGui::setEditorTitle(std::string newTitle) {
        m_title = std::move(newTitle);
    }

    void FScriptWidgetImGui::setEditorCode(const std::string& sourceCode) {
        editor.SetText(sourceCode);
    }

    void FScriptWidgetImGui::setPathToScript(std::string pathToScript) {
        m_pathToScript = std::move(pathToScript);
    }

    std::string FScriptWidgetImGui::getEditorSourceCode() const {
        return editor.GetText();
    }

    std::string FScriptWidgetImGui::getDefaultEditorSourceCode() {
        static const std::string defaultScript =
                "import MAREnginePy as mar\n"
                "\n"
                "class <put here name>(mar.Entity):\n"
                "\tdef __init__(self):\n"
                "\t\tmar.Entity.__init__(self)\n"
                "\n"
                "\tdef start(self):\n"
                "\t\tpass\n"
                "\n"
                "\tdef update(self):\n"
                "\t\tpass\n"
                "\n";
        return defaultScript;
    }

    std::string FScriptWidgetImGui::getDefaultEditorTitle() {
        static const std::string defaultTitle{ "EmptyModule" };
        return defaultTitle;
    }

    bool FScriptWidgetImGui::isEditorCurrentlyUsed() const {
        return m_title.empty() && editor.GetText().empty();
    }


    void definePythonLanguage(TextEditor::LanguageDefinition& langDefinition) {
        const std::array<std::string, 35> keywords{
                "False", "None", "True", "and", "as", "assert", "async", "await", "break",
                "class", "continue", "def", "del", "elif", "else", "except", "finally", "for",
                "from", "or", "global", "if", "import", "in", "is", "lambda", "nonlocal", "not",
                "pass", "raise", "return", "try", "while", "with", "yield"
        };
        const std::array<std::string, 37> identifiers {
            "transform", "center", "angles", "scale", "general_scale",
            "light", "camera", "color", "trig", "vec3", "vec4", "mat4",
            "sine", "cosine", "tangent", "toRadians", "self", "toDegrees", "mar",
            "p_fov", "p_aspectRatio", "p_near",	"p_far", "o_left", "o_right", "o_top",
            "o_bottom", "o_near", "o_far", "ambient","diffuse",	"specular",	"constant",
            "linear","quadratic","shininess","texture"
        };

        for(const std::string& keyword : keywords) {
            langDefinition.mKeywords.insert(keyword);
        }

        for (const std::string& identifier : identifiers) {
            TextEditor::Identifier id;
            id.mDeclaration = "Built-in function";
            langDefinition.mIdentifiers.insert(std::make_pair(identifier, id));
        }

        typedef TextEditor::PaletteIndex PalIndex;
        auto& tokenRegexString{ langDefinition.mTokenRegexStrings };

        tokenRegexString.emplace_back("[ \\t]*#[ \\t]*[a-zA-Z_]+", PalIndex::Preprocessor);
        tokenRegexString.emplace_back(R"(L?\"(\\.|[^\"])*\")", PalIndex::String);
        tokenRegexString.emplace_back(R"(\'\\?[^\']\')", PalIndex::CharLiteral);
        tokenRegexString.emplace_back("[+-]?([0-9]+([.][0-9]*)?|[.][0-9]+)([eE][+-]?[0-9]+)?[fF]?", PalIndex::Number);
        tokenRegexString.emplace_back("[+-]?[0-9]+[Uu]?[lL]?[lL]?", PalIndex::Number);
        tokenRegexString.emplace_back("0[0-7]+[Uu]?[lL]?[lL]?", PalIndex::Number);
        tokenRegexString.emplace_back("0[xX][0-9a-fA-F]+[uU]?[lL]?[lL]?", PalIndex::Number);
        tokenRegexString.emplace_back("[a-zA-Z_][a-zA-Z0-9_]*", PalIndex::Identifier);
        tokenRegexString.emplace_back(R"(([\[\]\{\}\!\%\^\&\*\(\)\-\+\=\~\|\<\>\?\/\;\,\.]))", PalIndex::Punctuation);

        langDefinition.mCommentStart = "/*";
        langDefinition.mCommentEnd = "*/";
        langDefinition.mSingleLineComment = "//";

        langDefinition.mCaseSensitive = true;
        langDefinition.mAutoIndentation = true;

        langDefinition.mName = "Python";
    }


}