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
#include "../Filesystem/EditorFilesystem.h"
#include "../../Core/ecs/Components/ScriptingComponents.h"


namespace mar {
	namespace editor {


		GUI_TextEditor GUI_TextEditor::s_Instance;
		std::string GUI_TextEditor::DEFAULT_SCRIPT{
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
			"\n"
		};

		GUI_TextEditor::GUI_TextEditor()
			: m_createNewScriptWindow(false),
			m_openScriptWindow(false)
		{
			startup();
		}

		void GUI_TextEditor::startup() {
			auto lang = TextEditorLanguageDefinition_Python();
			editor.SetLanguageDefinition(lang);
		}

		void GUI_TextEditor::update() {
			static bool window_focused = false;

			ImGui::Begin("Script Editor", nullptr, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_MenuBar);

			if (ImGui::IsWindowFocused())
				window_focused = true;
			else {
				if (window_focused) {
					auto textToSave = editor.GetText();
					Filesystem::savePyScript(m_pathToSave.c_str(), textToSave);
				}

				window_focused = false;
			}

			TextEditor_MenuBar();
			
			if (m_createNewScriptWindow) TextEditor_CreateNewScriptWindow();
			if (m_openScriptWindow) TextEditor_OpenScriptWindow();

			TextEditor_Render();

			ImGui::End();
		}

