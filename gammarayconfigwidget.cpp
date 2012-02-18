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

#include "gammarayconfigwidget.h"
#include "gammaraysettings.h"

#include "ui_gammarayconfigwidget.h"
#include "ui_gammarayversion.h"

#include <qtsupport/baseqtversion.h>
#include <qtsupport/qtversionmanager.h>

#include <utils/qtcassert.h>
#include <utils/pathchooser.h>

#include <QFileDialog>
#include <QTreeWidget>

using namespace Gammaray;

GammarayConfigWidget::GammarayConfigWidget(GammarayBaseSettings *settings,
                                           QWidget *parent)
    : QWidget(parent),
      m_settings(settings),
      m_ui(new Internal::Ui::GammarayConfigWidget),
      m_versionUi(new Internal::Ui::GammarayVersion)
{
    m_ui->setupUi(this);

    QWidget *versionInfoWidget = new QWidget();
    m_versionUi->setupUi(versionInfoWidget);

    m_ui->qtInfoWidget->setWidget(versionInfoWidget);
    m_ui->qtInfoWidget->setState(Utils::DetailsWidget::NoSummary);

    m_versionUi->gammarayExeChooser->setExpectedKind(Utils::PathChooser::ExistingCommand);
    m_versionUi->gammarayExeChooser->setPromptDialogTitle(tr("Gammaray Command"));

    m_ui->injectorCombo->addItem(tr("Default"), Constants::DefaultInjector);
#ifndef Q_OS_WIN
    m_ui->injectorCombo->addItem(tr("Preload"), Constants::PreloadInjector);
#else
    m_ui->injectorCombo->addItem(tr("WinDLL"), Constants::WinDLLInjector);
#endif
    m_ui->injectorCombo->addItem(tr("GDB"), Constants::GDBInjector);
    m_ui->injectorCombo->addItem(tr("Style"), Constants::StyleInjector);
    QTC_CHECK(m_ui->injectorCombo->count() == Constants::INJECTOR_COUNT);

    initialize();
    connect(m_ui->injectorCombo, SIGNAL(currentIndexChanged(int)),
            this, SLOT(selectInjector(int)));
    connect(m_ui->qtVersionTable, SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)),
            this, SLOT(selectQtVersion(QTreeWidgetItem*)));
    connect(m_versionUi->gammarayExeChooser, SIGNAL(changed(QString)),
            this, SLOT(setGammarayExecutable(QString)));
}

GammarayConfigWidget::~GammarayConfigWidget()
{
    delete m_ui;
}

void GammarayConfigWidget::initialize()
{
    QList<QtSupport::BaseQtVersion *> versions = QtSupport::QtVersionManager::instance()->versions();

    m_ui->qtVersionTable->clear();
    foreach(QtSupport::BaseQtVersion *version, versions) {
        QTreeWidgetItem *item = new QTreeWidgetItem(m_ui->qtVersionTable);
        item->setText(0, version->displayName());
        item->setText(1, version->qmakeCommand().toString());
        item->setText(2, m_settings->gammarayForQt(version->qmakeCommand().toString()));
    }
    m_ui->qtVersionTable->clearSelection();
    m_ui->qtInfoWidget->hide();
    m_versionUi->qtVersionName->clear();
    m_versionUi->gammarayExeChooser->setPath("");

    changeInjector(m_settings->injector());
}

void GammarayConfigWidget::selectInjector(int index)
{
    const QVariant data = m_ui->injectorCombo->itemData(index);

    bool ok = true;
    Constants::InjectorType type = static_cast<Constants::InjectorType>(data.toInt());
    QTC_ASSERT(ok && type >= 0 && type < Constants::INJECTOR_COUNT, return);

    m_settings->setInjector(type);
}

void GammarayConfigWidget::setGammarayExecutable(QString exec)
{
    QTreeWidgetItem *current = m_ui->qtVersionTable->currentItem();
    if (current) {
        m_settings->setGammarayForQt(current->text(1), exec);
    }
}

void GammarayConfigWidget::selectQtVersion(QTreeWidgetItem *current)
{
    if (!current) {
        m_ui->qtInfoWidget->hide();
    }
    else {
        m_ui->qtInfoWidget->show();
        m_versionUi->qtVersionName->setText(current->text(0));
        m_versionUi->gammarayExeChooser->setPath(current->text(2));
        m_versionUi->gammarayExeChooser->setEnabled(true);
    }
}

void GammarayConfigWidget::changeInjector(Constants::InjectorType type)
 {
    for (int i = 0; i < m_ui->injectorCombo->count(); ++i) {
        if (m_ui->injectorCombo->itemData(i).toInt() == type) {
            m_ui->injectorCombo->setCurrentIndex(i);
            return;
        }
    }
    // unhandled
    QTC_CHECK(false && "unhandled injector type");
    m_ui->injectorCombo->setCurrentIndex(0);
}
