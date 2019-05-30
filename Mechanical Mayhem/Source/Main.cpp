//------------------------------------------------------------------------------
//
// File Name:	Main.cpp
// Author(s):	Jeremy Kings (j.kings)
// Project:		BetaFramework
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include <filesystem>

// BetaFramework Engine
#include <Engine.h>
#include <StartupSettings.h>

// Engine modules
#include <Space.h>
#include <SoundManager.h>
#include <FullscreenManager.h>

// Initial game state
#include "MainMenu.h"
#include "HUDEmpty.h"

// Systems
#include <GameObjectFactory.h>
#include <Parser.h>

// Components
#include "Button.h"
#include "CameraFollow.h"
#include "ShiftPickup.h"
#include "ColorChange.h"
#include "DimensionController.h"
#include "Jetpack.h"
#include "Hazard.h"
#include "MonkeyAnimation.h"
#include "PlayerMovement.h"
#include "PlayerShip.h"
#include "RisingGears.h"
#include "ScreenWrap.h"
#include "TimedDeath.h"
#include "AbilityHolder.h"
#include "AbilityPickup.h"
#include "Health.h"
#include "SimpleAnimator.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Main function
int WINAPI WinMain(_In_ HINSTANCE instance, _In_opt_ HINSTANCE prevInstance, _In_ LPSTR command_line, _In_ int show)
{
	// Enable memory leak checking
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);

	// Unused parameters
	UNREFERENCED_PARAMETER(prevInstance);
	UNREFERENCED_PARAMETER(command_line);
	UNREFERENCED_PARAMETER(show);
	UNREFERENCED_PARAMETER(instance);
	 
	// Create a new space called "GameSpace"
	Space* space = new Space("GameSpace");

	// Create a new space called "HUDSpace"
	Space* hudSpace = new Space("HUDSpace");

	// Set initial level to the main menu.
	space->SetLevel<Levels::MainMenu>()->SetAltSpace(hudSpace);

	hudSpace->SetLevel<Levels::HUDEmpty>()->SetAltSpace(space);

	Engine& engine = Engine::GetInstance();

	// Add additional modules to engine
	engine.AddModule(new FullscreenManager());
	engine.AddModule(space);
	engine.AddModule(hudSpace);

	// Set up sound effects/music
	SoundManager* soundManager = new SoundManager();
	engine.AddModule(soundManager);
	soundManager->AddEffect("ambience.wav");
	soundManager->AddEffect("deathslapfinal.wav");
	soundManager->AddEffect("flamethrower_loop.wav", true);
	soundManager->AddEffect("flamethrower_start.wav");
	soundManager->AddEffect("gamestart.wav");
	soundManager->AddEffect("hoverselect.wav");
	soundManager->AddEffect("jump.wav");
	soundManager->AddEffect("Landing final.wav");
	soundManager->AddEffect("menu Apress.wav");
	soundManager->AddEffect("SoundDimensionShiftA.wav");
	soundManager->AddEffect("SoundDimensionShiftB.wav");
	soundManager->AddEffect("SoundFanf.wav");
	soundManager->AddEffect("SoundHorn.wav");
	soundManager->AddEffect("SoundJetp.wav", true);
	soundManager->AddEffect("step.wav");
	soundManager->AddEffect("wallattach.wav");
	soundManager->AddEffect("walloff.wav");
	soundManager->AddEffect("wallslide.wav");
	soundManager->AddMusic("SoundAlarm.wav");
	soundManager->AddMusic("SoundMenuM.wav");
	soundManager->AddMusic("SoundPlay.wav");

	// Register components
	GameObjectFactory& objectFactory = GameObjectFactory::GetInstance();

	{
		using namespace Behaviors;
		objectFactory.RegisterComponent<Button>();
		objectFactory.RegisterComponent<CameraFollow>();
		objectFactory.RegisterComponent<ShiftPickup>();
		objectFactory.RegisterComponent<ColorChange>();
		objectFactory.RegisterComponent<DimensionController>();
		objectFactory.RegisterComponent<Hazard>();
		objectFactory.RegisterComponent<MonkeyAnimation>();
		objectFactory.RegisterComponent<PlayerMovement>();
		objectFactory.RegisterComponent<PlayerShip>();
		objectFactory.RegisterComponent<RisingGears>();
		objectFactory.RegisterComponent<ScreenWrap>();
		objectFactory.RegisterComponent<TimedDeath>();
		objectFactory.RegisterComponent<AbilityHolder>();
		objectFactory.RegisterComponent<AbilityPickup>();
		objectFactory.RegisterComponent<Health>();
		objectFactory.RegisterComponent<SimpleAnimator>();
	}

	StartupSettings startupSettings;
	startupSettings.windowWidth = 0;
	startupSettings.windowHeight = 0;
	startupSettings.fullscreen = true;
	startupSettings.allowMaximize = true;
	startupSettings.vSync = true;

	if (std::filesystem::exists("settings.txt"))
	{
		Parser settingsParser("settings.txt", std::fstream::in);
		settingsParser.ReadSkip("Settings");
		settingsParser.ReadSkip('{');
		settingsParser.ReadVariable("framerateCap", startupSettings.framerateCap);
		settingsParser.ReadVariable("fullscreen", startupSettings.fullscreen);
		settingsParser.ReadVariable("vSync", startupSettings.vSync);
		settingsParser.ReadSkip('}');
	}
	else
	{
		Parser settingsParser("settings.txt", std::fstream::out);
		settingsParser.WriteValue("Settings");
		settingsParser.BeginScope();
		settingsParser.WriteVariable("framerateCap", startupSettings.framerateCap);
		settingsParser.WriteVariable("fullscreen", startupSettings.fullscreen);
		settingsParser.WriteVariable("vSync", startupSettings.vSync);
		settingsParser.EndScope();
	}

	// TEMPORARY NON-FULLSCREEN (FOR DEBUG)
	startupSettings.fullscreen = false;

	// Game engine goes!
	engine.Start(startupSettings);

	// Shut down the sound manager
	soundManager->Shutdown();

	return 0;
}
