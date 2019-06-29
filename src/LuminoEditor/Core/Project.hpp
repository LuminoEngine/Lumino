#pragma once

namespace lna {


class LanguageContext;
class Project;
class ProjectProperties;
class Workspace;

class Project
	: public ln::RefObject
{
public:
	static const ln::String ProjectFileExt;

	Project(Workspace* owner);
	virtual ~Project();

	ln::Result newProject(const ln::Path& projectDir, const ln::String& projectName, const ln::String& engineSource, const ln::String& templateName);
    ln::Result openProject2(const ln::Path& projectFile);
    ln::Result saveProject();
    ln::Result loadProject();
	void restore();

	const ln::Ref<ProjectProperties>& properties() const { return m_properties; }

	Workspace* workspace() const { return m_workspace; }
	const ln::Path& engineDirPath() const { return m_engineDir; }
	const ln::Path& rootDirPath() const { return m_rootDir; }
    const ln::Path& assetsDir() const { return m_assetsDir; }
	const ln::Path& buildDir() const { return m_buildDir; }
	const ln::Path& releaseDir() const { return m_releaseDir; }
	const ln::Path& intermediateAssetsDir() const { return m_intermediateAssetsDir; }

	ln::Path projectsDir() const { return ln::Path(rootDirPath(), u"Projects"); }
	ln::Path emscriptenProjectDir() const { return ln::Path(projectsDir(), u"LuminoApp.Web"); }
	ln::Path androidProjectDir() const { return ln::Path(projectsDir(), u"LuminoApp.Android"); }
	ln::Path macOSProjectDir() const { return ln::Path(projectsDir(), u"LuminoApp.macOS");  }
	ln::Path iOSProjectDir() const { return ln::Path(projectsDir(), u"LuminoApp.iOS");  }
	ln::Path windowsProjectDir() const { return ln::Path(projectsDir(), u"LuminoApp.Windows"); }

	/** 指定フォルダにプロジェクトファイルが含まれているかを確認する */
	static bool existsProjectFile(const ln::Path& dir);

private:
	void setupPathes();

	Workspace* m_workspace;
	ln::Ref<ProjectProperties> m_properties;
	ln::Ref<LanguageContext> m_context;

	ln::String m_projectName;
	ln::Path m_projectFilePath;
	ln::Path m_rootDir;
	ln::Path m_engineDir;
	ln::Path m_sourcesDir;
	ln::Path m_assetsDir;
	ln::Path m_buildDir;
	ln::Path m_releaseDir;
	ln::Path m_intermediateAssetsDir;
};

// プロジェクトファイルに保存する情報
class ProjectProperties
	: public ln::RefObject
{
public:
	ln::String language;	// "cpp", "cs" ...
	ln::String engine = u"";	// "x.y.z" or "" or "repo:master"

	LN_SERIALIZE_CLASS_VERSION(1)
	void serialize(ln::Archive& ar)
	{
		ar & LN_NVP(language);
		ar & LN_NVP(engine);
	}
};


#if 0

//class LanguageContext;
//class Project;
class ProjectProperties;
//class Workspace;
class AssetDatabase;

class Project
	: public ln::RefObject
{
public:
	static const ln::String ProjectFileExt;

    ln::Result saveProject();
    ln::Result loadProject();
	void restore();

	const ln::Ref<ProjectProperties>& properties() const { return m_properties; }

	const ln::Path& engineDirPath() const { return m_engineDir; }
	const ln::Path& rootDirPath() const { return m_rootDir; }
    const ln::Path& assetsDir() const { return m_assetsDir; }
	const ln::Path& buildDir() const { return m_buildDir; }
	const ln::Path& releaseDir() const { return m_releaseDir; }
	const ln::Path& intermediateAssetsDir() const { return m_intermediateAssetsDir; }

	ln::Path projectsDir() const { return ln::Path(rootDirPath(), u"Projects"); }
	ln::Path emscriptenProjectDir() const { return ln::Path(projectsDir(), u"LuminoApp.Web"); }
	ln::Path androidProjectDir() const { return ln::Path(projectsDir(), u"LuminoApp.Android"); }
	ln::Path macOSProjectDir() const { return ln::Path(projectsDir(), u"LuminoApp.macOS");  }
	ln::Path iOSProjectDir() const { return ln::Path(projectsDir(), u"LuminoApp.iOS");  }
	ln::Path windowsProjectDir() const { return ln::Path(projectsDir(), u"LuminoApp.Windows"); }

    const ln::Ref<AssetDatabase>& assetDatabase() const { return m_assetDatabase; }

	/** 指定フォルダにプロジェクトファイルが含まれているかを確認する */
	static bool existsProjectFile(const ln::Path& dir);

private:
    LN_INTERNAL_NEW_OBJECT;
    Project();
    virtual ~Project();
    ln::Result initForNew(const ln::Path& projectDir, const ln::String& projectName, const ln::String& engineSource, const ln::String& templateName);
    ln::Result initForOpen(const ln::Path& dir);

	void setupPathes();

	ln::Ref<ProjectProperties> m_properties;
	//ln::Ref<LanguageContext> m_context;

	ln::String m_projectName;
	ln::Path m_projectFilePath;
	ln::Path m_rootDir;
	ln::Path m_engineDir;
	ln::Path m_sourcesDir;
	ln::Path m_assetsDir;
	ln::Path m_buildDir;
	ln::Path m_releaseDir;
	ln::Path m_intermediateAssetsDir;
    ln::Ref<AssetDatabase> m_assetDatabase;
};

// プロジェクトファイルに保存する情報
class ProjectProperties
	: public ln::RefObject
{
public:
	ln::String language;	// "cpp", "cs" ...
	ln::String engine = u"";	// "x.y.z" or "" or "repo:master"

	LN_SERIALIZE_CLASS_VERSION(1)
	void serialize(ln::Archive& ar)
	{
		ar & LN_NVP(language);
		ar & LN_NVP(engine);
	}
};

#endif

} // namespace lna
