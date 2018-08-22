#ifndef __GUI_FACTORY_H__
#define __GUI_FACTORY_H__

class GUIUtil {
public:
	static tgui::Layout MinHeight(tgui::Widget::Ptr widget1, tgui::Widget::Ptr widget2, tgui::Widget::Ptr widget3=NULL);
	static tgui::Layout MinWidth(tgui::Widget::Ptr widget1, tgui::Widget::Ptr widget2, tgui::Widget::Ptr widget3 = NULL);
	static tgui::Layout MaxHeight(tgui::Widget::Ptr widget1, tgui::Widget::Ptr widget2, tgui::Widget::Ptr widget3 = NULL);
	static tgui::Layout MaxWidth(tgui::Widget::Ptr widget1, tgui::Widget::Ptr widget2, tgui::Widget::Ptr widget3 = NULL);

	static tgui::Layout bindHeightLimit(tgui::Widget::Ptr widget, float heightMax);
	static tgui::Layout bindWidthLimit(tgui::Widget::Ptr widget, float widthMax);
};

class GUIFactory {
	friend class GUILayout;
protected:
	static const std::string m_GUIThemeFile;
	static const int m_TextSizeTitle = 24;
	static const int m_TextSizeHeading = 16;
	static const int m_TextSizeContent = 12;

	static const tgui::Layout m_PaddingVertical;

	static const tgui::Layout2d m_BtnLargeSize;
	static const tgui::Layout2d m_BtnMediumSize;
	static const tgui::Layout2d m_BtnSmallSize;

	static const tgui::Layout2d m_EditLargeSize;
	static const tgui::Layout2d m_EditMediumSize;
	static const tgui::Layout2d m_EditSmallSize;

	static auto GetRenderer(const std::string& sID) {
		try {
			static tgui::Theme m_GUITheme(m_GUIThemeFile);
			return m_GUITheme.getRenderer(sID);
		} catch (std::exception e) {
			throw std::runtime_error("GUIFactory failed to get renderer: GUI Theme file '" + m_GUIThemeFile + "' failed to load: " + e.what());
		}
	}

	static tgui::CheckBox::Ptr MakeCheckbox(const std::string& text, const int textSize);
	static tgui::ListBox::Ptr MakeListBox(const int textSize);
	static tgui::ChatBox::Ptr MakeChatbox();
	static tgui::Label::Ptr MakeLabel(const std::string& text, const int textSize);
	static tgui::Button::Ptr MakeButton(const std::string& text, const tgui::Layout2d size, const int textSize); 
	static tgui::EditBox::Ptr MakeEditbox(const tgui::Layout2d size, const int textSize);
	static tgui::Slider::Ptr MakeSlider(float fMin, float fMax, float fCurVal);
	static tgui::Slider::Ptr MakeSliderInt(int iMin, int iMax, int iCurVal);
	static tgui::Picture::Ptr MakePicture(const sf::Texture& texture, sf::Vector2i vSize);
	
public:
	static tgui::Tabs::Ptr Tabs(const std::string& tab1, const std::string& tab2 = "", const std::string& tab3 = "", const std::string& tab4 = "", const std::string& tab5="");

	static tgui::Label::Ptr LabelTitle(const std::string& text);
	static tgui::Label::Ptr LabelHeading(const std::string& text);
	static tgui::Label::Ptr LabelContent(const std::string& text);

	static tgui::Button::Ptr ButtonLarge(const std::string& text);
	static tgui::Button::Ptr ButtonMedium(const std::string& text);
	static tgui::Button::Ptr ButtonSmall(const std::string& text);

	static tgui::CheckBox::Ptr CheckboxLarge(const std::string& text);
	static tgui::CheckBox::Ptr Checkbox(const std::string& text);

	static tgui::ListBox::Ptr ListBoxLarge(const std::vector<std::string>& vItems, int iSelectedIndex);
	static tgui::ListBox::Ptr ListBox(const std::vector<std::string>& vItems, int iSelectedIndex);

	static tgui::ChatBox::Ptr Chatbox();

	static tgui::Slider::Ptr SliderFloat(float fMin, float fMax, float fCurVal);
	static tgui::Slider::Ptr SliderInt(int iMin, int iMax, int iCurVal);

