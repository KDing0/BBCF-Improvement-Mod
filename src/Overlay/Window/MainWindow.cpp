#include "MainWindow.h"

//#include "CBR/CharacterStorage.h"
#include "HitboxOverlay.h"
#include "PaletteEditorWindow.h"

#include "Core/Settings.h"
#include "Core/info.h"
#include "Core/interfaces.h"
#include "Game/gamestates.h"
#include "Overlay/imgui_utils.h"
#include "Overlay/Widget/ActiveGameModeWidget.h"
#include "Overlay/Widget/GameModeSelectWidget.h"
#include "Overlay/Widget/StageSelectWidget.h"
#include "Overlay/NotificationBar/NotificationBar.h"

#include <fstream>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/filesystem.hpp>
#include "CBR/CharacterStorage.h"

MainWindow::MainWindow(const std::string& windowTitle, bool windowClosable, WindowContainer& windowContainer, ImGuiWindowFlags windowFlags)
	: IWindow(windowTitle, windowClosable, windowFlags), m_pWindowContainer(&windowContainer)
{
	m_windowTitle = MOD_WINDOW_TITLE;
	m_windowTitle += " ";
	m_windowTitle += MOD_VERSION_NUM;

#ifdef _DEBUG
	m_windowTitle += " (DEBUG)";
#endif

	m_windowTitle += "###MainTitle"; // Set unique identifier
}

void MainWindow::BeforeDraw()
{
	ImGui::SetWindowPos(m_windowTitle.c_str(), ImVec2(12, 20), ImGuiCond_FirstUseEver);

	ImVec2 windowSizeConstraints;
	switch (Settings::settingsIni.menusize)
	{
		case 1:
			windowSizeConstraints = ImVec2(250, 190);
			break;
		case 3:
			windowSizeConstraints = ImVec2(400, 230);
			break;
		default:
			windowSizeConstraints = ImVec2(330, 230);
	}

	ImGui::SetNextWindowSizeConstraints(windowSizeConstraints, ImVec2(1000, 1000));
}

void MainWindow::Draw()
{
	DrawFrameDataSection();

	ImGui::Text("Toggle me with %s", Settings::settingsIni.togglebutton.c_str());
	ImGui::Text("Toggle Online with %s", Settings::settingsIni.toggleOnlineButton.c_str());
	ImGui::Text("Toggle HUD with %s", Settings::settingsIni.toggleHUDbutton.c_str());
	ImGui::Separator();

	ImGui::VerticalSpacing(5);

	ImGui::AlignTextToFramePadding();
	ImGui::TextUnformatted("P$"); ImGui::SameLine();
	if (g_gameVals.pGameMoney)
	{
		ImGui::InputInt("##P$", *&g_gameVals.pGameMoney);
	}

	ImGui::VerticalSpacing(5);

	if (ImGui::Button("Online", BTN_SIZE))
	{
		m_pWindowContainer->GetWindow(WindowType_Room)->ToggleOpen();
	}

	ImGui::VerticalSpacing(5);

	DrawGameplaySettingSection();
	DrawCustomPalettesSection();
	DrawHitboxOverlaySection();
	DrawAvatarSection();
	DrawLoadedSettingsValuesSection();
	DrawCBRAiSection();
	DrawReversalSection();
	DrawNetaSection();
		
	
	DrawUtilButtons();
	
	

	ImGui::VerticalSpacing(5);

	DrawCurrentPlayersCount();
	DrawLinkButtons();
}

void MainWindow::DrawUtilButtons() const
{
#ifdef _DEBUG
	if (ImGui::Button("DEBUG", BTN_SIZE))
	{
		m_pWindowContainer->GetWindow(WindowType_Debug)->ToggleOpen();
	}
#endif

	if (ImGui::Button("Log", BTN_SIZE))
	{
		m_pWindowContainer->GetWindow(WindowType_Log)->ToggleOpen();
	}
}

void MainWindow::DrawCurrentPlayersCount() const
{
	ImGui::Text("Current online players:");
	ImGui::SameLine();

	std::string currentPlayersCount = g_interfaces.pSteamApiHelper ? g_interfaces.pSteamApiHelper->GetCurrentPlayersCountString() : "<No data>";
	ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "%s", currentPlayersCount.c_str());
}

void MainWindow::DrawAvatarSection() const
{
	

	if (!ImGui::CollapsingHeader("Avatar settings"))
		return;

	if (g_gameVals.playerAvatarAddr == NULL && g_gameVals.playerAvatarColAddr == NULL && g_gameVals.playerAvatarAcc1 == NULL && g_gameVals.playerAvatarAcc2 == NULL)
	{
		ImGui::HorizontalSpacing();
		ImGui::TextDisabled("CONNECT TO NETWORK MODE FIRST");
	}
	else
	{
		ImGui::HorizontalSpacing(); ImGui::SliderInt("Avatar", g_gameVals.playerAvatarAddr, 0, 0x2F);
		ImGui::HorizontalSpacing(); ImGui::SliderInt("Color", g_gameVals.playerAvatarColAddr, 0, 0x3);
		ImGui::HorizontalSpacing(); ImGui::SliderByte("Accessory 1", g_gameVals.playerAvatarAcc1, 0, 0xCF);
		ImGui::HorizontalSpacing(); ImGui::SliderByte("Accessory 2", g_gameVals.playerAvatarAcc2, 0, 0xCF);
	}
}

void MainWindow::DrawCustomPalettesSection() const
{
	if (!ImGui::CollapsingHeader("Custom palettes"))
		return;

	if (!isInMatch())
	{
		ImGui::HorizontalSpacing();
		ImGui::TextDisabled("YOU ARE NOT IN MATCH!");
	}
	else
	{
		ImGui::HorizontalSpacing();
		m_pWindowContainer->GetWindow<PaletteEditorWindow>(WindowType_PaletteEditor)->ShowAllPaletteSelections("Main");
	}

	ImGui::VerticalSpacing(15);
	ImGui::HorizontalSpacing();
	m_pWindowContainer->GetWindow<PaletteEditorWindow>(WindowType_PaletteEditor)->ShowReloadAllPalettesButton();

	if (isPaletteEditingEnabledInCurrentState())
	{
		ImGui::HorizontalSpacing();

		if (ImGui::Button("Palette editor"))
			m_pWindowContainer->GetWindow(WindowType_PaletteEditor)->ToggleOpen();
	}
}

