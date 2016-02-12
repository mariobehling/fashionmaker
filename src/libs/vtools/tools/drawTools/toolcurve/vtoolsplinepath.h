/************************************************************************
 **
 **  @file   vtoolsplinepath.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   November 15, 2013
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013-2015 Valentina project
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

#ifndef VTOOLSPLINEPATH_H
#define VTOOLSPLINEPATH_H

#include "vabstractspline.h"

class VSplinePath;
class VSplinePoint;
class VSpline;

/**
 * @brief The VToolSplinePath class tool for creation spline path.
 */
class VToolSplinePath:public VAbstractSpline
{
    Q_OBJECT
public:
    VToolSplinePath(VAbstractPattern *doc, VContainer *data, quint32 id, const QString &color,
                    const Source &typeCreation,
                    QGraphicsItem * parent = nullptr);
    virtual ~VToolSplinePath() Q_DECL_OVERRIDE;
    virtual void setDialog();
    static VToolSplinePath *Create(DialogTool *dialog, VMainGraphicsScene  *scene, VAbstractPattern *doc,
                                   VContainer *data);
    static VToolSplinePath *Create(const quint32 _id, VSplinePath *path, const QString &color,
                                   VMainGraphicsScene  *scene, VAbstractPattern *doc, VContainer *data,
                                   const Document &parse,
                                   const Source &typeCreation);
    static const QString ToolType;
    static void  UpdatePathPoint(VAbstractPattern *doc, QDomNode& node, const VSplinePath &path);
    virtual int  type() const Q_DECL_OVERRIDE {return Type;}
    enum { Type = UserType + static_cast<int>(Tool::SplinePath)};

    VSplinePath getSplinePath()const;
    void        setSplinePath(const VSplinePath &splPath);

    virtual void ShowVisualization(bool show) Q_DECL_OVERRIDE;
signals:
    /**
     * @brief RefreshLine refresh control line.
     * @param indexSpline position spline in spline list.
     * @param pos position point in spline.
     * @param controlPoint new position control point.
     * @param splinePoint new position spline point.
     */
    void          RefreshLine(const qint32 &indexSpline, SplinePointPosition pos, const QPointF &controlPoint,
                              const QPointF &splinePoint);
    /**
     * @brief setEnabledPoint disable control points.
     * @param enable enable or diasable points.
     */
    void          setEnabledPoint(bool enable);
public slots:

    void          ControlPointChangePosition(const qint32 &indexSpline, const SplinePointPosition &position,
                                             const QPointF &pos);
    virtual void  EnableToolMove(bool move) Q_DECL_OVERRIDE;
protected:
    virtual void  contextMenuEvent ( QGraphicsSceneContextMenuEvent * event ) Q_DECL_OVERRIDE;
    virtual void  RefreshDataInFile() Q_DECL_OVERRIDE;
    virtual void  RemoveReferens() Q_DECL_OVERRIDE;
    virtual void  SaveDialog(QDomElement &domElement) Q_DECL_OVERRIDE;
    virtual void  SaveOptions(QDomElement &tag, QSharedPointer<VGObject> &obj) Q_DECL_OVERRIDE;
    virtual void  mousePressEvent(QGraphicsSceneMouseEvent * event) Q_DECL_OVERRIDE;
    virtual void  mouseReleaseEvent ( QGraphicsSceneMouseEvent * event ) Q_DECL_OVERRIDE;
    virtual void  mouseMoveEvent(QGraphicsSceneMouseEvent * event) Q_DECL_OVERRIDE;
    virtual void  hoverEnterEvent ( QGraphicsSceneHoverEvent * event ) Q_DECL_OVERRIDE;
    virtual void  hoverLeaveEvent ( QGraphicsSceneHoverEvent * event ) Q_DECL_OVERRIDE;
    virtual void  SetVisualization() Q_DECL_OVERRIDE;
private:
    QPointF oldPosition;

    void          RefreshGeometry();
    static void   AddPathPoint(VAbstractPattern *doc, QDomElement &domElement, const VSplinePoint &splPoint);
    void          UpdateControlPoints(const VSpline &spl, VSplinePath &splPath, const qint32 &indexSpline) const;
    void          RefreshSplinePath(VSplinePath &splPath);
};

#endif // VTOOLSPLINEPATH_H