	static tgui::Picture::Ptr Picture(const sf::Texture& texture, sf::Vector2i vSize);
	
	static tgui::Panel::Ptr PanelSize(sf::Vector2i panelSize);
	static tgui::Panel::Ptr PanelScale(tgui::Gui& gui, sf::Vector2f scale);

	static tgui::Panel::Ptr PanelCentred(tgui::Gui& gui, sf::Vector2f sizeFactor);
	static tgui::Panel::Ptr PanelCentredSize(tgui::Gui& gui, sf::Vector2i panelSize);

	static tgui::EditBox::Ptr EditboxMedium();

	// Set widget position horizontal centred
	static void setPositionHCentred(tgui::Widget::Ptr widget, tgui::Layout2d position);
};

class GUILayout {
	friend class GUIFactory;
	static const tgui::Layout m_SpacerVertical;
	static const tgui::Layout2d m_MiniButtonSize;

public:
	// Create and insert a label at content size, incremement position by height, return label
	static tgui::Label::Ptr LabelContentVertical(tgui::Container::Ptr container, tgui::Layout2d& _inout_position, const std::string& text, const std::string& sID="");

	template<typename CallbackFn_t, typename... Args> 
	static void MiniButtonHorizontal(tgui::Container::Ptr container, tgui::Layout2d& _inout_position, const std::string& text, CallbackFn_t fn, Args... args) {
		auto btn = GUIFactory::ButtonSmall(text);
		btn->setSize(tgui::Layout2d(m_MiniButtonSize.x - 2.0f, m_MiniButtonSize.y));
		btn->setPosition(_inout_position + tgui::Layout2d(1.0f, 0.0f));
		btn->setText(text);
		btn->setTextSize(GUIFactory::m_TextSizeContent);

		btn->connect("pressed", fn, args...);
		container->add(btn);

		_inout_position.x = _inout_position.x + m_MiniButtonSize.x;
		if (_inout_position.x.getValue() >= container->getSize().x) {
			_inout_position.x = 0.0f;
			_inout_position.y = _inout_position.y + tgui::bindHeight(btn);
		}
	}

	static void EndMiniButtonsHorizontal(tgui::Container::Ptr container, tgui::Layout2d& _inout_position);

	static void VerticalSpacer(tgui::Layout2d& _inout_position);

	// Add a listbox
	// Set iSelectedItem to -1 to have no initial selected item
	template<typename CallbackFn_t, typename... Args>
	static void ListBoxVertical(tgui::Container::Ptr container, tgui::Layout2d& _inout_position, const std::string& sID, const std::string& sLabel, const std::vector<std::string>& vItems, int iSelectedItem, CallbackFn_t fn, Args... args) {
		LabelContentVertical(container, _inout_position, sLabel);
		auto listbox = GUIFactory::ListBox(vItems, iSelectedItem);
		listbox->setPosition(_inout_position);
		listbox->connect("DoubleClicked", fn, args...);
		container->add(listbox, sID);
		_inout_position.y = _inout_position.y + tgui::bindHeight(listbox) + GUIFactory::m_PaddingVertical;
	}

	template<typename CallbackFn_t, typename... Args>
	static void Checkbox(tgui::Container::Ptr container, tgui::Layout2d& _inout_position, const std::string& sID, const std::string& text, bool initialValue, CallbackFn_t fn, Args... args) {
		auto chk = GUIFactory::Checkbox(text);
		chk->setPosition(_inout_position);
		chk->connect("Checked", fn, args...);
		chk->connect("Unchecked", fn, args...);
		chk->setChecked(initialValue);
		container->add(chk, sID);
		_inout_position.y = _inout_position.y + chk->getSize().y + GUIFactory::m_PaddingVertical;
	}

	template<typename CallbackFn_t, typename... Args>
	static void Checkbox(tgui::Gui* container, tgui::Layout2d& _inout_position, const std::string& sID, const std::string& text, bool initialValue, CallbackFn_t fn, Args... args) {
		auto chk = GUIFactory::Checkbox(text);
		chk->setPosition(_inout_position);
		chk->connect("Checked", fn, args...);
		chk->connect("Unchecked", fn, args...);
		chk->setChecked(initialValue);
		container->add(chk, sID);
		_inout_position.y = _inout_position.y + chk->getSize().y + GUIFactory::m_PaddingVertical; 
	}