void MainWindow::DrawHitboxOverlaySection() const
{
	if (!ImGui::CollapsingHeader("Hitbox overlay"))
		return;

	if (!isHitboxOverlayEnabledInCurrentState())
	{
		ImGui::HorizontalSpacing();
		ImGui::TextDisabled("YOU ARE NOT IN TRAINING, VERSUS, OR REPLAY!");
		return;
	}

	static bool isOpen = false;

	ImGui::HorizontalSpacing();
	if (ImGui::Checkbox("Enable", &isOpen))
	{
		if (isOpen)
		{
			m_pWindowContainer->GetWindow(WindowType_HitboxOverlay)->Open();
		}
		else
		{
			g_gameVals.isFrameFrozen = false;
			m_pWindowContainer->GetWindow(WindowType_HitboxOverlay)->Close();
		}
	}

	if (isOpen)
	{
		ImGui::VerticalSpacing(10);

		if (!g_interfaces.player1.IsCharDataNullPtr() && !g_interfaces.player2.IsCharDataNullPtr())
		{
			ImGui::HorizontalSpacing();
			ImGui::Checkbox("Player1", &m_pWindowContainer->GetWindow<HitboxOverlay>(WindowType_HitboxOverlay)->drawCharacterHitbox[0]);
			ImGui::HoverTooltip(getCharacterNameByIndexA(g_interfaces.player1.GetData()->charIndex).c_str());
			ImGui::SameLine(); ImGui::HorizontalSpacing();
			ImGui::TextUnformatted(g_interfaces.player1.GetData()->currentAction);

			ImGui::HorizontalSpacing();
			ImGui::Checkbox("Player2", &m_pWindowContainer->GetWindow<HitboxOverlay>(WindowType_HitboxOverlay)->drawCharacterHitbox[1]);
			ImGui::HoverTooltip(getCharacterNameByIndexA(g_interfaces.player2.GetData()->charIndex).c_str());
			ImGui::SameLine(); ImGui::HorizontalSpacing();
			ImGui::TextUnformatted(g_interfaces.player2.GetData()->currentAction);
		}

		ImGui::VerticalSpacing(10);

		ImGui::HorizontalSpacing();
		m_pWindowContainer->GetWindow<HitboxOverlay>(WindowType_HitboxOverlay)->DrawRectThicknessSlider();

		ImGui::HorizontalSpacing();
		m_pWindowContainer->GetWindow<HitboxOverlay>(WindowType_HitboxOverlay)->DrawRectFillTransparencySlider();

		ImGui::HorizontalSpacing();
		ImGui::Checkbox("Draw origin",
			&m_pWindowContainer->GetWindow<HitboxOverlay>(WindowType_HitboxOverlay)->drawOriginLine);

		ImGui::VerticalSpacing();

		ImGui::HorizontalSpacing();
		ImGui::Checkbox("Freeze frame:", &g_gameVals.isFrameFrozen);
		if (g_gameVals.pFrameCount)
		{
			ImGui::SameLine();
			ImGui::Text("%d", *g_gameVals.pFrameCount);
			ImGui::SameLine();
			if (ImGui::Button("Reset"))
			{
				*g_gameVals.pFrameCount = 0;
				g_gameVals.framesToReach = 0;
			}
		}

		if (g_gameVals.isFrameFrozen)
		{
			static int framesToStep = 1;
			ImGui::HorizontalSpacing();
			if (ImGui::Button("Step frames"))
			{
				g_gameVals.framesToReach = *g_gameVals.pFrameCount + framesToStep;
			}

			ImGui::SameLine();
			ImGui::SliderInt("", &framesToStep, 1, 60);
		}
	}
}

void MainWindow::DrawGameplaySettingSection() const
{
	if (!ImGui::CollapsingHeader("Gameplay settings"))
		return;

	if (!isInMatch() && !isOnVersusScreen() && !isOnReplayMenuScreen() && !isOnCharacterSelectionScreen())
	{
		ImGui::HorizontalSpacing();
		ImGui::TextDisabled("YOU ARE NOT IN MATCH!");

		ImGui::HorizontalSpacing();
		ImGui::TextDisabled("YOU ARE NOT IN REPLAY MENU!");

		ImGui::HorizontalSpacing();
		ImGui::TextDisabled("YOU ARE NOT IN CHARACTER SELECTION!");

		return;
	}

	if (isStageSelectorEnabledInCurrentState())
	{
		ImGui::HorizontalSpacing();
		StageSelectWidget();
		ImGui::VerticalSpacing(10);
	}

	ImGui::HorizontalSpacing();
	ActiveGameModeWidget();

	if (isGameModeSelectorEnabledInCurrentState())
	{
		bool isThisPlayerSpectator = g_interfaces.pRoomManager->IsRoomFunctional() && g_interfaces.pRoomManager->IsThisPlayerSpectator();

		if (!isThisPlayerSpectator)
		{
			ImGui::HorizontalSpacing();
			GameModeSelectWidget();
		}
	}

	if (isInMatch())
	{
		ImGui::VerticalSpacing(10);
		ImGui::HorizontalSpacing();
		ImGui::Checkbox("Hide HUD", (bool*)g_gameVals.pIsHUDHidden);
	}
}

void MainWindow::DrawLinkButtons() const
{
	if (ImGui::Button("AI Filehost"))
	{
		m_pWindowContainer->GetWindow(WindowType_CbrServer)->ToggleOpen();
	}ImGui::SameLine();
	ImGui::ButtonUrl("Discord", MOD_LINK_DISCORD, BTN_SIZE);

	ImGui::SameLine();
	ImGui::ButtonUrl("Forum", MOD_LINK_FORUM, BTN_SIZE);

	ImGui::SameLine();
	ImGui::ButtonUrl("GitHub", MOD_LINK_GITHUB, BTN_SIZE);
}

void MainWindow::DrawLoadedSettingsValuesSection() const
{
	if (!ImGui::CollapsingHeader("Loaded settings.ini values"))
		return;

	// Not using ImGui columns here because they are bugged if the window has always_autoresize flag. The window 
	// starts extending to infinity, if the left edge of the window touches any edges of the screen

	std::ostringstream oss;

	ImGui::BeginChild("loaded_settings", ImVec2(0, 300.0f), true, ImGuiWindowFlags_HorizontalScrollbar);

	//X-Macro
#define SETTING(_type, _var, _inistring, _defaultval) \
	oss << " " << _inistring; \
	ImGui::TextUnformatted(oss.str().c_str()); ImGui::SameLine(ImGui::GetWindowWidth() * 0.6f); \
	oss.str(""); \
	oss << "= " << Settings::settingsIni.##_var; \
	ImGui::TextUnformatted(oss.str().c_str()); ImGui::Separator(); \
	oss.str("");
#include "Core/settings.def"
#undef SETTING

	ImGui::EndChild();
}

