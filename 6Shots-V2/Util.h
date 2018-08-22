#ifndef __UTIL_H__
#define __UTIL_H__

/*	Christopher De Pasquale

**	Helper functions & useful functions
**
**	Created:	24th April 2018
**	Last Edit:	25th April 2018
*/

namespace std {
	template <typename T> std::string to_string(const sf::Vector2<T>& v) {
		return std::string("(") + std::to_string(v.x) + std::string(", ") + \
			std::to_string(v.y) + std::string(")");
	}
};

namespace util {
	sf::Color toSFMLColor(const b2Color& c);
	sf::Vector2f toSFMLVec2f(const b2Vec2& v);
	sf::Vector2i toSFMLVec2i(const b2Vec2& v);
};

namespace vec {
	template <typename T> float LengthSquared(const sf::Vector2<T>& vector) {
		return vector.x * vector.x + vector.y * vector.y;
	}

	template <typename T> float Length(const sf::Vector2<T>& vector) {
		return (float)sqrt(vector.x * vector.x + vector.y * vector.y);
	}

	template <typename T> sf::Vector2<T> UnitVector(const sf::Vector2<T>& vector) {
		float length = Length(vector);
		assert(length > 0.0f);
		return sf::Vector2<T>(
			(T)((float)(vector.x)) / length,
			(T)((float)(vector.y)) / length
		);
	}

	/* Ensures provided vector length is not greater than max length. If vector
	is larger, vector is scaled down to max length.*/
	template <typename T> void LimitLength(sf::Vector2<T>& vector, float fMaxLength) {
		float length = Length(vector);
		if (length <= fMaxLength)
			return;
		vector.x = (T)((float)(vector.x) * fMaxLength / length);
		vector.y = (T)((float)(vector.y) * fMaxLength / length);
	}

	/* Returns a vector in same direction as provided vector, and length not
	greater than max length. If vector is under max length,
	is larger, vector is scaled down to max length.*/
	template <typename T> sf::Vector2<T>& LengthLimited(sf::Vector2<T>& vector, float fMaxLength) {
		float length = Length(vector);
		if (length <= fMaxLength)
			return vector;
		return (vector / (T)length) * (T)fMaxLength;
	}

	/* Scales vector to provided length. Vector must be non-zero */
	template <typename T> sf::Vector2<T> ScaleVector(const sf::Vector2<T>& vector, float fLength) {
		float length = Length(vector);
		assert(length > 0.0f);
		return sf::Vector2<T>( 
			(vector.x / length) * fLength, 
			(vector.y / length) * fLength
		);
	}

	template <typename T> void Rotate(sf::Vector2<T>& vector, float fRadian) {
		float cosVal = std::cos(fRadian);
		float sinVal = std::sin(fRadian);
		vector = sf::Vector2<T>(
			(T)(vector.x * cosVal - vector.y * sinVal),
			(T)(vector.y * cosVal + vector.x * sinVal)
		);
	}

	template <typename T> void Rotated(const sf::Vector2<T>& vector, float fRadian) {
		float cosVal = std::cos(fRadian);
		float sinVal = std::sin(fRadian);
		return sf::Vector2<T>(vector.x * cosVal - vector.y * sinVal,
			vector.y * cosVal + vector.y * sinVal);
	}

	template <typename T> void RotateDegree(sf::Vector2<T>& vector, float fDegree) {
		float cosVal = std::cos(fDegree);
		float sinVal = std::sin(fDegree);
		vector = sf::Vector2<T>(vector.x * cosVal - vector.y * sinVal,
			vector.y * cosVal + vector.y * sinVal);
	}

	template <typename T> void RotatedDegree(const sf::Vector2<T>& vector, float fDegree) {
		float cosVal = std::cos(fRadian);
		float sinVal = std::sin(fRadian);
		return sf::Vector2<T>(vector.x * cosVal - vector.y * sinVal,
			vector.y * cosVal + vector.y * sinVal);
	}

	template <typename T> bool IsNull(const sf::Vector2<T>& vector) {
		return vector.x == 0.0f && vector.y == 0.0f;
	}
};

namespace gui {
	/**
		Insert checkbox into panel at position, return size. 
		Callback function is called on status change with given args
	**/
	template<typename OnCheckFn_t, typename... Args>
	void PanelMakeToggle(tgui::Theme* theme, bool state, tgui::Panel* panel, tgui::Layout2d position, sf::Vector2f& _out_size, tgui::Layout size, std::string& text, const std::string& sID, OnCheckFn_t fn, Args... args)
	{
		tgui::CheckBox::Ptr checkbox = theme->load("Checkbox");
		checkbox->setSize(size, size);
		checkbox->setPosition(panelLayout);
		checkbox->setText(text);
		checkbox->setTextSize(size);
		checkbox->connect("checked unchecked", fn, args...);
		if (state)
			checkbox->check();
		panel->add(checkbox, sID);
		_out_size = checkbox->getSize();
	}

	/**
		Insert checkbox into panel at given position, advance position by size. 
		Callback function is called on status change with given args
	**/
	template<typename OnCheckFn_t, typename... Args>
	void PanelMakeToggleVertical(tgui::Theme* theme, tgui::Panel::Ptr panel, bool state, sf::Vector2f& _in_out_position, tgui::Layout size, std::string& text, const std::string& sID, OnCheckFn_t fn, Args... args)
	{
		tgui::CheckBox::Ptr checkbox = theme->load("Checkbox");
		checkbox->setSize(size, size);
		checkbox->setPosition(panelLayout);
		checkbox->setText(text);
		checkbox->setTextSize(size);
		checkbox->connect("checked unchecked", fn, args...);
		if (state)
			checkbox->check();
		panel->add(checkbox, sID);
		_out_size = checkbox->getSize();
	}

