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

#include "gammaraysettings.h"
#include "gammarayconfigwidget.h"

#include <coreplugin/icore.h>
#include <utils/qtcassert.h>

#include <QSettings>

using namespace Analyzer;

using namespace Gammaray;

namespace  {
static const QLatin1String injectorC("Analyzer.Gammaray.Injector");
static const QLatin1String gammarayForQtC("Analyzer.Gammaray.GammarayForQt");
}

QVariantMap GammarayBaseSettings::toMap() const
{
    QVariantMap map;

    map.insert(injectorC, m_injector);
    map.insert(gammarayForQtC, m_gammarayForQt);

    return map;
}

QVariantMap GammarayBaseSettings::defaults() const
{
    QVariantMap map;

    map.insert(injectorC, Constants::DefaultInjector);
    map.insert(gammarayForQtC, QVariantMap());

    return map;
}

void GammarayBaseSettings::fromMap(const QVariantMap &map)
{
    int injector = 0;
    setIfPresent(map, injectorC, &injector);
    QTC_CHECK(injector >= 0 && injector < Constants::INJECTOR_COUNT);
    m_injector = static_cast<Constants::InjectorType>(injector);

    setIfPresent(map, gammarayForQtC, &m_gammarayForQt);

    emit changed();
}

QString GammarayBaseSettings::id() const
{
    return "Analyzer.Gammaray.Settings";
}

QString GammarayBaseSettings::displayName() const
{
    return tr("Gammaray");
}

Constants::InjectorType GammarayBaseSettings::injector() const
{
    return m_injector;
}

QString GammarayBaseSettings::gammarayForQt(const QString &qmakePath) const
{
    if (m_gammarayForQt.contains(qmakePath))
        return m_gammarayForQt.value(qmakePath).toString();
    return QString();
}

void GammarayBaseSettings::setInjector(Constants::InjectorType injector)
{
    m_injector = injector;
    emit changed();
}

void GammarayBaseSettings::setGammarayForQt(const QString &qmakePath, const QString &gammaray)
{
    m_gammarayForQt[qmakePath] = gammaray;
    emit changed();
}

QWidget *GammarayBaseSettings::createConfigWidget(QWidget *parent)
{
    return new GammarayConfigWidget(this, parent);
}
