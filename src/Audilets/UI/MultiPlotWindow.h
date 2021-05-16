#pragma once

#include <QColor>
#include <QGridLayout>
#include <QMainWindow>
#include <QPen>
#include <QWidget>

#include <qcustomplot/qcustomplot.h>

#include <memory>
#include <string>
#include <vector>

namespace Audilets::UI
{
  class MultiPlotWindow : public QMainWindow
  {
  public:

    MultiPlotWindow(QWidget* parent = nullptr);
    ~MultiPlotWindow();

  private:

    std::shared_ptr<QWidget> root;
    std::shared_ptr<QGridLayout> layout;
    std::vector<std::shared_ptr<QCustomPlot>> plots;

    QColor getColor(const size_t index) const;
    QCustomPlot* getPlot(const size_t row, const size_t col) const;

  public:

    template<const size_t row, const size_t col>
    void addPlot(const size_t numberOfGraphs = 1)
    {
      assert(numberOfGraphs > 0);

      auto plot = std::make_shared<QCustomPlot>();

      for (size_t i = 0; i < numberOfGraphs; ++i)
      {
        plot->addGraph();

        QPen pen;
        pen.setColor(getColor(i));
        pen.setWidth(2);

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
  };
}
