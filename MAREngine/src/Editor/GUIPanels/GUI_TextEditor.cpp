/**
 *           MAREngine - open source 3D game engine
 * Copyright (C) 2020-present Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 * All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
**/


#include "GUI_TextEditor.h"
#include "../../Core/filesystem/EditorFilesystem.h"
#include "../../Core/ecs/Components/ScriptingComponents.h"
#include "../../Engine.h"


namespace marengine {


	GUI_TextEditor* GUI_TextEditor::s_instance{ nullptr };
	const std::string GUI_TextEditor::defaultTitle{ "EmptyModule" };
	const std::string GUI_TextEditor::defaultScript =
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


	void GUI_TextEditor::initialize() {
		s_instance = this;

		definePythonLanguage();
		editor.SetLanguageDefinition(m_languageDefinition);
		
		reset();
	}

	void GUI_TextEditor::update() {
		ImGui::Begin("Script Editor", nullptr, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_MenuBar);

		displayMainMenuBar();
		
		if (m_createNewScriptWindow) { createNewScriptWindow(); }
		if (m_openScriptWindow) { openScriptWindow(); }

		editorRender();

		ImGui::End();
	}

	void GUI_TextEditor::reset() {
		setEditorText(defaultScript);
		setEditorTitle(defaultTitle);
	}

	void GUI_TextEditor::editorRender() {
		const auto cpos = editor.GetCursorPosition();

		ImGui::Text("%6d/%-6d %6d lines  | %s | %s | %s | %s",
			cpos.mLine + 1,
			cpos.mColumn + 1,
			editor.GetTotalLines(),
			editor.IsOverwrite() ? "Ovr" : "Ins",
			editor.CanUndo() ? "*" : " ",
			editor.GetLanguageDefinition().mName.c_str(),
			m_title.c_str()
		);

		editor.Render(m_title.c_str());
	}

