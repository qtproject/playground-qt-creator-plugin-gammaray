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

#ifndef GAMMARAYCONFIGWIDGET_H
#define GAMMARAYCONFIGWIDGET_H

#include <QWidget>

#include "gammarayconstants.h"

QT_BEGIN_NAMESPACE
class QTreeWidgetItem;
QT_END_NAMESPACE

namespace Gammaray {

namespace Internal {
namespace Ui {
class GammarayConfigWidget;
class GammarayVersion;
}
}

class GammarayBaseSettings;

class GammarayConfigWidget : public QWidget
{
    Q_OBJECT

public:
    GammarayConfigWidget(GammarayBaseSettings *settings, QWidget *parent);
    virtual ~GammarayConfigWidget();

private:
    void changeInjector(Constants::InjectorType type);

private slots:
    void initialize();
    void selectInjector(int);
    void setGammarayExecutable(QString);
    void selectQtVersion(QTreeWidgetItem*);

private:
    GammarayBaseSettings *m_settings;
    Internal::Ui::GammarayConfigWidget *m_ui;
    Internal::Ui::GammarayVersion *m_versionUi;
};

} // namespace Gammaray

#endif // GAMMARAYCONFIGWIDGET_H