	template<typename CallbackFn_t, typename... Args>
	static void SliderInt(tgui::Container::Ptr container, tgui::Layout2d& _inout_position, const std::string& sliderID, const std::string& displayID, const std::string& text, int initialValue, const sf::Vector2i sliderRange, CallbackFn_t func, Args... args) {
		auto lblName = GUIFactory::LabelContent(text);
		auto sld = GUIFactory::SliderInt(sliderRange.x, sliderRange.y, initialValue);
		auto lblDisplay = GUIFactory::LabelContent(std::to_string(initialValue));

		lblName->setPosition(_inout_position);
		sld->setPosition(tgui::bindRight(lblName), _inout_position.y);
		lblDisplay->setPosition(tgui::bindRight(sld), _inout_position.y);

		sld->connect("ValueChanged", func, args...);
		container->add(lblName);
		container->add(sld, sliderID);
		container->add(lblDisplay, displayID);

		// Increase layout cursor by height (Max of three widgets), and padding
		tgui::Layout maxLabel = GUIUtil::MaxHeight(lblName, lblDisplay, sld);
		_inout_position.y = _inout_position.y + GUIUtil::MaxHeight(lblName, lblDisplay, sld) + GUIFactory::m_PaddingVertical;
	}

	template<typename CallbackFn_t, typename... Args>
	static void SliderFloat(tgui::Container::Ptr container, tgui::Layout2d& _inout_position, const std::string& sliderID, const std::string& displayID, const std::string& text, float initialValue, const sf::Vector2f sliderRange, CallbackFn_t func, Args... args) {
		auto lblName = GUIFactory::LabelContent(text);
		auto sld = GUIFactory::SliderFloat(sliderRange.x, sliderRange.y, initialValue);
		auto lblDisplay = GUIFactory::LabelContent(std::to_string(initialValue));

		lblName->setPosition(_inout_position);
		sld->setPosition(tgui::bindRight(lblName), _inout_position.y);
		lblDisplay->setPosition(tgui::bindRight(sld), _inout_position.y);

		sld->connect("ValueChanged", func, args...);
		container->add(lblName);
		container->add(sld, sliderID);
		container->add(lblDisplay, displayID);

		// Increase layout cursor by height (Max of three widgets), and padding
		tgui::Layout maxLabel = GUIUtil::MaxHeight(lblName, lblDisplay, sld);
		_inout_position.y = _inout_position.y + GUIUtil::MaxHeight(lblName, lblDisplay, sld) + GUIFactory::m_PaddingVertical;
	}

	template<typename CallbackFn_t, typename... Args>
	static void EditBoxButton(tgui::Container::Ptr container, tgui::Layout2d& _inout_position, const std::string& editID, const std::string& text, const std::string& initialText, CallbackFn_t func, Args... args) {
		auto editbox = GUIFactory::EditboxMedium();
		auto button = GUIFactory::ButtonMedium(text);

		tgui::Layout width = container->getSize().x;

		editbox->setSize(width - button->getSize().x, editbox->getSize().y);
		editbox->setPosition(_inout_position);
		editbox->setText(initialText);

		button->setPosition(_inout_position.x + editbox->getSize().x, _inout_position.y);
		button->connect("pressed", func, args...);

		container->add(editbox, editID);
		container->add(button, editID);

		_inout_position.y = _inout_position.y + GUIUtil::MaxHeight(button, editbox);
	}

	template<typename CallbackFn_t, typename... Args>
	static void Button(tgui::Container::Ptr container, tgui::Layout2d& _inout_position, const std::string& sID, const std::string& text, CallbackFn_t fn, Args... args) {
		auto btn = GUIFactory::ButtonMedium(text);
		btn->setPosition(_inout_position);
		btn->connect("pressed", fn, args...);

		container->add(btn, sID);
		_inout_position.y = _inout_position.y + tgui::bindHeight(btn) + GUIFactory::m_PaddingVertical;
	}
};

#endif