	void GUI_TextEditor::displayMainMenuBar() {
		if (ImGui::BeginMenuBar()) {
			if (ImGui::BeginMenu("File")) {
				if (ImGui::MenuItem("New", "Ctrl-N")) { m_createNewScriptWindow = true; }
				if (ImGui::MenuItem("Open", "Ctrl-O")) { m_openScriptWindow = true; }
				if (ImGui::MenuItem("Save", "Ctrl-S")) { Filesystem::savePyScript(m_pathToSave.c_str(), editor.GetText()); }

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Edit")) {
				const bool ro = editor.IsReadOnly();

				if (ImGui::MenuItem("Read-only mode", nullptr, &ro)) { editor.SetReadOnly(ro); }
					
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
	
	void GUI_TextEditor::createNewScriptWindow() {
		ImGui::Begin("Create New Script");

		const auto& assetsPath = MAREngine::Instance()->getAssetsPath();
		static char moduleName[50]{ "empty" };

		ImGui::InputText(".py", moduleName, 50);

		const auto moduleNameStr = std::string(moduleName);
		const auto savePath = assetsPath + moduleNameStr + ".py";

		ImGui::Separator();

		ImGui::Text("Creating to: %s", savePath.c_str());

		ImGui::Separator();

		if (ImGui::Button("Create")) { createNewFile(savePath, moduleNameStr); }

		ImGui::SameLine();

		if (ImGui::Button("Close")) { m_createNewScriptWindow = false; }
			
		ImGui::End();
	}

	void GUI_TextEditor::openScriptWindow() {
		ImGui::Begin("Open Script Menu");

		const auto& assetsPath = MAREngine::Instance()->getAssetsPath();
		static char moduleName[50]{ "empty" };

		ImGui::InputText(".py", moduleName, 30);

		ImGui::Separator();

		const auto moduleNameStr = std::string(moduleName);
		const auto loadPath = assetsPath + moduleNameStr + ".py";

		ImGui::Text("Opening: %s", loadPath.c_str());

		ImGui::Separator();

		if (ImGui::Button("Open")) { openFile(loadPath, moduleNameStr); }

		ImGui::SameLine();

		if (ImGui::Button("Close")) { m_openScriptWindow = false; }

		ImGui::End();
	}

	std::string GUI_TextEditor::replaceOcurrences(std::string str, const std::string& from, const std::string& to) {
		size_t start_pos = 0;

		while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
			str.replace(start_pos, from.length(), to);
			start_pos += to.length();
		}

		return str;
	}

	void GUI_TextEditor::createNewFile(const std::string& scriptPath, const std::string& moduleName) {
		const std::string toReplace = "<put here name>";

		const auto scriptSource = replaceOcurrences(defaultScript, toReplace, moduleName);
		Filesystem::savePyScript(scriptPath.c_str(), scriptSource);

		setEditorTitle(moduleName);
		setEditorText(scriptSource);
		setPathToSave(scriptPath);
		
		m_createNewScriptWindow = false;
	}

	void GUI_TextEditor::openFile(const std::string& scriptPath, const std::string& moduleName) {
		const auto source = Filesystem::loadPyScript(scriptPath.c_str());
		
		setEditorTitle(moduleName);
		setPathToSave(scriptPath);
		setEditorText(source);

		m_openScriptWindow = false;
	}

	void GUI_TextEditor::setPathToSave(std::string s) {
		m_pathToSave = std::move(s); 
	}

	void GUI_TextEditor::setEditorText(std::string s) {
		editor.SetText(std::move(s)); 
	}

	void GUI_TextEditor::setEditorTitle(std::string new_title) {
		m_title = std::move(new_title); 
	}

	void GUI_TextEditor::definePythonLanguage() {
		const char* const keywords[] = {
			"False", "None", "True", "and", "as", "assert", "async", "await", "break",
			"class", "continue", "def", "del", "elif", "else", "except", "finally", "for", "from", "or", "global", "if", "import",
			"in", "is", "lambda", "nonlocal", "not", "pass", "raise", "return", "try", "while", "with", "yield"
		};

		for (const auto& k : keywords) { m_languageDefinition.mKeywords.insert(k); }

		const char* const identifiers[] = {
			"transform", "center", "angles", "scale", "general_scale",
			"light", "camera", "color", "trig", "vec3", "vec4", "mat4",
			"sine", "cosine", "tangent", "toRadians", "self", "toDegrees", "mar",
			"p_fov", "p_aspectRatio", "p_near",	"p_far", "o_left", "o_right", "o_top",
			"o_bottom", "o_near", "o_far", "ambient","diffuse",	"specular",	"constant",
			"linear","quadratic","shininess","texture"
		};

		for (const auto& k : identifiers) {
			TextEditor::Identifier id;
			id.mDeclaration = "Built-in function";
			m_languageDefinition.mIdentifiers.insert(std::make_pair(std::string(k), id));
		}

		m_languageDefinition.mTokenRegexStrings.push_back(std::make_pair<std::string, TextEditor::PaletteIndex>("[ \\t]*#[ \\t]*[a-zA-Z_]+", TextEditor::PaletteIndex::Preprocessor));
		m_languageDefinition.mTokenRegexStrings.push_back(std::make_pair<std::string, TextEditor::PaletteIndex>("L?\\\"(\\\\.|[^\\\"])*\\\"", TextEditor::PaletteIndex::String));
		m_languageDefinition.mTokenRegexStrings.push_back(std::make_pair<std::string, TextEditor::PaletteIndex>("\\'\\\\?[^\\']\\'", TextEditor::PaletteIndex::CharLiteral));
		m_languageDefinition.mTokenRegexStrings.push_back(std::make_pair<std::string, TextEditor::PaletteIndex>("[+-]?([0-9]+([.][0-9]*)?|[.][0-9]+)([eE][+-]?[0-9]+)?[fF]?", TextEditor::PaletteIndex::Number));
		m_languageDefinition.mTokenRegexStrings.push_back(std::make_pair<std::string, TextEditor::PaletteIndex>("[+-]?[0-9]+[Uu]?[lL]?[lL]?", TextEditor::PaletteIndex::Number));
		m_languageDefinition.mTokenRegexStrings.push_back(std::make_pair<std::string, TextEditor::PaletteIndex>("0[0-7]+[Uu]?[lL]?[lL]?", TextEditor::PaletteIndex::Number));
		m_languageDefinition.mTokenRegexStrings.push_back(std::make_pair<std::string, TextEditor::PaletteIndex>("0[xX][0-9a-fA-F]+[uU]?[lL]?[lL]?", TextEditor::PaletteIndex::Number));
		m_languageDefinition.mTokenRegexStrings.push_back(std::make_pair<std::string, TextEditor::PaletteIndex>("[a-zA-Z_][a-zA-Z0-9_]*", TextEditor::PaletteIndex::Identifier));
		m_languageDefinition.mTokenRegexStrings.push_back(std::make_pair<std::string, TextEditor::PaletteIndex>("[\\[\\]\\{\\}\\!\\%\\^\\&\\*\\(\\)\\-\\+\\=\\~\\|\\<\\>\\?\\/\\;\\,\\.]", TextEditor::PaletteIndex::Punctuation));

		m_languageDefinition.mCommentStart = "/*";
		m_languageDefinition.mCommentEnd = "*/";
		m_languageDefinition.mSingleLineComment = "//";

		m_languageDefinition.mCaseSensitive = true;
		m_languageDefinition.mAutoIndentation = true;

		m_languageDefinition.mName = "Python";
	}


}
