#pragma once

#include <QtWidgets/QDialog>
#include "ui_SimulationUI.h"

#include "ModelAssemblyMainWindow.h"
#include "SimulationDeductionMainWindow.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	��������.	</summary>
///
/// <remarks>	������, 2024/05/18.	</remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

class SimulationUI : public QDialog
{
    Q_OBJECT

public:
    SimulationUI(QWidget *parent = nullptr);
    ~SimulationUI();

private:
    Ui::SimulationUIClass ui;

	/// <summary>	ģ��װ��������.	</summary>
	ModelAssemblyMainWindow *modelassemblyMainWindow;

	/// <summary>	��������������.	</summary>
	SimulationDeductionMainWindow *simulationdeductionMainWindow;
};
