#include "stdafx.h"
#include "GUIFactory.h"

const std::string GUIFactory::m_GUIThemeFile = "widgets/Black.txt";

const tgui::Layout GUIFactory::m_PaddingVertical = tgui::Layout(2.0f);

const tgui::Layout2d GUIFactory::m_BtnLargeSize = tgui::Layout2d(200, 36);
const tgui::Layout2d GUIFactory::m_BtnMediumSize = tgui::Layout2d(150, 24);
const tgui::Layout2d GUIFactory::m_BtnSmallSize = tgui::Layout2d(100, 14);

const tgui::Layout2d GUIFactory::m_EditLargeSize = tgui::Layout2d(200, 50);
const tgui::Layout2d GUIFactory::m_EditMediumSize = tgui::Layout2d(150, 36);
const tgui::Layout2d GUIFactory::m_EditSmallSize = tgui::Layout2d(100, 24);

const tgui::Layout GUILayout::m_SpacerVertical = tgui::Layout(24.0f);
const tgui::Layout2d GUILayout::m_MiniButtonSize = tgui::Layout2d(100.0f, 20.0f);

tgui::CheckBox::Ptr GUIFactory::MakeCheckbox(const std::string& text, const int textSize) {
	tgui::CheckBox::Ptr chk = tgui::CheckBox::create(text);
	chk->setRenderer(GetRenderer("Checkbox"));
	chk->setTextSize(textSize);
	return chk;
}

tgui::ListBox::Ptr GUIFactory::MakeListBox(const int textSize) {
	tgui::ListBox::Ptr lst = tgui::ListBox::create();
	lst->setRenderer(GetRenderer("ListBox"));
	lst->setTextSize(textSize);
	return lst;
}

tgui::ChatBox::Ptr GUIFactory::MakeChatbox() {
	auto chat = tgui::ChatBox::create();
	chat->setRenderer(GetRenderer("ChatBox"));
	return chat;
}

tgui::Label::Ptr GUIFactory::MakeLabel(const std::string& text, const int textSize) {
	tgui::Label::Ptr label = tgui::Label::create(text);
	label->setRenderer(GetRenderer("Label"));
	label->setTextSize(textSize);
	return label;
}

tgui::Button::Ptr GUIFactory::MakeButton(const std::string& text, const tgui::Layout2d btnSize, const int textSize) {
	tgui::Button::Ptr btn = tgui::Button::create(text);
	btn->setRenderer(GetRenderer("Button"));
	btn->setTextSize(textSize);
	btn->setSize(btnSize);
	return btn;
}
tgui::EditBox::Ptr GUIFactory::MakeEditbox(const tgui::Layout2d size, const int textSize) {
	auto edit = tgui::EditBox::create();
	edit->setRenderer(GetRenderer("EditBox"));
	edit->setTextSize(textSize);
	edit->setSize(size);
	return edit;
}
tgui::Slider::Ptr GUIFactory::MakeSlider(float fMin, float fMax, float fCurVal) {
	auto sld = tgui::Slider::create(fMin, fMax);
	sld->setRenderer(GetRenderer("Slider"));
	sld->setValue(fCurVal);
	sld->setStep(0);
	return sld;
}
tgui::Slider::Ptr GUIFactory::MakeSliderInt(int iMin, int iMax, int iCurVal) {
	auto sld = tgui::Slider::create((float)iMin, (float)iMax);
	sld->setRenderer(GetRenderer("Slider"));
	sld->setValue((float)iCurVal);
	return sld;
}
tgui::Picture::Ptr GUIFactory::MakePicture(const sf::Texture& texture, sf::Vector2i vSize) {
	auto picture = tgui::Picture::create(texture);
	picture->setRenderer(GetRenderer("Picture"));
	picture->setSize(tgui::Layout2d(vSize.x, vSize.y));
	return picture;
}
tgui::Tabs::Ptr GUIFactory::Tabs(const std::string& tab1, const std::string& tab2, const std::string& tab3, const std::string& tab4, const std::string& tab5) {
	auto tabs = tgui::Tabs::create();
	tabs->setRenderer(GetRenderer("Tabs"));
	tabs->add(tab1);

	if (!tab2.empty())
		tabs->add(tab2);
	if (!tab3.empty())
		tabs->add(tab3);
	if (!tab4.empty())
		tabs->add(tab4);
	if (!tab5.empty())
		tabs->add(tab5);

	return tabs;
}

