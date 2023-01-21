class PowercapInterface {
	public:
		virtual bool setPowercap(float powercapInWatts) = 0;
		virtual float getPowerReading() = 0;
};
