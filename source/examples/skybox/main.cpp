
#if defined(WIN32) && !defined(_DEBUG)
#pragma comment(linker, "/SUBSYSTEM:WINDOWS /entry:mainCRTStartup")
#endif

#include "Application.h"

#include "gui/mainwindow.h"
#include "utils/tr.h"


int main(int argc, char* argv[])
{
	int result = -1;

	Application app(argc, argv);


	MainWindow w;
#if defined(Q_WS_S60)
	w.showMaximized();
#else
	w.show();
#endif

	return app.exec();
}
