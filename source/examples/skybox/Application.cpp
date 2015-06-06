#include "Application.h"


#include <QFileInfo>

#include <widgetzeug/MessageHandler.h>

#include "glhimmel-version.h"


Application::Application(int & argc, char ** argv)
:   QApplication(argc, argv)
{
	qInstallMessageHandler(widgetzeug::globalMessageHandler);

	const QFileInfo fi(QCoreApplication::applicationFilePath());

	QApplication::setApplicationDisplayName(fi.baseName());

    QApplication::setApplicationName(GLHIMMEL_PROJECT_NAME);
    QApplication::setApplicationVersion(GLHIMMEL_VERSION);

    QApplication::setOrganizationName(GLHIMMEL_AUTHOR_ORGANIZATION);
    QApplication::setOrganizationDomain(GLHIMMEL_AUTHOR_DOMAIN);
}

Application::~Application()
{
}
