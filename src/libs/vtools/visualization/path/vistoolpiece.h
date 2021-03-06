/************************************************************************
 **
 **  @file
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   5 11, 2016
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2016 Valentina project
 **  <https://gitlab.com/smart-pattern/valentina> All Rights Reserved.
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

#ifndef VISTOOLPIECE_H
#define VISTOOLPIECE_H

#include <QtCore/QObject>
#include <QtGlobal>

#include "vispath.h"
#include "../vpatterndb/vpiece.h"
#include "../vgeometry/vpointf.h"

class VisToolPiece : public VisPath
{
    Q_OBJECT
public:
    VisToolPiece(const VContainer *data, QGraphicsItem *parent = nullptr);
    virtual ~VisToolPiece() Q_DECL_EQ_DEFAULT;

    virtual void RefreshGeometry() override;
    void         SetPiece(const VPiece &piece);
    virtual int  type() const override {return Type;}
    enum { Type = UserType + static_cast<int>(Vis::ToolPiece)};
private:
    Q_DISABLE_COPY(VisToolPiece)
    QVector<VScaledEllipse *> m_points;
    QVector<VCurvePathItem *> m_curves;

    VScaledLine *m_line1;
    VScaledLine *m_line2;
    VPiece m_piece;
    bool m_pieceCached;
    QPainterPath m_cachedMainPath;
    QVector<VPointF> m_cachedNodes;
    QVector<QPointF> m_cachedMainPathPoints;
    QVector<QPainterPath> m_cachedCurvesPath;

    VScaledEllipse* GetPoint(quint32 i, const QColor &color);
    VCurvePathItem *GetCurve(quint32 i, const QColor &color);

    void HideAllItems();
};

#endif // VISTOOLPIECE_H
