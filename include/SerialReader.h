#ifndef SERIAL_READER_h
#define SERIAL_READER_h

#include <HardwareSerial.h>
#define COMMAND_SIZE 25u
#define SERIAL_START '<'
#define SERIAL_END '>'

class SerialReader {
    private:
        // records the last COMMAND_SIZE characters of every valid message, including beginning and ending characters
        char buffer[COMMAND_SIZE] = "";
        
        unsigned int buffer_p = 0;
        unsigned int buffer_s = 0;
        unsigned int buffer_size = 0;
        bool buffer_v = false;

    public:
        SerialReader() {
            clearBuffer();
        }
        
        void clearBuffer() {
            buffer_p = 0;
            buffer_s = 0;
            buffer_size = 0;
        }

        void nextBuffer() {
            buffer_size++;
            buffer_p++;
            if (buffer_p >= COMMAND_SIZE) buffer_p = 0;
            if (buffer_p == buffer_s) buffer_s++;
            if (buffer_s >= COMMAND_SIZE) buffer_s = 0;
        }

        void rollBuffer() {
            for (size_t i = 0; i < buffer_s; i++) {
                char t = buffer[0];
                for (size_t j = 0; j < COMMAND_SIZE - 1; j++) buffer[j] = buffer[j + 1];
                buffer[COMMAND_SIZE - 1] = t;
            }
            ++buffer_size;
            if (buffer_size > COMMAND_SIZE) buffer_size = COMMAND_SIZE;
        }

        void readSerial(void(readCommand)(unsigned int, char[])) {
            while (Serial.available() > 0) {
                char c = Serial.read();
                if (c == SERIAL_START && !buffer_v) {
                    buffer_v = true;
                    buffer[buffer_p] = c;
                    nextBuffer();
                } else if (c == SERIAL_END && buffer_v) {
                    buffer_v = false;
                    buffer[buffer_p] = c;
                    rollBuffer();
                    readCommand(buffer_size, buffer);
                    clearBuffer();
                } else if (buffer_v) {
                    buffer[buffer_p] = c;
                    nextBuffer();
                }
            }
        }
};

#endif