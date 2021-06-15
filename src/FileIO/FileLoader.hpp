// Copied from Kawin's eDNA-Server

#pragma once
#include <KPFoundation.hpp>
#include <SD.h>

class FileLoader {
public:
	bool createDirectoryIfNeeded(const char * dir) {
		File folder = SD.open(dir, FILE_READ);
		if (folder) {
			if (folder.isDirectory()) {
				folder.close();
				return true;
			}

			// raise(Error("FileLoader: path already exists but "
			// "not directory please remove the file"));
		}

		// folder doesn't exist
		// Serial.print("FileLoader: directory doesn't exist. Creating...");
		SD.mkdir(dir);
		// Serial.print(success ? "success" : "failed");
		// return success;
		return false;
	}
};