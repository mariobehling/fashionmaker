/************************************************************************
 **
 **  @file   vpointfproperty.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   27 8, 2014
 **
 **  @brief
 **  @copyright
 **  All rights reserved. This program and the accompanying materials
 **  are made available under the terms of the GNU Lesser General Public License
 **  (LGPL) version 2.1 which accompanies this distribution, and is available at
 **  http://www.gnu.org/licenses/lgpl-2.1.html
 **
 **  This library is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 **  Lesser General Public License for more details.
 **
 *************************************************************************/

#include "vpointfproperty.h"

#include <QFlags>
#include <QList>
#include <QPointF>

#include "../vproperty_p.h"
#include "vnumberproperty.h"

VPE::VPointFProperty::VPointFProperty(const QString &name)
    : VProperty(name, QVariant::PointF)
{
    d_ptr->VariantValue.setValue(0);
    d_ptr->VariantValue.convert(QVariant::PointF);

    VDoubleProperty* tmpX = new VDoubleProperty("X");
    addChild(tmpX);
    tmpX->setUpdateBehaviour(true, false);

    VDoubleProperty* tmpY = new VDoubleProperty("Y");
    addChild(tmpY);
    tmpY->setUpdateBehaviour(true, false);

    setValue(QPointF());
}

QVariant VPE::VPointFProperty::data(int column, int role) const
{
    if (column == DPC_Data && Qt::DisplayRole == role)
    {
        return getPointF();
    }
    else
        return VProperty::data(column, role);
}

Qt::ItemFlags VPE::VPointFProperty::flags(int column) const
{
    if (column == DPC_Name || column == DPC_Data)
    {
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    }
    else
        return Qt::NoItemFlags;
}

QPointF VPE::VPointFProperty::getPointF() const
{
    QPointF tmpValue;

    if (d_ptr->Children.count() < 2)
    {
        return tmpValue;
    }

    tmpValue.setX(d_ptr->Children.at(0)->getValue().toDouble());
    tmpValue.setY(d_ptr->Children.at(1)->getValue().toDouble());

    return tmpValue;
}

void VPE::VPointFProperty::setPointF(const QPointF &point)
{
    setPointF(point.x(), point.y());
}

void VPE::VPointFProperty::setPointF(qreal x, qreal y)
{
    if (d_ptr->Children.count() < 2)
    {
        return;
    }

    QVariant tmpX(x);
    tmpX.convert(QVariant::Double);

    QVariant tmpY(y);
    tmpY.convert(QVariant::Double);

    d_ptr->Children.at(0)->setValue(tmpX);
    d_ptr->Children.at(1)->setValue(tmpY);
}

QString VPE::VPointFProperty::type() const
{
    return "pointF";
}

VPE::VProperty *VPE::VPointFProperty::clone(bool include_children, VProperty *container) const
{
    if (!container)
    {
        container = new VPointFProperty(getName());

        if (!include_children)
        {
            const QList<VProperty*> tmpChildren = container->getChildren();
            for(auto tmpChild : tmpChildren)
            {
                container->removeChild(tmpChild);
                delete tmpChild;
            }
        }
    }

    return VProperty::clone(false, container);  // Child
}

void VPE::VPointFProperty::setValue(const QVariant &value)
{
    QPointF tmpPoint = value.toPointF();
    setPointF(tmpPoint);
}

QVariant VPE::VPointFProperty::getValue() const
{
    QPointF tmpValue = getPointF();
    return QString("%1,%2").arg(QString::number(tmpValue.x()), QString::number(tmpValue.y()));
}