		void GUI_TextEditor::TextEditor_Render() {
			auto cpos = editor.GetCursorPosition();
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

		void GUI_TextEditor::TextEditor_MenuBar() {
			if (ImGui::BeginMenuBar()) {
				if (ImGui::BeginMenu("File")) {
					if (ImGui::MenuItem("New", "Ctrl-N")) {
						m_createNewScriptWindow = true;
					}

					if (ImGui::MenuItem("Open", "Ctrl-O")) {
						m_openScriptWindow = true;
					}

					if (ImGui::MenuItem("Save", "Ctrl-S")) {
						auto textToSave = editor.GetText();
						Filesystem::savePyScript(m_pathToSave.c_str(), textToSave);
					}

					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu("Edit")) {
					bool ro = editor.IsReadOnly();
					if (ImGui::MenuItem("Read-only mode", nullptr, &ro))
						editor.SetReadOnly(ro);
					ImGui::Separator();

					if (ImGui::MenuItem("Undo", "ALT-Backspace", nullptr, !ro && editor.CanUndo()))
						editor.Undo();
					if (ImGui::MenuItem("Redo", "Ctrl-Y", nullptr, !ro && editor.CanRedo()))
						editor.Redo();

					ImGui::Separator();

					if (ImGui::MenuItem("Copy", "Ctrl-C", nullptr, editor.HasSelection()))
						editor.Copy();
					if (ImGui::MenuItem("Cut", "Ctrl-X", nullptr, !ro && editor.HasSelection()))
						editor.Cut();
					if (ImGui::MenuItem("Delete", "Del", nullptr, !ro && editor.HasSelection()))
						editor.Delete();
					if (ImGui::MenuItem("Paste", "Ctrl-V", nullptr, !ro && ImGui::GetClipboardText() != nullptr))
						editor.Paste();

					ImGui::Separator();

					if (ImGui::MenuItem("Select all", nullptr, nullptr))
						editor.SetSelection(TextEditor::Coordinates(), TextEditor::Coordinates(editor.GetTotalLines(), 0));

					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("View")) {
					if (ImGui::MenuItem("Dark palette"))
						editor.SetPalette(TextEditor::GetDarkPalette());
					if (ImGui::MenuItem("Light palette"))
						editor.SetPalette(TextEditor::GetLightPalette());
					if (ImGui::MenuItem("Retro blue palette"))
						editor.SetPalette(TextEditor::GetRetroBluePalette());
					ImGui::EndMenu();
				}
				ImGui::EndMenuBar();
			}
		}
		
		void GUI_TextEditor::TextEditor_CreateNewScriptWindow() {
			ImGui::Begin("Create New Script Menu");

			static char filename[30]{ "empty" };

			ImGui::InputText(".py", filename, 30);

			ImGui::Separator();

			static std::string save;
			save = "resources/scripts/" + std::string(filename) + ".py";

			ImGui::Text("Creating to: ");
			ImGui::SameLine();
			ImGui::Text(save.c_str());

			ImGui::Separator();

			if (ImGui::Button("Create")) {
				std::string s = DEFAULT_SCRIPT;
				std::string repl = scripting::PythonScript::getModuleFromPath(save);
				ReplaceStringInPlace(s, "<put here name>", repl);
				Filesystem::savePyScript(save.c_str(), s);
				setEditorTitle(save.c_str());
				setPathToSave(save);
				setEditorText(s);
				m_createNewScriptWindow = false;
			}

			ImGui::SameLine();

			if (ImGui::Button("Close")) {
				m_createNewScriptWindow = false;
			}
				
			ImGui::End();
		}

		void GUI_TextEditor::TextEditor_OpenScriptWindow() {
			ImGui::Begin("Open Script Menu");

			static char filename[30]{ "empty" };

			ImGui::InputText(".py", filename, 30);

			ImGui::Separator();

			static std::string load;
			load = "resources/scripts/" + std::string(filename) + ".py";

			ImGui::Text("Opening: ");
			ImGui::SameLine();
			ImGui::Text(load.c_str());

			ImGui::Separator();

			if (ImGui::Button("Open")) {
				std::string s = Filesystem::loadPyScript(load.c_str());
				setEditorTitle(load.c_str());
				setPathToSave(load);
				setEditorText(s);
				m_openScriptWindow = false;
			}

			ImGui::SameLine();

			if (ImGui::Button("Close")) {
				m_openScriptWindow = false;
			}

			ImGui::End();
		}

		void GUI_TextEditor::ReplaceStringInPlace(std::string& subject, const std::string& search,
			const std::string& replace)
		{
			size_t pos = 0;
			while ((pos = subject.find(search, pos)) != std::string::npos) {
				subject.replace(pos, search.length(), replace);
				pos += replace.length();
			}
		}

		void GUI_TextEditor::createNewFile(std::string scriptPath) {
			// TODO: make sure that scriptPath is a PythonScript ex. resources/Script.py

			std::string s = DEFAULT_SCRIPT;
			std::string repl = scripting::PythonScript::getModuleFromPath(scriptPath);
			ReplaceStringInPlace(s, "<put here name>", repl);

			setEditorTitle(scriptPath.c_str());
			setPathToSave(scriptPath);
			setEditorText(s);
		}

		const TextEditor::LanguageDefinition& GUI_TextEditor::TextEditorLanguageDefinition_Python() {
			static bool inited = false;
			static TextEditor::LanguageDefinition langDef;
			if (!inited)
			{
				static const char* const keywords[] = {
					"False", "None", "True", "and", "as", "assert", "async", "await", "break",
					"class", "continue", "def", "del", "elif", "else", "except", "finally", "for", "from", "or", "global", "if", "import",
					"in", "is", "lambda", "nonlocal", "not", "pass", "raise", "return", "try", "while", "with", "yield"
				};

				for (auto& k : keywords)
					langDef.mKeywords.insert(k);

				static const char* const identifiers[] = {
					"transform", "center", "angles", "scale", "general_scale",
					"light", "camera", "color", "trig", "vec3", "vec4", "mat4",
					"sine", "cosine", "tangent", "toRadians", "self", "toDegrees", "mar",
					"p_fov", "p_aspectRatio", "p_near",	"p_far", "o_left", "o_right", "o_top",
					"o_bottom", "o_near", "o_far", "ambient","diffuse",	"specular",	"constant",
					"linear","quadratic","shininess","texture"
				};

				for (auto& k : identifiers)
				{
					TextEditor::Identifier id;
					id.mDeclaration = "Built-in function";
					langDef.mIdentifiers.insert(std::make_pair(std::string(k), id));
				}

				langDef.mTokenRegexStrings.push_back(std::make_pair<std::string, TextEditor::PaletteIndex>("[ \\t]*#[ \\t]*[a-zA-Z_]+", TextEditor::PaletteIndex::Preprocessor));
				langDef.mTokenRegexStrings.push_back(std::make_pair<std::string, TextEditor::PaletteIndex>("L?\\\"(\\\\.|[^\\\"])*\\\"", TextEditor::PaletteIndex::String));
				langDef.mTokenRegexStrings.push_back(std::make_pair<std::string, TextEditor::PaletteIndex>("\\'\\\\?[^\\']\\'", TextEditor::PaletteIndex::CharLiteral));
				langDef.mTokenRegexStrings.push_back(std::make_pair<std::string, TextEditor::PaletteIndex>("[+-]?([0-9]+([.][0-9]*)?|[.][0-9]+)([eE][+-]?[0-9]+)?[fF]?", TextEditor::PaletteIndex::Number));
				langDef.mTokenRegexStrings.push_back(std::make_pair<std::string, TextEditor::PaletteIndex>("[+-]?[0-9]+[Uu]?[lL]?[lL]?", TextEditor::PaletteIndex::Number));
				langDef.mTokenRegexStrings.push_back(std::make_pair<std::string, TextEditor::PaletteIndex>("0[0-7]+[Uu]?[lL]?[lL]?", TextEditor::PaletteIndex::Number));
				langDef.mTokenRegexStrings.push_back(std::make_pair<std::string, TextEditor::PaletteIndex>("0[xX][0-9a-fA-F]+[uU]?[lL]?[lL]?", TextEditor::PaletteIndex::Number));
				langDef.mTokenRegexStrings.push_back(std::make_pair<std::string, TextEditor::PaletteIndex>("[a-zA-Z_][a-zA-Z0-9_]*", TextEditor::PaletteIndex::Identifier));
				langDef.mTokenRegexStrings.push_back(std::make_pair<std::string, TextEditor::PaletteIndex>("[\\[\\]\\{\\}\\!\\%\\^\\&\\*\\(\\)\\-\\+\\=\\~\\|\\<\\>\\?\\/\\;\\,\\.]", TextEditor::PaletteIndex::Punctuation));

				langDef.mCommentStart = "/*";
				langDef.mCommentEnd = "*/";
				langDef.mSingleLineComment = "//";

				langDef.mCaseSensitive = true;
				langDef.mAutoIndentation = true;

				langDef.mName = "Python";

				inited = true;
			}

			return langDef;
		}

} }