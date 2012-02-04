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
#ifndef GAMMARAYTOOL_H
#define GAMMARAYTOOL_H

#include <analyzerbase/ianalyzertool.h>

namespace Gammaray {

class GammarayTool : public Analyzer::IAnalyzerTool
{
public:
    explicit GammarayTool(QObject *parent = 0);

    void startTool(Analyzer::StartMode mode);

    Analyzer::IAnalyzerEngine *createEngine(
            const Analyzer::AnalyzerStartParameters &sp,
            ProjectExplorer::RunConfiguration *runConfiguration = 0);

    QWidget *createWidgets();
    void extensionsInitialized();

    Analyzer::IAnalyzerTool::ToolMode toolMode() const;

    QString description() const;
    QString displayName() const;
    Core::Id id() const;
    ProjectExplorer::RunMode runMode() const;

    bool canRun(ProjectExplorer::RunConfiguration *runConfiguration,
                ProjectExplorer::RunMode mode) const;

    Analyzer::AnalyzerStartParameters createStartParameters(
            ProjectExplorer::RunConfiguration *runConfiguration,
            ProjectExplorer::RunMode mode) const;

    Analyzer::AbstractAnalyzerSubConfig *createGlobalSettings();
};

} // namespace Gammaray

#endif // GAMMARAYTOOL_H
