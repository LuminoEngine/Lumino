﻿#pragma once

class EditorApplication;
class MainWindow;

namespace lna {
class Project;
class AssetDatabase;

class EditorContext
    : public ln::Object
{
public:
	static EditorContext* current();

    EditorApplication* application() const { return m_application; }
    MainWindow* mainWindow() const { return m_mainWindow; }
    Project* mainProject() const;
    PluginManager* pluginManager() const;
	AssetDatabase* assetDatabase() const;

    //void activateAllExtensions();
    //void deactivateAllExtensions();

public: // TODO: internal
    EditorApplication* m_application;
    MainWindow* m_mainWindow;
};

} // namespace lna

