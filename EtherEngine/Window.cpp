#include "Window.h"

ETHER_API showMessageBox(lua_State* L)
{
	SDL_MessageBoxFlags flag;
	switch ((int)luaL_checknumber(L, 3))
	{
	case MSGBOX_ERROR:
		flag = SDL_MESSAGEBOX_ERROR;
		break;
	case MSGBOX_WARNING:
		flag = SDL_MESSAGEBOX_WARNING;
		break;
	case MSGBOX_INFO:
		flag = SDL_MESSAGEBOX_INFORMATION;
		break;
	default:
		luaL_error(L, "bad argument #3 to 'ShowMessageBox' (MACRO number expected, got %s)", luaL_typename(L, 3));
		break;
	}
	SDL_ShowSimpleMessageBox(flag, luaL_checkstring(L, 1), luaL_checkstring(L, 2), window);

	return 0;
}


ETHER_API createWindow(lua_State* L)
{
	if (window)
	{
		luaL_error(L, "bad operation to 'CreateWindow' (only one window can be created)");
	}
	else
	{
		SDL_Rect rect = GetRectParam(L, 2, "CreateWindow");
		rect.x = rect.x == WINDOW_POSITION_DEFAULT ? SDL_WINDOWPOS_UNDEFINED : rect.x;
		rect.y = rect.y == WINDOW_POSITION_DEFAULT ? SDL_WINDOWPOS_UNDEFINED : rect.y;

		int flags = SDL_WINDOW_SHOWN;
		if (!lua_istable(L, 3))
		{
			luaL_error(L, "bad argument #3 to 'CreateWindow' (table expected, got %s)", luaL_typename(L, 3));
		}
		else
		{
			lua_pushnil(L);
			while (lua_next(L, 3))
			{
				lua_pushvalue(L, -2);
				switch ((int)lua_tonumber(L, -2))
				{
				case WINDOW_FULLSCREEN:
					flags |= SDL_WINDOW_FULLSCREEN;
					break;
				case WINDOW_FULLSCREEN_DESKTOP:
					flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
					break;
				case WINDOW_BORDERLESS:
					flags |= SDL_WINDOW_BORDERLESS;
					break;
				case WINDOW_RESIZABLE:
					flags |= SDL_WINDOW_RESIZABLE;
					break;
				case WINDOW_MAXIMIZED:
					flags |= SDL_WINDOW_MAXIMIZED;
					break;
				case WINDOW_MINIMIZED:
					flags |= SDL_WINDOW_MINIMIZED;
					break;
				default:
					luaL_error(L, "bad argument #3 to 'CreateWindow' (the elements of table must be MACRO number, got %s)", luaL_typename(L, -2));
					break;
				}
				lua_pop(L, 2);
			}

			window = SDL_CreateWindow(luaL_checkstring(L, 1), rect.x, rect.y, rect.w, rect.h, flags);
			renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

#ifdef __WINDOWS__
			SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "best");
#else
			SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
#endif
		}
	}

	return 0;
}


ETHER_API closeWindow(lua_State* L)
{
	if (!window)
	{
		luaL_error(L, "close operation must be done after the window creation operation");
	}

	SDL_DestroyWindow(window);
	window = NULL;

	return 0;
}


ETHER_API setWindowTitle(lua_State* L)
{
	if (!window)
	{
		luaL_error(L, "title operation must be done after the window creation operation");
	}

	SDL_SetWindowTitle(window, luaL_checkstring(L, 1));

	return 0;
}


ETHER_API getWindowTitle(lua_State* L)
{
	if (!window)
	{
		luaL_error(L, "title operation must be done after the window creation operation");
	}

	lua_pushstring(L, SDL_GetWindowTitle(window));

	return 1;
}


ETHER_API setWindowMode(lua_State* L)
{
	if (!window)
	{
		luaL_error(L, "mode operation must be done after the window creation operation");
	}

	switch ((int)luaL_checknumber(L, 1))
	{
	case WINDOW_MODE_WINDOWED:
		SDL_SetWindowFullscreen(window, 0);
		break;
	case WINDOW_MODE_FULLSCREEN:
		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
		break;
	case WINDOW_MODE_FULLSCREEN_DESKTOP:
		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
		break;
	default:
		luaL_error(L, "bad argument #1 to 'SetWindowMode' (number-MACRO expected, got %s)", luaL_typename(L, -1));
		break;
	}

	return 0;
}


ETHER_API setWindowResizable(lua_State* L)
{
	if (!window)
	{
		luaL_error(L, "resize operation must be done after the window creation operation");
	}

	SDL_SetWindowResizable(window, lua_toboolean(L, 1) ? SDL_TRUE : SDL_FALSE);

	return 0;
}


ETHER_API setWindowOpacity(lua_State* L)
{
	if (!window)
	{
		luaL_error(L, "opacity operation must be done after the window creation operation");
	}

	SDL_SetWindowOpacity(window, luaL_checknumber(L, 1));

	return 0;
}


ETHER_API setWindowSize(lua_State* L)
{
	if (!window)
	{
		luaL_error(L, "size operation must be done after the window creation operation");
	}

	SDL_SetWindowSize(window, luaL_checknumber(L, 1), luaL_checknumber(L, 2));

	return 0;
}


ETHER_API getWindowSize(lua_State* L)
{
	if (!window)
	{
		luaL_error(L, "size operation must be done after the window creation operation");
	}

	int width = 0;
	int height = 0;
	SDL_GetWindowSize(window, &width, &height);
	lua_pushnumber(L, width);
	lua_pushnumber(L, height);

	return 2;
}


