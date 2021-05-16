#pragma once

#include <qcustomplot/qcustomplot.h>

#include <QColor>
#include <QGridLayout>
#include <QPen>
#include <QWidget>

#include <memory>
#include <string>
#include <vector>

namespace Audilets::UI
{
  class QPlotWidget : public QWidget
  {
  public:

    QPlotWidget(QWidget* parent = nullptr) :
      QWidget(parent)
    {
      layout = std::make_shared<QGridLayout>();
      setLayout(layout.get());
    }

    template<const size_t row, const size_t col>
    void addPlot(const size_t numberOfGraphs = 1)
    {
      assert(numberOfGraphs > 0);

      auto plot = std::make_shared<QCustomPlot>();

      for (size_t i = 0; i < numberOfGraphs; ++i)
      {
        plot->addGraph();

        QPen pen;
        pen.setColor(getLineColor(i));
        pen.setWidth(getLineWidth(i));

        plot->graph(i)->setPen(pen);
      }

      layout->addWidget(plot.get(), row, col);

      plots.push_back(plot);
    }

    template<const size_t row, const size_t col>
    void setPlotRangeX(const double min, const double max)
    {
      auto plot = getPlot(row, col);

      plot->xAxis->setRange(min, max);
    }

    template<const size_t row, const size_t col>
    void setPlotRangeY(const double min, const double max)
    {
      auto plot = getPlot(row, col);

      plot->yAxis->setRange(min, max);
    }

    template<const size_t row, const size_t col>
    void setPlotLabelX(const std::string& value)
    {
      auto plot = getPlot(row, col);

      plot->xAxis->setLabel(QString::fromStdString(value));
    }

    template<const size_t row, const size_t col>
    void setPlotLabelY(const std::string& value)
    {
      auto plot = getPlot(row, col);

      plot->yAxis->setLabel(QString::fromStdString(value));
    }

    template<const size_t row, const size_t col, const size_t graph = 0>
    void setPlotLegend(const std::string& value)
    {
      auto plot = getPlot(row, col);

      plot->graph(graph)->setName(QString::fromStdString(value));
    }

    template<const size_t row, const size_t col>
    void replot()
    {
      auto plot = getPlot(row, col);

      // FIXME find the right way to trigger replot...
      plot->replot(QCustomPlot::rpQueuedReplot);
    }

    template<const size_t row, const size_t col, const size_t graph = 0, typename T>
    void setPlotData(const std::vector<T>& _y_)
    {
      auto plot = getPlot(row, col);

      const size_t n = _y_.size();

      QVector<double> x(n), y(n);

      for (size_t i = 0; i < n; ++i)
      {
        x[i] = static_cast<double>(i);
        y[i] = static_cast<double>(_y_[i]);
      }

      plot->graph(graph)->setData(x, y);
    }

    template<const size_t row, const size_t col, const size_t graph = 0, typename T>
    void setPlotData(const std::vector<T>& _x_, const std::vector<T>& _y_)
    {
      auto plot = getPlot(row, col);

      const size_t n = std::min(_y_.size(), _x_.size());

      QVector<double> x(n), y(n);

      for (size_t i = 0; i < n; ++i)
      {
        x[i] = static_cast<double>(_x_[i]);
        y[i] = static_cast<double>(_y_[i]);
      }

      plot->graph(graph)->setData(x, y);
    }

    template<const size_t row, const size_t col, const size_t graph = 0, typename T>
    void setPlotData(const std::vector<T>& _x_, const std::vector<double>& _y_)
    {
      auto plot = getPlot(row, col);

      const size_t n = std::min(_y_.size(), _x_.size());

      QVector<double> x(n), y(n);

      for (size_t i = 0; i < n; ++i)
      {
        x[i] = static_cast<double>(_x_[i]);
        y[i] = static_cast<double>(_y_[i]);
      }

      plot->graph(graph)->setData(x, y);
    }

  protected:

    QCustomPlot* getPlot(const size_t row, const size_t col) const
    {
      return dynamic_cast<QCustomPlot*>(layout->itemAtPosition(row, col)->widget());
    }

    QColor getLineColor(const size_t index) const
    {
      const auto& color = colors[index % 10];

      return QColor(color[0], color[1], color[2]);
    }

    int getLineWidth(const size_t index) const
    {
      return 2;
    }

  private:

    std::shared_ptr<QGridLayout> layout;
    std::vector<std::shared_ptr<QCustomPlot>> plots;

    /*
    #!/usr/bin/env python

    import numpy
    import seaborn

    colors = numpy.array(seaborn.color_palette())
    colors = (colors * 0xFF).clip(0, 0xFF).astype(numpy.uint8)
    colors = colors.tolist()

    print(colors)
    */

    const int colors[10][3] =
    {
      {  31, 119, 180 },
      { 255, 127,  14 },
      {  44, 160,  44 },
      { 214,  39,  40 },
      { 148, 103, 189 },
      { 140,  86,  75 },
      { 227, 119, 194 },
      { 127, 127, 127 },
      { 188, 189,  34 },
      {  23, 190, 207 },
    };
  };
}
