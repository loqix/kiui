//  Copyright (c) 2016 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#ifndef TOY_DOCKBAR_H
#define TOY_DOCKBAR_H

/* toy */
#include <toyui/Forward.h>
#include <toyui/Widget/Sheet.h>
#include <toyui/Widget/Button.h>
#include <toyui/Scheme/Window.h>

namespace toy
{
	class TOY_UI_EXPORT DockToggle : public ImgButton
	{
	public:
		DockToggle(Dockbox& dockbox, const string& icon);

		void click();

		static StyleType& cls() { static StyleType ty("DockToggle", ImgButton::cls()); return ty; }

	protected:
		Dockbox& m_dockbox;
	};

	class TOY_UI_EXPORT Dockbox : public Window
	{
	public:
		Dockbox(Dockbar& dockbar, const string& title, const string& icon);

		Dockbar& dockbar() { return m_dockbar; }
		DockToggle& toggle() { return m_toggle; }

		static StyleType& cls() { static StyleType ty("Dockbox", Window::cls()); return ty; }

	protected:
		Dockbar& m_dockbar;
		DockToggle& m_toggle;
	};

	class TOY_UI_EXPORT Docker : public Sheet
	{
	public:
		Docker();

		static StyleType& cls() { static StyleType ty("Docker", Sheet::cls()); return ty; }
	};

	class TOY_UI_EXPORT Dockbar : public Sheet
	{
	public:
		Dockbar();

		Widget& addDock(const string& name, const string& icon, unique_ptr<Widget> widget);

		Widget& vappend(unique_ptr<Widget> widget);
		unique_ptr<Widget> vrelease(Widget& widget);

		static StyleType& cls() { static StyleType ty("Dockbar", Sheet::cls()); return ty; }

	protected:
		Docker& m_docker;
	};
}

#endif
