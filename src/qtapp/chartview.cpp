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

#include "chartview.h"
#include <QtGui/QResizeEvent>
#include <QtWidgets/QGraphicsScene>
#include <QtCharts/QChart>
#include <QtWidgets/QGraphicsTextItem>
#include <QtGui/QMouseEvent>

#include <QtCharts/QDateTimeAxis>
#include <QtCharts/QValueAxis>

#include <QDate>
#include <qtapp/callout.h>

ChartView::ChartView(QString name, QWidget *parent)
    : QGraphicsView(new QGraphicsScene, parent),
      m_coordX(0), m_coordY(0), m_coordRefY(0),
      m_chart(0), m_tooltip(0) {
    series = nullptr;
    obsSeries = nullptr;
    setDragMode(QGraphicsView::NoDrag);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setMinimumHeight(250);
    setMaximumHeight(250);

    m_chart = new QChart();
    m_chart->legend()->hide();
    m_chart->setTitle(name);
    m_chart->setAcceptHoverEvents(true);
    scene()->addItem(m_chart);

    m_coordX = new QGraphicsSimpleTextItem(m_chart);
    m_coordX->setPos(m_chart->size().width() / 2 - 40, m_chart->size().height());
    m_coordX->setText("X: ");
    m_coordY = new QGraphicsSimpleTextItem(m_chart);
    m_coordY->setPos(m_chart->size().width() / 2 + 20, m_chart->size().height());
    m_coordY->setText("Y: ");

    //  m_coordRefY = new QGraphicsSimpleTextItem(m_chart);
    //  m_coordRefY->setPos(m_chart->size().width() / 2 + 50, m_chart->size().height());
    //  m_coordRefY->setText("Obs: ");

    //    m_coordrefY->setPos(m_chart->size().width()/2 + 100, m_chart->size().height());
    //    m_coordrefY->setText("refY: ");
    //    connect(series, SIGNAL(clicked(QPointF)), this, SLOT(keepCallout()));
    //    connect(series, SIGNAL(hovered(QPointF, bool)), this, SLOT(tooltip(QPointF,bool)));
    //    connect(series2, SIGNAL(clicked(QPointF)), this, SLOT(keepCallout()));
    //    connect(series2, SIGNAL(hovered(QPointF, bool)), this, SLOT(tooltip(QPointF,bool)));

    this->setMouseTracking(true);
}

void ChartView::setSeries(QLineSeries *series, QScatterSeries *obsSeries) {
    double yMin = qInf();
    double yMax = -qInf();
    if(series != nullptr) {
        series->attachAxis(m_chart->axisX());
        series->attachAxis(m_chart->axisY());
        m_chart->addSeries(series);
        this->series = series;
        for (int i = 0; i < series->count(); ++i) {
            double x = series->at(i).x();
            double y = series->at(i).y();
            yMin = qMin(y,yMin);
            yMax = qMax(y,yMax);
            if(x == 0)
                sowing = i;
        }
    }

    if(obsSeries != nullptr) {
        obsSeries->attachAxis(m_chart->axisX());
        obsSeries->attachAxis(m_chart->axisY());
        m_chart->addSeries(obsSeries);
        this->obsSeries = obsSeries;
    }
    m_chart->createDefaultAxes();
    if(yMax < 0.5) {
        m_chart->axisY()->setMin(yMin);
        m_chart->axisY()->setMax(yMax * 1.1);
    }
    qobject_cast<QValueAxis*>(m_chart->axisY())->applyNiceNumbers();
//    m_chart->axisX()->setTitleText("DAS");
}


void ChartView::clear() {
    m_chart->removeAllSeries();
    series = nullptr;
    obsSeries = nullptr;
}

void ChartView::setSowing(bool sowing){
    if(sowing) {
        QRectF rect = m_chart->plotArea();
        double duration = series->at(series->count()-1).x() - series->at(0).x();
        double zeroPos = -series->at(0).x() / duration;
        rect.adjust(zeroPos*rect.width(),0,0,0);
        m_chart->zoomIn(rect);
    } else {
        m_chart->zoomReset();
    }
}

void ChartView::resizeEvent(QResizeEvent *event) {
    if (scene()) {
        scene()->setSceneRect(QRect(QPoint(0, 0), event->size()));
        m_chart->resize(event->size());
        m_coordX->setPos(m_chart->size().width() / 2 - 40, m_chart->size().height() - 20);
        m_coordY->setPos(m_chart->size().width() / 2 + 20, m_chart->size().height() - 20);
        foreach (Callout *callout, m_callouts)
            callout->updateGeometry();
    }
    QGraphicsView::resizeEvent(event);
}

void ChartView::mouseMoveEvent(QMouseEvent *event) {
    if (series != nullptr) {
        double xValue = m_chart->mapToValue(event->pos()).x();
        m_coordX->setText(QString("X: %1").arg(xValue));
        int serieIdx = (int)(xValue - series->at(0).x());
        if (serieIdx >= 0 && serieIdx < series->points().size())
            m_coordY->setText(QString("Y: %1").arg(series->at(serieIdx).y()));
    }
    QGraphicsView::mouseMoveEvent(event);
}

void ChartView::keepCallout() {
    m_callouts.append(m_tooltip);
    m_tooltip = new Callout(m_chart);
}

void ChartView::tooltip(QPointF point, bool state) {
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
