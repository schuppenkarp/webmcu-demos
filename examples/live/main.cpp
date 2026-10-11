#include "board.hpp"

enum class Mode : unsigned { Idle, Measuring, Ready };
struct Reading { int temperature; unsigned samples; };
class Sensor {
public:
    Reading reading;
    Mode mode;
    unsigned values[4];
};
volatile Sensor sensor = {{21, 0}, Mode::Idle, {1, 2, 3, 4}};
volatile unsigned ticks = 0;

extern "C" int main() {
    board::init();
    while (true) {
        ++ticks;
        sensor.mode = Mode::Measuring;
        sensor.reading.samples = ticks;
        sensor.reading.temperature = 20 + ticks % 10;
        sensor.values[ticks % 4] = ticks;
        sensor.mode = Mode::Ready;
        board::led((ticks & 1U) != 0);
        board::delay_ms(100);
    }
}
