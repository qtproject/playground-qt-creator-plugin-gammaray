/**************************************************************************
**
** This file is part of Qt Creator
**
** Copyright (C) 2011 Kläralvdalens Datakonsult AB, a KDAB Group company.
** Author: Milian Wolff, KDAB (milian.wolff@kdab.com)
**
** Contact: Kläralvdalens Datakonsult AB (info@kdab.com)
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
**************************************************************************/

#ifndef GAMMARAYSETTINGS_H
#define GAMMARAYSETTINGS_H

#include <analyzerbase/analyzersettings.h>

#include "gammarayconstants.h"

namespace Gammaray {

/**
 * Gammaray settings shared for global and per-project.
 */
class GammarayBaseSettings : public Analyzer::AbstractAnalyzerSubConfig
{
    Q_OBJECT

public:
    GammarayBaseSettings() {}

    virtual QVariantMap toMap() const;
    virtual QVariantMap defaults() const;
    virtual void fromMap(const QVariantMap &map);

    virtual QString id() const;
    virtual QString displayName() const;

    virtual QWidget *createConfigWidget(QWidget *parent);

   /**
   * Base gammaray settings
   */
public:
    Constants::InjectorType injector() const;
    QString gammarayForQt(const QString &qmakePath) const;

public slots:
    void setInjector(Constants::InjectorType);
    void setGammarayForQt(const QString &qmakePath, const QString &gammaray);

signals:
    void changed(); // sent when multiple values have changed simulatenously (e.g. fromMap)

private:
    Constants::InjectorType m_injector;
    QVariantMap m_gammarayForQt;
};

/**
 * Global gammaray settings
 */
typedef GammarayBaseSettings GammarayGlobalSettings;

/**
 * Per-project gammaray settings.
 */
typedef GammarayBaseSettings GammarayProjectSettings;

} // namespace Gammaray

#endif // GAMMARAYSETTINGS_H
