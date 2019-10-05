#include "MultiPlotWindow.h"

using namespace Audilets::GUI;

MultiPlotWindow::MultiPlotWindow(QWidget* parent) :
  QMainWindow(parent)
{
  root = std::make_shared<QWidget>();
  setCentralWidget(root.get());

  layout = std::make_shared<QGridLayout>();
  root->setLayout(layout.get());

  resize(800, 600);
}

MultiPlotWindow::~MultiPlotWindow()
{
}

QColor MultiPlotWindow::getColor(const size_t index) const
{
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
    {  23, 190, 207 }
  };

  const auto& color = colors[index % 10];

  return QColor(color[0], color[1], color[2]);
}

QCustomPlot* MultiPlotWindow::getPlot(const size_t row, const size_t col) const
{
  return dynamic_cast<QCustomPlot*>(layout->itemAtPosition(row, col)->widget());
}