tgui::Label::Ptr GUIFactory::LabelTitle(const std::string& text) {
	return MakeLabel(text, m_TextSizeTitle);
}
tgui::Label::Ptr GUIFactory::LabelHeading(const std::string& text) {
	return MakeLabel(text, m_TextSizeHeading);
}
tgui::Label::Ptr GUIFactory::LabelContent(const std::string& text) {
	return MakeLabel(text, m_TextSizeContent);
}

tgui::Button::Ptr GUIFactory::ButtonLarge(const std::string& text) {
	return MakeButton(text, m_BtnLargeSize, m_TextSizeTitle);
}
tgui::Button::Ptr GUIFactory::ButtonMedium(const std::string& text) {
	return MakeButton(text, m_BtnMediumSize, m_TextSizeHeading);
}
tgui::Button::Ptr GUIFactory::ButtonSmall(const std::string& text) {
	return MakeButton(text, m_BtnSmallSize, m_TextSizeContent);
}

tgui::CheckBox::Ptr GUIFactory::CheckboxLarge(const std::string& text) {
	return MakeCheckbox(text, m_TextSizeHeading);
}
tgui::CheckBox::Ptr GUIFactory::Checkbox(const std::string& text) {
	return MakeCheckbox(text, m_TextSizeContent);
}

tgui::ListBox::Ptr GUIFactory::ListBox(const std::vector<std::string>& vItems, int iSelectedIndex) {
	auto list = MakeListBox(m_TextSizeContent);
	for (int i = 0; i < vItems.size(); i++) {
		if (!list->addItem(vItems[ i ]))
			throw std::runtime_error("Failed to create listbox: Attempted to add too many items. (Failed on item " + std::to_string(i) + ")");
		if (i == iSelectedIndex)
			list->setSelectedItem(vItems[ i ]);
	}
	return list;

}
tgui::ListBox::Ptr GUIFactory::ListBoxLarge(const std::vector<std::string>& vItems, int iSelectedIndex) {
	auto list = MakeListBox(m_TextSizeHeading);
	for (int i = 0; i < vItems.size(); i++) {
		if (!list->addItem(vItems[ i ]))
			throw std::runtime_error("Failed to create listbox: Attempted to add too many items. (Failed on item " + std::to_string(i) + ")");
		if (i == iSelectedIndex)
			list->setSelectedItem(vItems[ i ]);
	}
	return list;
}

tgui::ChatBox::Ptr GUIFactory::Chatbox() {
	return MakeChatbox();
}
tgui::Slider::Ptr GUIFactory::SliderFloat(float fMin, float fMax, float fCurVal) {
	return MakeSlider(fMin, fMax, fCurVal);
}
tgui::Slider::Ptr GUIFactory::SliderInt(int iMin, int iMax, int iCurVal) {
	return MakeSliderInt(iMin, iMax, iCurVal);
}
tgui::Picture::Ptr GUIFactory::Picture(const sf::Texture& texture, sf::Vector2i vSize) {
	return MakePicture(texture, vSize);
}

std::shared_ptr<tgui::Panel> GUIFactory::PanelSize(sf::Vector2i panelSize) {
	auto panel = tgui::Panel::create();
	panel->setRenderer(GetRenderer("Panel"));
	panel->setSize(panelSize.x, panelSize.y);
	return panel;
}

tgui::Panel::Ptr GUIFactory::PanelScale(tgui::Gui& gui, sf::Vector2f scale) {
	auto panel = tgui::Panel::create();
	panel->setRenderer(GetRenderer("Panel"));
	panel->setSize(scale.x * tgui::bindWidth(gui), scale.y * tgui::bindHeight(gui));
	return panel;
}

tgui::Panel::Ptr GUIFactory::PanelCentred(tgui::Gui& gui, sf::Vector2f sizeFactor) {
	auto panel = tgui::Panel::create();
	panel->setRenderer(GetRenderer("Panel"));
	panel->setSize(tgui::bindWidth(gui)*sizeFactor.x, tgui::bindHeight(gui)*sizeFactor.y);
	panel->setPosition( // Put panel in centre of GUI
		tgui::bindWidth(gui) / 2.0f - tgui::bindWidth(panel) / 2.0f, 
		tgui::bindHeight(gui) / 2.0f - tgui::bindHeight(panel) / 2.0f
	);
	return panel;
}
std::shared_ptr<tgui::Panel> GUIFactory::PanelCentredSize(tgui::Gui& gui, sf::Vector2i vSize) {
	auto panel = tgui::Panel::create();
	panel->setRenderer(GetRenderer("Panel"));
	panel->setSize(vSize.x, vSize.y);
	panel->setPosition( // Put panel in centre of GUI
		tgui::bindWidth(gui) / 2.0f - tgui::bindWidth(panel) / 2.0f,
		tgui::bindHeight(gui) / 2.0f - tgui::bindHeight(panel) / 2.0f
	);
	return panel;
}

