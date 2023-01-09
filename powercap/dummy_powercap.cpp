class DummyPowercap:PowercapInterface {
	public:
		static float currentPower = 11.0;

		bool setPowercap(float powercapInWatts) {
			currentPower = powercapInWatts;
			return true;
		}

		float getPowerReading() {
			return currentPower;
		}
}
