#include <filesystem_manager.h>
#include <LittleFS.h>

bool mountFilesystem() 
{
    if (!LittleFS.begin()) 
    {
        Serial.println("Failed to mount LittleFS");
        return false;
    } 
    else 
    {
        Serial.println("LittleFS mounted successfully");
        return true;
    }
}

void listFilesInRootDirectory() 
{
    Dir dir = LittleFS.openDir("/");
    while (dir.next())
    {
        Serial.printf("File: %s, Size: %d bytes\n", dir.fileName().c_str(), dir.fileSize());
    }
}

void listDir(const char * dirname, uint8_t levels) 
{
    Serial.printf("Listing directory: %s\n", dirname);

    File root = LittleFS.open(dirname, "r");
    if (!root) 
    {
        Serial.println("Failed to open directory");
        return;
    }
    if (!root.isDirectory()) 
    {
        Serial.println("Not a directory");
        return;
    }

    File file = root.openNextFile();
    while (file) 
    {
        if (file.isDirectory()) 
        {
            Serial.print("  DIR : ");
            Serial.println(file.name());
            if (levels) 
            {
                listDir(file.name(), levels - 1);
            }
        } 
        else 
        {
            Serial.print("  FILE: ");
            Serial.print(file.name());
            Serial.print("\tSIZE: ");
            Serial.println(file.size());
        }
        file = root.openNextFile();
    }
}