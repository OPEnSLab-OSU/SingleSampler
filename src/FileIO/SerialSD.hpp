#include <KPFoundation.hpp>
#include <FileIO/CSVWriter.hpp>
#include <String>
class SerialSD : public KPComponent {
public:
	CSVWriter csvw{"serial.csv"};

	void print(const char * str) {
		std::string temp[1] = {str};
		csvw.writeStrings(temp, 1);
		Serial.print(str);
	}
	void print(float num) {
		std::string temp[1] = {std::to_string(num)};
		csvw.writeStrings(temp, 1);
		Serial.print(num);
	}
	void println(const char * str) {
		std::string temp[1] = {str};
		csvw.writeStrings(temp, 1);
		Serial.println(str);
	}
	void println(float num) {
		std::string temp[1] = {std::to_string(num)};
		csvw.writeStrings(temp, 1);
		Serial.println(num);
	}
} SSD;