#pragma once

#include <QMainWindow>
#include "ui_ModelAssemblyMainWindow.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	模型装配.	</summary>
///
/// <remarks>	朱连超, 2024/05/18.	</remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////


class ModelAssemblyMainWindow : public QMainWindow
{
	Q_OBJECT

public:
	ModelAssemblyMainWindow(QWidget *parent = nullptr);
	~ModelAssemblyMainWindow();

private:
	Ui::ModelAssemblyMainWindowClass ui;
};
