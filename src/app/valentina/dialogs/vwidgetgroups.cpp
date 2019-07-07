/************************************************************************
 **
 **  @file   vwidgetgroups.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   6 4, 2016
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

#include "vwidgetgroups.h"
#include "ui_vwidgetgroups.h"
#include "../vtools/dialogs/tools/dialoggroup.h"
#include "../vtools/undocommands/delgroup.h"
#include "../vpatterndb/vcontainer.h"

#include <QMenu>
#include <QTableWidget>

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief
 *
 * @param doc
 * @param parent
 */
VWidgetGroups::VWidgetGroups(VAbstractPattern *doc, QWidget *parent)
    : QWidget(parent),
      ui(new Ui::VWidgetGroups),
      doc(doc)
{
    ui->setupUi(this);

    FillTable(doc->GetGroups());

    ui->tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(ui->tableWidget, &QTableWidget::cellClicked, this, &VWidgetGroups::GroupVisibilityChanged);
    connect(ui->tableWidget, &QTableWidget::cellChanged, this, &VWidgetGroups::RenameGroup);
    connect(ui->tableWidget, &QTableWidget::customContextMenuRequested, this, &VWidgetGroups::CtxMenu);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief
 *
 */
VWidgetGroups::~VWidgetGroups()
{
    delete ui;
}
//----------------------------------------------------------------------------------------------------------------------

/**
 * @brief
 *set visibility and icon value
 * @param id
 * @param visible
 * @param item
 * @param openEye
 * @param closedEye
 */
void VWidgetGroups::SetIconValue(quint32 id, bool visible, QTableWidgetItem *item) const
{
    doc->SetGroupVisivility(id, visible);
    (visible) ? item->setIcon(QIcon( QStringLiteral("://icon/16x16/open_eye.png")))
              : item->setIcon(QIcon(QStringLiteral("://icon/16x16/closed_eye.png")));
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief
 *
 * @param row
 * @param column
 */
void VWidgetGroups::GroupVisibilityChanged(int row, int column)
{
    if (column != 0)
    {
        return;
    }
    QTableWidgetItem *item = ui->tableWidget->item(row, column);
    const quint32 id = item->data(Qt::UserRole).toUInt();
    const bool visible = not doc->GetGroupVisivility(id);
    SetIconValue(id, visible, item);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief
 *
 * @param row
 * @param column
 */
void VWidgetGroups::RenameGroup(int row, int column)
{
    if (column != 1)
    {
        return;
    }

    const quint32 id = ui->tableWidget->item(row, 0)->data(Qt::UserRole).toUInt();
    doc->SetGroupName(id, ui->tableWidget->item(row, column)->text());

    UpdateGroups();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief
 *
 * @param pos
 */
void VWidgetGroups::CtxMenu(const QPoint &pos)
{
    QTableWidgetItem *item = ui->tableWidget->itemAt(pos);
    if(not item)
    {
        return;
    }

    const int row = item->row();
    item = ui->tableWidget->item(row, 0);
    const quint32 id = item->data(Qt::UserRole).toUInt();

    QScopedPointer<QMenu> menu(new QMenu());
    QAction *actionRename = menu->addAction(tr("Rename"));
    QAction *actionDelete = menu->addAction(tr("Delete"));

    menu->addSeparator();//add separator to context menu

    QAction *actionHideAll = menu->addAction(tr("Hide All"));//add to context menu
    QAction *actionShowAll = menu->addAction(tr("Show All"));//add to context menu

    menu->addSeparator();//add separator

    /*
     * add two options to the context menu. Hide if visible row and
     * Show if hidden one.
     */
    QAction *hiddenMenu = new QAction(this);
    QAction *showMenu = new QAction(this);
    if (doc->GetGroupVisivility(id))
    {
        hiddenMenu = menu->addAction(tr("Hide"));
        showMenu = menu->addAction(tr("Show"));
        showMenu->setDisabled(true);
    }
    else
    {
        showMenu = menu->addAction(tr("Show"));
        hiddenMenu = menu->addAction(tr("Hide"));
        hiddenMenu->setDisabled(true);
    }


    QAction *selectedAction = menu->exec(
                ui->tableWidget->viewport()->mapToGlobal(pos));



    if (selectedAction == actionRename)
    {
        DialogGroup *dialog = new DialogGroup(new VContainer(qApp->TrVars(), qApp->patternUnitP(),
                                                             VContainer::UniqueNamespace()),
                                              NULL_ID, this);
        dialog->SetName(doc->GetGroupName(id));
        const int result = dialog->exec();

        if (result == QDialog::Accepted)
        {
            doc->SetGroupName(id, dialog->GetName());
            item = ui->tableWidget->item(row, 1);
            item->setText(dialog->GetName());
        }
        delete dialog;
    }
    else if (selectedAction == actionDelete)
    {
        DelGroup *delGroup = new DelGroup(doc, id);
        connect(delGroup, &DelGroup::UpdateGroups, this, &VWidgetGroups::UpdateGroups);
        qApp->getUndoStack()->push(delGroup);
    }
    //all groups in "group" make unvisible
    else if (selectedAction == actionHideAll)
    {
        for (int r = 0; r < ui->tableWidget->rowCount(); ++r)
        {
            QTableWidgetItem *rowItem = ui->tableWidget->item(r, 0);
            quint32 i = rowItem->data(Qt::UserRole).toUInt();
            SetIconValue(i, false, rowItem);
        }
    }

    //all groups in "group" make visible
    else if (selectedAction == actionShowAll)
    {
        for (int r = 0; r < ui->tableWidget->rowCount(); ++r)
        {
            QTableWidgetItem *rowItem = ui->tableWidget->item(r, 0);
            quint32 i = rowItem->data(Qt::UserRole).toUInt();
            SetIconValue(i, true, rowItem);
        }
    }
    //implement of Hide and Show selected row
    else if (selectedAction == hiddenMenu)
    {
        SetIconValue(id, false, item);
    }
    else if (selectedAction == showMenu)
    {
        SetIconValue(id, true, item);
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief
 *
 */
void VWidgetGroups::UpdateGroups()
{
    FillTable(doc->GetGroups());
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief
 *
 * @param QMap<quint32
 * @param QPair<QString
 * @param groups
 */
void VWidgetGroups::FillTable(const QMap<quint32, QPair<QString, bool> > &groups)
{
    ui->tableWidget->blockSignals(true);
    ui->tableWidget->clear();

    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->setRowCount(groups.size());
    qint32 currentRow = -1;
    auto i = groups.constBegin();
    while (i != groups.constEnd())
    {
        ++currentRow;
        const QPair<QString, bool> data = i.value();

        QTableWidgetItem *item = new QTableWidgetItem();
        item->setTextAlignment(Qt::AlignHCenter);
        quint32 id = item->data(Qt::UserRole).toUInt();
        if (data.second)
        {
            SetIconValue(id, true, item);
        }


        item->setData(Qt::UserRole, i.key());

        // set the item non-editable (view only), and non-selectable
        Qt::ItemFlags flags = item->flags();
        flags &= ~(Qt::ItemIsEditable); // reset/clear the flag
        item->setFlags(flags);

        ui->tableWidget->setItem(currentRow, 0, item);

        item = new QTableWidgetItem(data.first);
        item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);

        if(doc->GroupIsEmpty(i.key()))
        {
            QFont font = item->font();
            font.setStrikeOut(true);
            item->setFont(font);
        }

        ui->tableWidget->setItem(currentRow, 1, item);
        ++i;
    }
    ui->tableWidget->sortItems(1, Qt::AscendingOrder);
    ui->tableWidget->resizeColumnsToContents();
    ui->tableWidget->resizeRowsToContents();
    ui->tableWidget->blockSignals(false);
}
