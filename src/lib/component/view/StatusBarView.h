#ifndef STATUS_BAR_VIEW_H
#define STATUS_BAR_VIEW_H

#include "component/view/View.h"
#include "data/ErrorCountInfo.h"

class StatusBarController;

class StatusBarView : public View
{
public:
	StatusBarView(ViewLayout* viewLayout);
	~StatusBarView(void);

	virtual std::string getName() const;
	virtual void showMessage(const std::string& message, bool isError, bool showLoader) = 0;
	virtual void setErrorCount(ErrorCountInfo errorCount) = 0;

protected:
	StatusBarController* getController();
};

#endif //STATUS_BAR_VIEW_H
