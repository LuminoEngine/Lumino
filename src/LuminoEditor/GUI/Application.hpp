

class EditorApplication
    : public ln::Application
{
public:
    static Ref<ln::UICommand> NewCommand;
    static Ref<ln::UICommand> OpenCommand;


    static EditorApplication* instance();

    EditorApplication();
    ~EditorApplication();
    ln::Result init();
    void dispose();
    void run();
	void setMainWindow(ln::UIMainWindow* window);	// onInit でのみ可

    ln::UIMainWindow* mainWindow() const;

protected:
    virtual void onInit() override;

private:

};

