/**************************************************************************
**
** This file is part of Qt Creator Instrumentation Tools
**
** Copyright (c) 2011 Nokia Corporation and/or its subsidiary(-ies).
**
** Author: Milian Wolff, KDAB (milian.wolff@kdab.com)
**
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** Commercial Usage
**
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
**
** GNU Lesser General Public License Usage
**
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** If you are unsure which license is appropriate for your use, please
** contact the sales department at http://qt.nokia.com/contact.
**
**************************************************************************/
#ifndef GAMMARAYRUNCONTROLFACTORY_H
#define GAMMARAYRUNCONTROLFACTORY_H

#include <projectexplorer/runconfiguration.h>

namespace Gammaray {

class GammarayRunControlFactory : public ProjectExplorer::IRunControlFactory
{
    Q_OBJECT
public:
    GammarayRunControlFactory(QObject *parent = 0);

    virtual QString displayName() const;

    virtual bool canRun(ProjectExplorer::RunConfiguration *runConfiguration,
                        const QString &mode) const;

    virtual ProjectExplorer::RunControl *create(ProjectExplorer::RunConfiguration *runConfiguration,
                                                const QString &mode);

    virtual ProjectExplorer::RunConfigWidget *
    createConfigurationWidget(ProjectExplorer::RunConfiguration *runConfiguration);

    virtual ProjectExplorer::IRunConfigurationAspect *createRunConfigurationAspect();
};

}

#endif // GAMMARAYRUNCONTROLFACTORY_H