tgui::EditBox::Ptr GUIFactory::EditboxMedium() {
	return MakeEditbox(m_EditMediumSize, m_TextSizeHeading);
}
tgui::Layout GUIUtil::MinHeight(tgui::Widget::Ptr widget1, tgui::Widget::Ptr widget2, tgui::Widget::Ptr widget3) {
	return widget3 ? std::min(std::min(widget1->getSize().y, widget2->getSize().y), widget3->getSize().y) 
		: std::min(widget1->getSize().y, widget2->getSize().y);
}
tgui::Layout GUIUtil::MinWidth(tgui::Widget::Ptr widget1, tgui::Widget::Ptr widget2, tgui::Widget::Ptr widget3) {
	return widget3 ? std::min(std::min(widget1->getSize().x, widget2->getSize().x), widget3->getSize().x)
		: std::min(widget1->getSize().x, widget2->getSize().x);
}
tgui::Layout GUIUtil::MaxHeight(tgui::Widget::Ptr widget1, tgui::Widget::Ptr widget2, tgui::Widget::Ptr widget3) {
	return widget3 ? std::max(std::max(widget1->getSize().y, widget2->getSize().y), widget3->getSize().y)
		: std::max(widget1->getSize().y, widget2->getSize().y);
}
tgui::Layout GUIUtil::MaxWidth(tgui::Widget::Ptr widget1, tgui::Widget::Ptr widget2, tgui::Widget::Ptr widget3) {
	return widget3 ? std::max(std::max(widget1->getSize().x, widget2->getSize().x), widget3->getSize().x)
		: std::max(widget1->getSize().x, widget2->getSize().x);
}

tgui::Layout GUIUtil::bindHeightLimit(tgui::Widget::Ptr widget, float heightMax) {
	return std::min(widget->getSize().y, heightMax);
}
tgui::Layout GUIUtil::bindWidthLimit(tgui::Widget::Ptr widget, float widthMax) {
	return std::min(widget->getSize().x, widthMax);
}

void GUIFactory::setPositionHCentred(tgui::Widget::Ptr widget, tgui::Layout2d position) {
	widget->setPosition(position.x - (tgui::bindWidth(widget) / 2.0f), position.y);
}

tgui::Label::Ptr GUILayout::LabelContentVertical(tgui::Container::Ptr container, tgui::Layout2d& _inout_position, const std::string& text, const std::string& sID) {
	auto lbl = GUIFactory::LabelContent(text);
	lbl->setPosition(_inout_position);
	container->add(lbl, sID);

	_inout_position.y = _inout_position.y + tgui::bindHeight(lbl) + GUIFactory::m_PaddingVertical;
	return lbl;
}
/*
template<typename CallbackFn_t, typename... Args>
void GUILayout::MiniButtonHorizontal(tgui::Container::Ptr container, tgui::Layout2d& _inout_position, const std::string& text, CallbackFn_t fn, Args... args) {
	auto btn = GUIFactory::ButtonSmall(text);
	btn->setSize(tgui::Layout2d(m_MiniButtonSize.x - 2.0f, m_MiniButtonSize.y));
	btn->setPosition(_inout_position + tgui::Layout2d(1.0f, 0.0f));
	btn->setText(text);
	btn->setTextSize(GUIFactory::m_TextSizeContent);

	btn->connect("pressed", fn, args...);
	container->add(btn);

	_inout_position.x += fSizePct.x * container->getSize().x;
	if (_inout_position.x.getValue() >= container->getSize().x) {
		_inout_position.x = 0.0f;
		_inout_position.y += tgui::bindHeight(btn);
	}
}*/

