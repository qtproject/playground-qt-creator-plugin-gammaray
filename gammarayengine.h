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

#ifndef GAMMARAYENGINE_H
#define GAMMARAYENGINE_H

#include <analyzerbase/ianalyzerengine.h>

#include <utils/qtcprocess.h>

#include <QFutureWatcher>

namespace Gammaray {

class GammarayBaseSettings;

class GammarayEngine : public Analyzer::IAnalyzerEngine
{
    Q_OBJECT

public:
    GammarayEngine(Analyzer::IAnalyzerTool *tool,
                   const Analyzer::AnalyzerStartParameters &sp,
                   ProjectExplorer::RunConfiguration *runConfiguration);
    ~GammarayEngine();

    virtual bool start();
    virtual void stop();

private:
    void finishOnError();

private slots:
    void stopIt();
    void handleProgressCanceled();
    void handleProgressFinished();
    void processFinished();
    void processError(QProcess::ProcessError error);

    void receiveStandardOutput();
    void receiveStandardError();

private:
    Utils::QtcProcess m_process;
    QFutureInterface<void> m_progress;
    QFutureWatcher<void> m_progressWatcher;
    GammarayBaseSettings *m_settings;
};

}

#endif // GAMMARAYENGINE_H
