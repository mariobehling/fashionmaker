/************************************************************************
 **
 **  @file   dialogexporttocsv.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   1 6, 2016
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2016 Valentina project
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

#include "dialogexporttocsv.h"
#include "ui_dialogexporttocsv.h"

#include "../vmisc/vcommonsettings.h"
#include "../vabstractapplication.h"

#include <QPushButton>
#include <QShowEvent>
#include <QTextCodec>

//---------------------------------------------------------------------------------------------------------------------
DialogExportToCSV::DialogExportToCSV(QWidget *parent)
    : QDialog(parent),
      ui(new Ui::DialogExportToCSV),
      isInitialized(false)
{
    ui->setupUi(this);

    foreach (int mib, QTextCodec::availableMibs())
    {
        ui->comboBoxCodec->addItem(QTextCodec::codecForMib(mib)->name(), mib);
    }

    ui->comboBoxCodec->setCurrentIndex(ui->comboBoxCodec->findData(VCommonSettings::GetDefCSVCodec()));

    SetSeparator(VCommonSettings::GetDefCSVSeparator());

    QPushButton *bDefaults = ui->buttonBox->button(QDialogButtonBox::RestoreDefaults);
    SCASSERT(bDefaults != nullptr)
    connect(bDefaults, &QPushButton::clicked, this, [this]()
    {
        ui->checkBoxWithHeader->setChecked(qApp->Settings()->GetDefCSVWithHeader());
        ui->comboBoxCodec->setCurrentIndex(ui->comboBoxCodec->findData(VCommonSettings::GetDefCSVCodec()));

        SetSeparator(VCommonSettings::GetDefCSVSeparator());
    });
}

//---------------------------------------------------------------------------------------------------------------------
DialogExportToCSV::~DialogExportToCSV()
{
    delete ui;
}

//---------------------------------------------------------------------------------------------------------------------
bool DialogExportToCSV::IsWithHeader() const
{
    return ui->checkBoxWithHeader->isChecked();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogExportToCSV::SetWithHeader(bool value)
{
    ui->checkBoxWithHeader->setChecked(value);
}

//---------------------------------------------------------------------------------------------------------------------
int DialogExportToCSV::GetSelectedMib() const
{
    if (ui->comboBoxCodec->currentIndex() != -1)
    {
        return ui->comboBoxCodec->currentData().toInt();
    }
    else
    {
        return VCommonSettings::GetDefCSVCodec();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogExportToCSV::SetSelectedMib(int value)
{
    const int index = ui->comboBoxCodec->findData(value);
    if (index != -1)
    {
        ui->comboBoxCodec->setCurrentIndex(index);
    }
    else
    {
        ui->comboBoxCodec->setCurrentIndex(ui->comboBoxCodec->findData(VCommonSettings::GetDefCSVCodec()));
    }
}

//---------------------------------------------------------------------------------------------------------------------
QChar DialogExportToCSV::GetSeparator() const
{
    if (ui->radioButtonTab->isChecked())
    {
        return QChar('\t');
    }
    else if (ui->radioButtonSemicolon->isChecked())
    {
        return QChar(';');
    }
    else if (ui->radioButtonSpace->isChecked())
    {
        return QChar(' ');
    }
    else
    {
        return VCommonSettings::GetDefCSVSeparator();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogExportToCSV::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange)
    {
        // retranslate designer form (single inheritance approach)
        ui->retranslateUi(this);
    }

    // remember to call base class implementation
    QDialog::changeEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogExportToCSV::showEvent(QShowEvent *event)
{
    QDialog::showEvent( event );
    if ( event->spontaneous() )
    {
        return;
    }

    if (isInitialized)
    {
        return;
    }
    // do your init stuff here

    setMaximumSize(size());
    setMinimumSize(size());

    isInitialized = true;//first show windows are held
}

//---------------------------------------------------------------------------------------------------------------------
void DialogExportToCSV::SetSeparator(const QChar &separator)
{
    switch(separator.toLatin1())
    {
        case '\t':
            ui->radioButtonTab->setChecked(true);
            break;
        case ';':
            ui->radioButtonSemicolon->setChecked(true);
            break;
        case ' ':
            ui->radioButtonSpace->setChecked(true);
            break;
        case ',':
        default:
            ui->radioButtonComma->setChecked(true);
            break;
    }
}

//---------------------------------------------------------------------------------------------------------------------
QString DialogExportToCSV::MakeHelpCodecsList()
{
    QString out("\n");
    const QList<int> list = QTextCodec::availableMibs();
    for (int i = 0; i < list.size(); ++i)
    {
        out += QLatin1String("\t* ") + QTextCodec::codecForMib(list.at(i))->name();
        if (i < list.size()-1)
        {
            out += QLatin1String(",\n");
        }
        else
        {
            out += QLatin1String(".\n");
        }
    }
    return out;
}

//---------------------------------------------------------------------------------------------------------------------
QString DialogExportToCSV::MakeHelpSeparatorList()
{
    QString out("\n");
    out += QLatin1String("\t* 'Tab',\n");
    out += QLatin1String("\t* ';',\n");
    out += QLatin1String("\t* 'Space',\n");
    out += QLatin1String("\t* ','.\n");
    return out;
}
