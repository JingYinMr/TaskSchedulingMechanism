#pragma once

#include <QMainWindow>
#include "ui_SimulationDeductionMainWindow.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	��������.	</summary>
///
/// <remarks>	������, 2024/05/18.	</remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

class SimulationDeductionMainWindow : public QMainWindow
{
	Q_OBJECT

public:
	SimulationDeductionMainWindow(QWidget *parent = nullptr);
	~SimulationDeductionMainWindow();

private:
	Ui::SimulationDeductionMainWindowClass ui;
};
