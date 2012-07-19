/*
 * Copyright (C) 2011 Robin Burchell <robin+mer@viroteck.net>
 *
 * You may use this file under the terms of the BSD license as follows:
 *
 * "Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in
 *     the documentation and/or other materials provided with the
 *     distribution.
 *   * Neither the name of Nemo Mobile nor the names of its contributors
 *     may be used to endorse or promote products derived from this
 *     software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
 */

#include <QApplication>
#include <QDeclarativeView>
#include <QDeclarativeEngine>
#include <QDebug>
#include <QDir>
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
    QApplication stackApp(argc, argv);
    QDeclarativeView stackView;
    application = &stackApp;
    view = &stackView;
#endif

    QString path;
    QString urlstring;
    bool isFullscreen = false;
    QStringList arguments = application->arguments();
    for (int i = 0; i < arguments.count(); ++i) {
        QString parameter = arguments.at(i);
        if (parameter == "-path") {
            if (i + 1 >= arguments.count())
                qFatal("-path requires an argument");
            path = arguments.at(i + 1);
            i++;
        } else if (parameter == "-url") {
            if (i + 1 >= arguments.count())
                qFatal("-path requires an argument");
            urlstring = arguments.at(i + 1);
            i++;
        } else if (parameter == "-fullscreen") {
            isFullscreen = true;
        } else if (parameter == "-help") {
            qDebug() << "Fast QML-only application launcher";
            qDebug() << "-fullscreen   - show QML fullscreen";
            qDebug() << "-path         - path to cd to before launching -url";
            qDebug() << "-url          - file to launch (default: main.qml inside -path)";
            exit(0);
        }
    }

    if (!path.isEmpty())
        QDir::setCurrent(path);

    QUrl url;
    if (urlstring.isEmpty())
        url = QUrl::fromLocalFile("main.qml");
    else
        url = QUrl::fromUserInput(urlstring);
        

    QObject::connect(view->engine(), SIGNAL(quit()), application, SLOT(quit()));
    view->setSource(url);
    view->setAttribute(Qt::WA_OpaquePaintEvent);
    view->setAttribute(Qt::WA_NoSystemBackground);
    view->viewport()->setAttribute(Qt::WA_OpaquePaintEvent);
    view->viewport()->setAttribute(Qt::WA_NoSystemBackground);

    if (isFullscreen)
        view->showFullScreen();
    else
        view->show();

    return application->exec();
}
