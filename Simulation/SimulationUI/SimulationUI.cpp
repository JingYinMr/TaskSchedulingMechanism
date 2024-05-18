#include "SimulationUI.h"

SimulationUI::SimulationUI(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);
	
	//ģ��װ��
	modelassemblyMainWindow = new ModelAssemblyMainWindow;
	connect(ui.ModelAssembly_pushButton, &QPushButton::clicked, this, [&]() {
		modelassemblyMainWindow->show();
	});

	//��������
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