ETHER_API setWindowMaxSize(lua_State* L)
{
	if (!window)
	{
		luaL_error(L, "size operation must be done after the window creation operation");
	}

	SDL_SetWindowMaximumSize(window, luaL_checknumber(L, 1), luaL_checknumber(L, 2));

	return 0;
}


ETHER_API getWindowMaxSize(lua_State* L)
{
	if (!window)
	{
		luaL_error(L, "size operation must be done after the window creation operation");
	}

	int width = 0;
	int height = 0;
	SDL_GetWindowMaximumSize(window, &width, &height);
	lua_pushnumber(L, width);
	lua_pushnumber(L, height);

	return 2;
}


ETHER_API setWindowMinSize(lua_State* L)
{
	if (!window)
	{
		luaL_error(L, "size operation must be done after the window creation operation");
	}

	SDL_SetWindowMinimumSize(window, luaL_checknumber(L, 1), luaL_checknumber(L, 2));

	return 0;
}


ETHER_API getWindowMinSize(lua_State* L)
{
	if (!window)
	{
		luaL_error(L, "size operation must be done after the window creation operation");
	}

	int width = 0;
	int height = 0;
	SDL_GetWindowMinimumSize(window, &width, &height);
	lua_pushnumber(L, width);
	lua_pushnumber(L, height);

	return 2;
}


ETHER_API setWindowPosition(lua_State* L)
{
	if (!window)
	{
		luaL_error(L, "position operation must be done after the window creation operation");
	}

	SDL_Point point = GetPointParam(L, 1, "SetWindowPosition");

	SDL_SetWindowPosition(window, point.x, point.y);

	return 0;
}


ETHER_API getWindowPosition(lua_State* L)
{
	if (!window)
	{
		luaL_error(L, "position operation must be done after the window creation operation");
	}

	SDL_Point point;
	SDL_GetWindowPosition(window, &point.x, &point.y);
	lua_newtable(L);
	lua_pushstring(L, "x");
	lua_pushnumber(L, point.x);
	lua_settable(L, -3);
	lua_pushstring(L, "y");
	lua_pushnumber(L, point.y);
	lua_settable(L, -3);

	return 1;
}

ETHER_API setWindowIcon(lua_State* L)
{
	SDL_Surface* surface = (SDL_Surface*)lua_touserdata(L, 1);

	if (!surface)
	{
		luaL_error(L, "bad argument #1 to 'SetWindowIcon' (userdata-IMAGE expected, got %s)", luaL_typename(L, 1));
	}
	else
	{
		if (!window)
		{
			luaL_error(L, "icon operation must be done after the window creation operation");
		}

		SDL_SetWindowIcon(window, surface);
	}

	return 0;
}


ETHER_API clearWindow(lua_State* L)
{
	if (!window)
	{
		luaL_error(L, "clear operation must be done after the window creation operation");
	}

	SDL_RenderClear(renderer);

	return 0;
}


ETHER_API updateWindow(lua_State* L)
{
	if (!window)
	{
		luaL_error(L, "update operation must be done after the window creation operation");
	}

	SDL_RenderPresent(renderer);

	return 0;
}


MoudleWindow::MoudleWindow(lua_State* L)
{
	_pL = L;

	_vCMethods = {
		{ "ShowMessageBox", showMessageBox},
		{ "CreateWindow", createWindow },
		{ "CloseWindow", closeWindow },
		{ "SetWindowTitle", setWindowTitle },
		{ "GetWindowTitle", getWindowTitle },
		{ "SetWindowMode", setWindowMode },
		{ "SetWindowResizable", setWindowResizable },
		{ "SetWindowOpacity", setWindowOpacity },
		{ "SetWindowSize", setWindowSize },
		{ "GetWindowSize", getWindowSize },
		{ "SetWindowMaxSize", setWindowMaxSize },
		{ "GetWindowMaxSize", getWindowMaxSize },
		{ "SetWindowMinSize", setWindowMinSize },
		{ "GetWindowMinSize", getWindowMinSize },
		{ "SetWindowPosition", setWindowPosition },
		{ "GetWindowPosition", getWindowPosition },
		{ "SetWindowIcon", setWindowIcon },
		{ "ClearWindow", clearWindow },
		{ "UpdateWindow", updateWindow },
	};

	_vMacros = {
		{ "WINDOW_POSITION_DEFAULT", WINDOW_POSITION_DEFAULT },

		{ "MSGBOX_ERROR", MSGBOX_ERROR },
		{ "MSGBOX_WARNING", MSGBOX_WARNING },
		{ "MSGBOX_INFO", MSGBOX_INFO },

		{ "WINDOW_FULLSCREEN", WINDOW_FULLSCREEN },
		{ "WINDOW_FULLSCREEN_DESKTOP", WINDOW_FULLSCREEN_DESKTOP },
		{ "WINDOW_BORDERLESS", WINDOW_BORDERLESS },
		{ "WINDOW_RESIZABLE", WINDOW_RESIZABLE },
		{ "WINDOW_MAXIMIZED", WINDOW_MAXIMIZED },
		{ "WINDOW_MINIMIZED", WINDOW_MINIMIZED },

		{ "WINDOW_MODE_WINDOWED", WINDOW_MODE_WINDOWED },
		{ "WINDOW_MODE_FULLSCREEN", WINDOW_MODE_FULLSCREEN },
		{ "WINDOW_MODE_FULLSCREEN_DESKTOP", WINDOW_MODE_FULLSCREEN_DESKTOP },
	};
}