/**
 * Interface used by Agent to enforce powercap. Can use RAPL or PUPiL or even a dummy powercap under the hood.
 */
class PowercapInterface {
public:
    /**
     * Given the powercap in watts, set it on the node.
     *
     * @return Return true only on success.
     */
    virtual bool SetPowercap(float powercap_in_watts) = 0;

    /**
     * Use RAPL or any other framework to read the current power usage on the node.
     *
     * @return Current power usage in watts
     */
    virtual float GetPowerReading() = 0;

    virtual ~PowercapInterface() = default;
};
