/*
Application.hpp
Copyright (C) 2010-2013 Marc GILLERON
This file is part of the zCraft-Framework project.
*/

#ifndef HEADER_ZN_APPLICATION_HPP_INCLUDED
#define HEADER_ZN_APPLICATION_HPP_INCLUDED

#include <SFML/Graphics.hpp>

#include <fm/asset/AssetBank.hpp>
#include <fm/proto/Scene.hpp>
#include <fm/proto/PerformanceGraph.hpp>
#include <fm/proto/GameSettings.hpp>

namespace zn
{

// Top-level class handling the execution of the game.
// There must be only one Application (or derivated) instance created in the
// whole program execution.
class ZN_API Application
{
public:

	// Convenience routine for running a Application
	template <class App_T>
	static int run()
	{
		std::cout << "D: Enter run()" << std::endl;

		int exitCode = 0;

		try
		{
			App_T app;
			app.start();
		}
		catch(std::exception & e)
		{
			std::cout << "E: Fatal exception: " << e.what() << std::endl;
			std::cout << "The game will close. Press any key to dismiss..." << std::endl;
			getchar();
			exitCode = -1;
		}

		std::cout << "D: Exit run()" << std::endl;
		return exitCode;
	}

	// Gets the current game instance
	static Application * instance();

	// Constructs a Application with an optional title.
	// An empty name means using the title defined in GameSettings.
	Application(std::string title = "");

	virtual ~Application();

	// Init and executes the main loop
	void start();

	// Tells the game to stop
	void stop();

	// Gets the time span between two frames
	inline sf::Time frameTime() const { return m_frameTime; }

	// Gets the total elapsed time since startup
	inline sf::Time time() const { return m_timeClock.getElapsedTime(); }

	// Sets wether the game should be in fulscreen or not.
	// Does nothing if the game is already in the given state.
	void setFullScreen(bool fullScreen);

	// Get fullscreen state
	inline bool fullScreen() const { return m_fullScreen; }

	// Gets the size of the drawable area of the window in pixels
	inline sf::Vector2i screenSize() const
	{
		// Note: converted as signed integer coordinates because it's
		// easier to use in calculations with signed integers
		return sf::Vector2i(m_window.getSize().x, m_window.getSize().y);
	}

	// Gets mouse position relative to the window in pixels
	inline sf::Vector2i mousePosition() const { return sf::Mouse::getPosition(m_window); }

	// Hides or shows system's mouse cursor
	inline void setSystemCursorVisible(bool visible)
	{
		m_window.setMouseCursorVisible(visible);
		m_gameSettings.showSystemCursor = visible;
	}

protected:

	//-----------------------
	// Overridable methods
	//-----------------------

	// Called when the application starts, after window's creation, before main loop.
	// The default implementation does nothing.
	virtual bool onInit() { return true; }

	// Called when an event is received (input or window change)
	// The default implementation does nothing.
	// (some events like app-close or screen resize are handled in the main loop)
	virtual void onEvent(sf::Event & e) {}

	// Called before the application closes
	// The default implementation does nothing.
	virtual void onClose() {}

	//sf::RenderTexture   m_renderTexture;
	sf::RenderWindow    m_window;
	AssetBank           m_assets;
	Scene               m_scene;

private:

	bool init();
	void update();
	void render();

	void onScreenResize(sf::Vector2u size);

	GameSettings        m_gameSettings; // startup game settings

	bool                m_fullScreen;
	bool                m_runFlag;
	sf::Clock           m_frameClock;
	sf::Clock           m_timeClock;
	sf::Time            m_frameTime;
	std::string         m_title;
	sf::View            m_screenView;

#ifdef ZN_DEBUG
	sf::Clock           m_profileClock;
	PerformanceGraph    m_updateTimeGraph;
	PerformanceGraph    m_renderTimeGraph;
#endif

};

} // namespace zn

#endif // HEADER_ZN_APPLICATION_HPP_INCLUDED




