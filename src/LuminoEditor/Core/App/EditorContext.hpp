#pragma once

class EditorApplication;
class MainWindow;

namespace lna {
    
class EditorContext
    : public ln::Object
{
public:
    EditorApplication* application() const { return m_application; }
    MainWindow* mainWindow() const { return m_mainWindow; }

    //void activateAllExtensions();
    //void deactivateAllExtensions();

public: // TODO: internal
    EditorApplication* m_application;
    MainWindow* m_mainWindow;
};

} // namespace lna

