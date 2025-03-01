#ifndef TSA_INPUT_PROVIDER_H
#define TSA_INPUT_PROVIDER_H

#include <vector>
#include <memory>
#include <string>
#include <stdexcept>
#include <functional>

// TS packet is typically 188 bytes
constexpr size_t TS_PACKET_SIZE = 188;
// Default buffer size (in packets)
constexpr size_t DEFAULT_BUFFER_SIZE = 512; // ~96KB

// Forward declaration for TSPacket callback function type
using TSPacketCallback = std::function<bool(const std::vector<uint8_t>&)>;

/**
 * @brief Abstract base class for all MPEG-2 TS input providers
 * 
 * This class defines the interface for components that read TS packets
 * from various sources and forward them through the processing pipeline.
 */
class InputProvider {
public:
    virtual ~InputProvider() = default;
    
    // Set the callback to be invoked for each packet
    virtual void setPacketCallback(TSPacketCallback callback) = 0;
    
    // Open the input source and start processing
    virtual bool open() = 0;
    
    // Close the input source and stop processing
    virtual void close() = 0;
    
    // Process the next batch of packets (returns number of packets processed)
    virtual size_t processNextBatch() = 0;
    
    // Check if more packets are available
    virtual bool hasMorePackets() const = 0;
    
    // Reset to beginning if supported
    virtual bool reset() = 0;
    
    // Get a description of the source
    virtual std::string getSourceDescription() const = 0;
};

#endif // TSA_INPUT_PROVIDER_H