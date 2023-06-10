#include<iostream>

enum class Alignment {LEFT, RIGHT, CENTER, JUSTIFY};
enum class FontStyle {TIMES_NEW_ROMAN, ARIAL, CALIBRI};
enum class ParagraphStyle {NORMAL, HEADING};

std::ostream& operator << (std::ostream& s, const Alignment alignment) {
	switch (alignment) {
	case Alignment::CENTER:
		return s << "CENTER";
	case Alignment::JUSTIFY:
		return s << "JUSTIFY";
	case Alignment::RIGHT:
		return s << "RIGHT";
	default:
		return s << "LEFT";
	}
}

std::ostream& operator << (std::ostream& s, const FontStyle font) {
	switch (font) {
	case FontStyle::ARIAL:
		return s << "Arial";
	case FontStyle::CALIBRI:
		return s << "Calibri";
	default:
		return s << "Times New Roman";
	}
}

class ParagraphBuilder;

class Paragraph {
public:
	static Paragraph* getParagraph(const std::string& text, ParagraphStyle style);
	void setFontStyle(FontStyle fontStyle) { this->fontStyle = fontStyle; }
	void setFontSize(unsigned int fontSize) { this->fontSize = fontSize; }
	void setAlignment(Alignment alignment) { this->alignment = alignment; }
	void setIndentFirstLine(bool indenFirstLine) { this->indentFirstLine = indenFirstLine; }
	friend std::ostream& operator << (std::ostream & s, const Paragraph & p) {
		s << p.text << "\tconfig: ";
		s << "{fontSize: " << p.fontSize << ", fontStyle: " << p.fontStyle << ", alignment: " \
			<< p.alignment << ", firstLineIndented: " << p.indentFirstLine << "}";
		return s;
	}
private:
	Paragraph(std::string text) : text(text) {}

public:
	std::string text;
private:
	FontStyle fontStyle;
	Alignment alignment;
	unsigned int fontSize;
	bool indentFirstLine;

	friend class ParagraphBuilder;
};

class ParagraphBuilder {
protected:
	Paragraph* paragraph;

	static const unsigned int DEFAULT_FONT_SIZE = 12;
	static const FontStyle DEFAULT_FONT_STYLE = FontStyle::TIMES_NEW_ROMAN;
	static const Alignment DEFAULT_ALIGNMENT = Alignment::LEFT;
	static const bool DEFAULT_FIRST_LINE_INDENT = false;

public:
	ParagraphBuilder(const std::string& text) : paragraph(new Paragraph(text)) {
		paragraph->alignment = DEFAULT_ALIGNMENT;
		paragraph->fontSize = DEFAULT_FONT_SIZE;
		paragraph->fontStyle = DEFAULT_FONT_STYLE;
		paragraph->indentFirstLine = DEFAULT_FIRST_LINE_INDENT;
	}

	ParagraphBuilder* fontStyle(const FontStyle fontStyle) {
		paragraph->fontStyle = fontStyle;
		return this;
	}
	ParagraphBuilder* fontSize(const unsigned int fontSize) {
		paragraph->fontSize = fontSize;
		return this;
	}
	ParagraphBuilder* alignment(const Alignment alignment) {
		paragraph->alignment = alignment;
		return this;
	}
	ParagraphBuilder* indentFirstLine(const bool indentFirstLine) {
		paragraph->indentFirstLine = indentFirstLine;
		return this;
	}

	static ParagraphBuilder* builder(const std::string& text) {
		return new ParagraphBuilder(text);
	}

	Paragraph* build() const {
		return paragraph;
	}
};

class HeadingParagraphBuilder : public ParagraphBuilder {
public:
	HeadingParagraphBuilder(const std::string& text): ParagraphBuilder(text) {
		paragraph->setAlignment(Alignment::CENTER);
		paragraph->setFontSize(18);
		paragraph->setIndentFirstLine(false);
		paragraph->setFontStyle(FontStyle::TIMES_NEW_ROMAN);
	}
	static HeadingParagraphBuilder* builder(const std::string& text) {
		return new HeadingParagraphBuilder(text);
	}
};

class NormalParagraphBuilder : public ParagraphBuilder {
public:
	NormalParagraphBuilder(const std::string& text) : ParagraphBuilder(text) {
		paragraph->setAlignment(Alignment::JUSTIFY);
		paragraph->setFontSize(12);
		paragraph->setIndentFirstLine(true);
		paragraph->setFontStyle(FontStyle::TIMES_NEW_ROMAN);
	}
	static NormalParagraphBuilder* builder(const std::string& text) {
		return new NormalParagraphBuilder(text);
	}
};

Paragraph* Paragraph::getParagraph(const std::string& text, ParagraphStyle style) {
	switch (style) {
	case ParagraphStyle::NORMAL:
		return NormalParagraphBuilder::builder(text)->build();
	case ParagraphStyle::HEADING:
		return HeadingParagraphBuilder::builder(text)->build();
	}
}

int main() {
	Paragraph* p4 = ParagraphBuilder("tekst paragrafa").
		fontSize(32)->fontStyle(FontStyle::CALIBRI)->build();
	Paragraph* p = ParagraphBuilder::builder("tekst")->fontSize(14)->build();
	Paragraph* p2 = HeadingParagraphBuilder::builder("t")->alignment(Alignment::LEFT)->fontSize(12)->build();
	Paragraph* p3 = Paragraph::getParagraph("naslov", ParagraphStyle::HEADING);

	std::cout << *p << std::endl;
	std::cout << *p2 << std::endl;
	std::cout << *p3 << std::endl;
	return 0;
}