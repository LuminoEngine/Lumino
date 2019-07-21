

class EditorApplication
    : public ln::Application
{
public:
    static EditorApplication* instance();

    EditorApplication();
    ~EditorApplication();
    ln::Result init();
    void dispose();
    void run();

    ln::UIMainWindow* mainWindow() const;

protected:
    virtual void onInit() override;

private:

};

