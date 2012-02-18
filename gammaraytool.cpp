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

#include "gammaraytool.h"

#include "gammarayengine.h"
#include "gammarayconstants.h"
#include "gammaraysettings.h"

#include <analyzerbase/analyzermanager.h>
#include <analyzerbase/analyzersettings.h>

#include <projectexplorer/applicationrunconfiguration.h>

#include <QWidget>

using namespace Analyzer;
using namespace ProjectExplorer;
using namespace Gammaray;

GammarayTool::GammarayTool(QObject *parent)
    : IAnalyzerTool(parent)
{
    setObjectName("GammarayTool");
}

void GammarayTool::startTool(StartMode mode)
{
    Q_UNUSED(mode)
    AnalyzerManager::startLocalTool(this);
}

IAnalyzerEngine *GammarayTool::createEngine(const AnalyzerStartParameters &sp,
                                            ProjectExplorer::RunConfiguration *runConfiguration)
{
    GammarayEngine *engine = new GammarayEngine(this, sp, runConfiguration);

    AnalyzerManager::showStatusMessage(AnalyzerManager::msgToolStarted(displayName()));
    return engine;
}

QWidget *GammarayTool::createWidgets()
{
    return new QWidget;
}

void GammarayTool::extensionsInitialized()
{

}

IAnalyzerTool::ToolMode GammarayTool::toolMode() const
{
    return DebugMode;
}

QString GammarayTool::description() const
{
    return tr("Get internal informations from your applications.");
}

QString GammarayTool::displayName() const
{
    return tr("GammaRay");
}

Core::Id GammarayTool::id() const
{
    return "GammaRay";
}

RunMode GammarayTool::runMode() const
{
    return (RunMode)0x6825;
}

bool GammarayTool::canRun(ProjectExplorer::RunConfiguration *runConfiguration, RunMode mode) const
{
    Q_UNUSED(mode)
    return dynamic_cast<LocalApplicationRunConfiguration*>(runConfiguration);
}

AnalyzerStartParameters GammarayTool::createStartParameters(RunConfiguration *runConfiguration,
                                                            RunMode mode) const
{
    Q_UNUSED(mode)

    AnalyzerStartParameters sp;
    sp.startMode = StartLocal;

    LocalApplicationRunConfiguration *rc =
            dynamic_cast<LocalApplicationRunConfiguration*>(runConfiguration);
    sp.environment = rc->environment();
    sp.workingDirectory = rc->workingDirectory();
    sp.debuggee = rc->executable();
    sp.debuggeeArgs = rc->commandLineArguments();
    sp.displayName = rc->displayName();

    return sp;
}

AbstractAnalyzerSubConfig *GammarayTool::createGlobalSettings()
{
    return new GammarayGlobalSettings();
}
