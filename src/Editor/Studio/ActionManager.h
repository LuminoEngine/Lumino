#pragma once

class MainWindow;

class ActionManager : public QObject
{
    Q_OBJECT
public:
	static ActionManager* instance();

	void registerAction(QString id, QAction* action);

	/** 指定した Action を実行する。実行できない場合は何もしない。 */
	void triggerAction(QString id);

	QAction* action(QString id) const;

private:
	explicit ActionManager(QObject* parent = nullptr);
	~ActionManager();

	QHash<QString, QAction*> m_actionMap;

	friend class MainWindow;   // creation
};