void GUILayout::EndMiniButtonsHorizontal(tgui::Container::Ptr container, tgui::Layout2d& _inout_position) {
	if (_inout_position.x.getValue() <= 0.0f) {
		_inout_position.x = 0.0f;
		_inout_position.y = _inout_position.y + m_MiniButtonSize.y + GUIFactory::m_PaddingVertical;
	}
}
/*
template<typename CallbackFn_t, typename... Args>
void GUILayout::Checkbox(tgui::Container::Ptr container, tgui::Layout2d& _inout_position, const std::string& sID, const std::string& text, bool initialValue, CallbackFn_t fn, Args... args) {
	auto chk = GUIFactory::Checkbox(text);
	chk->setPosition(_inout_position);
	chk->connect("checked unchecked", fn, args...);
	if (initialValue) {
		chk->check();
	}
	container->add(chk, sID);
	_inout_position.y += checkbox->getSize().y + VERTICAL_PADDING;
}

template<typename CallbackFn_t, typename... Args>
void GUILayout::Checkbox(tgui::Gui* container, tgui::Layout2d& _inout_position, const std::string& sID, const std::string& text, bool initialValue, CallbackFn_t fn, Args... args) {
	auto chk = GUIFactory::Checkbox(text);
	chk->setPosition(_inout_position);
	chk->connect("checked unchecked", fn, args...);
	if (initialValue) {
		chk->check();
	}
	container->add(chk, sID);
	_inout_position.y += checkbox->getSize().y + VERTICAL_PADDING;
}

template<typename CallbackFn_t, typename... Args>
void GUILayout::SliderInt(tgui::Container::Ptr container, tgui::Layout2d& _inout_position, const std::string& sliderID, const std::string& displayID, const std::string& text, int initialValue, const sf::Vector2i sliderRange, CallbackFn_t func, Args... args) {
	auto lblName = GUIFactory::LabelContent(text);
	auto sld = GUIFactory::SliderInt(sliderRange.x, sliderRange.y, initialValue);
	auto lblDisplay = GUIFactory::LabelContent(std::to_string(initialValue));

	lblName->setPosition(_inout_position);
	sld->setPosition(tgui::bindRight(lblName), _inout_position.y);
	lblDisplay->setPosition(tgui::bindRight(slider), _inout_position.y);

	sld->connect("ValueChanged", func, args...);
	container->add(lblName);
	container->add(slider, sliderID);
	container->add(lblDisplay, displayID);

	// Increase layout cursor by height (Max of three widgets), and padding
	tgui::Layout maxLabel = GUIUtil::MaxHeight(lblName, lblDisplay, sld);
	_inout_position.y += GUIUtil::MaxHeight(lblName, lblDisplay, sld) + GUIFactory::m_PaddingVertical;
}

template<typename CallbackFn_t, typename... Args>
void GUILayout::SliderFloat(tgui::Container::Ptr container, tgui::Layout2d& _inout_position, const std::string& sliderID, const std::string& displayID, const std::string& text, float initialValue, const sf::Vector2f sliderRange, CallbackFn_t func, Args... args) {
	auto lblName = GUIFactory::LabelContent(text);
	auto sld = GUIFactory::SliderFloat(sliderRange.x, sliderRange.y, initialValue);
	auto lblDisplay = GUIFactory::LabelContent(std::to_string(initialValue));

	lblName->setPosition(_inout_position);
	sld->setPosition(tgui::bindRight(lblName), _inout_position.y);
	lblDisplay->setPosition(tgui::bindRight(slider), _inout_position.y);

	sld->connect("ValueChanged", func, args...);
	container->add(lblName);
	container->add(slider, sliderID);
	container->add(lblDisplay, displayID);

	// Increase layout cursor by height (Max of three widgets), and padding
	tgui::Layout maxLabel = GUIUtil::MaxHeight(lblName, lblDisplay, sld);
	_inout_position.y += GUIUtil::MaxHeight(lblName, lblDisplay, sld) + GUIFactory::m_PaddingVertical;
}

template<typename CallbackFn_t, typename... Args>
void GUILayout::EditBoxButton(tgui::Container::Ptr container, tgui::Layout2d& _inout_position, const std::string& editID, const std::string& text, const std::string& initialText, CallbackFn_t func, Args... args) {
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
void GUILayout::Button(tgui::Container::Ptr container, tgui::Layout2d& _inout_position, const std::string& sID, const std::string& text, CallbackFn_t fn, Args... args) {
	auto btn = GUIFactory::ButtonMedium(text);
	btn->setPosition(_inout_position);
	btn->connect("pressed", fn, args...);

	container->add(btn, sID);
	_inout_position.y += tgui::bindHeight(btn) + VERTICAL_PADDING;
}
*/
void GUILayout::VerticalSpacer(tgui::Layout2d& _inout_position) {
	_inout_position.y = _inout_position.y + GUILayout::m_SpacerVertical;
}