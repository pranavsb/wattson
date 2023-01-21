#include "powercap_interface.cpp"

class DummyPowercap:PowercapInterface {
	public:
		float currentPower = 11.0;

		bool setPowercap(float powercapInWatts) override {
			currentPower = powercapInWatts;
			return true;
		}

		float getPowerReading() override {
            // low power reading, well within budget
			return currentPower - 4.0;
		}
};
