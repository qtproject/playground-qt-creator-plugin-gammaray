/**************************************************************************
**
** This file is part of Qt Creator
**
** Copyright (C) 2011 Kläralvdalens Datakonsult AB, a KDAB Group company.
** Author: Milian Wolff, KDAB (milian.wolff@kdab.com)
**
** Contact: Kläralvdalens Datakonsult AB (info@kdab.com)
**
**
** GNU Lesser General Public License Usage
**
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this file.
** Please review the following information to ensure the GNU Lesser General
** Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** Other Usage
**
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
** If you have questions regarding the use of this file, please contact
** Nokia at info@qt.nokia.com.
**
**************************************************************************/

#include "gammarayengine.h"

#include "gammaraysettings.h"

#include <analyzerbase/analyzermanager.h>
#include <analyzerbase/analyzerstartparameters.h>

#include <coreplugin/icore.h>
#include <coreplugin/ioutputpane.h>
#include <coreplugin/progressmanager/progressmanager.h>
#include <coreplugin/progressmanager/futureprogress.h>

#include <extensionsystem/pluginmanager.h>

#include <utils/qtcassert.h>
#include <utils/qtcprocess.h>

#include <projectexplorer/runconfiguration.h>
#include <projectexplorer/debugginghelper.h>
#include <projectexplorer/applicationrunconfiguration.h>

#include <qtsupport/baseqtversion.h>
#include <qtsupport/qtversionmanager.h>

#include <QApplication>
#include <QMainWindow>
#include <QAction>
#include <QFileInfo>

using namespace Analyzer;

using namespace Gammaray;

GammarayEngine::GammarayEngine(IAnalyzerTool *tool,
                               const AnalyzerStartParameters &sp,
                               ProjectExplorer::RunConfiguration *runConfiguration)
    : IAnalyzerEngine(tool, sp, runConfiguration), m_settings(0)
{
    AnalyzerSettings *settings = 0;

    if (runConfiguration) {
        settings = runConfiguration->extraAspect<AnalyzerRunConfigurationAspect>();
    }
    if (!settings) {
        settings = AnalyzerGlobalSettings::instance();
    }

    m_settings = settings->subConfig<GammarayBaseSettings>();
    QTC_CHECK(m_settings);

    connect(&m_progressWatcher, SIGNAL(canceled()),
            this, SLOT(handleProgressCanceled()));
    connect(&m_progressWatcher, SIGNAL(finished()),
            this, SLOT(handleProgressFinished()));

    connect(&m_process, SIGNAL(readyReadStandardOutput()),
            SLOT(receiveStandardOutput()));
    connect(&m_process, SIGNAL(readyReadStandardError()),
            SLOT(receiveStandardError()));
    connect(&m_process, SIGNAL(finished(int)),
            SLOT(processFinished()));
    connect(&m_process, SIGNAL(error(QProcess::ProcessError)),
            SLOT(processError(QProcess::ProcessError)));

    connect(AnalyzerManager::stopAction(), SIGNAL(triggered()), this, SLOT(stopIt()));
}

GammarayEngine::~GammarayEngine()
{
}

void GammarayEngine::handleProgressCanceled()
{
    AnalyzerManager::stopTool();
}

void GammarayEngine::handleProgressFinished()
{
    QApplication::alert(Core::ICore::instance()->mainWindow(), 3000);
}

bool GammarayEngine::start()
{
    ProjectExplorer::LocalApplicationRunConfiguration* rc =
            dynamic_cast<ProjectExplorer::LocalApplicationRunConfiguration*>(runConfiguration());
    QTC_ASSERT(rc, return false);

    QString exe;

    const Utils::FileName qmakePath = ProjectExplorer::DebuggingHelperLibrary::findSystemQt(rc->environment());
    QtSupport::BaseQtVersion *version = QtSupport::QtVersionManager::instance()->qtVersionForQMakeBinary(qmakePath);
    exe = m_settings->gammarayForQt(qmakePath.toString());
    QFileInfo fi(exe);

    if (!version) {
        emit outputReceived(tr("Could not find Qt Version for QMake binary %1").arg(qmakePath.toString()),
                            Utils::ErrorMessageFormat);
        finishOnError();
        return false;
    } else if (exe.isEmpty() || !fi.exists()) {
        emit outputReceived(tr("The GammaRay executable is no set for Qt version %1.\n"
                               "Go to Options -> Analyzer -> GammaRay to set it.").arg(version->displayName()),
                            Utils::ErrorMessageFormat);
        finishOnError();
        return false;
    }

    emit starting(this);

    Core::FutureProgress *fp =
            Core::ICore::instance()->progressManager()->addTask(m_progress.future(),
                                                                tr("Investigating Application"),
                                                                "gammaray");
    fp->setKeepOnFinish(Core::FutureProgress::HideOnFinish);
    m_progress.reportStarted();
    m_progressWatcher.setFuture(m_progress.future());

    const AnalyzerStartParameters &sp = startParameters();
    m_process.setWorkingDirectory(sp.workingDirectory);

    if (!sp.analyzerCmdPrefix.isEmpty()) {
        exe = sp.analyzerCmdPrefix + ' ' + exe;
    }

    QString arguments;
    switch(m_settings->injector()) {
    case Constants::DefaultInjector:
        // nothing
        break;
    case Constants::GDBInjector:
        Utils::QtcProcess::addArg(&arguments, "-i gdb");
    case Constants::StyleInjector:
        Utils::QtcProcess::addArg(&arguments, "-i style");
        break;
#ifndef Q_OS_WIN
    case Constants::PreloadInjector:
        Utils::QtcProcess::addArg(&arguments, "-i preload");
        break;
#else
    case Constants::WinDLLInjector:
        Utils::QtcProcess::addArg(&arguments, "-i windll");
        break;
#endif
    }

    Utils::QtcProcess::addArg(&arguments, sp.debuggee);
    Utils::QtcProcess::addArgs(&arguments, sp.debuggeeArgs);

    m_process.setCommand(exe, arguments);

    m_process.setEnvironment(sp.environment);

    m_process.start();

    return true;
}

void GammarayEngine::stop()
{
    m_process.terminate();
}

void GammarayEngine::stopIt()
{
    stop();
}

void GammarayEngine::receiveStandardOutput()
{
    emit outputReceived(m_process.readAllStandardOutput(), Utils::StdOutFormat);
}

void GammarayEngine::receiveStandardError()
{
    emit outputReceived(m_process.readAllStandardError(), Utils::StdErrFormat);
}

void GammarayEngine::processFinished()
{
    emit outputReceived(tr("** Analyzing finished **\n"), Utils::NormalMessageFormat);
    emit finished();

    m_progress.reportFinished();
}

void GammarayEngine::processError(QProcess::ProcessError error)
{
    Q_UNUSED(error)
    emit outputReceived(m_process.errorString(), Utils::ErrorMessageFormat);
}

void GammarayEngine::finishOnError()
{
    emit finished();

    ///FIXME: get a better API for this into Qt Creator
    ExtensionSystem::PluginManager *pm = ExtensionSystem::PluginManager::instance();
    QList<Core::IOutputPane *> panes = pm->getObjects<Core::IOutputPane>();
    foreach (Core::IOutputPane *pane, panes) {
        if (pane->displayName() == tr("Application Output")) {
            pane->popup(false);
            break;
        }
    }
}
