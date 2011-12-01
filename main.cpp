#include <QApplication>
#include <QDeclarativeView>
#include <QDeclarativeEngine>
#include <QDebug>
#ifdef HAS_BOOSTER
#include <applauncherd/MDeclarativeCache>
#endif

#ifdef HAS_BOOSTER
Q_DECL_EXPORT
#endif
int main(int argc, char **argv)
{
    QApplication *application;
    QDeclarativeView *view;
#ifdef HAS_BOOSTER
    application = MDeclarativeCache::qApplication(argc, argv);
    view = MDeclarativeCache::qDeclarativeView();
#else
    qWarning() << Q_FUNC_INFO << "Warning! Running without booster. This may be a bit slower.";
    application = new QApplication(argc, argv);
    view = new QDeclarativeView;
#endif

    QObject::connect(view->engine(), SIGNAL(quit()), application, SLOT(quit()));
    view->setSource(QUrl::fromLocalFile("main.qml"));
    view->setAttribute(Qt::WA_OpaquePaintEvent);
    view->setAttribute(Qt::WA_NoSystemBackground);
    view->viewport()->setAttribute(Qt::WA_OpaquePaintEvent);
    view->viewport()->setAttribute(Qt::WA_NoSystemBackground);
    view->showFullScreen();
    return application->exec();
}
