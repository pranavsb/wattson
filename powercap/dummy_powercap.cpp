#include "powercap_interface.cpp"

/**
 * Implementation of PowercapInterface used for testing.
 * Does NOT actually enforce the powercap on the machine.
 *
 * Just stores a variable to act as the powercap set. (Note that this is not stored on disk so not immune to crashes.)
 *
 * In the future, we plan to support generating virtual workloads with random distributions similar to power
 * characteristics of real machines, to run simulations with multiple machines.
 */
class DummyPowercap : public PowercapInterface {
private:
    float current_power_ = 15.12;

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
