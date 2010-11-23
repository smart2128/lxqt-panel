#ifndef RAZOR_MAINMENU_CPP
#define RAZOR_MAINMENU_CPP
#include "razormainmenu.h"
#include "razor.h"

/**
 * @file razormainmenu.cpp
 * @brief implements the classes Razormainmenu and Razormenu
 * @author Christopher "VdoP" Regali
 */


/**
 * @brief the constructor
 */
RazorMenu::RazorMenu(RazorBar * panel, QWidget * parent) : RazorPluginSquare(panel, parent)
{
    //we only need to make the gui
    gui = new RazorMainMenu(this);
    mainLayout()->addWidget(gui);
}

/**
 * @brief the destructor
 */
RazorMenu::~RazorMenu()
{
    //todo here: remove the widget cleanly too
    //delete gui;
}




/**
 * @brief this creates our mainmenu and gets the xdg-menus for it
 */
void RazorMainMenu::createMenus()
{
    //make the menu
    mainMenu = new QMenu;
    // add the basic actions (shutdown and stuff)

    // 1 create the actions
    shutdown = new QAction((QIcon)Razor::getInstance().get_Xdgmanager()->get_xdgiconthememanager()->getIconpath("system-shutdown","","Actions"), tr("Shutdown"),this);
    reboot = new QAction((QIcon)Razor::getInstance().get_Xdgmanager()->get_xdgiconthememanager()->getIconpath("system-reboot","","Actions"), tr("Reboot"),this);
    logout = new QAction((QIcon)Razor::getInstance().get_Xdgmanager()->get_xdgiconthememanager()->getIconpath("system-logout","","Actions"), tr("Logout"),this);
    about = new QAction((QIcon)Razor::getInstance().get_Xdgmanager()->get_xdgiconthememanager()->getIconpath("help-about","","Actions"), tr("about"),this);

    // 2 link with handler
    connect(shutdown, SIGNAL(triggered()), Razor::getInstance().get_handler(),SLOT(sys_shutdown()));

    connect(reboot, SIGNAL(triggered()), Razor::getInstance().get_handler(),SLOT(sys_reboot()));
    connect(logout, SIGNAL(triggered()), Razor::getInstance().get_handler(),SLOT(sys_logout()));
    connect(about, SIGNAL(triggered()), Razor::getInstance().get_handler(),SLOT(gui_showabout()));


    //add the xdg-menu
    //prepare it first
    Razor::getInstance().get_Xdgmanager()->get_xdgmenu()->getFilelist();
    Razor::getInstance().get_Xdgmanager()->get_xdgmenu()->readMenufile();
    Razor::getInstance().get_Xdgmanager()->get_xdgmenu()->assignIcons();
    Razor::getInstance().get_Xdgmanager()->get_xdgmenu()->feedTree();
    // then add it
    mainMenu->addMenu(Razor::getInstance().get_Xdgmanager()->get_xdgmenu()->get_QMenus());
    mainMenu->addSeparator();
    // 3 add our custom actions
    mainMenu->addAction(about);
    mainMenu->addAction(logout);
    mainMenu->addAction(reboot);
    mainMenu->addAction(shutdown);


}

/**
 * @brief this initializes us
 */
RazorMainMenu::RazorMainMenu(RazorPluginSquare* parent)
    : QLabel(parent)
{
    makeUp();
    createMenus();
}

/**
 * @brief cleans up our mess
 */
RazorMainMenu::~RazorMainMenu()
{
    delete mainMenu;
    delete logout;
    delete reboot;
    delete shutdown;
    delete about;
}


/**
 * @brief makes up the menu and style
 */

void RazorMainMenu::makeUp()
{
    int barheight = Razor::getInstance().get_looknfeel()->getInt("razorbar_height");
    setFixedSize(barheight, barheight );
    icon = Razor::getInstance().get_looknfeel()->getPath() + Razor::getInstance().get_looknfeel()->getString("razormainmenu_icon");
    //setPixmap( ((QPixmap)icon ).scaled(barheight - 5, barheight - 5));
    picon = ((QPixmap)icon ).scaled(barheight - 2, barheight - 2);
    acticon = Razor::getInstance().get_looknfeel()->getPath() + Razor::getInstance().get_looknfeel()->getString("razormainmenu_acticon");
    //setPixmap( ((QPixmap)icon ).scaled(barheight - 5, barheight - 5));
    actpicon = ((QPixmap)acticon ).scaled(barheight, barheight);
    setPixmap(picon);
    show();
}

/**
 * @brief makes the menu popup
 */

void RazorMainMenu::mousePressEvent(QMouseEvent* _event)
{
    if (_event->button() == Qt::LeftButton)
    {
        QPoint popuppos = mapToGlobal(QPoint(0,0));
        popuppos.setY(popuppos.y()-mainMenu->sizeHint().height()-5);
        mainMenu->popup(popuppos);
    }
}


/**
 * @brief makes the menu glow on mouseover
 */

void RazorMainMenu::enterEvent(QEvent* _event)
{
    Q_UNUSED(_event);
    setPixmap(actpicon);

}

/**
 * @brief makes the menu glow on mouseover
 */

void RazorMainMenu::leaveEvent(QEvent* _event)
{
    Q_UNUSED(_event);
    setPixmap(picon);
}



#endif