//CBR MOD related -----------------------------------------------------------------------------------------------------------------
void MainWindow::DrawFrameDataSection() const {
	if (!g_interfaces.player1.IsCharDataNullPtr() && !g_interfaces.player2.IsCharDataNullPtr()) {
		static bool p1Recovering = false;
		static bool p2Recovering = false;
		static int p1Adv = 0;
		static int p2Adv = 0;
		static bool gapFound = false;

		//TODO: CmnActLandingStiffLoop == cant act but currently not recognized

		if (g_interfaces.player2.GetData()->blockstun <= 0 && g_interfaces.player1.GetData()->attackLevel <= 0) {
			gapFound = true;
		}

		if (((g_interfaces.player1.GetData()->blockstun > 0) && (g_interfaces.player1.GetData()->hitstop > 0) && (g_interfaces.player1.GetData()->actionTimeNoHitstop == 1)) || ((g_interfaces.player1.GetData()->hitstun > 0) && (g_interfaces.player1.GetData()->hitstop > 0) && (g_interfaces.player1.GetData()->actionTimeNoHitstop == 1))) {
			p1Recovering = true;
			p2Adv = 0;
			p1Adv = 0;
			gapFound = false;
		}
		if (((g_interfaces.player2.GetData()->blockstun > 0) && (g_interfaces.player2.GetData()->hitstop > 0) && (g_interfaces.player2.GetData()->actionTimeNoHitstop == 1)) || ((g_interfaces.player2.GetData()->hitstun > 0) && (g_interfaces.player2.GetData()->hitstop > 0) && (g_interfaces.player2.GetData()->actionTimeNoHitstop == 1))) {
			p2Recovering = true;
			p2Adv = 0;
			p1Adv = 0;
			gapFound = false;
		}
		if (p1Recovering) {
			if (!(g_interfaces.player2.GetData()->typeOfAttack > 0)) {
				if (g_interfaces.player1.GetData()->blockstun > 0) {
					p2Adv = g_interfaces.player1.GetData()->blockstun;
					p1Recovering = false;
				}
				if (g_interfaces.player1.GetData()->hitstun > 0) {
					p2Adv = g_interfaces.player1.GetData()->hitstun;
					p1Recovering = false;
				}
			}

			if (!(g_interfaces.player1.GetData()->hitstun > 0) && !(g_interfaces.player1.GetData()->blockstun > 0) && (g_interfaces.player2.GetData()->typeOfAttack > 0)) {
				p2Adv--;
			}
			if ((g_interfaces.player1.GetData()->hitstun == 0) && (g_interfaces.player1.GetData()->blockstun == 0) && (g_interfaces.player2.GetData()->typeOfAttack == 0)) {
				p1Recovering = false;
			}
		}

		if (p2Recovering) {

			if (g_interfaces.player2.GetData()->blockstun <= 0) {
				gapFound = true;
			}


			if (!(g_interfaces.player1.GetData()->typeOfAttack > 0)) {
				if (g_interfaces.player2.GetData()->blockstun > 0) {
					p1Adv = g_interfaces.player2.GetData()->blockstun;
					p2Recovering = false;
				}
				if (g_interfaces.player2.GetData()->hitstun > 0) {
					p1Adv = g_interfaces.player2.GetData()->hitstun;
					p2Recovering = false;
				}
			}

			if (!(g_interfaces.player2.GetData()->hitstun > 0) && !(g_interfaces.player2.GetData()->blockstun > 0) && (g_interfaces.player1.GetData()->typeOfAttack > 0)) {
				p1Adv--;
			}
			if ((g_interfaces.player2.GetData()->hitstun == 0) && (g_interfaces.player2.GetData()->blockstun == 0) && (g_interfaces.player1.GetData()->typeOfAttack == 0)) {
				p2Recovering = false;
			}
		}
		static float lastProrationPerDmg = 0;
		static float lastProration = 0;
		if (g_interfaces.player2.GetData()->comboProration != 10000) {
			lastProrationPerDmg = float(g_interfaces.player2.GetData()->comboDamage) / (float(10000 - g_interfaces.player2.GetData()->comboProration));
			lastProration = g_interfaces.player2.GetData()->comboProration;
		}
		auto str = "Dmg/Proration: " + std::to_string(lastProrationPerDmg) + " - " + std::to_string(lastProration);
		ImGui::Text(str.c_str());

		//str = "FrameAdv: " + std::to_string(p1Adv) + " - " + std::to_string(p2Adv);
		//ImGui::Text(str.c_str());
		//str = "ComboTime: " + std::to_string(g_interfaces.player2.GetData()->comboTime);
		//ImGui::Text(str.c_str());
		//str = "GapFound: " + std::to_string(gapFound);
		//ImGui::Text(str.c_str());
	}
}

