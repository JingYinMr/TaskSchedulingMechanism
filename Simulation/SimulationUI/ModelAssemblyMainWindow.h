#pragma once

#include <QMainWindow>
#include "ui_ModelAssemblyMainWindow.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	ģ��װ��.	</summary>
///
/// <remarks>	������, 2024/05/18.	</remarks>
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
