#pragma once

#include <QtWidgets/QDialog>
#include "ui_SimulationUI.h"

#include "ModelAssemblyMainWindow.h"
#include "SimulationDeductionMainWindow.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	仿真引擎.	</summary>
///
/// <remarks>	朱连超, 2024/05/18.	</remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

class SimulationUI : public QDialog
{
    Q_OBJECT

public:
    SimulationUI(QWidget *parent = nullptr);
    ~SimulationUI();

private:
    Ui::SimulationUIClass ui;

	/// <summary>	模型装配主界面.	</summary>
	ModelAssemblyMainWindow *modelassemblyMainWindow;

	/// <summary>	仿真推演主界面.	</summary>
	SimulationDeductionMainWindow *simulationdeductionMainWindow;
};