	/*
	*/
	template<typename CallbackFn_t, typename... Args>
	void PanelMakeSliderFloatVertical(tgui::Theme* theme, const sf::Vector2f valRange, float initialValue, tgui::Panel::Ptr pPanel, tgui::Layout2d& _inout_position, const std::string& text, const std::string& sID, const std::string& sLblID, CallbackFn_t fn, Args... args) {
		tgui::Label::Ptr lbl = std::make_shared<tgui::Label>();
		lbl->setTextSize(TEXT_SIZE);
		lbl->setText(text);
		lbl->setPosition(_inout_position);

		tgui::Slider::Ptr slider = theme->load("Slider");
		slider->setMinimum(std::static_cast<int>(valRange.x / PANEL_FACTOR));
		slider->setMaximum(std::static_cast<int>(valRange.y / PANEL_FACTOR));
		slider->setValue(std::static_cast<int>(initialValue / PANEL_FACTOR));
		slider->setSize(tgui::Layout2d(SLIDER_WIDTH, TEXT_SIZE));
		slider->setPosition(tgui::bindRight(lblName), panelLayout.y);
		slider->connect("ValueChanged", fn, args...);

		tgui::Label::Ptr lblDisplay = std::make_shared<tgui::Label>();
		lblDisplay->setTextSize(TEXT_SIZE);
		lblDisplay->setText(std::to_string(initialValue));
		lblDisplay->setTextColor(SLIDER_TEXT_COLOR);
		lblDisplay->setPosition(tgui::bindRight(slider), panelLayout.y);

		// Increase layout cursor by height (Max of three widgets), and padding
		panelLayout.y += tgui::bindMax(tgui::bindMax(tgui::bindHeight(lblName),
			tgui::bindHeight(slider)), tgui::bindHeight(lblDisplay)) + tgui::Layout(MENU_PADDING);

		insertPanel->add(lblName);
		insertPanel->add(slider, szSliderID);
		insertPanel->add(lblDisplay, szDisplayLabelID);
	}


	/*
	void InsertTab(const tgui::Tab::Ptr tabs){}

	template<typename... Args>
	void InsertTab(const tgui::Tab::Ptr tabs, tgui::Theme::Ptr theme, tgui::Layout2d size, const std::string& s, const Args&... args) {
		tgui::Panel::Ptr panel = theme->load("Panel");
		panel->setSize(tgui::bindSize(tabs));
		tabs->add(s);
		InsertTab(tabs, args...);
	}

	template<typename MemberCallbackFn_t, typename... Args>
	void MakeTabset(tgui::Theme::Ptr pTheme, tgui::Gui* pGui, tgui::Layout2d position, tgui::Layout2d size, const std::string& sTabsID, tgui::Gui* pGui, MemberCallbackFn_t pClassCallback, void* pClassThis, Args... args) {
		tgui::Tab::Ptr tabs = pTheme->load("Tab");
		tabs->setPosition(position);
		tabs->connect("TabSelected", pClassCallback, pClassThis);
		m_pGUI->add(tabs, "TabsMenu");

		tgui::Panel::Ptr backgroundPanel = theme->load("Panel");
		backgroundPanel->setSize(400, 300);
		backgroundPanel->setPosition(tabs->getPosition().x, tabs->getPosition().y + tabs->getTabHeight());
		m_pGUI->add(backgroundPanel, "BackgroundPanel");
	}*/
};
namespace util {

	/* Allocate memory for a new string, copy its contents, and return the pointer. Must free() the pointer. */
	char* strcpyalloc(char* szSourceStr);
	char* strcpyalloc(char* szSourceStr, int strlen);

	/* Allocate memory for a new string, copy its contents, and return the pointer. Must delete the pointer. */
	char* strcpynew(char* szSourceStr);
	char* strcpynew(char* szSourceStr, int strlen);

	/* Returns int value of a base-10 numeric null-terminated string */
	int strtoint(char* szString);

	/* Returns int value of a base-10 numeric string, for a given length */
	int strtoint(char* szString, int len);

	bool StrInt(std::string str, int& _out_int);

	/* Returns lowercase char if c is an uppercase char, otherwise returns c */
	char CharToLower(char c);

	// Character check for letter/number/either
	bool CharAlphaNum(char c);
	bool CharAlpha(char c);
	bool CharNum(char c);

	/* Convert std::string to lowercase */
	std::string& StrLower(std::string& string);

	// Split std::string into vector of substrings between provided delimeter
	// Removes whitespace made up of multiple delimeters
	std::vector<std::string> StrSplit(std::string& str, char delim);

	// Splits a string first for quoted substrings (e.g. 'hi'), then by spacing
	// e.g. Given input: "echo "This is an echo'd command" RED", result is:
	// {"echo", "This is an echo'd command", "RED"}
	bool StrSplitQuoteDelim(std::string& str, std::vector<std::string>& _out_result, char delim=' ', char* szQuotationChars="'\"");

	// Check if string is only letters, only numbers, or only letter/numbers
	bool StrAlphaNumeric(std::string& str);
	bool StrAlpha(std::string& str);
	bool StrNum(std::string& str);

	bool StrContains(std::string strHaystack, std::string& strNeedle);

	/* Case-insensitive std::string equality check */
	bool StrCaseCmp(std::string& s1, std::string& s2);

	/* Remove whitespace (Space, tabs, and newline chars)*/
	void LTrim(std::string& str);
	void RTrim(std::string& str);
	void Trim(std::string& str);

	// Returns true if file exists and can be read
	bool FileExists(std::string& sFilePath);
}

#endif