int selected_radio;
int weightsPlayer;
void MainWindow::DrawCBRAiSection() const
{
	if (!ImGui::CollapsingHeader("CBR AI"))
		return;

	g_interfaces.cbrInterface.loadSettings(&g_interfaces.cbrInterface);

	if (g_interfaces.cbrInterface.debugErrorCounter[0] > 0) {
		ImGui::Text("ErrorCountP1: %d", g_interfaces.cbrInterface.debugErrorCounter[0]);
	}
	if (g_interfaces.cbrInterface.debugErrorCounter[1] > 0) {
		ImGui::Text("ErrorCountP2: %d", g_interfaces.cbrInterface.debugErrorCounter[1]);
	}
	if (g_interfaces.cbrInterface.threadActiveCheck()) {
		ImGui::TextDisabled("SAVING OR LOADING CBR DATA. \nPLEASE WAIT.");
		return;
	}
	ImGui::Text("CBR AI Explenation:");
	ImGui::SameLine(); // Move to the same line as the button
	ImGui::SmallButton("?");
	if (ImGui::IsItemHovered())
	{
		ImGui::SetTooltip("CBR AI learns by recording player behaviour and imitating them, essentially letting you generate player clones.\nThis means the AI will only know how to do things that a player demonstrated beforehand.\nThis lets you create smarter AI opponents to play with, or even training dummys with behaviour finetuned for training.\nAn example of this would be a carl AI that only runs blockstrings, which lets you practice blocking carl.\n\nTo create an AI you must record yourself and/or your opponent by playing online, or use the CBR menu in training mode.\nIn training mode set the opponents behaviour to \"Controller\" to let the AI control them.\n\nYou can use instant learning in mirror matches to play against an AI while it learns.\nI recommend setting life to not regenerate to play normal rounds against the AI.\nThis gives the AI more chances to learn neutral, since it will be bad at everything while it has little data.\n\nYou can also download AI data from my server by pressing the AI Filehost button after you logged into the Blazblue network.");
	}
	if (!isInMatch() || !(*g_gameVals.pGameMode == GameMode_Training || *g_gameVals.pGameMode == GameMode_Versus || 
		*g_gameVals.pGameMode == GameMode_Online) )
	{
		ImGui::HorizontalSpacing();
		ImGui::TextDisabled("CBR Menu only accesible in training, versus and online versus");


		if (ImGui::Checkbox("Auto Record Myself", &g_interfaces.cbrInterface.autoRecordGameOwner)) {

			g_interfaces.cbrInterface.saveSettings();
		}
		if (ImGui::Checkbox("Auto Record Opponents", &g_interfaces.cbrInterface.autoRecordAllOtherPlayers)) {
			g_interfaces.cbrInterface.saveSettings();
		}
		if (ImGui::Checkbox("Auto Upload Own Data", &g_interfaces.cbrInterface.autoUploadOwnData)) {
			g_interfaces.cbrInterface.saveSettings();
		}
		if (ImGui::Checkbox("Auto Saving In Lobby", &g_interfaces.cbrInterface.autoRecordConfirmation)) {
			g_interfaces.cbrInterface.saveSettings();
		}

		ImGui::Text("Names for players in versus mode:");
		ImGui::InputText("Player1Name", g_interfaces.cbrInterface.nameVersusP1, IM_ARRAYSIZE(g_interfaces.cbrInterface.nameVersusP1));
		ImGui::InputText("Player2Name", g_interfaces.cbrInterface.nameVersusP2, IM_ARRAYSIZE(g_interfaces.cbrInterface.nameVersusP2));
		
		return;
	}

	if (isInMatch())
	{
		//ImGui::VerticalSpacing(10);
		//ImGui::HorizontalSpacing();
		if (!m_pWindowContainer->GetWindow(WindowType_CbrServer)->IsOpen()) {
			g_interfaces.cbrInterface.windowLoadNr = -1;
		}
		ImGui::Columns(2, "cbr");
		ImGui::Text("Player 1");
		ImGui::NextColumn();
		ImGui::Text("Player 2");
		ImGui::Separator();
		ImGui::NextColumn();
		ImGui::PushID(1);
		ImGui::Text("Replays: %d", g_interfaces.cbrInterface.getCbrData(0)->getReplayCount());
		ImGui::Text("FramesRecorded: %d",g_interfaces.cbrInterface.getAnnotatedReplay(1)->getInputSize());
		ImGui::Text("Input: %d", g_interfaces.cbrInterface.input);
		ImVec2 buttonSize = { 120,20 };
		ImVec2 buttonSizeHalf = { 60,20 };

		if (ImGui::Button("Record", buttonSize))
		{
			if (g_interfaces.cbrInterface.Recording == false) {
				g_interfaces.cbrInterface.EndCbrActivities(0 );
				g_interfaces.cbrInterface.StartCbrRecording(g_interfaces.player1.GetData()->char_abbr, g_interfaces.player2.GetData()->char_abbr, g_interfaces.player1.GetData()->charIndex, g_interfaces.player2.GetData()->charIndex, 0);
			}
			else {
				g_interfaces.cbrInterface.EndCbrActivities(0, true);
			}
		}
		if (!(*g_gameVals.pGameMode == GameMode_Versus)) {
			if (ImGui::Button("Replaying", buttonSize))
			{

				if (g_interfaces.cbrInterface.getCbrData(0)->getReplayCount() > 0 && g_interfaces.cbrInterface.Replaying == false) {
					g_interfaces.cbrInterface.EndCbrActivities(0);
					g_interfaces.cbrInterface.Replaying = true;
				}
				else {
					g_interfaces.cbrInterface.EndCbrActivities(0);
				}
			}

			if (ImGui::IsItemHovered())
			{
				ImGui::SetTooltip("Let the AI take control of player 1s character.\nYou need AI data stored in Slot 1 for this to work.\nClick to turn on and again to turn off.");
			}

		}
		if (!(*g_gameVals.pGameMode == GameMode_Versus) && !g_interfaces.player1.IsCharDataNullPtr() && g_interfaces.player1.GetData()->charIndex == g_interfaces.player2.GetData()->charIndex) {
			if (ImGui::Button("InstantLearning", buttonSize))
			{

				if (g_interfaces.cbrInterface.instantLearning != true) {
					g_interfaces.cbrInterface.EndCbrActivities();
					g_interfaces.cbrInterface.StartCbrInstantLearning(g_interfaces.player1.GetData()->char_abbr, g_interfaces.player2.GetData()->char_abbr, g_interfaces.player1.GetData()->charIndex, g_interfaces.player2.GetData()->charIndex, 0);
				}
				else {
					g_interfaces.cbrInterface.EndCbrActivities();
				}
			}

			if (ImGui::IsItemHovered())
			{
				ImGui::SetTooltip("The AI takes control over player 2 while learning in real time from player 1.\nOnly use if you are player 1.\nStores data in slot 2.\nClick to turn on and again to turn off.");
			}

		}
		if (ImGui::Button("Delete", buttonSizeHalf))
		{
			g_interfaces.cbrInterface.EndCbrActivities();
			g_interfaces.cbrInterface.getCbrData(0)->deleteReplays(g_interfaces.cbrInterface.deletionStart, g_interfaces.cbrInterface.deletionEnd);
		}
		if (ImGui::IsItemHovered())
		{
			ImGui::SetTooltip("Deletes AI data in a certain range from the AI file in slot 1.\nThe range to be deleted can be set in the \"Replay Deletion Range\" entry below.");
		}ImGui::SameLine(0,0);
		if (ImGui::Button("Last", buttonSizeHalf))
		{
			g_interfaces.cbrInterface.EndCbrActivities();
			g_interfaces.cbrInterface.getCbrData(0)->deleteLastReplay();
		}
		if (ImGui::IsItemHovered())
		{
			ImGui::SetTooltip("Deletes the last AI replay that was added.");
		}

		if (ImGui::Button("Save", buttonSize))
		{
			g_interfaces.cbrInterface.EndCbrActivities();
			g_interfaces.cbrInterface.getCbrData(0)->setPlayerName(g_interfaces.cbrInterface.playerName);
			g_interfaces.cbrInterface.getCbrData(0)->setCharName(g_interfaces.player1.GetData()->char_abbr);
			g_interfaces.cbrInterface.SaveCbrDataThreaded(*g_interfaces.cbrInterface.getCbrData(0), true);
		}

		if (ImGui::IsItemHovered())
		{
			ImGui::SetTooltip("Save the AI data stored in slot 1.\nYou can determine under what player name the data is stored by writing a name in the \"Player name: \" textbox");
		}

		if (ImGui::Button("Load", buttonSize)) {
			g_interfaces.cbrInterface.EndCbrActivities();
			m_pWindowContainer->GetWindow(WindowType_CbrServer)->ToggleOpen();
			g_interfaces.cbrInterface.windowLoadNr = 0;
			g_interfaces.cbrInterface.windowReload = true;
			//g_interfaces.cbrInterface.setCbrData(g_interfaces.cbrInterface.LoadCbrData(g_interfaces.cbrInterface.playerName, g_interfaces.player1.GetData()->char_abbr), 0);
		}
		if (ImGui::Button("Load By Name", buttonSize)) {
			g_interfaces.cbrInterface.EndCbrActivities();
			g_interfaces.cbrInterface.LoadCbrData(g_interfaces.cbrInterface.playerName, g_interfaces.player1.GetData()->char_abbr,true, 0);
			//g_interfaces.cbrInterface.setCbrData(g_interfaces.cbrInterface.LoadCbrDataNoThread(g_interfaces.cbrInterface.playerName, g_interfaces.player1.GetData()->char_abbr), 0);
		}

		ImGui::NextColumn();
		ImGui::Text("Replays: %d", g_interfaces.cbrInterface.getCbrData(1)->getReplayCount());
		ImGui::Text("FramesRecorded: %d", g_interfaces.cbrInterface.getAnnotatedReplay(0)->getInputSize());
		ImGui::Text("Input: %d", g_interfaces.cbrInterface.inputP2);
		ImGui::PopID();
		ImGui::PushID(2);
		if (ImGui::Button("Record", buttonSize))
		{

			if (g_interfaces.cbrInterface.RecordingP2 == false) {
				g_interfaces.cbrInterface.EndCbrActivities(1);
				g_interfaces.cbrInterface.StartCbrRecording(g_interfaces.player2.GetData()->char_abbr, g_interfaces.player1.GetData()->char_abbr, g_interfaces.player2.GetData()->charIndex, g_interfaces.player1.GetData()->charIndex, 1);
			}
			else {
				g_interfaces.cbrInterface.EndCbrActivities(1, true);
			}
		}

		if (ImGui::IsItemHovered())
		{
			ImGui::SetTooltip("Records all the actions of player 2,\nwhich the AI will then learn from. \nLearned data is stored in slot 2. \nClick to turn on and again to turn off.");
		}
		if (!(*g_gameVals.pGameMode == GameMode_Versus)) {
			if (ImGui::Button("Replaying", buttonSize))
			{

				if (g_interfaces.cbrInterface.getCbrData(1)->getReplayCount() > 0 && g_interfaces.cbrInterface.ReplayingP2 == false) {
					g_interfaces.cbrInterface.EndCbrActivities(1);
					g_interfaces.cbrInterface.ReplayingP2 = true;
				}
				else {
					g_interfaces.cbrInterface.EndCbrActivities(1);
				}
			}

			if (ImGui::IsItemHovered())
			{
				ImGui::SetTooltip("Let the AI take control of player 1s character.\nYou need AI data stored in Slot 1 for this to work.\nClick to turn on and again to turn off.");
			}

		}
		if (!(*g_gameVals.pGameMode == GameMode_Versus) && !g_interfaces.player1.IsCharDataNullPtr() && g_interfaces.player1.GetData()->charIndex == g_interfaces.player2.GetData()->charIndex) {
			if (ImGui::Button("InstantLearning", buttonSize))
			{

				if (g_interfaces.cbrInterface.instantLearningP2 != true) {
					g_interfaces.cbrInterface.EndCbrActivities();
					g_interfaces.cbrInterface.StartCbrInstantLearning(g_interfaces.player2.GetData()->char_abbr, g_interfaces.player1.GetData()->char_abbr, g_interfaces.player2.GetData()->charIndex, g_interfaces.player1.GetData()->charIndex, 1);
				}
				else {
					g_interfaces.cbrInterface.EndCbrActivities();
				}
			}

			if (ImGui::IsItemHovered())
			{
				ImGui::SetTooltip("The AI takes control over player 1 while learning in real time from player 2.\nOnly use if you are player 2.\nStores data in slot 1.\nClick to turn on and again to turn off.");
			}
		}
		if (ImGui::Button("Delete", buttonSizeHalf))
		{
			g_interfaces.cbrInterface.EndCbrActivities();
			g_interfaces.cbrInterface.getCbrData(1)->deleteReplays(g_interfaces.cbrInterface.deletionStart, g_interfaces.cbrInterface.deletionEnd);
		}
		if (ImGui::IsItemHovered())
		{
			ImGui::SetTooltip("Deletes AI data in a certain range from the AI file in slot 2.\n The range to be deleted can be set in the \"Replay Deletion Range\" entry below.");
		}
		ImGui::SameLine(0, 0);
		if (ImGui::Button("Last", buttonSizeHalf))
		{
			g_interfaces.cbrInterface.EndCbrActivities();
			g_interfaces.cbrInterface.getCbrData(1)->deleteLastReplay();
		}
		if (ImGui::IsItemHovered())
		{
			ImGui::SetTooltip("Deletes the last AI replay that was added.");
		}
		if (ImGui::Button("Save", buttonSize))
		{
			g_interfaces.cbrInterface.EndCbrActivities();
			g_interfaces.cbrInterface.getCbrData(1)->setPlayerName(g_interfaces.cbrInterface.playerName);
			g_interfaces.cbrInterface.getCbrData(1)->setCharName(g_interfaces.player2.GetData()->char_abbr);
			g_interfaces.cbrInterface.SaveCbrDataThreaded(*g_interfaces.cbrInterface.getCbrData(1), true);
		}

		if (ImGui::IsItemHovered())
		{
			ImGui::SetTooltip("Save the AI data stored in slot 2.\nYou can determine under what player name the data is stored by writing a name in the \"Player name: \" textbox");
		}

		if (ImGui::Button("Load", buttonSize)) {
			g_interfaces.cbrInterface.EndCbrActivities();
			m_pWindowContainer->GetWindow(WindowType_CbrServer)->ToggleOpen();
			g_interfaces.cbrInterface.windowLoadNr = 1;
			g_interfaces.cbrInterface.windowReload = true;
			//g_interfaces.cbrInterface.setCbrData(g_interfaces.cbrInterface.LoadCbrData(g_interfaces.cbrInterface.playerName, g_interfaces.player2.GetData()->char_abbr), 1);
		}
		if (ImGui::Button("Load By Name", buttonSize)) {
			g_interfaces.cbrInterface.EndCbrActivities();
			g_interfaces.cbrInterface.LoadCbrData(g_interfaces.cbrInterface.playerName, g_interfaces.player2.GetData()->char_abbr, true, 1);
			//g_interfaces.cbrInterface.setCbrData(g_interfaces.cbrInterface.LoadCbrDataNoThread(g_interfaces.cbrInterface.playerName, g_interfaces.player2.GetData()->char_abbr), 1);
		}

		ImGui::PopID();

		ImGui::Columns(1);
		ImGui::Separator();
		if (!(*g_gameVals.pGameMode == GameMode_Versus)) {
			if (ImGui::Button("Replaying Both"))
			{

				if (g_interfaces.cbrInterface.getCbrData(0)->getReplayCount() > 0 && g_interfaces.cbrInterface.Replaying == false
					&& g_interfaces.cbrInterface.getCbrData(1)->getReplayCount() > 0 && g_interfaces.cbrInterface.ReplayingP2 == false) {
					g_interfaces.cbrInterface.EndCbrActivities();
					g_interfaces.cbrInterface.Replaying = true;
					g_interfaces.cbrInterface.ReplayingP2 = true;
				}
				else {
					g_interfaces.cbrInterface.EndCbrActivities();
				}
			}
			ImGui::Separator();
		}

		ImGui::Text("Player name:");
		ImGui::PushID(177);
		ImGui::InputText("", g_interfaces.cbrInterface.playerName, IM_ARRAYSIZE(g_interfaces.cbrInterface.playerName));
		ImGui::PopID();
		ImGui::Text("Replay deletion range:");
		ImGui::PushItemWidth(80);
		ImGui::DragIntRange2("", &g_interfaces.cbrInterface.deletionStart, &g_interfaces.cbrInterface.deletionEnd, 1.0F, 0);
		ImGui::PushItemWidth(0);
		ImGui::Text(g_interfaces.cbrInterface.WriteAiInterfaceState().c_str());


		ImGui::RadioButton("Basic: ", &selected_radio, 0);
		ImGui::SameLine();
		ImGui::RadioButton("Pressure: ", &selected_radio, 1);
		ImGui::SameLine();
		ImGui::RadioButton("Blocking: ", &selected_radio, 2);
		ImGui::SameLine();
		ImGui::RadioButton("Combo: ", &selected_radio, 3);
		if (selected_radio == 0) {
			if (ImGui::CollapsingHeader("Weights")) {
				ImGui::RadioButton("Player1: ", &weightsPlayer, 0);
				ImGui::SameLine();
				ImGui::RadioButton("Player2: ", &weightsPlayer, 1);
				ImGui::Text("Save File Name:");
				ImGui::PushID(179);
				ImGui::InputText("", g_interfaces.cbrInterface.weightName, IM_ARRAYSIZE(g_interfaces.cbrInterface.weightName));
				ImGui::PopID();
				if (ImGui::Button("Save Cst"))
				{
					g_interfaces.cbrInterface.SaveWeights(g_interfaces.cbrInterface.getCbrData(weightsPlayer)->costs);
				}ImGui::SameLine();
				if (ImGui::Button("Load Cst"))
				{
					auto Cst = g_interfaces.cbrInterface.LoadWeights(g_interfaces.cbrInterface.weightName);
					if (Cst.name[0] != "-1") {
						g_interfaces.cbrInterface.getCbrData(weightsPlayer)->costs = Cst;
					}
				}


				
				auto costs = &g_interfaces.cbrInterface.getCbrData(weightsPlayer)->costs;
				for (int i = 0; i < costs->basic.size(); i++) {
					if (costs->name[i] != "-1" && costs->name[i] != "") {
						ImGui::InputFloat(costs->name[i].c_str(), &costs->basic[i]);
					}

				}
			}
		}
		if (selected_radio == 1) {
			if (ImGui::CollapsingHeader("Weights")) {
				ImGui::RadioButton("Player1: ", &weightsPlayer, 0);
				ImGui::SameLine();
				ImGui::RadioButton("Player2: ", &weightsPlayer, 1);

				ImGui::Text("Save File Name:");
				ImGui::PushID(179);
				ImGui::InputText("", g_interfaces.cbrInterface.weightName, IM_ARRAYSIZE(g_interfaces.cbrInterface.weightName));
				ImGui::PopID();
				if (ImGui::Button("Save Cst"))
				{
					g_interfaces.cbrInterface.SaveWeights(g_interfaces.cbrInterface.getCbrData(weightsPlayer)->costs);
				}ImGui::SameLine();
				if (ImGui::Button("Load Cst"))
				{
					auto Cst = g_interfaces.cbrInterface.LoadWeights(g_interfaces.cbrInterface.weightName);
					if (Cst.name[0] != "-1") {
						g_interfaces.cbrInterface.getCbrData(weightsPlayer)->costs = Cst;
					}
				}

				auto costs = &g_interfaces.cbrInterface.getCbrData(weightsPlayer)->costs;
				for (int i = 0; i < costs->pressure.size(); i++) {
					if (costs->name[i] != "-1" && costs->name[i] != "") {
						ImGui::InputFloat(costs->name[i].c_str(), &costs->pressure[i]);
					}

				}
			}
		}
		if (selected_radio == 2) {
			if (ImGui::CollapsingHeader("Weights")) {
				ImGui::RadioButton("Player1: ", &weightsPlayer, 0);
				ImGui::SameLine();
				ImGui::RadioButton("Player2: ", &weightsPlayer, 1);

				ImGui::Text("Save File Name:");
				ImGui::PushID(179);
				ImGui::InputText("", g_interfaces.cbrInterface.weightName, IM_ARRAYSIZE(g_interfaces.cbrInterface.weightName));
				ImGui::PopID();
				if (ImGui::Button("Save Cst"))
				{
					g_interfaces.cbrInterface.SaveWeights(g_interfaces.cbrInterface.getCbrData(weightsPlayer)->costs);
				}ImGui::SameLine();
				if (ImGui::Button("Load Cst"))
				{
					auto Cst = g_interfaces.cbrInterface.LoadWeights(g_interfaces.cbrInterface.weightName);
					if (Cst.name[0] != "-1") {
						g_interfaces.cbrInterface.getCbrData(weightsPlayer)->costs = Cst;
					}
				}

				auto costs = &g_interfaces.cbrInterface.getCbrData(weightsPlayer)->costs;
				for (int i = 0; i < costs->blocking.size(); i++) {
					if (costs->name[i] != "-1" && costs->name[i] != "") {
						ImGui::InputFloat(costs->name[i].c_str(), &costs->blocking[i]);
					}

				}
			}
		}
		if (selected_radio == 3) {
			if (ImGui::CollapsingHeader("Weights")) {
				ImGui::RadioButton("Player1: ", &weightsPlayer, 0);
				ImGui::SameLine();
				ImGui::RadioButton("Player2: ", &weightsPlayer, 1);

				ImGui::Text("Save File Name:");
				ImGui::PushID(179);
				ImGui::InputText("", g_interfaces.cbrInterface.weightName, IM_ARRAYSIZE(g_interfaces.cbrInterface.weightName));
				ImGui::PopID();
				if (ImGui::Button("Save Cst"))
				{
					g_interfaces.cbrInterface.SaveWeights(g_interfaces.cbrInterface.getCbrData(weightsPlayer)->costs);
				}ImGui::SameLine();
				if (ImGui::Button("Load Cst"))
				{
					auto Cst = g_interfaces.cbrInterface.LoadWeights(g_interfaces.cbrInterface.weightName);
					if (Cst.name[0] != "-1") {
						g_interfaces.cbrInterface.getCbrData(weightsPlayer)->costs = Cst;
					}
				}

				auto costs = &g_interfaces.cbrInterface.getCbrData(weightsPlayer)->costs;
				for (int i = 0; i < costs->combo.size(); i++) {
					if (costs->name[i] != "-1" && costs->name[i] != "") {
						ImGui::InputFloat(costs->name[i].c_str(), &costs->combo[i]);
					}

				}
			}
		}
	}
}

