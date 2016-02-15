#ifndef QT_MAIN_WINDOW_H
#define QT_MAIN_WINDOW_H

#include <memory>
#include <utility>
#include <vector>

#include <QMainWindow>
#include <QShortcut>

#include "utility/messaging/MessageListener.h"
#include "utility/messaging/type/MessageNewProject.h"

#include "qt/utility/QtThreadedFunctor.h"
#include "qt/window/QtWindowStack.h"

class QDockWidget;
class QtSettingsWindow;
class View;

class QtViewToggle
	: public QWidget
{
	Q_OBJECT

public:
	QtViewToggle(View* view, QWidget *parent = nullptr);

public slots:
	void toggledByAction();
	void toggledByUI();

private:
	View* m_view;
};


class MouseReleaseFilter
	: public QObject
{
	Q_OBJECT

public:
	MouseReleaseFilter(QObject* parent);

protected:
	bool eventFilter(QObject* obj, QEvent* event);

private:
	size_t m_backButton;
	size_t m_forwardButton;
};

class MouseWheelFilter
	: public QObject
{
	Q_OBJECT

public:
	MouseWheelFilter(QObject* parent);

protected:
	bool eventFilter(QObject* obj, QEvent* event);
};


class QtMainWindow
	: public QMainWindow
	, public MessageListener<MessageNewProject>
{
	Q_OBJECT

public:
	QtMainWindow();
	~QtMainWindow();

	void init();

	void addView(View* view);
	void removeView(View* view);

	void showView(View* view);
	void hideView(View* view);

	void loadLayout();
	void saveLayout();

	void forceEnterLicense();

	void handleMessage(MessageNewProject* message);

protected:
	bool event(QEvent* event);
	void keyPressEvent(QKeyEvent* event);

public slots:
	void activateWindow();

	QtSettingsWindow* about();
	QtSettingsWindow* openSettings();
	QtSettingsWindow* showLicenses();
	QtSettingsWindow* enterLicense();

	QtSettingsWindow* showStartScreen();
	void hideStartScreen();

	void newProject();
	void openProject(const QString &path = QString());
	void editProject();
	void openRecentProject();

	void find();
	void closeWindow();
	void refresh();
	void forceRefresh();

	void saveProject();
	void saveAsProject();

	void undo();
	void redo();
	void zoomIn();
	void zoomOut();
	void resetZoom();
	void switchColorScheme();

	void toggleView(View* view, bool fromMenu);

	void handleEscapeShortcut();

	void updateRecentProjectMenu();

	static void setWindowSettingsPath(const std::string& windowSettingsPath);

private slots:
	void toggleShowDockWidgetTitleBars();

private:
	struct DockWidget
	{
		QDockWidget* widget;
		View* view;
		QAction* action;
		QtViewToggle* toggle;
	};

	void doCreateNewProject(const std::string& name, const std::string& location,
		const std::vector<std::string>& sourceFiles, const std::vector<std::string>& includePaths);

    void setupEditMenu();
	void setupProjectMenu();
	void setupViewMenu();
	void setupHelpMenu();

	void setupShortcuts();

	DockWidget* getDockWidgetForView(View* view);

	void setShowDockWidgetTitleBars(bool showTitleBars);

	template<typename T>
		T* createWindow();

	std::vector<DockWidget> m_dockWidgets;
	QMenu* m_viewMenu;
	QAction* m_viewSeparator;
	QAction** m_recentProjectAction;
	QAction* m_showTitleBarsAction;
	bool m_showDockWidgetTitleBars;

	QtWindowStack m_windowStack;

	QShortcut* m_escapeShortcut;

	static std::string m_windowSettingsPath;

	QtThreadedFunctor<std::string, std::string, std::vector<std::string>, std::vector<std::string>> m_createNewProjectFunctor;
};

#endif // QT_MAIN_WINDOW_H