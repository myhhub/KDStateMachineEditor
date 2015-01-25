/*
  quickmaskedmousearea.h

  This file is part of the KDAB State Machine Editor Library.

  Copyright (C) 2014-2015 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com.
  All rights reserved.
  Author: Kevin Funk <kevin.funk@kdab.com>

  Licensees holding valid commercial KDAB State Machine Editor Library
  licenses may use this file in accordance with the KDAB State Machine Editor
  Library License Agreement provided with the Software.

  This file may be distributed and/or modified under the terms of the
  GNU Lesser General Public License version 2.1 as published by the
  Free Software Foundation and appearing in the file LICENSE.LGPL.txt included.

  This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
  WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.

  Contact info@kdab.com if any conditions of this licensing are not
  clear to you.
*/

#ifndef KDSME_QUICK_QUICKMASKEDMOUSEAREA_H
#define KDSME_QUICK_QUICKMASKEDMOUSEAREA_H

#include <QObject>
#include <QPainterPath>
#include <QQuickItem>

class AbstractMask : public QObject
{
    Q_OBJECT
public:
    explicit AbstractMask(QObject* parent = nullptr);

    virtual bool contains(const QPointF& point) const = 0;
    virtual bool intersects(const QRectF& rect) const = 0;
};

Q_DECLARE_METATYPE(AbstractMask*)

class PainterPathMask : public AbstractMask
{
    Q_OBJECT
    Q_PROPERTY(QPainterPath path READ path WRITE setPath NOTIFY pathChanged)

public:
    explicit PainterPathMask(QObject* parent = nullptr);

    QPainterPath path() const;
    void setPath(const QPainterPath& path);

    virtual bool contains(const QPointF& point) const override;
    virtual bool intersects(const QRectF& rect) const override;

Q_SIGNALS:
    void pathChanged(const QPainterPath& path);

private:
    QPainterPath m_path;
};

class QuickMaskedMouseArea : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(bool pressed READ isPressed NOTIFY pressedChanged)
    Q_PROPERTY(bool containsMouse READ containsMouse NOTIFY containsMouseChanged)
    Q_PROPERTY(AbstractMask* mask READ mask WRITE setMask NOTIFY maskChanged)
    Q_PROPERTY(qreal tolerance READ tolerance WRITE setTolerance NOTIFY toleranceChanged)

public:
    explicit QuickMaskedMouseArea(QQuickItem* parent = nullptr);

    bool isPressed() const { return m_pressed; }
    bool containsMouse() const { return m_containsMouse; }

    AbstractMask* mask() const;
    void setMask(AbstractMask* mask);

    qreal tolerance() const;
    void setTolerance(qreal tolerance);

    virtual bool contains(const QPointF& point) const override;

Q_SIGNALS:
    void pressed();
    void released();
    void clicked();
    void canceled();
    void pressedChanged(bool pressed);
    void containsMouseChanged(bool containsMouse);
    void maskChanged(AbstractMask* mask);
    void toleranceChanged(qreal tolerance);

protected:
    void setPressed(bool pressed);
    void setContainsMouse(bool containsMouse);

    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void hoverEnterEvent(QHoverEvent *event) override;
    virtual void hoverLeaveEvent(QHoverEvent *event) override;
    virtual void mouseUngrabEvent() override;

private:
    bool m_pressed;
    bool m_containsMouse;
    QPointF m_pressPoint;
    AbstractMask* m_mask;
    qreal m_tolerance;
};

#endif // QUICKMASKEDMOUSEAREA_H
