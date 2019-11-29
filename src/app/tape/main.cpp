/************************************************************************
 **
 **  @file   main.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   10 7, 2015
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2015 Valentina project
 **  <https://bitbucket.org/dismine/valentina> All Rights Reserved.
 **
 **  Valentina is free software: you can redistribute it and/or modify
 **  it under the terms of the GNU General Public License as published by
 **  the Free Software Foundation, either version 3 of the License, or
 **  (at your option) any later version.
 **
 **  Valentina is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 **  GNU General Public License for more details.
 **
 **  You should have received a copy of the GNU General Public License
 **  along with Valentina.  If not, see <http://www.gnu.org/licenses/>.
 **
 *************************************************************************/

#include "tmainwindow.h"
#include "mapplication.h"
#include "../fervor/fvupdater.h"
#include "../vmisc/vsysexits.h"

#include <QMessageBox> // For QT_REQUIRE_VERSION
#include <QTimer>

#if defined(APPIMAGE) && defined(Q_OS_LINUX)
#if QT_VERSION < QT_VERSION_CHECK(5, 12, 0)
#   include "../vmisc/backport/qscopeguard.h"
#else
#   include <QScopeGuard>
#endif
#   include "unicode/putil.h"
extern "C" {
#   include "../vmisc/binreloc.h"
}
#endif // defined(APPIMAGE) && defined(Q_OS_LINUX)

int main(int argc, char *argv[])
{
#if defined(APPIMAGE) && defined(Q_OS_LINUX)
    /* When deploying with AppImage based on OpenSuse, the ICU library has a hardcoded path to the icudt*.dat file.
     * This prevents the library from using shared in memory data. There are few ways to resolve this issue. According
     * to documentation we can either use ICU_DATA environment variable or the function u_setDataDirectory().
     */
//    VAbstractApplication::SetICUData(argc, argv);
    char *exe_dir = nullptr;

    BrInitError error;
    if (br_init (&error))
    {
        char *path = br_find_exe_dir(nullptr);
        if (path)
        {
            const char* correction = "/../share/icu";
            exe_dir = static_cast<char *> (malloc(strlen(path)+strlen(correction)+1));
            if(exe_dir)
            {
                strcpy(exe_dir, path);
                strcat(exe_dir, correction);
                u_setDataDirectory(exe_dir);
            }
            free(path);
        }
    }

    auto FreeMemory = qScopeGuard([exe_dir] {free(exe_dir);});
#endif

    Q_INIT_RESOURCE(tapeicon);
    Q_INIT_RESOURCE(theme);
    Q_INIT_RESOURCE(icon);
    Q_INIT_RESOURCE(schema);
    Q_INIT_RESOURCE(flags);
    Q_INIT_RESOURCE(style);

    QT_REQUIRE_VERSION(argc, argv, "5.4.0")// clazy:exclude=qstring-arg,qstring-allocations

#if defined(Q_OS_WIN)
    VAbstractApplication::WinAttachConsole();
#endif

#ifndef Q_OS_MAC // supports natively
    InitHighDpiScaling(argc, argv);
#endif //Q_OS_MAC

    MApplication app(argc, argv);
    app.InitOptions();

    if (FvUpdater::IsStaledTestBuild())
    {
        qWarning() << QApplication::translate("Tape",
                                              "This test build is older than %1 days. To provide you with better "
                                              "quality service we restrict the lifetime you can use a test build. "
                                              "To continue using Tape please update to newer test build. The "
                                              "application will be shut down.")
                       .arg(FvUpdater::testBuildLifetime);
        return V_EX_UNAVAILABLE;
    }

    QTimer::singleShot(0, &app, &MApplication::ProcessCMD);

#if defined(APPIMAGE) && defined(Q_OS_LINUX)
    if (exe_dir)
    {
        qDebug() << "Path to ICU folder:" << exe_dir;
    }
#endif // defined(APPIMAGE) && defined(Q_OS_LINUX)

    return app.exec();
}
