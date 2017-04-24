/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt Charts module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 or (at your option) any later version
** approved by the KDE Free Qt Foundation. The licenses are as published by
** the Free Software Foundation and appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "view.h"
#include <QtGui/QResizeEvent>
#include <QtWidgets/QGraphicsScene>
#include <QtCharts/QChart>
#include <QtCharts/QSplineSeries>
#include <QtWidgets/QGraphicsTextItem>
#include <QtGui/QMouseEvent>
#include <QtCharts/QDateTimeAxis>
#include <QDate>
#include <QDebug>

#include <qtapp/callout.h>

ChartView::ChartView(QChart * chart, QLineSeries *series, QLineSeries *refseries, QWidget *parent)
    : QGraphicsView(new QGraphicsScene, parent),
      m_coordX(0), m_coordY(0), m_coordRefY(0),
      m_chart(0), m_tooltip(0),
      series(series), refseries(refseries)
{
    setDragMode(QGraphicsView::NoDrag);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setMinimumHeight(250);

    // chart
    m_chart = chart;

    //    m_chart->createDefaultAxes();
    m_chart->setAcceptHoverEvents(true);

    setRenderHint(QPainter::Antialiasing);
    scene()->addItem(m_chart);

    m_coordX = new QGraphicsSimpleTextItem(m_chart);
    m_coordX->setPos(m_chart->size().width()/2 -100, m_chart->size().height());
    m_coordX->setText("X: ");
    m_coordY = new QGraphicsSimpleTextItem(m_chart);
    m_coordY->setPos(m_chart->size().width()/2 -30, m_chart->size().height());
    m_coordY->setText("Y: ");
    m_coordRefY = new QGraphicsSimpleTextItem(m_chart);
    m_coordRefY->setPos(m_chart->size().width()/2 + 50, m_chart->size().height());
    m_coordRefY->setText("Ref: ");


    //    m_coordrefY->setPos(m_chart->size().width()/2 + 100, m_chart->size().height());
    //    m_coordrefY->setText("refY: ");

    //    connect(series, SIGNAL(clicked(QPointF)), this, SLOT(keepCallout()));
    //    connect(series, SIGNAL(hovered(QPointF, bool)), this, SLOT(tooltip(QPointF,bool)));
    //    connect(series2, SIGNAL(clicked(QPointF)), this, SLOT(keepCallout()));
    //    connect(series2, SIGNAL(hovered(QPointF, bool)), this, SLOT(tooltip(QPointF,bool)));

    this->setMouseTracking(true);
}

void ChartView::resizeEvent(QResizeEvent *event)
{
    if (scene()) {
        scene()->setSceneRect(QRect(QPoint(0, 0), event->size()));
        m_chart->resize(event->size());
        m_coordX->setPos(m_chart->size().width()/2 - 100, m_chart->size().height() - 20);
        m_coordY->setPos(m_chart->size().width()/2 - 30, m_chart->size().height() - 20);
        m_coordRefY->setPos(m_chart->size().width()/2 + 50, m_chart->size().height() - 20);
        foreach (Callout *callout, m_callouts)
            callout->updateGeometry();
    }
    QGraphicsView::resizeEvent(event);
}

void ChartView::mouseMoveEvent(QMouseEvent *event)
{

    double startX = ((QDateTimeAxis*)(m_chart->axisX(series)))->min().toMSecsSinceEpoch();
    double endX = ((QDateTimeAxis*)(m_chart->axisX(series)))->max().toMSecsSinceEpoch();
    double xValue = m_chart->mapToValue(event->pos()).x();
    double xPos = (startX - xValue)/(startX-endX);
    double serieIdx = qRound(xPos * series->points().size());

    m_coordX->setText(QString("X: %1").arg(QDateTime::fromMSecsSinceEpoch(xValue).toString("dd-MM")));
    if(serieIdx >=0 && serieIdx < series->points().size())
        m_coordY->setText(QString("Y: %1").arg(series->at(serieIdx).y()));
    if(refseries != NULL)
        if(serieIdx >=0 && serieIdx < refseries->points().size())
            m_coordRefY->setText(QString("Ref: %1").arg(refseries->at(serieIdx).y()));

    QGraphicsView::mouseMoveEvent(event);
}

void ChartView::keepCallout()
{
    m_callouts.append(m_tooltip);
    m_tooltip = new Callout(m_chart);
}

void ChartView::tooltip(QPointF point, bool state)
{
    if (m_tooltip == 0)
        m_tooltip = new Callout(m_chart);

    if (state) {
        m_tooltip->setText(QString("%1").arg(point.y()));
        m_tooltip->setAnchor(point);
        m_tooltip->setZValue(11);
        m_tooltip->updateGeometry();
        m_tooltip->show();
    } else {
        m_tooltip->hide();
    }
}
