1、一共有3个QTextEdit作为显示的文本域，还有1个QLineEdit作为输入的文本框。初始文本我都改成变量名了。我感觉这几个东西没什么写接口的必要，到时候只要直接调用其成员函数即可。
例如：
	get: txtSystemStatus->toPlainText()
	set: txtSystemStatus->setText(QString)
	append: txtSystemStatus->append(QString)。


2、按钮的显示文本我也改成变量名了。单击信号和槽已经连接好，在对应的槽函数里面加单击按钮后要进行的操作就行。例如：
	按钮 bFile ——> 槽 void MainWindow:: fileBtnOnClick()


3、点击文件按钮会弹出文件选择的对话框，选择后会返回文件路径。因为不知道你打算怎么读文件，所以文件读取的代码没写。


4、画图组件有两个方法：
	void MainWindow:: initPlot(int groupSize, int dataxSize, QVector<QString> itemName)
	groupSize: 在同一个横坐标下有几个对比的bar(demo为2)
	dataxSize: 横坐标一共几个点(demo为4)
	itemName:  bar的名字，用以绘制legend
以及
	void MainWindow:: updatePlot(QVector<double> x, QVector<double> y, int barIndex)
	x, y: 横坐标，纵坐标数据
	barIndex: 要更新的bar的下标(所有bar存储在QVector<QCPBars*> bars中)
目前initPlot在MainWindow的构造函数中调用。另外bSwitch和bStart两个按钮目前的动作是使用updatePlot在树枝不同的情况下进行画图demo。如果每组的groupSize>2的话要在initPlot中的对应QVector中增加预设颜色，现在一共就两种。
祝一切顺利！