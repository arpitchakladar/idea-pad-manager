#include <memory>
#include <ftxui/component/component.hpp>

class TabView {
public:
	TabView();
	inline ftxui::Component component() { return _tabs; }
	static inline std::shared_ptr<TabView> Create() { return std::make_shared<TabView>(); }

private:
	int _tabNumber;
	ftxui::Component _tabs;
};