void MainWindow::DrawReversalSection() const
{
	if (!ImGui::CollapsingHeader("Reversal Actions"))
		return;

	if (!isInMatch())
	{
		ImGui::TextDisabled("YOU ARE NOT IN MATCH!");


		return;
	}



	if (isInMatch())
	{
		//ImGui::VerticalSpacing(10);
		//ImGui::HorizontalSpacing();
		if (ImGui::Button("Record"))
		{
			if (!g_interfaces.cbrInterface.reversalActive) {
				if (g_interfaces.cbrInterface.reversalRecording == false) {
					//g_interfaces.cbrInterface.reversalReplayNr = 0;
					//g_interfaces.cbrInterface.deleteReversalReplays();
					g_interfaces.cbrInterface.setAnnotatedReplay(AnnotatedReplay(g_interfaces.cbrInterface.playerName, g_interfaces.player1.GetData()->char_abbr, g_interfaces.player2.GetData()->char_abbr, g_interfaces.player1.GetData()->charIndex, g_interfaces.player2.GetData()->charIndex),0);
				}
				else {
					g_interfaces.cbrInterface.addReversalReplay(*g_interfaces.cbrInterface.getAnnotatedReplay(0));
					g_interfaces.cbrInterface.reversalReplayNr++;
					g_interfaces.cbrInterface.reversalReplaysTriggerOdds.push_back(0);
					g_interfaces.cbrInterface.reversalBuffer.push_back(1);
					g_interfaces.cbrInterface.reversalRecordingActive = false;
				}
				g_interfaces.cbrInterface.reversalRecording = !g_interfaces.cbrInterface.reversalRecording;
			}
		}ImGui::SameLine();
		if (ImGui::Button("Delete Reversals")) {
			for (int i = g_interfaces.cbrInterface.reversalReplaysTriggerOdds.size() - 1; i >= 0; i--) {
				g_interfaces.cbrInterface.reversalReplaysTriggerOdds.erase(g_interfaces.cbrInterface.reversalReplaysTriggerOdds.begin() + i);
				g_interfaces.cbrInterface.deleteReversalReplay(i);
				g_interfaces.cbrInterface.reversalReplayNr--;
			}
		}ImGui::SameLine();
		if (g_interfaces.cbrInterface.reversalReplayNr == 0) {
			g_interfaces.cbrInterface.reversalActive = false;
		}
		if (ImGui::Button("Reversal Active"))
		{
			if (g_interfaces.cbrInterface.reversalReplayNr > 0) {
				g_interfaces.cbrInterface.reversalActive = !g_interfaces.cbrInterface.reversalActive;
			}
			else {
				g_interfaces.cbrInterface.reversalActive = false;
			}
		}
		ImGui::Separator();
		for (int i = 0; i < g_interfaces.cbrInterface.reversalReplaysTriggerOdds.size(); i++) {
			ImGui::PushID(2323+i);
			ImGui::Text("---Reversal: %d---", i+1); ImGui::SameLine();
			if (ImGui::Button("Delete")) {
				g_interfaces.cbrInterface.reversalReplaysTriggerOdds.erase(g_interfaces.cbrInterface.reversalReplaysTriggerOdds.begin() + i);
				g_interfaces.cbrInterface.reversalBuffer.erase(g_interfaces.cbrInterface.reversalBuffer.begin() + i);
				g_interfaces.cbrInterface.deleteReversalReplay(i);
				g_interfaces.cbrInterface.reversalReplayNr--;
			}ImGui::SameLine();

			if (ImGui::Button("Save"))
			{
				g_interfaces.cbrInterface.SaveReversal(*g_interfaces.cbrInterface.getReversalReplay(i));
			}
			std::string s = "Odds";
			ImGui::SliderInt(s.c_str(), &g_interfaces.cbrInterface.reversalReplaysTriggerOdds[i], 0, 100, "%.0f");

			ImGui::SliderInt("Buffer", &g_interfaces.cbrInterface.reversalBuffer[i], 1, 100, "%.0f");
			ImGui::PopID();
		}
		ImGui::Separator();
		if (ImGui::Checkbox("Block Crouching", &g_interfaces.cbrInterface.blockCrouching)) {
			if (g_interfaces.cbrInterface.blockCrouching == true) {
				g_interfaces.cbrInterface.blockStanding = false;
			}
		}ImGui::SameLine();
		if (ImGui::Checkbox("Block Standing", &g_interfaces.cbrInterface.blockStanding)) {
			if (g_interfaces.cbrInterface.blockStanding == true) {
				g_interfaces.cbrInterface.blockCrouching = false;
			}
		}
		if (ImGui::Checkbox("Barrier", &g_interfaces.cbrInterface.barrierReversal)) {

		}
		ImGui::Separator();
		/*
		if (ImGui::Button("DeleteAll"))
		{
			g_interfaces.player1.reversalReplayNr = 0;
			g_interfaces.player1.deleteReversalReplays();
		}*/
		ImGui::Text("Recording: %d", g_interfaces.cbrInterface.reversalRecording); ImGui::SameLine();
		ImGui::Text("ReversalActive %d", g_interfaces.cbrInterface.reversalActive); ImGui::SameLine();
		if (!g_interfaces.player2.IsCharDataNullPtr()) {
			ImGui::Text("Hitstun: %d", g_interfaces.player2.GetData()->hitstun);
		}
	}

	
	

	ImGui::Text("Save File Name:");
	ImGui::PushID(178);
	ImGui::InputText("", g_interfaces.cbrInterface.reversalName, IM_ARRAYSIZE(g_interfaces.cbrInterface.reversalName));
	ImGui::SameLine();
	if (ImGui::Button("Load Rev"))
	{
		auto rev = g_interfaces.cbrInterface.LoadReversal(g_interfaces.cbrInterface.reversalName);
		if (rev.debugFrameIndex == 0) {
			g_interfaces.cbrInterface.addReversalReplay(rev);
			g_interfaces.cbrInterface.reversalReplayNr++;
			g_interfaces.cbrInterface.reversalReplaysTriggerOdds.push_back(0);
			g_interfaces.cbrInterface.reversalBuffer.push_back(1);
		}
	}
	ImGui::PopID();/*
	if (!g_interfaces.player1.IsCharDataNullPtr() && !g_interfaces.player2.IsCharDataNullPtr()) {
		
		ImGui::Text("isDoingDistortion %d", g_interfaces.player1.GetData()->isDoingDistortion);
		ImGui::Text("position_x %d", g_interfaces.player1.GetData()->position_x);
		ImGui::Text("position_y_dupe %d", g_interfaces.player1.GetData()->position_y_dupe);
		ImGui::Text("facingLeft %d", g_interfaces.player1.GetData()->facingLeft);
		ImGui::Text("currentActionNoNeutral %d", g_interfaces.player1.GetData()->currentActionNoNeutral);
		ImGui::Text("blockstun %d", g_interfaces.player1.GetData()->blockstun);
		ImGui::Text("hitstun %d", g_interfaces.player1.GetData()->hitstun);
		ImGui::Text("typeOfAttack %d", g_interfaces.player1.GetData()->typeOfAttack);
		ImGui::Text("hitstop %d", g_interfaces.player1.GetData()->hitstop);
		ImGui::Text("actionTimeNoHitstop %d", g_interfaces.player1.GetData()->actionTimeNoHitstop);
		ImGui::Text("lastAction %d", g_interfaces.player1.GetData()->lastAction);
		ImGui::Text("comboProration %d", g_interfaces.player1.GetData()->comboProration);
		ImGui::Text("starterRating %d", g_interfaces.player1.GetData()->starterRating);
		ImGui::Text("comboTime %d", g_interfaces.player1.GetData()->comboTime);
		ImGui::Text("frame_count_minus_1 %d", g_interfaces.player1.GetData()->frame_count_minus_1);
		ImGui::Text("timeAfterTechIsPerformed %d", g_interfaces.player1.GetData()->timeAfterTechIsPerformed);
		ImGui::Text("heatMeter %d", g_interfaces.player1.GetData()->heatMeter);
		ImGui::Text("overdriveMeter %d", g_interfaces.player1.GetData()->overdriveMeter);
		ImGui::Text("overdriveTimeleft %d", g_interfaces.player1.GetData()->overdriveTimeleft);
		ImGui::Text("currentHP %d", g_interfaces.player1.GetData()->currentHP);
		ImGui::Text("charIndex %d", g_interfaces.player1.GetData()->charIndex);
		ImGui::Text("position_x %d", g_interfaces.player1.GetData()->position_x);
		ImGui::Text("position_y_dupe %d", g_interfaces.player1.GetData()->position_y_dupe);

		ImGui::Text("Drive1 %d", g_interfaces.player1.GetData()->Drive1);
		ImGui::Text("Drive0 %d", g_interfaces.player1.GetData()->Drive0);
		ImGui::Text("TagerMagnetism %d", g_interfaces.player1.GetData()->TagerMagnetism);
		ImGui::Text("LitchiStaffState %d", g_interfaces.player1.GetData()->LitchiStaffState);
		ImGui::Text("Drive11 %d", g_interfaces.player1.GetData()->Drive11);
		ImGui::Text("Drive2 %d", g_interfaces.player1.GetData()->Drive2);
		ImGui::Text("SLOT_59 %d", g_interfaces.player1.GetData()->SLOT_59);
		ImGui::Text("CelicaRegen %d", g_interfaces.player1.GetData()->CelicaRegen);
		ImGui::Text("SLOT_31 %d", g_interfaces.player1.GetData()->SLOT_31);
		ImGui::Text("Drive1_type %d", g_interfaces.player1.GetData()->Drive1_type);

		ImGui::Text("Drive12 %d", g_interfaces.player1.GetData()->slot2_or_slot4);
		ImGui::Text("Drive3 %d", g_interfaces.player1.GetData()->Drive3);
		ImGui::Text("SLOT_60 %d", g_interfaces.player1.GetData()->SLOT_60);
		ImGui::Text("SLOT_61 %d", g_interfaces.player1.GetData()->SLOT_61);
		ImGui::Text("SLOT_62 %d", g_interfaces.player1.GetData()->SLOT_62);
	}*/
	
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
}

