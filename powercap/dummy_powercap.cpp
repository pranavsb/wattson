#include "powercap_interface.cpp"

class DummyPowercap : public PowercapInterface {
private:
    float current_power_ = 11.0;

public:
    bool SetPowercap(float powercapInWatts) override {
        current_power_ = powercapInWatts;
        return true;
    }

    float GetPowerReading() override {
        // low power reading, well within budget
        return current_power_ - 4.0;
    }
};
