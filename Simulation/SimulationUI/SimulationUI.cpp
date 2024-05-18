#include "SimulationUI.h"

SimulationUI::SimulationUI(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);
	
	//模型装配
	modelassemblyMainWindow = new ModelAssemblyMainWindow;
	connect(ui.ModelAssembly_pushButton, &QPushButton::clicked, this, [&]() {
		modelassemblyMainWindow->show();
	});

	//仿真推演
	simulationdeductionMainWindow = new SimulationDeductionMainWindow;
	connect(ui.SimulationDeduction_pushButton, &QPushButton::clicked, this, [&]() {
		simulationdeductionMainWindow->show();
	});
}

SimulationUI::~SimulationUI()
{
	delete modelassemblyMainWindow;
	delete simulationdeductionMainWindow;
}