void MainWindow::DrawNetaSection() const
{
	if (!ImGui::CollapsingHeader("Neta Recorder"))
		return;

	if (!isInMatch())
	{
		ImGui::HorizontalSpacing();
		ImGui::TextDisabled("YOU ARE NOT IN MATCH!");


		return;
	}

	ImGui::HorizontalSpacing();

	if (isInMatch())
	{
		//ImGui::VerticalSpacing(10);
		//ImGui::HorizontalSpacing();

		if (ImGui::Button("Record Both Players"))
		{
			if (!g_interfaces.cbrInterface.netaPlaying) {
				if (g_interfaces.cbrInterface.netaRecording == false) {
					g_interfaces.cbrInterface.netaRecording = true;
					g_interfaces.cbrInterface.setAnnotatedReplay(AnnotatedReplay(g_interfaces.cbrInterface.playerName, g_interfaces.player1.GetData()->char_abbr, g_interfaces.player2.GetData()->char_abbr, g_interfaces.player1.GetData()->charIndex, g_interfaces.player2.GetData()->charIndex), 0);
					g_interfaces.cbrInterface.setAnnotatedReplay(AnnotatedReplay(g_interfaces.cbrInterface.playerName, g_interfaces.player2.GetData()->char_abbr, g_interfaces.player1.GetData()->char_abbr, g_interfaces.player2.GetData()->charIndex, g_interfaces.player1.GetData()->charIndex), 1);
				}
				else {
					g_interfaces.cbrInterface.netaRecording = false;
				}
			}
		}
		if (ImGui::Button("Replay Both Players"))
		{
			if (g_interfaces.cbrInterface.netaPlaying == false && g_interfaces.cbrInterface.getAnnotatedReplay(0)->getInputSize()> 0 &&
				g_interfaces.cbrInterface.getAnnotatedReplay(1)->getInputSize() > 0) {
				g_interfaces.cbrInterface.netaPlaying = !g_interfaces.cbrInterface.netaPlaying;
				g_interfaces.cbrInterface.netaReplayCounter = 0;
			}
			else {
				g_interfaces.cbrInterface.netaReplayCounter = 0;
				g_interfaces.cbrInterface.netaPlaying = false;
			}
		}

		ImGui::Checkbox("Freeze frame:", &g_gameVals.isFrameFrozen);
		if (g_gameVals.pFrameCount)
		{
			ImGui::SameLine();
			ImGui::Text("%d", *g_gameVals.pFrameCount);
			ImGui::SameLine();
			if (ImGui::Button("Reset"))
			{
				*g_gameVals.pFrameCount = 0;
				g_gameVals.framesToReach = 0;
			}
		}

		if (g_gameVals.isFrameFrozen)
		{
			static int framesToStep = 1;
			ImGui::HorizontalSpacing();
			if (ImGui::Button("Step frames"))
			{
				g_gameVals.framesToReach = *g_gameVals.pFrameCount + framesToStep;
			}

			ImGui::SameLine();
			ImGui::SliderInt("", &framesToStep, 1, 60);
		}
		ImGui::Text("Recording: %d", g_interfaces.cbrInterface.netaRecording);
		ImGui::Text("Replaying %d", g_interfaces.cbrInterface.netaPlaying);
	
	}
}
void MainWindow::DrawCBRDebuggingSection() const {
	if (ImGui::CollapsingHeader("Debugging")) {
		static std::string debugS = "";
		static int replay = 0;
		static int caseIndex = 0;
		static int playerIndex = 0;
		ImGui::DragInt("Replay", &replay, 1.0F, 0, 99999);
		ImGui::SameLine();
		ImGui::DragInt("Case", &caseIndex);
		ImGui::DragInt("Player", &playerIndex, 1.0F, 0, 1);
		ImGui::SameLine();
		if (ImGui::Button("Display Case")) {
			auto caseFile = g_interfaces.cbrInterface.getCbrData(playerIndex)->getReplayFiles()->at(replay).getCase(caseIndex);
			debugS = "";
			debugS = caseFile->getMetadata()->PrintState();

			debugS += "\nInput: ";
			for (int i = caseFile->getStartingIndex(); i <= caseFile->getEndIndex(); i++) {
				debugS += std::to_string(g_interfaces.cbrInterface.getCbrData(playerIndex)->getReplayFiles()->at(replay).getInput(i)) + ", ";
			}

		}
		ImGui::Text(debugS.c_str());
	}
}
